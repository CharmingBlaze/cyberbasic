// Everything the UI can do: run/debug, project files, panels and workspaces. Menus, the command
// palette, toolbar buttons and keyboard shortcuts all call these.
import type { DockviewApi } from "dockview-react";
import { compile, loadCompiler } from "./compiler";
import { monaco, registerLanguage } from "./language";
import { EXAMPLE_ASSETS, TEMPLATES, exportGame, fromTemplate, fsSupported, gamePage, openFolder, saveProject, type Template } from "./project";
import { get, set, type Project, type WorkspaceId } from "./store";

export const refs: {
  editor?: monaco.editor.IStandaloneCodeEditor;
  dock?: DockviewApi;
  frame?: HTMLIFrameElement;
} = {};

let model: monaco.editor.ITextModel;
let loading = false; // true while replacing the model text, so it isn't marked dirty
export const getModel = () => model;

// ---------------------------------------------------------------- startup

const SESSION = "cyberbasic.session";
/** True on the very first launch, so the start page opens once the layout exists. */
export let firstLaunch = false;

export async function init() {
  await loadCompiler();
  registerLanguage();
  const saved = JSON.parse(localStorage.getItem(SESSION) || "null") as Pick<Project, "name" | "source" | "fileName"> | null;
  const project: Project = saved ? { ...saved, assets: EXAMPLE_ASSETS } : fromTemplate(TEMPLATES[0]);
  model = monaco.editor.createModel(project.source, "cyberbasic", monaco.Uri.parse("inmemory://project/main.cb"));
  model.updateOptions({ tabSize: 4, insertSpaces: true });
  model.onDidChangeContent(() => {
    syncBreakpoints();
    if (loading) return;
    set((s) => ({ project: { ...s.project, source: model.getValue() }, dirty: true }));
    clearRuntimeErrors();
    scheduleCheck();
    persist();
  });
  firstLaunch = !saved;
  window.addEventListener("beforeunload", (e) => {
    if (get().dirty && get().project.dir) e.preventDefault(); // unsaved changes to a folder on disk
  });
  set({ project, ready: true });
  check();
}

const persist = () => {
  const { name, source, fileName } = get().project;
  localStorage.setItem(SESSION, JSON.stringify({ name, source, fileName }));
};

function loadProject(project: Project) {
  stop();
  loading = true;
  model.setValue(project.source);
  loading = false;
  set({ project, dirty: false, breakpoints: [], logs: [] });
  bpDecorations = model.deltaDecorations(bpDecorations, []);
  check();
  persist();
  refs.dock?.getPanel("editor")?.api.setTitle(project.fileName);
  openPanel("editor");
  refs.editor?.focus();
}

// ---------------------------------------------------------------- diagnostics

let checkTimer = 0;
const scheduleCheck = () => {
  clearTimeout(checkTimer);
  checkTimer = window.setTimeout(check, 200);
};

export function check() {
  const r = compile(get().project.source, false);
  const runtime = get().diagnostics.filter((d) => d.kind === "runtime");
  set({ diagnostics: r.ok ? runtime : [{ line: r.line, col: r.col, message: r.message, kind: "compile" }, ...runtime] });
  monaco.editor.setModelMarkers(
    model,
    "cyberbasic",
    get().diagnostics.map((d) => ({
      severity: monaco.MarkerSeverity.Error,
      message: d.kind === "runtime" ? `Runtime error: ${d.message}` : d.message,
      startLineNumber: d.line,
      startColumn: d.col,
      endLineNumber: d.line,
      endColumn: d.kind === "runtime" ? model.getLineMaxColumn(d.line) : model.getWordAtPosition({ lineNumber: d.line, column: d.col })?.endColumn ?? d.col + 1,
    })),
  );
}

const clearRuntimeErrors = () => {
  if (get().diagnostics.some((d) => d.kind === "runtime")) set((s) => ({ diagnostics: s.diagnostics.filter((d) => d.kind !== "runtime") }));
};

// ---------------------------------------------------------------- feedback

let nextId = 0;
export function toast(text: string, kind: "info" | "error" | "success" = "info") {
  const id = ++nextId;
  set((s) => ({ toasts: [...s.toasts.slice(-3), { id, text, kind }] }));
  setTimeout(() => set((s) => ({ toasts: s.toasts.filter((t) => t.id !== id) })), kind === "error" ? 6000 : 3000);
}

export function log(kind: "log" | "error" | "info", text: string, line?: number) {
  const time = new Date().toLocaleTimeString([], { hour12: false });
  set((s) => ({ logs: [...s.logs.slice(-999), { id: ++nextId, kind, text, line, time }] }));
}

export function revealLine(line: number, col = 1, focus = true) {
  const e = refs.editor;
  if (!e) return;
  e.revealLineInCenterIfOutsideViewport(line);
  e.setPosition({ lineNumber: line, column: col });
  if (focus) e.focus();
}

// ---------------------------------------------------------------- run & debug

const send = (m: object) => refs.frame?.contentWindow?.postMessage({ cyberbasic: true, ...m }, "*");

export function run() {
  const { run: r, project } = get();
  if (r.state === "paused") return cont();
  const result = compile(project.source, true); // debug build: breakpoints always work in the IDE
  if (!result.ok) {
    check();
    toast(`Can't run: line ${result.line}: ${result.message}`, "error");
    revealLine(result.line, result.col);
    return;
  }
  const page = gamePage(result.js, { assets: project.assets, breakpoints: get().breakpoints });
  clearRuntimeErrors();
  check();
  set((s) => ({ run: { state: "starting", session: s.run.session + 1, page } }));
  log("info", `▶ Running ${project.name}`);
  openPanel("game", { focus: false });
}

export function stop() {
  if (get().run.state === "stopped" && !get().run.page) return;
  set((s) => ({ run: { state: "stopped", session: s.run.session, page: "" } }));
}

export function restart() {
  stop();
  run();
}

export const pause = () => get().run.state === "running" && send({ type: "pause" });
export const cont = () => get().run.state === "paused" && send({ type: "continue" });
export function step() {
  const s = get().run.state;
  if (s === "paused") send({ type: "step" });
  else if (s === "running") pause();
  else if (s === "stopped" || s === "error") {
    // Start paused on the first statement.
    pauseOnStart = true;
    run();
    if (get().run.state !== "starting") pauseOnStart = false; // didn't compile
  }
}
let pauseOnStart = false;

export function onGameMessage(m: { type: string; [k: string]: unknown }) {
  switch (m.type) {
    case "started":
      set((s) => ({ run: { ...s.run, state: "running" } }));
      if (pauseOnStart) {
        pauseOnStart = false;
        send({ type: "pause" });
      }
      break;
    case "log":
      log("log", String(m.text), m.line as number);
      break;
    case "fps":
      set((s) => ({ run: { ...s.run, fps: m.fps as number } }));
      break;
    case "paused":
      set((s) => ({ run: { ...s.run, state: "paused", line: m.line as number, fn: m.fn as string, vars: m.vars as Record<string, string> } }));
      revealLine(m.line as number, 1, false);
      openPanel("debugger"); // bring its tab to the front so the variables are in view
      break;
    case "resumed":
      set((s) => ({ run: { ...s.run, state: "running", line: undefined, fn: undefined, vars: undefined } }));
      break;
    case "error": {
      const line = m.line as number, message = String(m.message);
      set((s) => ({
        run: { ...s.run, state: "error", fps: undefined },
        diagnostics: [...s.diagnostics.filter((d) => d.kind !== "runtime"), { line, col: 1, message, kind: "runtime" }],
      }));
      check();
      log("error", message, line);
      toast(`Runtime error on line ${line}: ${message}`, "error");
      revealLine(line);
      break;
    }
    case "ended":
      set((s) => ({ run: { ...s.run, state: "stopped", fps: undefined } }));
      log("info", "■ Program ended");
      break;
    case "key": // an IDE shortcut pressed while the game had focus
      window.dispatchEvent(new KeyboardEvent("keydown", { key: String(m.key), ctrlKey: !!m.ctrl, shiftKey: !!m.shift }));
      break;
  }
}

// ---------------------------------------------------------------- breakpoints
// Breakpoints live as model decorations so they follow their line while you edit.

let bpDecorations: string[] = [];

export function toggleBreakpoint(line = refs.editor?.getPosition()?.lineNumber) {
  if (!line) return;
  const lines = new Set(get().breakpoints);
  lines.has(line) ? lines.delete(line) : lines.add(line);
  setBreakpoints([...lines]);
}

export function setBreakpoints(lines: number[]) {
  bpDecorations = model.deltaDecorations(
    bpDecorations,
    lines.map((l) => ({
      range: new monaco.Range(l, 1, l, 1),
      options: {
        glyphMarginClassName: "bp-glyph",
        glyphMarginHoverMessage: { value: "Breakpoint (F9 or click to remove)" },
        stickiness: monaco.editor.TrackedRangeStickiness.NeverGrowsWhenTypingAtEdges,
      },
    })),
  );
  syncBreakpoints();
}

function syncBreakpoints() {
  const lines = [...new Set(bpDecorations.map((id) => model.getDecorationRange(id)?.startLineNumber).filter((l): l is number => !!l))].sort((a, b) => a - b);
  if (lines.join() === get().breakpoints.join()) return;
  set({ breakpoints: lines });
  send({ type: "breakpoints", lines });
}

// ---------------------------------------------------------------- project commands

export function newProject() {
  openPanel("start");
}

export function createFromTemplate(t: Template) {
  if (get().dirty && get().project.dir && !confirm(`Discard unsaved changes to ${get().project.name}?`)) return;
  loadProject(fromTemplate(t));
  toast(`New project from “${t.name}”`, "success");
}

export async function openProject() {
  if (!fsSupported) return toast("Opening folders needs Chrome or Edge.", "error");
  try {
    const p = await openFolder();
    loadProject(p);
    toast(`Opened ${p.name}`, "success");
  } catch (e) {
    if ((e as Error).name !== "AbortError") toast((e as Error).message, "error");
  }
}

export async function save(saveAs = false) {
  try {
    const p = await saveProject(get().project, saveAs);
    set({ project: p, dirty: false });
    persist();
    toast(p.dir ? `Saved to ${p.dir.name}/${p.fileName}` : `Downloaded ${p.fileName}`, "success");
    if (get().run.state === "running" || get().run.state === "paused") restart(); // save = see it
  } catch (e) {
    if ((e as Error).name !== "AbortError") toast(`Save failed: ${(e as Error).message}`, "error");
  }
}

export async function exportWeb() {
  const r = compile(get().project.source, false);
  if (!r.ok) {
    toast(`Fix line ${r.line} before exporting: ${r.message}`, "error");
    return revealLine(r.line, r.col);
  }
  const file = await exportGame(get().project, r.js);
  toast(`Exported ${file}: one file, runs in any browser`, "success");
}

export function insertSnippet(snippet: string) {
  const e = refs.editor;
  if (!e) return;
  e.focus();
  (e.getContribution("snippetController2") as unknown as { insert(s: string): void }).insert(snippet);
}

export const editorAction = (id: string) => {
  refs.editor?.focus();
  refs.editor?.trigger("menu", id, null);
};

// ---------------------------------------------------------------- panels & workspaces

export const PANELS = {
  start: "Start",
  editor: "main.cb",
  game: "Game",
  console: "Console",
  problems: "Problems",
  debugger: "Debugger",
  reference: "Reference",
  project: "Project",
} as const;
export type PanelId = keyof typeof PANELS;

export const WORKSPACES: { id: WorkspaceId; name: string; hint: string }[] = [
  { id: "game", name: "Game", hint: "Code, game and console side by side" },
  { id: "code", name: "Code", hint: "Big editor with the reference; game floats" },
  { id: "debug", name: "Debug", hint: "Game, variables and console around the editor" },
  { id: "learn", name: "Learn", hint: "Command reference next to your code" },
];

type Spot = { ref: PanelId; dir: "left" | "right" | "above" | "below" | "within"; size?: number };

// Sizes asked for while building a preset; reapplied at the end because later splits resize earlier ones.
let sizes: [PanelId, { width?: number; height?: number }][] = [];

function add(id: PanelId, spot?: Spot | "float", float?: { x: number; y: number; width: number; height: number }) {
  const api = refs.dock!;
  const title = id === "editor" ? get().project.fileName : PANELS[id];
  if (spot === "float") return api.addPanel({ id, component: id, title, floating: float ?? true });
  const horizontal = spot?.dir === "left" || spot?.dir === "right";
  const sized = horizontal ? { initialWidth: spot.size } : { initialHeight: spot?.size };
  if (spot?.size) sizes.push([id, horizontal ? { width: spot.size } : { height: spot.size }]);
  return api.addPanel({
    id,
    component: id,
    title,
    ...(spot ? { position: { referencePanel: spot.ref, direction: spot.dir } } : {}),
    ...(spot?.size ? sized : {}),
    inactive: spot?.dir === "within",
  });
}

const PRESETS: Record<WorkspaceId, () => void> = {
  game() {
    add("editor");
    add("game", { ref: "editor", dir: "right", size: Math.round(window.innerWidth * 0.42) });
    add("project", { ref: "editor", dir: "left", size: 240 });
    add("console", { ref: "editor", dir: "below", size: 190 });
    add("problems", { ref: "console", dir: "within" });
    add("debugger", { ref: "console", dir: "within" });
  },
  code() {
    add("editor");
    add("reference", { ref: "editor", dir: "right", size: 340 });
    add("project", { ref: "editor", dir: "left", size: 240 });
    add("console", { ref: "editor", dir: "below", size: 170 });
    add("problems", { ref: "console", dir: "within" });
    // Float the game in the editor's bottom-right corner, above the console, clear of the code.
    const w = 440, h = 290;
    add("game", "float", { x: window.innerWidth - 340 - w - 30, y: window.innerHeight - 26 - 170 - h - 60, width: w, height: h });
  },
  debug() {
    add("editor");
    add("game", { ref: "editor", dir: "right", size: Math.round(window.innerWidth * 0.4) });
    add("debugger", { ref: "game", dir: "below", size: Math.round(window.innerHeight * 0.4) });
    add("console", { ref: "editor", dir: "below", size: 200 });
    add("problems", { ref: "console", dir: "within" });
  },
  learn() {
    add("editor");
    add("reference", { ref: "editor", dir: "left", size: 380 });
    add("game", { ref: "editor", dir: "right", size: Math.round(window.innerWidth * 0.34) });
    add("console", { ref: "editor", dir: "below", size: 160 });
  },
};

const layoutKey = (ws: WorkspaceId) => `cyberbasic.layout.${ws}`;
let switching = false;

export function applyWorkspace(ws: WorkspaceId, reset = false) {
  const api = refs.dock;
  if (!api) return;
  switching = true;
  if (reset) localStorage.removeItem(layoutKey(ws));
  api.clear();
  const saved = localStorage.getItem(layoutKey(ws));
  try {
    if (!saved) throw new Error("no saved layout");
    const data = JSON.parse(saved);
    delete data.popoutGroups; // never reopen windows on their own
    api.fromJSON(data);
    if (!api.getPanel("editor")) throw new Error("layout lost the editor");
  } catch {
    api.clear();
    sizes = [];
    PRESETS[ws]();
    for (const [id, size] of sizes) api.getPanel(id)?.group.api.setSize(size);
  }
  set({ workspace: ws });
  localStorage.setItem("cyberbasic.workspace", ws);
  switching = false;
  api.getPanel("editor")?.api.setActive();
}

/** Remembers your changes to the current workspace's layout. */
export function watchLayout() {
  let t = 0;
  refs.dock!.onDidLayoutChange(() => {
    if (switching) return;
    clearTimeout(t);
    t = window.setTimeout(() => localStorage.setItem(layoutKey(get().workspace), JSON.stringify(refs.dock!.toJSON())), 400);
  });
}

/** Shows a panel: focuses it if it's in the layout, otherwise opens it as a floating window. */
export function openPanel(id: PanelId, { focus = true } = {}) {
  const api = refs.dock;
  if (!api) return;
  const panel = api.getPanel(id);
  if (panel) {
    if (focus || !panel.group.api.isVisible) panel.api.setActive();
    return;
  }
  if (id === "start" || id === "editor") {
    add(id, api.getPanel("editor") ? { ref: "editor", dir: "within" } : undefined);
    api.getPanel(id)?.api.setActive();
    return;
  }
  const w = id === "game" ? 520 : 380, h = id === "game" ? 340 : 320;
  add(id, "float", { x: Math.max(20, window.innerWidth / 2 - w / 2), y: Math.max(60, window.innerHeight / 2 - h / 2 - 40), width: w, height: h });
}

export function togglePanel(id: PanelId) {
  const panel = refs.dock?.getPanel(id);
  if (panel) panel.api.close();
  else openPanel(id);
}

export function openAsset(path: string) {
  const api = refs.dock!;
  const id = `asset:${path}`;
  const existing = api.getPanel(id);
  if (existing) return existing.api.setActive();
  api.addPanel({ id, component: "asset", title: path.split("/").pop(), params: { path }, floating: { x: window.innerWidth / 2 - 180, y: 120, width: 360, height: 320 } });
}

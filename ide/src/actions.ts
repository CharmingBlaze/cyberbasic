// Everything the UI can do: run/debug, project files, panels and workspaces. Menus, the command
// palette, toolbar buttons and keyboard shortcuts all call these.
import type { DockviewApi } from "dockview-react";
import { compile, loadCompiler } from "./compiler";
import { monaco, registerLanguage } from "./language";
import { EXAMPLE_ASSETS, TEMPLATES, exportGame, fromTemplate, fsSupported, gamePage, openFolder, readFolder, saveProject, writeFile, type Template } from "./project";
import { addRecent, removeRecent, type Recent } from "./recent";
import { format } from "./symbols";
import { get, set, type Breakpoint, type Project, type WorkspaceId } from "./store";

export const refs: {
  editor?: monaco.editor.IStandaloneCodeEditor;
  dock?: DockviewApi;
  frame?: HTMLIFrameElement;
} = {};

// One Monaco model per source file; editors show them. Model URIs are inmemory://project/<file>.
const models = new Map<string, monaco.editor.ITextModel>();
let loading = false; // true while replacing model text, so it isn't marked dirty
export const getModel = (file = get().activeFile) => models.get(file) ?? models.get(get().project.fileName)!;
export const fileOf = (model: monaco.editor.ITextModel) => decodeURIComponent(model.uri.path.slice(1));
const uriOf = (file: string) => monaco.Uri.parse(`inmemory://project/${encodeURIComponent(file)}`);

// ---------------------------------------------------------------- startup

const SESSION = "cyberbasic.session";
/** True on the very first launch, so the start page opens once the layout exists. */
export let firstLaunch = false;

export async function init() {
  await loadCompiler();
  registerLanguage();
  monaco.editor.registerEditorOpener({
    // Go to definition / references in another file: open that file's editor tab.
    openCodeEditor(_source, resource, selection) {
      const line = selection && "startLineNumber" in selection ? selection.startLineNumber : selection?.lineNumber ?? 1;
      revealLine(line, 1, true, fileOf(monaco.editor.getModel(resource)!));
      return true;
    },
  });
  const saved = JSON.parse(localStorage.getItem(SESSION) || "null") as (Partial<Project> & { source?: string }) | null;
  let project: Project = fromTemplate(TEMPLATES[0]);
  // Older sessions stored a single `source`; a session without its main file is ignored.
  const files = saved?.fileName ? saved.files ?? { [saved.fileName]: saved.source ?? "" } : undefined;
  if (saved?.fileName && files?.[saved.fileName] !== undefined) {
    project = { name: saved.name ?? "Untitled", files, fileName: saved.fileName, assets: EXAMPLE_ASSETS };
  }
  firstLaunch = !saved;
  window.addEventListener("beforeunload", (e) => {
    if (get().dirty && get().project.dir) e.preventDefault(); // unsaved changes to a folder on disk
  });
  createModels(project);
  set({ project, activeFile: project.fileName, ready: true });
  check();
}

function createModels(project: Project) {
  for (const m of models.values()) m.dispose();
  models.clear();
  for (const [file, src] of Object.entries(project.files)) addModel(file, src);
}

function addModel(file: string, src: string) {
  const model = monaco.editor.createModel(src, "cyberbasic", uriOf(file));
  model.updateOptions({ tabSize: 4, insertSpaces: true });
  model.onDidChangeContent(() => {
    syncBreakpoints();
    if (loading) return;
    set((s) => ({ project: { ...s.project, files: { ...s.project.files, [file]: model.getValue() } }, dirty: true }));
    clearRuntimeErrors();
    scheduleCheck();
    persist();
  });
  models.set(file, model);
  return model;
}

const persist = () => {
  const { name, files, fileName } = get().project;
  localStorage.setItem(SESSION, JSON.stringify({ name, files, fileName }));
};

function loadProject(project: Project) {
  stop();
  for (const p of [...(refs.dock?.panels ?? [])]) if (p.id.startsWith("file:")) p.api.close();
  const mainEditor = editors.get(get().project.fileName);
  loading = true;
  createModels(project);
  mainEditor?.setModel(models.get(project.fileName)!); // the "editor" panel always shows the main file
  loading = false;
  set({ project, activeFile: project.fileName, dirty: false, breakpoints: [], logs: [], diagnostics: [] });
  bpDecorations.clear();
  check();
  persist();
  refs.dock?.getPanel("editor")?.api.setTitle(project.fileName);
  openFile(project.fileName);
}

// ---------------------------------------------------------------- files

/** Shows a file in its editor tab: the main file in "editor", others in "file:<name>" tabs. */
export function openFile(file: string) {
  const api = refs.dock;
  if (!api || !models.has(file)) return;
  const id = file === get().project.fileName ? "editor" : `file:${file}`;
  const panel = api.getPanel(id);
  if (panel) return panel.api.setActive();
  const anchor = api.getPanel("editor") ?? api.panels.find((p) => p.id.startsWith("file:"));
  api.addPanel({
    id,
    component: "editor",
    title: file,
    params: { file },
    ...(anchor ? { position: { referencePanel: anchor.id, direction: "within" as const } } : {}),
  });
}

const validName = (name: string) => /^[\w-]+\.cb$/i.test(name);

/** Adds a file and an `Include` for it at the top of the main file, then opens it. */
export function newFile() {
  const input = prompt("Name of the new file (for example enemies.cb):", "");
  if (!input) return;
  const file = input.trim().toLowerCase().endsWith(".cb") ? input.trim() : `${input.trim()}.cb`;
  if (!validName(file)) return toast("Use letters, numbers, - or _ and end with .cb", "error");
  if (Object.keys(get().project.files).some((f) => f.toLowerCase() === file.toLowerCase())) return toast(`${file} already exists`, "error");
  addModel(file, `' ${file}\n`);
  set((s) => ({ project: { ...s.project, files: { ...s.project.files, [file]: `' ${file}\n` } }, dirty: true }));
  // Put the Include after the main file's leading comment lines.
  const main = getModel(get().project.fileName);
  const lines = main.getLinesContent();
  let at = 0;
  while (at < lines.length && /^\s*'/.test(lines[at])) at++;
  main.pushEditOperations([], [{ range: new monaco.Range(at + 1, 1, at + 1, 1), text: `Include "${file}"\n` }], () => null);
  persist();
  openFile(file);
  toast(`Created ${file} and added Include "${file}" to ${get().project.fileName}`, "success");
}

export async function deleteFile(file: string) {
  const { project } = get();
  if (file === project.fileName) return toast("The main file can't be deleted.", "error");
  if (!confirm(`Delete ${file}${project.dir ? ` from ${project.dir.name} on disk` : ""}? This can't be undone.`)) return;
  if (project.dir) {
    try {
      await project.dir.removeEntry(file);
    } catch (e) {
      return toast(`Couldn't delete ${file}: ${(e as Error).message}`, "error");
    }
  }
  refs.dock?.getPanel(`file:${file}`)?.api.close();
  models.get(file)?.dispose();
  models.delete(file);
  // Remove `Include "<file>"` lines that would now fail.
  const include = new RegExp(`^\\s*Include\\s+"${file.replace(/[.*+?^${}()|[\]\\]/g, "\\$&")}"\\s*$`, "i");
  for (const m of models.values()) {
    const edits = m
      .getLinesContent()
      .map((l, i) => (include.test(l) ? { range: new monaco.Range(i + 1, 1, i + 2, 1), text: "" } : null))
      .filter((e): e is { range: monaco.Range; text: string } => !!e);
    if (edits.length) m.pushEditOperations([], edits, () => null);
  }
  bpDecorations.delete(file);
  set((s) => ({ project: { ...s.project, files: Object.fromEntries(Object.entries(s.project.files).filter(([f]) => f !== file)) }, activeFile: s.activeFile === file ? s.project.fileName : s.activeFile, breakpoints: s.breakpoints.filter((b) => b.file !== file) }));
  persist();
  check();
  toast(`Deleted ${file} and its Include line`, "info");
}

// ---------------------------------------------------------------- diagnostics

let checkTimer = 0;
const scheduleCheck = () => {
  clearTimeout(checkTimer);
  checkTimer = window.setTimeout(check, 200);
};

export function check() {
  const { project } = get();
  const r = compile(project.files, project.fileName, false);
  const runtime = get().diagnostics.filter((d) => d.kind === "runtime");
  set({ diagnostics: r.ok ? runtime : [{ file: r.file, line: r.line, col: r.col, message: r.message, kind: "compile" }, ...runtime] });
  for (const [file, model] of models) {
    monaco.editor.setModelMarkers(
      model,
      "cyberbasic",
      get()
        .diagnostics.filter((d) => d.file === file && d.line <= model.getLineCount())
        .map((d) => ({
          severity: monaco.MarkerSeverity.Error,
          message: d.kind === "runtime" ? `Runtime error: ${d.message}` : d.message,
          startLineNumber: d.line,
          startColumn: d.col,
          endLineNumber: d.line,
          endColumn: d.kind === "runtime" ? model.getLineMaxColumn(d.line) : model.getWordAtPosition({ lineNumber: d.line, column: d.col })?.endColumn ?? d.col + 1,
        })),
    );
  }
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

export function log(kind: "log" | "error" | "info", text: string, at?: { file: string; line: number }) {
  const time = new Date().toLocaleTimeString([], { hour12: false });
  set((s) => ({ logs: [...s.logs.slice(-999), { id: ++nextId, kind, text, file: at?.file, line: at?.line, time }] }));
}

/** Editors by file, registered by EditorPanel; and where to jump once a just-opened editor exists. */
export const editors = new Map<string, monaco.editor.IStandaloneCodeEditor>();
export const pendingReveal = new Map<string, { line: number; col: number; focus: boolean }>();

export function revealLine(line: number, col = 1, focus = true, file = get().activeFile) {
  openFile(file);
  const e = editors.get(file);
  if (!e) return void pendingReveal.set(file, { line, col, focus }); // its editor mounts on the next render
  e.revealLineInCenterIfOutsideViewport(line);
  e.setPosition({ lineNumber: line, column: col });
  if (focus) e.focus();
}

// ---------------------------------------------------------------- run & debug

const send = (m: object) => refs.frame?.contentWindow?.postMessage({ cyberbasic: true, ...m }, "*");

/** Runtime locations are file index * 1e6 + line, indexed by the running build's file order. */
const decode = (loc: number) => ({ file: get().run.files[Math.floor(loc / 1e6)] ?? get().project.fileName, line: loc % 1e6 });
const encode = (bps: Breakpoint[], files = get().run.files) =>
  bps.filter((b) => files.includes(b.file)).map((b) => files.indexOf(b.file) * 1e6 + b.line);

export function run() {
  const { run: r, project } = get();
  if (r.state === "paused") return cont();
  const result = compile(project.files, project.fileName, true); // debug build: breakpoints always work in the IDE
  if (!result.ok) {
    check();
    toast(`Can't run: ${result.file} line ${result.line}: ${result.message}`, "error");
    revealLine(result.line, result.col, true, result.file);
    return;
  }
  const page = gamePage(result.js, { assets: project.assets, breakpoints: encode(get().breakpoints, result.files) });
  clearRuntimeErrors();
  check();
  set((s) => ({ run: { state: "starting", session: s.run.session + 1, page, files: result.files } }));
  log("info", `▶ Running ${project.name}`);
  openPanel("game", { focus: false });
}

export function stop() {
  if (get().run.state === "stopped" && !get().run.page) return;
  set((s) => ({ run: { state: "stopped", session: s.run.session, page: "", files: s.run.files } }));
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
      log("log", String(m.text), decode(m.line as number));
      break;
    case "fps":
      set((s) => ({ run: { ...s.run, fps: m.fps as number } }));
      break;
    case "paused": {
      const at = decode(m.line as number);
      set((s) => ({ run: { ...s.run, state: "paused", ...at, fn: m.fn as string, vars: m.vars as Record<string, string> } }));
      revealLine(at.line, 1, false, at.file);
      openPanel("debugger"); // bring its tab to the front so the variables are in view
      break;
    }
    case "resumed":
      set((s) => ({ run: { ...s.run, state: "running", file: undefined, line: undefined, fn: undefined, vars: undefined } }));
      break;
    case "error": {
      const at = decode(m.line as number), message = String(m.message);
      set((s) => ({
        run: { ...s.run, state: "error", fps: undefined },
        diagnostics: [...s.diagnostics.filter((d) => d.kind !== "runtime"), { ...at, col: 1, message, kind: "runtime" }],
      }));
      check();
      log("error", message, at);
      toast(`Runtime error in ${at.file} line ${at.line}: ${message}`, "error");
      revealLine(at.line, 1, true, at.file);
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

const bpDecorations = new Map<string, string[]>();

export function toggleBreakpoint(line = refs.editor?.getPosition()?.lineNumber, file = get().activeFile) {
  if (!line) return;
  const bps = get().breakpoints;
  const on = bps.some((b) => b.file === file && b.line === line);
  setBreakpoints(on ? bps.filter((b) => !(b.file === file && b.line === line)) : [...bps, { file, line }]);
}

export function setBreakpoints(bps: Breakpoint[]) {
  for (const [file, model] of models) {
    bpDecorations.set(
      file,
      model.deltaDecorations(
        bpDecorations.get(file) ?? [],
        bps
          .filter((b) => b.file === file)
          .map((b) => ({
            range: new monaco.Range(b.line, 1, b.line, 1),
            options: {
              glyphMarginClassName: "bp-glyph",
              glyphMarginHoverMessage: { value: "Breakpoint (F9 or click to remove)" },
              stickiness: monaco.editor.TrackedRangeStickiness.NeverGrowsWhenTypingAtEdges,
            },
          })),
      ),
    );
  }
  syncBreakpoints();
}

function syncBreakpoints() {
  const bps: Breakpoint[] = [];
  for (const [file, ids] of bpDecorations) {
    const lines = new Set(ids.map((id) => models.get(file)?.getDecorationRange(id)?.startLineNumber).filter((l): l is number => !!l));
    for (const line of [...lines].sort((a, b) => a - b)) bps.push({ file, line });
  }
  const key = (l: Breakpoint[]) => l.map((b) => `${b.file}:${b.line}`).join();
  if (key(bps) === key(get().breakpoints)) return;
  set({ breakpoints: bps });
  send({ type: "breakpoints", lines: encode(bps) });
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
    if (p.dir) addRecent(p.dir);
    toast(`Opened ${p.name}`, "success");
  } catch (e) {
    if ((e as Error).name !== "AbortError") toast((e as Error).message, "error");
  }
}

export async function openRecent(r: Recent) {
  try {
    // Browsers ask again for access to a folder after a restart; this must run from a click.
    const handle = r.handle as FileSystemDirectoryHandle & { requestPermission(o: { mode: string }): Promise<PermissionState> };
    if ((await handle.requestPermission({ mode: "readwrite" })) !== "granted") return toast(`No access to ${r.name}`, "error");
    const p = await readFolder(r.handle);
    loadProject(p);
    addRecent(r.handle);
    toast(`Opened ${p.name}`, "success");
  } catch (e) {
    toast(`Couldn't open ${r.name}: ${(e as Error).message}`, "error");
    removeRecent(r.name);
  }
}

/** Adds image, sound and model files to the project's assets/ folder (on disk too, when linked). */
export async function importAssets(files: File[]) {
  const ok = files.filter((f) => /\.(png|jpe?g|gif|svg|webp|ogg|mp3|wav|gltf|glb)$/i.test(f.name));
  if (!ok.length) return toast("Drop images (png, jpg, svg), sounds (ogg, mp3, wav) or models (gltf, glb).", "error");
  const { project } = get();
  const assets = { ...project.assets };
  for (const f of ok) {
    const path = `assets/${f.name}`;
    assets[path] = URL.createObjectURL(f);
    if (project.dir) await writeFile(project.dir, path, f);
  }
  set((s) => ({ project: { ...s.project, assets } }));
  toast(`Added ${ok.map((f) => f.name).join(", ")}${project.dir ? "" : " (saved with the project)"}`, "success");
}

export function formatDocument(model = getModel()) {
  const text = model.getValue();
  const tidy = format(text, " ".repeat(model.getOptions().indentSize));
  if (tidy !== text) model.pushEditOperations([], [{ range: model.getFullModelRange(), text: tidy }], () => null);
}

/** PNG of the game as it looks now (3D view plus 2D/HUD layer). */
export function screenshot() {
  const doc = refs.frame?.contentDocument;
  const canvases = [...(doc?.querySelectorAll("canvas") ?? [])];
  if (!canvases.length) return toast("Run the game first, then take a screenshot.", "error");
  const out = document.createElement("canvas");
  out.width = canvases[0].width;
  out.height = canvases[0].height;
  const ctx = out.getContext("2d")!;
  for (const c of canvases) ctx.drawImage(c, 0, 0);
  const a = document.createElement("a");
  a.href = out.toDataURL("image/png");
  a.download = `${get().project.name.replace(/[^\w-]+/g, "-").toLowerCase()}-${Date.now()}.png`;
  a.click();
  toast("Screenshot saved", "success");
}

export const fullscreen = () => refs.frame?.requestFullscreen().catch(() => toast("Fullscreen isn't available here.", "error"));

export async function save(saveAs = false) {
  if (get().settings.formatOnSave) for (const m of models.values()) formatDocument(m);
  try {
    const p = await saveProject(get().project, saveAs);
    set({ project: p, dirty: false });
    if (p.dir) addRecent(p.dir);
    persist();
    const count = Object.keys(p.files).length;
    toast(p.dir ? `Saved ${count} file${count > 1 ? "s" : ""} to ${p.dir.name}` : `Downloaded ${count} file${count > 1 ? "s" : ""}`, "success");
    if (get().run.state === "running" || get().run.state === "paused") restart(); // save = see it
  } catch (e) {
    if ((e as Error).name !== "AbortError") toast(`Save failed: ${(e as Error).message}`, "error");
  }
}

export async function exportWeb() {
  const { project } = get();
  const r = compile(project.files, project.fileName, false);
  if (!r.ok) {
    toast(`Fix ${r.file} line ${r.line} before exporting: ${r.message}`, "error");
    return revealLine(r.line, r.col, true, r.file);
  }
  const file = await exportGame(project, r.js);
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
  outline: "Outline",
  settings: "Settings",
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
    add("outline", { ref: "project", dir: "within" });
    add("console", { ref: "editor", dir: "below", size: 190 });
    add("problems", { ref: "console", dir: "within" });
    add("debugger", { ref: "console", dir: "within" });
  },
  code() {
    add("editor");
    add("reference", { ref: "editor", dir: "right", size: 340 });
    add("project", { ref: "editor", dir: "left", size: 240 });
    add("outline", { ref: "project", dir: "below", size: Math.round(window.innerHeight * 0.45) });
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
    // Tabs for files that aren't in this project (saved with another one) would be empty.
    for (const p of [...api.panels]) if (p.id.startsWith("file:") && !models.has(p.id.slice(5))) p.api.close();
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
  // Tool windows open in the bottom-right corner, cascading so they don't cover each other or the code.
  const w = id === "game" ? 520 : 380, h = id === "game" ? 340 : 320;
  const n = api.groups.filter((g) => g.api.location.type === "floating").length;
  add(id, "float", { x: Math.max(20, window.innerWidth - w - 40 - n * 36), y: Math.max(60, window.innerHeight - h - 80 - n * 36), width: w, height: h });
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

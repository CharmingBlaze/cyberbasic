import { useEffect, useMemo, useRef, useState } from "react";
import { DockviewReact, themeAbyssSpaced, type IDockviewHeaderActionsProps } from "dockview-react";
import * as A from "./actions";
import { manifest } from "./compiler";
import { Icon, type IconName } from "./icons";
import { appWindow, guardClose, isDesktop } from "./desktop";
import { COMPONENTS, Tab } from "./panels";
import { TEMPLATES, fsSupported } from "./project";
import { get, set, useStore, type WorkspaceId } from "./store";

// ------------------------------------------------------------------ commands (menus + palette)

interface Cmd {
  id: string;
  label: string;
  group?: string;
  keys?: string;
  icon?: IconName;
  run: () => void;
  enabled?: () => boolean;
  checked?: () => boolean;
}

const running = () => ["running", "paused", "starting"].includes(get().run.state);
const has = (id: A.PanelId) => () => !!A.refs.dock?.getPanel(id);

const COMMANDS: Cmd[] = [
  { id: "new", group: "File", label: "New Project…", keys: "Ctrl+N", icon: "plus", run: A.newProject },
  { id: "newfile", group: "File", label: "New File…", icon: "code", run: A.newFile },
  { id: "open", group: "File", label: "Open Folder…", keys: "Ctrl+O", icon: "folder", run: A.openProject, enabled: () => fsSupported },
  { id: "save", group: "File", label: "Save", keys: "Ctrl+S", icon: "save", run: () => A.save() },
  { id: "saveas", group: "File", label: "Save As…", keys: "Ctrl+Shift+S", run: () => A.save(true) },
  { id: "import", group: "File", label: "Import Assets…", icon: "image", run: () => pickFiles() },
  { id: "export", group: "File", label: "Export Web Game…", keys: "Ctrl+Shift+E", icon: "download", run: A.exportWeb },
  { id: "settings", group: "File", label: "Settings", keys: "Ctrl+,", icon: "keyboard", run: () => A.openPanel("settings") },

  { id: "undo", group: "Edit", label: "Undo", keys: "Ctrl+Z", run: () => A.editorAction("undo") },
  { id: "redo", group: "Edit", label: "Redo", keys: "Ctrl+Y", run: () => A.editorAction("redo") },
  { id: "find", group: "Edit", label: "Find", keys: "Ctrl+F", icon: "search", run: () => A.editorAction("actions.find") },
  { id: "replace", group: "Edit", label: "Replace", keys: "Ctrl+H", run: () => A.editorAction("editor.action.startFindReplaceAction") },
  { id: "comment", group: "Edit", label: "Toggle Comment", keys: "Ctrl+/", run: () => A.editorAction("editor.action.commentLine") },
  { id: "format", group: "Edit", label: "Format Document", keys: "Shift+Alt+F", icon: "layout", run: A.formatDocument },
  { id: "definition", group: "Edit", label: "Go to Definition", keys: "F12", run: () => A.editorAction("editor.action.revealDefinition") },
  { id: "references", group: "Edit", label: "Find All References", keys: "Shift+F12", run: () => A.editorAction("editor.action.goToReferences") },
  { id: "rename", group: "Edit", label: "Rename Symbol", keys: "F2", run: () => A.editorAction("editor.action.rename") },
  { id: "symbol", group: "Edit", label: "Go to Function or Type…", keys: "Ctrl+Shift+O", run: () => A.editorAction("editor.action.quickOutline") },
  { id: "palette", group: "Edit", label: "Command Palette…", keys: "Ctrl+Shift+P", icon: "search", run: () => set({ palette: true }) },

  ...(["project", "outline", "game", "console", "problems", "debugger", "reference", "settings", "start"] as A.PanelId[]).map((id) => ({
    id: "view." + id,
    group: "View",
    label: A.PANELS[id],
    run: () => A.togglePanel(id),
    checked: has(id),
  })),

  { id: "run", group: "Run", label: "Run", keys: "F5", icon: "play", run: A.run, enabled: () => get().run.state !== "paused" },
  { id: "stop", group: "Run", label: "Stop", keys: "Shift+F5", icon: "stop", run: A.stop, enabled: running },
  { id: "restart", group: "Run", label: "Restart", keys: "Ctrl+Shift+F5", icon: "restart", run: A.restart },

  { id: "continue", group: "Debug", label: "Continue", keys: "F5", icon: "play", run: A.cont, enabled: () => get().run.state === "paused" },
  { id: "pause", group: "Debug", label: "Pause", keys: "F6", icon: "pause", run: A.pause, enabled: () => get().run.state === "running" },
  { id: "step", group: "Debug", label: "Step", keys: "F10", icon: "step", run: A.step },
  { id: "bp", group: "Debug", label: "Toggle Breakpoint", keys: "F9", icon: "bug", run: () => A.toggleBreakpoint() },
  { id: "bpclear", group: "Debug", label: "Remove All Breakpoints", run: () => A.setBreakpoints([]), enabled: () => get().breakpoints.length > 0 },

  ...A.WORKSPACES.map((w, i) => ({
    id: "ws." + w.id,
    group: "Workspace",
    label: w.name,
    keys: `Ctrl+${i + 1}`,
    icon: "layout" as IconName,
    run: () => A.applyWorkspace(w.id),
    checked: () => get().workspace === w.id,
  })),
  { id: "ws.reset", group: "Workspace", label: "Reset Workspace Layout", run: () => A.applyWorkspace(get().workspace, true) },

  { id: "help.ref", group: "Help", label: "Command Reference", icon: "book", run: () => A.openPanel("reference") },
  { id: "help.start", group: "Help", label: "Start Page", icon: "sparkle", run: () => A.openPanel("start") },
  { id: "help.keys", group: "Help", label: "Keyboard Shortcuts", icon: "keyboard", run: () => set({ palette: true }) },
  { id: "help.repo", group: "Help", label: "CyberBasic on GitHub", icon: "external", run: () => window.open("https://github.com/CharmingBlaze/cyberbasic", "_blank", "noopener") },
];
const MENUS = ["File", "Edit", "View", "Run", "Debug", "Workspace", "Help"];
const SEPARATE_BEFORE = new Set(["import", "settings", "find", "format", "definition", "palette", "restart", "bp", "ws.reset", "help.repo", "view.start"]);

/** Opens the system file picker for assets (menus and the palette can't render an <input>). */
function pickFiles() {
  const input = document.createElement("input");
  input.type = "file";
  input.multiple = true;
  input.accept = ".png,.jpg,.jpeg,.gif,.svg,.webp,.ogg,.mp3,.wav,.gltf,.glb";
  input.onchange = () => A.importAssets([...(input.files ?? [])]);
  input.click();
}

// ------------------------------------------------------------------ app

export default function App() {
  const ready = useStore((s) => s.ready);
  const [error, setError] = useState("");

  useEffect(() => {
    A.init().catch((e) => setError(String(e.message ?? e)));
  }, []);

  useShortcuts();
  useWindowTitle();
  useEffect(guardClose, []);

  if (error) return <div className="boot error"><Icon name="alert" size={28} /><p>{error}</p></div>;
  if (!ready) return <div className="boot"><img src="/icon.svg" width={48} alt="" /><p>Starting CyberBasic…</p></div>;

  return (
    <div className="app">
      <TitleBar />
      <div className="dock">
        <DockviewReact
          className="cb-dock"
          theme={themeAbyssSpaced}
          components={COMPONENTS}
          defaultTabComponent={Tab}
          rightHeaderActionsComponent={GroupActions}
          watermarkComponent={Watermark}
          floatingGroupBounds="boundedWithinViewport"
          floatingGroupDragHandle="tabbar"
          popoutUrl="/popout.html"
          onReady={(e) => {
            A.refs.dock = e.api;
            A.applyWorkspace((localStorage.getItem("cyberbasic.workspace") as WorkspaceId) || "game");
            A.watchLayout();
            if (A.firstLaunch) A.openPanel("start");
          }}
        />
      </div>
      <StatusBar />
      <Palette />
      <Toasts />
    </div>
  );
}

function Watermark() {
  return (
    <div className="watermark">
      <img src="/icon.svg" width={40} alt="" />
      <p>Drop a panel here, or pick one from <b>View</b>.</p>
    </div>
  );
}

/** Buttons on every panel group: float / dock, pop out into a window, maximize. */
function GroupActions({ group, containerApi, location }: IDockviewHeaderActionsProps) {
  const [max, setMax] = useState(group.api.isMaximized());
  useEffect(() => {
    const d = containerApi.onDidMaximizedGroupChange(() => setMax(group.api.isMaximized()));
    return () => d.dispose();
  }, [group, containerApi]);
  const where = location?.type ?? "grid";
  return (
    <div className="group-actions">
      {where === "grid" && (
        <button title="Float (popup panel)" onClick={() => containerApi.addFloatingGroup(group, { width: 460, height: 320, x: 80, y: 80 })}>
          <Icon name="float" size={14} />
        </button>
      )}
      {where === "floating" && (
        <button title="Dock back into the layout" onClick={() => group.api.moveTo({ position: "right" })}>
          <Icon name="dock" size={14} />
        </button>
      )}
      {where !== "popout" && !isDesktop && (
        <button title="Pop out into its own window" onClick={() => containerApi.addPopoutGroup(group, { popoutUrl: "/popout.html" })}>
          <Icon name="popout" size={14} />
        </button>
      )}
      {where === "grid" && (
        <button title={max ? "Restore" : "Maximize"} onClick={() => (max ? group.api.exitMaximized() : group.api.maximize())}>
          <Icon name={max ? "minimize" : "maximize"} size={14} />
        </button>
      )}
    </div>
  );
}

// ------------------------------------------------------------------ title bar

function TitleBar() {
  const project = useStore((s) => s.project.name);
  const dirty = useStore((s) => s.dirty);
  return (
    // data-tauri-drag-region: in the desktop app, dragging empty title bar space moves the window
    // and double-clicking it maximizes. Buttons inside keep working as buttons.
    <header className={"titlebar" + (isDesktop ? " desktop" : "")} data-tauri-drag-region>
      <img className="logo" src="/icon.svg" alt="CyberBasic" width={20} height={20} data-tauri-drag-region />
      <MenuBar />
      <div className="title-center" data-tauri-drag-region>
        <span className="project-title">
          {project}
          {dirty && <span className="dirty" title="Unsaved changes" />}
        </span>
      </div>
      <RunControls />
      <WorkspaceTabs />
      <button className="palette-btn" onClick={() => set({ palette: true })} title="Command palette (Ctrl+Shift+P)">
        <Icon name="search" size={14} />
        <span>Search commands</span>
        <kbd>Ctrl Shift P</kbd>
      </button>
      {isDesktop && <WindowControls />}
    </header>
  );
}

/** Minimize / maximize / close for the frameless desktop window, styled like Windows' own. */
function WindowControls() {
  const [max, setMax] = useState(false);
  useEffect(() => {
    const w = appWindow();
    const update = () => w.isMaximized().then(setMax);
    update();
    const off = w.onResized(update);
    return () => void off.then((f) => f());
  }, []);
  return (
    <div className="window-controls">
      <button title="Minimize" onClick={() => appWindow().minimize()}>
        <svg width="10" height="10" viewBox="0 0 10 10" aria-hidden="true"><path d="M0 5h10" stroke="currentColor" /></svg>
      </button>
      <button title={max ? "Restore" : "Maximize"} onClick={() => appWindow().toggleMaximize()}>
        <svg width="10" height="10" viewBox="0 0 10 10" aria-hidden="true" fill="none" stroke="currentColor">
          {max ? <path d="M2.5 .5h7v7M.5 2.5h7v7h-7z" /> : <path d="M.5 .5h9v9h-9z" />}
        </svg>
      </button>
      <button className="close" title="Close" onClick={() => appWindow().close()}>
        <svg width="10" height="10" viewBox="0 0 10 10" aria-hidden="true"><path d="M0 0l10 10M10 0L0 10" stroke="currentColor" /></svg>
      </button>
    </div>
  );
}

function MenuBar() {
  const [open, setOpen] = useState<string | null>(null);
  const bar = useRef<HTMLDivElement>(null);
  useStore((s) => s.run.state); // re-render so enabled states stay current
  useStore((s) => s.workspace);
  useEffect(() => {
    if (!open) return;
    const close = (e: MouseEvent | KeyboardEvent) => {
      if (e instanceof KeyboardEvent ? e.key === "Escape" : !bar.current?.contains(e.target as Node)) setOpen(null);
    };
    window.addEventListener("mousedown", close);
    window.addEventListener("keydown", close);
    return () => {
      window.removeEventListener("mousedown", close);
      window.removeEventListener("keydown", close);
    };
  }, [open]);
  return (
    <nav className="menubar" ref={bar}>
      {MENUS.map((m) => (
        <div key={m} className="menu">
          <button
            className={"menu-label" + (open === m ? " on" : "")}
            onMouseDown={(e) => {
              e.preventDefault();
              setOpen(open === m ? null : m);
            }}
            onMouseEnter={() => open && setOpen(m)}
          >
            {m}
          </button>
          {open === m && (
            <div className="menu-pop" role="menu">
              {COMMANDS.filter((c) => c.group === m).map((c) => (
                <div key={c.id}>
                  {SEPARATE_BEFORE.has(c.id) && <hr />}
                  <button
                    role="menuitem"
                    className="menu-item"
                    disabled={c.enabled ? !c.enabled() : false}
                    onClick={() => {
                      setOpen(null);
                      c.run();
                    }}
                  >
                    <span className="menu-check">{c.checked?.() && <Icon name="check" size={13} />}</span>
                    <span className="grow">{c.label}</span>
                    {c.keys && <span className="menu-keys">{c.keys}</span>}
                  </button>
                </div>
              ))}
            </div>
          )}
        </div>
      ))}
    </nav>
  );
}

/** One main button whose job follows the state: Run, Restart while running, Continue while paused. */
function RunControls() {
  const state = useStore((s) => s.run.state);
  const active = state === "running" || state === "paused" || state === "starting";
  const main =
    state === "paused"
      ? { label: "Continue", icon: "play" as const, cls: " resume", title: "Continue (F5)", run: A.cont }
      : active
        ? { label: "Restart", icon: "restart" as const, cls: " live", title: "Restart with your latest code (Ctrl+Shift+F5)", run: A.restart }
        : { label: "Run", icon: "play" as const, cls: "", title: "Run (F5)", run: A.run };
  return (
    <div className="run-controls">
      <button className={"run-btn" + main.cls} onClick={main.run} title={main.title}>
        <Icon name={main.icon} size={14} />
        <span>{main.label}</span>
      </button>
      <button className="ctl" disabled={state !== "running"} onClick={A.pause} title="Pause (F6)"><Icon name="pause" size={15} /></button>
      <button className="ctl" onClick={A.step} title={active ? "Step (F10)" : "Start paused on the first line (F10)"}><Icon name="step" size={15} /></button>
      <button className="ctl stop" disabled={!active} onClick={A.stop} title="Stop (Shift+F5)"><Icon name="stop" size={13} /></button>
    </div>
  );
}

function useWindowTitle() {
  const name = useStore((s) => s.project.name);
  const dirty = useStore((s) => s.dirty);
  useEffect(() => {
    document.title = `${dirty ? "● " : ""}${name} — CyberBasic`;
  }, [name, dirty]);
}

function WorkspaceTabs() {
  const ws = useStore((s) => s.workspace);
  return (
    <div className="seg workspaces" role="tablist" aria-label="Workspaces">
      {A.WORKSPACES.map((w, i) => (
        <button key={w.id} role="tab" aria-selected={ws === w.id} className={ws === w.id ? "on" : ""} title={`${w.hint} (Ctrl+${i + 1})`} onClick={() => A.applyWorkspace(w.id)}>
          {w.name}
        </button>
      ))}
    </div>
  );
}

// ------------------------------------------------------------------ status bar

const STATE_LABEL = { stopped: "Ready", starting: "Starting…", running: "Running", paused: "Paused", error: "Stopped with an error" };

function StatusBar() {
  const run = useStore((s) => s.run);
  const diags = useStore((s) => s.diagnostics.length);
  const cursor = useStore((s) => s.cursor);
  const bps = useStore((s) => s.breakpoints.length);
  const ws = useStore((s) => s.workspace);
  return (
    <footer className={"statusbar " + run.state}>
      <span className="status-item state"><span className="state-dot" />{STATE_LABEL[run.state]}{run.state === "paused" && ` in ${run.file} on line ${run.line}`}</span>
      <button className={"status-item" + (diags ? " bad" : "")} onClick={() => A.openPanel("problems")}>
        <Icon name={diags ? "alert" : "check"} size={13} /> {diags ? `${diags} problem` : "No problems"}
      </button>
      {bps > 0 && <button className="status-item" onClick={() => A.openPanel("debugger")}><span className="bp-dot small" /> {bps} breakpoint{bps > 1 ? "s" : ""}</button>}
      <span className="grow" />
      {run.fps !== undefined && run.state === "running" && <span className="status-item">{run.fps} fps</span>}
      <span className="status-item">Ln {cursor.line}, Col {cursor.col}</span>
      <span className="status-item">CyberBasic</span>
      <span className="status-item">{A.WORKSPACES.find((w) => w.id === ws)?.name} workspace</span>
    </footer>
  );
}

// ------------------------------------------------------------------ command palette

function Palette() {
  const open = useStore((s) => s.palette);
  const [q, setQ] = useState("");
  const [i, setI] = useState(0);
  const items = useMemo(() => {
    const all: Cmd[] = [
      ...COMMANDS,
      ...TEMPLATES.map((t) => ({ id: "tpl." + t.id, group: "New", label: `New ${t.kind} project: ${t.name}`, icon: "plus" as IconName, run: () => A.createFromTemplate(t) })),
      ...manifest.commands.map((c) => ({
        id: "doc." + c.name,
        group: "Docs",
        label: `${c.name}: ${c.doc}`,
        icon: "book" as IconName,
        run: () => {
          set({ referenceFilter: c.name });
          A.openPanel("reference");
        },
      })),
    ];
    const words = q.toLowerCase().split(/\s+/).filter(Boolean);
    return all.filter((c) => (c.enabled ? c.enabled() : true) && words.every((w) => `${c.group} ${c.label}`.toLowerCase().includes(w))).slice(0, 60);
  }, [q, open]);
  useEffect(() => {
    setQ("");
    setI(0);
  }, [open]);
  if (!open) return null;
  const close = () => set({ palette: false });
  const pick = (c?: Cmd) => {
    close();
    c?.run();
  };
  return (
    <div className="overlay" onMouseDown={close}>
      <div className="palette" onMouseDown={(e) => e.stopPropagation()} role="dialog" aria-label="Command palette">
        <div className="palette-input">
          <Icon name="search" size={16} />
          <input
            autoFocus
            placeholder="Type a command, a template, or a CyberBasic keyword…"
            value={q}
            onChange={(e) => {
              setQ(e.target.value);
              setI(0);
            }}
            onKeyDown={(e) => {
              if (e.key === "Escape") close();
              if (e.key === "ArrowDown") setI((i + 1) % Math.max(items.length, 1));
              if (e.key === "ArrowUp") setI((i - 1 + items.length) % Math.max(items.length, 1));
              if (e.key === "Enter") pick(items[i]);
              if (["ArrowDown", "ArrowUp", "Enter"].includes(e.key)) e.preventDefault();
            }}
          />
        </div>
        <div className="palette-list">
          {items.length === 0 && <div className="empty">Nothing matches “{q}”.</div>}
          {items.map((c, n) => (
            <button key={c.id} className={"palette-item" + (n === i ? " on" : "")} onMouseEnter={() => setI(n)} onClick={() => pick(c)}>
              <Icon name={c.icon ?? "chevron"} size={15} />
              <span className="palette-group">{c.group}</span>
              <span className="grow ellipsis">{c.label}</span>
              {c.keys && <kbd>{c.keys}</kbd>}
            </button>
          ))}
        </div>
      </div>
    </div>
  );
}

function Toasts() {
  const toasts = useStore((s) => s.toasts);
  return (
    <div className="toasts" aria-live="polite">
      {toasts.map((t) => (
        <div key={t.id} className={"toast " + t.kind}>
          <Icon name={t.kind === "error" ? "alert" : t.kind === "success" ? "check" : "sparkle"} size={15} />
          <span>{t.text}</span>
        </div>
      ))}
    </div>
  );
}

// ------------------------------------------------------------------ keyboard

function useShortcuts() {
  useEffect(() => {
    const onKey = (e: KeyboardEvent) => {
      const k = [e.ctrlKey || e.metaKey ? "Ctrl" : "", e.shiftKey ? "Shift" : "", e.key.length === 1 ? e.key.toUpperCase() : e.key].filter(Boolean).join("+");
      const map: Record<string, () => void> = {
        F5: A.run,
        "Shift+F5": A.stop,
        "Ctrl+Shift+F5": A.restart,
        F6: A.pause,
        F9: () => A.toggleBreakpoint(),
        F10: A.step,
        "Ctrl+S": () => A.save(),
        "Ctrl+Shift+S": () => A.save(true),
        "Ctrl+O": A.openProject,
        "Ctrl+N": A.newProject,
        "Ctrl+Shift+E": A.exportWeb,
        "Ctrl+,": () => A.openPanel("settings"),
        "Ctrl+Shift+P": () => set({ palette: !get().palette }),
        "Ctrl+K": () => set({ palette: !get().palette }),
        ...Object.fromEntries(A.WORKSPACES.map((w, i) => [`Ctrl+${i + 1}`, () => A.applyWorkspace(w.id)])),
      };
      // Undo/redo in sheets and graphs (Monaco keeps its own history for code).
      const undo = k === "Ctrl+Z" ? false : k === "Ctrl+Y" || k === "Ctrl+Shift+Z" ? true : undefined;
      if (undo !== undefined && A.isVisual(get().activeFile) && !(document.activeElement as HTMLElement | null)?.closest(".monaco-editor")) {
        e.preventDefault();
        e.stopPropagation();
        A.undoVisual(undo);
        return;
      }
      const fn = map[k];
      if (!fn) return;
      e.preventDefault();
      e.stopPropagation();
      fn();
    };
    window.addEventListener("keydown", onKey, true); // capture: runs before Monaco handles the key
    return () => window.removeEventListener("keydown", onKey, true);
  }, []);
}

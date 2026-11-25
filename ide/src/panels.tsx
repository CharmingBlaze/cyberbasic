// Dockable panels. Each reads what it needs from the store; actions do the work.
import { useEffect, useMemo, useRef, useState } from "react";
import type { IDockviewPanelProps } from "dockview-react";
import * as A from "./actions";
import { manifest, type Command } from "./compiler";
import { Icon } from "./icons";
import { callSnippet, monaco, signature } from "./language";
import { TEMPLATES, fsSupported } from "./project";
import { get, set, useStore } from "./store";

// ------------------------------------------------------------------ editor

export function EditorPanel() {
  const host = useRef<HTMLDivElement>(null);
  const runLine = useStore((s) => (s.run.state === "paused" ? s.run.line : undefined));
  const decorations = useRef<monaco.editor.IEditorDecorationsCollection>(null);

  useEffect(() => {
    const editor = monaco.editor.create(host.current!, {
      model: A.getModel(),
      theme: "cyberbasic",
      automaticLayout: true,
      glyphMargin: true,
      fontFamily: "'Cascadia Code', 'JetBrains Mono', Consolas, monospace",
      fontLigatures: true,
      fontSize: 14,
      lineHeight: 22,
      padding: { top: 12 },
      minimap: { enabled: true, scale: 1, renderCharacters: false },
      smoothScrolling: true,
      cursorBlinking: "smooth",
      cursorSmoothCaretAnimation: "on",
      renderLineHighlight: "all",
      bracketPairColorization: { enabled: true },
      guides: { indentation: true },
      scrollBeyondLastLine: false,
      fixedOverflowWidgets: true,
      suggest: { showStatusBar: true, preview: true },
      stickyScroll: { enabled: false },
    });
    A.refs.editor = editor;
    decorations.current = editor.createDecorationsCollection();
    const hint = editor.createDecorationsCollection();
    editor.onDidChangeCursorPosition((e) => set({ cursor: { line: e.position.lineNumber, col: e.position.column } }));
    const GLYPH = monaco.editor.MouseTargetType.GUTTER_GLYPH_MARGIN;
    const LINE_NO = monaco.editor.MouseTargetType.GUTTER_LINE_NUMBERS;
    editor.onMouseDown((e) => {
      if (e.target.type === GLYPH && e.target.position) A.toggleBreakpoint(e.target.position.lineNumber);
    });
    // Faint dot under the mouse in the gutter: shows where a click adds a breakpoint.
    editor.onMouseMove((e) => {
      const l = (e.target.type === GLYPH || e.target.type === LINE_NO) && e.target.position?.lineNumber;
      hint.set(l && !get().breakpoints.includes(l) ? [{ range: new monaco.Range(l, 1, l, 1), options: { glyphMarginClassName: "bp-hint" } }] : []);
    });
    editor.onMouseLeave(() => hint.clear());
    editor.focus();
    return () => {
      editor.dispose();
      if (A.refs.editor === editor) A.refs.editor = undefined;
    };
  }, []);

  // Highlight where the debugger is paused.
  useEffect(() => {
    decorations.current?.set(
      runLine
        ? [{ range: new monaco.Range(runLine, 1, runLine, 1), options: { isWholeLine: true, className: "exec-line", glyphMarginClassName: "exec-glyph" } }]
        : [],
    );
  }, [runLine]);

  return <div className="editor-host" ref={host} />;
}

// ------------------------------------------------------------------ game

export function GamePanel() {
  const frame = useRef<HTMLIFrameElement>(null);
  const page = useStore((s) => s.run.page);
  const session = useStore((s) => s.run.session);
  const state = useStore((s) => s.run.state);
  const line = useStore((s) => s.run.line);

  useEffect(() => {
    const f = frame.current!;
    A.refs.frame = f;
    // Listen on the window that owns the iframe: it changes when the panel pops out.
    const win = f.ownerDocument.defaultView!;
    const onMessage = (e: MessageEvent) => {
      if (e.source === f.contentWindow && e.data?.cyberbasic) A.onGameMessage(e.data);
    };
    win.addEventListener("message", onMessage);
    return () => {
      win.removeEventListener("message", onMessage);
      if (A.refs.frame === f) A.refs.frame = undefined;
    };
  }, []);

  useEffect(() => {
    const f = frame.current!;
    f.srcdoc = page;
    if (page) f.onload = () => f.contentWindow?.focus();
  }, [page, session]);

  return (
    <div className="game">
      <iframe ref={frame} title="Game" className={page ? "" : "hidden"} />
      {!page && (
        <button className="game-empty" onClick={A.run}>
          <span className="game-play"><Icon name="play" size={28} /></span>
          <span>Run your game</span>
          <kbd>F5</kbd>
        </button>
      )}
      {state === "paused" && (
        <div className="game-badge">
          <Icon name="pause" size={13} /> Paused on line {line}
          <button onClick={A.cont}>Continue</button>
        </div>
      )}
    </div>
  );
}

// ------------------------------------------------------------------ console & problems

export function ConsolePanel() {
  const logs = useStore((s) => s.logs);
  const [filter, setFilter] = useState<"all" | "log" | "error">("all");
  const list = useRef<HTMLDivElement>(null);
  const shown = filter === "all" ? logs : logs.filter((l) => l.kind === filter);
  useEffect(() => {
    const el = list.current;
    if (el && el.scrollHeight - el.scrollTop - el.clientHeight < 80) el.scrollTop = el.scrollHeight; // stick to bottom
  }, [shown.length]);
  return (
    <div className="panel">
      <div className="panel-bar">
        <div className="seg small">
          {(["all", "log", "error"] as const).map((f) => (
            <button key={f} className={filter === f ? "on" : ""} onClick={() => setFilter(f)}>
              {f === "all" ? "All" : f === "log" ? "Output" : "Errors"}
            </button>
          ))}
        </div>
        <span className="grow" />
        <button className="icon-btn" title="Clear console" onClick={() => set({ logs: [] })}>
          <Icon name="trash" size={14} />
        </button>
      </div>
      <div className="log-list mono" ref={list}>
        {shown.length === 0 && <div className="empty">Output from <code>Print</code> and runtime errors appear here.</div>}
        {shown.map((l) => (
          <div key={l.id} className={"log " + l.kind}>
            <span className="log-time">{l.time}</span>
            <span className="log-text">{l.text}</span>
            {l.line && <button className="link" onClick={() => A.revealLine(l.line!)}>line {l.line}</button>}
          </div>
        ))}
      </div>
    </div>
  );
}

export function ProblemsPanel() {
  const diags = useStore((s) => s.diagnostics);
  const file = useStore((s) => s.project.fileName);
  return (
    <div className="panel">
      <div className="list">
        {diags.length === 0 && (
          <div className="empty ok"><Icon name="check" size={16} /> No problems. Your code compiles.</div>
        )}
        {diags.map((d, i) => (
          <button key={i} className="row problem" onClick={() => A.revealLine(d.line, d.col)}>
            <Icon name="alert" size={15} className="err" />
            <span className="grow">{d.kind === "runtime" ? `Runtime error: ${d.message}` : d.message}</span>
            <span className="muted mono">{file}:{d.line}:{d.col}</span>
          </button>
        ))}
      </div>
    </div>
  );
}

// ------------------------------------------------------------------ debugger

export function DebuggerPanel() {
  const run = useStore((s) => s.run);
  const bps = useStore((s) => s.breakpoints);
  const [q, setQ] = useState("");
  const paused = run.state === "paused";
  const vars = Object.entries(run.vars ?? {}).filter(([k]) => k.includes(q.toLowerCase()));
  return (
    <div className="panel">
      <div className="panel-bar">
        <button className="tool" disabled={!paused} onClick={A.cont} title="Continue (F5)"><Icon name="play" size={13} /> Continue</button>
        <button className="tool" disabled={!paused} onClick={A.step} title="Step (F10)"><Icon name="step" size={14} /> Step</button>
        <button className="tool" disabled={run.state !== "running"} onClick={A.pause} title="Pause (F6)"><Icon name="pause" size={14} /> Pause</button>
        <span className="grow" />
        {paused && <span className="where">line {run.line} · {run.fn === "main" ? "main program" : `in ${run.fn}`}</span>}
      </div>
      <div className="section-title">Variables</div>
      {paused ? (
        <>
          <input className="filter" placeholder="Filter variables" value={q} onChange={(e) => setQ(e.target.value)} />
          <div className="vars mono">
            {vars.length === 0 && <div className="empty">No variables in scope.</div>}
            {vars.map(([k, v]) => (
              <div key={k} className="var">
                <span className="var-name">{k}</span>
                <span className="var-value" title={v}>{v}</span>
              </div>
            ))}
          </div>
        </>
      ) : (
        <div className="empty">
          Click the gutter (or press <kbd>F9</kbd>) to add a breakpoint, or press <kbd>F6</kbd> while the game runs. Variables show up here when it pauses.
        </div>
      )}
      <div className="section-title">
        Breakpoints <span className="count">{bps.length}</span>
        <span className="grow" />
        {bps.length > 0 && <button className="link" onClick={() => A.setBreakpoints([])}>Remove all</button>}
      </div>
      <div className="list">
        {bps.map((l) => (
          <div key={l} className="row">
            <span className="bp-dot" />
            <button className="link grow left" onClick={() => A.revealLine(l)}>Line {l}</button>
            <span className="muted mono line-preview">{A.getModel().getLineContent(l).trim().slice(0, 40)}</span>
            <button className="icon-btn" title="Remove" onClick={() => A.toggleBreakpoint(l)}><Icon name="close" size={12} /></button>
          </div>
        ))}
      </div>
    </div>
  );
}

// ------------------------------------------------------------------ reference

export function ReferencePanel() {
  const q = useStore((s) => s.referenceFilter);
  const groups = useMemo(() => {
    const ql = q.toLowerCase();
    const hit = (c: Command) => !ql || c.name.toLowerCase().includes(ql) || c.doc.toLowerCase().includes(ql) || c.category.toLowerCase().includes(ql);
    const map = new Map<string, Command[]>();
    for (const c of manifest.commands.filter(hit)) map.set(c.category, [...(map.get(c.category) ?? []), c]);
    return [...map];
  }, [q]);
  return (
    <div className="panel">
      <div className="panel-bar">
        <div className="search">
          <Icon name="search" size={14} />
          <input placeholder={`Search ${manifest.commands.length} commands`} value={q} onChange={(e) => set({ referenceFilter: e.target.value })} />
        </div>
      </div>
      <div className="ref-list">
        {groups.length === 0 && <div className="empty">No command matches “{q}”.</div>}
        {groups.map(([cat, cmds]) => (
          <section key={cat}>
            <h4>{cat}</h4>
            {cmds.map((c) => (
              <button key={c.name} className="ref" onClick={() => A.insertSnippet(callSnippet(c))} title="Insert at cursor">
                <code>{signature(c)}</code>
                <span>{c.doc}</span>
                <span className="tags">
                  {c.returns && <em>returns</em>}
                  {c.wait && <em>waits</em>}
                </span>
              </button>
            ))}
          </section>
        ))}
      </div>
    </div>
  );
}

// ------------------------------------------------------------------ project

export function ProjectPanel() {
  const project = useStore((s) => s.project);
  const dirty = useStore((s) => s.dirty);
  const assets = Object.keys(project.assets).sort();
  return (
    <div className="panel">
      <div className="project-head">
        <div className="project-name">
          <Icon name={project.dir ? "folder" : "sparkle"} size={15} />
          <span>{project.name}</span>
          {dirty && <span className="dirty" title="Unsaved changes" />}
        </div>
        <div className="muted small">{project.dir ? "Folder on disk" : "Not saved to disk yet"}</div>
        <div className="btn-row">
          <button className="tool" onClick={A.newProject}><Icon name="plus" size={14} /> New</button>
          <button className="tool" onClick={A.openProject} disabled={!fsSupported}><Icon name="folder" size={14} /> Open</button>
          <button className="tool" onClick={() => A.save()}><Icon name="save" size={14} /> Save</button>
        </div>
      </div>
      <div className="tree">
        <button className="row file active" onClick={() => A.openPanel("editor")}>
          <Icon name="code" size={14} /> {project.fileName}
        </button>
        {assets.length > 0 && <div className="tree-label"><Icon name="folder" size={14} /> assets</div>}
        {assets.map((a) => (
          <button key={a} className="row file nested" onClick={() => A.openAsset(a)} title="Preview">
            <Icon name={/\.(gltf|glb)$/i.test(a) ? "cube" : /\.(png|jpe?g|gif|svg|webp)$/i.test(a) ? "image" : "file"} size={14} />
            {a.slice("assets/".length)}
          </button>
        ))}
      </div>
    </div>
  );
}

export function AssetPanel({ params }: IDockviewPanelProps<{ path: string }>) {
  const url = useStore((s) => s.project.assets[params.path]);
  const isImage = /\.(png|jpe?g|gif|svg|webp)$/i.test(params.path);
  const [size, setSize] = useState("");
  const load = isImage ? `LoadImage("${params.path}")` : /\.(gltf|glb)$/i.test(params.path) ? `LoadMesh("${params.path}")` : `LoadSound("${params.path}")`;
  return (
    <div className="panel asset">
      <div className="asset-view checker">
        {isImage && url ? <img src={url} alt={params.path} onLoad={(e) => setSize(`${e.currentTarget.naturalWidth} × ${e.currentTarget.naturalHeight}`)} /> : <Icon name="cube" size={48} />}
      </div>
      <div className="asset-info">
        <code>{params.path}</code>
        <span className="muted">{size}</span>
        <span className="grow" />
        <button className="tool" onClick={() => A.insertSnippet(load)}>Insert {load.split("(")[0]}</button>
      </div>
    </div>
  );
}

// ------------------------------------------------------------------ start page

export function StartPanel() {
  return (
    <div className="start">
      <div className="start-inner">
        <header className="hero">
          <img src="/icon.svg" alt="" width={56} height={56} />
          <div>
            <h1>CyberBasic</h1>
            <p>Make 2D and 3D games in a modern BASIC.</p>
          </div>
        </header>
        <div className="start-grid">
          <section>
            <h3>New project</h3>
            <div className="cards">
              {TEMPLATES.map((t) => (
                <button key={t.id} className="card" onClick={() => A.createFromTemplate(t)}>
                  <span className={"badge k" + t.kind.toLowerCase()}>{t.kind}</span>
                  <strong>{t.name}</strong>
                  <span>{t.description}</span>
                </button>
              ))}
            </div>
          </section>
          <aside>
            <h3>Open</h3>
            <button className="action" onClick={A.openProject} disabled={!fsSupported}>
              <Icon name="folder" /> Open folder… <kbd>Ctrl O</kbd>
            </button>
            <button className="action" onClick={() => A.openPanel("editor")}>
              <Icon name="code" /> Continue “{get().project.name}”
            </button>
            <h3>Shortcuts</h3>
            <dl className="keys">
              {[
                ["Run / Continue", "F5"],
                ["Stop", "Shift F5"],
                ["Pause", "F6"],
                ["Step", "F10"],
                ["Breakpoint", "F9"],
                ["Command palette", "Ctrl Shift P"],
                ["Save", "Ctrl S"],
                ["Workspaces", "Ctrl 1–4"],
              ].map(([k, v]) => (
                <div key={k}><dt>{k}</dt><dd><kbd>{v}</kbd></dd></div>
              ))}
            </dl>
          </aside>
        </div>
      </div>
    </div>
  );
}

export const COMPONENTS = {
  start: StartPanel,
  editor: EditorPanel,
  game: GamePanel,
  console: ConsolePanel,
  problems: ProblemsPanel,
  debugger: DebuggerPanel,
  reference: ReferencePanel,
  project: ProjectPanel,
  asset: AssetPanel,
};

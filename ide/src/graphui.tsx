// The node editor for .graph files: pan and zoom a canvas of nodes, drag wires between pins,
// type values into unconnected inputs. Every change is saved into the file (with undo).
import { useEffect, useLayoutEffect, useMemo, useRef, useState } from "react";
import * as A from "./actions";
import { canLink, graphPrefix, nextNodeId, parseGraph, type GNode, type Graph, type NodeDef, type PinDef } from "./graph";
import { Icon } from "./icons";
import { monaco } from "./language";
import { get, set, useStore } from "./store";

// Node geometry. Rows are absolutely positioned with these numbers, so wires always meet the pins.
const W = 236, HEAD = 30, ROW = 28, FIELD = 32, PAD = 8;

interface Layout {
  height: number;
  headIn?: PinDef;
  headOut?: PinDef;
  outRows: PinDef[];
  inRows: PinDef[];
  fieldsTop: number;
  rowsTop: number;
}

function layoutOf(def: NodeDef | undefined): Layout {
  if (!def) return { height: HEAD + PAD, outRows: [], inRows: [], fieldsTop: HEAD, rowsTop: HEAD };
  const headIn = def.inputs.find((p) => p.kind === "exec");
  const headOut = def.outputs.find((p) => p.kind === "exec" && p.key === "then" && !p.label);
  const outRows = def.outputs.filter((p) => p !== headOut);
  const inRows = def.inputs.filter((p) => p.kind === "data");
  const rowsTop = HEAD + (def.fields?.length ?? 0) * FIELD;
  return { height: rowsTop + (outRows.length + inRows.length) * ROW + PAD, headIn, headOut, outRows, inRows, fieldsTop: HEAD, rowsTop };
}

/** Where a pin is, in canvas coordinates. */
function pinPos(n: GNode, l: Layout, side: "in" | "out", key: string) {
  const x = side === "in" ? n.x : n.x + W;
  if (side === "in" && l.headIn?.key === key) return { x, y: n.y + HEAD / 2 };
  if (side === "out" && l.headOut?.key === key) return { x, y: n.y + HEAD / 2 };
  const out = l.outRows.findIndex((p) => p.key === key);
  if (side === "out" && out >= 0) return { x, y: n.y + l.rowsTop + out * ROW + ROW / 2 };
  const inp = l.inRows.findIndex((p) => p.key === key);
  return { x, y: n.y + l.rowsTop + (l.outRows.length + Math.max(0, inp)) * ROW + ROW / 2 };
}

const wirePath = (a: { x: number; y: number }, b: { x: number; y: number }) => {
  const dx = Math.max(50, Math.abs(b.x - a.x) / 2);
  return `M ${a.x} ${a.y} C ${a.x + dx} ${a.y}, ${b.x - dx} ${b.y}, ${b.x} ${b.y}`;
};

type View = { x: number; y: number; zoom: number };
const views = new Map<string, View>(); // per file, so switching tabs keeps your place
type PinRef = { node: string; pin: PinDef; side: "in" | "out" };

export function GraphPanel({ file }: { file: string }) {
  const text = useStore((s) => s.project.files[file] ?? "");
  const isMain = useStore((s) => s.project.fileName === file);
  const diagnostics = useStore((s) => s.diagnostics);
  const run = useStore((s) => s.run);
  const focus = useStore((s) => s.graphFocus);
  const graph = useMemo(() => parseGraph(text), [text]);
  const defs = A.nodeDefs();
  const [mode, setMode] = useState<"graph" | "code">("graph");
  const [view, setViewState] = useState<View>(views.get(file) ?? { x: 40, y: 40, zoom: 1 });
  const setView = (v: View) => {
    views.set(file, v);
    setViewState(v);
  };
  const [selected, setSelected] = useState<string[]>([]);
  const [selectedLink, setSelectedLink] = useState<number>();
  const [moving, setMoving] = useState<Record<string, { x: number; y: number }>>({}); // live drag positions
  const [wire, setWire] = useState<{ from: PinRef; to: { x: number; y: number } }>();
  const [palette, setPalette] = useState<{ x: number; y: number; at: { x: number; y: number }; pending?: PinRef }>();
  const [flash, setFlash] = useState<string>();
  const host = useRef<HTMLDivElement>(null);

  const save = (g: Graph) => A.updateGraph(file, g);
  const latest = () => parseGraph(get().project.files[file] ?? "");
  const node = (id: string) => {
    const n = graph.nodes.find((x) => x.id === id)!;
    return moving[id] ? { ...n, ...moving[id] } : n;
  };
  const select = (ids: string[]) => {
    setSelected(ids);
    setSelectedLink(undefined);
    A.graphSelection.set(file, ids);
  };

  // Generated-line -> node: errors and the paused position.
  const map = A.graphMaps.get(file);
  const errors = new Map<string, string>();
  for (const d of diagnostics) if (d.file === file) {
    const id = d.node ?? map?.get(d.line);
    if (id && !errors.has(id)) errors.set(id, d.message);
  }
  const pausedNode = run.state === "paused" && run.file === file ? map?.get(run.line ?? 0) : undefined;

  const toCanvas = (cx: number, cy: number) => {
    const r = host.current!.getBoundingClientRect();
    return { x: (cx - r.left - view.x) / view.zoom, y: (cy - r.top - view.y) / view.zoom };
  };

  const fit = (nodes = graph.nodes) => {
    const r = host.current?.getBoundingClientRect();
    if (!r || !nodes.length) return setView({ x: 40, y: 40, zoom: 1 });
    const x0 = Math.min(...nodes.map((n) => n.x)), y0 = Math.min(...nodes.map((n) => n.y));
    const x1 = Math.max(...nodes.map((n) => n.x + W)), y1 = Math.max(...nodes.map((n) => n.y + layoutOf(defs.get(n.type)).height));
    const zoom = Math.min(1.2, Math.max(0.3, Math.min((r.width - 80) / (x1 - x0), (r.height - 80) / (y1 - y0))));
    setView({ zoom, x: (r.width - (x1 - x0) * zoom) / 2 - x0 * zoom, y: (r.height - (y1 - y0) * zoom) / 2 - y0 * zoom });
  };
  useLayoutEffect(() => {
    if (!views.has(file)) fit();
  }, []);

  // Jump to a node when something asks (Problems, the debugger pausing).
  useEffect(() => {
    if (!focus || focus.file !== file) return;
    const id = A.graphMaps.get(file)?.get(focus.line);
    const n = graph.nodes.find((x) => x.id === id);
    const r = host.current?.getBoundingClientRect();
    if (!n || !r) return;
    setMode("graph");
    select([n.id]);
    setView({ ...view, x: r.width / 2 - (n.x + W / 2) * view.zoom, y: r.height / 2 - (n.y + 60) * view.zoom });
    setFlash(n.id);
    const t = setTimeout(() => setFlash(undefined), 1200);
    return () => clearTimeout(t);
  }, [focus, file]);

  // ---------------------------------------------------------------- pointer interactions

  const onBackgroundDown = (e: React.PointerEvent) => {
    if (e.button === 2) return;
    setPalette(undefined);
    const start = { x: e.clientX, y: e.clientY, view };
    let moved = false;
    const move = (ev: PointerEvent) => {
      moved ||= Math.abs(ev.clientX - start.x) + Math.abs(ev.clientY - start.y) > 3;
      setView({ ...start.view, x: start.view.x + ev.clientX - start.x, y: start.view.y + ev.clientY - start.y });
    };
    const up = () => {
      window.removeEventListener("pointermove", move);
      window.removeEventListener("pointerup", up);
      if (!moved) select([]);
    };
    window.addEventListener("pointermove", move);
    window.addEventListener("pointerup", up);
  };

  const onWheel = (e: React.WheelEvent) => {
    const r = host.current!.getBoundingClientRect();
    const zoom = Math.min(2, Math.max(0.25, view.zoom * (e.deltaY < 0 ? 1.1 : 1 / 1.1)));
    const px = e.clientX - r.left, py = e.clientY - r.top;
    setView({ zoom, x: px - ((px - view.x) / view.zoom) * zoom, y: py - ((py - view.y) / view.zoom) * zoom });
  };

  const onNodeDown = (e: React.PointerEvent, id: string) => {
    if (e.button !== 0) return;
    e.stopPropagation();
    const ids = e.shiftKey ? [...new Set([...selected, id])] : selected.includes(id) ? selected : [id];
    select(ids);
    const start = { x: e.clientX, y: e.clientY };
    const origin = Object.fromEntries(ids.map((i) => [i, { x: node(i).x, y: node(i).y }]));
    let last: Record<string, { x: number; y: number }> = {};
    const move = (ev: PointerEvent) => {
      const dx = (ev.clientX - start.x) / view.zoom, dy = (ev.clientY - start.y) / view.zoom;
      last = Object.fromEntries(ids.map((i) => [i, { x: Math.round(origin[i].x + dx), y: Math.round(origin[i].y + dy) }]));
      setMoving(last);
    };
    const up = () => {
      window.removeEventListener("pointermove", move);
      window.removeEventListener("pointerup", up);
      if (Object.keys(last).length) {
        const g = latest();
        save({ ...g, nodes: g.nodes.map((n) => (last[n.id] ? { ...n, ...last[n.id] } : n)) });
      }
      setMoving({});
    };
    window.addEventListener("pointermove", move);
    window.addEventListener("pointerup", up);
  };

  /** Connects two pins (either order). An exec output and a data input each take one wire. */
  const link = (a: PinRef, b: PinRef, g = latest()) => {
    if (!canLink(a, b)) return g;
    const [out, inp] = a.side === "out" ? [a, b] : [b, a];
    const keep = g.links.filter(
      (l) =>
        !(l.to === inp.node && l.in === inp.pin.key && inp.pin.kind === "data") &&
        !(l.from === out.node && l.out === out.pin.key && out.pin.kind === "exec"),
    );
    return { ...g, links: [...keep, { from: out.node, out: out.pin.key, to: inp.node, in: inp.pin.key }] };
  };

  const onPinDown = (e: React.PointerEvent, ref: PinRef) => {
    e.stopPropagation();
    if (e.altKey) {
      // Alt-click a pin to cut its wires.
      const g = latest();
      return save({ ...g, links: g.links.filter((l) => !((ref.side === "out" ? l.from : l.to) === ref.node && (ref.side === "out" ? l.out : l.in) === ref.pin.key)) });
    }
    const move = (ev: PointerEvent) => setWire({ from: ref, to: toCanvas(ev.clientX, ev.clientY) });
    const up = (ev: PointerEvent) => {
      window.removeEventListener("pointermove", move);
      window.removeEventListener("pointerup", up);
      setWire(undefined);
      const target = (document.elementFromPoint(ev.clientX, ev.clientY) as HTMLElement | null)?.closest<HTMLElement>("[data-pin]");
      if (target) {
        const n = graph.nodes.find((x) => x.id === target.dataset.node);
        const def = n && defs.get(n.type);
        const side = target.dataset.side as "in" | "out";
        const pin = def && (side === "in" ? def.inputs : def.outputs).find((p) => p.key === target.dataset.pin);
        if (pin && n) save(link(ref, { node: n.id, pin, side }));
        return;
      }
      // Dropped on empty canvas: pick a node to create there, wired to this pin.
      const r = host.current!.getBoundingClientRect();
      if (Math.abs(ev.clientX - e.clientX) + Math.abs(ev.clientY - e.clientY) > 6) {
        setPalette({ x: ev.clientX - r.left, y: ev.clientY - r.top, at: toCanvas(ev.clientX, ev.clientY), pending: ref });
      }
    };
    window.addEventListener("pointermove", move);
    window.addEventListener("pointerup", up);
  };

  const addNode = (def: NodeDef, at: { x: number; y: number }, pending?: PinRef) => {
    let g = latest();
    const id = nextNodeId(g);
    const n: GNode = { id, type: def.type, x: Math.round(at.x - (pending?.side === "in" ? W : 0)), y: Math.round(at.y - HEAD / 2), values: {} };
    if (def.type === "math.op" || def.type === "var.set") n.values.op = def.type === "math.op" ? "+" : "=";
    if (def.type === "math.compare") n.values.op = "=";
    g = { ...g, nodes: [...g.nodes, n] };
    if (pending) {
      // Wire the new node's first compatible pin to the one the wire was dragged from.
      const side = pending.side === "out" ? "in" : "out";
      const pin = (side === "in" ? def.inputs : def.outputs).find((p) => p.kind === pending.pin.kind);
      if (pin) g = link(pending, { node: id, pin, side }, g);
    }
    save(g);
    select([id]);
    setPalette(undefined);
  };

  const removeSelected = () => {
    const g = latest();
    if (selectedLink !== undefined) {
      save({ ...g, links: g.links.filter((_, i) => i !== selectedLink) });
      setSelectedLink(undefined);
      return;
    }
    if (!selected.length) return;
    save({ ...g, nodes: g.nodes.filter((n) => !selected.includes(n.id)), links: g.links.filter((l) => !selected.includes(l.from) && !selected.includes(l.to)) });
    select([]);
  };

  const duplicate = () => {
    const g = latest();
    let next = Number(nextNodeId(g).slice(1));
    const ids = new Map(selected.map((id) => [id, `n${next++}`]));
    const copies = g.nodes.filter((n) => ids.has(n.id)).map((n) => ({ ...structuredClone(n), id: ids.get(n.id)!, x: n.x + 30, y: n.y + 30, breakpoint: false }));
    const wires = g.links.filter((l) => ids.has(l.from) && ids.has(l.to)).map((l) => ({ ...l, from: ids.get(l.from)!, to: ids.get(l.to)! }));
    save({ ...g, nodes: [...g.nodes, ...copies], links: [...g.links, ...wires] });
    select([...ids.values()]);
  };

  const onKeyDown = (e: React.KeyboardEvent) => {
    if ((e.target as HTMLElement).closest("input, select, textarea")) return; // typing in a field
    const ctrl = e.ctrlKey || e.metaKey;
    if (e.key === "Delete" || e.key === "Backspace") removeSelected();
    else if (ctrl && e.key.toLowerCase() === "d") duplicate();
    else if (ctrl && e.key.toLowerCase() === "a") select(graph.nodes.map((n) => n.id));
    else if (e.key.toLowerCase() === "f") fit(selected.length ? graph.nodes.filter((n) => selected.includes(n.id)) : graph.nodes);
    else if (e.key === " " || e.key === "Tab") {
      const r = host.current!.getBoundingClientRect();
      setPalette({ x: r.width / 2 - 160, y: 60, at: toCanvas(r.left + r.width / 2, r.top + r.height / 2) });
    } else return;
    e.preventDefault();
  };

  const setValue = (id: string, key: string, value: string) => {
    const g = latest();
    save({ ...g, nodes: g.nodes.map((n) => (n.id === id ? { ...n, values: { ...n.values, [key]: value } } : n)) });
  };
  const toggleBreakpoint = (id: string) => {
    const g = latest();
    save({ ...g, nodes: g.nodes.map((n) => (n.id === id ? { ...n, breakpoint: !n.breakpoint } : n)) });
  };

  const linked = (id: string, side: "in" | "out", key: string) =>
    graph.links.some((l) => (side === "in" ? l.to === id && l.in === key : l.from === id && l.out === key));

  // ---------------------------------------------------------------- render

  const wires = graph.links.map((l, i) => {
    const a = graph.nodes.find((n) => n.id === l.from), b = graph.nodes.find((n) => n.id === l.to);
    if (!a || !b) return null;
    const da = defs.get(a.type), db = defs.get(b.type);
    const kind = da?.outputs.find((p) => p.key === l.out)?.kind ?? "data";
    const pa = pinPos(node(a.id), layoutOf(da), "out", l.out), pb = pinPos(node(b.id), layoutOf(db), "in", l.in);
    const d = wirePath(pa, pb);
    return (
      <g key={i} className={"wire " + kind + (selectedLink === i ? " selected" : "") + (pausedNode === a.id ? " live" : "")}>
        <path d={d} className="wire-hit" onPointerDown={(e) => { e.stopPropagation(); setSelected([]); setSelectedLink(i); }} />
        <path d={d} className="wire-line" />
      </g>
    );
  });
  let draft = null;
  if (wire) {
    const n = graph.nodes.find((x) => x.id === wire.from.node)!;
    const p = pinPos(node(n.id), layoutOf(defs.get(n.type)), wire.from.side, wire.from.pin.key);
    draft = <path className={"wire-line draft " + wire.from.pin.kind} d={wire.from.side === "out" ? wirePath(p, wire.to) : wirePath(wire.to, p)} />;
  }

  return (
    <div className="panel graph-panel" onMouseDown={() => set({ activeFile: file })}>
      <div className="panel-bar vis-bar">
        <Icon name="graph" size={15} className="vis-icon" />
        <strong>{file}</strong>
        <span className="muted small ellipsis">
          {isMain ? "Node graph · runs the game (Esc quits)" : `Node graph · call ${graphPrefix(file)}_Frame each frame from your code`}
        </span>
        <span className="grow" />
        {mode === "graph" && (
          <>
            <button className="tool" onClick={() => { const r = host.current!.getBoundingClientRect(); setPalette({ x: 16, y: 12, at: toCanvas(r.left + r.width / 2 - W / 2, r.top + 80) }); }}>
              <Icon name="plus" size={13} /> Node
            </button>
            <button className="icon-btn" title="Fit everything in view (F)" onClick={() => fit()}><Icon name="fit" size={14} /></button>
          </>
        )}
        <div className="seg small">
          <button className={mode === "graph" ? "on" : ""} onClick={() => setMode("graph")}>Graph</button>
          <button className={mode === "code" ? "on" : ""} onClick={() => setMode("code")} title="The CyberBasic this graph becomes">Code</button>
        </div>
      </div>
      {mode === "code" ? (
        <CodeView file={file} />
      ) : (
        <>
          <div className="graph-vars">
            <Variables variables={graph.variables} change={(variables) => save({ ...latest(), variables })} />
          </div>
          <div
            ref={host}
            className="graph-canvas"
            tabIndex={0}
            onPointerDown={onBackgroundDown}
            onWheel={onWheel}
            onKeyDown={onKeyDown}
            onContextMenu={(e) => {
              e.preventDefault();
              const r = host.current!.getBoundingClientRect();
              setPalette({ x: e.clientX - r.left, y: e.clientY - r.top, at: toCanvas(e.clientX, e.clientY) });
            }}
            style={{ backgroundPosition: `${view.x}px ${view.y}px`, backgroundSize: `${24 * view.zoom}px ${24 * view.zoom}px` }}
          >
            <div className="graph-world" style={{ transform: `translate(${view.x}px, ${view.y}px) scale(${view.zoom})` }}>
              <svg className="graph-wires">
                {wires}
                {draft}
              </svg>
              {graph.nodes.map((raw) => {
                const n = node(raw.id);
                const def = defs.get(n.type);
                const l = layoutOf(def);
                const err = errors.get(n.id);
                const pin = (p: PinDef, side: "in" | "out", y: number) => (
                  <span
                    key={side + p.key}
                    className={`pin ${p.kind} ${side}` + (linked(n.id, side, p.key) ? " linked" : "")}
                    style={{ top: y - 7 }}
                    data-pin={p.key}
                    data-node={n.id}
                    data-side={side}
                    title={`${p.label || p.key} (${p.kind === "exec" ? "drag to connect what happens next" : "drag to connect a value"}; Alt+click cuts)`}
                    onPointerDown={(e) => onPinDown(e, { node: n.id, pin: p, side })}
                  />
                );
                return (
                  <div
                    key={n.id}
                    className={
                      `node c-${def?.color ?? "code"}` +
                      (selected.includes(n.id) ? " selected" : "") +
                      (pausedNode === n.id ? " paused" : "") +
                      (err ? " has-error" : "") +
                      (flash === n.id ? " flash" : "")
                    }
                    style={{ left: n.x, top: n.y, width: W, height: l.height }}
                    onPointerDown={(e) => onNodeDown(e, n.id)}
                  >
                    <div className="node-head" title={def?.doc}>
                      <button
                        className={"node-bp" + (n.breakpoint ? " on" : "")}
                        title={n.breakpoint ? "Remove breakpoint (F9)" : "Pause when this node runs (F9)"}
                        onPointerDown={(e) => e.stopPropagation()}
                        onClick={() => toggleBreakpoint(n.id)}
                      />
                      <span className="node-title">{def?.title ?? `Unknown: ${n.type}`}</span>
                    </div>
                    {l.headIn && pin(l.headIn, "in", HEAD / 2)}
                    {l.headOut && pin(l.headOut, "out", HEAD / 2)}
                    {def?.fields?.map((f, i) => (
                      <div key={f.key} className="node-field" style={{ top: l.fieldsTop + i * FIELD }} onPointerDown={(e) => e.stopPropagation()}>
                        {f.label && <span className="muted">{f.label}</span>}
                        {f.options ? (
                          <select value={n.values[f.key] ?? f.options[0]} onChange={(e) => setValue(n.id, f.key, e.target.value)}>
                            {f.options.map((o) => <option key={o}>{o}</option>)}
                          </select>
                        ) : (
                          <Field wide value={n.values[f.key] ?? ""} placeholder={f.placeholder ?? ""} onChange={(v) => setValue(n.id, f.key, v)} />
                        )}
                      </div>
                    ))}
                    {l.outRows.map((p, i) => {
                      const y = l.rowsTop + i * ROW;
                      return (
                        <div key={"o" + p.key} className="node-row out" style={{ top: y }}>
                          <span>{p.label || p.key}</span>
                          {pin(p, "out", ROW / 2)}
                        </div>
                      );
                    })}
                    {l.inRows.map((p, i) => {
                      const y = l.rowsTop + (l.outRows.length + i) * ROW;
                      const wired = linked(n.id, "in", p.key);
                      return (
                        <div key={"i" + p.key} className="node-row in" style={{ top: y }} onPointerDown={(e) => !wired && e.stopPropagation()}>
                          {pin(p, "in", ROW / 2)}
                          <span className="pin-label">{p.label}</span>
                          {!wired && (
                            <input
                              className="node-input mono"
                              value={n.values[p.key] ?? ""}
                              placeholder={p.key}
                              spellCheck={false}
                              onChange={(e) => setValue(n.id, p.key, e.target.value)}
                            />
                          )}
                        </div>
                      );
                    })}
                    {err && <div className="node-error" title={err}>{err}</div>}
                    {pausedNode === n.id && <div className="node-paused"><Icon name="pause" size={11} /> paused here</div>}
                  </div>
                );
              })}
            </div>
            {graph.nodes.length === 0 && (
              <div className="graph-empty">Right-click, press Space, or use <b>+ Node</b> to add your first node. Start with <b>On Frame</b>.</div>
            )}
            {palette && <NodePalette defs={defs} pending={palette.pending} style={{ left: palette.x, top: palette.y }} pick={(d) => addNode(d, palette.at, palette.pending)} close={() => setPalette(undefined)} />}
            <div className="graph-hint muted small">Drag background to pan · wheel to zoom · drag from a pin to wire · Space adds a node · Del removes · F fits</div>
          </div>
        </>
      )}
    </div>
  );
}

/** Searchable list of nodes; when a wire is being dragged, only nodes it can connect to. */
function NodePalette({ defs, pending, style, pick, close }: { defs: Map<string, NodeDef>; pending?: PinRef; style: React.CSSProperties; pick: (d: NodeDef) => void; close: () => void }) {
  const [q, setQ] = useState("");
  const [i, setI] = useState(0);
  const words = q.toLowerCase().split(/\s+/).filter(Boolean);
  const order = ["Events", "Flow", "Variables", "Math & Logic", "Code"];
  const shown = [...defs.values()]
    .filter((d) => !pending || (pending.side === "out" ? d.inputs : d.outputs).some((p) => p.kind === pending.pin.kind))
    .filter((d) => words.every((w) => `${d.title} ${d.category} ${d.doc}`.toLowerCase().includes(w)))
    .sort((a, b) => {
      const ia = order.indexOf(a.category), ib = order.indexOf(b.category);
      return (ia < 0 ? 99 : ia) - (ib < 0 ? 99 : ib) || a.category.localeCompare(b.category);
    });
  let group = "";
  return (
    <div className="picker-pop node-palette" style={style} onPointerDown={(e) => e.stopPropagation()} onWheel={(e) => e.stopPropagation()}>
      <div className="search">
        <Icon name="search" size={13} />
        <input
          autoFocus
          placeholder="Search nodes: branch, set, rect, key…"
          value={q}
          onChange={(e) => {
            setQ(e.target.value);
            setI(0);
          }}
          onKeyDown={(e) => {
            if (e.key === "Escape") close();
            if (e.key === "ArrowDown") setI(Math.min(i + 1, shown.length - 1));
            if (e.key === "ArrowUp") setI(Math.max(i - 1, 0));
            if (e.key === "Enter" && shown[i]) pick(shown[i]);
            if (["ArrowDown", "ArrowUp", "Enter"].includes(e.key)) e.preventDefault();
            e.stopPropagation();
          }}
        />
      </div>
      <div className="picker-list">
        {shown.length === 0 && <div className="empty small">Nothing matches “{q}”.</div>}
        {shown.map((d, n) => {
          const head = d.category !== group ? <div className="picker-group">{(group = d.category)}</div> : null;
          return (
            <div key={d.type}>
              {head}
              <button className={"picker-item" + (n === i ? " on" : "")} onMouseEnter={() => setI(n)} onClick={() => pick(d)}>
                <span className="mono"><i className={`swatch c-${d.color}`} />{d.title}</span>
                <span className="muted small">{d.doc}</span>
              </button>
            </div>
          );
        })}
      </div>
    </div>
  );
}


// ---------------------------------------------------------------- small shared pieces

/** An input that grows with its text; shows its placeholder until filled. */
function Field({ value, placeholder, onChange, wide }: { value: string; placeholder: string; onChange: (v: string) => void; wide?: boolean }) {
  return (
    <input
      className={"field mono" + (wide ? " wide" : "")}
      value={value}
      placeholder={placeholder}
      size={Math.max(value.length, placeholder.length, 2)}
      spellCheck={false}
      onChange={(e) => onChange(e.target.value)}
    />
  );
}

type Vars = { name: string; value: string }[];

/** The graph's variables: each becomes a Global. */
function Variables({ variables, change }: { variables: Vars; change: (v: Vars) => void }) {
  const setVar = (i: number, v: { name: string; value: string }) => change(variables.map((x, j) => (j === i ? v : x)));
  return (
    <section className="vis-vars">
      <span className="col-label">Variables</span>
      {variables.map((v, i) => (
        <span key={i} className="var-chip">
          <Field value={v.name} placeholder="name" onChange={(name) => setVar(i, { ...v, name })} />
          <span className="muted">=</span>
          <Field value={v.value} placeholder="0" onChange={(value) => setVar(i, { ...v, value })} />
          <button className="row-del" title="Remove" onClick={() => change(variables.filter((_, j) => j !== i))}>
            <Icon name="close" size={11} />
          </button>
        </span>
      ))}
      <button className="add-row" onClick={() => change([...variables, { name: "", value: "0" }])}>
        <Icon name="plus" size={12} /> Variable
      </button>
    </section>
  );
}

/** The CyberBasic a graph becomes, read only and highlighted. */
function CodeView({ file }: { file: string }) {
  const text = useStore((s) => s.project.files[file]);
  const [html, setHtml] = useState("");
  useEffect(() => {
    monaco.editor.colorize(A.generatedCode(file), "cyberbasic", { tabSize: 4 }).then(setHtml);
  }, [file, text]);
  return (
    <div className="vis-code">
      <div className="muted small vis-code-note">Read only: this is the CyberBasic this graph becomes. Edit the graph to change it.</div>
      <pre className="mono" dangerouslySetInnerHTML={{ __html: html }} />
    </div>
  );
}

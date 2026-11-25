// The level editor for .scene files: a 3D view with move/rotate/scale gizmos, a list of the
// scene's objects and an inspector. Every change is saved into the file (with undo); the compiler
// turns the file into LoadScene_<Name>, which the game calls.
import { useEffect, useMemo, useRef, useState } from "react";
import * as A from "./actions";
import { Icon } from "./icons";
import { KINDS, dab, decodeColors, decodeHeights, encodeColors, encodeHeights, isMesh, loadFunction, newObject, parseScene, sceneText, type Brush, type Scene, type SceneKind, type SceneObject, type Vec3 } from "./scene";
import { buildModel, parseModel } from "./model";
import { get, set, useStore } from "./store";
import { pickable, round, setTransform, transformOf, useGizmo, useViewport, type Any, type GizmoMode } from "./viewport";

const BRUSHES: { tool: Brush; label: string; tip: string }[] = [
  { tool: "raise", label: "Raise", tip: "Pull the ground up" },
  { tool: "lower", label: "Lower", tip: "Push the ground down" },
  { tool: "smooth", label: "Smooth", tip: "Soften bumps and edges" },
  { tool: "flatten", label: "Flatten", tip: "Level to the height where the stroke started" },
  { tool: "paint", label: "Paint", tip: "Paint the ground's color" },
];
/** Squares per side of a terrain's grid, as the runtime clamps it. */
const gridOf = (o: SceneObject) => Math.max(1, Math.min(256, Math.trunc(o.divisions ?? 64)));
export interface Tools {
  brush: Brush | null;
  setBrush: (b: Brush | null) => void;
  radius: number;
  setRadius: (n: number) => void;
  strength: number;
  setStrength: (n: number) => void;
  paint: Vec3;
  setPaint: (c: Vec3) => void;
}

type Mode = GizmoMode;

export function ScenePanel({ file }: { file: string }) {
  const text = useStore((s) => s.project.files[file] ?? "");
  const assets = useStore((s) => s.project.assets);
  const files = useStore((s) => s.project.files);
  const diagnostics = useStore((s) => s.diagnostics);
  const focus = useStore((s) => s.graphFocus);
  const scene = useMemo(() => parseScene(text), [text]);
  const behaviours = useMemo(() => A.projectBehaviours(files), [files]);
  const [selected, setSelected] = useState<string>();
  const [mode, setMode] = useState<Mode>("move");
  const [snap, setSnap] = useState(true);
  const [brush, setBrush] = useState<Brush | null>(null);
  const [radius, setRadius] = useState(4);
  const [strength, setStrength] = useState(0.5);
  const [paint, setPaint] = useState<Vec3>([150, 120, 85]);
  const brushRef = useRef({ tool: "raise" as Brush, radius, strength, color: paint });
  brushRef.current = { tool: brush ?? "raise", radius, strength, color: paint };
  const tools: Tools = { brush, setBrush, radius, setRadius, strength, setStrength, paint, setPaint };
  const canvas = useRef<HTMLCanvasElement>(null);
  const { view, ready, error } = useViewport(canvas, setSelected);
  const nodes = useRef(new Map<string, { node: Any; key: string }>()).current;

  const latest = () => parseScene(get().project.files[file] ?? "");
  const save = (s: Scene) => A.updateVisual(file, sceneText(s));
  const change = (id: string, patch: Partial<SceneObject>) => {
    const s = latest();
    save({ ...s, objects: s.objects.map((o) => (o.id === id ? { ...o, ...patch } : o)) });
  };
  const sel = scene.objects.find((o) => o.id === selected);
  const sculpting = !!brush && sel?.kind === "terrain";
  const errors = new Map<string, string>();
  for (const d of diagnostics) if (d.file === file) {
    const id = d.node ?? A.graphMaps.get(file)?.get(d.line);
    if (id && !errors.has(id)) errors.set(id, d.message);
  }

  // ------------------------------------------------------------ the 3D view

  // Make the 3D view match the file: create, replace or update each object's node.
  useEffect(() => {
    const v = view.current;
    if (!v) return;
    const { B } = v;
    const seen = new Set<string>();
    for (const o of scene.objects) {
      seen.add(o.id);
      const key = o.kind === "terrain" ? `terrain:${o.size}:${gridOf(o)}:${o.heights}:${o.colors}` : `${o.kind}:${o.model ?? ""}:${o.model?.endsWith(".model") ? files[o.model] : ""}`;
      let entry = nodes.get(o.id);
      if (entry && entry.key !== key) {
        entry.node.dispose();
        entry = undefined;
      }
      if (!entry) {
        entry = { node: build(v, o, assets, files), key };
        nodes.set(o.id, entry);
      }
      const n = entry.node;
      setTransform(B, n, o.position, o.rotation, isMesh(o.kind) ? o.scale : undefined);
      for (const m of [n, ...n.getChildMeshes()]) m.visibility = o.hidden ? 0.3 : 1; // hidden things show faintly
      const c = o.color ?? [200, 200, 210];
      for (const m of [n, ...n.getChildMeshes()]) {
        if (m.material && isMesh(o.kind)) "albedoColor" in m.material ? (m.material.albedoColor = new B.Color3(c[0] / 255, c[1] / 255, c[2] / 255)) : (m.material.diffuseColor = new B.Color3(c[0] / 255, c[1] / 255, c[2] / 255));
      }
    }
    for (const [id, e] of nodes) if (!seen.has(id)) {
      e.node.dispose();
      nodes.delete(id);
    }
  }, [scene, ready, assets, files]);

  // Gizmo on the selected object; saves when a drag ends.
  const selNode = selected && !sculpting ? nodes.get(selected)?.node : undefined;
  useGizmo(view, selNode, { mode, snap, canScale: !!sel && isMesh(sel.kind) }, (node) => {
    const t = transformOf(node);
    change(selected!, { position: t.position, rotation: t.rotation, ...(sel && isMesh(sel.kind) ? { scale: t.scale } : {}) });
  }, [ready, scene]);

  // Sculpting and painting a terrain: left-drag applies the brush; right-drag orbits, middle pans.
  useEffect(() => {
    const v = view.current;
    const mesh = sculpting && selected ? nodes.get(selected)?.node : undefined;
    if (!v || !mesh) return;
    const { B, scene: s, camera } = v;
    const o = latest().objects.find((x) => x.id === selected)!;
    const n = gridOf(o);
    const rgb = decodeColors(o.colors ?? "", (n + 1) * (n + 1));
    const pointers = camera.inputs.attached.pointers;
    pointers.buttons = [1, 2];
    camera._panningMouseButtonInternal = 1;
    const ring = B.MeshBuilder.CreateTorus("editor-brush", { diameter: 2, thickness: 0.05, tessellation: 64 }, s);
    ring.isPickable = false;
    ring.material = new B.StandardMaterial("editor-brush", s);
    ring.material.emissiveColor = new B.Color3(0.3, 0.76, 1);
    ring.material.disableLighting = true;
    ring.setEnabled(false);
    let down = false, target = 0;
    const hit = () => {
      const pick = s.pick(s.pointerX, s.pointerY, (m: Any) => m === mesh);
      if (!pick?.hit) return undefined;
      return { world: pick.pickedPoint, local: B.Vector3.TransformCoordinates(pick.pickedPoint, mesh.getWorldMatrix().clone().invert()) };
    };
    const apply = (p: Any) => {
      const pos = mesh.getVerticesData(B.VertexBuffer.PositionKind);
      if (dab(pos, rgb, n, p.x, p.z, { ...brushRef.current, target })) refreshTerrain(B, mesh, pos, rgb);
    };
    const obs = s.onPointerObservable.add((info: Any) => {
      const P = B.PointerEventTypes;
      if (info.type === P.POINTERMOVE || info.type === P.POINTERDOWN) {
        const h = hit();
        ring.setEnabled(!!h);
        if (!h) return;
        ring.position.set(h.world.x, h.world.y + 0.05, h.world.z);
        ring.scaling.set(brushRef.current.radius, 1, brushRef.current.radius);
        if (info.type === P.POINTERDOWN && info.event.button === 0) {
          down = true;
          target = h.local.y;
          apply(h.local);
        } else if (down) apply(h.local);
      } else if (info.type === P.POINTERUP && down) {
        down = false;
        const pos = mesh.getVerticesData(B.VertexBuffer.PositionKind);
        change(o.id, { heights: encodeHeights(Array.from({ length: pos.length / 3 }, (_, i) => pos[i * 3 + 1])), colors: encodeColors(rgb) });
      }
    });
    return () => {
      s.onPointerObservable.remove(obs);
      ring.dispose();
      pointers.buttons = [0, 1, 2];
      camera._panningMouseButtonInternal = 2;
    };
  }, [sculpting, selected, ready, scene]);

  // Jump to an object when Problems asks.
  useEffect(() => {
    if (focus?.file !== file) return;
    const id = A.graphMaps.get(file)?.get(focus.line);
    if (id) setSelected(id);
  }, [focus, file]);

  // ------------------------------------------------------------ editing

  const target = (): Vec3 => {
    const t = view.current?.camera.target;
    return t ? [round(t.x, 0.5), 0.5, round(t.z, 0.5)] : [0, 0.5, 0];
  };
  const add = (kind: SceneKind, model?: string) => {
    const s = latest();
    const at = target();
    const o = newObject(s, kind, kind === "plane" ? [at[0], 0, at[2]] : kind === "light" || kind === "pointlight" ? [at[0], 4, at[2]] : at, model);
    save({ ...s, objects: [...s.objects, o] });
    setSelected(o.id);
  };
  const remove = () => {
    if (!selected) return;
    const s = latest();
    save({ ...s, objects: s.objects.filter((o) => o.id !== selected) });
    setSelected(undefined);
  };
  const duplicate = () => {
    const s = latest();
    const o = s.objects.find((x) => x.id === selected);
    if (!o) return;
    const fresh = newObject(s, o.kind);
    const copy: SceneObject = { ...structuredClone(o), id: fresh.id, name: fresh.name, position: [o.position[0] + 1, o.position[1], o.position[2] + 1] };
    save({ ...s, objects: [...s.objects, copy] });
    setSelected(copy.id);
  };
  const frame = () => {
    const n = selected && nodes.get(selected)?.node;
    if (n) view.current!.camera.setTarget(n.getAbsolutePosition().clone());
  };
  const onKeyDown = (e: React.KeyboardEvent) => {
    if ((e.target as HTMLElement).closest("input, select, textarea")) return;
    const k = e.key.toLowerCase(), ctrl = e.ctrlKey || e.metaKey;
    if (k === "w" && !ctrl) setMode("move");
    else if (k === "e" && !ctrl) setMode("rotate");
    else if (k === "r" && !ctrl) setMode("scale");
    else if (k === "f" && !ctrl) frame();
    else if (k === "delete" || k === "backspace") remove();
    else if (ctrl && k === "d") duplicate();
    else if (k === "escape") brush ? setBrush(null) : setSelected(undefined);
    else return;
    e.preventDefault();
  };
  const models = [...Object.keys(files).filter(A.isModel), ...Object.keys(assets).filter((a) => /\.(glb|gltf)$/i.test(a))];

  return (
    <div className="panel scene-panel" onMouseDown={() => set({ activeFile: file })} onKeyDown={onKeyDown}>
      <div className="panel-bar vis-bar">
        <Icon name="cube" size={15} className="vis-icon" />
        <strong>{file}</strong>
        <span className="muted small ellipsis">Level · build it in code with {loadFunction(file)} (after Graphics3D), then FindEntity("Name")</span>
        <span className="grow" />
        <div className="seg small" title="Gizmo: W move, E rotate, R scale">
          {(["move", "rotate", "scale"] as Mode[]).map((m) => (
            <button key={m} className={mode === m ? "on" : ""} onClick={() => setMode(m)}>{m[0].toUpperCase() + m.slice(1)}</button>
          ))}
        </div>
        <label className="check small" title="Snap moves to 0.5, turns to 15° and scaling to 0.25">
          <input type="checkbox" checked={snap} onChange={(e) => setSnap(e.target.checked)} /> Snap
        </label>
      </div>
      <div className="scene-body">
        <div className="scene-side">
          <div className="scene-add">
            {KINDS.filter((k) => k.kind !== "model").map((k) => (
              <button key={k.kind} className="tool" onClick={() => add(k.kind)}>+ {k.label}</button>
            ))}
            {models.length > 0 && (
              <select className="field" value="" onChange={(e) => e.target.value && add("model", e.target.value)} title="Add a model from assets/">
                <option value="">+ Model…</option>
                {models.map((m) => <option key={m}>{m}</option>)}
              </select>
            )}
          </div>
          <div className="list scene-list">
            {scene.objects.map((o) => (
              <button
                key={o.id}
                className={"row scene-row" + (o.id === selected ? " active" : "") + (errors.has(o.id) ? " has-error" : "")}
                title={errors.get(o.id) ?? o.kind}
                onClick={() => setSelected(o.id)}
                onDoubleClick={() => { setSelected(o.id); setTimeout(frame); }}
              >
                <span className={`kind-dot k-${o.kind}`} />
                <span className="ellipsis">{o.name || "(no name)"}</span>
                {o.behaviours?.length ? <span className="muted small">{o.behaviours.join(", ")}</span> : null}
              </button>
            ))}
          </div>
        </div>
        <div className="scene-view">
          <canvas ref={canvas} tabIndex={0} />
          {error && <div className="graph-empty">{error}</div>}
          <div className="scene-hint muted small">
            {sculpting
              ? `${BRUSHES.find((b) => b.tool === brush)?.label}: left-drag on the terrain · right-drag orbits · middle-drag pans · Esc stops sculpting`
              : "Drag to orbit · right-drag to pan · wheel to zoom · click to select · W/E/R gizmo · F frame · Ctrl+D duplicate · Del delete"}
          </div>
        </div>
        <div className="scene-inspector">
          {sel ? (
            <Inspector key={sel.id} o={sel} change={(p) => change(sel.id, p)} behaviours={behaviours} models={models} error={errors.get(sel.id)} remove={remove} duplicate={duplicate} tools={tools} />
          ) : (
            <div className="empty muted small">Select an object to edit it, or add one on the left.</div>
          )}
        </div>
      </div>
    </div>
  );
}

/** Puts new heights (in `pos`) and colors on a terrain mesh. */
function refreshTerrain(B: Any, mesh: Any, pos: Any, rgb: Uint8Array) {
  const count = pos.length / 3, col = new Float32Array(count * 4), normals: number[] = [];
  for (let i = 0; i < count; i++) col.set([rgb[i * 3] / 255, rgb[i * 3 + 1] / 255, rgb[i * 3 + 2] / 255, 1], i * 4);
  B.VertexData.ComputeNormals(pos, mesh.getIndices(), normals);
  mesh.updateVerticesData(B.VertexBuffer.PositionKind, pos);
  mesh.updateVerticesData(B.VertexBuffer.NormalKind, normals);
  if (mesh.isVerticesDataPresent(B.VertexBuffer.ColorKind)) mesh.updateVerticesData(B.VertexBuffer.ColorKind, col);
  else mesh.setVerticesData(B.VertexBuffer.ColorKind, col, true);
  mesh.refreshBoundingInfo();
}

/** Creates the editor's stand-in for an object. Lights and cameras get small markers. */
function build(v: { B: Any; scene: Any }, o: SceneObject, assets: Record<string, string>, files: Record<string, string>): Any {
  const { B, scene: s } = v;
  const mat = (c: [number, number, number], emissive = false) => {
    const m = new B.StandardMaterial("m", s);
    m.diffuseColor = new B.Color3(...c);
    if (emissive) m.emissiveColor = new B.Color3(...c);
    return m;
  };
  let node: Any;
  switch (o.kind) {
    case "cube":
      node = B.MeshBuilder.CreateBox(o.id, { size: 1 }, s);
      node.material = mat([0.8, 0.8, 0.8]);
      break;
    case "sphere":
      node = B.MeshBuilder.CreateSphere(o.id, { diameter: 1 }, s);
      node.material = mat([0.8, 0.8, 0.8]);
      break;
    case "plane":
      node = B.MeshBuilder.CreateGround(o.id, { width: 1, height: 1 }, s);
      node.material = mat([0.8, 0.8, 0.8]);
      node.material.backFaceCulling = false;
      break;
    case "terrain": {
      const size = o.size ?? 40, n = gridOf(o);
      node = B.MeshBuilder.CreateGround(o.id, { width: size, height: size, subdivisions: n, updatable: true }, s);
      const pos = node.getVerticesData(B.VertexBuffer.PositionKind), count = pos.length / 3;
      const h = decodeHeights(o.heights ?? "", count);
      for (let i = 0; i < count; i++) pos[i * 3 + 1] = h[i];
      refreshTerrain(B, node, pos, decodeColors(o.colors ?? "", count));
      node.material = mat([1, 1, 1]);
      node.material.specularColor = new B.Color3(0.04, 0.04, 0.04);
      break;
    }
    case "light":
    case "pointlight": {
      node = B.MeshBuilder.CreateSphere(o.id, { diameter: 0.5 }, s);
      node.material = mat([1, 0.85, 0.3], true);
      if (o.kind === "light") {
        // Shows the way a directional light shines (+Z, like the runtime's light).
        const ray = B.MeshBuilder.CreateCylinder(o.id + "-ray", { height: 1.6, diameterTop: 0, diameterBottom: 0.25 }, s);
        ray.rotation.x = Math.PI / 2;
        ray.position.z = 1;
        ray.material = node.material;
        ray.parent = node;
      }
      break;
    }
    case "camera": {
      node = B.MeshBuilder.CreateBox(o.id, { width: 0.6, height: 0.45, depth: 0.7 }, s);
      node.material = mat([0.55, 0.6, 0.75]);
      const lens = B.MeshBuilder.CreateCylinder(o.id + "-lens", { height: 0.5, diameterTop: 0.7, diameterBottom: 0.25 }, s);
      lens.rotation.x = Math.PI / 2;
      lens.position.z = 0.55;
      lens.material = node.material;
      lens.parent = node;
      break;
    }
    case "model": {
      if (o.model && A.isModel(o.model) && o.model in files) {
        node = buildModel(B, s, parseModel(files[o.model]), o.id, false).root;
        break;
      }
      node = new B.TransformNode(o.id, s);
      const path = o.model ?? "", url = assets[path] ?? path;
      B.SceneLoader.ImportMeshAsync("", "", url, s, null, path.slice(path.lastIndexOf(".")))
        .then((r: Any) => {
          if (node.isDisposed()) return r.meshes.forEach((m: Any) => m.dispose());
          r.meshes[0].parent = node;
          pickable(node, o.id);
        })
        .catch(() => {
          const box = B.MeshBuilder.CreateBox(o.id + "-missing", { size: 1 }, s);
          box.material = mat([0.9, 0.3, 0.3]);
          box.material.wireframe = true;
          box.parent = node;
          pickable(node, o.id);
        });
      break;
    }
  }
  pickable(node, o.id);
  return node;
}

function Inspector({ o, change, behaviours, models, error, remove, duplicate, tools }: {
  o: SceneObject;
  change: (p: Partial<SceneObject>) => void;
  behaviours: string[];
  models: string[];
  error?: string;
  remove: () => void;
  duplicate: () => void;
  tools: Tools;
}) {
  const vec = (label: string, key: "position" | "rotation" | "scale") => (
    <div className="insp-row">
      <span className="col-label">{label}</span>
      <div className="vec">
        {[0, 1, 2].map((i) => (
          <Num key={i} axis={"xyz"[i]} value={o[key][i]} onChange={(n) => change({ [key]: o[key].map((x, j) => (j === i ? n : x)) as Vec3 })} />
        ))}
      </div>
    </div>
  );
  const hex = (c: Vec3) => "#" + c.map((x) => Math.round(x).toString(16).padStart(2, "0")).join("");
  return (
    <div className="inspector">
      <div className="insp-row">
        <span className="col-label">Name</span>
        <input className="field wide" value={o.name} spellCheck={false} onChange={(e) => change({ name: e.target.value })} title='FindEntity("…") finds the object by this name' />
      </div>
      <div className="muted small insp-kind">{KINDS.find((k) => k.kind === o.kind)?.label}{o.kind === "model" ? ` · ${o.model}` : ""}</div>
      {error && <div className="node-error static">{error}</div>}
      {vec("Position", "position")}
      {vec("Rotation", "rotation")}
      {isMesh(o.kind) && vec("Scale", "scale")}
      {o.kind === "model" && (
        <div className="insp-row">
          <span className="col-label">Model</span>
          <select className="field wide" value={o.model} onChange={(e) => change({ model: e.target.value })}>
            {[...new Set([o.model ?? "", ...models])].map((m) => <option key={m}>{m}</option>)}
          </select>
        </div>
      )}
      {o.kind === "terrain" && (
        <>
          <div className="insp-row">
            <span className="col-label">Size</span>
            <Num axis="m" value={o.size ?? 40} onChange={(size) => change({ size: Math.max(1, size) })} />
            <Num
              axis="grid"
              value={gridOf(o)}
              onChange={(v) => {
                const divisions = Math.max(1, Math.min(256, Math.round(v)));
                const keep = !o.heights && !o.colors;
                if (keep || confirm("Changing the grid clears this terrain's sculpting and paint. Continue?")) change({ divisions, heights: "", colors: "" });
              }}
            />
          </div>
          <div className="insp-row">
            <span className="col-label">Physics</span>
            <select className="field" value={o.physics === "none" ? "none" : "static"} onChange={(e) => change({ physics: e.target.value as SceneObject["physics"] })} title="Static: things stand and roll on it.">
              <option value="none">None</option>
              <option value="static">Static</option>
            </select>
          </div>
          <div className="insp-row top">
            <span className="col-label">Sculpt</span>
            <div className="brushes">
              {BRUSHES.map((b) => (
                <button key={b.tool} className={"tool" + (tools.brush === b.tool ? " on" : "")} title={b.tip} onClick={() => tools.setBrush(tools.brush === b.tool ? null : b.tool)}>
                  {b.label}
                </button>
              ))}
            </div>
          </div>
          {tools.brush && (
            <>
              <label className="insp-row">
                <span className="col-label">Radius</span>
                <input type="range" min={0.5} max={15} step={0.5} value={tools.radius} onChange={(e) => tools.setRadius(Number(e.target.value))} />
                <span className="muted small mono">{tools.radius}</span>
              </label>
              <label className="insp-row">
                <span className="col-label">Strength</span>
                <input type="range" min={0.05} max={1} step={0.05} value={tools.strength} onChange={(e) => tools.setStrength(Number(e.target.value))} />
                <span className="muted small mono">{tools.strength}</span>
              </label>
              {tools.brush === "paint" && (
                <div className="insp-row">
                  <span className="col-label">Paint</span>
                  <input type="color" value={hex(tools.paint)} onChange={(e) => tools.setPaint([1, 3, 5].map((i) => parseInt(e.target.value.slice(i, i + 2), 16)) as Vec3)} />
                  {(["#6ea05a", "#968058", "#8a8d93", "#e9ecf2", "#d8c48c"] as const).map((c) => (
                    <button key={c} className="swatch" style={{ background: c }} title={c} onClick={() => tools.setPaint([1, 3, 5].map((i) => parseInt(c.slice(i, i + 2), 16)) as Vec3)} />
                  ))}
                </div>
              )}
            </>
          )}
        </>
      )}
      {isMesh(o.kind) && (
        <>
          <div className="insp-row">
            <span className="col-label">Color</span>
            <input type="color" value={hex(o.color ?? [200, 200, 210])} onChange={(e) => change({ color: [1, 3, 5].map((i) => parseInt(e.target.value.slice(i, i + 2), 16)) as Vec3 })} />
          </div>
          <div className="insp-row">
            <span className="col-label">Physics</span>
            <select className="field" value={o.physics ?? "none"} onChange={(e) => change({ physics: e.target.value as SceneObject["physics"] })} title="Static: floors and walls. Dynamic: falls and gets pushed.">
              <option value="none">None</option>
              <option value="static">Static</option>
              <option value="dynamic">Dynamic</option>
            </select>
            {o.physics === "dynamic" && <Num axis="kg" value={o.mass ?? 1} onChange={(mass) => change({ mass: Math.max(0.01, mass) })} />}
          </div>
          <div className="insp-row">
            <span className="col-label" />
            {o.physics === "dynamic" && (
              <label className="check small" title="It won't tip over or spin: for players and characters">
                <input type="checkbox" checked={!!o.upright} onChange={(e) => change({ upright: e.target.checked || undefined })} /> Keep upright
              </label>
            )}
            <label className="check small" title="Invisible in the game but still there: e.g. a player's physics box under its model">
              <input type="checkbox" checked={!!o.hidden} onChange={(e) => change({ hidden: e.target.checked || undefined })} /> Hidden
            </label>
          </div>
        </>
      )}
      <div className="insp-row top">
        <span className="col-label">Behaviours</span>
        <div className="insp-checks">
          {behaviours.length === 0 && <span className="muted small">Make a graph a Behaviour to attach it here.</span>}
          {behaviours.map((b) => (
            <label key={b} className="check small">
              <input
                type="checkbox"
                checked={o.behaviours?.includes(b) ?? false}
                onChange={(e) => change({ behaviours: e.target.checked ? [...(o.behaviours ?? []), b] : (o.behaviours ?? []).filter((x) => x !== b) })}
              />{" "}
              {b}
            </label>
          ))}
        </div>
      </div>
      <div className="insp-row">
        <button className="tool" onClick={duplicate}>Duplicate</button>
        <button className="tool danger" onClick={remove}><Icon name="trash" size={13} /> Delete</button>
      </div>
    </div>
  );
}

/** A number field that saves when you leave it or press Enter. */
function Num({ value, onChange, axis }: { value: number; onChange: (n: number) => void; axis: string }) {
  const [text, setText] = useState(String(value));
  useEffect(() => setText(String(value)), [value]);
  const commit = () => {
    const n = Number(text);
    if (Number.isFinite(n) && n !== value) onChange(n);
    else setText(String(value));
  };
  return (
    <label className="num">
      <span>{axis}</span>
      <input className="field mono" value={text} onChange={(e) => setText(e.target.value)} onBlur={commit} onKeyDown={(e) => e.key === "Enter" && commit()} />
    </label>
  );
}

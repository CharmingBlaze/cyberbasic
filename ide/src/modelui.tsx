// The modeller for .model files: build a model from parts (boxes, spheres, cylinders, cones) hung
// from joints, then animate it: pick a clip, move to a time, pose parts and each pose becomes a key.
// Games build it with CreateModel_<Name>() and play clips with AnimateEntity.
import { useEffect, useMemo, useRef, useState } from "react";
import * as A from "./actions";
import { Icon } from "./icons";
import {
  PART_KINDS, buildModel, createFunction, hasKey, modelText, newPart, parseModel, partTree, poseAt, removeKey, setKey, withChildren,
  type Clip, type Model, type Part, type PartKind, type Pose,
} from "./model";
import { get, set, useStore } from "./store";
import { pickable, setTransform, transformOf, useGizmo, useViewport, type Any, type GizmoMode, type Vec3 } from "./viewport";

type Mode = "model" | "animate";

export function ModelPanel({ file }: { file: string }) {
  const text = useStore((s) => s.project.files[file] ?? "");
  const model = useMemo(() => parseModel(text), [text]);
  const [selected, setSelected] = useState<string>();
  const [gizmo, setGizmo] = useState<GizmoMode>("move");
  const [snap, setSnap] = useState(false);
  const [mode, setMode] = useState<Mode>("model");
  const [clipName, setClipName] = useState<string>();
  const [time, setTime] = useState(0);
  const [playing, setPlaying] = useState(false);
  const canvas = useRef<HTMLCanvasElement>(null);
  const { view, ready, error } = useViewport(canvas, setSelected, { distance: 7, gridSize: 10, targetY: 1.1 });
  const [built, setBuilt] = useState<{ root: Any; parts: Map<string, Any> }>();

  const clip = model.animations.find((c) => c.name === clipName) ?? model.animations[0];
  const animating = mode === "animate" && !!clip;
  const sel = model.parts.find((p) => p.id === selected);
  const latest = () => parseModel(get().project.files[file] ?? "");
  const save = (m: Model) => A.updateVisual(file, modelText(m));
  const changePart = (id: string, patch: Partial<Part>) => {
    const m = latest();
    save({ ...m, parts: m.parts.map((p) => (p.id === id ? { ...p, ...patch } : p)) });
  };
  const changeClip = (f: (c: Clip) => Clip) => {
    const m = latest();
    const c = m.animations.find((x) => x.name === clip?.name);
    if (c) save({ ...m, animations: m.animations.map((x) => (x === c ? f(x) : x)) });
  };
  /** A new transform for a part: its rest pose while modelling, a key at the current time while animating. */
  const commit = (id: string, t: Pose) => (animating ? changeClip((c) => setKey(c, id, { t: round2(time), ...t })) : changePart(id, t));

  // ------------------------------------------------------------ the 3D view

  useEffect(() => {
    const v = view.current;
    if (!v) return;
    const b = buildModel(v.B, v.scene, model);
    for (const [id, n] of b.parts) pickable(n, id);
    setBuilt(b);
    return () => b.root.dispose();
  }, [model, ready]);

  // Pose: rest while modelling, the clip at `time` while animating.
  useEffect(() => {
    const v = view.current;
    if (!v || !built) return;
    for (const [id, p] of poseAt(model, animating ? clip : undefined, time)) {
      const n = built.parts.get(id);
      if (n) setTransform(v.B, n, p.position, p.rotation, p.scale);
    }
  }, [built, animating, clip, time]);

  useGizmo(view, selected ? built?.parts.get(selected) : undefined, { mode: gizmo, snap, canScale: true }, (node) => selected && commit(selected, transformOf(node)), [built, animating, time]);

  // Playback.
  useEffect(() => {
    if (!playing || !clip) return;
    let last = performance.now(), raf = 0;
    const tick = (now: number) => {
      const len = clip.length || 1;
      setTime((t) => {
        const next = t + (now - last) / 1000;
        return clip.loop ? next % len : Math.min(next, len);
      });
      last = now;
      raf = requestAnimationFrame(tick);
    };
    raf = requestAnimationFrame(tick);
    return () => cancelAnimationFrame(raf);
  }, [playing, clip]);
  useEffect(() => {
    if (playing && clip && !clip.loop && time >= clip.length) setPlaying(false);
  }, [time]);

  // ------------------------------------------------------------ editing

  const add = (kind: PartKind) => {
    const m = latest();
    // New parts hang from the selected joint (or the selected part's joint), so they move with it.
    const under = sel?.kind === "joint" ? sel.id : sel?.parent;
    const p = newPart(m, kind, under);
    save({ ...m, parts: [...m.parts, p] });
    setSelected(p.id);
  };
  const remove = () => {
    if (!selected) return;
    const m = latest();
    const gone = withChildren(m, selected);
    const animations = m.animations.map((c) => ({ ...c, tracks: Object.fromEntries(Object.entries(c.tracks).filter(([id]) => !gone.has(id))) }));
    save({ ...m, parts: m.parts.filter((p) => !gone.has(p.id)), animations });
    setSelected(undefined);
  };
  const duplicate = () => {
    const m = latest();
    const p = m.parts.find((x) => x.id === selected);
    if (!p) return;
    const fresh = newPart(m, p.kind, p.parent);
    const copy: Part = { ...structuredClone(p), id: fresh.id, name: fresh.name, position: [p.position[0] + 0.3, p.position[1], p.position[2]] };
    save({ ...m, parts: [...m.parts, copy] });
    setSelected(copy.id);
  };
  const newClip = () => {
    const name = prompt("Name of the animation (AnimateEntity plays it by this name):", model.animations.length ? "" : "walk")?.trim();
    if (!name) return;
    const m = latest();
    if (m.animations.some((c) => c.name.toLowerCase() === name.toLowerCase())) return A.toast(`There's already an animation called ${name}`, "error");
    save({ ...m, animations: [...m.animations, { name, length: 1, loop: true, tracks: {} }] });
    setClipName(name);
    setMode("animate");
    setTime(0);
  };
  const renameClip = () => {
    if (!clip) return;
    const name = prompt("New name for the animation:", clip.name)?.trim();
    if (!name || name === clip.name) return;
    if (model.animations.some((c) => c.name.toLowerCase() === name.toLowerCase())) return A.toast(`There's already an animation called ${name}`, "error");
    changeClip((c) => ({ ...c, name }));
    setClipName(name);
  };
  const deleteClip = () => {
    if (!clip || !confirm(`Delete the animation "${clip.name}"?`)) return;
    const m = latest();
    save({ ...m, animations: m.animations.filter((c) => c.name !== clip.name) });
    setClipName(undefined);
  };
  const frame = () => {
    const n = selected && built?.parts.get(selected);
    if (n && view.current) view.current.camera.setTarget(n.getAbsolutePosition().clone());
  };
  const onKeyDown = (e: React.KeyboardEvent) => {
    if ((e.target as HTMLElement).closest("input, select, textarea")) return;
    const k = e.key.toLowerCase(), ctrl = e.ctrlKey || e.metaKey;
    if (k === "w" && !ctrl) setGizmo("move");
    else if (k === "e" && !ctrl) setGizmo("rotate");
    else if (k === "r" && !ctrl) setGizmo("scale");
    else if (k === "f" && !ctrl) frame();
    else if (k === " " && animating) setPlaying((p) => !p);
    else if (k === "k" && animating && sel) commit(sel.id, currentPose(sel.id));
    else if ((k === "delete" || k === "backspace") && !animating) remove();
    else if (ctrl && k === "d" && !animating) duplicate();
    else if (k === "escape") setSelected(undefined);
    else return;
    e.preventDefault();
  };
  const currentPose = (id: string): Pose => poseAt(model, animating ? clip : undefined, time).get(id)!;
  const tree = partTree(model);
  const shown = sel ? currentPose(sel.id) : undefined;

  return (
    <div className="panel scene-panel" onMouseDown={() => set({ activeFile: file })} onKeyDown={onKeyDown}>
      <div className="panel-bar vis-bar">
        <Icon name="cube" size={15} className="vis-icon" />
        <strong>{file}</strong>
        <span className="muted small ellipsis">Model · make one in code with {createFunction(file)}(), play a clip with AnimateEntity e, "name"</span>
        <span className="grow" />
        <div className="seg small" title="Model: shape the parts. Animate: pose them over time.">
          <button className={mode === "model" ? "on" : ""} onClick={() => (setMode("model"), setPlaying(false))}>Model</button>
          <button className={mode === "animate" ? "on" : ""} onClick={() => (model.animations.length ? setMode("animate") : newClip())}>Animate</button>
        </div>
        <div className="seg small" title="Gizmo: W move, E rotate, R scale">
          {(["move", "rotate", "scale"] as GizmoMode[]).map((m) => (
            <button key={m} className={gizmo === m ? "on" : ""} onClick={() => setGizmo(m)}>{m[0].toUpperCase() + m.slice(1)}</button>
          ))}
        </div>
        <label className="check small" title="Snap moves to 0.5, turns to 15° and scaling to 0.25">
          <input type="checkbox" checked={snap} onChange={(e) => setSnap(e.target.checked)} /> Snap
        </label>
      </div>
      <div className="scene-body">
        <div className="scene-side">
          <div className="scene-add">
            {PART_KINDS.map((k) => (
              <button key={k.kind} className="tool" disabled={animating} onClick={() => add(k.kind)} title={k.kind === "joint" ? "An invisible pivot: parts hung from it turn around it (shoulders, knees)" : `Add a ${k.label.toLowerCase()}`}>
                + {k.label}
              </button>
            ))}
          </div>
          <div className="list scene-list">
            {tree.map(({ part: p, depth }) => (
              <button
                key={p.id}
                className={"row scene-row" + (p.id === selected ? " active" : "")}
                style={{ paddingLeft: 10 + depth * 14 }}
                onClick={() => setSelected(p.id)}
                onDoubleClick={() => (setSelected(p.id), setTimeout(frame))}
              >
                <span className={`kind-dot k-${p.kind === "joint" ? "light" : "cube"}`} />
                <span className="ellipsis">{p.name || "(no name)"}</span>
                {animating && clip?.tracks[p.id]?.length ? <span className="muted small">◆ {clip.tracks[p.id].length}</span> : null}
              </button>
            ))}
          </div>
        </div>
        <div className="scene-view model-view">
          <canvas ref={canvas} tabIndex={0} />
          {error && <div className="graph-empty">{error}</div>}
          <div className="scene-hint muted small">
            {animating
              ? `Animating "${clip.name}": move the time, pose parts (each pose is a key) · K keys the selected part · Space plays`
              : "Drag to orbit · right-drag to pan · wheel to zoom · click a part · W/E/R gizmo · F frame · Ctrl+D duplicate · Del delete"}
          </div>
          {mode === "animate" && (
            <Timeline
              model={model}
              clip={clip}
              part={selected}
              time={time}
              playing={playing}
              setTime={(t) => (setPlaying(false), setTime(t))}
              setPlaying={setPlaying}
              pickClip={(n) => (setClipName(n), setTime(0), setPlaying(false))}
              newClip={newClip}
              renameClip={renameClip}
              deleteClip={deleteClip}
              changeClip={changeClip}
              keyPart={() => sel && commit(sel.id, currentPose(sel.id))}
              unkeyPart={() => sel && changeClip((c) => removeKey(c, sel.id, time))}
            />
          )}
        </div>
        <div className="scene-inspector">
          {sel && shown ? (
            <PartInspector key={sel.id + mode} model={model} p={sel} pose={shown} animating={animating} change={(patch) => changePart(sel.id, patch)} pose_={(t) => commit(sel.id, t)} remove={remove} duplicate={duplicate} />
          ) : (
            <div className="empty muted small">Select a part to edit it. Add parts on the left; hang them from joints so they turn around the joint.</div>
          )}
        </div>
      </div>
    </div>
  );
}

const round2 = (t: number) => Math.round(t * 100) / 100;

function Timeline(props: {
  model: Model; clip: Clip; part?: string; time: number; playing: boolean;
  setTime: (t: number) => void; setPlaying: (p: boolean) => void; pickClip: (name: string) => void;
  newClip: () => void; renameClip: () => void; deleteClip: () => void; changeClip: (f: (c: Clip) => Clip) => void;
  keyPart: () => void; unkeyPart: () => void;
}) {
  const { model, clip, part, time } = props;
  const len = clip.length || 1;
  const keys = [...new Set(Object.entries(clip.tracks).flatMap(([id, ks]) => (part && id !== part ? [] : ks.map((k) => k.t))))];
  const keyed = hasKey(clip, part, time);
  return (
    <div className="timeline">
      <div className="timeline-bar">
        <select className="field" value={clip.name} onChange={(e) => props.pickClip(e.target.value)} title="Animation">
          {model.animations.map((c) => <option key={c.name}>{c.name}</option>)}
        </select>
        <button className="icon-btn" title="New animation" onClick={props.newClip}><Icon name="plus" size={13} /></button>
        <button className="tool" onClick={props.renameClip}>Rename</button>
        <button className="icon-btn" title="Delete this animation" onClick={props.deleteClip}><Icon name="trash" size={13} /></button>
        <span className="sep" />
        <button className="tool" onClick={() => props.setPlaying(!props.playing)} title="Play / pause (Space)">
          <Icon name={props.playing ? "pause" : "play"} size={12} /> {props.playing ? "Pause" : "Play"}
        </button>
        <span className="mono small time">{time.toFixed(2)}s</span>
        <span className="grow" />
        <label className="small muted">Length <Len value={len} onChange={(length) => props.changeClip((c) => ({ ...c, length }))} />s</label>
        <label className="check small"><input type="checkbox" checked={clip.loop} onChange={(e) => props.changeClip((c) => ({ ...c, loop: e.target.checked }))} /> Loop</label>
        <button className={"tool" + (keyed ? " on" : "")} disabled={!part} onClick={props.keyPart} title="Key the selected part's pose at this time (K)">◆ Key</button>
        <button className="tool" disabled={!keyed} onClick={props.unkeyPart} title="Remove the selected part's key at this time">Remove key</button>
      </div>
      <div className="timeline-track">
        <input type="range" min={0} max={len} step={0.01} value={Math.min(time, len)} onChange={(e) => props.setTime(Number(e.target.value))} />
        {keys.map((t) => (
          <button key={t} className={"key-mark" + (Math.abs(t - time) < 0.005 ? " at" : "")} style={{ left: `calc(${(t / len) * 100}% - 5px)` }} title={`${t}s`} onClick={() => props.setTime(t)} />
        ))}
      </div>
    </div>
  );
}

function Len({ value, onChange }: { value: number; onChange: (n: number) => void }) {
  const [text, setText] = useState(String(value));
  useEffect(() => setText(String(value)), [value]);
  const commit = () => {
    const n = Number(text);
    if (Number.isFinite(n) && n > 0 && n !== value) onChange(Math.round(n * 100) / 100);
    else setText(String(value));
  };
  return <input className="field mono len" value={text} onChange={(e) => setText(e.target.value)} onBlur={commit} onKeyDown={(e) => e.key === "Enter" && commit()} />;
}

function PartInspector({ model, p, pose, animating, change, pose_, remove, duplicate }: {
  model: Model; p: Part; pose: Pose; animating: boolean;
  change: (patch: Partial<Part>) => void; pose_: (t: Pose) => void; remove: () => void; duplicate: () => void;
}) {
  const vec = (label: string, key: keyof Pose) => (
    <div className="insp-row">
      <span className="col-label">{label}</span>
      <div className="vec">
        {[0, 1, 2].map((i) => (
          <Num key={i} axis={"xyz"[i]} value={pose[key][i]} onChange={(n) => pose_({ ...pose, [key]: pose[key].map((x, j) => (j === i ? n : x)) as Vec3 })} />
        ))}
      </div>
    </div>
  );
  const hex = (c: Vec3) => "#" + c.map((x) => Math.round(x).toString(16).padStart(2, "0")).join("");
  const below = withChildren(model, p.id);
  return (
    <div className="inspector">
      <div className="insp-row">
        <span className="col-label">Name</span>
        <input className="field wide" value={p.name} spellCheck={false} disabled={animating} onChange={(e) => change({ name: e.target.value })} />
      </div>
      <div className="muted small insp-kind">{PART_KINDS.find((k) => k.kind === p.kind)?.label}{animating ? " · posing keys" : ""}</div>
      <div className="insp-row">
        <span className="col-label">Hangs from</span>
        <select className="field wide" value={p.parent ?? ""} disabled={animating} onChange={(e) => change({ parent: e.target.value || undefined })} title="It moves and turns with this part">
          <option value="">(nothing)</option>
          {model.parts.filter((x) => !below.has(x.id)).map((x) => <option key={x.id} value={x.id}>{x.name}</option>)}
        </select>
      </div>
      {vec("Position", "position")}
      {vec("Rotation", "rotation")}
      {vec("Scale", "scale")}
      {p.kind !== "joint" && (
        <div className="insp-row">
          <span className="col-label">Color</span>
          <input type="color" disabled={animating} value={hex(p.color ?? [200, 200, 210])} onChange={(e) => change({ color: [1, 3, 5].map((i) => parseInt(e.target.value.slice(i, i + 2), 16)) as Vec3 })} />
        </div>
      )}
      {!animating && (
        <div className="insp-row">
          <button className="tool" onClick={duplicate}>Duplicate</button>
          <button className="tool danger" onClick={remove}><Icon name="trash" size={13} /> Delete</button>
        </div>
      )}
    </div>
  );
}

function Num({ value, onChange, axis }: { value: number; onChange: (n: number) => void; axis: string }) {
  const shown = String(Math.round(value * 1000) / 1000);
  const [text, setText] = useState(shown);
  useEffect(() => setText(shown), [shown]);
  const commit = () => {
    const n = Number(text);
    if (Number.isFinite(n) && String(n) !== shown) onChange(n);
    else setText(shown);
  };
  return (
    <label className="num">
      <span>{axis}</span>
      <input className="field mono" value={text} onChange={(e) => setText(e.target.value)} onBlur={commit} onKeyDown={(e) => e.key === "Enter" && commit()} />
    </label>
  );
}

// Models (.model files) from the modeller: parts and joints in a hierarchy, plus animation clips.
// The compiler turns `hero.model` into `CreateModel_Hero()` (crates/cyberbasic/src/model.rs);
// the runtime's CreateModel builds the same thing `buildModel` builds here.
import type { Any, Vec3 } from "./viewport";

export type PartKind = "box" | "sphere" | "cylinder" | "cone" | "joint";
export interface Part {
  id: string;
  name: string;
  kind: PartKind;
  /** The part this one hangs from (moves with). */
  parent?: string;
  position: Vec3;
  /** Degrees, like RotateEntity. */
  rotation: Vec3;
  scale: Vec3;
  color?: Vec3;
}
/** A part's whole transform at a time (seconds) in a clip. */
export interface Key { t: number; position: Vec3; rotation: Vec3; scale: Vec3 }
export interface Clip { name: string; length: number; loop: boolean; tracks: Record<string, Key[]> }
export interface Model { version: 1; parts: Part[]; animations: Clip[] }
export type Pose = { position: Vec3; rotation: Vec3; scale: Vec3 };

export const PART_KINDS: { kind: PartKind; label: string }[] = [
  { kind: "box", label: "Box" },
  { kind: "sphere", label: "Sphere" },
  { kind: "cylinder", label: "Cylinder" },
  { kind: "cone", label: "Cone" },
  { kind: "joint", label: "Joint" },
];

export function parseModel(text: string): Model {
  try {
    const m = JSON.parse(text) as Partial<Model>;
    return { version: 1, parts: m.parts ?? [], animations: m.animations ?? [] };
  } catch {
    return { version: 1, parts: [], animations: [] };
  }
}
export const modelText = (m: Model) => JSON.stringify(m, null, 2) + "\n";
export const nextPartId = (m: Model) => `p${Math.max(0, ...m.parts.map((p) => Number(p.id.slice(1)) || 0)) + 1}`;
/** `hero.model` -> `CreateModel_Hero`, the function that builds it in a game. */
export const createFunction = (file: string) => {
  const base = (file.split(/[/\\]/).pop() ?? file).replace(/\.model$/i, "").replace(/\W/g, "_");
  return `CreateModel_${base.charAt(0).toUpperCase()}${base.slice(1)}`;
};

export function newPart(m: Model, kind: PartKind, parent?: string): Part {
  const label = PART_KINDS.find((k) => k.kind === kind)!.label;
  let n = 1;
  while (m.parts.some((p) => p.name.toLowerCase() === `${label} ${n}`.toLowerCase())) n++;
  return { id: nextPartId(m), name: `${label} ${n}`, kind, parent, position: parent ? [0, 0, 0] : [0, 0.5, 0], rotation: [0, 0, 0], scale: kind === "joint" ? [1, 1, 1] : [0.5, 0.5, 0.5], ...(kind === "joint" ? {} : { color: [200, 200, 210] as Vec3 }) };
}

/** Parts in tree order (each followed by its children), with their depth. */
export function partTree(m: Model): { part: Part; depth: number }[] {
  const out: { part: Part; depth: number }[] = [];
  const ids = new Set(m.parts.map((p) => p.id));
  const walk = (parent: string | undefined, depth: number) => {
    for (const p of m.parts) if ((p.parent && ids.has(p.parent) ? p.parent : undefined) === parent) {
      out.push({ part: p, depth });
      walk(p.id, depth + 1);
    }
  };
  walk(undefined, 0);
  return out;
}
/** A part and everything hanging from it. */
export function withChildren(m: Model, id: string): Set<string> {
  const out = new Set([id]);
  for (let grew = true; grew; ) {
    grew = false;
    for (const p of m.parts) if (p.parent && out.has(p.parent) && !out.has(p.id)) (out.add(p.id), (grew = true));
  }
  return out;
}

// ---------------------------------------------------------------- animation

const lerp3 = (a: Vec3, b: Vec3, f: number) => [0, 1, 2].map((i) => a[i] + (b[i] - a[i]) * f) as Vec3;
/** A track's pose at time t (as the runtime samples it). */
export function sampleKeys(keys: Key[], t: number): Pose {
  if (t <= keys[0].t) return keys[0];
  for (let i = 0; i < keys.length - 1; i++) {
    const a = keys[i], b = keys[i + 1];
    if (t < b.t) {
      const f = (t - a.t) / (b.t - a.t || 1);
      return { position: lerp3(a.position, b.position, f), rotation: lerp3(a.rotation, b.rotation, f), scale: lerp3(a.scale, b.scale, f) };
    }
  }
  return keys[keys.length - 1];
}
/** Every part's pose at time t in a clip (parts without keys stay at rest). */
export function poseAt(m: Model, clip: Clip | undefined, t: number): Map<string, Pose> {
  const out = new Map<string, Pose>(m.parts.map((p) => [p.id, p]));
  if (clip) for (const [id, keys] of Object.entries(clip.tracks)) if (keys.length && out.has(id)) out.set(id, sampleKeys(keys, t));
  return out;
}
const SAME = 0.005; // keys closer than this are the same key
/** The clip with a part's key at t set (replacing one already there). */
export function setKey(clip: Clip, id: string, key: Key): Clip {
  const keys = (clip.tracks[id] ?? []).filter((k) => Math.abs(k.t - key.t) > SAME);
  return { ...clip, tracks: { ...clip.tracks, [id]: [...keys, key].sort((a, b) => a.t - b.t) } };
}
export function removeKey(clip: Clip, id: string, t: number): Clip {
  const keys = (clip.tracks[id] ?? []).filter((k) => Math.abs(k.t - t) > SAME);
  const tracks = { ...clip.tracks };
  if (keys.length) tracks[id] = keys;
  else delete tracks[id];
  return { ...clip, tracks };
}
export const hasKey = (clip: Clip | undefined, id: string | undefined, t: number) => !!(clip && id && clip.tracks[id]?.some((k) => Math.abs(k.t - t) <= SAME));

// ---------------------------------------------------------------- building (editor view)

/** Builds the model in a Babylon scene, as the runtime does. Returns the root and each part's node. */
export function buildModel(B: Any, scene: Any, m: Model, name = "model", markers = true) {
  const root = new B.TransformNode(name, scene);
  const parts = new Map<string, Any>();
  for (const p of m.parts) {
    let n: Any;
    if (p.kind === "joint" && !markers) n = new B.TransformNode(p.id, scene);
    else if (p.kind === "joint") {
      // Joints are invisible in games; the editor shows a small marker.
      n = B.MeshBuilder.CreateSphere(p.id, { diameter: 0.14, segments: 8 }, scene);
      const mat = new B.StandardMaterial(p.id, scene);
      mat.emissiveColor = new B.Color3(1, 0.75, 0.25);
      mat.disableLighting = true;
      n.material = mat;
      n.renderingGroupId = 1; // drawn on top, so joints inside parts can be clicked
    } else {
      n =
        p.kind === "box" ? B.MeshBuilder.CreateBox(p.id, { size: 1 }, scene)
        : p.kind === "sphere" ? B.MeshBuilder.CreateSphere(p.id, { diameter: 1, segments: 16 }, scene)
        : p.kind === "cylinder" ? B.MeshBuilder.CreateCylinder(p.id, { height: 1, diameter: 1, tessellation: 24 }, scene)
        : B.MeshBuilder.CreateCylinder(p.id, { height: 1, diameterTop: 0, diameterBottom: 1, tessellation: 24 }, scene);
      const mat = new B.StandardMaterial(p.id, scene);
      const c = p.color ?? [200, 200, 210];
      mat.diffuseColor = new B.Color3(c[0] / 255, c[1] / 255, c[2] / 255);
      n.material = mat;
    }
    parts.set(p.id, n);
  }
  for (const p of m.parts) parts.get(p.id).parent = parts.get(p.parent ?? "") ?? root;
  return { root, parts };
}

// ---------------------------------------------------------------- a starter model

/** A little robot with joints at the shoulders and hips, and a walk cycle to start from. */
export function emptyModel(): Model {
  const parts: Part[] = [];
  const add = (id: string, name: string, kind: PartKind, parent: string | undefined, position: Vec3, scale: Vec3 = [1, 1, 1], color?: Vec3) =>
    parts.push({ id, name, kind, ...(parent ? { parent } : {}), position, rotation: [0, 0, 0], scale, ...(color ? { color } : {}) });
  const body: Vec3 = [76, 194, 255], limb: Vec3 = [60, 70, 110];
  add("p1", "Hips", "joint", undefined, [0, 1.1, 0]);
  add("p2", "Body", "box", "p1", [0, 0.6, 0], [0.9, 1.1, 0.55], body);
  add("p3", "Head", "box", "p1", [0, 1.45, 0], [0.6, 0.55, 0.55], [230, 235, 245]);
  add("p4", "Left shoulder", "joint", "p1", [-0.6, 1.05, 0]);
  add("p5", "Left arm", "box", "p4", [0, -0.4, 0], [0.25, 0.85, 0.25], limb);
  add("p6", "Right shoulder", "joint", "p1", [0.6, 1.05, 0]);
  add("p7", "Right arm", "box", "p6", [0, -0.4, 0], [0.25, 0.85, 0.25], limb);
  add("p8", "Left hip", "joint", "p1", [-0.22, 0, 0]);
  add("p9", "Left leg", "box", "p8", [0, -0.5, 0], [0.3, 1, 0.3], limb);
  add("p10", "Right hip", "joint", "p1", [0.22, 0, 0]);
  add("p11", "Right leg", "box", "p10", [0, -0.5, 0], [0.3, 1, 0.3], limb);
  const swing = (id: string, a: number) => {
    const p = parts.find((x) => x.id === id)!;
    const key = (t: number, x: number): Key => ({ t, position: p.position, rotation: [x, 0, 0], scale: [1, 1, 1] });
    return [key(0, a), key(0.4, -a), key(0.8, a)];
  };
  const walk: Clip = { name: "walk", length: 0.8, loop: true, tracks: { p4: swing("p4", 35), p6: swing("p6", -35), p8: swing("p8", -30), p10: swing("p10", 30) } };
  const hips = parts[0];
  const bob = (t: number, y: number): Key => ({ t, position: [0, y, 0], rotation: [0, 0, 0], scale: [1, 1, 1] });
  const idle: Clip = { name: "idle", length: 2, loop: true, tracks: { p1: [bob(0, 1.1), bob(1, hips.position[1] + 0.05), bob(2, 1.1)] } };
  return { version: 1, parts, animations: [walk, idle] };
}

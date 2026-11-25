// Scenes (.scene files): levels built in the level editor. Stored as JSON; the compiler turns
// `level.scene` into `Function LoadScene_Level()` (crates/cyberbasic/src/scene.rs).

export type SceneKind = "cube" | "sphere" | "plane" | "terrain" | "light" | "pointlight" | "camera" | "model";
export type Vec3 = [number, number, number];

export interface SceneObject {
  id: string;
  name: string;
  kind: SceneKind;
  /** Asset path, for models. */
  model?: string;
  position: Vec3;
  /** Degrees: pitch, yaw, roll (as RotateEntity). */
  rotation: Vec3;
  scale: Vec3;
  color?: Vec3;
  physics?: "none" | "static" | "dynamic";
  mass?: number;
  behaviours?: string[];
  /** Terrains: width and depth, squares per side, each point's height ("0,0.5,...") and color (6 hex digits each). */
  size?: number;
  divisions?: number;
  heights?: string;
  colors?: string;
}
export interface Scene { version: 1; objects: SceneObject[] }

export const KINDS: { kind: SceneKind; label: string }[] = [
  { kind: "cube", label: "Cube" },
  { kind: "sphere", label: "Sphere" },
  { kind: "plane", label: "Plane" },
  { kind: "terrain", label: "Terrain" },
  { kind: "light", label: "Light" },
  { kind: "pointlight", label: "Point light" },
  { kind: "camera", label: "Camera" },
  { kind: "model", label: "Model" },
];
export const isMesh = (k: SceneKind) => k === "cube" || k === "sphere" || k === "plane" || k === "model";

export function parseScene(text: string): Scene {
  try {
    const s = JSON.parse(text) as Partial<Scene>;
    return { version: 1, objects: s.objects ?? [] };
  } catch {
    return { version: 1, objects: [] };
  }
}
export const sceneText = (s: Scene) => JSON.stringify(s, null, 2) + "\n";
export const nextObjectId = (s: Scene) => `o${Math.max(0, ...s.objects.map((o) => Number(o.id.slice(1)) || 0)) + 1}`;
/** `level.scene` -> `LoadScene_Level`, the function that builds it. */
export const loadFunction = (file: string) => {
  const base = file.replace(/\.scene$/i, "").replace(/\W/g, "_");
  return `LoadScene_${base.charAt(0).toUpperCase()}${base.slice(1)}`;
};

/** A new object of a kind, named so FindEntity can find it ("Cube 2"). */
export function newObject(s: Scene, kind: SceneKind, at: Vec3 = [0, 0, 0], model?: string): SceneObject {
  const label = KINDS.find((k) => k.kind === kind)!.label;
  let n = 1;
  while (s.objects.some((o) => o.name.toLowerCase() === `${label} ${n}`.toLowerCase())) n++;
  const o: SceneObject = { id: nextObjectId(s), name: `${label} ${n}`, kind, position: at, rotation: [0, 0, 0], scale: [1, 1, 1] };
  if (kind === "plane") o.scale = [10, 1, 10];
  if (kind === "light") o.rotation = [50, 30, 0];
  if (isMesh(kind)) o.color = [200, 200, 210];
  if (kind === "model") o.model = model;
  if (kind === "terrain") Object.assign(o, { position: [at[0], 0, at[2]], size: 40, divisions: 64, heights: "", colors: "", physics: "static" });
  return o;
}

/** A starter level: ground, a player cube, a sun and a camera. */
export const emptyScene = (): Scene => ({
  version: 1,
  objects: [
    { id: "o1", name: "Ground", kind: "plane", position: [0, 0, 0], rotation: [0, 0, 0], scale: [20, 1, 20], color: [70, 140, 90], physics: "static" },
    { id: "o2", name: "Player", kind: "cube", position: [0, 0.5, 0], rotation: [0, 0, 0], scale: [1, 1, 1], color: [76, 194, 255] },
    { id: "o3", name: "Sun", kind: "light", position: [0, 8, 0], rotation: [50, 30, 0], scale: [1, 1, 1] },
    { id: "o4", name: "Camera", kind: "camera", position: [0, 6, -12], rotation: [25, 0, 0], scale: [1, 1, 1] },
  ],
});

// ---------------------------------------------------------------- terrain data

export const TERRAIN_COLOR = 0x6ea05a; // unpainted grass, as the runtime's CreateTerrain

/** A terrain's heights from its saved text (missing points are 0). */
export function decodeHeights(text: string, count: number) {
  const out = new Float32Array(count), parts = text ? text.split(",") : [];
  for (let i = 0; i < count; i++) out[i] = Number(parts[i]) || 0;
  return out;
}
/** Heights as saved text; "" when the terrain is flat. */
export function encodeHeights(h: ArrayLike<number>) {
  const parts = Array.from(h, (v) => String(Math.round(v * 100) / 100));
  return parts.every((p) => p === "0") ? "" : parts.join(",");
}
/** RGB 0-255 per point. */
export function decodeColors(text: string, count: number) {
  const out = new Uint8Array(count * 3);
  for (let i = 0; i < count; i++) {
    const v = parseInt(text.substr(i * 6, 6), 16), c = Number.isNaN(v) ? TERRAIN_COLOR : v;
    out.set([(c >> 16) & 255, (c >> 8) & 255, c & 255], i * 3);
  }
  return out;
}
/** Colors as saved text; "" when nothing has been painted. */
export function encodeColors(c: Uint8Array) {
  let s = "";
  for (let i = 0; i < c.length; i += 3) s += ((c[i] << 16) | (c[i + 1] << 8) | c[i + 2]).toString(16).padStart(6, "0");
  const plain = TERRAIN_COLOR.toString(16).padStart(6, "0");
  return s === plain.repeat(c.length / 3) ? "" : s;
}

export type Brush = "raise" | "lower" | "smooth" | "flatten" | "paint";
/**
 * Applies a brush dab at local (x, z) to a (n+1) x (n+1) grid of points. `pos` is the mesh's
 * x,y,z per point; heights change in pos, paint in `rgb`. Returns whether anything changed.
 */
export function dab(pos: Float32Array | number[], rgb: Uint8Array, n: number, x: number, z: number, brush: { tool: Brush; radius: number; strength: number; color: Vec3; target: number }) {
  const { tool, radius, strength, color, target } = brush, side = n + 1, count = side * side;
  const before = tool === "smooth" ? Float32Array.from({ length: count }, (_, i) => pos[i * 3 + 1]) : undefined;
  let changed = false;
  for (let i = 0; i < count; i++) {
    const d = Math.hypot(pos[i * 3] - x, pos[i * 3 + 2] - z);
    if (d >= radius) continue;
    const f = (0.5 + 0.5 * Math.cos((Math.PI * d) / radius)) * strength; // soft edge
    const y = pos[i * 3 + 1];
    changed = true;
    if (tool === "raise") pos[i * 3 + 1] = y + f * 0.3;
    else if (tool === "lower") pos[i * 3 + 1] = y - f * 0.3;
    else if (tool === "flatten") pos[i * 3 + 1] = y + (target - y) * f;
    else if (tool === "smooth") {
      const r = Math.floor(i / side), c = i % side;
      let sum = 0, k = 0;
      for (const [dr, dc] of [[-1, 0], [1, 0], [0, -1], [0, 1]]) {
        const rr = r + dr, cc = c + dc;
        if (rr >= 0 && rr < side && cc >= 0 && cc < side) (sum += before![rr * side + cc]), k++;
      }
      pos[i * 3 + 1] = y + (sum / k - y) * f;
    } else for (let j = 0; j < 3; j++) rgb[i * 3 + j] = Math.round(rgb[i * 3 + j] + (color[j] - rgb[i * 3 + j]) * Math.min(1, f * 1.5));
  }
  return changed;
}

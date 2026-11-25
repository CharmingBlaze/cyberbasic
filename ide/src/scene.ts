// Scenes (.scene files): levels built in the level editor. Stored as JSON; the compiler turns
// `level.scene` into `Function LoadScene_Level()` (crates/cyberbasic/src/scene.rs).

export type SceneKind = "cube" | "sphere" | "plane" | "light" | "pointlight" | "camera" | "model";
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
}
export interface Scene { version: 1; objects: SceneObject[] }

export const KINDS: { kind: SceneKind; label: string }[] = [
  { kind: "cube", label: "Cube" },
  { kind: "sphere", label: "Sphere" },
  { kind: "plane", label: "Plane" },
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

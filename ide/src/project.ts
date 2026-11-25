// Projects: templates, building the game page, and folders on disk (File System Access API).
import runtimeJs from "../../runtime/runtime.js?raw";
import pageTemplate from "../../runtime/index.html?raw";
import { LIB_URLS } from "./libs";
import type { Project } from "./store";

const exampleSources = import.meta.glob("../../examples/*.cb", { query: "?raw", import: "default", eager: true }) as Record<string, string>;
const shooterFiles = import.meta.glob("../../examples/shooter/*.cb", { query: "?raw", import: "default", eager: true }) as Record<string, string>;
const platformerFiles = import.meta.glob("../../examples/platformer/*.{cb,scene,model,graph}", { query: "?raw", import: "default", eager: true }) as Record<string, string>;
const robotFiles = import.meta.glob("../../examples/robot/*.{cb,scene,model}", { query: "?raw", import: "default", eager: true }) as Record<string, string>;
const terrainFiles = import.meta.glob("../../examples/terrain/*.{cb,scene}", { query: "?raw", import: "default", eager: true }) as Record<string, string>;
const levelFiles = import.meta.glob("../../examples/level/*.{cb,scene,graph}", { query: "?raw", import: "default", eager: true }) as Record<string, string>;
const behaviourFiles = import.meta.glob("../../examples/behaviours/*.graph", { query: "?raw", import: "default", eager: true }) as Record<string, string>;
const bouncerFiles = import.meta.glob("../../examples/bouncer/*.graph", { query: "?raw", import: "default", eager: true }) as Record<string, string>;
const byName = (files: Record<string, string>) => Object.fromEntries(Object.entries(files).map(([path, src]) => [path.split("/").pop()!, src]));
const exampleAssetUrls = import.meta.glob("../../examples/assets/*", { query: "?url", import: "default", eager: true }) as Record<string, string>;

/** The example assets, keyed the way code refers to them ("assets/ship.svg"). */
export const EXAMPLE_ASSETS = Object.fromEntries(
  Object.entries(exampleAssetUrls).map(([path, url]) => ["assets/" + path.split("/").pop(), url]),
);
const example = (name: string) => exampleSources[`../../examples/${name}.cb`] ?? "";

const EMPTY_2D = `' My Game
Game "My Game"
Graphics 800, 600

x = 400 : y = 300

While Not KeyHit(KEY_ESCAPE)
    If KeyDown(KEY_LEFT) Then x -= 300 * Delta
    If KeyDown(KEY_RIGHT) Then x += 300 * Delta
    If KeyDown(KEY_UP) Then y -= 300 * Delta
    If KeyDown(KEY_DOWN) Then y += 300 * Delta

    Cls
    Color 76, 194, 255
    Rect x - 20, y - 20, 40, 40
    Color 255, 255, 255
    Text 10, 10, "Arrow keys move. Esc quits."
    Sync
Wend
`;

const EMPTY_3D = `' My 3D Game
Game "My 3D Game"
Graphics3D 1280, 720

camera = CreateCamera()
PositionEntity camera, 0, 4, -8
PointEntity camera, 0, 0, 0
light = CreateLight()

ground = CreatePlane(40)
EntityColor ground, 40, 44, 66
cube = CreateCube()
PositionEntity cube, 0, 0.5, 0
EntityColor cube, 76, 194, 255

While Not KeyHit(KEY_ESCAPE)
    If KeyDown(KEY_LEFT) Then TurnEntity cube, 0, -120 * Delta, 0
    If KeyDown(KEY_RIGHT) Then TurnEntity cube, 0, 120 * Delta, 0
    If KeyDown(KEY_UP) Then MoveEntity cube, 0, 0, 4 * Delta
    If KeyDown(KEY_DOWN) Then MoveEntity cube, 0, 0, -4 * Delta
    Text 10, 10, "Arrow keys drive. Esc quits."
    Sync
Wend
`;

export interface Template {
  id: string;
  name: string;
  kind: "2D" | "3D";
  description: string;
  /** Source files by name; main.cb runs. */
  files: Record<string, string>;
}
const one = (source: string) => ({ "main.cb": source });

export const TEMPLATES: Template[] = [
  { id: "empty2d", name: "Empty 2D", kind: "2D", description: "A screen, a game loop and a box you can move.", files: one(EMPTY_2D) },
  { id: "empty3d", name: "Empty 3D", kind: "3D", description: "Camera, light, floor and a cube you can drive.", files: one(EMPTY_3D) },
  { id: "pong", name: "Pong", kind: "2D", description: "Two-player classic: functions, globals, collision.", files: one(example("pong")) },
  { id: "breakout", name: "Breakout", kind: "2D", description: "Types, For Each, arrays and mouse control.", files: one(example("breakout")) },
  {
    id: "shooter",
    name: "Star Shooter",
    kind: "2D",
    description: "A game split across files with Include: player, enemies, main loop.",
    files: byName(shooterFiles),
  },
  {
    id: "bouncer",
    name: "Bouncer (nodes)",
    kind: "2D",
    description: "Made with the visual node editor: wires instead of code.",
    files: byName(bouncerFiles),
  },
  {
    id: "level",
    name: "Level (3D)",
    kind: "3D",
    description: "A level made in the level editor: walls, physics crates and spinning gems to collect.",
    files: byName(levelFiles),
  },
  {
    id: "terrain",
    name: "Terrain (3D)",
    kind: "3D",
    description: "Roll a marble over hills sculpted and painted in the level editor; gems sit on the ground with TerrainHeight.",
    files: byName(terrainFiles),
  },
  {
    id: "platformer",
    name: "Platformer (3D)",
    kind: "3D",
    description: "Jump across platforms collecting coins: a level from the level editor, a robot from the modeller, a behaviour graph and code.",
    files: byName(platformerFiles),
  },
  {
    id: "robot",
    name: "Robot (3D)",
    kind: "3D",
    description: "A robot from the modeller walking around a level, switching between its walk and idle animations.",
    files: byName(robotFiles),
  },
  {
    id: "behaviours",
    name: "Behaviours (3D)",
    kind: "3D",
    description: "Two behaviour graphs, Spinner and Bobber, added to seven cubes.",
    files: byName(behaviourFiles),
  },
  { id: "drift", name: "Drift", kind: "2D", description: "Animated sprite sheet with rotation and thrust.", files: one(example("drift")) },
  { id: "cube", name: "Spinning Cube", kind: "3D", description: "The smallest 3D program: a cube with a HUD.", files: one(example("cube")) },
  { id: "marble", name: "Marble Run", kind: "3D", description: "Havok physics, a glTF model and a follow camera.", files: one(example("marble")) },
];

export const fromTemplate = (t: Template): Project => ({
  name: t.name,
  files: { ...t.files },
  assets: EXAMPLE_ASSETS,
  fileName: mainOf(t.files),
});

const json = (v: unknown) => JSON.stringify(v).replace(/</g, "\\u003c");
const inline = (s: string) => s.replaceAll("</script", "<\\/script");

/**
 * A complete HTML page that runs compiled game code. The engine libraries come from `libs`
 * (name -> URL; the IDE's own copies by default); `babylon` inlines Babylon.js instead.
 */
export function gamePage(js: string, opts: { assets: Record<string, string>; breakpoints?: number[]; title?: string; libs?: Record<string, string>; babylon?: string }) {
  const libs = opts.libs ?? LIB_URLS;
  const config = `<script>CB_FIT=true;CB_ASSETS=${json(opts.assets)};CB_BREAKPOINTS=${json(opts.breakpoints ?? [])};CB_LIBS=${json(libs)};</script>`;
  let page = pageTemplate
    .replace('<meta charset="utf-8">', (m) => m + config)
    .replace('<script src="lib/babylon.js"></script>', () => (opts.babylon ? `<script>${inline(opts.babylon)}</script>` : `<script src="${libs["babylon.js"]}"></script>`))
    .replace('<script src="runtime.js"></script>', () => `<script>${inline(runtimeJs)}</script>`)
    .replace('<script src="game.js"></script>', () => `<script>${inline(js)}</script>`);
  if (opts.title) page = page.replace("<title>CyberBasic</title>", () => `<title>${opts.title!.replace(/[<&]/g, "")}</title>`);
  return page;
}

// --- Files on disk

declare global {
  interface Window {
    showDirectoryPicker(options?: { mode?: "read" | "readwrite" }): Promise<FileSystemDirectoryHandle>;
  }
}
export const fsSupported = typeof window !== "undefined" && "showDirectoryPicker" in window;

export const openFolder = async () => readFolder(await window.showDirectoryPicker({ mode: "readwrite" }));

/** The file that runs: main.cb or main.graph, else the first one. */
export const mainOf = (files: Record<string, string>) => ["main.cb", "main.graph"].find((f) => f in files) ?? Object.keys(files).sort()[0];

/** Loads a project from a folder: its .cb and .graph files plus everything under assets/. */
// shortcut: only .cb files directly in the folder are loaded; add subfolders if projects need them.
export async function readFolder(dir: FileSystemDirectoryHandle): Promise<Project> {
  const files: Record<string, string> = {};
  for await (const [name, handle] of dir.entries()) {
    if (handle.kind === "file" && /\.(cb|graph|scene|model)$/i.test(name)) files[name] = await (await (handle as FileSystemFileHandle).getFile()).text();
  }
  const names = Object.keys(files).sort();
  if (!names.length) throw new Error(`"${dir.name}" has no .cb or .graph file`);
  const fileName = mainOf(files);
  const assets: Record<string, string> = {};
  try {
    await collect(await dir.getDirectoryHandle("assets"), "assets", assets);
  } catch {
    // no assets folder
  }
  return { name: dir.name, files, assets, dir, fileName };
}

async function collect(dir: FileSystemDirectoryHandle, prefix: string, out: Record<string, string>) {
  for await (const [name, handle] of dir.entries()) {
    const path = `${prefix}/${name}`;
    if (handle.kind === "directory") await collect(handle as FileSystemDirectoryHandle, path, out);
    else out[path] = URL.createObjectURL(await (handle as FileSystemFileHandle).getFile());
  }
}

/** Saves to the linked folder; without one, asks for a folder (Save As) and copies used assets there. */
export async function saveProject(p: Project, saveAs = false): Promise<Project> {
  let dir = saveAs ? undefined : p.dir;
  if (!dir) {
    if (!fsSupported) {
      for (const [name, src] of Object.entries(p.files)) download(name, src, "text/plain");
      return p;
    }
    dir = await window.showDirectoryPicker({ mode: "readwrite" });
    for (const [path, url] of usedAssets(p)) await writeFile(dir, path, await (await fetch(url)).blob());
  }
  for (const [name, src] of Object.entries(p.files)) await writeFile(dir, name, src);
  return { ...p, dir, name: dir.name };
}

export async function writeFile(dir: FileSystemDirectoryHandle, path: string, data: string | Blob) {
  const parts = path.split("/");
  let d = dir;
  for (const part of parts.slice(0, -1)) d = await d.getDirectoryHandle(part, { create: true });
  const w = await (await d.getFileHandle(parts[parts.length - 1], { create: true })).createWritable();
  await w.write(data);
  await w.close();
}

const usedAssets = (p: Project) => Object.entries(p.assets).filter(([path]) => Object.values(p.files).some((src) => src.includes(path)));

/**
 * Downloads the game as one HTML file that runs offline: the runtime, Babylon.js, every used asset
 * and, only if the game uses them, the glTF loaders and Havok physics are embedded.
 */
export async function exportGame(p: Project, js: string) {
  const assets: Record<string, string> = {};
  for (const [path, url] of usedAssets(p)) assets[path] = await dataUrl(await (await fetch(url)).blob());
  const libs: Record<string, string> = {};
  const embed = async (name: string, type: string) => (libs[name] = await dataUrl(new Blob([await (await fetch(LIB_URLS[name])).arrayBuffer()], { type })));
  if (js.includes("CB.LoadMesh(")) await embed("babylonjs.loaders.min.js", "text/javascript");
  if (js.includes("CB.EnablePhysics(")) {
    await embed("HavokPhysics_umd.js", "text/javascript");
    await embed("HavokPhysics.wasm", "application/wasm");
  }
  const babylon = await (await fetch(LIB_URLS["babylon.js"])).text();
  const file = `${p.name.replace(/[^\w-]+/g, "-").replace(/^-|-$/g, "").toLowerCase() || "game"}.html`;
  download(file, gamePage(js, { assets, title: p.name, libs, babylon }), "text/html");
  return file;
}

const dataUrl = (blob: Blob) =>
  new Promise<string>((ok, fail) => {
    const r = new FileReader();
    r.onload = () => ok(r.result as string);
    r.onerror = () => fail(r.error);
    r.readAsDataURL(blob);
  });

function download(name: string, data: string, type: string) {
  const a = document.createElement("a");
  a.href = URL.createObjectURL(new Blob([data], { type }));
  a.download = name;
  a.click();
  setTimeout(() => URL.revokeObjectURL(a.href), 10_000);
}

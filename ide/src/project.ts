// Projects: templates, building the game page, and folders on disk (File System Access API).
import runtimeJs from "../../runtime/runtime.js?raw";
import pageTemplate from "../../runtime/index.html?raw";
import type { Project } from "./store";

const exampleSources = import.meta.glob("../../examples/*.cb", { query: "?raw", import: "default", eager: true }) as Record<string, string>;
const shooterFiles = import.meta.glob("../../examples/shooter/*.cb", { query: "?raw", import: "default", eager: true }) as Record<string, string>;
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
    files: Object.fromEntries(Object.entries(shooterFiles).map(([path, src]) => [path.split("/").pop()!, src])),
  },
  { id: "drift", name: "Drift", kind: "2D", description: "Animated sprite sheet with rotation and thrust.", files: one(example("drift")) },
  { id: "cube", name: "Spinning Cube", kind: "3D", description: "The smallest 3D program: a cube with a HUD.", files: one(example("cube")) },
  { id: "marble", name: "Marble Run", kind: "3D", description: "Havok physics, a glTF model and a follow camera.", files: one(example("marble")) },
];

export const fromTemplate = (t: Template): Project => ({
  name: t.name,
  files: { ...t.files },
  assets: EXAMPLE_ASSETS,
  fileName: "main.cb",
});

const json = (v: unknown) => JSON.stringify(v).replace(/</g, "\\u003c");
const inline = (s: string) => s.replaceAll("</script", "<\\/script");

/** A complete, self-contained HTML page that runs compiled game code. */
export function gamePage(js: string, opts: { assets: Record<string, string>; breakpoints?: number[]; title?: string }) {
  const config = `<script>CB_FIT=true;CB_ASSETS=${json(opts.assets)};CB_BREAKPOINTS=${json(opts.breakpoints ?? [])};</script>`;
  let page = pageTemplate
    .replace('<meta charset="utf-8">', (m) => m + config)
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

/** Loads a project from a folder: its .cb files plus everything under assets/. main.cb runs. */
// shortcut: only .cb files directly in the folder are loaded; add subfolders if projects need them.
export async function readFolder(dir: FileSystemDirectoryHandle): Promise<Project> {
  const files: Record<string, string> = {};
  for await (const [name, handle] of dir.entries()) {
    if (handle.kind === "file" && name.toLowerCase().endsWith(".cb")) files[name] = await (await (handle as FileSystemFileHandle).getFile()).text();
  }
  const names = Object.keys(files).sort();
  if (!names.length) throw new Error(`"${dir.name}" has no .cb file`);
  const fileName = names.includes("main.cb") ? "main.cb" : names[0];
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

/** Downloads the game as one HTML file with the runtime and every used asset embedded. */
export async function exportGame(p: Project, js: string) {
  const assets: Record<string, string> = {};
  for (const [path, url] of usedAssets(p)) assets[path] = await dataUrl(await (await fetch(url)).blob());
  const file = `${p.name.replace(/[^\w-]+/g, "-").replace(/^-|-$/g, "").toLowerCase() || "game"}.html`;
  download(file, gamePage(js, { assets, title: p.name }), "text/html");
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

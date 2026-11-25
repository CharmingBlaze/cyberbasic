// Copies the engine libraries games need into runtime/lib/, so games (and the CLI, which embeds
// them) work offline. Run after updating babylonjs / babylonjs-loaders / @babylonjs/havok.
import { copyFileSync, mkdirSync } from "node:fs";
const out = new URL("../../runtime/lib/", import.meta.url);
mkdirSync(out, { recursive: true });
for (const f of [
  "babylonjs/babylon.js",
  "babylonjs-loaders/babylonjs.loaders.min.js",
  "@babylonjs/havok/lib/umd/HavokPhysics_umd.js",
  "@babylonjs/havok/lib/umd/HavokPhysics.wasm",
]) copyFileSync(new URL(`../node_modules/${f}`, import.meta.url), new URL(f.split("/").pop(), out));
console.log("runtime/lib updated");

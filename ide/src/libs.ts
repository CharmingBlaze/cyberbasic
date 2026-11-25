// The engine libraries games use (runtime/lib/, refreshed with `npm run lib`), served by the IDE
// itself at lib/ (see vite.config.ts) so the game preview, the 3D editors and exported games work
// without internet.
const url = (name: string) => new URL(`${import.meta.env.BASE_URL}lib/${name}`, location.href).href;

/** Library file name -> URL, as the runtime's CB_LIBS expects. */
export const LIB_URLS: Record<string, string> = Object.fromEntries(
  ["babylon.js", "babylonjs.loaders.min.js", "HavokPhysics_umd.js", "HavokPhysics.wasm"].map((n) => [n, url(n)]),
);

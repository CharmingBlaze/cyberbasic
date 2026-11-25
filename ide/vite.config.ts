import { defineConfig, type Plugin } from "vite";
import react from "@vitejs/plugin-react";
import { readFileSync, readdirSync } from "node:fs";

/**
 * Serves the engine libraries in runtime/lib/ at /lib/ exactly as they are (Vite would rewrite
 * them as modules otherwise, which breaks Babylon's script), and copies them into the build.
 */
function engineLibs(): Plugin {
  const dir = new URL("../runtime/lib/", import.meta.url);
  const types: Record<string, string> = { js: "text/javascript", wasm: "application/wasm" };
  return {
    name: "cyberbasic-engine-libs",
    configureServer(server) {
      server.middlewares.use("/lib", (req, res, next) => {
        const name = decodeURIComponent((req.url ?? "").split("?")[0].replace(/^\//, ""));
        if (!/^[\w.-]+$/.test(name)) return next();
        try {
          const body = readFileSync(new URL(name, dir));
          res.setHeader("Content-Type", types[name.split(".").pop()!] ?? "application/octet-stream");
          res.end(body);
        } catch {
          next();
        }
      });
    },
    generateBundle() {
      for (const name of readdirSync(dir)) this.emitFile({ type: "asset", fileName: `lib/${name}`, source: readFileSync(new URL(name, dir)) });
    },
  };
}

export default defineConfig({
  plugins: [react(), engineLibs()],
  // The IDE imports the runtime, examples and the WASM compiler from the repo root.
  server: { port: 5173, fs: { allow: [".."] } },
  build: { chunkSizeWarningLimit: 6000 },
});

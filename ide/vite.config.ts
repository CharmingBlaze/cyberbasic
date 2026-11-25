import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";

export default defineConfig({
  plugins: [react()],
  // The IDE imports the runtime, examples and the WASM compiler from the repo root.
  server: { port: 5173, fs: { allow: [".."] } },
  build: { chunkSizeWarningLimit: 6000 },
});

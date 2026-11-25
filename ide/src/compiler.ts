// The Rust compiler, loaded as WebAssembly (see crates/cyberbasic/src/wasm.rs for the ABI).
import wasmUrl from "../../target/wasm32-unknown-unknown/release/cyberbasic_core.wasm?url";

export interface Command {
  name: string;
  category: string;
  args: string;
  doc: string;
  returns: boolean;
  wait: boolean;
}
export interface Manifest {
  commands: Command[];
  constants: string[];
  keywords: string[];
}
export type CompileResult =
  | { ok: true; js: string; files: string[] }
  | { ok: false; file: string; line: number; col: number; message: string };

interface Exports {
  memory: WebAssembly.Memory;
  cb_alloc(len: number): number;
  cb_free(ptr: number, len: number): void;
  cb_compile(ptr: number, len: number, debug: number): number;
  cb_manifest(): void;
  cb_out_ptr(): number;
  cb_out_len(): number;
}

let wasm: Exports;
export let manifest: Manifest;

export async function loadCompiler() {
  const bytes = await fetch(wasmUrl).then((r) => {
    if (!r.ok) throw new Error("Compiler not built. Run: npm run wasm");
    return r.arrayBuffer();
  });
  wasm = (await WebAssembly.instantiate(bytes)).instance.exports as unknown as Exports;
  wasm.cb_manifest();
  manifest = JSON.parse(output());
}

const output = () =>
  new TextDecoder().decode(new Uint8Array(wasm.memory.buffer, wasm.cb_out_ptr(), wasm.cb_out_len()));

/** Compiles a project; `main` runs and may `Include` the others. */
export function compile(files: Record<string, string>, main: string, debug: boolean): CompileResult {
  const order = [main, ...Object.keys(files).filter((f) => f !== main)];
  const bytes = new TextEncoder().encode(order.map((f) => `${f}\0${files[f]}`).join("\0"));
  const p = wasm.cb_alloc(bytes.length);
  new Uint8Array(wasm.memory.buffer, p, bytes.length).set(bytes);
  const ok = wasm.cb_compile(p, bytes.length, debug ? 1 : 0);
  wasm.cb_free(p, bytes.length);
  const out = output();
  if (ok) {
    // The generated code lists its files in index order (see codegen.rs): `CB.files = [...];`
    const list = out.match(/^CB\.files = (\[.*\]);$/m);
    return { ok: true, js: out, files: list ? JSON.parse(list[1]) : [main] };
  }
  const [file, line, col, ...msg] = out.split("\t");
  return { ok: false, file: file || main, line: Number(line), col: Number(col), message: msg.join("\t") };
}

export const findCommand = (name: string) =>
  manifest.commands.find((c) => c.name.toLowerCase() === name.toLowerCase());

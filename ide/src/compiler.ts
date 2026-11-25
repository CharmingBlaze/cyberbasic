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
  | { ok: true; js: string }
  | { ok: false; line: number; col: number; message: string };

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

export function compile(src: string, debug: boolean): CompileResult {
  const bytes = new TextEncoder().encode(src);
  const p = wasm.cb_alloc(bytes.length);
  new Uint8Array(wasm.memory.buffer, p, bytes.length).set(bytes);
  const ok = wasm.cb_compile(p, bytes.length, debug ? 1 : 0);
  wasm.cb_free(p, bytes.length);
  const out = output();
  if (ok) return { ok: true, js: out };
  const [line, col, ...msg] = out.split(":");
  return { ok: false, line: Number(line), col: Number(col), message: msg.join(":") };
}

export const findCommand = (name: string) =>
  manifest.commands.find((c) => c.name.toLowerCase() === name.toLowerCase());

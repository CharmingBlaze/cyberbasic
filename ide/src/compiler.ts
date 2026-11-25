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
  cb_lower(ptr: number, len: number): void;
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

/** Copies the project into WASM memory (main file first), as the exports expect, and runs `f` on it. */
function withProject<T>(files: Record<string, string>, main: string, f: (p: number, len: number) => T): T {
  const order = [main, ...Object.keys(files).filter((n) => n !== main)];
  const bytes = new TextEncoder().encode(order.map((n) => `${n}\0${files[n]}`).join("\0"));
  const p = wasm.cb_alloc(bytes.length);
  new Uint8Array(wasm.memory.buffer, p, bytes.length).set(bytes);
  try {
    return f(p, bytes.length);
  } finally {
    wasm.cb_free(p, bytes.length);
  }
}

/** A node graph as CyberBasic: the code, generated line -> node id, and wiring problems. */
export interface Lowered { code: string; map: Map<number, string>; problems: { node: string; message: string }[] }

/** Turns every node graph in the project into CyberBasic (the compiler does it, as the CLI does). */
export function lowerGraphs(files: Record<string, string>, main: string): Record<string, Lowered> {
  withProject(files, main, (p, len) => wasm.cb_lower(p, len));
  const raw = JSON.parse(output()) as Record<string, { code: string; map: [number, string][]; problems: [string, string][] }>;
  return Object.fromEntries(
    Object.entries(raw).map(([f, l]) => [f, { code: l.code, map: new Map(l.map), problems: l.problems.map(([node, message]) => ({ node, message })) }]),
  );
}

/** Compiles a project; `main` runs and may `Include` the others. */
export function compile(files: Record<string, string>, main: string, debug: boolean): CompileResult {
  const ok = withProject(files, main, (p, len) => wasm.cb_compile(p, len, debug ? 1 : 0));
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

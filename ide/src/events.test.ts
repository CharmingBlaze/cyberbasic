// Run with `npm test`. Generates code from event sheets and compiles it with the real compiler (WASM).
import { test } from "node:test";
import assert from "node:assert/strict";
import { readFileSync } from "node:fs";
import { emptySheet, eventLine, generate, parseSheet, type Sheet } from "./events.ts";

const wasmPath = new URL("../../target/wasm32-unknown-unknown/release/cyberbasic_core.wasm", import.meta.url);
const { instance } = await WebAssembly.instantiate(readFileSync(wasmPath));
const w = instance.exports as Record<string, any>;
const out = () => new TextDecoder().decode(new Uint8Array(w.memory.buffer, w.cb_out_ptr(), w.cb_out_len()));
w.cb_manifest();
const manifest = JSON.parse(out()) as { commands: { name: string; args: string }[] };
const required = (name: string) => {
  const c = manifest.commands.find((c) => c.name.toLowerCase() === name.toLowerCase());
  return c ? c.args.split("[")[0].split(",").map((a) => a.trim()).filter(Boolean).length : 0;
};

/** Compiles files (main first); returns the JS or "file:line: message". */
function compile(files: Record<string, string>) {
  const b = new TextEncoder().encode(Object.entries(files).map(([n, s]) => `${n}\0${s}`).join("\0"));
  const p = w.cb_alloc(b.length);
  new Uint8Array(w.memory.buffer, p, b.length).set(b);
  const ok = w.cb_compile(p, b.length, 1);
  w.cb_free(p, b.length);
  const text = out();
  if (ok) return { ok: true as const, js: text };
  const [file, line, , ...msg] = text.split("\t");
  return { ok: false as const, file, line: Number(line), message: msg.join("\t") };
}

test("the Coin Catcher example sheet compiles as a main file", () => {
  const sheet = parseSheet(readFileSync(new URL("../../examples/coins/main.events", import.meta.url), "utf8"));
  const { code } = generate("main.events", sheet, true, required);
  const r = compile({ "main.events": code });
  assert.ok(r.ok, r.ok ? "" : `${r.line}: ${r.message}\n${code}`);
  assert.match(code, /While Not KeyHit\(KEY_ESCAPE\)\n {4}Sheet_main_Frame\n {4}Sync/);
});

test("a new sheet compiles, both as main and included from code", () => {
  assert.ok(compile({ "main.events": generate("main.events", emptySheet(true), true, required).code }).ok);
  const sub = generate("enemies.events", emptySheet(false), false, required).code;
  const r = compile({ "main.cb": 'Include "enemies.events"\nSheet_enemies_Frame', "enemies.events": sub });
  assert.ok(r.ok, r.ok ? "" : r.message);
});

const BROKEN: Sheet = {
  version: 1,
  variables: [{ name: "hp", value: "3" }],
  includes: [],
  events: [
    { id: 7, trigger: "frame", conditions: [{ kind: "compare", left: "hp", op: "<=", right: "0" }], actions: [{ kind: "code", text: "Print \"dead\"" }] },
    { id: 9, trigger: "frame", conditions: [], actions: [{ kind: "call", name: "Cls", args: [] }, { kind: "call", name: "Rect", args: ["1", "2", "", ""] }] },
  ],
};

test("errors map back to the event and action they came from", () => {
  const { code, map } = generate("main.events", BROKEN, true, required);
  const r = compile({ "main.events": code });
  assert.equal(r.ok, false);
  if (r.ok) return;
  assert.deepEqual(map.get(r.line), { event: 9, action: 1 }, `${r.message}\n${code}`);
});

test("breakpoints sit on the event's If line; timers reset when they fire", () => {
  const sheet: Sheet = { ...BROKEN, events: [{ id: 3, trigger: "frame", conditions: [{ kind: "every", seconds: "2" }], actions: [{ kind: "code", text: "Print 1" }] }] };
  const { code, map } = generate("main.events", sheet, true, required);
  const lines = code.split("\n");
  assert.equal(lines[eventLine(map, 3)! - 1].trim(), "sheet_main_timer3 += Delta");
  assert.match(code, /If sheet_main_timer3 >= 2\n\s+sheet_main_timer3 = 0\n\s+Print 1\n\s+EndIf/);
  assert.ok(compile({ "main.events": code }).ok);
});

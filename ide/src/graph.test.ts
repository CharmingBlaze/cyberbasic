// Run with `npm test`. Compiles node graphs to CyberBasic and builds that with the real compiler (WASM).
import { test } from "node:test";
import assert from "node:assert/strict";
import { readFileSync } from "node:fs";
import { emptyGraph, generateGraph, nodeCatalog, nodeLine, parseGraph, type Graph } from "./graph.ts";

const { instance } = await WebAssembly.instantiate(readFileSync(new URL("../../target/wasm32-unknown-unknown/release/cyberbasic_core.wasm", import.meta.url)));
const w = instance.exports as Record<string, any>;
const out = () => new TextDecoder().decode(new Uint8Array(w.memory.buffer, w.cb_out_ptr(), w.cb_out_len()));
w.cb_manifest();
const defs = nodeCatalog(JSON.parse(out()).commands);

function compile(files: Record<string, string>) {
  const b = new TextEncoder().encode(Object.entries(files).map(([n, s]) => `${n}\0${s}`).join("\0"));
  const p = w.cb_alloc(b.length);
  new Uint8Array(w.memory.buffer, p, b.length).set(b);
  const ok = w.cb_compile(p, b.length, 1);
  w.cb_free(p, b.length);
  const text = out();
  if (ok) return { ok: true as const };
  const [, line, , ...msg] = text.split("\t");
  return { ok: false as const, line: Number(line), message: msg.join("\t") };
}

test("the Bouncer example graph compiles and its wires become expressions", () => {
  const g = parseGraph(readFileSync(new URL("../../examples/bouncer/main.graph", import.meta.url), "utf8"));
  const { code, problems } = generateGraph("main.graph", g, true, defs);
  assert.deepEqual(problems, []);
  const r = compile({ "main.graph": code });
  assert.ok(r.ok, r.ok ? "" : `${r.line}: ${r.message}\n${code}`);
  assert.match(code, /x \+= \(dx \* Delta\)/); // Get -> Math -> Set
  assert.match(code, /If \(\(x < 0\) Or \(x > 780\)\)/); // Compare, Compare -> Or -> Branch
  assert.match(code, /Oval x, y, 20, 20/);
});

test("visual functions: the Bouncer defines Flip and CountBounce and calls them", () => {
  const g = parseGraph(readFileSync(new URL("../../examples/bouncer/main.graph", import.meta.url), "utf8"));
  const { code } = generateGraph("main.graph", g, true, defs);
  assert.match(code, /Function Flip\(v\)\n {4}Return \(v \* -1\)\nEnd Function/);
  assert.match(code, /Function CountBounce\(\)\n {4}bounces \+= 1\nEnd Function/);
  assert.match(code, /dx = Flip\(dx\)\n\s+CountBounce\n/);
  assert.match(code, /dy = Flip\(dy\)/);
});

test("graphs call functions written in code, and code calls functions built in graphs", () => {
  const funcs = [{ name: "Twice", params: ["n"] }, { name: "Hello", params: ["who"] }];
  const lib: Graph = {
    version: 1, variables: [], includes: [], links: [
      { from: "n1", out: "then", to: "n2", in: "in" },
      { from: "n1", out: "n", to: "n3", in: "a" },
      { from: "n3", out: "result", to: "n2", in: "value" },
      { from: "n4", out: "then", to: "n5", in: "in" },
    ],
    nodes: [
      { id: "n1", type: "func.define", x: 0, y: 0, values: { name: "Twice", inputs: "n" } },
      { id: "n2", type: "func.return", x: 0, y: 0, values: {} },
      { id: "n3", type: "math.op", x: 0, y: 0, values: { op: "*", b: "2" } },
      { id: "n4", type: "event.frame", x: 0, y: 0, values: {} },
      { id: "n5", type: "func.call", x: 0, y: 0, values: { name: "Hello", who: '"graph"' } },
    ],
  };
  const sub = generateGraph("lib.graph", lib, false, defs, funcs).code;
  assert.match(sub, /Function Twice\(n\)\n {4}Return \(n \* 2\)/);
  assert.match(sub, /Hello "graph"/);
  const main = 'Include "lib.graph"\nPrint Twice(3)\nGraph_lib_Frame\nFunction Hello(who)\n  Print "hi " + who\nEnd Function';
  assert.ok(compile({ "main.cb": main, "lib.graph": sub }).ok);
});

test("function mistakes are reported on the Function node", () => {
  const g: Graph = {
    version: 1, variables: [], includes: [], links: [{ from: "n1", out: "gone", to: "n2", in: "value" }, { from: "n1", out: "then", to: "n2", in: "in" }],
    nodes: [
      { id: "n1", type: "func.define", x: 0, y: 0, values: { name: "Rect", inputs: "a" } },
      { id: "n2", type: "func.return", x: 0, y: 0, values: {} },
    ],
  };
  const { code, map, problems } = generateGraph("main.graph", g, true, defs);
  assert.equal(problems[0]?.node, "n1"); // the wire from an input that no longer exists
  const r = compile({ "main.graph": code });
  assert.equal(r.ok, false);
  if (!r.ok) assert.equal(map.get(r.line), "n1", r.message); // "Rect" is a built-in name
});

test("new graphs compile as main and when included from code", () => {
  assert.ok(compile({ "main.graph": generateGraph("main.graph", emptyGraph(true), true, defs).code }).ok);
  const sub = generateGraph("enemies.graph", emptyGraph(false), false, defs).code;
  assert.ok(compile({ "main.cb": 'Include "enemies.graph"\nGraph_enemies_Frame', "enemies.graph": sub }).ok);
});

const G = (nodes: Graph["nodes"], links: Graph["links"]): Graph => ({ version: 1, variables: [{ name: "n", value: "0" }], includes: [], nodes, links });
const at = (id: string, type: string, values: Record<string, string> = {}) => ({ id, type, x: 0, y: 0, values });

test("compile errors map back to the node they came from", () => {
  const g = G([at("n1", "event.frame"), at("n2", "cmd.Cls"), at("n3", "cmd.Rect", { x: "1", y: "2" })], [
    { from: "n1", out: "then", to: "n2", in: "in" },
    { from: "n2", out: "then", to: "n3", in: "in" },
  ]);
  const { code, map } = generateGraph("main.graph", g, true, defs);
  const r = compile({ "main.graph": code });
  assert.equal(r.ok, false);
  if (!r.ok) assert.equal(map.get(r.line), "n3", `${r.message}\n${code}`);
  assert.equal(code.split("\n")[nodeLine(map, "n3")! - 1].trim(), "Rect 1, 2");
});

test("branches, sequences and for loops nest; loops in wires are reported, not hung on", () => {
  const g = G(
    [at("n1", "event.frame"), at("n2", "flow.for", { first: "1", last: "3" }), at("n3", "flow.branch"), at("n4", "math.compare", { op: ">", b: "1" }), at("n5", "code.line", { text: "Print n" }), at("n6", "var.set", { name: "n", op: "+=", value: "1" })],
    [
      { from: "n1", out: "then", to: "n2", in: "in" },
      { from: "n2", out: "body", to: "n3", in: "in" },
      { from: "n2", out: "index", to: "n4", in: "a" },
      { from: "n4", out: "result", to: "n3", in: "condition" },
      { from: "n3", out: "true", to: "n5", in: "in" },
      { from: "n3", out: "false", to: "n6", in: "in" },
    ],
  );
  const { code, problems } = generateGraph("main.graph", g, true, defs);
  assert.deepEqual(problems, []);
  assert.match(code, /For graph_main_i2 = 1 To 3\n\s+If \(graph_main_i2 > 1\)\n\s+Print n\n\s+Else\n\s+n \+= 1\n\s+EndIf\n\s+Next/);
  assert.ok(compile({ "main.graph": code }).ok);

  const loop = G([at("n1", "event.frame"), at("n2", "cmd.Cls"), at("n3", "cmd.Cls")], [
    { from: "n1", out: "then", to: "n2", in: "in" },
    { from: "n2", out: "then", to: "n3", in: "in" },
    { from: "n3", out: "then", to: "n2", in: "in" },
  ]);
  assert.equal(generateGraph("main.graph", loop, true, defs).problems[0]?.node, "n2");
  const selfData = G([at("n1", "event.frame"), at("n2", "code.line", { text: "x" }), at("n3", "logic.not")], [{ from: "n3", out: "result", to: "n3", in: "a" }]);
  const v = generateGraph("main.graph", { ...selfData, nodes: [...selfData.nodes, at("n4", "flow.branch")], links: [...selfData.links, { from: "n1", out: "then", to: "n4", in: "in" }, { from: "n3", out: "result", to: "n4", in: "condition" }] }, true, defs);
  assert.equal(v.problems[0]?.node, "n3");
});

test("behaviour graphs: each object gets its own variables, and the Behaviours example compiles", () => {
  const read = (f: string) => readFileSync(new URL(`../../examples/behaviours/${f}`, import.meta.url), "utf8");
  const names = ["Spinner", "Bobber"];
  const files: Record<string, string> = {};
  for (const [f, main] of [["main.graph", true], ["spinner.graph", false], ["bobber.graph", false]] as const) {
    const { code, problems } = generateGraph(f, parseGraph(read(f)), main, defs, undefined, names);
    assert.deepEqual(problems, [], f);
    files[f] = code;
  }
  const spin = files["spinner.graph"];
  assert.match(spin, /Type Behaviour_Spinner\n {4}Field entity, speed\nEnd Type/);
  assert.match(spin, /Function Behaviour_Spinner_Attach\(entity\)\n {4}me = New Behaviour_Spinner\n {4}me.entity = entity\n {4}me.speed = 90\n {4}Behaviour_Spinner_OnAttach me/);
  assert.match(spin, /me.speed = Rnd\(60, 240\)/);
  assert.match(spin, /TurnEntity me.entity, 0, \(me.speed \* Delta\), 0/);
  assert.match(spin, /EveryFrame Behaviour_Spinner_UpdateAll/);
  assert.match(files["bobber.graph"], /PositionEntity me.entity, EntityX\(me.entity\), Sin\(me.t \+ me.phase\) \* 0.6, 0/); // typed-in names too
  assert.match(files["main.graph"], /Behaviour_Spinner_Attach c\n\s+Behaviour_Bobber_Attach c/);
  const r = compile(files);
  assert.ok(r.ok, r.ok ? "" : `${r.line}: ${r.message}\n${Object.values(files).join("\n----\n")}`);
});

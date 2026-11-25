// Node graphs: the visual scripting language. Nodes connected by wires; white "exec" wires give
// the order things happen in, coloured data wires carry values. A graph is stored as JSON in a
// .graph file. This file has what the editor needs (the node catalog, pins, clipboard); the
// compiler turns graphs into CyberBasic (crates/cyberbasic/src/graph.rs, via compiler.lowerGraphs).

import type { Command } from "./compiler";

export interface GNode {
  id: string;
  type: string;
  x: number;
  y: number;
  /** Field settings (variable name, operator) and inline values for unconnected data inputs. */
  values: Record<string, string>;
  breakpoint?: boolean;
}
export interface GLink { from: string; out: string; to: string; in: string }
/** A comment box on the canvas. Dragging it moves the nodes inside it. */
export interface GComment { id: string; x: number; y: number; w: number; h: number; text: string }
export interface Graph {
  version: 1;
  /** "behaviour": attached to objects with an Add node; its variables belong to each object. */
  kind?: "script" | "behaviour";
  variables: { name: string; value: string }[];
  includes: string[];
  nodes: GNode[];
  links: GLink[];
  comments?: GComment[];
}

export type PinKind = "exec" | "data";
export interface PinDef { key: string; label: string; kind: PinKind }
export interface FieldDef { key: string; label: string; options?: string[]; placeholder?: string }
export type NodeColor = "event" | "flow" | "var" | "math" | "command" | "value" | "code" | "func";

/** A callable function: defined by a Function node in any graph, or by `Function` in code. */
export interface FuncSig { name: string; params: string[] }
export interface NodeDef {
  type: string;
  title: string;
  category: string;
  color: NodeColor;
  doc: string;
  inputs: PinDef[];
  outputs: PinDef[];
  fields?: FieldDef[];
}

const exec = (key = "in", label = ""): PinDef => ({ key, label, kind: "exec" });
const data = (key: string, label = key): PinDef => ({ key, label, kind: "data" });
const MATH_OPS = ["+", "-", "*", "/", "Mod", "^"];
const COMPARE_OPS = ["=", "<>", "<", "<=", ">", ">="];

/** The built-in nodes, plus one node per CyberBasic command. */
export function nodeCatalog(commands: Command[]): Map<string, NodeDef> {
  const defs: NodeDef[] = [
    { type: "event.start", title: "On Start", category: "Events", color: "event", doc: "Runs once when the game starts.", inputs: [], outputs: [exec("then")] },
    { type: "event.frame", title: "On Frame", category: "Events", color: "event", doc: "Runs every frame (about 60 times a second).", inputs: [], outputs: [exec("then")] },
    { type: "behaviour.attach", title: "On Attach", category: "Behaviour", color: "event", doc: "Runs once when this behaviour is added to an object. Self is that object.", inputs: [], outputs: [exec("then"), data("self")] },
    { type: "behaviour.update", title: "On Update", category: "Behaviour", color: "event", doc: "Runs every frame for each object that has this behaviour. Self is that object.", inputs: [], outputs: [exec("then"), data("self")] },
    { type: "behaviour.collision", title: "On Collision", category: "Behaviour", color: "event", doc: "Runs when this object starts touching another (needs physics: EnablePhysics and EntityPhysics). Other is what it hit.", inputs: [], outputs: [exec("then"), data("self"), data("other")] },
    { type: "behaviour.removed", title: "On Remove", category: "Behaviour", color: "event", doc: "Runs when this behaviour comes off its object: a Remove Behaviour node ran, or the object was freed.", inputs: [], outputs: [exec("then"), data("self")] },
    { type: "behaviour.self", title: "Self", category: "Behaviour", color: "var", doc: "The object this behaviour is attached to.", inputs: [], outputs: [data("self")] },
    { type: "behaviour.remove", title: "Remove Behaviour", category: "Behaviour", color: "event", doc: "Takes this behaviour off its object (the object stays).", inputs: [exec()], outputs: [] },
    { type: "behaviour.add", title: "Add Behaviour", category: "Behaviours", color: "func", doc: "Attaches a behaviour to an object.", inputs: [exec(), data("object")], outputs: [exec("then")], fields: [{ key: "name", label: "", options: [] }] },
    { type: "flow.branch", title: "Branch", category: "Flow", color: "flow", doc: "Goes down True or False depending on the condition.", inputs: [exec(), data("condition")], outputs: [exec("true", "True"), exec("false", "False")] },
    { type: "flow.sequence", title: "Sequence", category: "Flow", color: "flow", doc: "Runs Then 1, then Then 2, then Then 3.", inputs: [exec()], outputs: [exec("then1", "Then 1"), exec("then2", "Then 2"), exec("then3", "Then 3")] },
    { type: "flow.for", title: "For Loop", category: "Flow", color: "flow", doc: "Runs Body for each number from First to Last, then Done.", inputs: [exec(), data("first"), data("last")], outputs: [exec("body", "Body"), data("index"), exec("then", "Done")] },
    { type: "var.get", title: "Get", category: "Variables", color: "var", doc: "The current value of a variable.", inputs: [], outputs: [data("value")], fields: [{ key: "name", label: "variable", placeholder: "score" }] },
    { type: "var.set", title: "Set", category: "Variables", color: "var", doc: "Changes a variable: =, += or -= a value.", inputs: [exec(), data("value")], outputs: [exec("then")], fields: [{ key: "name", label: "variable", placeholder: "score" }, { key: "op", label: "", options: ["=", "+=", "-="] }] },
    { type: "math.op", title: "Math", category: "Math & Logic", color: "math", doc: "A + B, A - B, A * B, A / B, A Mod B or A ^ B.", inputs: [data("a", "A"), data("b", "B")], outputs: [data("result")], fields: [{ key: "op", label: "", options: MATH_OPS }] },
    { type: "math.compare", title: "Compare", category: "Math & Logic", color: "math", doc: "True when A is =, <>, <, <=, > or >= B.", inputs: [data("a", "A"), data("b", "B")], outputs: [data("result")], fields: [{ key: "op", label: "", options: COMPARE_OPS }] },
    { type: "logic.and", title: "And", category: "Math & Logic", color: "math", doc: "True when both are true.", inputs: [data("a", "A"), data("b", "B")], outputs: [data("result")] },
    { type: "logic.or", title: "Or", category: "Math & Logic", color: "math", doc: "True when either is true.", inputs: [data("a", "A"), data("b", "B")], outputs: [data("result")] },
    { type: "logic.not", title: "Not", category: "Math & Logic", color: "math", doc: "True when the input is false.", inputs: [data("a", "A")], outputs: [data("result")] },
    { type: "value.expr", title: "Value", category: "Math & Logic", color: "value", doc: "Any CyberBasic expression, e.g. Delta * 300 or \"Score: \" + score.", inputs: [], outputs: [data("value")], fields: [{ key: "text", label: "", placeholder: "expression" }] },
    { type: "func.define", title: "Function", category: "Functions", color: "func", doc: "Defines a function you can call from any graph or code. Its inputs become pins; its white wire is the body.", inputs: [], outputs: [exec("then")], fields: [{ key: "name", label: "name", placeholder: "Spawn" }, { key: "inputs", label: "inputs", placeholder: "x, y" }] },
    { type: "func.return", title: "Return", category: "Functions", color: "func", doc: "Ends the function, giving back a value (optional).", inputs: [exec(), data("value")], outputs: [] },
    { type: "func.call", title: "Call", category: "Functions", color: "func", doc: "Runs one of your functions.", inputs: [exec()], outputs: [exec("then")], fields: [{ key: "name", label: "", options: [] }] },
    { type: "func.value", title: "Call (value)", category: "Functions", color: "func", doc: "Calls one of your functions and gives back what it returns.", inputs: [], outputs: [data("result")], fields: [{ key: "name", label: "", options: [] }] },
    { type: "code.line", title: "Code", category: "Code", color: "code", doc: "Runs a line of CyberBasic, e.g. a call to one of your own functions.", inputs: [exec()], outputs: [exec("then")], fields: [{ key: "text", label: "", placeholder: "SpawnEnemy 3" }] },
  ];
  for (const c of commands) {
    const args = c.args.replace(/[[\]]/g, "").split(",").map((a) => a.trim()).filter(Boolean).map((a) => data(a));
    defs.push(
      c.returns
        ? { type: `cmd.${c.name}`, title: c.name, category: c.category, color: "value", doc: c.doc, inputs: args, outputs: [data("result")] }
        : { type: `cmd.${c.name}`, title: c.name, category: c.category, color: "command", doc: c.doc, inputs: [exec(), ...args], outputs: [exec("then")] },
    );
  }
  return new Map(defs.map((d) => [d.type, d]));
}

export const emptyGraph = (main: boolean): Graph => ({
  version: 1,
  variables: [],
  includes: [],
  nodes: main
    ? [
        { id: "n1", type: "event.start", x: 40, y: 40, values: {} },
        { id: "n2", type: "cmd.Graphics", x: 260, y: 40, values: { width: "800", height: "600" } },
        { id: "n3", type: "event.frame", x: 40, y: 220, values: {} },
        { id: "n4", type: "cmd.Cls", x: 260, y: 220, values: {} },
      ]
    : [{ id: "n1", type: "event.frame", x: 40, y: 40, values: {} }],
  links: main
    ? [
        { from: "n1", out: "then", to: "n2", in: "in" },
        { from: "n3", out: "then", to: "n4", in: "in" },
      ]
    : [],
});

export function parseGraph(text: string): Graph {
  try {
    const g = JSON.parse(text) as Partial<Graph>;
    return { version: 1, kind: g.kind === "behaviour" ? "behaviour" : "script", variables: g.variables ?? [], includes: g.includes ?? [], nodes: g.nodes ?? [], links: g.links ?? [], comments: g.comments ?? [] };
  } catch {
    return emptyGraph(false);
  }
}

export const graphText = (g: Graph) => JSON.stringify(g, null, 2) + "\n";
export const graphPrefix = (file: string) => "Graph_" + file.replace(/\.graph$/i, "").replace(/\W/g, "_");
export const nextNodeId = (g: Graph) => `n${Math.max(0, ...g.nodes.map((n) => Number(n.id.slice(1)) || 0)) + 1}`;

export const nextCommentId = (g: Graph) => `c${Math.max(0, ...(g.comments ?? []).map((c) => Number(c.id.slice(1)) || 0)) + 1}`;

/** What Copy puts on the clipboard: the nodes and the wires between them. */
export interface Clip { nodes: GNode[]; links: GLink[] }
export const copyNodes = (g: Graph, ids: string[]): Clip => ({
  nodes: g.nodes.filter((n) => ids.includes(n.id)),
  links: g.links.filter((l) => ids.includes(l.from) && ids.includes(l.to)),
});
/** Adds copies of the clip's nodes with fresh ids, its top-left corner at `at`. */
export function pasteNodes(g: Graph, clip: Clip, at: { x: number; y: number }) {
  let next = Number(nextNodeId(g).slice(1));
  const ids = new Map(clip.nodes.map((n) => [n.id, `n${next++}`]));
  const x0 = Math.min(...clip.nodes.map((n) => n.x)), y0 = Math.min(...clip.nodes.map((n) => n.y));
  const nodes = clip.nodes.map((n) => ({ ...structuredClone(n), id: ids.get(n.id)!, x: Math.round(at.x + n.x - x0), y: Math.round(at.y + n.y - y0), breakpoint: false }));
  const links = clip.links.map((l) => ({ ...l, from: ids.get(l.from)!, to: ids.get(l.to)! }));
  return { graph: { ...g, nodes: [...g.nodes, ...nodes], links: [...g.links, ...links] }, ids: [...ids.values()] };
}
/** Nodes lying wholly inside a comment box (`height` gives each node's height). */
export const nodesIn = (g: Graph, c: GComment, height: (n: GNode) => number, width: number) =>
  g.nodes.filter((n) => n.x >= c.x && n.y >= c.y && n.x + width <= c.x + c.w && n.y + height(n) <= c.y + c.h).map((n) => n.id);

/** The names typed into a Function node's "inputs" field. */
export const paramsOf = (text = "") =>
  text.split(",").map((p) => p.trim().replace(/[#%$]$/, "")).filter((p) => /^[A-Za-z_]\w*$/.test(p));

/** Functions defined by Function nodes in a graph. */
export const graphFunctions = (g: Graph): FuncSig[] =>
  g.nodes.filter((n) => n.type === "func.define" && (n.values.name ?? "").trim()).map((n) => ({ name: n.values.name.trim(), params: paramsOf(n.values.inputs) }));

/**
 * A node's definition with its pins for this particular node: Function nodes get a pin per input,
 * Call nodes get the pins of the function they call. Other nodes come straight from the catalog.
 */
export function nodeDef(n: GNode, defs: Map<string, NodeDef>, funcs: FuncSig[], behaviours: string[] = []): NodeDef | undefined {
  const base = defs.get(n.type);
  if (!base) return undefined;
  if (n.type === "behaviour.add") {
    const name = (n.values.name ?? "").trim();
    const options = [...new Set([...(name && !behaviours.includes(name) ? [name] : []), ...behaviours])];
    return { ...base, title: name ? `Add ${name}` : "Add Behaviour", fields: [{ key: "name", label: "", options: options.length ? options : [""] }] };
  }
  if (n.type === "func.define") {
    const name = (n.values.name ?? "").trim();
    return { ...base, title: name ? `Function ${name}` : "Function", outputs: [exec("then"), ...paramsOf(n.values.inputs).map((p) => data(p))] };
  }
  if (n.type === "func.call" || n.type === "func.value") {
    const name = (n.values.name ?? "").trim();
    const sig = funcs.find((f) => f.name.toLowerCase() === name.toLowerCase());
    const options = [...new Set([...(name && !sig ? [name] : []), ...funcs.map((f) => f.name)])];
    const fields = [{ key: "name", label: "", options: options.length ? options : [""] }];
    const params = (sig?.params ?? []).map((p) => data(p));
    return n.type === "func.call"
      ? { ...base, title: name ? `Call ${name}` : "Call", inputs: [exec(), ...params], fields }
      : { ...base, title: name ? `${name}()` : "Call (value)", inputs: params, fields };
  }
  return base;
}

/** `spinner.graph` -> `Spinner`: a behaviour's name. */
export const behaviourName = (file: string) => {
  const base = file.replace(/\.graph$/i, "").replace(/\W/g, "_");
  return base.charAt(0).toUpperCase() + base.slice(1);
};
/** The function that attaches a behaviour to an object. */
export const attachFunction = (name: string) => `Behaviour_${name}_Attach`;

/** First generated line of a node (where a breakpoint on it sits). */
export function nodeLine(map: Map<number, string>, node: string) {
  for (const [line, id] of map) if (id === node) return line;
  return undefined;
}

/** Whether a wire may join these two pins: exec to exec, data to data, output to input, different nodes. */
export function canLink(a: { node: string; pin: PinDef; side: "in" | "out" }, b: { node: string; pin: PinDef; side: "in" | "out" }) {
  return a.node !== b.node && a.side !== b.side && a.pin.kind === b.pin.kind;
}

// Node graphs: the visual scripting language. Nodes connected by wires; white "exec" wires give
// the order things happen in, coloured data wires carry values. A graph is stored as JSON in a
// .graph file and compiled to CyberBasic, which the normal compiler builds. A line map ties
// errors, breakpoints and the debugger back to nodes.
// shortcut: graphs become BASIC text rather than the shared IR (PLAN.md §7); fine while the
// compiler is AST -> JS, revisit when the IR exists.

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

/** Problems the graph itself has (not compile errors): loops in wires, unknown nodes. */
export interface GraphProblem { node: string; message: string }

/**
 * Compiles a graph to CyberBasic. Each On Start / On Frame node starts a chain that follows exec
 * wires; data inputs become expressions (an unconnected input uses its typed-in value). Events run
 * top to bottom by their position. As the main file the graph also gets the game loop.
 */
export function generateGraph(file: string, graph: Graph, isMain: boolean, defs: Map<string, NodeDef>, funcs: FuncSig[] = graphFunctions(graph), behaviours: string[] = []) {
  const lines: string[] = [];
  const map = new Map<number, string>(); // generated line -> node id
  const problems: GraphProblem[] = [];
  const emit = (text: string, node?: string) => {
    lines.push(text);
    if (node) map.set(lines.length, node);
  };
  const prefix = graphPrefix(file);
  const nodes = new Map(graph.nodes.map((n) => [n.id, n]));
  const linkInto = (node: string, pin: string) => graph.links.find((l) => l.to === node && l.in === pin);
  const linksOut = (node: string, pin: string) => graph.links.filter((l) => l.from === node && l.out === pin);
  const loopVar = (n: GNode) => `${prefix.toLowerCase()}_i${n.id.slice(1)}`;
  const defOf = (n: GNode) => nodeDef(n, defs, funcs, behaviours);
  // In a behaviour's events, its variables are fields of the object's behaviour record `me`.
  const behaviour = graph.kind === "behaviour";
  const T = `Behaviour_${behaviourName(file)}`;
  const fieldVars = new Set(graph.variables.map((v) => v.name.trim().toLowerCase()).filter(Boolean));
  let inEvent = false;
  const varRef = (name: string) => (behaviour && inEvent && fieldVars.has(name.toLowerCase()) ? `me.${name}` : name);
  const fname = (n: GNode) => (n.values.name ?? "").trim();
  /** Typed-in text, with the behaviour's variables (outside strings) turned into `me.` fields. */
  const typed = (text: string) =>
    behaviour && inEvent
      ? text.split(/("[^"]*"?)/).map((part, i) => (i % 2 ? part : part.replace(/(?<![.\w])[A-Za-z_]\w*/g, varRef))).join("")
      : text;

  /** The expression for a data input: the wired node's value, or the typed-in value. */
  const input = (n: GNode, pin: string, seen: Set<string>): string => {
    const l = linkInto(n.id, pin);
    if (!l) return typed((n.values[pin] ?? "").trim());
    return value(l.from, l.out, seen);
  };
  const value = (id: string, out: string, seen: Set<string>): string => {
    const n = nodes.get(id);
    if (!n) return "0";
    if (seen.has(id)) {
      problems.push({ node: id, message: "This value depends on itself (its data wires form a loop)." });
      return "0";
    }
    const next = new Set(seen).add(id);
    const v = (pin: string) => input(n, pin, next);
    switch (n.type) {
      case "var.get":
        return varRef((n.values.name ?? "").trim());
      case "behaviour.collision":
        return out === "other" ? "other" : "me.entity";
      case "behaviour.attach":
      case "behaviour.update":
      case "behaviour.removed":
      case "behaviour.self":
        return "me.entity";
      case "value.expr":
        return `(${(n.values.text ?? "").trim()})`;
      case "math.op":
        return `(${v("a")} ${n.values.op || "+"} ${v("b")})`;
      case "math.compare":
        return `(${v("a")} ${n.values.op || "="} ${v("b")})`;
      case "logic.and":
        return `(${v("a")} And ${v("b")})`;
      case "logic.or":
        return `(${v("a")} Or ${v("b")})`;
      case "logic.not":
        return `(Not ${v("a")})`;
      case "flow.for":
        return loopVar(n); // its "index" output
      case "func.define":
        // A Function node's data outputs are its inputs (parameters).
        if (paramsOf(n.values.inputs).includes(out)) return out;
        problems.push({ node: id, message: `This function has no input called "${out}" any more.` });
        return "0";
      case "func.value":
        return `${fname(n)}(${args(n, defOf(n)!, next)})`;
    }
    const def = defOf(n);
    if (def?.type.startsWith("cmd.")) return `${def.title}(${args(n, def, next)})`;
    problems.push({ node: id, message: `This node has no value to give.` });
    return "0";
  };
  /** Arguments of a command node; trailing empty optional ones are dropped (the compiler reports missing required ones). */
  const args = (n: GNode, def: NodeDef, seen: Set<string>) => {
    const vals = def.inputs.filter((p) => p.kind === "data").map((p) => input(n, p.key, seen));
    while (vals.length && !vals[vals.length - 1]) vals.pop();
    return vals.join(", ");
  };

  /** Emits the chain of statements that starts at the node(s) wired to `from`'s exec output `pin`. */
  const chain = (from: string, pin: string, pad: string, path: Set<string>) => {
    for (const l of linksOut(from, pin)) statement(l.to, pad, path);
  };
  const statement = (id: string, pad: string, path: Set<string>) => {
    const n = nodes.get(id);
    if (!n) return;
    if (path.has(id)) {
      problems.push({ node: id, message: "The white wires loop back here; use a For Loop or the frame loop instead." });
      return;
    }
    const p = new Set(path).add(id);
    const seen = new Set<string>();
    const def = defOf(n);
    switch (n.type) {
      case "func.call":
        emit(`${pad}${[fname(n), args(n, def!, seen)].filter(Boolean).join(" ")}`, id);
        break;
      case "behaviour.add":
        emit(`${pad}${attachFunction(fname(n))} ${input(n, "object", seen)}`, id);
        break;
      case "behaviour.remove":
        // entity = 0 tells UpdateAll this record is gone, even within the frame it was removed in.
        emit(`${pad}${T}_OnRemove me`, id);
        emit(`${pad}me.entity = 0`, id);
        emit(`${pad}Delete me`, id);
        return;
      case "func.return": {
        const v = input(n, "value", seen);
        emit(`${pad}Return${v ? " " + v : ""}`, id);
        return; // nothing runs after a Return
      }
      case "flow.branch":
        emit(`${pad}If ${input(n, "condition", seen) || "0"}`, id);
        chain(id, "true", pad + "    ", p);
        if (linksOut(id, "false").length) {
          emit(`${pad}Else`, id);
          chain(id, "false", pad + "    ", p);
        }
        emit(`${pad}EndIf`, id);
        return;
      case "flow.sequence":
        for (const k of ["then1", "then2", "then3"]) chain(id, k, pad, p);
        return;
      case "flow.for":
        emit(`${pad}For ${loopVar(n)} = ${input(n, "first", seen)} To ${input(n, "last", seen)}`, id);
        chain(id, "body", pad + "    ", p);
        emit(`${pad}Next`, id);
        break;
      case "var.set":
        emit(`${pad}${varRef((n.values.name ?? "").trim())} ${n.values.op || "="} ${input(n, "value", seen)}`, id);
        break;
      case "code.line":
        emit(`${pad}${typed((n.values.text ?? "").trim())}`, id);
        break;
      default:
        if (!def?.type.startsWith("cmd.") || def.outputs.some((o) => o.kind === "data")) {
          problems.push({ node: id, message: "This node can't run from a white wire." });
          return;
        }
        emit(`${pad}${[def.title, args(n, def, seen)].filter(Boolean).join(" ")}`, id);
    }
    chain(id, "then", pad, p);
  };

  emit(`' Generated from ${file} by the visual editor. Edit the graph, not this code.`);
  for (const inc of graph.includes) emit(`Include "${inc}"`);
  if (!behaviour) for (const v of graph.variables) if (v.name.trim()) emit(`Global ${v.name.trim()} = ${v.value.trim() || "0"}`);
  for (const n of graph.nodes) if (!defs.has(n.type)) problems.push({ node: n.id, message: `Unknown node "${n.type}".` });

  // Functions first: CyberBasic wants them at the top level.
  for (const f of graph.nodes.filter((n) => n.type === "func.define").sort((a, b) => a.y - b.y || a.x - b.x)) {
    emit("");
    emit(`Function ${fname(f)}(${paramsOf(f.values.inputs).join(", ")})`, f.id);
    chain(f.id, "then", "    ", new Set());
    emit("End Function", f.id);
  }

  const roots = (type: string) => graph.nodes.filter((n) => n.type === type).sort((a, b) => a.y - b.y || a.x - b.x);

  if (behaviour) {
    // Each object with this behaviour gets a record (a Type) holding the object and its own variables.
    const b = behaviourName(file);
    const vars = graph.variables.filter((v) => v.name.trim());
    emit("");
    emit(`Type ${T}`);
    emit(`    Field ${["entity", ...vars.map((v) => v.name.trim())].join(", ")}`);
    emit("End Type");
    emit("");
    emit(`Function ${attachFunction(b)}(entity)`);
    emit(`    me = New ${T}`);
    emit("    me.entity = entity");
    for (const v of vars) emit(`    me.${v.name.trim()} = ${v.value.trim() || "0"}`);
    emit(`    ${T}_OnAttach me`);
    emit("    Return me");
    emit("End Function");
    inEvent = true;
    const collides = roots("behaviour.collision").length > 0;
    const events = [["behaviour.attach", "OnAttach"], ["behaviour.update", "OnUpdate"], ["behaviour.removed", "OnRemove"], ...(collides ? [["behaviour.collision", "OnCollision"]] : [])];
    for (const [event, fn] of events) {
      emit("");
      emit(`Function ${T}_${fn}(${event === "behaviour.collision" ? "me, other" : "me"})`);
      for (const r of roots(event)) {
        emit(`    ' ${defs.get(event)?.title}`, r.id);
        chain(r.id, "then", "    ", new Set());
      }
      emit("End Function");
    }
    inEvent = false;
    emit("");
    emit(`Function ${T}_UpdateAll()`);
    emit(`    For b = Each ${T}`);
    emit("        If EntityExists(b.entity)");
    emit(`            ${T}_OnUpdate b`);
    if (collides) {
      emit("            If EntityExists(b.entity)");
      emit(`                If EntityCollided(b.entity) Then ${T}_OnCollision b, CollidedWith(b.entity)`);
      emit("            EndIf");
    }
    emit("        ElseIf b.entity <> 0");
    emit("            ' The object was freed: the behaviour comes off it.");
    emit(`            ${T}_OnRemove b`);
    emit("            Delete b");
    emit("        EndIf");
    emit("    Next");
    emit("End Function");
    emit(`EveryFrame ${T}_UpdateAll`);
    return { code: lines.join("\n") + "\n", map, problems };
  }

  for (const [event, fn] of [["event.start", "Start"], ["event.frame", "Frame"]] as const) {
    emit("");
    emit(`Function ${prefix}_${fn}()`);
    for (const r of roots(event)) {
      emit(`    ' ${defs.get(event)?.title}`, r.id);
      chain(r.id, "then", "    ", new Set());
    }
    emit("End Function");
  }
  if (isMain) {
    emit("");
    emit(`${prefix}_Start`);
    emit("While Not KeyHit(KEY_ESCAPE)");
    emit(`    ${prefix}_Frame`);
    emit("    Sync");
    emit("Wend");
  }
  return { code: lines.join("\n") + "\n", map, problems };
}

/** First generated line of a node (where a breakpoint on it sits). */
export function nodeLine(map: Map<number, string>, node: string) {
  for (const [line, id] of map) if (id === node) return line;
  return undefined;
}

/** Whether a wire may join these two pins: exec to exec, data to data, output to input, different nodes. */
export function canLink(a: { node: string; pin: PinDef; side: "in" | "out" }, b: { node: string; pin: PinDef; side: "in" | "out" }) {
  return a.node !== b.node && a.side !== b.side && a.pin.kind === b.pin.kind;
}

//! Node graphs (.graph files): the visual scripting language, lowered to CyberBasic text.
//! White "exec" wires give the order things happen in, data wires carry values. A line map ties
//! errors, breakpoints and the debugger back to nodes. The IDE (through WASM) and the CLI both
//! use this, so a graph compiles the same everywhere.
// shortcut: graphs become BASIC text rather than the shared IR (PLAN.md §7); fine while the
// compiler is AST -> JS, revisit when the IR exists.

use crate::builtins;
use crate::json::{self, Json};
use std::collections::{BTreeMap, HashSet};

pub struct Node {
    pub id: String,
    pub ty: String,
    pub x: f64,
    pub y: f64,
    pub values: BTreeMap<String, String>,
}
pub struct Link {
    pub from: String,
    pub out: String,
    pub to: String,
    pub inp: String,
}
pub struct Graph {
    pub behaviour: bool,
    pub variables: Vec<(String, String)>,
    pub includes: Vec<String>,
    pub nodes: Vec<Node>,
    pub links: Vec<Link>,
}

/// A function any graph can call: a Function node in a graph, or `Function` in code.
#[derive(Debug, Clone, PartialEq)]
pub struct FuncSig {
    pub name: String,
    pub params: Vec<String>,
}

/// A graph as CyberBasic, with generated line -> node id and wiring problems (node id, message).
pub struct Lowered {
    pub code: String,
    pub map: Vec<(usize, String)>,
    pub problems: Vec<(String, String)>,
}

pub fn parse(text: &str) -> Result<Graph, String> {
    let j = json::parse(text)?;
    let s = |v: &Json, k: &str| v.get(k).str().to_string();
    Ok(Graph {
        behaviour: j.get("kind").str() == "behaviour",
        variables: j.get("variables").arr().iter().map(|v| (s(v, "name"), s(v, "value"))).collect(),
        includes: j.get("includes").arr().iter().map(|v| v.str().to_string()).collect(),
        nodes: j
            .get("nodes")
            .arr()
            .iter()
            .map(|n| Node {
                id: s(n, "id"),
                ty: s(n, "type"),
                x: n.get("x").num(),
                y: n.get("y").num(),
                values: n.get("values").obj().map_or_else(BTreeMap::new, |m| m.iter().map(|(k, v)| (k.clone(), v.str().to_string())).collect()),
            })
            .collect(),
        links: j.get("links").arr().iter().map(|l| Link { from: s(l, "from"), out: s(l, "out"), to: s(l, "to"), inp: s(l, "in") }).collect(),
    })
}

/// The names typed into a Function node's "inputs" field.
pub fn params_of(text: &str) -> Vec<String> {
    text.split(',')
        .map(|p| p.trim().trim_end_matches(['#', '%', '$']))
        .filter(|p| is_ident(p))
        .map(str::to_string)
        .collect()
}

fn is_ident(s: &str) -> bool {
    let mut c = s.chars();
    c.next().is_some_and(|f| f.is_ascii_alphabetic() || f == '_') && c.all(|x| x.is_ascii_alphanumeric() || x == '_')
}

/// Functions defined by Function nodes in a graph.
pub fn graph_functions(g: &Graph) -> Vec<FuncSig> {
    g.nodes
        .iter()
        .filter(|n| n.ty == "func.define" && !val(n, "name").is_empty())
        .map(|n| FuncSig { name: val(n, "name").to_string(), params: params_of(n.values.get("inputs").map_or("", String::as_str)) })
        .collect()
}

/// `Function Name(a, b)` lines in code.
pub fn code_functions(src: &str) -> Vec<FuncSig> {
    src.lines()
        .filter_map(|l| {
            let t = l.trim();
            if t.len() < 9 || !t[..9].eq_ignore_ascii_case("function ") {
                return None;
            }
            let rest = t[9..].trim_start();
            let end = rest.find(|c: char| !(c.is_ascii_alphanumeric() || c == '_')).unwrap_or(rest.len());
            let name = &rest[..end];
            if !is_ident(name) {
                return None;
            }
            let after = rest[end..].trim_start();
            let params = after.strip_prefix('(').and_then(|p| p.split(')').next()).map(params_of).unwrap_or_default();
            Some(FuncSig { name: name.to_string(), params })
        })
        .collect()
}

pub fn is_graph(file: &str) -> bool {
    file.to_ascii_lowercase().ends_with(".graph")
}

/// Every function in a project, from graphs and code alike.
pub fn project_functions(files: &[(&str, &str)]) -> Vec<FuncSig> {
    files
        .iter()
        .flat_map(|(f, t)| {
            if is_graph(f) {
                parse(t).map(|g| graph_functions(&g)).unwrap_or_default()
            } else if crate::scene::is_scene(f) {
                vec![FuncSig { name: crate::scene::load_function(f), params: vec![] }]
            } else {
                code_functions(t)
            }
        })
        .collect()
}

fn stem(file: &str) -> String {
    let base = file.rfind('.').map_or(file, |i| &file[..i]);
    base.chars().map(|c| if c.is_alphanumeric() || c == '_' { c } else { '_' }).collect()
}
pub fn graph_prefix(file: &str) -> String {
    format!("Graph_{}", stem(file))
}
/// `spinner.graph` -> `Spinner`.
pub fn behaviour_name(file: &str) -> String {
    let s = stem(file);
    let mut c = s.chars();
    c.next().map_or(String::new(), |f| f.to_uppercase().collect::<String>() + c.as_str())
}

const BUILT_IN_NODES: &[&str] = &[
    "event.start", "event.frame", "behaviour.attach", "behaviour.update", "behaviour.collision", "behaviour.removed", "behaviour.self",
    "behaviour.remove", "behaviour.add", "flow.branch", "flow.sequence", "flow.for", "var.get", "var.set", "math.op", "math.compare",
    "logic.and", "logic.or", "logic.not", "value.expr", "func.define", "func.return", "func.call", "func.value", "code.line",
];

fn title(event: &str) -> &'static str {
    match event {
        "event.start" => "On Start",
        "event.frame" => "On Frame",
        "behaviour.attach" => "On Attach",
        "behaviour.update" => "On Update",
        "behaviour.removed" => "On Remove",
        _ => "On Collision",
    }
}

fn val<'a>(n: &'a Node, key: &str) -> &'a str {
    n.values.get(key).map_or("", |v| v.trim())
}

/// A command node's command, by its exact name (`cmd.Cls`).
fn command(ty: &str) -> Option<&'static builtins::Builtin> {
    ty.strip_prefix("cmd.").and_then(builtins::find).filter(|b| format!("cmd.{}", b.name) == ty)
}
fn command_args(b: &builtins::Builtin) -> Vec<String> {
    b.args.replace(['[', ']'], "").split(',').map(|a| a.trim().to_string()).filter(|a| !a.is_empty()).collect()
}

struct Lower<'a> {
    g: &'a Graph,
    funcs: &'a [FuncSig],
    prefix: String,
    t: String,
    fields: HashSet<String>,
    in_event: bool,
    lines: Vec<String>,
    map: Vec<(usize, String)>,
    problems: Vec<(String, String)>,
}

impl Lower<'_> {
    fn emit(&mut self, text: String, node: Option<&str>) {
        self.lines.push(text);
        if let Some(n) = node {
            self.map.push((self.lines.len(), n.to_string()));
        }
    }
    fn problem(&mut self, node: &str, msg: impl Into<String>) {
        self.problems.push((node.to_string(), msg.into()));
    }
    fn node(&self, id: &str) -> Option<&Node> {
        self.g.nodes.iter().find(|n| n.id == id)
    }
    fn link_into(&self, node: &str, pin: &str) -> Option<&Link> {
        self.g.links.iter().find(|l| l.to == node && l.inp == pin)
    }
    fn links_out(&self, node: &str, pin: &str) -> Vec<String> {
        self.g.links.iter().filter(|l| l.from == node && l.out == pin).map(|l| l.to.clone()).collect()
    }
    fn loop_var(&self, n: &Node) -> String {
        format!("{}_i{}", self.prefix.to_lowercase(), n.id.get(1..).unwrap_or(""))
    }
    /// In a behaviour's events, its variables are fields of the object's behaviour record `me`.
    fn var_ref(&self, name: &str) -> String {
        if self.g.behaviour && self.in_event && self.fields.contains(&name.to_lowercase()) {
            format!("me.{name}")
        } else {
            name.to_string()
        }
    }
    /// Typed-in text, with the behaviour's variables (outside strings) turned into `me.` fields.
    fn typed(&self, text: &str) -> String {
        if !(self.g.behaviour && self.in_event) {
            return text.to_string();
        }
        let (mut out, mut in_str, mut prev) = (String::new(), false, ' ');
        let cs: Vec<char> = text.chars().collect();
        let mut i = 0;
        while i < cs.len() {
            let c = cs[i];
            let word = |c: char| c.is_ascii_alphanumeric() || c == '_';
            if c == '"' {
                in_str = !in_str;
            } else if !in_str && (c.is_ascii_alphabetic() || c == '_') && prev != '.' && !word(prev) {
                let start = i;
                while i < cs.len() && word(cs[i]) {
                    i += 1;
                }
                let id: String = cs[start..i].iter().collect();
                out.push_str(&self.var_ref(&id));
                prev = cs[i - 1];
                continue;
            }
            out.push(c);
            prev = c;
            i += 1;
        }
        out
    }
    fn sig(&self, name: &str) -> Option<&FuncSig> {
        self.funcs.iter().find(|f| f.name.eq_ignore_ascii_case(name))
    }
    /// The node's data inputs, in order.
    fn data_inputs(&self, n: &Node) -> Vec<String> {
        match n.ty.as_str() {
            "func.call" | "func.value" => self.sig(val(n, "name")).map(|s| s.params.clone()).unwrap_or_default(),
            t => command(t).map(command_args).unwrap_or_default(),
        }
    }

    /// The expression for a data input: the wired node's value, or the typed-in value.
    fn input(&mut self, n: &Node, pin: &str, seen: &HashSet<String>) -> String {
        match self.link_into(&n.id, pin).map(|l| (l.from.clone(), l.out.clone())) {
            Some((from, out)) => self.value(&from, &out, seen),
            None => self.typed(val(n, pin)),
        }
    }
    fn value(&mut self, id: &str, out: &str, seen: &HashSet<String>) -> String {
        let g = self.g;
        let Some(n) = g.nodes.iter().find(|n| n.id == id) else { return "0".into() };
        if seen.contains(id) {
            self.problem(id, "This value depends on itself (its data wires form a loop).");
            return "0".into();
        }
        let mut next = seen.clone();
        next.insert(id.to_string());
        let v = |me: &mut Self, pin: &str| me.input(n, pin, &next);
        match n.ty.as_str() {
            "var.get" => self.var_ref(val(n, "name")),
            "behaviour.collision" if out == "other" => "other".into(),
            "behaviour.collision" | "behaviour.attach" | "behaviour.update" | "behaviour.removed" | "behaviour.self" => "me.entity".into(),
            "value.expr" => format!("({})", val(n, "text")),
            "math.op" => format!("({} {} {})", v(self, "a"), or(val(n, "op"), "+"), v(self, "b")),
            "math.compare" => format!("({} {} {})", v(self, "a"), or(val(n, "op"), "="), v(self, "b")),
            "logic.and" => format!("({} And {})", v(self, "a"), v(self, "b")),
            "logic.or" => format!("({} Or {})", v(self, "a"), v(self, "b")),
            "logic.not" => format!("(Not {})", v(self, "a")),
            "flow.for" => self.loop_var(n), // its "index" output
            "func.define" => {
                // A Function node's data outputs are its inputs (parameters).
                if params_of(val(n, "inputs")).iter().any(|p| p == out) {
                    out.to_string()
                } else {
                    self.problem(id, format!("This function has no input called \"{out}\" any more."));
                    "0".into()
                }
            }
            "func.value" => format!("{}({})", val(n, "name"), self.args(n, &next)),
            t => match command(t) {
                Some(b) => format!("{}({})", b.name, self.args(n, &next)),
                None => {
                    self.problem(id, "This node has no value to give.");
                    "0".into()
                }
            },
        }
    }
    /// Arguments of a command or call node; trailing empty optional ones are dropped (the compiler
    /// reports missing required ones).
    fn args(&mut self, n: &Node, seen: &HashSet<String>) -> String {
        let mut vals: Vec<String> = self.data_inputs(n).iter().map(|p| self.input(n, p, seen)).collect();
        while vals.last().is_some_and(|v| v.is_empty()) {
            vals.pop();
        }
        vals.join(", ")
    }

    /// Emits the chain of statements wired to `from`'s exec output `pin`.
    fn chain(&mut self, from: &str, pin: &str, pad: &str, path: &HashSet<String>) {
        for to in self.links_out(from, pin) {
            self.statement(&to, pad, path);
        }
    }
    fn statement(&mut self, id: &str, pad: &str, path: &HashSet<String>) {
        let g = self.g;
        let Some(n) = g.nodes.iter().find(|n| n.id == id) else { return };
        if path.contains(id) {
            self.problem(id, "The white wires loop back here; use a For Loop or the frame loop instead.");
            return;
        }
        let mut p = path.clone();
        p.insert(id.to_string());
        let seen = HashSet::new();
        let inner = format!("{pad}    ");
        let at = Some(id);
        match n.ty.as_str() {
            "func.call" => {
                let line = [val(n, "name").to_string(), self.args(n, &seen)].into_iter().filter(|s| !s.is_empty()).collect::<Vec<_>>().join(" ");
                self.emit(format!("{pad}{line}"), at);
            }
            "behaviour.add" => {
                let obj = self.input(n, "object", &seen);
                self.emit(format!("{pad}Behaviour_{}_Attach {obj}", val(n, "name")), at);
            }
            "behaviour.remove" => {
                // entity = 0 tells UpdateAll this record is gone, even within the frame it was removed in.
                let t = self.t.clone();
                self.emit(format!("{pad}{t}_OnRemove me"), at);
                self.emit(format!("{pad}me.entity = 0"), at);
                self.emit(format!("{pad}Delete me"), at);
                return;
            }
            "func.return" => {
                let v = self.input(n, "value", &seen);
                self.emit(format!("{pad}Return{}", if v.is_empty() { String::new() } else { format!(" {v}") }), at);
                return; // nothing runs after a Return
            }
            "flow.branch" => {
                let c = self.input(n, "condition", &seen);
                self.emit(format!("{pad}If {}", or(&c, "0")), at);
                self.chain(id, "true", &inner, &p);
                if !self.links_out(id, "false").is_empty() {
                    self.emit(format!("{pad}Else"), at);
                    self.chain(id, "false", &inner, &p);
                }
                self.emit(format!("{pad}EndIf"), at);
                return;
            }
            "flow.sequence" => {
                for k in ["then1", "then2", "then3"] {
                    self.chain(id, k, pad, &p);
                }
                return;
            }
            "flow.for" => {
                let (a, b) = (self.input(n, "first", &seen), self.input(n, "last", &seen));
                self.emit(format!("{pad}For {} = {a} To {b}", self.loop_var(n)), at);
                self.chain(id, "body", &inner, &p);
                self.emit(format!("{pad}Next"), at);
            }
            "var.set" => {
                let v = self.input(n, "value", &seen);
                self.emit(format!("{pad}{} {} {v}", self.var_ref(val(n, "name")), or(val(n, "op"), "=")), at);
            }
            "code.line" => self.emit(format!("{pad}{}", self.typed(val(n, "text"))), at),
            t => match command(t).filter(|b| !b.returns) {
                Some(b) => {
                    let a = self.args(n, &seen);
                    self.emit(format!("{pad}{}{}", b.name, if a.is_empty() { String::new() } else { format!(" {a}") }), at);
                }
                None => {
                    self.problem(id, "This node can't run from a white wire.");
                    return;
                }
            },
        }
        self.chain(id, "then", pad, &p);
    }
    fn roots(&self, ty: &str) -> Vec<String> {
        let mut r: Vec<&Node> = self.g.nodes.iter().filter(|n| n.ty == ty).collect();
        r.sort_by(|a, b| a.y.total_cmp(&b.y).then(a.x.total_cmp(&b.x)));
        r.into_iter().map(|n| n.id.clone()).collect()
    }
    fn events(&mut self, ty: &str) {
        for r in self.roots(ty) {
            self.emit(format!("    ' {}", title(ty)), Some(&r));
            self.chain(&r, "then", "    ", &HashSet::new());
        }
    }
}

fn or<'a>(s: &'a str, default: &'a str) -> &'a str {
    if s.is_empty() { default } else { s }
}

/// Compiles a graph to CyberBasic. Each event node starts a chain that follows exec wires; data
/// inputs become expressions (an unconnected input uses its typed-in value). Events run top to
/// bottom by their position. As the main file the graph also gets the game loop.
pub fn lower(file: &str, g: &Graph, is_main: bool, funcs: &[FuncSig]) -> Lowered {
    let mut l = Lower {
        g,
        funcs,
        prefix: graph_prefix(file),
        t: format!("Behaviour_{}", behaviour_name(file)),
        fields: g.variables.iter().map(|(n, _)| n.trim().to_lowercase()).filter(|n| !n.is_empty()).collect(),
        in_event: false,
        lines: vec![],
        map: vec![],
        problems: vec![],
    };
    let vars: Vec<(&str, &str)> = g.variables.iter().map(|(n, v)| (n.trim(), or(v.trim(), "0"))).filter(|(n, _)| !n.is_empty()).collect();
    l.emit(format!("' Generated from {file} by the visual editor. Edit the graph, not this code."), None);
    for inc in &g.includes {
        l.emit(format!("Include \"{inc}\""), None);
    }
    if !g.behaviour {
        for (n, v) in &vars {
            l.emit(format!("Global {n} = {v}"), None);
        }
    }
    for n in &g.nodes {
        if !BUILT_IN_NODES.contains(&n.ty.as_str()) && command(&n.ty).is_none() {
            l.problem(&n.id, format!("Unknown node \"{}\".", n.ty));
        }
    }

    // Functions first: CyberBasic wants them at the top level.
    for f in l.roots("func.define") {
        let n = l.node(&f).unwrap();
        let head = format!("Function {}({})", val(n, "name"), params_of(val(n, "inputs")).join(", "));
        l.emit(String::new(), None);
        l.emit(head, Some(&f));
        l.chain(&f, "then", "    ", &HashSet::new());
        l.emit("End Function".into(), Some(&f));
    }

    let t = l.t.clone();
    if g.behaviour {
        // Each object with this behaviour gets a record (a Type) holding the object and its own variables.
        let fields: Vec<&str> = std::iter::once("entity").chain(vars.iter().map(|v| v.0)).collect();
        let mut text = vec![String::new(), format!("Type {t}"), format!("    Field {}", fields.join(", ")), "End Type".into(), String::new()];
        text.push(format!("Function Behaviour_{}_Attach(entity)", behaviour_name(file)));
        text.push(format!("    me = New {t}"));
        text.push("    me.entity = entity".into());
        text.extend(vars.iter().map(|(n, v)| format!("    me.{n} = {v}")));
        text.extend([format!("    {t}_OnAttach me"), "    Return me".into(), "End Function".into()]);
        for s in text {
            l.emit(s, None);
        }
        l.in_event = true;
        let collides = !l.roots("behaviour.collision").is_empty();
        let mut events = vec![("behaviour.attach", "OnAttach"), ("behaviour.update", "OnUpdate"), ("behaviour.removed", "OnRemove")];
        if collides {
            events.push(("behaviour.collision", "OnCollision"));
        }
        for (ev, f) in events {
            l.emit(String::new(), None);
            l.emit(format!("Function {t}_{f}({})", if ev == "behaviour.collision" { "me, other" } else { "me" }), None);
            l.events(ev);
            l.emit("End Function".into(), None);
        }
        l.in_event = false;
        let mut tail = vec![
            String::new(),
            format!("Function {t}_UpdateAll()"),
            format!("    For b = Each {t}"),
            "        If EntityExists(b.entity)".into(),
            format!("            {t}_OnUpdate b"),
        ];
        if collides {
            tail.push("            If EntityExists(b.entity)".into());
            tail.push(format!("                If EntityCollided(b.entity) Then {t}_OnCollision b, CollidedWith(b.entity)"));
            tail.push("            EndIf".into());
        }
        tail.extend([
            "        ElseIf b.entity <> 0".into(),
            "            ' The object was freed: the behaviour comes off it.".into(),
            format!("            {t}_OnRemove b"),
            "            Delete b".into(),
            "        EndIf".into(),
            "    Next".into(),
            "End Function".into(),
            format!("EveryFrame {t}_UpdateAll"),
        ]);
        for s in tail {
            l.emit(s, None);
        }
    } else {
        for (ev, f) in [("event.start", "Start"), ("event.frame", "Frame")] {
            l.emit(String::new(), None);
            l.emit(format!("Function {}_{f}()", l.prefix), None);
            l.events(ev);
            l.emit("End Function".into(), None);
        }
        if is_main {
            let p = l.prefix.clone();
            for s in [String::new(), format!("{p}_Start"), "While Not KeyHit(KEY_ESCAPE)".into(), format!("    {p}_Frame"), "    Sync".into(), "Wend".into()] {
                l.emit(s, None);
            }
        }
    }
    Lowered { code: l.lines.join("\n") + "\n", map: l.map, problems: l.problems }
}

/// Lowers every graph and scene in a project (`files[0]` is the main file). A file that isn't
/// valid JSON becomes an empty program with one problem.
pub fn lower_project(files: &[(&str, &str)]) -> Vec<(String, Lowered)> {
    let funcs = project_functions(files);
    files
        .iter()
        .enumerate()
        .filter(|(_, (f, _))| is_graph(f) || crate::scene::is_scene(f))
        .map(|(i, (f, t))| {
            if crate::scene::is_scene(f) {
                return (f.to_string(), crate::scene::lower(f, t));
            }
            let low = match parse(t) {
                Ok(g) => lower(f, &g, i == 0, &funcs),
                Err(e) => Lowered { code: "\n".into(), map: vec![], problems: vec![(String::new(), format!("This graph file is damaged: {e}"))] },
            };
            (f.to_string(), low)
        })
        .collect()
}

/// `lower_project` as JSON for the IDE: {"file": {"code", "map": [[line, node]], "problems": [[node, msg]]}}.
pub fn lowered_json(all: &[(String, Lowered)]) -> String {
    let q = json::quote;
    let items: Vec<String> = all
        .iter()
        .map(|(f, l)| {
            let map: Vec<String> = l.map.iter().map(|(line, n)| format!("[{line},{}]", q(n))).collect();
            let probs: Vec<String> = l.problems.iter().map(|(n, m)| format!("[{},{}]", q(n), q(m))).collect();
            format!("{}:{{\"code\":{},\"map\":[{}],\"problems\":[{}]}}", q(f), q(&l.code), map.join(","), probs.join(","))
        })
        .collect();
    format!("{{{}}}", items.join(","))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn graph_projects_compile_like_code() {
        let dir = concat!(env!("CARGO_MANIFEST_DIR"), "/../../examples/behaviours/");
        let read = |f: &str| std::fs::read_to_string(format!("{dir}{f}")).unwrap();
        let (m, s, b) = (read("main.graph"), read("spinner.graph"), read("bobber.graph"));
        let js = crate::compile_files(&[("main.graph", &m), ("spinner.graph", &s), ("bobber.graph", &b)], false).unwrap();
        assert!(js.contains("CB.hooks.push(f_behaviour_spinner_updateall)"));
    }

    #[test]
    fn wiring_problems_and_code_functions() {
        // A Call node gets its inputs from a function written in code.
        let g = r#"{"nodes":[{"id":"n1","type":"event.frame","x":0,"y":0,"values":{}},
            {"id":"n2","type":"func.call","x":0,"y":0,"values":{"name":"hello","who":"\"graph\""}},
            {"id":"n3","type":"cmd.Cls","x":0,"y":0,"values":{}}],
          "links":[{"from":"n1","out":"then","to":"n2","in":"in"},{"from":"n2","out":"then","to":"n3","in":"in"},{"from":"n3","out":"then","to":"n2","in":"in"}]}"#;
        let code = "Include \"lib.graph\"\nFunction Hello(who$)\nEnd Function";
        let low = lower_project(&[("main.cb", code), ("lib.graph", g)]);
        let l = &low[0].1;
        assert!(l.code.contains("    hello \"graph\"\n    Cls\n"), "{}", l.code);
        assert_eq!(l.problems[0].0, "n2"); // the white wires loop back
        let e = crate::compile_files(&[("main.cb", code), ("lib.graph", g)], false).unwrap_err();
        assert_eq!((e.file.as_str(), e.line), ("lib.graph", l.map.iter().find(|m| m.1 == "n2").unwrap().0));
        assert!(e.msg.starts_with("node n2: The white wires loop back"));
        assert!(crate::compile_files(&[("main.graph", "{oops")], false).unwrap_err().msg.contains("damaged"));
        assert_eq!(code_functions("  function Spawn ( x#, y )\nFunctionX\n"), vec![FuncSig { name: "Spawn".into(), params: vec!["x".into(), "y".into()] }]);
    }
}

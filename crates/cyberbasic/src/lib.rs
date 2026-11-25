//! CyberBasic compiler: source -> tokens -> AST -> JavaScript.

pub mod builtins;
mod codegen;
mod lexer;
mod parser;
#[cfg(target_arch = "wasm32")]
mod wasm;

/// Runtime and page template, embedded so the CLI is a single self-contained binary.
pub const RUNTIME_JS: &str = include_str!("../../../runtime/runtime.js");
pub const INDEX_HTML: &str = include_str!("../../../runtime/index.html");

#[derive(Debug, PartialEq)]
pub struct Error {
    /// Source file the error is in ("" until known).
    pub file: String,
    pub line: usize,
    pub col: usize,
    pub msg: String,
}

impl Error {
    pub fn new(line: usize, col: usize, msg: impl Into<String>) -> Self {
        Error { file: String::new(), line, col, msg: msg.into() }
    }
    pub fn in_file(mut self, file: &str) -> Self {
        self.file = file.to_string();
        self
    }
}

pub use lexer::LOC_FILE;

pub fn compile(src: &str) -> Result<String, Error> {
    compile_with(src, false)
}

/// `debug` adds breakpoint/step hooks and variable snapshots for the IDE debugger.
pub fn compile_with(src: &str, debug: bool) -> Result<String, Error> {
    compile_project("main.cb", &|name| (name == "main.cb").then(|| src.to_string()), debug)
}

/// Compiles a program that may `Include` other files. `load` returns a file's source by its
/// project-relative name. The generated code lists the files in `CB.files`, in index order.
pub fn compile_project(main: &str, load: &dyn Fn(&str) -> Option<String>, debug: bool) -> Result<String, Error> {
    let (toks, files) = lexer::lex_project(main, load)?;
    let prog = parser::Parser::new(toks, files).program()?;
    Ok(codegen::generate(&prog, debug))
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::process::Command;

    fn err(src: &str) -> Error {
        compile(src).expect_err("expected a compile error")
    }

    /// Compiles `src`, runs it under Node with the real runtime (DOM and Babylon stubbed), and
    /// returns everything it printed plus the final status line. None if Node isn't installed.
    fn run(src: &str) -> Option<String> {
        run_js(&compile(src).unwrap_or_else(|e| panic!("compile error {e:?}")))
    }

    /// Compiles a multi-file project; the first file is the main one.
    fn project(files: &[(&str, &str)], debug: bool) -> Result<String, Error> {
        let load = |name: &str| files.iter().find(|f| f.0 == name).map(|f| f.1.to_string());
        compile_project(files[0].0, &load, debug)
    }

    fn run_js(js: &str) -> Option<String> {
        let stub = r#"
globalThis.BABYLON = {};
globalThis.addEventListener = () => {};
const out = [];
console.log = (s) => out.push(s);
console.error = () => {};
globalThis.document = {
  title: "",
  createElement: () => ({}),
  body: { append(p) { out.push((p.className.includes("err") ? "ERR " : "") + p.textContent); process.stdout.write(out.join("\n")); } },
};
globalThis.requestAnimationFrame = (f) => setImmediate(() => f(performance.now()));
"#;
        let id = format!("{:?}", std::thread::current().id()).replace(|c: char| !c.is_alphanumeric(), "");
        let path = std::env::temp_dir().join(format!("cyberbasic-test-{}-{id}.js", std::process::id()));
        std::fs::write(&path, format!("{stub}\n{RUNTIME_JS}\n{js}")).unwrap();
        let output = Command::new("node").arg(&path).output();
        let _ = std::fs::remove_file(&path);
        match output {
            Ok(o) => Some(String::from_utf8_lossy(&o.stdout).into_owned()),
            Err(_) => {
                eprintln!("node not found, skipping runtime test");
                None
            }
        }
    }

    fn check(src: &str, expected: &str) {
        if let Some(out) = run(src) {
            assert_eq!(out, expected, "program:\n{src}");
        }
    }

    // ---- Compile-time behaviour

    #[test]
    fn examples_compile() {
        for (name, src) in [
            ("cube", include_str!("../../../examples/cube.cb")),
            ("pong", include_str!("../../../examples/pong.cb")),
            ("breakout", include_str!("../../../examples/breakout.cb")),
            ("drift", include_str!("../../../examples/drift.cb")),
            ("marble", include_str!("../../../examples/marble.cb")),
        ] {
            compile(src).unwrap_or_else(|e| panic!("{name}.cb: {e:?}"));
        }
        // The multi-file example, loaded from disk the way the CLI does it.
        let dir = std::path::Path::new(env!("CARGO_MANIFEST_DIR")).join("../../examples/shooter");
        let js = compile_project("main.cb", &|n| std::fs::read_to_string(dir.join(n)).ok(), false).unwrap_or_else(|e| panic!("shooter: {e:?}"));
        assert!(js.contains(r#"CB.files = ["main.cb", "player.cb", "enemies.cb"];"#));
    }

    #[test]
    fn commands_and_keys_are_case_insensitive() {
        let js = compile("c = createcube()\nmoveENTITY c, 1, 2, 3\nx = keydown(key_w)").unwrap();
        assert!(js.contains("CB.CreateCube()"));
        assert!(js.contains("CB.MoveEntity(v_c, 1, 2, 3)"));
        assert!(js.contains("CB.KeyDown(CB.KEY_W)"));
    }

    #[test]
    fn unknown_command_suggests_fix() {
        let e = err("cube = CreateCube()\nMoveEnity cube, 0, 0, 1");
        assert_eq!((e.line, e.col), (2, 1));
        assert!(e.msg.contains("Did you mean `MoveEntity`?"), "{}", e.msg);
    }

    #[test]
    fn arity_and_value_errors() {
        assert!(err("c = CreateCube()\nMoveEntity c, 1").msg.contains("expects 4 arguments, got 2"));
        assert!(err("x = MoveEntity(1, 2, 3, 4)").msg.contains("doesn't return a value"));
        assert!(err("x = CreateCube").msg.contains("needs parentheses"));
        assert!(err("CreateCube = 3").msg.contains("built-in"));
        assert!(err("Function F(a)\nEnd Function\nF 1, 2").msg.contains("expects 1 argument, got 2"));
    }

    #[test]
    fn unclosed_blocks_point_at_their_start() {
        let e = err("x = 1\nWhile x < 3\n  x += 1\n");
        assert_eq!((e.line, e.col, e.msg.as_str()), (2, 1, "missing `Wend` for this `While`"));
        assert_eq!(err("If x\n  x = 1\n").msg, "missing `End If` for this `If`");
        assert_eq!(err("For i = 1 To 3\n").msg, "missing `Next` for this `For`");
        assert_eq!(err("Function F()\n").msg, "missing `End Function` for this `Function`");
        assert_eq!(err("Repeat\n").msg, "missing `Until` or `Forever` for this `Repeat`");
        assert!(err("Wend").msg.contains("no matching block start"));
    }

    #[test]
    fn misuse_errors() {
        assert_eq!(err("Exit").msg, "`Exit` can only be used inside a loop");
        assert!(err("For i = 1 To 2\nNext j").msg.contains("doesn't match"));
        assert!(err("Const A = 1\nA = 2").msg.contains("is a Const"));
        assert!(err("x = New Thing").msg.contains("is not a Type"));
        assert!(err("If 1\nFunction F()\nEnd Function\nEndIf").msg.contains("top level"));
        assert!(err("Function F()\nGlobal x\nEnd Function").msg.contains("top level"));
        assert_eq!(err("Game \"oops").msg, "unterminated string (missing closing `\"`)");
    }

    // Stand-in for the manifest parity check (PLAN.md §6): every compiler builtin and
    // constant must exist in the runtime.
    #[test]
    fn runtime_implements_every_builtin() {
        for b in builtins::BUILTINS {
            let n = b.name;
            let found = [format!("\n    {n}("), format!("\n    *{n}("), format!("\n    {n}:")]
                .iter()
                .any(|p| RUNTIME_JS.contains(p.as_str()));
            assert!(found, "runtime is missing {n}");
            assert_eq!(b.wait, RUNTIME_JS.contains(&format!("\n    *{n}(")), "{n}: wait flag doesn't match the runtime");
        }
        for c in builtins::CONSTANTS {
            assert!(RUNTIME_JS.contains(&format!("{c}:")), "runtime is missing {c}");
        }
    }

    // ---- Runtime behaviour (needs Node)

    #[test]
    fn arithmetic_strings_and_conditions() {
        check(
            "Print 7 Mod 3 + 2 ^ 3 * 2\nPrint -2 ^ 2\nPrint \"a\" + 1 + 2\nPrint Mid(\"Hello\", 2, 3) + Left(\"xy\", 1) + Upper(\"q\")\n\
             If 1 < 2 And Not 0 Then Print \"yes\" Else Print \"no\"",
            "17\n-4\na12\nellxQ\nyes\nProgram ended.",
        );
    }

    #[test]
    fn single_line_if_owns_the_whole_line() {
        check("x = 0\nIf x = 1 Then Print \"a\" : Print \"b\" Else Print \"c\" : Print \"d\"\nPrint \"e\"", "c\nd\ne\nProgram ended.");
    }

    #[test]
    fn loops() {
        check(
            "For i = 1 To 10 Step 4 : Print i : Next\nFor i = 3 To 1 Step -1 : Print i : Next i\n\
             n = 0\nRepeat : n += 1 : Until n = 3\nPrint n\n\
             While 1\n  n += 1\n  If n > 5 Then Exit\nWend\nPrint n\n\
             Repeat\n  n -= 1\n  If n = 0 Then Exit\nForever\nPrint n",
            "1\n5\n9\n3\n2\n1\n3\n6\n0\nProgram ended.",
        );
    }

    #[test]
    fn select_case() {
        check(
            "For i = 1 To 4\n  Select i\n  Case 1, 2\n    Print \"low\"\n  Case 3\n    Print \"three\"\n  Default\n    Print \"other\"\n  End Select\nNext",
            "low\nlow\nthree\nother\nProgram ended.",
        );
    }

    #[test]
    fn functions_locals_and_globals() {
        check(
            "Global g = 1\nx = 5\nPrint Add(2, 3) + \" \" + g + \" \" + x\nBump\nPrint g + \" \" + x\n\
             Function Add(a, b)\n  Return a + b\nEnd Function\n\
             Function Bump()\n  g += 1\n  x = 100\nEnd Function\n\
             Function Fact(n)\n  If n <= 1 Then Return 1\n  Return n * Fact(n - 1)\nEnd Function\nPrint Fact(5)",
            "5 1 5\n2 5\n120\nProgram ended.",
        );
    }

    #[test]
    fn arrays() {
        check(
            "Dim a[3]\na[0] = 5 : a[2] += 7\nDim grid[2, 3]\ngrid[1, 2] = 9\nPrint a[0] + a[1] + a[2] + grid[1, 2]\nShow\n\
             Function Show()\n  Print a[2]\nEnd Function\na[3] = 1",
            "21\n7\nERR Runtime error on line 10: array index 3 is out of range (0 to 2)",
        );
    }

    #[test]
    fn types_new_each_delete() {
        check(
            "Type Enemy\n  Field name, hp\nEnd Type\n\
             For i = 1 To 3 : e = New Enemy : e.name = \"e\" + i : e.hp = i : Next\n\
             For e = Each Enemy\n  If e.hp = 2 Then Delete e\nNext\n\
             For e = Each Enemy : Print e.name : Next",
            "e1\ne3\nProgram ended.",
        );
    }

    #[test]
    fn end_inside_a_function_stops_the_program() {
        check("Stop\nPrint \"never\"\nFunction Stop()\n  Print \"bye\"\n  End\nEnd Function", "bye\nProgram ended.");
    }

    #[test]
    fn manifest_is_documented_and_json_safe() {
        for b in builtins::BUILTINS {
            assert!(!b.doc.is_empty(), "{} has no doc", b.name);
            for s in [b.doc, b.args, b.category] {
                assert!(s.is_ascii() && !s.contains(['"', '\\']), "{}: unsafe text {s:?}", b.name);
            }
        }
        let json = builtins::manifest_json();
        assert!(json.contains(r#"{"name":"Rect","category":"2D Drawing","args":"x, y, width, height[, fill]""#));
        assert!(json.contains(r#""KEY_Z""#));
    }

    #[test]
    fn debug_builds_pause_with_a_variable_snapshot() {
        let js = compile_with("Global g\nx = 1\nF 2\nFunction F(a)\n  b = a\nEnd Function", true).unwrap();
        assert!(js.contains(r#"if (CB.trap) yield* CB.pause(2, "main", () => ({ g: g_g, x: v_x }));"#), "{js}");
        assert!(js.contains(r#"CB.pause(5, "f", () => ({ a: v_a, b: v_b, g: g_g }))"#), "{js}");
        assert!(!compile("x = 1").unwrap().contains("CB.pause"));
    }

    // ---- Multi-file projects

    const UTIL: &str = "Function Double(n)\n  Return n * 2\nEnd Function\nGlobal ready = 1";

    #[test]
    fn include_splices_files_in_place_once() {
        let main = "Print Double(2)\nInclude \"util.cb\"\nInclude \"util.cb\"\nPrint ready";
        let js = project(&[("main.cb", main), ("util.cb", UTIL)], false).unwrap();
        assert!(js.contains(r#"CB.files = ["main.cb", "util.cb"];"#), "{js}");
        assert_eq!(js.matches("function* f_double").count(), 1, "included twice");
        // A function from an included file can be called above the Include line.
        if let Some(out) = run_js(&js) {
            assert_eq!(out, "4\n1\nProgram ended.");
        }
    }

    #[test]
    fn include_cycles_and_nesting() {
        let a = "Include \"b.cb\"\nPrint \"a\"";
        let b = "Include \"a.cb\"\nInclude \"c.cb\"\nPrint \"b\"";
        let js = project(&[("a.cb", a), ("b.cb", b), ("c.cb", "Print \"c\"")], false).unwrap();
        if let Some(out) = run_js(&js) {
            assert_eq!(out, "c\nb\na\nProgram ended.");
        }
    }

    #[test]
    fn errors_name_the_file_they_are_in() {
        let e = project(&[("main.cb", "Include \"util.cb\""), ("util.cb", "x = 1\nRecct 1, 2, 3, 4")], false).unwrap_err();
        assert_eq!((e.file.as_str(), e.line, e.col), ("util.cb", 2, 1));
        let e = project(&[("main.cb", "x = 1\nInclude \"missing.cb\"")], false).unwrap_err();
        assert_eq!((e.file.as_str(), e.line), ("main.cb", 2));
        assert!(e.msg.contains("can't find \"missing.cb\""), "{}", e.msg);
        let e = project(&[("main.cb", "If 1 Then Include \"util.cb\"")], false).unwrap_err();
        assert!(e.msg.contains("own line"), "{}", e.msg);
        assert_eq!(compile("Recct 1, 2, 3, 4").unwrap_err().file, "main.cb");
    }

    #[test]
    fn statements_carry_their_file_in_debug_hooks_and_runtime_errors() {
        let js = project(&[("main.cb", "Include \"util.cb\"\nx = Double(1)"), ("util.cb", UTIL)], true).unwrap();
        assert!(js.contains(&format!("CB.pause({}, \"double\"", LOC_FILE + 2)), "{js}");
        assert!(js.contains("CB.pause(2, \"main\""), "{js}");
        let js = project(&[("main.cb", "Include \"util.cb\"\nBoom"), ("util.cb", "Function Boom()\n  Dim a[1]\n  a[5] = 1\nEnd Function")], false).unwrap();
        if let Some(out) = run_js(&js) {
            assert_eq!(out, "ERR Runtime error on util.cb line 3: array index 5 is out of range (0 to 0)");
        }
    }

    #[test]
    fn everyframe_runs_a_function_at_every_sync() {
        check(
            "Global n\nFunction Tick()\n  n += 1\nEnd Function\nEveryFrame Tick\nFor i = 1 To 3 : Sync : Next\nPrint n",
            "3\nProgram ended.",
        );
        assert!(err("EveryFrame Nope").msg.contains("not one of your functions"));
        assert!(err("Function F(a)\nEnd Function\nEveryFrame F").msg.contains("takes inputs"));
    }

    #[test]
    fn delay_waits_across_frames() {
        check("t = Millisecs()\nDelay 30\nPrint Millisecs() - t >= 30", "true\nProgram ended.");
    }
}

//! CyberBasic compiler, Phase 0 walking skeleton: source -> tokens -> AST -> JavaScript.

pub mod builtins;
mod codegen;
mod lexer;
mod parser;

/// Runtime and page template, embedded so the CLI is a single self-contained binary.
pub const RUNTIME_JS: &str = include_str!("../../../runtime/runtime.js");
pub const INDEX_HTML: &str = include_str!("../../../runtime/index.html");

#[derive(Debug, PartialEq)]
pub struct Error {
    pub line: usize,
    pub col: usize,
    pub msg: String,
}

impl Error {
    pub fn new(line: usize, col: usize, msg: impl Into<String>) -> Self {
        Error { line, col, msg: msg.into() }
    }
}

pub fn compile(src: &str) -> Result<String, Error> {
    let toks = lexer::lex(src)?;
    let prog = parser::Parser::new(toks).program()?;
    Ok(codegen::generate(&prog))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn err(src: &str) -> Error {
        compile(src).expect_err("expected a compile error")
    }

    #[test]
    fn cube_example_compiles() {
        let js = compile(include_str!("../../../examples/cube.cb")).unwrap();
        assert!(js.contains("yield;"));
        assert!(js.contains("CB.MoveEntity(v_cube, 0, 0, (5 * CB.delta));"));
        assert!(js.contains("while ((!CB.KeyHit(CB.KEY_ESCAPE)))"));
    }

    #[test]
    fn commands_and_keys_are_case_insensitive() {
        let js = compile("c = createcube()\nmoveENTITY c, 1, 2, 3\nx = keydown(key_w)").unwrap();
        assert!(js.contains("CB.CreateCube()"));
        assert!(js.contains("CB.MoveEntity(v_c, 1, 2, 3)"));
        assert!(js.contains("CB.KeyDown(CB.KEY_W)"));
    }

    #[test]
    fn single_line_if_else_and_compound_assign() {
        let js = compile("If x > 1 Then x -= 1 Else x = 10").unwrap();
        assert!(js.contains("if ((v_x > 1)) {"));
        assert!(js.contains("v_x -= 1;"));
        assert!(js.contains("} else {"));
        assert!(js.contains("v_x = 10;"));
    }

    #[test]
    fn block_if_elseif_and_end_if_spellings() {
        let src = "If a = 1\n  b = 1\nElseIf a = 2\n  b = 2\nElse\n  b = 3\nEnd If\nIf b Then\nEndIf";
        let js = compile(src).unwrap();
        assert_eq!(js.matches("if (").count(), 3);
        assert!(js.contains("(v_a === 1)"));
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
        assert!(err("CreateCube = 3").msg.contains("built-in name"));
    }

    #[test]
    fn unclosed_blocks_point_at_their_start() {
        let e = err("x = 1\nWhile x < 3\n  x += 1\n");
        assert_eq!((e.line, e.col, e.msg.as_str()), (2, 1, "missing `Wend` for this `While`"));
        assert_eq!(err("If x\n  x = 1\n").msg, "missing `EndIf` for this `If`");
        assert!(err("Wend").msg.contains("no matching"));
    }

    #[test]
    fn strings_comments_and_colons() {
        // BASIC strings have no escapes; a backslash must survive into valid JS.
        let js = compile("Game \"C:\\games\" ' comment\nx = 1 : y = 2").unwrap();
        assert!(js.contains(r#"CB.Game("C:\\games");"#), "{js}");
        assert!(js.contains("v_x = 1;") && js.contains("v_y = 2;"));
        assert_eq!(err("Game \"oops").msg, "unterminated string (missing closing `\"`)");
    }

    // Stand-in for the manifest parity check (PLAN.md §6): every compiler builtin and
    // constant must exist in the runtime.
    #[test]
    fn runtime_implements_every_builtin() {
        for b in builtins::BUILTINS {
            assert!(RUNTIME_JS.contains(&format!("\n    {}(", b.name)), "runtime is missing {}", b.name);
        }
        for c in builtins::CONSTANTS {
            assert!(RUNTIME_JS.contains(&format!("{c}:")), "runtime is missing {c}");
        }
    }
}

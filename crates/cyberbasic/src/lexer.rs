use crate::Error;

#[derive(Debug, Clone, PartialEq)]
pub enum Tok {
    Num(String),
    Str(String),
    Ident(String),
    Op(&'static str),
    Nl,
    Eof,
}

#[derive(Debug, Clone)]
pub struct Token {
    pub tok: Tok,
    pub line: usize,
    pub col: usize,
    /// Index of the source file (0 = main) in the order files were included.
    pub file: usize,
}

/// Lines in generated code are `file * LOC_FILE + line`, so one number says where a statement is.
/// The main file is 0, so single-file programs just use plain line numbers.
pub const LOC_FILE: usize = 1_000_000;

impl Token {
    pub fn loc(&self) -> usize {
        self.file * LOC_FILE + self.line
    }
}

// Two-char operators first so `<=` wins over `<`. `:` separates statements on one line.
const OPS: [&str; 20] = [
    "<>", "<=", ">=", "+=", "-=", "=", "<", ">", "+", "-", "*", "/", "^", "(", ")", "[", "]", ",", ".", ":",
];

pub fn lex(src: &str, file: usize) -> Result<Vec<Token>, Error> {
    let cs: Vec<char> = src.chars().collect();
    let mut out = Vec::new();
    let (mut i, mut line, mut line_start) = (0, 1, 0);
    while i < cs.len() {
        let (c, start, ln) = (cs[i], i, line);
        let col = i - line_start + 1;
        let tok = if c == '\n' {
            i += 1;
            line += 1;
            line_start = i;
            Tok::Nl
        } else if c.is_whitespace() {
            i += 1;
            continue;
        } else if c == '\'' {
            while i < cs.len() && cs[i] != '\n' {
                i += 1;
            }
            continue;
        } else if c.is_ascii_digit() || (c == '.' && cs.get(i + 1).is_some_and(|d| d.is_ascii_digit())) {
            let mut dot = false;
            while i < cs.len() && (cs[i].is_ascii_digit() || cs[i] == '.') {
                if cs[i] == '.' {
                    if dot {
                        return Err(Error::new(ln, col, "number has two decimal points"));
                    }
                    dot = true;
                }
                i += 1;
            }
            Tok::Num(cs[start..i].iter().collect())
        } else if c == '"' {
            i += 1;
            while i < cs.len() && cs[i] != '"' && cs[i] != '\n' {
                i += 1;
            }
            if cs.get(i) != Some(&'"') {
                return Err(Error::new(ln, col, "unterminated string (missing closing `\"`)"));
            }
            i += 1;
            Tok::Str(cs[start + 1..i - 1].iter().collect())
        } else if c.is_ascii_alphabetic() || c == '_' {
            while i < cs.len() && (cs[i].is_ascii_alphanumeric() || cs[i] == '_') {
                i += 1;
            }
            let name = cs[start..i].iter().collect();
            // Blitz type suffixes (x#, n%, s$) are accepted and ignored until the type checker exists.
            if i < cs.len() && matches!(cs[i], '#' | '%' | '$') {
                i += 1;
            }
            Tok::Ident(name)
        } else if let Some(op) = OPS.iter().find(|op| starts_with(&cs[i..], op)) {
            i += op.len();
            Tok::Op(op)
        } else {
            return Err(Error::new(ln, col, format!("unexpected character `{c}`")));
        };
        out.push(Token { tok, line: ln, col, file });
    }
    out.push(Token { tok: Tok::Eof, line, col: i - line_start + 1, file });
    Ok(out)
}

fn starts_with(cs: &[char], s: &str) -> bool {
    cs.len() >= s.len() && s.chars().zip(cs).all(|(a, b)| a == *b)
}

/// Lexes `main` and splices in every `Include "file.cb"` line (Blitz style: the file's code goes
/// right there). Each file is included once, which also makes include cycles harmless.
/// Returns the tokens and the file names in index order.
pub fn lex_project(main: &str, load: &dyn Fn(&str) -> Option<String>) -> Result<(Vec<Token>, Vec<String>), Error> {
    let mut files = Vec::new();
    let src = load(main).ok_or_else(|| Error::new(0, 0, format!("can't read {main}")))?;
    let toks = include(main, &src, load, &mut files)?;
    Ok((toks, files))
}

fn include(name: &str, src: &str, load: &dyn Fn(&str) -> Option<String>, files: &mut Vec<String>) -> Result<Vec<Token>, Error> {
    let idx = files.len();
    files.push(name.to_string());
    let toks = lex(src, idx).map_err(|e| e.in_file(name))?;
    let mut out = Vec::with_capacity(toks.len());
    let mut i = 0;
    while i < toks.len() {
        let t = &toks[i];
        let at_line_start = i == 0 || toks[i - 1].tok == Tok::Nl;
        let is_include = matches!(&t.tok, Tok::Ident(k) if k.eq_ignore_ascii_case("include"));
        if let (true, true, Some(Tok::Str(path))) = (at_line_start, is_include, toks.get(i + 1).map(|t| &t.tok)) {
            if !files.iter().any(|f| f.eq_ignore_ascii_case(path)) {
                let Some(inner_src) = load(path) else {
                    return Err(Error::new(t.line, t.col, format!("can't find \"{path}\" to include (paths are relative to the project folder)")).in_file(name));
                };
                let mut inner = include(path, &inner_src, load, files)?;
                inner.pop(); // its Eof
                out.extend(inner);
                out.push(Token { tok: Tok::Nl, line: t.line, col: t.col, file: t.file });
            }
            i += 2;
            continue;
        }
        out.push(t.clone());
        i += 1;
    }
    Ok(out)
}

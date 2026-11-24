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
}

// Two-char operators first so `<=` wins over `<`. `:` separates statements on one line.
const OPS: [&str; 20] = [
    "<>", "<=", ">=", "+=", "-=", "=", "<", ">", "+", "-", "*", "/", "^", "(", ")", "[", "]", ",", ".", ":",
];

pub fn lex(src: &str) -> Result<Vec<Token>, Error> {
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
        out.push(Token { tok, line: ln, col });
    }
    out.push(Token { tok: Tok::Eof, line, col: i - line_start + 1 });
    Ok(out)
}

fn starts_with(cs: &[char], s: &str) -> bool {
    cs.len() >= s.len() && s.chars().zip(cs).all(|(a, b)| a == *b)
}

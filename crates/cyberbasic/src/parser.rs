use crate::builtins::{self, Builtin, KEYWORDS};
use crate::lexer::{Tok, Token};
use crate::Error;

#[derive(Debug)]
pub enum Expr {
    Num(String),
    Str(String),
    Var(String),
    Const(String),
    Delta,
    Call(&'static str, Vec<Expr>),
    Field(Box<Expr>, String),
    Neg(Box<Expr>),
    Not(Box<Expr>),
    Bin(&'static str, Box<Expr>, Box<Expr>), // op is already the JS operator
}

#[derive(Debug)]
pub struct Stmt {
    pub line: usize,
    pub kind: Kind,
}

#[derive(Debug)]
pub enum Kind {
    Assign(Expr, &'static str, Expr),
    Cmd(&'static str, Vec<Expr>),
    If(Expr, Vec<Stmt>, Vec<Stmt>),
    While(Expr, Vec<Stmt>),
    Sync,
    End,
}

type R<T> = Result<T, Error>;

pub struct Parser {
    toks: Vec<Token>,
    pos: usize,
}

fn describe(t: &Tok) -> String {
    match t {
        Tok::Num(n) => format!("`{n}`"),
        Tok::Str(_) => "a string".into(),
        Tok::Ident(s) => format!("`{s}`"),
        Tok::Op(o) => format!("`{o}`"),
        Tok::Nl => "end of line".into(),
        Tok::Eof => "end of file".into(),
    }
}

fn err_at<T>(t: &Token, msg: impl Into<String>) -> R<T> {
    Err(Error::new(t.line, t.col, msg))
}

impl Parser {
    /// `toks` must end with `Tok::Eof` (the lexer guarantees it).
    pub fn new(toks: Vec<Token>) -> Self {
        Parser { toks, pos: 0 }
    }

    fn tok(&self) -> &Token {
        &self.toks[self.pos]
    }
    fn peek_at(&self, n: usize) -> &Tok {
        &self.toks[(self.pos + n).min(self.toks.len() - 1)].tok
    }
    fn bump(&mut self) -> Token {
        let t = self.toks[self.pos].clone();
        if self.pos < self.toks.len() - 1 {
            self.pos += 1;
        }
        t
    }
    fn kw_at(&self, n: usize, kw: &str) -> bool {
        matches!(self.peek_at(n), Tok::Ident(s) if s.eq_ignore_ascii_case(kw))
    }
    fn kw(&self, kw: &str) -> bool {
        self.kw_at(0, kw)
    }
    fn op(&self, op: &str) -> bool {
        matches!(self.peek_at(0), Tok::Op(o) if *o == op)
    }
    fn eat_op(&mut self, op: &str) -> bool {
        let hit = self.op(op);
        if hit {
            self.bump();
        }
        hit
    }
    fn expect_op(&mut self, op: &str) -> R<()> {
        if self.eat_op(op) {
            return Ok(());
        }
        err_at(self.tok(), format!("expected `{op}`, found {}", describe(self.peek_at(0))))
    }
    fn at_line_end(&self) -> bool {
        matches!(self.peek_at(0), Tok::Nl | Tok::Eof)
    }
    fn at_stmt_end(&self) -> bool {
        self.at_line_end() || self.kw("else")
    }
    fn at_endif(&self) -> bool {
        self.kw("endif") || (self.kw("end") && self.kw_at(1, "if"))
    }

    pub fn program(&mut self) -> R<Vec<Stmt>> {
        let body = self.block()?;
        if !matches!(self.peek_at(0), Tok::Eof) {
            let what = describe(self.peek_at(0));
            return err_at(self.tok(), format!("unexpected {what} with no matching `If` or `While`"));
        }
        Ok(body)
    }

    /// Statements until end of file or a block terminator; the caller checks which one.
    fn block(&mut self) -> R<Vec<Stmt>> {
        let mut out = Vec::new();
        loop {
            while self.peek_at(0) == &Tok::Nl {
                self.bump();
            }
            let done = matches!(self.peek_at(0), Tok::Eof)
                || self.kw("else")
                || self.kw("elseif")
                || self.kw("wend")
                || self.at_endif();
            if done {
                return Ok(out);
            }
            out.push(self.statement()?);
            if !self.at_line_end() {
                return err_at(self.tok(), format!("unexpected {}", describe(self.peek_at(0))));
            }
        }
    }

    fn statement(&mut self) -> R<Stmt> {
        let start = self.tok().clone();
        let Tok::Ident(name) = &start.tok else {
            return err_at(&start, format!("expected a statement, found {}", describe(&start.tok)));
        };
        self.bump();
        let kind = match name.to_ascii_lowercase().as_str() {
            "if" => self.if_rest(&start)?,
            "while" => {
                let cond = self.expr()?;
                let body = self.block()?;
                if !self.kw("wend") {
                    return err_at(&start, "missing `Wend` for this `While`");
                }
                self.bump();
                Kind::While(cond, body)
            }
            "sync" => Kind::Sync,
            "end" => Kind::End,
            _ => self.ident_stmt(name, &start)?,
        };
        Ok(Stmt { line: start.line, kind })
    }

    fn if_rest(&mut self, start: &Token) -> R<Kind> {
        let cond = self.expr()?;
        if self.kw("then") {
            self.bump();
        } else if !self.at_line_end() {
            return err_at(self.tok(), "expected `Then` or end of line after the `If` condition");
        }
        if !self.at_line_end() {
            // Single-line form: If cond Then stmt [Else stmt]
            let then = vec![self.statement()?];
            let els = if self.kw("else") {
                self.bump();
                vec![self.statement()?]
            } else {
                vec![]
            };
            return Ok(Kind::If(cond, then, els));
        }
        let then = self.block()?;
        let els = if self.kw("elseif") {
            let t = self.bump();
            vec![Stmt { line: t.line, kind: self.if_rest(&t)? }] // nested If consumes the EndIf
        } else if self.kw("else") {
            self.bump();
            let b = self.block()?;
            self.end_if(start)?;
            b
        } else {
            self.end_if(start)?;
            vec![]
        };
        Ok(Kind::If(cond, then, els))
    }

    fn end_if(&mut self, start: &Token) -> R<()> {
        if self.kw("endif") {
            self.bump();
        } else if self.at_endif() {
            self.bump();
            self.bump();
        } else {
            return err_at(start, "missing `EndIf` for this `If`");
        }
        Ok(())
    }

    fn ident_stmt(&mut self, name: &str, start: &Token) -> R<Kind> {
        let low = name.to_ascii_lowercase();
        if ["=", "+=", "-=", "."].iter().any(|o| self.op(o)) {
            if builtins::find(name).is_some() || builtins::constant(name).is_some() || KEYWORDS.contains(&low.as_str()) {
                return err_at(start, format!("`{name}` is a built-in name and can't be used as a variable"));
            }
            let mut target = Expr::Var(low);
            while self.eat_op(".") {
                target = Expr::Field(Box::new(target), self.field()?);
            }
            let op = match self.peek_at(0) {
                Tok::Op(o @ ("=" | "+=" | "-=")) => *o,
                t => return err_at(self.tok(), format!("expected `=`, found {}", describe(t))),
            };
            self.bump();
            return Ok(Kind::Assign(target, op, self.expr()?));
        }
        if KEYWORDS.contains(&low.as_str()) {
            return err_at(start, format!("unexpected `{name}`"));
        }
        let Some(b) = builtins::find(name) else {
            let hint = builtins::suggest(name).map(|s| format!(". Did you mean `{s}`?")).unwrap_or_default();
            return err_at(start, format!("`{name}` is not a command{hint}"));
        };
        let args = if self.op("(") && self.peek_at(1) == &Tok::Op(")") {
            self.bump();
            self.bump();
            vec![]
        } else if self.at_stmt_end() {
            vec![]
        } else {
            self.arg_list()?
        };
        check_arity(b, args.len(), start)?;
        Ok(Kind::Cmd(b.name, args))
    }

    fn field(&mut self) -> R<String> {
        let t = self.bump();
        match &t.tok {
            Tok::Ident(s) => Ok(s.to_ascii_lowercase()),
            other => err_at(&t, format!("expected a field name after `.`, found {}", describe(other))),
        }
    }

    fn arg_list(&mut self) -> R<Vec<Expr>> {
        let mut args = vec![self.expr()?];
        while self.eat_op(",") {
            args.push(self.expr()?);
        }
        Ok(args)
    }

    fn expr(&mut self) -> R<Expr> {
        self.or()
    }

    fn or(&mut self) -> R<Expr> {
        let mut l = self.and()?;
        while self.kw("or") {
            self.bump();
            l = Expr::Bin("||", Box::new(l), Box::new(self.and()?));
        }
        Ok(l)
    }

    fn and(&mut self) -> R<Expr> {
        let mut l = self.not()?;
        while self.kw("and") {
            self.bump();
            l = Expr::Bin("&&", Box::new(l), Box::new(self.not()?));
        }
        Ok(l)
    }

    fn not(&mut self) -> R<Expr> {
        if self.kw("not") {
            self.bump();
            return Ok(Expr::Not(Box::new(self.not()?)));
        }
        self.cmp()
    }

    fn cmp(&mut self) -> R<Expr> {
        let mut l = self.add()?;
        loop {
            let op = match self.peek_at(0) {
                Tok::Op("=") => "===",
                Tok::Op("<>") => "!==",
                Tok::Op(o @ ("<" | "<=" | ">" | ">=")) => *o,
                _ => return Ok(l),
            };
            self.bump();
            l = Expr::Bin(op, Box::new(l), Box::new(self.add()?));
        }
    }

    fn add(&mut self) -> R<Expr> {
        let mut l = self.mul()?;
        while let Tok::Op(o @ ("+" | "-")) = self.peek_at(0) {
            let o = *o;
            self.bump();
            l = Expr::Bin(o, Box::new(l), Box::new(self.mul()?));
        }
        Ok(l)
    }

    fn mul(&mut self) -> R<Expr> {
        let mut l = self.unary()?;
        while let Tok::Op(o @ ("*" | "/")) = self.peek_at(0) {
            let o = *o;
            self.bump();
            l = Expr::Bin(o, Box::new(l), Box::new(self.unary()?));
        }
        Ok(l)
    }

    fn unary(&mut self) -> R<Expr> {
        if self.eat_op("-") {
            return Ok(Expr::Neg(Box::new(self.unary()?)));
        }
        let mut e = self.primary()?;
        while self.eat_op(".") {
            e = Expr::Field(Box::new(e), self.field()?);
        }
        Ok(e)
    }

    fn primary(&mut self) -> R<Expr> {
        let t = self.bump();
        match &t.tok {
            Tok::Num(n) => Ok(Expr::Num(n.clone())),
            Tok::Str(s) => Ok(Expr::Str(s.clone())),
            Tok::Op("(") => {
                let e = self.expr()?;
                self.expect_op(")")?;
                Ok(e)
            }
            Tok::Ident(s) => self.name_expr(s, &t),
            other => err_at(&t, format!("expected an expression, found {}", describe(other))),
        }
    }

    fn name_expr(&mut self, name: &str, t: &Token) -> R<Expr> {
        let low = name.to_ascii_lowercase();
        if low == "delta" {
            return Ok(Expr::Delta);
        }
        if KEYWORDS.contains(&low.as_str()) {
            return err_at(t, format!("expected an expression, found `{name}`"));
        }
        if let Some(c) = builtins::constant(name) {
            return Ok(Expr::Const(c));
        }
        let Some(b) = builtins::find(name) else {
            return Ok(Expr::Var(low));
        };
        if !b.returns {
            return err_at(t, format!("`{}` doesn't return a value, so it can't be used in an expression", b.name));
        }
        if !self.eat_op("(") {
            return err_at(t, format!("`{0}` needs parentheses here: `{0}(...)`", b.name));
        }
        let args = if self.eat_op(")") {
            vec![]
        } else {
            let a = self.arg_list()?;
            self.expect_op(")")?;
            a
        };
        check_arity(b, args.len(), t)?;
        Ok(Expr::Call(b.name, args))
    }
}

fn check_arity(b: &Builtin, n: usize, at: &Token) -> R<()> {
    if (b.min..=b.max).contains(&n) {
        return Ok(());
    }
    let want = if b.min == b.max { b.min.to_string() } else { format!("{} to {}", b.min, b.max) };
    let s = if want == "1" { "" } else { "s" };
    err_at(at, format!("`{}` expects {want} argument{s}, got {n}", b.name))
}

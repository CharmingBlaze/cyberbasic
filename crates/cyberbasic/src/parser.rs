use crate::builtins::{self, is_keyword, Builtin};
use crate::lexer::{Tok, Token};
use crate::Error;
use std::collections::{BTreeMap, BTreeSet};

#[derive(Debug)]
pub enum Expr {
    Num(String),
    Str(String),
    Var(String),
    Const(String),
    Delta,
    Call(&'static Builtin, Vec<Expr>),
    UserCall(String, Vec<Expr>),
    New(String),
    Field(Box<Expr>, String),
    Index(Box<Expr>, Box<Expr>),
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
    Cmd(&'static Builtin, Vec<Expr>),
    UserCall(String, Vec<Expr>),
    If(Expr, Vec<Stmt>, Vec<Stmt>),
    While(Expr, Vec<Stmt>),
    Repeat(Vec<Stmt>, Option<Expr>), // None = Forever
    For { var: String, from: Expr, to: Expr, step: Option<Expr>, body: Vec<Stmt> },
    ForEach { var: String, ty: String, body: Vec<Stmt> },
    Select(Expr, Vec<(Vec<Expr>, Vec<Stmt>)>, Vec<Stmt>),
    Dim(String, Vec<Expr>),
    Delete(Expr),
    Return(Option<Expr>),
    Seq(Vec<Stmt>),
    Exit,
    Sync,
    End,
    Nop,
}

pub struct Func {
    pub name: String,
    pub params: Vec<String>,
    pub body: Vec<Stmt>,
}

pub struct TypeDef {
    pub name: String,
    pub fields: Vec<String>,
}

pub struct Program {
    pub main: Vec<Stmt>,
    pub funcs: Vec<Func>,
    pub types: Vec<TypeDef>,
    pub globals: BTreeSet<String>,
}

type R<T> = Result<T, Error>;

pub struct Parser {
    toks: Vec<Token>,
    pos: usize,
    fn_arity: BTreeMap<String, usize>, // pre-scanned so functions can be called before they're defined
    type_names: BTreeSet<String>,
    funcs: Vec<Func>,
    types: Vec<TypeDef>,
    globals: BTreeSet<String>,
    consts: BTreeSet<String>,
    in_func: bool,
    depth: usize,
    loops: usize,
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

const TERMINATORS: &[&str] = &["else", "elseif", "endif", "wend", "next", "until", "forever", "case", "default"];
const END_PAIRS: &[&str] = &["if", "function", "select", "type"];

impl Parser {
    /// `toks` must end with `Tok::Eof` (the lexer guarantees it).
    pub fn new(toks: Vec<Token>) -> Self {
        let mut fn_arity = BTreeMap::new();
        let mut type_names = BTreeSet::new();
        for i in 0..toks.len() {
            if i > 0 && toks[i - 1].tok != Tok::Nl {
                continue;
            }
            let (Tok::Ident(kw), Some(Tok::Ident(name))) = (&toks[i].tok, toks.get(i + 1).map(|t| &t.tok)) else {
                continue;
            };
            if kw.eq_ignore_ascii_case("type") {
                type_names.insert(name.to_ascii_lowercase());
            } else if kw.eq_ignore_ascii_case("function") {
                let mut n = 0;
                let mut j = i + 2;
                if toks.get(j).map(|t| &t.tok) == Some(&Tok::Op("(")) {
                    j += 1;
                    while let Some(Tok::Ident(_)) = toks.get(j).map(|t| &t.tok) {
                        n += 1;
                        j += 1;
                        if toks.get(j).map(|t| &t.tok) != Some(&Tok::Op(",")) {
                            break;
                        }
                        j += 1;
                    }
                }
                fn_arity.insert(name.to_ascii_lowercase(), n);
            }
        }
        Parser {
            toks,
            pos: 0,
            fn_arity,
            type_names,
            funcs: vec![],
            types: vec![],
            globals: BTreeSet::new(),
            consts: BTreeSet::new(),
            in_func: false,
            depth: 0,
            loops: 0,
        }
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
    fn end_kw(&self, what: &str) -> bool {
        (self.kw("end") && self.kw_at(1, what)) || (what == "if" && self.kw("endif"))
    }
    fn eat_end(&mut self, what: &str, start: &Token, opener: &str) -> R<()> {
        if !self.end_kw(what) {
            return err_at(start, format!("missing `End {opener}` for this `{opener}`"));
        }
        if !self.kw("endif") {
            self.bump();
        }
        self.bump();
        Ok(())
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
        self.at_line_end() || self.op(":") || self.kw("else")
    }
    fn at_terminator(&self) -> bool {
        matches!(self.peek_at(0), Tok::Eof)
            || TERMINATORS.iter().any(|k| self.kw(k))
            || (self.kw("end") && END_PAIRS.iter().any(|k| self.kw_at(1, k)))
    }

    pub fn program(mut self) -> R<Program> {
        let main = self.block()?;
        if !matches!(self.peek_at(0), Tok::Eof) {
            let what = describe(self.peek_at(0));
            return err_at(self.tok(), format!("unexpected {what} with no matching block start"));
        }
        Ok(Program { main, funcs: self.funcs, types: self.types, globals: self.globals })
    }

    /// Statements until end of file or a block terminator; the caller checks which one.
    fn block(&mut self) -> R<Vec<Stmt>> {
        self.depth += 1;
        let mut out = Vec::new();
        loop {
            while matches!(self.peek_at(0), Tok::Nl | Tok::Op(":")) {
                self.bump();
            }
            if self.at_terminator() {
                self.depth -= 1;
                return Ok(out);
            }
            out.push(self.statement()?);
            if !self.at_line_end() && !self.op(":") {
                return err_at(self.tok(), format!("unexpected {}", describe(self.peek_at(0))));
            }
        }
    }

    fn loop_block(&mut self) -> R<Vec<Stmt>> {
        self.loops += 1;
        let b = self.block();
        self.loops -= 1;
        b
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
                let body = self.loop_block()?;
                if !self.kw("wend") {
                    return err_at(&start, "missing `Wend` for this `While`");
                }
                self.bump();
                Kind::While(cond, body)
            }
            "repeat" => {
                let body = self.loop_block()?;
                if self.kw("until") {
                    self.bump();
                    Kind::Repeat(body, Some(self.expr()?))
                } else if self.kw("forever") {
                    self.bump();
                    Kind::Repeat(body, None)
                } else {
                    return err_at(&start, "missing `Until` or `Forever` for this `Repeat`");
                }
            }
            "for" => self.for_rest(&start)?,
            "select" => self.select_rest(&start)?,
            "function" => self.function(&start)?,
            "type" => self.type_def(&start)?,
            "global" | "const" => self.global(&start, name.eq_ignore_ascii_case("const"))?,
            "dim" => {
                let (name, _) = self.var_name()?;
                self.expect_op("[")?;
                let dims = self.arg_list()?;
                self.expect_op("]")?;
                if !self.in_func {
                    self.globals.insert(name.clone()); // Blitz: Dim arrays in the main program are global
                }
                Kind::Dim(name, dims)
            }
            "delete" => Kind::Delete(self.expr()?),
            "return" => Kind::Return(if self.at_stmt_end() { None } else { Some(self.expr()?) }),
            "exit" => {
                if self.loops == 0 {
                    return err_at(&start, "`Exit` can only be used inside a loop");
                }
                Kind::Exit
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
            // Single-line form: everything up to the end of the line belongs to the If, as in Blitz.
            let then = self.line_stmts()?;
            let els = if self.kw("else") {
                self.bump();
                self.line_stmts()?
            } else {
                vec![]
            };
            return Ok(Kind::If(cond, then, els));
        }
        let then = self.block()?;
        let els = if self.kw("elseif") {
            let t = self.bump();
            vec![Stmt { line: t.line, kind: self.if_rest(start)? }] // nested If consumes the EndIf
        } else if self.kw("else") {
            self.bump();
            let b = self.block()?;
            self.eat_end("if", start, "If")?;
            b
        } else {
            self.eat_end("if", start, "If")?;
            vec![]
        };
        Ok(Kind::If(cond, then, els))
    }

    /// `stmt : stmt : ...` up to the end of the line or `Else`.
    fn line_stmts(&mut self) -> R<Vec<Stmt>> {
        let mut out = vec![self.statement()?];
        while self.eat_op(":") {
            out.push(self.statement()?);
        }
        Ok(out)
    }

    fn for_rest(&mut self, start: &Token) -> R<Kind> {
        let (var, _) = self.var_name()?;
        self.expect_op("=")?;
        if self.kw("each") {
            self.bump();
            let ty = self.type_name()?;
            let body = self.loop_block()?;
            self.eat_next(start, &var)?;
            return Ok(Kind::ForEach { var, ty, body });
        }
        let from = self.expr()?;
        if !self.kw("to") {
            return err_at(self.tok(), format!("expected `To`, found {}", describe(self.peek_at(0))));
        }
        self.bump();
        let to = self.expr()?;
        let step = if self.kw("step") {
            self.bump();
            Some(self.expr()?)
        } else {
            None
        };
        let body = self.loop_block()?;
        self.eat_next(start, &var)?;
        Ok(Kind::For { var, from, to, step, body })
    }

    fn eat_next(&mut self, start: &Token, var: &str) -> R<()> {
        if !self.kw("next") {
            return err_at(start, "missing `Next` for this `For`");
        }
        self.bump();
        if let Tok::Ident(n) = self.peek_at(0).clone() {
            if !n.eq_ignore_ascii_case(var) {
                return err_at(self.tok(), format!("`Next {n}` doesn't match `For {var}`"));
            }
            self.bump();
        }
        Ok(())
    }

    fn select_rest(&mut self, start: &Token) -> R<Kind> {
        let subject = self.expr()?;
        let (mut cases, mut default) = (vec![], vec![]);
        loop {
            while matches!(self.peek_at(0), Tok::Nl | Tok::Op(":")) {
                self.bump();
            }
            if self.kw("case") {
                self.bump();
                let vals = self.arg_list()?;
                cases.push((vals, self.block()?));
            } else if self.kw("default") {
                self.bump();
                default = self.block()?;
            } else if self.end_kw("select") || matches!(self.peek_at(0), Tok::Eof) {
                self.eat_end("select", start, "Select")?;
                return Ok(Kind::Select(subject, cases, default));
            } else {
                return err_at(self.tok(), format!("expected `Case`, `Default` or `End Select`, found {}", describe(self.peek_at(0))));
            }
        }
    }

    fn function(&mut self, start: &Token) -> R<Kind> {
        if self.in_func || self.depth > 1 {
            return err_at(start, "functions must be defined at the top level, not inside another block");
        }
        let t = self.tok().clone();
        let (name, orig) = self.var_name()?;
        if self.funcs.iter().any(|f| f.name == name) {
            return err_at(&t, format!("function `{orig}` is defined twice"));
        }
        let mut params = vec![];
        if self.eat_op("(") && !self.eat_op(")") {
            loop {
                params.push(self.var_name()?.0);
                if !self.eat_op(",") {
                    break;
                }
            }
            self.expect_op(")")?;
        }
        self.in_func = true;
        let body = self.block()?;
        self.in_func = false;
        self.eat_end("function", start, "Function")?;
        self.funcs.push(Func { name, params, body });
        Ok(Kind::Nop)
    }

    fn type_def(&mut self, start: &Token) -> R<Kind> {
        if self.in_func || self.depth > 1 {
            return err_at(start, "types must be defined at the top level");
        }
        let (name, _) = self.var_name_any()?;
        let mut fields = vec![];
        loop {
            while matches!(self.peek_at(0), Tok::Nl | Tok::Op(":")) {
                self.bump();
            }
            if self.kw("field") {
                self.bump();
                loop {
                    fields.push(self.var_name_any()?.0);
                    if !self.eat_op(",") {
                        break;
                    }
                }
            } else if self.end_kw("type") || matches!(self.peek_at(0), Tok::Eof) {
                self.eat_end("type", start, "Type")?;
                self.types.push(TypeDef { name, fields });
                return Ok(Kind::Nop);
            } else {
                return err_at(self.tok(), format!("expected `Field` or `End Type`, found {}", describe(self.peek_at(0))));
            }
        }
    }

    fn global(&mut self, start: &Token, is_const: bool) -> R<Kind> {
        let word = if is_const { "Const" } else { "Global" };
        if self.in_func {
            return err_at(start, format!("`{word}` must be used at the top level, not inside a function"));
        }
        let mut out = vec![];
        loop {
            let (name, _) = self.var_name()?;
            self.globals.insert(name.clone());
            if is_const {
                self.consts.insert(name.clone());
            }
            if self.eat_op("=") {
                out.push(Stmt { line: start.line, kind: Kind::Assign(Expr::Var(name), "=", self.expr()?) });
            } else if is_const {
                return err_at(self.tok(), format!("`Const {name}` needs a value: `Const {name} = ...`"));
            }
            if !self.eat_op(",") {
                return Ok(Kind::Seq(out));
            }
        }
    }

    /// A name usable for a variable, parameter or function. Returns (lowercase, as written).
    fn var_name(&mut self) -> R<(String, String)> {
        let t = self.tok().clone();
        let (low, orig) = self.var_name_any()?;
        if builtins::find(&orig).is_some() || builtins::constant(&orig).is_some() {
            return err_at(&t, format!("`{orig}` is a built-in name and can't be used as a variable"));
        }
        Ok((low, orig))
    }

    /// Any identifier that isn't a keyword (also used for type and field names).
    fn var_name_any(&mut self) -> R<(String, String)> {
        let t = self.bump();
        match &t.tok {
            Tok::Ident(s) if !is_keyword(s) => Ok((s.to_ascii_lowercase(), s.clone())),
            other => err_at(&t, format!("expected a name, found {}", describe(other))),
        }
    }

    fn type_name(&mut self) -> R<String> {
        let t = self.tok().clone();
        let (name, orig) = self.var_name_any()?;
        if !self.type_names.contains(&name) {
            return err_at(&t, format!("`{orig}` is not a Type"));
        }
        Ok(name)
    }

    fn ident_stmt(&mut self, name: &str, start: &Token) -> R<Kind> {
        let low = name.to_ascii_lowercase();
        if ["=", "+=", "-=", ".", "["].iter().any(|o| self.op(o)) {
            if builtins::find(name).is_some() || builtins::constant(name).is_some() || is_keyword(name) || self.fn_arity.contains_key(&low) {
                return err_at(start, format!("`{name}` is a built-in or function name and can't be used as a variable"));
            }
            let mut target = Expr::Var(low.clone());
            loop {
                if self.eat_op(".") {
                    target = Expr::Field(Box::new(target), self.var_name_any()?.0);
                } else if self.eat_op("[") {
                    for i in self.arg_list()? {
                        target = Expr::Index(Box::new(target), Box::new(i));
                    }
                    self.expect_op("]")?;
                } else {
                    break;
                }
            }
            if matches!(target, Expr::Var(_)) && self.consts.contains(&low) {
                return err_at(start, format!("`{name}` is a Const and can't be changed"));
            }
            let op = match self.peek_at(0) {
                Tok::Op(o @ ("=" | "+=" | "-=")) => *o,
                t => return err_at(self.tok(), format!("expected `=`, found {}", describe(t))),
            };
            self.bump();
            return Ok(Kind::Assign(target, op, self.expr()?));
        }
        if is_keyword(name) {
            return err_at(start, format!("unexpected `{name}`"));
        }
        if let Some(&n) = self.fn_arity.get(&low) {
            let args = self.stmt_args()?;
            if args.len() != n {
                return err_at(start, format!("`{name}` expects {n} argument{}, got {}", plural(n), args.len()));
            }
            return Ok(Kind::UserCall(low, args));
        }
        let Some(b) = builtins::find(name) else {
            let hint = builtins::suggest(name).map(|s| format!(". Did you mean `{s}`?")).unwrap_or_default();
            return err_at(start, format!("`{name}` is not a command or function{hint}"));
        };
        let args = self.stmt_args()?;
        check_arity(b, args.len(), start)?;
        Ok(Kind::Cmd(b, args))
    }

    /// Arguments of a statement call: `Name a, b` or `Name(a, b)`.
    fn stmt_args(&mut self) -> R<Vec<Expr>> {
        if self.at_stmt_end() {
            return Ok(vec![]);
        }
        if self.op("(") {
            // Treat a leading `(` as the call's own parentheses.
            self.bump();
            let args = if self.eat_op(")") { vec![] } else { self.arg_list()? };
            if !args.is_empty() {
                self.expect_op(")")?;
            }
            if !self.at_stmt_end() {
                return err_at(self.tok(), format!("unexpected {} after the argument list", describe(self.peek_at(0))));
            }
            return Ok(args);
        }
        self.arg_list()
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
        loop {
            let op = match self.peek_at(0) {
                Tok::Op(o @ ("*" | "/")) => *o,
                _ if self.kw("mod") => "%",
                _ => return Ok(l),
            };
            self.bump();
            l = Expr::Bin(op, Box::new(l), Box::new(self.unary()?));
        }
    }

    fn unary(&mut self) -> R<Expr> {
        if self.eat_op("-") {
            return Ok(Expr::Neg(Box::new(self.unary()?)));
        }
        let base = self.postfix()?;
        if self.eat_op("^") {
            return Ok(Expr::Bin("**", Box::new(base), Box::new(self.unary()?))); // right-associative
        }
        Ok(base)
    }

    fn postfix(&mut self) -> R<Expr> {
        let mut e = self.primary()?;
        loop {
            if self.eat_op(".") {
                e = Expr::Field(Box::new(e), self.var_name_any()?.0);
            } else if self.eat_op("[") {
                for i in self.arg_list()? {
                    e = Expr::Index(Box::new(e), Box::new(i));
                }
                self.expect_op("]")?;
            } else {
                return Ok(e);
            }
        }
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
        match low.as_str() {
            "delta" => return Ok(Expr::Delta),
            "new" => return Ok(Expr::New(self.type_name()?)),
            _ if is_keyword(name) => return err_at(t, format!("expected an expression, found `{name}`")),
            _ => {}
        }
        if let Some(c) = builtins::constant(name) {
            return Ok(Expr::Const(c));
        }
        if let Some(&n) = self.fn_arity.get(&low) {
            let args = self.call_args(name, t)?;
            if args.len() != n {
                return err_at(t, format!("`{name}` expects {n} argument{}, got {}", plural(n), args.len()));
            }
            return Ok(Expr::UserCall(low, args));
        }
        let Some(b) = builtins::find(name) else {
            return Ok(Expr::Var(low));
        };
        if !b.returns {
            return err_at(t, format!("`{}` doesn't return a value, so it can't be used in an expression", b.name));
        }
        let args = self.call_args(b.name, t)?;
        check_arity(b, args.len(), t)?;
        Ok(Expr::Call(b, args))
    }

    fn call_args(&mut self, name: &str, t: &Token) -> R<Vec<Expr>> {
        if !self.eat_op("(") {
            return err_at(t, format!("`{name}` needs parentheses here: `{name}(...)`"));
        }
        if self.eat_op(")") {
            return Ok(vec![]);
        }
        let a = self.arg_list()?;
        self.expect_op(")")?;
        Ok(a)
    }
}

fn plural(n: usize) -> &'static str {
    if n == 1 { "" } else { "s" }
}

fn check_arity(b: &Builtin, n: usize, at: &Token) -> R<()> {
    if (b.min..=b.max).contains(&n) {
        return Ok(());
    }
    let want = if b.min == b.max { b.min.to_string() } else { format!("{} to {}", b.min, b.max) };
    err_at(at, format!("`{}` expects {want} argument{}, got {n}", b.name, plural(b.max)))
}

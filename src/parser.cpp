#include "bas/parser.hpp"
#include "bas/value.hpp"
#include "bas/ast.hpp"
#include <stdexcept>

using namespace bas;

// Equality operators only; relational operators are handled in comparison()
static bool is_cmp(Tok k){ return k==Tok::Eq||k==Tok::Neq; }

Program Parser::parse(){
  Program p; i=0; 
  while(peek().kind!=Tok::Eof){
    skipNewlines();
    if(peek().kind==Tok::Eof) break;
    // Emit any pending desugared statements first
    if(!pending.empty()){
      auto s = std::move(pending.front());
      pending.erase(pending.begin());
      p.stmts.push_back(std::move(s));
      continue;
    }
    if(auto s = statement()) {
      p.stmts.push_back(std::move(s));
    } else {
      // Error recovery: consume until end of line to prevent infinite loop
      while(peek().kind != Tok::Eof && !check(Tok::Newline)) advance();
      // consume the newline if present
      if(check(Tok::Newline)) advance();
    }
    while(check(Tok::Newline)) advance();
  }
  return p;
}

std::unique_ptr<Stmt> Parser::statement() {
    skipNewlines();
    const Token& t = peek();

    switch (t.kind) {
        case Tok::Print:    return parse_print();
        case Tok::Let:      return parse_let();
        case Tok::Var:      return parse_let();
        case Tok::Input:    return parse_input();
        case Tok::If:       return parse_if();
        case Tok::While:    return parse_while();
        case Tok::For:      return parse_for();
        case Tok::Sub:      return parse_sub_decl();
        case Tok::Function: return parse_function_decl();
        case Tok::Return:   return parse_return();
        case Tok::Dim:      return parse_dim();
        case Tok::Redim:    return parse_dim();
        case Tok::Ident:    return parse_ident_statement();
        case Tok::Call:     return parse_call();

        case Tok::Option: {
            advance();
            if (!match(Tok::Explicit)) {
                diag.err(peek().line, peek().col, "OPTION directive: expected EXPLICIT", "Use: OPTION EXPLICIT", "OPTION EXPLICIT");
                return nullptr;
            }
            auto s = std::make_unique<OptionExplicit>();
            s->enabled = true;
            return s;
        }
        case Tok::Break: {
            advance();
            return std::make_unique<Break>();
        }
        default: break;
    }

    diag.err(t.line, t.col, "unexpected statement", "check syntax", "PRINT x");
    advance();
    return nullptr;
}

std::vector<std::unique_ptr<Stmt>> Parser::stmt_list_until(Tok end1, Tok end2){
  std::vector<std::unique_ptr<Stmt>> v; 
  while(peek().kind!=Tok::Eof && !check(end1) && !check(end2)){
    if(auto s=statement()) {
      v.push_back(std::move(s));
    } else {
      // Error recovery within block: consume until newline or end token
      while(peek().kind != Tok::Eof && !check(Tok::Newline) && !check(end1) && !check(end2)) advance();
      if(check(Tok::Newline)) advance();
    }
    if(check(Tok::Newline)) while(check(Tok::Newline)) advance();
  }
  return v;
}

std::unique_ptr<Expr> Parser::expression(){ return or_(); }
std::unique_ptr<Expr> Parser::or_(){
  auto e = and_();
  while(check(Tok::Or)){ Tok op=advance().kind; auto r=and_(); e = std::make_unique<Binary>(std::move(e), op, std::move(r)); }
  return e;
}
std::unique_ptr<Expr> Parser::and_(){
  auto e = equality();
  while(check(Tok::And)){ Tok op=advance().kind; auto r=equality(); e = std::make_unique<Binary>(std::move(e), op, std::move(r)); }
  return e;
}
std::unique_ptr<Expr> Parser::equality(){
  auto e = comparison();
  while(is_cmp(peek().kind)){ Tok op=advance().kind; auto r=comparison(); e = std::make_unique<Binary>(std::move(e), op, std::move(r)); }
  return e;
}
std::unique_ptr<Expr> Parser::comparison(){
  auto e = term();
  while(check(Tok::Lt) || check(Tok::Lte) || check(Tok::Gt) || check(Tok::Gte)){
    Tok op=advance().kind; auto r=term(); e = std::make_unique<Binary>(std::move(e), op, std::move(r));
  }
  return e;
}
std::unique_ptr<Expr> Parser::term(){
  auto e = factor();
  while(check(Tok::Plus) || check(Tok::Minus)){
    Tok op=advance().kind; auto r=factor(); e = std::make_unique<Binary>(std::move(e), op, std::move(r));
  }
  return e;
}
std::unique_ptr<Expr> Parser::factor(){
  auto e = unary();
  while(check(Tok::Star) || check(Tok::Slash) || check(Tok::Mod)){
    Tok op=advance().kind; auto r=unary(); e = std::make_unique<Binary>(std::move(e), op, std::move(r));
  }
  return e;
}
std::unique_ptr<Expr> Parser::unary(){
  if(check(Tok::Plus) || check(Tok::Minus) || check(Tok::Not)){
    Tok op=advance().kind; auto r=unary(); return std::make_unique<Unary>(op, std::move(r));
  }
  return primary();
}
std::unique_ptr<Expr> Parser::primary(){
  const Token& t = peek();
  if(t.kind==Tok::Number || t.kind==Tok::String || t.kind==Tok::True || t.kind==Tok::False){ return std::make_unique<Literal>(advance()); }
  if(t.kind==Tok::LBracket){
    // Array literal: [ e1, e2, ... ]
    advance();
    auto arr = std::make_unique<ArrayLiteral>();
    if(!check(Tok::RBracket)){
      do { arr->elements.push_back(expression()); } while(check(Tok::Comma) && (advance(), true));
    }
    if(!check(Tok::RBracket)) diag.err_at(peek().line, peek().col, "array: expected ']' "); else advance();
    std::unique_ptr<Expr> base = std::move(arr);
    // Postfix indexing chain
    while(check(Tok::LBracket)){
      advance(); auto idx = expression(); if(!check(Tok::RBracket)) { diag.err_at(peek().line, peek().col, "index: expected ']'"); break; } advance();
      base = std::make_unique<Index>(std::move(base), std::move(idx));
    }
    return base;
  }
  if(t.kind==Tok::Ident){
    Token id = advance();
    std::unique_ptr<Expr> base;
    if(check(Tok::LParen)){
      advance(); std::vector<std::unique_ptr<Expr>> args; if(!check(Tok::RParen)){
        do { args.push_back(expression()); } while(check(Tok::Comma) && (advance(), true));
      }
      if(!check(Tok::RParen)){ diag.err_at(peek().line, peek().col, "call: expected ')'"); return nullptr; }
      advance(); base = std::make_unique<Call>(id.lex, std::move(args));
    } else {
      base = std::make_unique<Variable>(id.lex);
    }
    while(check(Tok::LBracket)){
      advance(); auto idx = expression(); if(!check(Tok::RBracket)) { diag.err("index: expected ']'"); break; } advance();
      base = std::make_unique<Index>(std::move(base), std::move(idx));
    }
    return base;
  }
  if(t.kind==Tok::LParen){ advance(); auto e=expression(); if(!check(Tok::RParen)) diag.err_at(peek().line, peek().col, "expected ')'"); else advance(); return e; }
  diag.err_at(t.line, t.col, "expected expression"); return std::make_unique<Literal>(Token{Tok::Number,"0",t.line,t.col});
}

std::unique_ptr<Stmt> Parser::parse_print() {
    advance(); // consume PRINT
    auto e = expression();
    auto s = std::make_unique<Print>();
    s->value = std::move(e);
    return s;
}

std::unique_ptr<Stmt> Parser::parse_let() {
    advance(); // consume LET or VAR
    if (!check(Tok::Ident)) {
        diag.err(peek().line, peek().col, "Assignment: expected variable name", "Make sure you have a valid variable name", "LET x = 10");
        return nullptr;
    }
    std::string name = advance().lex;
    if (check(Tok::LBracket)) { // Indexed assignment
        std::vector<std::unique_ptr<Expr>> indices;
        do {
            advance();
            indices.push_back(expression());
            if (!match(Tok::RBracket)) {
                diag.err_at(peek().line, peek().col, "Indexed assignment: expected ']'");
                return nullptr;
            }
        } while (check(Tok::LBracket));
        if (!match(Tok::Eq)) {
            diag.err(peek().line, peek().col, "Indexed assignment: expected '=' after indexes", "Add '=' to assign a value", "LET arr[i] = value");
            return nullptr;
        }
        auto val = expression();
        auto s = std::make_unique<AssignIndex>();
        s->name = std::move(name);
        s->indices = std::move(indices);
        s->value = std::move(val);
        return s;
    } else { // Simple assignment
        if (!match(Tok::Eq)) {
            diag.err(peek().line, peek().col, "Assignment: expected '=' after variable name", "Add an equals sign to assign a value", "LET x = value");
            return nullptr;
        }
        auto v = expression();
        auto s = std::make_unique<Let>();
        s->name = std::move(name);
        s->value = std::move(v);
        return s;
    }
}

std::unique_ptr<Stmt> Parser::parse_input() {
    advance(); // consume INPUT
    std::vector<std::unique_ptr<Expr>> args;
    if (check(Tok::String)) {
        auto prompt = expression();
        if (!match(Tok::Comma)) {
            diag.err(peek().line, peek().col, "INPUT: expected ',' after prompt", "Use: INPUT \"Prompt\", x", "INPUT \"Name? \", x");
            return nullptr;
        }
        args.push_back(std::move(prompt));
    }
    if (!check(Tok::Ident)) {
        diag.err(peek().line, peek().col, "INPUT: expected variable name", "Provide a variable to store the input", "INPUT x");
        return nullptr;
    }
    std::string var = advance().lex;
    auto call = std::make_unique<Call>(std::string("INPUT"), std::move(args));
    auto asn = std::make_unique<Assign>();
    asn->name = std::move(var);
    asn->value = std::move(call);
    return asn;
}

std::unique_ptr<Stmt> Parser::parse_if() {
    advance(); // consume IF
    auto firstCond = expression();
    bool hasThen = match(Tok::Then);

    if (hasThen && !check(Tok::Newline) && !check(Tok::Eof)) { // Single-line IF THEN
        auto node = std::make_unique<IfChain>();
        IfChain::Branch br;
        br.cond = std::move(firstCond);
        if (auto s = statement()) br.body.push_back(std::move(s));
        node->branches.push_back(std::move(br));
        return node;
    }

    // Multi-line IF
    auto node = std::make_unique<IfChain>();
    {
        IfChain::Branch br;
        br.cond = std::move(firstCond);
        br.body = stmt_list_until(Tok::ElseIf, Tok::Else);
        node->branches.push_back(std::move(br));
    }

    while (match(Tok::ElseIf)) {
        auto c = expression();
        if (!match(Tok::Then)) {
            diag.err(peek().line, peek().col, "ELSEIF: expected THEN after condition");
            return nullptr;
        }
        IfChain::Branch br;
        br.cond = std::move(c);
        br.body = stmt_list_until(Tok::ElseIf, Tok::Else);
        node->branches.push_back(std::move(br));
    }

    if (match(Tok::Else)) {
        node->hasElse = true;
        node->elseBody = stmt_list_until(Tok::EndIf, Tok::EndIf);
    }

    if (!match(Tok::EndIf)) {
        diag.err(peek().line, peek().col, "IF: expected ENDIF to close the block");
        return nullptr;
    }
    return node;
}

std::unique_ptr<Stmt> Parser::parse_while() {
    advance(); // consume WHILE
    auto cond = expression();
    auto body = stmt_list_until(Tok::Wend, Tok::Wend);
    if (!match(Tok::Wend)) {
        diag.err(peek().line, peek().col, "WHILE: expected WEND to close the loop");
        return nullptr;
    }
    advance();
    auto s = std::make_unique<WhileWend>();
    s->cond = std::move(cond);
    s->body = std::move(body);
    return s;
}

std::unique_ptr<Stmt> Parser::parse_for() {
    advance(); // consume FOR
    if (!check(Tok::Ident)) {
        diag.err(peek().line, peek().col, "FOR: expected loop variable name");
        return nullptr;
    }
    std::string var = advance().lex;

    if (match(Tok::In)) { // FOR..IN (desugared)
        auto list_expr = expression();
        auto list_var = "__list" + std::to_string(gensym);
        auto idx_var = "__i" + std::to_string(gensym);
        gensym++;

        auto list_assign = std::make_unique<Let>();
        list_assign->name = list_var;
        list_assign->value = std::move(list_expr);
        pending.push_back(std::move(list_assign));

        auto idx_assign = std::make_unique<Let>();
        idx_assign->name = idx_var;
        Token zero_token{Tok::Number, "0", peek().line, peek().col};
        idx_assign->value = std::make_unique<Literal>(zero_token);
        pending.push_back(std::move(idx_assign));

        auto cond = std::make_unique<Binary>(
            std::make_unique<Variable>(idx_var),
            Tok::Lt,
            [&]() {
                std::vector<std::unique_ptr<Expr>> args;
                args.push_back(std::make_unique<Variable>(list_var));
                return std::make_unique<Call>("LEN", std::move(args));
            }()
        );

        auto loop = std::make_unique<WhileWend>();
        loop->cond = std::move(cond);

        auto user_var_assign = std::make_unique<Assign>();
        user_var_assign->name = var;
        user_var_assign->value = std::make_unique<Index>(
            std::make_unique<Variable>(list_var),
            std::make_unique<Variable>(idx_var)
        );
        loop->body.push_back(std::move(user_var_assign));

        auto user_body = stmt_list_until(Tok::Next, Tok::Next);
        for(auto& s : user_body) loop->body.push_back(std::move(s));

        if(!match(Tok::Next)){
            diag.err(peek().line, peek().col, "FOR..IN: expected NEXT to close loop");
            return nullptr;
        }
        if(check(Tok::Ident)) advance();

        auto incr = std::make_unique<Assign>();
        incr->name = idx_var;
        incr->value = std::make_unique<Binary>(
            std::make_unique<Variable>(idx_var),
            Tok::Plus,
            std::make_unique<Literal>(Token{Tok::Number, "1", peek().line, peek().col})
        );
        loop->body.push_back(std::move(incr));
        return loop;
    }

    if (!match(Tok::Eq)) {
        diag.err(peek().line, peek().col, "FOR: expected '=' after variable");
        return nullptr;
    }
    auto init = expression();
    if (!match(Tok::To)) {
        diag.err(peek().line, peek().col, "FOR: expected TO after initializer");
        return nullptr;
    }
    auto limit = expression();
    std::unique_ptr<Expr> step;
    if (match(Tok::Step)) {
        step = expression();
    }
    auto body = stmt_list_until(Tok::Next, Tok::Next);
    if (!match(Tok::Next)) {
        diag.err(peek().line, peek().col, "FOR: expected NEXT to close loop");
        return nullptr;
    }
    if (check(Tok::Ident)) advance(); // optional var after NEXT

    auto s = std::make_unique<ForNext>();
    s->var = std::move(var);
    s->init = std::move(init);
    s->limit = std::move(limit);
    s->step = std::move(step);
    s->body = std::move(body);
    return s;
}

std::unique_ptr<Stmt> Parser::parse_sub_decl() {
    advance(); // consume SUB
    if (!check(Tok::Ident)) {
        diag.err(peek().line, peek().col, "SUB: expected subroutine name");
        return nullptr;
    }
    std::string name = advance().lex;
    std::vector<std::string> params;
    if (match(Tok::LParen)) {
        if (!check(Tok::RParen)) {
            do {
                if (!check(Tok::Ident)) {
                    diag.err(peek().line, peek().col, "SUB: expected parameter name");
                    return nullptr;
                }
                params.push_back(advance().lex);
            } while (match(Tok::Comma));
        }
        if (!match(Tok::RParen)) {
            diag.err(peek().line, peek().col, "SUB: expected ')' to close parameters");
            return nullptr;
        }
    }
    auto body = stmt_list_until(Tok::EndSub, Tok::EndSub);
    if (!match(Tok::EndSub)) {
        diag.err(peek().line, peek().col, "SUB: expected ENDSUB");
        return nullptr;
    }
    advance();
    auto s = std::make_unique<SubDecl>();
    s->name = std::move(name);
    s->params = std::move(params);
    s->body = std::move(body);
    return s;
}

std::unique_ptr<Stmt> Parser::parse_function_decl() {
    advance(); // consume FUNCTION
    if (!check(Tok::Ident)) {
        diag.err(peek().line, peek().col, "FUNCTION: expected function name");
        return nullptr;
    }
    std::string name = advance().lex;
    std::vector<std::string> params;
    if (match(Tok::LParen)) {
        if (!check(Tok::RParen)) {
            do {
                if (!check(Tok::Ident)) {
                    diag.err(peek().line, peek().col, "FUNCTION: expected parameter name");
                    return nullptr;
                }
                params.push_back(advance().lex);
            } while (match(Tok::Comma));
        }
        if (!match(Tok::RParen)) {
            diag.err(peek().line, peek().col, "FUNCTION: expected ')' to close parameters");
            return nullptr;
        }
    }
    auto body = stmt_list_until(Tok::EndFunction, Tok::EndFunction);
    if (!match(Tok::EndFunction)) {
        diag.err(peek().line, peek().col, "FUNCTION: expected ENDFUNCTION");
        return nullptr;
    }
    advance();
    auto f = std::make_unique<FunctionDecl>();
    f->name = std::move(name);
    f->params = std::move(params);
    f->body = std::move(body);
    return f;
}

std::unique_ptr<Stmt> Parser::parse_return() {
    advance(); // consume RETURN
    std::unique_ptr<Expr> val;
    Tok k = peek().kind;
    if (k == Tok::Number || k == Tok::String || k == Tok::True || k == Tok::False || k == Tok::Ident || k == Tok::LParen || k == Tok::Minus || k == Tok::Plus || k == Tok::Not || k == Tok::LBracket) {
        val = expression();
    }
    auto r = std::make_unique<Return>();
    r->value = std::move(val);
    return r;
}

std::unique_ptr<Stmt> Parser::parse_dim() {
    bool is_redim = match(Tok::Redim);
    if (!is_redim) advance(); // consume DIM

    bool preserve = match(Tok::Preserve);
    if (!check(Tok::Ident)) {
        diag.err(peek().line, peek().col, "DIM/REDIM: expected array name");
        return nullptr;
    }
    std::string name = advance().lex;
    if (!match(Tok::LParen)) {
        diag.err(peek().line, peek().col, "DIM/REDIM: expected '(' after name");
        return nullptr;
    }
    std::vector<std::unique_ptr<Expr>> sizes;
    if (!check(Tok::RParen)) {
        do {
            sizes.push_back(expression());
        } while (match(Tok::Comma));
    }
    if (!match(Tok::RParen)) {
        diag.err(peek().line, peek().col, "DIM/REDIM: expected ')' after sizes");
        return nullptr;
    }
    advance();

    if (is_redim) {
        auto d = std::make_unique<Redim>();
        d->name = std::move(name);
        d->sizes = std::move(sizes);
        d->preserve = preserve;
        return d;
    } else {
        auto d = std::make_unique<Dim>();
        d->name = std::move(name);
        d->sizes = std::move(sizes);
        return d;
    }
}

std::unique_ptr<Stmt> Parser::parse_ident_statement() {
    Token id = advance(); // consume identifier

    if (check(Tok::LBracket)) { // Indexed assignment
        std::vector<std::unique_ptr<Expr>> indices;
        do {
            advance();
            indices.push_back(expression());
            if (!match(Tok::RBracket)) {
                diag.err_at(peek().line, peek().col, "INDEX: expected ']'");
                return nullptr;
            }
        } while (check(Tok::LBracket));
        if (!match(Tok::Eq)) {
            diag.err_at(peek().line, peek().col, "INDEX: expected '=' after indices");
            return nullptr;
        }
        auto val = expression();
        auto s = std::make_unique<AssignIndex>();
        s->name = id.lex;
        s->indices = std::move(indices);
        s->value = std::move(val);
        return s;
    } else if (match(Tok::Eq)) { // Simple assignment
        auto v = expression();
        auto s = std::make_unique<Assign>();
        s->name = id.lex;
        s->value = std::move(v);
        return s;
    } else if (check(Tok::LParen)) { // Procedure call
        advance();
        std::vector<std::unique_ptr<Expr>> args;
        if (!check(Tok::RParen)) {
            do {
                args.push_back(expression());
            } while (match(Tok::Comma));
        }
        if (!match(Tok::RParen)) {
            diag.err_at(peek().line, peek().col, "CALL: expected ')'");
            return nullptr;
        }
        auto s = std::make_unique<CallStmt>();
        s->name = id.lex;
        s->args = std::move(args);
        return s;
    } else { // Standalone identifier is a procedure call with no args
        auto s = std::make_unique<CallStmt>();
        s->name = id.lex;
        return s;
    }
}

std::unique_ptr<Stmt> Parser::parse_call() {
    advance(); // consume CALL
    if (!check(Tok::Ident)) {
        diag.err_at(peek().line, peek().col, "CALL: expected function name");
        return nullptr;
    }
    Token id = advance();
    std::vector<std::unique_ptr<Expr>> args;
    if (match(Tok::LParen)) {
        if (!check(Tok::RParen)) {
            do {
                args.push_back(expression());
            } while (match(Tok::Comma));
        }
        if (!match(Tok::RParen)) {
            diag.err("CALL: expected ')'");
            return nullptr;
        }
    }
    auto s = std::make_unique<CallStmt>();
    s->name = id.lex;
    s->args = std::move(args);
    return s;
}

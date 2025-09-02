#include "bas/parser.hpp"
#include <stdexcept>

using namespace bas;

// Equality operators only; relational operators are handled in comparison()
static bool is_cmp(Tok k){ return k==Tok::Eq||k==Tok::Neq; }

Program Parser::parse(){
  Program p; i=0; 
  while(peek().kind!=Tok::Eof){
    skipNewlines();
    if(peek().kind==Tok::Eof) break;
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

std::unique_ptr<Stmt> Parser::statement(){
  skipNewlines();
  const Token& t = peek();
  if(t.kind==Tok::Let){
    advance();
    if(!check(Tok::Ident)){
      diag.err(peek().line, peek().col, 
               "LET statement: expected variable name", 
               "Make sure you have a valid variable name after LET",
               "LET x = 10");
      return nullptr; 
    }
    std::string name = advance().lex;
    // Optional indexed target: LET name[expr][expr]... = expr
    if(check(Tok::LBracket)){
      std::vector<std::unique_ptr<Expr>> indices;
      do {
        advance();
        indices.push_back(expression());
        if(!check(Tok::RBracket)){
          diag.err_at(peek().line, peek().col, "LET index: expected ']' ");
          return nullptr;
        }
        advance();
      } while(check(Tok::LBracket));
      if(!check(Tok::Eq)){
        diag.err(peek().line, peek().col,
                 "LET indexed assignment: expected '=' after indexes",
                 "Add '=' to assign a value",
                 "LET " + name + "[i] = value");
        return nullptr;
      }
      advance();
      auto val = expression();
      auto s = std::make_unique<AssignIndex>();
      s->name = std::move(name);
      s->indices = std::move(indices);
      s->value = std::move(val);
      return s;
    } else {
      if(!check(Tok::Eq)){
        diag.err(peek().line, peek().col, 
                 "LET statement: expected '=' after variable name", 
                 "Add an equals sign to assign a value",
                 "LET " + name + " = value");
        return nullptr; 
      }
      advance();
      auto v = expression();
      auto s = std::make_unique<Let>(); s->name=std::move(name); s->value=std::move(v); return s;
    }
  }
  if(t.kind==Tok::Print){
    advance(); auto e = expression(); auto s = std::make_unique<Print>(); s->value=std::move(e); return s;
  }
  if(t.kind==Tok::Input){
    // INPUT [prompt ,] var  -> sugar for var = INPUT([prompt])
    advance();
    std::vector<std::unique_ptr<Expr>> args;
    // Only treat a String followed by comma as prompt to avoid rollback complexities
    if(check(Tok::String)){
      auto prompt = expression();
      if(!check(Tok::Comma)){
        diag.err(peek().line, peek().col,
                 "INPUT: expected ',' after prompt",
                 "Use: INPUT \"Prompt\", x",
                 "INPUT \"Name? \", x");
        return nullptr;
      }
      advance(); // consume comma
      args.push_back(std::move(prompt));
    }
    if(!check(Tok::Ident)){
      diag.err(peek().line, peek().col,
               "INPUT: expected variable name",
               "Provide a variable to store the input",
               "INPUT x or INPUT \"Prompt\", x");
      return nullptr;
    }
    std::string var = advance().lex;
    auto call = std::make_unique<Call>(std::string("INPUT"), std::move(args));
    auto asn = std::make_unique<Assign>(); asn->name = std::move(var); asn->value = std::move(call); return asn;
  }
  if(t.kind==Tok::If){
    advance();
    auto firstCond = expression();
    bool hasThen = false;
    if(check(Tok::Then)) { advance(); hasThen = true; }

    // Single-line IF with THEN: IF <cond> THEN <stmt>
    // Allowed in permissive mode; in classic, only valid if THEN is followed by newline (i.e., start of multi-line block)
    if(hasThen && !check(Tok::Newline) && !check(Tok::Eof)){
      if(!agk){
        diag.err(peek().line, peek().col,
                 "IF single-line not allowed in classic dialect",
                 "Use multi-line IF...THEN...ENDIF or run without --dialect classic",
                 "IF x > 5 THEN\n  PRINT x\nENDIF");
        return nullptr;
      }
      auto node = std::make_unique<IfChain>();
      IfChain::Branch br; br.cond = std::move(firstCond);
      if(auto s = statement()) br.body.push_back(std::move(s));
      node->branches.push_back(std::move(br));
      return node;
    }
    // Single-line IF without THEN: IF <cond> <stmt>
    if(!hasThen){
      if(check(Tok::Newline) || check(Tok::Eof)){
        diag.err(peek().line, peek().col,
                 "IF statement: expected THEN after condition",
                 "Add THEN keyword after the condition",
                 "IF x > 5 THEN");
        return nullptr;
      }
      if(!agk){
        diag.err(peek().line, peek().col,
                 "THEN-less single-line IF is not allowed in classic dialect",
                 "Either add THEN and use multi-line form, or run without --dialect classic",
                 "IF x > 5 THEN\n  PRINT x\nENDIF");
        return nullptr;
      }
      auto node = std::make_unique<IfChain>();
      IfChain::Branch br; br.cond = std::move(firstCond);
      if(auto s = statement()) br.body.push_back(std::move(s));
      node->branches.push_back(std::move(br));
      return node;
    }

    // Multi-line IF ... THEN ... [ELSEIF ... THEN ...]* [ELSE ...] ENDIF
    auto node = std::make_unique<IfChain>();
    {
      IfChain::Branch br;
      // Parse body until ELSEIF/ELSE/ENDIF
      while(peek().kind!=Tok::Eof && !check(Tok::ElseIf) && !check(Tok::Else) && !check(Tok::EndIf)){
        if(auto s=statement()) br.body.push_back(std::move(s));
        else {
          while(peek().kind != Tok::Eof && !check(Tok::Newline) && !check(Tok::ElseIf) && !check(Tok::Else) && !check(Tok::EndIf)) advance();
          if(check(Tok::Newline)) advance();
        }
        while(check(Tok::Newline)) advance();
      }
      br.cond = std::move(firstCond);
      node->branches.push_back(std::move(br));
    }
    // Zero or more ELSEIF branches
    while(check(Tok::ElseIf)){
      advance();
      auto c = expression();
      if(!check(Tok::Then)){
        diag.err(peek().line, peek().col,
                 "ELSEIF branch: expected THEN after condition",
                 "Add THEN keyword after the condition",
                 "ELSEIF x > 0 THEN");
        return nullptr;
      }
      advance();
      IfChain::Branch br;
      while(peek().kind!=Tok::Eof && !check(Tok::ElseIf) && !check(Tok::Else) && !check(Tok::EndIf)){
        if(auto s=statement()) br.body.push_back(std::move(s));
        else {
          while(peek().kind != Tok::Eof && !check(Tok::Newline) && !check(Tok::ElseIf) && !check(Tok::Else) && !check(Tok::EndIf)) advance();
          if(check(Tok::Newline)) advance();
        }
        while(check(Tok::Newline)) advance();
      }
      br.cond = std::move(c);
      node->branches.push_back(std::move(br));
    }
    // Optional ELSE
    if(check(Tok::Else)){
      advance();
      node->hasElse = true;
      while(peek().kind!=Tok::Eof && !check(Tok::EndIf)){
        if(auto s=statement()) node->elseBody.push_back(std::move(s));
        else {
          while(peek().kind != Tok::Eof && !check(Tok::Newline) && !check(Tok::EndIf)) advance();
          if(check(Tok::Newline)) advance();
        }
        while(check(Tok::Newline)) advance();
      }
    }
    if(!check(Tok::EndIf)){
      diag.err(peek().line, peek().col,
               "IF statement: expected ENDIF to close the block",
               "Add ENDIF to close the IF statement",
               "IF x > 5 THEN\n  ...\nENDIF");
      return nullptr;
    }
    advance();
    return node;
  }
  if(t.kind==Tok::While){
    advance(); auto cond = expression();
    auto body = stmt_list_until(Tok::Wend, Tok::Wend);
    if(!check(Tok::Wend)){
      diag.err(peek().line, peek().col, 
               "WHILE loop: expected WEND to close the loop", 
               "Add WEND to close the WHILE loop",
               "WHILE x < 10\n  PRINT x\n  LET x = x + 1\nWEND");
      return nullptr; 
    }
    advance();
    auto s = std::make_unique<WhileWend>(); s->cond=std::move(cond); s->body=std::move(body); return s;
  }
  if(t.kind==Tok::Do){
    // DO ... LOOP (permissive only)
    if(!agk){
      diag.err(peek().line, peek().col,
               "DO/LOOP is not allowed in classic dialect",
               "Use WHILE/WEND or FOR/NEXT, or run without --dialect classic",
               "WHILE cond\n  ...\nWEND");
      return nullptr;
    }
    advance();
    auto body = stmt_list_until(Tok::Loop, Tok::Loop);
    if(!check(Tok::Loop)){
      diag.err(peek().line, peek().col,
               "DO loop: expected LOOP to close the loop",
               "Add LOOP to close the DO block",
               "DO\n  ...\nLOOP");
      return nullptr;
    }
    advance();
    auto s = std::make_unique<DoLoop>();
    s->body = std::move(body);
    return s;
  }
  if(t.kind==Tok::Repeat){
    // REPEAT ... UNTIL <cond> (permissive only)
    if(!agk){
      diag.err(peek().line, peek().col,
               "REPEAT/UNTIL is not allowed in classic dialect",
               "Use WHILE/WEND or FOR/NEXT, or run without --dialect classic",
               "WHILE cond\n  ...\nWEND");
      return nullptr;
    }
    advance();
    auto body = stmt_list_until(Tok::Until, Tok::Until);
    if(!check(Tok::Until)){
      diag.err(peek().line, peek().col,
               "REPEAT loop: expected UNTIL to close the loop",
               "Add UNTIL <condition> after the body",
               "REPEAT\n  ...\nUNTIL x > 10");
      return nullptr;
    }
    advance();
    auto cond = expression();
    auto s = std::make_unique<RepeatUntil>();
    s->body = std::move(body);
    s->cond = std::move(cond);
    return s;
  }
  if(t.kind==Tok::Sub){
    advance();
    if(!check(Tok::Ident)){
      diag.err(peek().line, peek().col, 
               "SUB declaration: expected subroutine name", 
               "Provide a name for the subroutine",
               "SUB MySub");
      return nullptr; 
    }
    std::string name = advance().lex;
    std::vector<std::string> params;
    if(check(Tok::LParen)){
      advance();
      if(!check(Tok::RParen)){
        do {
          if(!check(Tok::Ident)){
            diag.err(peek().line, peek().col, 
                     "SUB parameters: expected parameter name", 
                     "Parameter names must be valid identifiers",
                     "SUB MySub(param1, param2)");
            return nullptr; 
          }
          params.push_back(advance().lex);
        } while(check(Tok::Comma) && (advance(), true));
      }
      if(!check(Tok::RParen)){
        diag.err(peek().line, peek().col, 
                 "SUB declaration: expected ')' to close parameters", 
                 "Close the parameter list with a right parenthesis",
                 "SUB MySub(param1, param2)");
        return nullptr; 
      }
      advance();
    }
    // Body until ENDSUB
    auto body = stmt_list_until(Tok::EndSub, Tok::EndSub);
    if(!check(Tok::EndSub)){
      diag.err(peek().line, peek().col, 
               "SUB declaration: expected ENDSUB to close subroutine", 
               "Add ENDSUB to close the subroutine definition",
               "SUB MySub\n  PRINT 'Hello'\nENDSUB");
      return nullptr; 
    }
    advance();
    auto s = std::make_unique<SubDecl>();
    s->name = std::move(name);
    s->params = std::move(params);
    s->body = std::move(body);
    return s;
  }
  if(t.kind==Tok::Select){
    advance();
    if(!check(Tok::Case)){
      diag.err(peek().line, peek().col, 
               "SELECT statement: expected CASE after SELECT", 
               "Add CASE keyword after SELECT",
               "SELECT x\nCASE 1");
      return nullptr; 
    }
    advance();
    auto selector = expression();
    auto node = std::make_unique<SelectCaseStmt>();
    node->selector = std::move(selector);
    // Branches
    while(true){
      while(check(Tok::Newline)) advance();
      if(check(Tok::EndSelect)) { advance(); return node; }
      if(!check(Tok::Case)){
        diag.err(peek().line, peek().col, 
                 "SELECT statement: expected CASE or ENDSELECT", 
                 "Add CASE for each branch or ENDSELECT to close",
                 "SELECT x\nCASE 1\n  PRINT 'One'\nCASE 2\n  PRINT 'Two'\nENDSELECT");
        return nullptr; 
      }
      advance();
      CaseBranch br;
      // CASE ELSE (support both keyword token and legacy ident)
      if(check(Tok::Else)) { advance(); br.isElse = true; }
      else if(check(Tok::Ident)){
        std::string u = peek().lex; for(char& ch:u) ch = (char)std::toupper((unsigned char)ch);
        if(u=="ELSE") { advance(); br.isElse=true; }
      }
      if(!br.isElse){
        // Check relational form: CASE IS <op> expr
        if(check(Tok::Is)){
          advance();
          Tok op = peek().kind;
          if(op!=Tok::Eq && op!=Tok::Neq && op!=Tok::Lt && op!=Tok::Lte && op!=Tok::Gt && op!=Tok::Gte){
            diag.err(peek().line, peek().col,
                     "CASE IS: expected comparison operator",
                     "Use one of: =, <>, <, <=, >, >=",
                     "CASE IS >= 10");
            return nullptr;
          }
          advance();
          br.isRel = true; br.relOp = op; br.relExpr = expression();
        } else {
          // One or more expressions separated by commas
          br.values.push_back(expression());
          while(check(Tok::Comma)) { advance(); br.values.push_back(expression()); }
        }
      }
      // Body until CASE or ENDSELECT
      br.body = stmt_list_until(Tok::Case, Tok::EndSelect);
      node->branches.push_back(std::move(br));
    }
  }
  if(t.kind==Tok::Dim){
    advance();
    if(!check(Tok::Ident)){
      diag.err(peek().line, peek().col, 
               "DIM statement: expected array variable name", 
               "Provide a name for the array",
               "DIM myArray(10)");
      return nullptr; 
    }
    std::string name = advance().lex;
    if(!check(Tok::LParen)){
      diag.err(peek().line, peek().col, 
               "DIM statement: expected '(' after array name", 
               "Add opening parenthesis for array size",
               "DIM " + name + "(size)");
      return nullptr; 
    }
    advance();
    std::vector<std::unique_ptr<Expr>> sizes;
    if(!check(Tok::RParen)){
      do { sizes.push_back(expression()); } while(check(Tok::Comma) && (advance(), true));
    }
    if(!check(Tok::RParen)){
      diag.err(peek().line, peek().col, 
               "DIM statement: expected ')' after sizes", 
               "Close the array size specification",
               "DIM " + name + "(n[,m,...])");
      return nullptr; 
    }
    advance();
    auto d = std::make_unique<Dim>(); d->name = std::move(name); d->sizes = std::move(sizes); return d;
  }
  if(t.kind==Tok::Redim){
    advance();
    bool preserve = false;
    if(check(Tok::Preserve)) { advance(); preserve = true; }
    if(!check(Tok::Ident)){
      diag.err(peek().line, peek().col,
               "REDIM statement: expected array variable name",
               "Provide a name for the array",
               "REDIM myArray(10)");
      return nullptr;
    }
    std::string name = advance().lex;
    if(!check(Tok::LParen)){
      diag.err(peek().line, peek().col,
               "REDIM statement: expected '(' after array name",
               "Add opening parenthesis for array size",
               "REDIM " + name + "(size)");
      return nullptr;
    }
    advance();
    std::vector<std::unique_ptr<Expr>> sizes;
    if(!check(Tok::RParen)){
      do { sizes.push_back(expression()); } while(check(Tok::Comma) && (advance(), true));
    }
    if(!check(Tok::RParen)){
      diag.err(peek().line, peek().col,
               "REDIM statement: expected ')' after sizes",
               "Close the array size specification",
               "REDIM " + name + "(n[,m,...])");
      return nullptr;
    }
    advance();
    auto d = std::make_unique<Redim>(); d->name = std::move(name); d->sizes = std::move(sizes); d->preserve = preserve; return d;
  }
  if(t.kind==Tok::Function){
    advance();
    if(!check(Tok::Ident)){
      diag.err(peek().line, peek().col, 
               "FUNCTION declaration: expected function name", 
               "Provide a name for the function",
               "FUNCTION MyFunc");
      return nullptr; 
    }
    std::string name = advance().lex;
    std::vector<std::string> params;
    if(check(Tok::LParen)){
      advance();
      if(!check(Tok::RParen)){
        do {
          if(!check(Tok::Ident)){
            diag.err(peek().line, peek().col, 
                     "FUNCTION parameters: expected parameter name", 
                     "Parameter names must be valid identifiers",
                     "FUNCTION MyFunc(param1, param2)");
            return nullptr; 
          }
          params.push_back(advance().lex);
        } while(check(Tok::Comma) && (advance(), true));
      }
      if(!check(Tok::RParen)){
        diag.err(peek().line, peek().col, 
                 "FUNCTION declaration: expected ')' to close parameters", 
                 "Close the parameter list with a right parenthesis",
                 "FUNCTION MyFunc(param1, param2)");
        return nullptr; 
      }
      advance();
    }
    auto body = stmt_list_until(Tok::EndFunction, Tok::EndFunction);
    if(!check(Tok::EndFunction)){
      diag.err(peek().line, peek().col, 
               "FUNCTION declaration: expected ENDFUNCTION to close function", 
               "Add ENDFUNCTION to close the function definition",
               "FUNCTION MyFunc\n  PRINT 'Hello'\nENDFUNCTION");
      return nullptr; 
    }
    advance();
    auto f = std::make_unique<FunctionDecl>();
    f->name = std::move(name);
    f->params = std::move(params);
    f->body = std::move(body);
    return f;
  }
  if(t.kind==Tok::Return){
    advance();
    // Optional expression
    std::unique_ptr<Expr> val;
    // If next token can start an expression
    Tok k = peek().kind;
    if(k==Tok::Number||k==Tok::String||k==Tok::True||k==Tok::False||k==Tok::Ident||k==Tok::LParen||k==Tok::Minus||k==Tok::Plus||k==Tok::Not||k==Tok::LBracket){
      val = expression();
    }
    auto r = std::make_unique<Return>(); r->value = std::move(val); return r;
  }
  if(t.kind==Tok::For){
    advance();
    if(!check(Tok::Ident)){
      diag.err(peek().line, peek().col, 
               "FOR loop: expected loop variable name", 
               "Provide a name for the loop variable",
               "FOR x = 1 TO 10");
      return nullptr; 
    }
    std::string var = advance().lex;
    if(!check(Tok::Eq)){
      diag.err(peek().line, peek().col, 
               "FOR loop: expected '=' after variable name", 
               "Add an equals sign to initialize the loop variable",
               "FOR " + var + " = 1");
      return nullptr; 
    }
    advance();
    auto init = expression();
    if(!check(Tok::To)){
      diag.err(peek().line, peek().col, 
               "FOR loop: expected TO after initialization", 
               "Add TO keyword after the initialization",
               "FOR " + var + " = 1 TO 10");
      return nullptr; 
    }
    advance();
    auto limit = expression();
    std::unique_ptr<Expr> step;
    if(check(Tok::Step)){
      advance();
      step = expression();
    }
    auto body = stmt_list_until(Tok::Next, Tok::Next);
    if(!check(Tok::Next)){
      diag.err(peek().line, peek().col, 
               "FOR loop: expected NEXT to close the loop", 
               "Add NEXT to close the FOR loop",
               "FOR " + var + " = 1 TO 10\n  PRINT x\nNEXT");
      return nullptr; 
    }
    advance();
    // Optional variable name after NEXT
    if(check(Tok::Ident)) advance();
    auto s = std::make_unique<ForNext>();
    s->var = std::move(var);
    s->init = std::move(init);
    s->limit = std::move(limit);
    s->step = std::move(step);
    s->body = std::move(body);
    return s;
  }
  if(t.kind==Tok::Ident){
    // Assignment or call
    Token id=advance();
    if(check(Tok::LBracket)){
      // Indexed assignment: name[expr][expr]... = expr
      std::vector<std::unique_ptr<Expr>> indices;
      do {
        advance();
        indices.push_back(expression());
        if(!check(Tok::RBracket)){ diag.err_at(peek().line, peek().col, "INDEX: expected ']' "); return nullptr; }
        advance();
      } while(check(Tok::LBracket));
      if(!check(Tok::Eq)){ diag.err_at(peek().line, peek().col, "INDEX: expected '='"); return nullptr; }
      advance();
      auto val = expression();
      auto s = std::make_unique<AssignIndex>(); s->name=id.lex; s->indices=std::move(indices); s->value=std::move(val); return s;
    }
    if(check(Tok::Eq)){
      advance(); auto v = expression(); auto s = std::make_unique<Assign>(); s->name=id.lex; s->value=std::move(v); return s;
    }
    if(check(Tok::LParen)){
      advance(); std::vector<std::unique_ptr<Expr>> args; if(!check(Tok::RParen)){
        do { args.push_back(expression()); } while(check(Tok::Comma) && (advance(), true));
      }
      if(!check(Tok::RParen)){ diag.err_at(peek().line, peek().col, "CALL: expected ')'"); return nullptr; }
      advance(); auto s = std::make_unique<CallStmt>(); s->name=id.lex; s->args=std::move(args); return s;
    }
    diag.err_at(peek().line, peek().col, "Unexpected identifier usage"); return nullptr;
  }
  if(t.kind==Tok::Call){
    advance();
    if(!check(Tok::Ident)){ diag.err_at(peek().line, peek().col, "CALL: expected function name"); return nullptr; }
    Token id = advance();
    if(!check(Tok::LParen)){ diag.err_at(peek().line, peek().col, "CALL: expected '('"); return nullptr; }
    advance();
    std::vector<std::unique_ptr<Expr>> args; if(!check(Tok::RParen)){
      do { args.push_back(expression()); } while(check(Tok::Comma) && (advance(), true));
    }
    if(!check(Tok::RParen)){ diag.err("CALL: expected ')'"); return nullptr; }
    advance(); auto s = std::make_unique<CallStmt>(); s->name=id.lex; s->args=std::move(args); return s;
  }
  if(t.kind==Tok::Break){
    advance();
    return std::make_unique<Break>();
  }
  if(t.kind==Tok::Continue){
    // CONTINUE (permissive only)
    if(!agk){
      diag.err(peek().line, peek().col,
               "CONTINUE is not allowed in classic dialect",
               "Remove CONTINUE or run without --dialect classic",
               "");
      return nullptr;
    }
    advance();
    return std::make_unique<Continue>();
  }
  if(t.kind==Tok::Eof) return nullptr;
  diag.err_at(peek().line, peek().col, "Unexpected statement"); return nullptr;
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

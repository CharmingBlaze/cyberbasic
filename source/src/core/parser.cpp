#include "bas/parser.hpp"
#include "bas/value.hpp"
#include "bas/ast.hpp"
#include <stdexcept>

using namespace bas;

// Equality operators only; relational operators are handled in comparison()
static bool is_cmp(Tok k){ return k==Tok::Eq||k==Tok::Neq; }

// Helper: Check if token is a statement separator (newline or colon)
static bool is_statement_separator(Tok k) { return k == Tok::Newline || k == Tok::Colon; }

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
      while(peek().kind != Tok::Eof && !check(Tok::Newline) && !check(Tok::Colon)) advance();
      // consume the newline or colon if present
      if(check(Tok::Newline)) advance();
      if(check(Tok::Colon)) advance();
    }
    // Handle statement separator : (colon)
    while(check(Tok::Colon)) {
      advance(); // consume colon
      skipNewlines();
      if(peek().kind==Tok::Eof) break;
      if(auto s = statement()) {
        p.stmts.push_back(std::move(s));
      }
    }
    while(check(Tok::Newline)) advance();
  }
  return p;
}

std::unique_ptr<Stmt> Parser::parse_statement_no_skip() {
    // Language Engineering: Statement parser WITHOUT newline skipping
    // Used by stmt_list_until to avoid redundant skipping
    // Performance: Direct dispatch, no token copying
    // Note: stmt_list_until should have already consumed separators before calling this
    if(peek().kind == Tok::Eof) return nullptr;
    
    // CRITICAL: Safety check - if we're at a separator, something went wrong
    // This is a defensive check - stmt_list_until should have handled this
    // But if a separator slips through, we need to skip it to avoid misclassifying the next statement
    // This should NOT happen if stmt_list_until is working correctly, but we check anyway
    if(peek().kind == Tok::Newline || peek().kind == Tok::Colon) {
        // This indicates a bug in stmt_list_until - it should have consumed separators
        // But we'll handle it gracefully by skipping and retrying
        consume_statement_separators();
        if(peek().kind == Tok::Eof) return nullptr;
        // Re-enter to parse the actual statement (prevents infinite recursion since we skip separators)
        return parse_statement_no_skip();
    }
    
    // At this point, we should be at a statement-starting token (PRINT, VAR, IF, etc.)
    const Token& t = peek();
    return dispatch_statement(t);
}

std::unique_ptr<Stmt> Parser::statement() {
    // Language Engineering: Fast statement dispatcher for top-level parsing
    // Skips leading newlines and dispatches to appropriate parser
    if(peek().kind == Tok::Eof) return nullptr;
    
    // CRITICAL: Guard against stray newlines/colons
    // This prevents "unexpected statement" errors when separators slip through
    if(peek().kind == Tok::Newline || peek().kind == Tok::Colon) {
        consume_statement_separators();
        if(peek().kind == Tok::Eof) return nullptr;
        // Re-enter to parse the actual statement (prevents infinite recursion since we skip separators)
        return statement();
    }
    
    const Token& t = peek();
    return dispatch_statement(t);
}

std::unique_ptr<Stmt> Parser::dispatch_statement(const Token& t) {
    // Language Engineering: Centralized statement dispatch
    // Performance: Single switch, fast path for common statements
    // Extensibility: Easy to add new statement types

    switch (t.kind) {
        case Tok::Print:    return parse_print();
        case Tok::PrintC:   return parse_printc();
        case Tok::Let:      return parse_let();
        case Tok::Var:      return parse_let();
        case Tok::Const:    return parse_const();
        case Tok::Input:    return parse_input();
        case Tok::If:       return parse_if();
        case Tok::While:    return parse_while();
        case Tok::Sub:      return parse_sub_decl();
        case Tok::Function: return parse_function_decl();
        case Tok::Return:   return parse_return();
        case Tok::Gosub:    return parse_gosub();
        case Tok::Goto:     return parse_goto();
        case Tok::End:      return parse_end();
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
        case Tok::Exit: {
            advance();
            auto exitStmt = std::make_unique<Exit>();
            if (check(Tok::Ident)) {
                std::string target = advance().lex;
                for (char& ch : target) ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
                if (target == "FOR" || target == "WHILE" || target == "SUB" || target == "FUNCTION") {
                    exitStmt->target = target;
                } else {
                    diag.err_at(peek().line, peek().col, "EXIT: expected FOR, WHILE, SUB, or FUNCTION");
                    return nullptr;
                }
            } else {
                diag.err_at(peek().line, peek().col, "EXIT: expected target (FOR, WHILE, SUB, or FUNCTION)");
                return nullptr;
            }
            return exitStmt;
        }
        case Tok::Type: {
            return parse_type_decl();
        }
        case Tok::Module: {
            return parse_module_decl();
        }
        case Tok::For: {
            // Check if it's FOR EACH
            if (i + 1 < ts.size() && ts[i + 1].kind == Tok::Each) {
                return parse_for_each();
            }
            return parse_for();
        }
        case Tok::Assert: {
            return parse_assert();
        }
        case Tok::Breakpoint: {
            return parse_breakpoint();
        }
        case Tok::Debug: {
            if (i + 1 < ts.size() && ts[i + 1].kind == Tok::Print) {
                return parse_debug_print();
            }
            break;
        }
        case Tok::Yield: {
            return parse_yield();
        }
        case Tok::Await: {
            return parse_await();
        }
        case Tok::Operator: {
            return parse_operator_decl();
        }
        case Tok::Using: {
            return parse_using();
        }
        case Tok::Enum: {
            return parse_enum();
        }
        case Tok::Union: {
            return parse_union();
        }
        case Tok::Match: {
            // Match can be used as expression or statement
            if (auto match_expr = parse_match()) {
                return std::make_unique<ExprStmt>(std::move(match_expr));
            }
            break;
        }
        case Tok::Try: {
            return parse_try_catch();
        }
        case Tok::Throw: {
            return parse_throw();
        }
        case Tok::Import:
        case Tok::Include: {
            return parse_import();
        }
        default: break;
    }

    // Error recovery: Don't consume token if we can't parse it
    // This allows stmt_list_until to handle error recovery gracefully
    diag.err(t.line, t.col, "unexpected statement", "check syntax", "PRINT x");
    return nullptr;
}

std::vector<std::unique_ptr<Stmt>> Parser::stmt_list_until(Tok end1, Tok end2){
  // Language Engineering: Fast, safe statement list parser with proper error recovery
  // Architecture: Single-pass recursive descent with incremental parsing
  // Performance: Pre-allocated vector, minimal token lookahead, fast path for common cases
  // Critical Fix: Consistent separator handling ensures correct token positioning
  // Pattern: consume separators BEFORE -> parse -> consume separators AFTER
  // This matches the user's guidance: "consume_statement_separators() before/after each statement"
  std::vector<std::unique_ptr<Stmt>> v; 
  v.reserve(16); // Pre-allocate for common case (small blocks)
  
  while(peek().kind!=Tok::Eof && !check(end1) && !check(end2)){
    // CRITICAL: Always consume statement separators (newlines/colons) BEFORE parsing
    // This ensures we're positioned at the beginning of a statement, not on whitespace
    // This is the key fix - we MUST skip separators before trying to parse each statement
    consume_statement_separators();
    
    // Safeguard: Double-check for terminator after consuming separators
    // Check all common block terminators (EndWhile/EndFor don't exist - use Wend/Next)
    if(check(end1) || check(end2) || peek().kind == Tok::Eof || 
       check(Tok::EndIf) || check(Tok::Wend) || check(Tok::Next) ||
       check(Tok::EndFunction) || check(Tok::EndSub) ||
       check(Tok::EndSelect) || check(Tok::EndState) ||
       check(Tok::EndModule) || check(Tok::EndEnum) ||
       check(Tok::EndUnion) || check(Tok::EndTry)) break;
    
    // Parse one statement - use parse_statement_no_skip() since we've already consumed separators
    // This avoids double separator consumption that would happen with statement()
    size_t start_pos = i; // Track position for error recovery
    auto s = parse_statement_no_skip();
    
    if(s) {
      v.push_back(std::move(s));
      // CRITICAL: After successful parse, consume trailing separators
      // This positions us correctly for the next iteration
      // Pattern: consume separators at start -> parse -> consume separators at end
      // This ensures that after parsing "VAR x = 10\n", we consume the \n and are ready for "PRINT x"
      consume_statement_separators();
    } else {
      // Error recovery: Always skip to next separator to prevent cascading failures
      // This matches the user's guidance: "skip forward to the next separator or terminator token"
      if(i > start_pos) {
        // We consumed some tokens but failed - skip to next statement boundary
        consume_statement_separators();
        if(check(end1) || check(end2)) break;
        // Fast recovery: skip to next separator or end token
        while(peek().kind != Tok::Eof && !check(Tok::Newline) && !check(Tok::Colon) && !check(end1) && !check(end2)) {
          advance();
        }
        consume_statement_separators();
      } else {
        // No tokens consumed - we're stuck on an unexpected token
        // Skip it and try again (prevents infinite loop)
        if(!check(end1) && !check(end2) && peek().kind != Tok::Eof) {
          advance();
          consume_statement_separators();
        }
      }
    }
  }
  return v;
}

std::unique_ptr<Expr> Parser::expression(){ 
  auto expr = or_();
  // CRITICAL: Stop at statement separators and block terminators to prevent swallowing the next statement
  // This ensures expressions don't run past their intended end
  if(peek().kind == Tok::Newline || peek().kind == Tok::Colon || peek().kind == Tok::Eof ||
     peek().kind == Tok::EndIf || peek().kind == Tok::Else || peek().kind == Tok::ElseIf ||
     peek().kind == Tok::Wend || peek().kind == Tok::Next ||
     peek().kind == Tok::EndFunction || peek().kind == Tok::EndSub) {
    return expr;
  }
  return expr;
}
std::unique_ptr<Expr> Parser::or_(){
  auto e = xor_();
  // CRITICAL: Stop at statement separators to prevent swallowing the next statement
  while(check(Tok::Or) && !is_statement_separator(peek().kind)){ Tok op=advance().kind; auto r=xor_(); e = std::make_unique<Binary>(std::move(e), op, std::move(r)); }
  return e;
}
std::unique_ptr<Expr> Parser::xor_(){
  auto e = and_();
  while(check(Tok::Xor) && !is_statement_separator(peek().kind)){ Tok op=advance().kind; auto r=and_(); e = std::make_unique<Binary>(std::move(e), op, std::move(r)); }
  return e;
}
std::unique_ptr<Expr> Parser::and_(){
  auto e = equality();
  while(check(Tok::And) && !is_statement_separator(peek().kind)){ Tok op=advance().kind; auto r=equality(); e = std::make_unique<Binary>(std::move(e), op, std::move(r)); }
  return e;
}
std::unique_ptr<Expr> Parser::equality(){
  auto e = comparison();
  while(is_cmp(peek().kind) && !is_statement_separator(peek().kind)){ Tok op=advance().kind; auto r=comparison(); e = std::make_unique<Binary>(std::move(e), op, std::move(r)); }
  return e;
}
std::unique_ptr<Expr> Parser::comparison(){
  auto e = term();
  while((check(Tok::Lt) || check(Tok::Lte) || check(Tok::Gt) || check(Tok::Gte)) && !is_statement_separator(peek().kind)){
    Tok op=advance().kind; auto r=term(); e = std::make_unique<Binary>(std::move(e), op, std::move(r));
  }
  return e;
}
std::unique_ptr<Expr> Parser::term(){
  auto e = factor();
  while((check(Tok::Plus) || check(Tok::Minus)) && !is_statement_separator(peek().kind)){
    Tok op=advance().kind; auto r=factor(); e = std::make_unique<Binary>(std::move(e), op, std::move(r));
  }
  return e;
}
std::unique_ptr<Expr> Parser::factor(){
  auto e = power();
  while((check(Tok::Star) || check(Tok::Slash) || check(Tok::Mod) || check(Tok::IntDiv)) && !is_statement_separator(peek().kind)){
    Tok op=advance().kind; auto r=power(); e = std::make_unique<Binary>(std::move(e), op, std::move(r));
  }
  return e;
}
std::unique_ptr<Expr> Parser::power(){
  auto e = unary();
  while(check(Tok::Power) && !is_statement_separator(peek().kind)){
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
  if(t.kind==Tok::Number || t.kind==Tok::String || t.kind==Tok::True || t.kind==Tok::False ||
     t.kind==Tok::Nil || t.kind==Tok::None || t.kind==Tok::Null){ 
    std::unique_ptr<Expr> base = std::make_unique<Literal>(advance());
    return parse_postfix(std::move(base));
  }
  if(t.kind==Tok::FString){
    return parse_fstring();
  }
  if(t.kind==Tok::LBracket){
    // Array literal: [ e1, e2, ... ]
    advance();
    auto arr = std::make_unique<ArrayLiteral>();
    if(!check(Tok::RBracket)){
      do { arr->elements.push_back(expression()); } while(check(Tok::Comma) && (advance(), true));
    }
    if(!check(Tok::RBracket)) diag.err_at(peek().line, peek().col, "array: expected ']' "); else advance();
    std::unique_ptr<Expr> base = std::move(arr);
    return parse_postfix(std::move(base));
  }
  if(t.kind==Tok::Ident){
    Token id = advance();
    std::unique_ptr<Expr> base;
    
    // Check for special keywords
    if(id.lex == "TYPEOF") {
      return parse_typeof();
    } else if(id.lex == "GETPROPERTIES") {
      return parse_get_properties();
    } else if(id.lex == "GETMETHODS") {
      return parse_get_methods();
    } else if(id.lex == "SUPER") {
      return parse_super_call();
    } else if(id.lex == "LAMBDA") {
      return parse_lambda();
    }
    
    if(check(Tok::LParen)){
      advance(); std::vector<std::unique_ptr<Expr>> args; 
      std::vector<NamedArg> namedArgs;
      if(!check(Tok::RParen)){
        do {
          // Check for named parameter: name := value
          if(check(Tok::Ident) && i + 1 < ts.size() && ts[i + 1].kind == Tok::Eq) {
            Token paramName = advance();
            advance(); // consume := or =
            auto value = expression();
            NamedArg named;
            named.name = paramName.lex;
            named.value = std::move(value);
            namedArgs.push_back(std::move(named));
          } else {
            args.push_back(expression());
          }
        } while(check(Tok::Comma) && (advance(), true));
      }
      if(!check(Tok::RParen)){ diag.err_at(peek().line, peek().col, "call: expected ')'"); return nullptr; }
      advance(); 
      auto call = std::make_unique<Call>(id.lex, std::move(args));
      call->namedArgs = std::move(namedArgs);
      base = std::move(call);
    } else {
      base = std::make_unique<Variable>(id.lex);
    }
    return parse_postfix(std::move(base));
  }
  if(t.kind==Tok::LParen){ 
    advance();
    // Check if this is a tuple literal: (expr, expr, ...)
    // vs a parenthesized expression: (expr)
    if(check(Tok::RParen)) {
      // Empty parentheses - error
      advance();
      diag.err_at(peek().line, peek().col, "expected expression");
      return std::make_unique<Literal>(Token{Tok::Number,"0",t.line,t.col});
    }
    
    auto first = expression();
    if(!first) {
      diag.err_at(peek().line, peek().col, "expected expression");
      return std::make_unique<Literal>(Token{Tok::Number,"0",t.line,t.col});
    }
    
    // If there's a comma, it's a tuple
    if(check(Tok::Comma)) {
      std::vector<std::unique_ptr<Expr>> elements;
      elements.push_back(std::move(first));
      advance(); // consume comma
      do {
        elements.push_back(expression());
      } while(match(Tok::Comma));
      if(!check(Tok::RParen)) {
        diag.err_at(peek().line, peek().col, "tuple: expected ')'");
        return std::make_unique<Literal>(Token{Tok::Number,"0",t.line,t.col});
      }
      advance(); // consume ')'
      auto tuple = std::make_unique<TupleLiteral>();
      tuple->elements = std::move(elements);
      return tuple;
    }
    
    // Single expression in parentheses (not a tuple)
    if(!check(Tok::RParen)) {
      diag.err_at(peek().line, peek().col, "expected ')'"); 
      return std::make_unique<Literal>(Token{Tok::Number,"0",t.line,t.col});
    }
    advance(); // consume ')'
    return parse_postfix(std::move(first));
  }
  diag.err_at(t.line, t.col, "expected expression"); return std::make_unique<Literal>(Token{Tok::Number,"0",t.line,t.col});
}

std::unique_ptr<Expr> Parser::parse_postfix(std::unique_ptr<Expr> base) {
  // Handle postfix operations: indexing [ ], member access . , method calls ( )
  // CRITICAL: Stop at statement separators and block terminators to prevent swallowing the next statement
  while(true) {
    // Stop if we hit a statement separator or block terminator
    if(is_statement_separator(peek().kind) || peek().kind == Tok::Eof ||
       peek().kind == Tok::EndIf || peek().kind == Tok::Else || peek().kind == Tok::ElseIf ||
       peek().kind == Tok::Wend || peek().kind == Tok::Next ||
       peek().kind == Tok::EndFunction || peek().kind == Tok::EndSub) {
      break;
    }
    if(check(Tok::LBracket)) {
      // Array indexing: base[index]
      advance();
      auto idx = expression();
      if(!check(Tok::RBracket)) {
        diag.err_at(peek().line, peek().col, "index: expected ']'");
        break;
      }
      advance();
      base = std::make_unique<Index>(std::move(base), std::move(idx));
    } else if(check(Tok::Dot)) {
      // Member access: base.member or base.method(args)
      advance();
      if(!check(Tok::Ident)) {
        diag.err_at(peek().line, peek().col, "member access: expected identifier after '.'");
        break;
      }
      Token member = advance();
      if(check(Tok::LParen)) {
        // Method call: base.method(args)
        advance();
        std::vector<std::unique_ptr<Expr>> args;
        if(!check(Tok::RParen)) {
          do {
            args.push_back(expression());
          } while(check(Tok::Comma) && (advance(), true));
        }
        if(!check(Tok::RParen)) {
          diag.err_at(peek().line, peek().col, "method call: expected ')'");
          break;
        }
        advance();
        base = std::make_unique<MethodCall>(std::move(base), member.lex, std::move(args));
      } else {
        // Property access: base.member
        base = std::make_unique<MemberAccess>(std::move(base), member.lex);
      }
    } else {
      // No more postfix operations
      break;
    }
  }
  return base;
}

std::unique_ptr<Stmt> Parser::parse_print() {
    advance(); // consume PRINT
    auto e = expression();
    auto s = std::make_unique<Print>();
    s->value = std::move(e);
    return s;
}

std::unique_ptr<Stmt> Parser::parse_printc() {
    advance(); // consume PRINTC
    auto e = expression();
    auto s = std::make_unique<PrintC>();
    s->value = std::move(e);
    return s;
}

std::unique_ptr<Stmt> Parser::parse_let() {
    // VAR/LET statement parser - works everywhere (top-level, IF blocks, WHILE loops, etc.)
    // This is a critical function that must be robust and safe
    
    // Consume VAR or LET token (we know it's there because statement() checked)
    advance();
    
    // Check for destructuring assignment: VAR (x, y) = tuple
    if (check(Tok::LParen)) {
        advance(); // consume '('
        std::vector<std::string> names;
        if (!check(Tok::RParen)) {
            do {
                if (!check(Tok::Ident)) {
                    diag.err_at(peek().line, peek().col, "destructuring: expected variable name");
                    return nullptr;
                }
                names.push_back(advance().lex);
            } while (match(Tok::Comma));
        }
        if (!match(Tok::RParen)) {
            diag.err_at(peek().line, peek().col, "destructuring: expected ')'");
            return nullptr;
        }
        if (!match(Tok::Eq)) {
            diag.err_at(peek().line, peek().col, "destructuring: expected '='");
            return nullptr;
        }
        auto value = expression();
        if (!value) {
            diag.err_at(peek().line, peek().col, "destructuring: invalid value expression");
            return nullptr;
        }
        auto destr = std::make_unique<DestructureAssign>();
        destr->names = std::move(names);
        destr->value = std::move(value);
        return destr;
    }
    
    // Require identifier - this is mandatory for VAR/LET
    if (!check(Tok::Ident)) {
        diag.err(peek().line, peek().col, "Assignment: expected variable name", 
                 "Make sure you have a valid variable name", 
                 "LET x = 10 or VAR x = 10");
        return nullptr;
    }
    std::string name = advance().lex;
    
    // Optional type annotation: VAR x AS Integer = 10
    std::string typeName;
    bool hasType = false;
    if (match(Tok::As)) {
        if (!check(Tok::Ident)) {
            diag.err_at(peek().line, peek().col, "Type annotation: expected type name after AS");
            return nullptr;
        }
        typeName = advance().lex;
        hasType = true;
    }
    
    // Check for indexed assignment: VAR arr[0] = value
    if (check(Tok::LBracket)) {
        std::vector<std::unique_ptr<Expr>> indices;
        do {
            advance(); // consume '['
            auto idx_expr = expression();
            if (!idx_expr) {
                diag.err_at(peek().line, peek().col, "Indexed assignment: invalid index expression");
                return nullptr;
            }
            indices.push_back(std::move(idx_expr));
            if (!match(Tok::RBracket)) {
                diag.err_at(peek().line, peek().col, "Indexed assignment: expected ']'");
                return nullptr;
            }
        } while (check(Tok::LBracket));
        
        // Require '=' for assignment
        if (!match(Tok::Eq)) {
            diag.err(peek().line, peek().col, 
                     "Indexed assignment: expected '=' after indexes", 
                     "Add '=' to assign a value", 
                     "LET arr[i] = value or VAR arr[i] = value");
            return nullptr;
        }
        
        auto val = expression();
        if (!val) {
            diag.err_at(peek().line, peek().col, "Indexed assignment: invalid value expression");
            return nullptr;
        }
        
        auto s = std::make_unique<AssignIndex>();
        s->name = std::move(name);
        s->indices = std::move(indices);
        s->value = std::move(val);
        return s;
    } 
    
    // Simple assignment: VAR x = value
    // Require '=' - this is what makes it an assignment, not just a declaration
    if (!match(Tok::Eq)) {
        diag.err(peek().line, peek().col, 
                 "Assignment: expected '=' after variable name", 
                 "Add an equals sign to assign a value", 
                 "LET x = value or VAR x = value");
        return nullptr;
    }
    
    // Parse the value expression
    auto v = expression();
    if (!v) {
        diag.err_at(peek().line, peek().col, "Assignment: invalid value expression");
        return nullptr;
    }
    
    // Successfully parsed VAR/LET assignment
    // Note: We do NOT consume trailing newlines here - that's handled by the caller
    // (either stmt_list_until for blocks, or parse() for top-level)
    auto s = std::make_unique<Let>();
    s->name = std::move(name);
    s->value = std::move(v);
    s->typeName = std::move(typeName);
    s->hasType = hasType;
    return s;
}

std::unique_ptr<Stmt> Parser::parse_const() {
    advance(); // consume CONST
    if (!check(Tok::Ident)) {
        diag.err(peek().line, peek().col, "CONST: expected constant name", "Make sure you have a valid constant name", "CONST PI = 3.14159");
        return nullptr;
    }
    std::string name = advance().lex;
    
    if (!match(Tok::Eq)) {
        diag.err(peek().line, peek().col, "CONST: expected '=' after constant name", "Add an equals sign to assign a value", "CONST PI = 3.14159");
        return nullptr;
    }
    auto v = expression();
    auto s = std::make_unique<ConstDecl>();
    s->name = std::move(name);
    s->value = std::move(v);
    return s;
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
        // CRITICAL: stmt_list_until stops at ElseIf, Else, or EndIf
        // We pass EndIf as well so it stops correctly and doesn't try to parse ENDIF as a statement
        br.body = stmt_list_until(Tok::ElseIf, Tok::Else);
        // After stmt_list_until returns, we should be positioned at EndIf, ElseIf, or Else
        // But we also need to handle EndIf separately since it's not in the terminator list
        // Actually, stmt_list_until will stop when it encounters EndIf because it's not a valid statement
        // But to be safe, let's make sure we consume any trailing separators
        consume_statement_separators();
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
    skipNewlines(); // Align for next statement
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
    // Language Engineering: match() already consumed WEND, don't advance() again
    skipNewlines(); // Align for next statement
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
    std::vector<FunctionParam> params;
    if (match(Tok::LParen)) {
        if (!check(Tok::RParen)) {
            do {
                params.push_back(parse_function_param());
            } while (match(Tok::Comma));
        }
        if (!match(Tok::RParen)) {
            diag.err(peek().line, peek().col, "FUNCTION: expected ')' to close parameters");
            return nullptr;
        }
    }
    
    // Check for return type
    std::string returnType;
    bool hasReturnType = false;
    if (match(Tok::As)) {
        if (!check(Tok::Ident)) {
            diag.err(peek().line, peek().col, "FUNCTION: expected return type after AS");
            return nullptr;
        }
        returnType = advance().lex;
        hasReturnType = true;
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
    f->returnType = std::move(returnType);
    f->hasReturnType = hasReturnType;
    f->body = std::move(body);
    return f;
}

std::unique_ptr<Stmt> Parser::parse_return() {
    advance(); // consume RETURN
    std::unique_ptr<Expr> val;
    // Language Engineering: Use expression() directly - it handles all valid expression starts
    // This avoids duplicating token checks and supports RETURN NOT x, RETURN TYPEOF y, etc.
    if (!check(Tok::Newline) && !check(Tok::Eof)) {
        val = expression();
    }
    auto r = std::make_unique<Return>();
    r->value = std::move(val);
    return r;
}

std::unique_ptr<Stmt> Parser::parse_gosub() {
    advance(); // consume GOSUB
    if (!check(Tok::Ident)) {
        diag.err(peek().line, peek().col, "GOSUB: expected label name");
        return nullptr;
    }
    std::string label = advance().lex;
    auto s = std::make_unique<Gosub>();
    s->label = std::move(label);
    return s;
}

std::unique_ptr<Stmt> Parser::parse_goto() {
    advance(); // consume GOTO
    if (!check(Tok::Ident)) {
        diag.err(peek().line, peek().col, "GOTO: expected label name");
        return nullptr;
    }
    std::string label = advance().lex;
    auto s = std::make_unique<Goto>();
    s->label = std::move(label);
    return s;
}

std::unique_ptr<Stmt> Parser::parse_end() {
    advance(); // consume END
    return std::make_unique<End>();
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
    // Language Engineering: match() already consumed RParen, don't advance() again

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
    
    // Check for label: identifier followed by colon
    if (check(Tok::Colon)) {
        advance(); // consume colon
        auto label = std::make_unique<Label>();
        label->name = id.lex;
        return label;
    }
    
    std::unique_ptr<Expr> base_expr = std::make_unique<Variable>(id.lex);

    // Check for member access: name.member
    if (check(Tok::Dot)) {
        advance(); // consume '.'
        if (!check(Tok::Ident)) {
            diag.err_at(peek().line, peek().col, "MEMBER: expected identifier after '.'");
            return nullptr;
        }
        Token member = advance();
        base_expr = std::make_unique<MemberAccess>(std::move(base_expr), member.lex);
        
        // Check if this is a member assignment: name.member = value
        if (match(Tok::Eq)) {
            auto val = expression();
            auto s = std::make_unique<AssignMember>();
            s->object = std::move(base_expr);
            s->member = member.lex;
            s->value = std::move(val);
            return s;
        }
        // Otherwise, it's just a member access expression statement
        auto s = std::make_unique<ExprStmt>();
        s->expr = std::move(base_expr);
        return s;
    }

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

// ============================================================================
// Parser Extension Functions - Modular implementations for new features
// ============================================================================

// Parse TYPE declaration: TYPE Name [EXTENDS ParentType] ... END TYPE
std::unique_ptr<Stmt> Parser::parse_type_decl() {
    advance(); // consume TYPE
    if (!check(Tok::Ident)) {
        diag.err_at(peek().line, peek().col, "TYPE: expected type name");
        return nullptr;
    }
    Token typeName = advance();
    auto decl = std::make_unique<TypeDecl>();
    decl->name = typeName.lex;
    
    // Check for EXTENDS
    if (match(Tok::Extends)) {
        if (!check(Tok::Ident)) {
            diag.err_at(peek().line, peek().col, "TYPE: expected parent type name after EXTENDS");
            return nullptr;
        }
        decl->parentType = advance().lex;
        decl->hasParent = true;
    }
    
    // Parse fields and methods until END TYPE
    while (peek().kind != Tok::Eof && !check(Tok::EndType)) {
        skipNewlines();
        if (check(Tok::EndType)) break;
        
        if (check(Tok::Ident)) {
            Token fieldName = advance();
            TypeField field;
            field.name = fieldName.lex;
            
            // Check for AS type annotation
            if (match(Tok::As)) {
                if (!check(Tok::Ident)) {
                    diag.err_at(peek().line, peek().col, "TYPE: expected type name after AS");
                    return nullptr;
                }
                field.typeName = advance().lex;
                field.hasType = true;
            }
            
            decl->fields.push_back(field);
        } else if (check(Tok::Function) || check(Tok::Sub)) {
            // Method declaration
            auto method = statement();
            if (method) {
                decl->methods.push_back(std::move(method));
            }
        } else {
            advance(); // Skip unknown token
        }
    }
    
    if (!match(Tok::EndType)) {
        diag.err_at(peek().line, peek().col, "TYPE: expected END TYPE");
        return nullptr;
    }
    
    return decl;
}

// Parse MODULE declaration: MODULE Name ... END MODULE
std::unique_ptr<Stmt> Parser::parse_module_decl() {
    advance(); // consume MODULE
    if (!check(Tok::Ident)) {
        diag.err_at(peek().line, peek().col, "MODULE: expected module name");
        return nullptr;
    }
    Token moduleName = advance();
    auto decl = std::make_unique<ModuleDecl>();
    decl->name = moduleName.lex;
    
    // Check for PUBLIC/PRIVATE
    if (match(Tok::Public)) {
        decl->isPublic = true;
    } else if (match(Tok::Private)) {
        decl->isPublic = false;
    }
    
    // Parse body until END MODULE
    decl->body = stmt_list_until(Tok::EndModule, Tok::Eof);
    
    if (!match(Tok::EndModule)) {
        diag.err_at(peek().line, peek().col, "MODULE: expected END MODULE");
        return nullptr;
    }
    skipNewlines(); // Align for next statement
    return decl;
}

// Parse FOR EACH loop: FOR EACH var IN collection ... NEXT
std::unique_ptr<Stmt> Parser::parse_for_each() {
    advance(); // consume FOR
    advance(); // consume EACH
    if (!check(Tok::Ident)) {
        diag.err_at(peek().line, peek().col, "FOR EACH: expected variable name");
        return nullptr;
    }
    Token varName = advance();
    if (!match(Tok::In)) {
        diag.err_at(peek().line, peek().col, "FOR EACH: expected IN");
        return nullptr;
    }
    auto collection = expression();
    auto stmt = std::make_unique<ForEach>();
    stmt->var = varName.lex;
    stmt->collection = std::move(collection);
    stmt->body = stmt_list_until(Tok::Next, Tok::Eof);
    if (!match(Tok::Next)) {
        diag.err_at(peek().line, peek().col, "FOR EACH: expected NEXT");
        return nullptr;
    }
    // Optional variable after NEXT (matches FOR loop behavior)
    if (check(Tok::Ident)) advance();
    skipNewlines(); // Align for next statement
    return stmt;
}

// Parse ASSERT statement: ASSERT condition [, message]
std::unique_ptr<Stmt> Parser::parse_assert() {
    advance(); // consume ASSERT
    auto condition = expression();
    auto stmt = std::make_unique<AssertStmt>();
    stmt->condition = std::move(condition);
    if (match(Tok::Comma)) {
        stmt->message = expression();
    }
    return stmt;
}

// Parse BREAKPOINT statement
std::unique_ptr<Stmt> Parser::parse_breakpoint() {
    advance(); // consume BREAKPOINT
    return std::make_unique<BreakpointStmt>();
}

// Parse DEBUG PRINT statement
std::unique_ptr<Stmt> Parser::parse_debug_print() {
    advance(); // consume DEBUG
    advance(); // consume PRINT
    auto value = expression();
    auto stmt = std::make_unique<DebugPrintStmt>();
    stmt->value = std::move(value);
    return stmt;
}

// Parse AWAIT statement: AWAIT expression
std::unique_ptr<Stmt> Parser::parse_await() {
    advance(); // consume AWAIT
    auto expr = expression();
    if (!expr) {
        diag.err_at(peek().line, peek().col, "AWAIT: expected expression");
        return nullptr;
    }
    auto awaitStmt = std::make_unique<AwaitStmt>();
    awaitStmt->expression = std::move(expr);
    return awaitStmt;
}

// Parse YIELD statement: YIELD [value]
std::unique_ptr<Stmt> Parser::parse_yield() {
    advance(); // consume YIELD
    auto stmt = std::make_unique<YieldStmt>();
    if (!check(Tok::Newline) && !check(Tok::Eof)) {
        stmt->value = expression();
    }
    return stmt;
}

// Parse OPERATOR declaration: OPERATOR op FOR typeName ... END OPERATOR
std::unique_ptr<Stmt> Parser::parse_operator_decl() {
    advance(); // consume OPERATOR
    if (peek().kind == Tok::Plus || peek().kind == Tok::Minus || 
        peek().kind == Tok::Star || peek().kind == Tok::Slash ||
        peek().kind == Tok::Eq || peek().kind == Tok::Lt || 
        peek().kind == Tok::Gt || peek().kind == Tok::Lte || 
        peek().kind == Tok::Gte || peek().kind == Tok::Neq) {
        Tok op = advance().kind;
        if (!match(Tok::For)) {
            diag.err_at(peek().line, peek().col, "OPERATOR: expected FOR");
            return nullptr;
        }
        if (!check(Tok::Ident)) {
            diag.err_at(peek().line, peek().col, "OPERATOR: expected type name");
            return nullptr;
        }
        Token typeName = advance();
        auto decl = std::make_unique<OperatorDecl>();
        decl->op = op;
        decl->typeName = typeName.lex;
        
        // Parse parameters
        if (match(Tok::LParen)) {
            if (!check(Tok::RParen)) {
                do {
                    decl->params.push_back(parse_function_param());
                } while (match(Tok::Comma));
            }
            if (!match(Tok::RParen)) {
                diag.err_at(peek().line, peek().col, "OPERATOR: expected ')'");
                return nullptr;
            }
        }
        
        // Parse body
        decl->body = stmt_list_until(Tok::EndOperator, Tok::Eof);
        if (!match(Tok::EndOperator)) {
            diag.err_at(peek().line, peek().col, "OPERATOR: expected END OPERATOR");
            return nullptr;
        }
        skipNewlines(); // Align for next statement
        return decl;
    }
    diag.err_at(peek().line, peek().col, "OPERATOR: expected operator symbol");
    return nullptr;
}

// Parse function parameter with optional type and default value
FunctionParam Parser::parse_function_param() {
    FunctionParam param;
    if (!check(Tok::Ident)) {
        diag.err_at(peek().line, peek().col, "Parameter: expected parameter name");
        return param;
    }
    param.name = advance().lex;
    
    // Check for AS type annotation
    if (match(Tok::As)) {
        if (!check(Tok::Ident)) {
            diag.err_at(peek().line, peek().col, "Parameter: expected type name after AS");
            return param;
        }
        param.typeName = advance().lex;
        param.hasType = true;
    }
    
    // Check for default value
    if (match(Tok::Eq)) {
        param.defaultValue = expression();
        param.hasDefault = true;
    }
    
    return param;
}

// Parse lambda expression: LAMBDA (params) ... END LAMBDA
std::unique_ptr<Expr> Parser::parse_lambda() {
    advance(); // consume LAMBDA
    auto lambda = std::make_unique<LambdaExpr>();
    
    // Parse parameters
    if (match(Tok::LParen)) {
        if (!check(Tok::RParen)) {
            do {
                lambda->params.push_back(parse_function_param());
            } while (match(Tok::Comma));
        }
        if (!match(Tok::RParen)) {
            diag.err_at(peek().line, peek().col, "LAMBDA: expected ')'");
            return nullptr;
        }
    }
    
    // Check for return type
    if (match(Tok::As)) {
        if (!check(Tok::Ident)) {
            diag.err_at(peek().line, peek().col, "LAMBDA: expected return type");
            return nullptr;
        }
        lambda->returnType = advance().lex;
        lambda->hasReturnType = true;
    }
    
    // Parse body
    lambda->body = stmt_list_until(Tok::EndLambda, Tok::Eof);
    if (!match(Tok::EndLambda)) {
        diag.err_at(peek().line, peek().col, "LAMBDA: expected END LAMBDA");
        return nullptr;
    }
    
    return lambda;
}

// Parse interpolated string: "text {expr} more text"
// Note: This requires lexer support for $ prefix or {} syntax
std::unique_ptr<Expr> Parser::parse_interpolated_string() {
    // Placeholder - would need lexer changes to support $"..."
    auto interp = std::make_unique<InterpolatedString>();
    // Implementation would parse string with embedded expressions
    return interp;
}

// Parse f-string: f"text {expr} more text"
std::unique_ptr<Expr> Parser::parse_fstring() {
    Token fstr = advance(); // Consume FString token
    auto interp = std::make_unique<InterpolatedString>();
    
    // Parse the f-string content - look for {expr} patterns
    std::string content = fstr.lex;
    size_t pos = 0;
    std::string current_text;
    
    while (pos < content.length()) {
        if (content[pos] == '{' && pos + 1 < content.length()) {
            // Found expression start
            if (!current_text.empty()) {
                interp->parts.push_back(std::make_unique<Literal>(Token{Tok::String, current_text, fstr.line, fstr.col}));
                current_text.clear();
            }
            
            // Find matching }
            size_t expr_start = pos + 1;
            size_t expr_end = content.find('}', expr_start);
            if (expr_end == std::string::npos) {
                diag.err_at(fstr.line, fstr.col, "f-string: unmatched '{'");
                break;
            }
            
            // Parse expression between { and }
            std::string expr_str = content.substr(expr_start, expr_end - expr_start);
            // For now, treat as a simple variable name - full expression parsing would require more work
            // This is a simplified implementation
            interp->parts.push_back(std::make_unique<Variable>(expr_str));
            
            pos = expr_end + 1;
        } else if (content[pos] == '}' && pos + 1 < content.length() && content[pos + 1] == '}') {
            // Escaped }
            current_text += '}';
            pos += 2;
        } else {
            current_text += content[pos];
            pos++;
        }
    }
    
    if (!current_text.empty()) {
        interp->parts.push_back(std::make_unique<Literal>(Token{Tok::String, current_text, fstr.line, fstr.col}));
    }
    
    return interp;
}

// Parse range literal: 1..10 or "a".."z"
std::unique_ptr<Expr> Parser::parse_range() {
    auto start = expression();
    if (!match(Tok::Spread)) {
        diag.err_at(peek().line, peek().col, "range: expected '..'");
        return nullptr;
    }
    auto end = expression();
    auto range = std::make_unique<RangeLiteral>();
    range->start = std::move(start);
    range->end = std::move(end);
    return range;
}

// Parse array comprehension: [expr FOR var IN collection IF cond]
std::unique_ptr<Expr> Parser::parse_comprehension() {
    if (!match(Tok::LBracket)) {
        diag.err_at(peek().line, peek().col, "comprehension: expected '['");
        return nullptr;
    }
    
    auto expr = expression();
    if (!match(Tok::For)) {
        diag.err_at(peek().line, peek().col, "comprehension: expected FOR");
        return nullptr;
    }
    
    if (!check(Tok::Ident)) {
        diag.err_at(peek().line, peek().col, "comprehension: expected variable name");
        return nullptr;
    }
    std::string var = advance().lex;
    
    if (!match(Tok::In)) {
        diag.err_at(peek().line, peek().col, "comprehension: expected IN");
        return nullptr;
    }
    
    auto collection = expression();
    auto comp = std::make_unique<ArrayComprehension>();
    comp->expr = std::move(expr);
    comp->var = var;
    comp->collection = std::move(collection);
    
    // Optional IF condition
    if (check(Tok::If)) {
        advance();
        comp->condition = expression();
    }
    
    if (!match(Tok::RBracket)) {
        diag.err_at(peek().line, peek().col, "comprehension: expected ']'");
        return nullptr;
    }
    
    return comp;
}

// Parse match expression: MATCH value CASE pattern: result CASE ELSE: default END MATCH
std::unique_ptr<Expr> Parser::parse_match() {
    if (!match(Tok::Match)) {
        return nullptr;
    }
    
    auto value = expression();
    auto match_expr = std::make_unique<MatchExpr>();
    match_expr->value = std::move(value);
    
    while (check(Tok::Case)) {
        advance();
        MatchExpr::Case c;
        c.pattern = expression();
        if (!match(Tok::Colon)) {
            diag.err_at(peek().line, peek().col, "match: expected ':'");
            return nullptr;
        }
        c.result = expression();
        match_expr->cases.push_back(std::move(c));
    }
    
    if (check(Tok::Else)) {
        advance();
        if (!match(Tok::Colon)) {
            diag.err_at(peek().line, peek().col, "match: expected ':'");
            return nullptr;
        }
        match_expr->defaultCase = expression();
    }
    
    if (!match(Tok::EndMatch)) {
        diag.err_at(peek().line, peek().col, "match: expected END MATCH");
        return nullptr;
    }
    skipNewlines(); // Align for next statement
    
    return match_expr;
}

// Parse USING block: USING var = expr ... END USING
std::unique_ptr<Stmt> Parser::parse_using() {
    if (!match(Tok::Using)) {
        return nullptr;
    }
    
    if (!check(Tok::Ident)) {
        diag.err_at(peek().line, peek().col, "using: expected variable name");
        return nullptr;
    }
    std::string varName = advance().lex;
    
    if (!match(Tok::Eq)) {
        diag.err_at(peek().line, peek().col, "using: expected '='");
        return nullptr;
    }
    
    auto resource = expression();
    auto using_block = std::make_unique<UsingBlock>();
    using_block->varName = varName;
    using_block->resource = std::move(resource);
    using_block->body = stmt_list_until(Tok::EndUsing, Tok::EndUsing);
    
    if (!match(Tok::EndUsing)) {
        diag.err_at(peek().line, peek().col, "using: expected END USING");
        return nullptr;
    }
    skipNewlines(); // Align for next statement
    
    return using_block;
}

// Parse ENUM declaration: ENUM Name value1, value2, ... END ENUM
std::unique_ptr<Stmt> Parser::parse_enum() {
    if (!match(Tok::Enum)) {
        return nullptr;
    }
    
    if (!check(Tok::Ident)) {
        diag.err_at(peek().line, peek().col, "enum: expected enum name");
        return nullptr;
    }
    std::string name = advance().lex;
    
    auto enum_decl = std::make_unique<EnumDecl>();
    enum_decl->name = name;
    
    // Consume any separators after enum name
    consume_statement_separators();
    
    // Parse enum values - support both comma-separated and newline-separated
    // Grammar: EnumMember { (Comma | Newline) EnumMember }*
    // Continue until we see END ENUM (either as ENDENUM token or END + ENUM tokens)
    while (peek().kind != Tok::Eof) {
        // Consume statement separators (newlines/colons) before each value
        // This allows multi-line enum declarations
        consume_statement_separators();
        
        // Check for END ENUM (two tokens: END + ENUM) or ENDENUM (one token)
        if (check(Tok::End) && i + 1 < ts.size() && ts[i + 1].kind == Tok::Enum) {
            // "END ENUM" - consume both tokens
            advance(); // consume END
            advance(); // consume ENUM
            break;
        } else if (check(Tok::EndEnum)) {
            // "ENDENUM" - single token
            advance(); // consume ENDENUM
            break;
        }
        
        // Check if we've reached end of file
        if (peek().kind == Tok::Eof) {
            diag.err_at(peek().line, peek().col, "enum: expected END ENUM before end of file");
            return nullptr;
        }
        
        if (!check(Tok::Ident)) {
            diag.err_at(peek().line, peek().col, "enum: expected value name or END ENUM");
            return nullptr;
        }
        
        std::string valueName = advance().lex;
        EnumValue enumVal;
        enumVal.name = valueName;
        
        // Check for custom value: ANOTHER_THING = -1
        if (match(Tok::Eq)) {
            enumVal.value = expression();
        } else {
            enumVal.value = nullptr; // Auto-assign
        }
        enum_decl->values.push_back(std::move(enumVal));
        
        // Consume separators after the value (and optional expression)
        consume_statement_separators();
        
        // Check for comma separator (single-line style: "Red, Green")
        if (match(Tok::Comma)) {
            // Consume any separators after comma (allows "Red, \n Green")
            consume_statement_separators();
            // Continue to next value
            continue;
        }
        
        // No comma found - check what's next
        // If it's an identifier, it's the next value on a new line (multi-line style)
        // If it's END ENUM, we're done (checked at start of loop)
        // Otherwise, continue to next iteration to check for END ENUM
        if (check(Tok::Ident)) {
            // Next identifier on new line - continue parsing (multi-line style)
            continue;
        }
        // If not an identifier and not END ENUM, loop will check again at the top
    }
    skipNewlines(); // Align for next statement
    
    return enum_decl;
}

// Parse UNION declaration: UNION Name AS type1, type2, ... END UNION
std::unique_ptr<Stmt> Parser::parse_union() {
    if (!match(Tok::Union)) {
        return nullptr;
    }
    
    if (!check(Tok::Ident)) {
        diag.err_at(peek().line, peek().col, "union: expected union name");
        return nullptr;
    }
    std::string name = advance().lex;
    
    auto union_decl = std::make_unique<UnionDecl>();
    union_decl->name = name;
    
    if (match(Tok::As)) {
        do {
            if (!check(Tok::Ident)) {
                diag.err_at(peek().line, peek().col, "union: expected type name");
                return nullptr;
            }
            union_decl->types.push_back(advance().lex);
        } while (match(Tok::Comma));
    }
    
    if (!match(Tok::EndUnion)) {
        diag.err_at(peek().line, peek().col, "union: expected END UNION");
        return nullptr;
    }
    skipNewlines(); // Align for next statement
    
    return union_decl;
}

// Parse IMPORT/INCLUDE statement with flexible syntax:
// IMPORT "file.bas" or INCLUDE "file.bas" 
// INCLUDE file.bas (without quotes)
// INCLUDE (file.bas) (with parentheses)
// INCLUDE ("file.bas") (with parentheses and quotes)
std::unique_ptr<Stmt> Parser::parse_import() {
    // Consume IMPORT or INCLUDE token
    bool is_include = check(Tok::Include);
    advance(); // consume IMPORT or INCLUDE
    
    std::string file_path;
    bool has_parentheses = false;
    
    // Check for optional parentheses: INCLUDE (file.bas)
    if (check(Tok::LParen)) {
        advance(); // consume '('
        has_parentheses = true;
    }
    
    // Handle different path formats
    if (check(Tok::String)) {
        // Quoted string: "file.bas"
        Token path_token = advance();
        file_path = path_token.lex;
    } else if (check(Tok::Ident)) {
        // Unquoted identifier: file.bas (need to handle extensions)
        std::string base_name = advance().lex; // consume identifier
        
        // Check for file extension with dot
        if (check(Tok::Dot)) {
            advance(); // consume '.'
            if (check(Tok::Ident)) {
                std::string extension = advance().lex; // consume extension
                file_path = base_name + "." + extension;
            } else {
                diag.err_at(peek().line, peek().col, 
                           is_include ? "INCLUDE: expected file extension after '.'" : "IMPORT: expected file extension after '.'", 
                           "Provide a valid extension", 
                           is_include ? "INCLUDE file.bas" : "IMPORT file.bas");
                return nullptr;
            }
        } else {
            // No extension provided, assume .bas
            file_path = base_name + ".bas";
        }
    } else {
        // Invalid syntax
        std::string expected = has_parentheses ? 
            (is_include ? "INCLUDE (\"file.bas\") or INCLUDE (file.bas)" : "IMPORT (\"file.bas\") or IMPORT (file.bas)") :
            (is_include ? "INCLUDE \"file.bas\" or INCLUDE file.bas" : "IMPORT \"file.bas\" or IMPORT file.bas");
        
        diag.err_at(peek().line, peek().col, 
                   is_include ? "INCLUDE: expected file path" : "IMPORT: expected file path", 
                   "Provide a file path", expected);
        return nullptr;
    }
    
    // Check for closing parenthesis if we had an opening one
    if (has_parentheses) {
        if (!check(Tok::RParen)) {
            diag.err_at(peek().line, peek().col, 
                       is_include ? "INCLUDE: expected closing ')'" : "IMPORT: expected closing ')'", 
                       "Close the parentheses", 
                       is_include ? "INCLUDE (file.bas)" : "IMPORT (file.bas)");
            return nullptr;
        }
        advance(); // consume ')'
    }
    
    auto import_stmt = std::make_unique<ImportStmt>();
    import_stmt->path = file_path;
    
    return import_stmt;
}

// Parse map/dictionary literal: {key: value, key2: value2}
// Note: Currently using LBracket/RBracket as placeholder - would need curly braces
std::unique_ptr<Expr> Parser::parse_map_literal() {
    if (!match(Tok::LBracket)) {
        diag.err_at(peek().line, peek().col, "Map literal: expected '{'");
        return nullptr;
    }
    auto map = std::make_unique<MapLiteral>();
    
    if (!check(Tok::RBracket)) {
        do {
            auto key = expression();
            if (!match(Tok::Eq)) {
                diag.err_at(peek().line, peek().col, "Map literal: expected '='");
                return nullptr;
            }
            auto value = expression();
            map->entries.emplace_back(std::move(key), std::move(value));
        } while (match(Tok::Comma));
    }
    
    if (!match(Tok::RBracket)) {
        diag.err_at(peek().line, peek().col, "Map literal: expected '}'");
        return nullptr;
    }
    
    return map;
}

// Parse TYPEOF expression: TYPEOF(expr)
std::unique_ptr<Expr> Parser::parse_typeof() {
    advance(); // consume TYPEOF
    if (!match(Tok::LParen)) {
        diag.err_at(peek().line, peek().col, "TYPEOF: expected '('");
        return nullptr;
    }
    auto value = expression();
    if (!match(Tok::RParen)) {
        diag.err_at(peek().line, peek().col, "TYPEOF: expected ')'");
        return nullptr;
    }
    auto expr = std::make_unique<TypeOfExpr>();
    expr->value = std::move(value);
    return expr;
}

// Parse GETPROPERTIES expression: GETPROPERTIES(expr)
std::unique_ptr<Expr> Parser::parse_get_properties() {
    advance(); // consume GETPROPERTIES (would be parsed as identifier)
    if (!match(Tok::LParen)) {
        diag.err_at(peek().line, peek().col, "GETPROPERTIES: expected '('");
        return nullptr;
    }
    auto value = expression();
    if (!match(Tok::RParen)) {
        diag.err_at(peek().line, peek().col, "GETPROPERTIES: expected ')'");
        return nullptr;
    }
    auto expr = std::make_unique<GetPropertiesExpr>();
    expr->value = std::move(value);
    return expr;
}

// Parse GETMETHODS expression: GETMETHODS(expr)
std::unique_ptr<Expr> Parser::parse_get_methods() {
    advance(); // consume GETMETHODS (would be parsed as identifier)
    if (!match(Tok::LParen)) {
        diag.err_at(peek().line, peek().col, "GETMETHODS: expected '('");
        return nullptr;
    }
    auto value = expression();
    if (!match(Tok::RParen)) {
        diag.err_at(peek().line, peek().col, "GETMETHODS: expected ')'");
        return nullptr;
    }
    auto expr = std::make_unique<GetMethodsExpr>();
    expr->value = std::move(value);
    return expr;
}

// Parse SUPER call: SUPER.method(args)
std::unique_ptr<Expr> Parser::parse_super_call() {
    advance(); // consume SUPER
    if (!match(Tok::Dot)) {
        diag.err_at(peek().line, peek().col, "SUPER: expected '.'");
        return nullptr;
    }
    if (!check(Tok::Ident)) {
        diag.err_at(peek().line, peek().col, "SUPER: expected method name");
        return nullptr;
    }
    Token method = advance();
    std::vector<std::unique_ptr<Expr>> args;
    if (match(Tok::LParen)) {
        if (!check(Tok::RParen)) {
            do {
                args.push_back(expression());
            } while (match(Tok::Comma));
        }
        if (!match(Tok::RParen)) {
            diag.err_at(peek().line, peek().col, "SUPER: expected ')'");
            return nullptr;
        }
    }
    auto expr = std::make_unique<SuperCall>();
    expr->method = method.lex;
    expr->args = std::move(args);
    return expr;
}

// Parse TRY/CATCH/FINALLY block
std::unique_ptr<Stmt> Parser::parse_try_catch() {
    advance(); // consume TRY
    
    auto tryCatch = std::make_unique<TryCatchStmt>();
    
    // Parse try body
    tryCatch->tryBody = stmt_list_until(Tok::Catch, Tok::Finally);
    
    // Check for CATCH
    if (match(Tok::Catch)) {
        tryCatch->hasCatch = true;
        
        // Optional catch variable
        if (check(Tok::Ident)) {
            tryCatch->catchVar = advance().lex;
        }
        
        // Parse catch body
        tryCatch->catchBody = stmt_list_until(Tok::Finally, Tok::EndTry);
    }
    
    // Check for FINALLY
    if (match(Tok::Finally)) {
        tryCatch->hasFinally = true;
        tryCatch->finallyBody = stmt_list_until(Tok::EndTry, Tok::Eof);
    }
    
    // Consume END TRY
    if (!match(Tok::EndTry)) {
        diag.err_at(peek().line, peek().col, "TRY: expected END TRY");
        return nullptr;
    }
    skipNewlines(); // Align for next statement
    
    return tryCatch;
}

// Parse THROW statement
std::unique_ptr<Stmt> Parser::parse_throw() {
    advance(); // consume THROW
    
    auto throwStmt = std::make_unique<ThrowStmt>();
    throwStmt->error = expression();
    
    return throwStmt;
}

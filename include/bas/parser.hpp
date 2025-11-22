#pragma once
#include "ast.hpp"
#include "diag.hpp"
#include <vector>

namespace bas {
// Recursive-descent parser for BASIC producing an AST.
class Parser {
public:
  Parser(std::vector<Token> toks, Diag& d, bool agk_mode=false)
    : ts(std::move(toks)), diag(d), agk(agk_mode) {}
  Program parse();
private:
  std::vector<Token> ts; size_t i=0; Diag& diag; bool agk=false;
  // Queue of statements to emit before parsing the next statement (for desugaring)
  std::vector<std::unique_ptr<Stmt>> pending;
  int gensym{0};
  const Token& peek() const { return i<ts.size()? ts[i] : ts.back(); }
  const Token& advance(){ if(i<ts.size()) ++i; return ts[i-1]; }
  bool match(Tok k){ if(peek().kind==k){ advance(); return true;} return false; }
  bool match2(Tok a, Tok b){ if(peek().kind==a){ advance(); return match(b);} return false; }
  bool check(Tok k) const { return peek().kind==k; }
  void skipNewlines(){ while(check(Tok::Newline)) advance(); }

  std::unique_ptr<Stmt> statement();
  std::unique_ptr<Stmt> parse_print();
  std::unique_ptr<Stmt> parse_let();
  std::unique_ptr<Stmt> parse_if();
  std::unique_ptr<Stmt> parse_while();
  std::unique_ptr<Stmt> parse_for();
  std::unique_ptr<Stmt> parse_sub_decl();
  std::unique_ptr<Stmt> parse_function_decl();
  std::unique_ptr<Stmt> parse_return();
  std::unique_ptr<Stmt> parse_dim();
  std::unique_ptr<Stmt> parse_ident_statement();
  std::unique_ptr<Stmt> parse_call();
  std::unique_ptr<Stmt> parse_input();
  std::unique_ptr<Stmt> parse_type_decl();
  std::unique_ptr<Stmt> parse_module_decl();
  std::unique_ptr<Stmt> parse_for_each();
  std::unique_ptr<Stmt> parse_event_handler();
  std::unique_ptr<Stmt> parse_assert();
  std::unique_ptr<Stmt> parse_breakpoint();
  std::unique_ptr<Stmt> parse_debug_print();
  std::unique_ptr<Stmt> parse_yield();
  std::unique_ptr<Stmt> parse_operator_decl();
  std::unique_ptr<Stmt> parse_import();
  FunctionParam parse_function_param();
  std::vector<std::unique_ptr<Stmt>> stmt_list_until(Tok end1, Tok end2);
  std::unique_ptr<Expr> expression();
  std::unique_ptr<Expr> parse_lambda();
  std::unique_ptr<Expr> parse_interpolated_string();
  std::unique_ptr<Expr> parse_fstring();
  std::unique_ptr<Expr> parse_map_literal();
  std::unique_ptr<Expr> parse_typeof();
  std::unique_ptr<Expr> parse_get_properties();
  std::unique_ptr<Expr> parse_get_methods();
  std::unique_ptr<Expr> parse_super_call();
  std::unique_ptr<Expr> parse_range();
  std::unique_ptr<Expr> parse_comprehension();
  std::unique_ptr<Expr> parse_match();
  std::unique_ptr<Stmt> parse_using();
  std::unique_ptr<Stmt> parse_enum();
  std::unique_ptr<Stmt> parse_union();
  std::unique_ptr<Stmt> parse_try_catch();
  std::unique_ptr<Stmt> parse_throw();
  std::unique_ptr<Expr> or_();
  std::unique_ptr<Expr> xor_();
  std::unique_ptr<Expr> and_();
  std::unique_ptr<Expr> equality();
  std::unique_ptr<Expr> comparison();
  std::unique_ptr<Expr> term();
  std::unique_ptr<Expr> factor();
  std::unique_ptr<Expr> power();
  std::unique_ptr<Expr> unary();
  std::unique_ptr<Expr> primary();
  std::unique_ptr<Expr> parse_postfix(std::unique_ptr<Expr> base);
};
} // namespace bas

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
  const Token& peek() const { return i<ts.size()? ts[i] : ts.back(); }
  const Token& advance(){ if(i<ts.size()) ++i; return ts[i-1]; }
  bool match(Tok k){ if(peek().kind==k){ advance(); return true;} return false; }
  bool match2(Tok a, Tok b){ if(peek().kind==a){ advance(); return match(b);} return false; }
  bool check(Tok k) const { return peek().kind==k; }
  void skipNewlines(){ while(check(Tok::Newline)) advance(); }

  std::unique_ptr<Stmt> statement();
  std::vector<std::unique_ptr<Stmt>> stmt_list_until(Tok end1, Tok end2);
  std::unique_ptr<Expr> expression();
  std::unique_ptr<Expr> or_();
  std::unique_ptr<Expr> and_();
  std::unique_ptr<Expr> equality();
  std::unique_ptr<Expr> comparison();
  std::unique_ptr<Expr> term();
  std::unique_ptr<Expr> factor();
  std::unique_ptr<Expr> unary();
  std::unique_ptr<Expr> primary();
};
} // namespace bas

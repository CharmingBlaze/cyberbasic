#pragma once
#include "token.hpp"
#include <string>
#include <vector>

namespace bas {
// Lexical analyzer for BASIC source text.
class Lexer {
public:
  explicit Lexer(std::string src);
  std::vector<Token> lex();
private:
  std::string s; size_t i=0; int line=1, col=1;
  bool atEnd() const { return i>=s.size(); }
  char peek() const { return atEnd()? '\0' : s[i]; }
  char advance();
  Token lex_identifier();
  Token lex_number();
  Token lex_string();
};
} // namespace bas

#pragma once
#include <string>

namespace bas {

// Token kinds for the BASIC lexer/parser.
enum class Tok {
  Eof, Newline, Ident, Number, String,
  Let, Print, If, Then, EndIf, While, Wend, Not, And, Or, True, False, Call,
  Sub, EndSub, Function, EndFunction, Return,
  For, To, Step, Next, Dim, Gosub, Redim, Preserve,
  Else, ElseIf,
  Open, Close, Read, Write, Input,
  LParen, RParen, Comma, Assign, Plus, Minus, Star, Slash, Mod,
  Eq, Neq, Lt, Lte, Gt, Gte, LBracket, RBracket,
  Select, Case, EndSelect, Is, Break, Continue, Do, Loop, Repeat, Until
};

// Token with lexeme and position.
struct Token {
  Tok kind{};
  std::string lex;
  int line{1};
  int col{1};
};

} // namespace bas

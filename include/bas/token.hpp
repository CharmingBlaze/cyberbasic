#pragma once
#include <string>

namespace bas {

// Token kinds for the BASIC lexer/parser.
enum class Tok {
    Eof, Newline, Error, Ident, Number, String,
  Let, Print, If, Then, EndIf, While, Wend, Not, And, Or, True, False, Call,
  Sub, EndSub, Function, EndFunction, Return,
  For, To, Step, Next, Dim, Gosub, Redim, Preserve,
  Else, ElseIf,
  Open, Close, Read, Write, Input,
  LParen, RParen, Comma, Assign, Plus, Minus, Star, Slash, Mod,
  Eq, Neq, Lt, Lte, Gt, Gte, LBracket, RBracket, Dot,
  Select, Case, EndSelect, Is, Break, Continue, Do, Loop, Repeat, Until,
  Var, Const,
  Option, Explicit,
  Local, Global,
  Import, Include, In,
  Type, EndType, As,
  Extends, Super,
  Each, Yield,
  Operator, EndOperator,
  Module, EndModule, Public, Private,
  Lambda, EndLambda,
  Assert, Breakpoint, Debug,
  Exit, Xor,
  Colon, Power, IntDiv,
  FString, Using, EndUsing,
  Match, EndMatch,
  Enum, EndEnum,
  Union, EndUnion,
  Spread, NullSafe, NullCoalesce,
  TypeInfer, Tuple, Destructure,
  // State system keywords
  State, EndState, Transition, On, Enter, Exit, Update,
  Parallel, EndParallel, Group, EndGroup, Event,
  Define, System, EndSystem, Attach, To, From,
  Override, Priority, Enable, Disable, Debug
};

// Token with lexeme and position.
struct Token {
  Tok kind{};
  std::string lex;
  int line{1};
  int col{1};
};

} // namespace bas

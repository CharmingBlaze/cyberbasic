#include "bas/lexer.hpp"
#include <cctype>
#include <unordered_map>

using namespace bas;

static Tok kw(const std::string& w){
  static const std::unordered_map<std::string, Tok> k{
    {"LET",Tok::Let},{"PRINT",Tok::Print},{"IF",Tok::If},{"THEN",Tok::Then},{"ENDIF",Tok::EndIf},
    {"ELSE",Tok::Else},{"ELSEIF",Tok::ElseIf},
    {"WHILE",Tok::While},{"WEND",Tok::Wend},{"NOT",Tok::Not},{"AND",Tok::And},{"OR",Tok::Or},
    {"TRUE",Tok::True},{"FALSE",Tok::False},
    {"CALL",Tok::Call},
    {"FOR",Tok::For},{"TO",Tok::To},{"STEP",Tok::Step},{"NEXT",Tok::Next},
    {"SUB",Tok::Sub},{"ENDSUB",Tok::EndSub},{"RETURN",Tok::Return},
    {"FUNCTION",Tok::Function},{"ENDFUNCTION",Tok::EndFunction},
    {"DIM",Tok::Dim},{"REDIM",Tok::Redim},{"PRESERVE",Tok::Preserve},
    {"OPEN",Tok::Open},{"CLOSE",Tok::Close},{"READ",Tok::Read},{"WRITE",Tok::Write},
    {"INPUT",Tok::Input},
    {"SELECT",Tok::Select},{"CASE",Tok::Case},{"ENDSELECT",Tok::EndSelect},{"IS",Tok::Is},
    {"MOD",Tok::Mod},{"BREAK",Tok::Break},{"CONTINUE",Tok::Continue},{"DO",Tok::Do},{"LOOP",Tok::Loop},{"REPEAT",Tok::Repeat},{"UNTIL",Tok::Until},
    {"VAR",Tok::Var},{"CONST",Tok::Const},
    {"OPTION",Tok::Option},{"EXPLICIT",Tok::Explicit},
    {"LOCAL",Tok::Local},{"GLOBAL",Tok::Global}
  };
  auto it=k.find(w); return it==k.end()?Tok::Ident:it->second;
}

Lexer::Lexer(std::string src): s(std::move(src)){}

char Lexer::advance(){
  char c=s[i++];
  if(c=='\n'){line++;col=1;}
  else col++;
  return c;
}

std::vector<Token> Lexer::lex(){
  std::vector<Token> out;
  while(!atEnd()){
    char c=peek();
    if(c==' '||c=='\t'||c=='\r'){ advance(); continue; }
    if(c=='\n'){ advance(); out.push_back({Tok::Newline,"",line,col}); continue; }
    if(c=='#'){ while(!atEnd() && peek()!='\n') advance(); continue; }
    if(std::isalpha((unsigned char)c) || c=='_'){
      std::string id;
      while(std::isalnum((unsigned char)peek())||peek()=='_') id.push_back(advance());
      for(char& ch:id) ch=std::toupper((unsigned char)ch);
      // REM comments: skip until end of line
      if(id == "REM"){
        while(!atEnd() && peek()!='\n') advance();
        continue;
      }
      // Support END SELECT -> EndSelect token
      if(id == "END"){
        // Save position
        size_t si = i; int sline = line; int scol = col;
        // Skip spaces/tabs
        while(!atEnd() && (peek()==' ' || peek()=='\t' || peek()=='\r')) advance();
        // Read next identifier
        std::string id2;
        while(std::isalnum((unsigned char)peek())||peek()=='_') id2.push_back(advance());
        for(char& ch:id2) ch=(char)std::toupper((unsigned char)ch);
        if(id2 == "SELECT"){
          out.push_back({Tok::EndSelect, "END SELECT", sline, scol});
          continue;
        } else {
          // revert if not SELECT
          i = si; line = sline; col = scol;
        }
      }
      Tok k=kw(id);
      out.push_back({k,id,line,col});
      continue;
    }
    if(std::isdigit((unsigned char)c)){
      std::string num; bool dot=false;
      while(std::isdigit((unsigned char)peek()) || (!dot && peek()=='.')){ if(peek()=='.') dot=true; num.push_back(advance()); }
      out.push_back({Tok::Number,num,line,col});
      continue;
    }
    if(c=='"'){
      advance();
      std::string str;
      while(!atEnd() && peek()!='"'){ str.push_back(advance()); }
      if(peek()=='"') advance();
      out.push_back({Tok::String,str,line,col});
      continue;
    }
    switch(c){
      case '(': advance(); out.push_back({Tok::LParen,"(",line,col}); break;
      case ')': advance(); out.push_back({Tok::RParen,")",line,col}); break;
      case '[': advance(); out.push_back({Tok::LBracket,"[",line,col}); break;
      case ']': advance(); out.push_back({Tok::RBracket,"]",line,col}); break;
      
      case ',': advance(); out.push_back({Tok::Comma,",",line,col}); break;
      case '+': advance(); out.push_back({Tok::Plus,"+",line,col}); break;
      case '-': advance(); out.push_back({Tok::Minus,"-",line,col}); break;
      case '*': advance(); out.push_back({Tok::Star,"*",line,col}); break;
      case '/': advance(); out.push_back({Tok::Slash,"/",line,col}); break;
      case '=': advance(); out.push_back({Tok::Eq,"=",line,col}); break;
      case ':': advance(); out.push_back({Tok::Newline,"",line,col}); break;
      case '\'':
        // Single-quote comment to end of line
        advance(); while(!atEnd() && peek()!='\n') advance();
        break;
      case '<':
        advance();
        if(peek()=='>'){ advance(); out.push_back({Tok::Neq,"<>",line,col}); }
        else if(peek()=='='){ advance(); out.push_back({Tok::Lte,"<=",line,col}); }
        else out.push_back({Tok::Lt,"<",line,col});
        break;
      case '>':
        advance();
        if(peek()=='='){ advance(); out.push_back({Tok::Gte,">=",line,col}); }
        else out.push_back({Tok::Gt,">",line,col});
        break;
      default:
        advance(); // skip unknown
    }
  }
  out.push_back({Tok::Eof,"",line,col});
  return out;
}

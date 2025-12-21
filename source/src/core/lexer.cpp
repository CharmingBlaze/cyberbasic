#include "bas/lexer.hpp"
#include <cctype>
#include <unordered_map>

using namespace bas;

// Keyword map for converting identifiers to tokens (all lowercase for case-insensitive matching)
static Tok kw(const std::string& w) {
    static const std::unordered_map<std::string, Tok> k{
        {"let", Tok::Let}, {"print", Tok::Print}, {"printc", Tok::PrintC}, {"if", Tok::If}, {"ifn", Tok::IfNot}, {"then", Tok::Then}, {"endif", Tok::EndIf},
        {"else", Tok::Else}, {"elseif", Tok::ElseIf},
        {"while", Tok::While}, {"wend", Tok::Wend}, {"not", Tok::Not}, {"and", Tok::And}, {"or", Tok::Or},
        {"true", Tok::True}, {"false", Tok::False},
        {"call", Tok::Call},
        {"for", Tok::For}, {"to", Tok::To}, {"step", Tok::Step}, {"next", Tok::Next},
        {"sub", Tok::Sub}, {"endsub", Tok::EndSub}, {"return", Tok::Return}, {"gosub", Tok::Gosub}, {"goto", Tok::Goto},
        {"function", Tok::Function}, {"endfunction", Tok::EndFunction},
        {"dim", Tok::Dim}, {"redim", Tok::Redim}, {"preserve", Tok::Preserve},
        {"open", Tok::Open}, {"close", Tok::Close}, {"read", Tok::Read}, {"write", Tok::Write},
        {"input", Tok::Input},
        {"select", Tok::Select}, {"case", Tok::Case}, {"endselect", Tok::EndSelect}, {"is", Tok::Is}, {"default", Tok::Default},
        {"mod", Tok::Mod}, {"break", Tok::Break}, {"continue", Tok::Continue}, {"do", Tok::Do}, {"loop", Tok::Loop}, {"repeat", Tok::Repeat}, {"until", Tok::Until},
        {"var", Tok::Var}, {"const", Tok::Const},
        {"option", Tok::Option}, {"explicit", Tok::Explicit},
        {"local", Tok::Local}, {"global", Tok::Global},
        {"import", Tok::Import}, {"include", Tok::Include}, {"in", Tok::In},
        {"type", Tok::Type}, {"endtype", Tok::EndType}, {"as", Tok::As},
        {"extends", Tok::Extends}, {"super", Tok::Super},
        {"each", Tok::Each}, {"yield", Tok::Yield},
        {"operator", Tok::Operator}, {"endoperator", Tok::EndOperator},
        {"module", Tok::Module}, {"endmodule", Tok::EndModule},
        {"public", Tok::Public}, {"private", Tok::Private},
        {"lambda", Tok::Lambda}, {"endlambda", Tok::EndLambda},
        {"assert", Tok::Assert}, {"breakpoint", Tok::Breakpoint}, {"debug", Tok::Debug},
        {"exit", Tok::Exit}, {"xor", Tok::Xor},
        {"using", Tok::Using}, {"endusing", Tok::EndUsing},
        {"match", Tok::Match}, {"endmatch", Tok::EndMatch},
        {"enum", Tok::Enum}, {"endenum", Tok::EndEnum},
        {"union", Tok::Union}, {"endunion", Tok::EndUnion},
        // State system keywords
        {"state", Tok::State}, {"endstate", Tok::EndState},
        {"transition", Tok::Transition},
        {"on", Tok::On}, {"enter", Tok::Enter}, {"exit", Tok::Exit}, {"update", Tok::Update},
        {"parallel", Tok::Parallel}, {"endparallel", Tok::EndParallel},
        {"group", Tok::Group}, {"endgroup", Tok::EndGroup},
        {"event", Tok::Event},
        {"define", Tok::Define}, {"system", Tok::System}, {"endsystem", Tok::EndSystem},
        {"attach", Tok::Attach}, {"from", Tok::From},
        {"override", Tok::Override}, {"priority", Tok::Priority},
        {"enable", Tok::Enable}, {"disable", Tok::Disable},
        {"nil", Tok::Nil}, {"none", Tok::None}, {"null", Tok::Null}, {"void", Tok::Void},
        {"end", Tok::End}, {"await", Tok::Await}, {"coroutine", Tok::Coroutine}
    };
    auto it = k.find(w);
    return it == k.end() ? Tok::Ident : it->second;
}

Lexer::Lexer(std::string src) : s(std::move(src)) {}

char Lexer::advance() {
    if (atEnd()) return '\0';
    char c = s[i++];
    if (c == '\n') {
        line++;
        col = 1;
    } else {
        col++;
    }
    return c;
}

Token Lexer::lex_identifier() {
    std::string id;
    int start_col = col;
    while (std::isalnum((unsigned char)peek()) || peek() == '_' || peek() == '$') {
        id.push_back(advance());
    }
    for (char& ch : id) {
        ch = std::tolower((unsigned char)ch);
    }

    // REM is still supported for backward compatibility, but // is preferred
    if (id == "rem") {
        while (!atEnd() && peek() != '\n') {
            advance();
        }
        return {Tok::Newline, "", line, start_col}; // Treat REM as a newline
    }

    Tok k = kw(id);
    return {k, id, line, start_col};
}

Token Lexer::lex_number() {
    std::string num;
    int start_col = col;
    bool dot = false;
    while (std::isdigit((unsigned char)peek()) || (!dot && peek() == '.')) {
        if (peek() == '.') {
            dot = true;
        }
        num.push_back(advance());
    }
    return {Tok::Number, num, line, start_col};
}

Token Lexer::lex_string() {
    std::string str;
    int start_col = col;
    advance(); // Consume opening quote
    while (!atEnd() && peek() != '"') {
        // Handle escaped quotes ""
        if (peek() == '"' && i + 1 < s.size() && s[i+1] == '"') {
            str.push_back(advance()); // Add one quote
            advance(); // Skip the second quote
        } else {
            str.push_back(advance());
        }
    }
    if (peek() == '"') {
        advance(); // Consume closing quote
    }
    return {Tok::String, str, line, start_col};
}

Token Lexer::lex_fstring() {
    Token t = lex_string();
    t.kind = Tok::FString;
    return t;
}

std::vector<Token> Lexer::lex() {
    std::vector<Token> out;
    while (!atEnd()) {
        char c = peek();
        int start_col = col;

        if (c == ' ' || c == '\t' || c == '\r') {
            advance();
            continue;
        }
        if (c == '\n') {
            advance();
            out.push_back({Tok::Newline, "", line, start_col});
            continue;
        }
        // C++ style single-line comment: //
        if (c == '/' && i + 1 < s.size() && s[i + 1] == '/') {
            advance(); // consume first '/'
            advance(); // consume second '/'
            while (!atEnd() && peek() != '\n') advance();
            continue;
        }
        
        // C++ style multi-line comment: /* */
        if (c == '/' && i + 1 < s.size() && s[i + 1] == '*') {
            advance(); // consume '/'
            advance(); // consume '*'
            while (!atEnd()) {
                if (peek() == '*' && i + 1 < s.size() && s[i + 1] == '/') {
                    advance(); // consume '*'
                    advance(); // consume '/'
                    break;
                }
                advance();
            }
            continue;
        }
        
        // Legacy comment support: # and '
        if (c == '#') {
            while (!atEnd() && peek() != '\n') advance();
            continue;
        }
        if (std::isalpha((unsigned char)c) || c == '_' || c == '$') {
            out.push_back(lex_identifier());
            continue;
        }
        if (std::isdigit((unsigned char)c)) {
            out.push_back(lex_number());
            continue;
        }
        // Check for f-string prefix: f" or F" (before the quote)
        if ((c == 'f' || c == 'F') && i + 1 < s.size() && s[i + 1] == '"') {
            advance(); // consume 'f' or 'F'
            out.push_back(lex_fstring());
            continue;
        }
        if (c == '"') {
            out.push_back(lex_string());
            continue;
        }

        advance(); // Consume the character for single-char tokens
        switch (c) {
            case '(': out.push_back({Tok::LParen, "(", line, start_col}); break;
            case ')': out.push_back({Tok::RParen, ")", line, start_col}); break;
            case '[': out.push_back({Tok::LBracket, "[", line, start_col}); break;
            case ']': out.push_back({Tok::RBracket, "]", line, start_col}); break;
            case ',': out.push_back({Tok::Comma, ",", line, start_col}); break;
            case '+': out.push_back({Tok::Plus, "+", line, start_col}); break;
            case '-': out.push_back({Tok::Minus, "-", line, start_col}); break;
            case '*': out.push_back({Tok::Star, "*", line, start_col}); break;
            case '/': out.push_back({Tok::Slash, "/", line, start_col}); break;
            case '\\': out.push_back({Tok::IntDiv, "\\", line, start_col}); break;
            case '^': out.push_back({Tok::Power, "^", line, start_col}); break;
            case '&': {
                if (peek() == '&') {
                    advance();
                    out.push_back({Tok::And, "&&", line, start_col});
                } else {
                    out.push_back({Tok::BitAnd, "&", line, start_col});
                }
                break;
            }
            case '|': {
                if (peek() == '|') {
                    advance();
                    out.push_back({Tok::Or, "||", line, start_col});
                } else {
                    out.push_back({Tok::BitOr, "|", line, start_col});
                }
                break;
            }
            case '~': out.push_back({Tok::BitNot, "~", line, start_col}); break;
            case '=': {
                if (peek() == '=') {
                    advance();
                    out.push_back({Tok::TypeInfer, ":=", line, start_col});
                } else {
                    out.push_back({Tok::Eq, "=", line, start_col});
                }
                break;
            }
            case '.': {
                if (peek() == '.') {
                    advance();
                    out.push_back({Tok::Spread, "..", line, start_col});
                } else {
                    out.push_back({Tok::Dot, ".", line, start_col});
                }
                break;
            }
            case '?': {
                if (peek() == '.') {
                    advance();
                    out.push_back({Tok::NullSafe, "?.", line, start_col});
                } else if (peek() == '[') {
                    advance();
                    out.push_back({Tok::NullSafe, "?[", line, start_col});
                } else if (peek() == '?') {
                    advance();
                    out.push_back({Tok::NullCoalesce, "??", line, start_col});
                } else {
                    out.push_back({Tok::Question, "?", line, start_col});
                }
                break;
            }
            case ':': {
                if (peek() == '=') {
                    advance();
                    out.push_back({Tok::TypeInfer, ":=", line, start_col});
                } else {
                    out.push_back({Tok::Colon, ":", line, start_col});
                }
                break;
            }
            case '\'': // Single-quote comment
                while (!atEnd() && peek() != '\n') advance();
                break;
            case '<':
                if (peek() == '<') {
                    advance();
                    out.push_back({Tok::ShiftLeft, "<<", line, start_col});
                } else if (peek() == '>') { 
                    advance(); 
                    out.push_back({Tok::Neq, "<>", line, start_col}); 
                } else if (peek() == '=') { 
                    advance(); 
                    out.push_back({Tok::Lte, "<=", line, start_col}); 
                } else { 
                    out.push_back({Tok::Lt, "<", line, start_col}); 
                }
                break;
            case '>':
                if (peek() == '>') {
                    advance();
                    out.push_back({Tok::ShiftRight, ">>", line, start_col});
                } else if (peek() == '=') { 
                    advance(); 
                    out.push_back({Tok::Gte, ">=", line, start_col}); 
                } else { 
                    out.push_back({Tok::Gt, ">", line, start_col}); 
                }
                break;
            default: {
                std::string unknown(1, c);
                out.push_back({Tok::Error, "Unexpected character: " + unknown, line, start_col});
                break;
            }
        }
    }
    out.push_back({Tok::Eof, "", line, col});
    return out;
}

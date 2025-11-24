#include "bas/lexer.hpp"
#include <cctype>
#include <unordered_map>

using namespace bas;

// Keyword map for converting identifiers to tokens
static Tok kw(const std::string& w) {
    static const std::unordered_map<std::string, Tok> k{
        {"LET", Tok::Let}, {"PRINT", Tok::Print}, {"PRINTC", Tok::PrintC}, {"IF", Tok::If}, {"THEN", Tok::Then}, {"ENDIF", Tok::EndIf},
        {"ELSE", Tok::Else}, {"ELSEIF", Tok::ElseIf},
        {"WHILE", Tok::While}, {"WEND", Tok::Wend}, {"NOT", Tok::Not}, {"AND", Tok::And}, {"OR", Tok::Or},
        {"TRUE", Tok::True}, {"FALSE", Tok::False},
        {"CALL", Tok::Call},
        {"FOR", Tok::For}, {"TO", Tok::To}, {"STEP", Tok::Step}, {"NEXT", Tok::Next},
        {"SUB", Tok::Sub}, {"ENDSUB", Tok::EndSub}, {"RETURN", Tok::Return}, {"GOSUB", Tok::Gosub}, {"GOTO", Tok::Goto},
        {"FUNCTION", Tok::Function}, {"ENDFUNCTION", Tok::EndFunction},
        {"DIM", Tok::Dim}, {"REDIM", Tok::Redim}, {"PRESERVE", Tok::Preserve},
        {"OPEN", Tok::Open}, {"CLOSE", Tok::Close}, {"READ", Tok::Read}, {"WRITE", Tok::Write},
        {"INPUT", Tok::Input},
        {"SELECT", Tok::Select}, {"CASE", Tok::Case}, {"ENDSELECT", Tok::EndSelect}, {"IS", Tok::Is},
        {"MOD", Tok::Mod}, {"BREAK", Tok::Break}, {"CONTINUE", Tok::Continue}, {"DO", Tok::Do}, {"LOOP", Tok::Loop}, {"REPEAT", Tok::Repeat}, {"UNTIL", Tok::Until},
        {"VAR", Tok::Var}, {"CONST", Tok::Const},
        {"OPTION", Tok::Option}, {"EXPLICIT", Tok::Explicit},
        {"LOCAL", Tok::Local}, {"GLOBAL", Tok::Global},
        {"IMPORT", Tok::Import}, {"INCLUDE", Tok::Include}, {"IN", Tok::In},
        {"TYPE", Tok::Type}, {"ENDTYPE", Tok::EndType}, {"AS", Tok::As},
        {"EXTENDS", Tok::Extends}, {"SUPER", Tok::Super},
        {"EACH", Tok::Each}, {"YIELD", Tok::Yield},
        {"OPERATOR", Tok::Operator}, {"ENDOPERATOR", Tok::EndOperator},
        {"MODULE", Tok::Module}, {"ENDMODULE", Tok::EndModule},
        {"PUBLIC", Tok::Public}, {"PRIVATE", Tok::Private},
        {"LAMBDA", Tok::Lambda}, {"ENDLAMBDA", Tok::EndLambda},
        {"ASSERT", Tok::Assert}, {"BREAKPOINT", Tok::Breakpoint}, {"DEBUG", Tok::Debug},
        {"EXIT", Tok::Exit}, {"XOR", Tok::Xor},
        {"USING", Tok::Using}, {"ENDUSING", Tok::EndUsing},
        {"MATCH", Tok::Match}, {"ENDMATCH", Tok::EndMatch},
        {"ENUM", Tok::Enum}, {"ENDENUM", Tok::EndEnum},
        {"UNION", Tok::Union}, {"ENDUNION", Tok::EndUnion},
        // State system keywords
        {"STATE", Tok::State}, {"ENDSTATE", Tok::EndState},
        {"TRANSITION", Tok::Transition},
        {"ON", Tok::On}, {"ENTER", Tok::Enter}, {"EXIT", Tok::Exit}, {"UPDATE", Tok::Update},
        {"PARALLEL", Tok::Parallel}, {"ENDPARALLEL", Tok::EndParallel},
        {"GROUP", Tok::Group}, {"ENDGROUP", Tok::EndGroup},
        {"EVENT", Tok::Event},
        {"DEFINE", Tok::Define}, {"SYSTEM", Tok::System}, {"ENDSYSTEM", Tok::EndSystem},
        {"ATTACH", Tok::Attach}, {"TO", Tok::To}, {"FROM", Tok::From},
        {"OVERRIDE", Tok::Override}, {"PRIORITY", Tok::Priority},
        {"ENABLE", Tok::Enable}, {"DISABLE", Tok::Disable},
        {"TRY", Tok::Try}, {"CATCH", Tok::Catch}, {"FINALLY", Tok::Finally},
        {"ENDTRY", Tok::EndTry}, {"THROW", Tok::Throw},
        {"NIL", Tok::Nil}, {"NONE", Tok::None}, {"NULL", Tok::Null}, {"VOID", Tok::Void},
        {"END", Tok::End}, {"AWAIT", Tok::Await}, {"COROUTINE", Tok::Coroutine}
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
        ch = std::toupper((unsigned char)ch);
    }

    // REM is still supported for backward compatibility, but // is preferred
    if (id == "REM") {
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
                    std::string unknown(1, c);
                    out.push_back({Tok::Error, "Unexpected character: " + unknown, line, start_col});
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
                if (peek() == '>') { advance(); out.push_back({Tok::Neq, "<>", line, start_col}); }
                else if (peek() == '=') { advance(); out.push_back({Tok::Lte, "<=", line, start_col}); }
                else { out.push_back({Tok::Lt, "<", line, start_col}); }
                break;
            case '>':
                if (peek() == '=') { advance(); out.push_back({Tok::Gte, ">=", line, start_col}); }
                else { out.push_back({Tok::Gt, ">", line, start_col}); }
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

#include "bas/lexer.hpp"
#include <cctype>
#include <unordered_map>

using namespace bas;

// Keyword map for converting identifiers to tokens
static Tok kw(const std::string& w) {
    static const std::unordered_map<std::string, Tok> k{
        {"LET", Tok::Let}, {"PRINT", Tok::Print}, {"IF", Tok::If}, {"THEN", Tok::Then}, {"ENDIF", Tok::EndIf},
        {"ELSE", Tok::Else}, {"ELSEIF", Tok::ElseIf},
        {"WHILE", Tok::While}, {"WEND", Tok::Wend}, {"NOT", Tok::Not}, {"AND", Tok::And}, {"OR", Tok::Or},
        {"TRUE", Tok::True}, {"FALSE", Tok::False},
        {"CALL", Tok::Call},
        {"FOR", Tok::For}, {"TO", Tok::To}, {"STEP", Tok::Step}, {"NEXT", Tok::Next},
        {"SUB", Tok::Sub}, {"ENDSUB", Tok::EndSub}, {"RETURN", Tok::Return},
        {"FUNCTION", Tok::Function}, {"ENDFUNCTION", Tok::EndFunction},
        {"DIM", Tok::Dim}, {"REDIM", Tok::Redim}, {"PRESERVE", Tok::Preserve},
        {"OPEN", Tok::Open}, {"CLOSE", Tok::Close}, {"READ", Tok::Read}, {"WRITE", Tok::Write},
        {"INPUT", Tok::Input},
        {"SELECT", Tok::Select}, {"CASE", Tok::Case}, {"ENDSELECT", Tok::EndSelect}, {"IS", Tok::Is},
        {"MOD", Tok::Mod}, {"BREAK", Tok::Break}, {"CONTINUE", Tok::Continue}, {"DO", Tok::Do}, {"LOOP", Tok::Loop}, {"REPEAT", Tok::Repeat}, {"UNTIL", Tok::Until},
        {"VAR", Tok::Var}, {"CONST", Tok::Const},
        {"OPTION", Tok::Option}, {"EXPLICIT", Tok::Explicit},
        {"LOCAL", Tok::Local}, {"GLOBAL", Tok::Global},
        {"IMPORT", Tok::Import}, {"IN", Tok::In}
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
    while (std::isalnum((unsigned char)peek()) || peek() == '_') {
        id.push_back(advance());
    }
    for (char& ch : id) {
        ch = std::toupper((unsigned char)ch);
    }

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
        if (c == '#') {
            while (!atEnd() && peek() != '\n') advance();
            continue;
        }
        if (std::isalpha((unsigned char)c) || c == '_') {
            out.push_back(lex_identifier());
            continue;
        }
        if (std::isdigit((unsigned char)c)) {
            out.push_back(lex_number());
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
            case '=': out.push_back({Tok::Eq, "=", line, start_col}); break;
            case ':': out.push_back({Tok::Newline, "", line, start_col}); break;
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

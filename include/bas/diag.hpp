#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm> // Required for std::any_of

namespace bas {

enum class ErrorLevel {
    INFO,
    WARNING,
    ERROR,
    FATAL
};

struct Diagnostic {
    ErrorLevel level;
    int line;
    int column;
    std::string message;
    std::string suggestion;
    std::string context;
    
    Diagnostic(ErrorLevel l, int ln, int col, const std::string& msg, 
               const std::string& sugg = "", const std::string& ctx = "")
        : level(l), line(ln), column(col), message(msg), suggestion(sugg), context(ctx) {}
};

// Enhanced diagnostics collector with better error reporting
struct Diag {
    std::vector<Diagnostic> diagnostics;
    bool debug_mode = false;
    
    void info(int line, int col, const std::string& msg, const std::string& ctx = "") {
        diagnostics.emplace_back(ErrorLevel::INFO, line, col, msg, "", ctx);
    }
    
    void warning(int line, int col, const std::string& msg, const std::string& sugg = "", const std::string& ctx = "") {
        diagnostics.emplace_back(ErrorLevel::WARNING, line, col, msg, sugg, ctx);
    }
    
    void err(int line, int col, const std::string& msg, const std::string& sugg = "", const std::string& ctx = "") {
        diagnostics.emplace_back(ErrorLevel::ERROR, line, col, msg, sugg, ctx);
    }
    
    void fatal(int line, int col, const std::string& msg, const std::string& sugg = "", const std::string& ctx = "") {
        diagnostics.emplace_back(ErrorLevel::FATAL, line, col, msg, sugg, ctx);
    }
    
    // Legacy compatibility
    void err(const std::string& m) { err(0, 0, m); }
    void err_at(int line, int col, const std::string& m,
                const std::string& sugg = "", const std::string& ctx = "") {
        err(line, col, m, sugg, ctx);
    }
    
    bool has_errors() const { 
        return std::any_of(diagnostics.begin(), diagnostics.end(), 
                          [](const Diagnostic& d) { return d.level >= ErrorLevel::ERROR; });
    }
    
    bool has_fatal() const {
        return std::any_of(diagnostics.begin(), diagnostics.end(),
                          [](const Diagnostic& d) { return d.level == ErrorLevel::FATAL; });
    }
    
    void print_all(std::ostream& out = std::cerr) const {
        for (const auto& diag : diagnostics) {
            print_diagnostic(diag, out);
        }
    }
    
    void print_errors(std::ostream& out = std::cerr) const {
        for (const auto& diag : diagnostics) {
            if (diag.level >= ErrorLevel::ERROR) {
                print_diagnostic(diag, out);
            }
        }
    }
    
    void print_warnings(std::ostream& out = std::cerr) const {
        for (const auto& diag : diagnostics) {
            if (diag.level == ErrorLevel::WARNING) {
                print_diagnostic(diag, out);
            }
        }
    }
    
    void clear() { diagnostics.clear(); }
    
    void set_debug_mode(bool enabled) { debug_mode = enabled; }
    
private:
    void print_diagnostic(const Diagnostic& diag, std::ostream& out) const {
        // Color coding for different error levels
        const char* color_code = "";
        const char* level_str = "";
        
        switch (diag.level) {
            case ErrorLevel::INFO:
                color_code = "\033[36m"; // Cyan
                level_str = "INFO";
                break;
            case ErrorLevel::WARNING:
                color_code = "\033[33m"; // Yellow
                level_str = "WARNING";
                break;
            case ErrorLevel::ERROR:
                color_code = "\033[31m"; // Red
                level_str = "ERROR";
                break;
            case ErrorLevel::FATAL:
                color_code = "\033[35m"; // Magenta
                level_str = "FATAL";
                break;
        }
        
        // Print error header
        if (diag.line > 0 && diag.column > 0) {
            out << color_code << level_str << " at line " << diag.line << ", column " << diag.column << ":\033[0m" << std::endl;
        } else {
            out << color_code << level_str << ":\033[0m" << std::endl;
        }
        
        // Print context if available
        if (!diag.context.empty()) {
            out << "  Context: " << diag.context << std::endl;
        }
        
        // Print main message
        out << "  " << diag.message << std::endl;
        
        // Print suggestion if available
        if (!diag.suggestion.empty()) {
            out << "  Suggestion: " << diag.suggestion << std::endl;
        }
        
        // Print debug info if enabled
        if (debug_mode && diag.line > 0) {
            out << "  Debug: Check around line " << diag.line << std::endl;
        }
        
        out << std::endl;
    }
};

} // namespace bas

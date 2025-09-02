#include "bas/lexer.hpp"
#include "bas/parser.hpp"
#include "bas/runtime.hpp"
#include "bas/builtins.hpp"
#include "bas/navigation.hpp"
#include "bas/physics.hpp"
#include "bas/ai.hpp"
#include "bas/graphics.hpp"
#include "bas/networking.hpp"
#include "bas/audio.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <cstring>
#include <algorithm>

void print_usage(const char* program_name) {
    std::cout << "BASIC + Raylib Interpreter v1.0" << std::endl;
    std::cout << "Usage: " << program_name << " [options] <file.bas>" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --debug, -d     Enable debug mode with detailed error information" << std::endl;
    std::cout << "  --verbose, -v   Show warnings and additional information" << std::endl;
    std::cout << "  --dialect <name>  Select dialect: classic | agklite" << std::endl;
    std::cout << "                   Default is permissive (agklite features ON)." << std::endl;
    std::cout << "                   Use --dialect classic to enforce strict classic." << std::endl;
    std::cout << "  --agk             [deprecated] Alias for --dialect agklite" << std::endl;
    std::cout << "  --help, -h      Show this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << program_name << " game.bas" << std::endl;
    std::cout << "  " << program_name << " --debug test.bas" << std::endl;
    std::cout << "  " << program_name << " --verbose demo.bas" << std::endl;
}

int main(int argc, char** argv) {
    bool debug_mode = false;
    bool verbose_mode = false;
    // Permissive by default: single-line IF and AGK-like loops enabled
    bool agk_mode = true;
    std::string dialect;
    std::string filename;
    bool suppressDialectWarn = false;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-d") == 0) {
            debug_mode = true;
        } else if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
            verbose_mode = true;
        } else if (strncmp(argv[i], "--dialect=", 10) == 0) {
            dialect = std::string(argv[i] + 10);
        } else if (strcmp(argv[i], "--dialect") == 0 && i + 1 < argc) {
            dialect = std::string(argv[++i]);
        } else if (strcmp(argv[i], "--agk") == 0) { // deprecated alias
            dialect = "agklite";
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (argv[i][0] != '-') {
            filename = argv[i];
        } else {
            std::cerr << "Unknown option: " << argv[i] << std::endl;
            print_usage(argv[0]);
            return 64;
        }
    }
    
    if (filename.empty()) {
        std::cerr << "Error: No input file specified" << std::endl;
        print_usage(argv[0]);
        return 64;
    }
    
    if (verbose_mode) {
        std::cout << "BASIC + Raylib Interpreter v1.0" << std::endl;
        if (debug_mode) std::cout << "Debug mode: ENABLED" << std::endl;
        std::cout << "Loading: " << filename << std::endl;
        std::cout << std::endl;
    }
    
    // Open and read the source file
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "Error: Cannot open file '" << filename << "'" << std::endl;
        std::cerr << "  Check if the file exists and you have read permissions" << std::endl;
        return 66;
    }
    
    std::string src((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    if (src.empty()) {
        std::cerr << "Error: File '" << filename << "' is empty" << std::endl;
        return 65;
    }
    
    if (debug_mode) {
        std::cout << "Source file loaded: " << src.length() << " characters" << std::endl;
    }
    
    // Check pragma in source to select dialect (case-insensitive)
    {
        std::string upper = src;
        std::transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char c){ return (char)std::toupper(c); });
        // Warning suppression
        if (upper.find("#PRAGMA NOWARN DIALECT") != std::string::npos) {
            suppressDialectWarn = true;
        }
        // New preferred pragmas
        if (upper.find("#PRAGMA DIALECT AGKLITE") != std::string::npos ||
            upper.find("#PRAGMA DIALECT AGKLIKE") != std::string::npos) {
            agk_mode = true;
        }
        // Back-compat pragma
        if (upper.find("#PRAGMA AGK ON") != std::string::npos) {
            agk_mode = true;
        }
        // Strict classic selection pragmas
        if (upper.find("#PRAGMA DIALECT CLASSIC") != std::string::npos ||
            upper.find("#PRAGMA STRICT CLASSIC") != std::string::npos ||
            upper.find("#PRAGMA CLASSIC ON") != std::string::npos) {
            agk_mode = false;
        }
    }

    // Dialect from CLI overrides/augments pragma
    if (!dialect.empty()) {
        std::string d = dialect; std::transform(d.begin(), d.end(), d.begin(), [](unsigned char c){ return (char)std::tolower(c); });
        if (d == "agk" || d == "agklike" || d == "agklite") agk_mode = true;
        else if (d == "classic") agk_mode = false;
    }

    // Lexical analysis
    if (debug_mode) std::cout << "Phase 1: Lexical Analysis..." << std::endl;
    bas::Lexer lx(std::move(src));
    auto toks = lx.lex();
    
    if (debug_mode) {
        std::cout << "  Tokens generated: " << toks.size() << std::endl;
        if (verbose_mode) {
            std::cout << "  First few tokens:" << std::endl;
            for (size_t i = 0; i < std::min(toks.size(), size_t(10)); i++) {
                std::cout << "    " << i << ": " << toks[i].lex << " (line " << toks[i].line << ")" << std::endl;
            }
        }
    }
    
    // Auto-detect agklite dialect if not set by pragma/CLI and a single-line IF is present
    bool autodetected_agk = false;
    if (!agk_mode && dialect.empty()) {
        bool detected = false;
        for (size_t i = 0; i < toks.size(); ++i) {
            if (toks[i].kind == bas::Tok::If) {
                // scan until newline/eof for THEN
                size_t j = i + 1;
                bool foundThen = false;
                bool anyAfterIf = false;
                while (j < toks.size() && toks[j].kind != bas::Tok::Newline && toks[j].kind != bas::Tok::Eof) {
                    if (toks[j].kind == bas::Tok::Then) { foundThen = true; break; }
                    if (!anyAfterIf && toks[j].kind != bas::Tok::Ident && toks[j].kind != bas::Tok::Number && toks[j].kind != bas::Tok::String) anyAfterIf = true;
                    ++j;
                }
                if (foundThen) {
                    // after THEN, ensure there is at least one more token before newline/eof
                    size_t k = j + 1;
                    if (k < toks.size() && toks[k].kind != bas::Tok::Newline && toks[k].kind != bas::Tok::Eof) {
                        agk_mode = true;
                        detected = true;
                        autodetected_agk = true;
                        break;
                    }
                } else {
                    // THEN-less single-line IF: tokens exist after IF before newline
                    if (anyAfterIf) {
                        agk_mode = true;
                        detected = true;
                        autodetected_agk = true;
                        break;
                    }
                }
            }
        }
        if (debug_mode && detected) {
            std::cout << "Auto-detected dialect: AGK/DBPro (single-line IF)" << std::endl;
        }
    }
    if (autodetected_agk && verbose_mode && !suppressDialectWarn) {
        std::cerr << "WARNING: Permissive mode engaged due to single-line IF. Use #pragma NOWARN DIALECT to suppress this message." << std::endl;
    }
    
    // Parsing
    if (debug_mode) std::cout << "Phase 2: Parsing..." << std::endl;
    bas::Diag diag;
    diag.set_debug_mode(debug_mode);
    
    bas::Parser ps(std::move(toks), diag, agk_mode);
    bas::Program prog = ps.parse();
    
    // Check for parsing errors
    if (diag.has_errors()) {
        std::cerr << std::endl;
        std::cerr << "Parsing failed with " << diag.diagnostics.size() << " error(s):" << std::endl;
        std::cerr << "================================================" << std::endl;
        diag.print_errors();
        
        if (verbose_mode) {
            std::cerr << "All diagnostics:" << std::endl;
            std::cerr << "================================================" << std::endl;
            diag.print_all();
        }
        
        std::cerr << "Compilation failed. Please fix the errors above and try again." << std::endl;
        return 65;
    }
    
    if (debug_mode) {
        std::cout << "  Parse tree created successfully" << std::endl;
        std::cout << "  Statements: " << prog.stmts.size() << std::endl;
        std::cout << "  Dialect: " << (agk_mode? "Permissive" : "Classic (strict)") << std::endl;
    }
    
    // Show warnings if any
    if (verbose_mode && diag.diagnostics.size() > 0) {
        std::cout << "Warnings during compilation:" << std::endl;
        diag.print_warnings();
    }
    
    // Runtime setup
    if (debug_mode) std::cout << "Phase 3: Runtime Setup..." << std::endl;
    bas::FunctionRegistry R;
    bas::register_builtins(R);
    bas::register_raylib_bindings(R);
    bas::register_navigation_functions(R);
    bas::register_physics_functions(R);
    bas::register_ai_functions(R);
    bas::register_graphics_functions(R);
    bas::register_networking_functions(R);
    bas::register_audio_functions(R);
    
    if (debug_mode) {
        std::cout << "  Built-in functions registered: " << R.size() << std::endl;
    }
    
    // Execution
    if (debug_mode) {
        std::cout << "Phase 4: Execution..." << std::endl;
    }
    
    try {
        int rc = bas::interpret(prog, R);
        if (rc != 0) {
            std::cerr << "Runtime error: Program returned exit code " << rc << std::endl;
            return 70;
        }
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << std::endl;
        std::cerr << "Fatal runtime error: " << e.what() << std::endl;
        if (debug_mode) {
            std::cerr << "Exception type: " << typeid(e).name() << std::endl;
        }
        return 70;
    } catch (...) {
        std::cerr << std::endl;
        std::cerr << "Unknown fatal runtime error occurred" << std::endl;
        return 70;
    }
}

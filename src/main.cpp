#include "bas/lexer.hpp"
#include "bas/parser.hpp"
#include "bas/runtime.hpp"
#include "bas/namespace_registry.hpp"
#include "bas/type_system.hpp"
#include "bas/builtins.hpp"
#include "bas/navigation.hpp"
#include "bas/physics.hpp"
#include "bas/ai.hpp"
#include "bas/graphics.hpp"
#include "bas/networking.hpp"
#include "bas/audio.hpp"
#include "bas/camera3d.hpp"
#include "bas/lighting3d.hpp"
#include "bas/models3d.hpp"
#include "bas/game_systems.hpp"
#include "bas/gui.hpp"
#include "bas/raymath.hpp"
#include "bas/sprite_system.hpp"
#include "bas/timer_system.hpp"
#include "bas/input_events.hpp"
#include "bas/animation_system.hpp"
#include "bas/scene_entity_system.hpp"
#include "bas/ecs_system.hpp"
#include "bas/camera_system.hpp"
#include "bas/collision_system.hpp"
#include "bas/game_loop.hpp"
#include "bas/game_helpers.hpp"
#include "bas/builtins_json.hpp"
#include "bas/builtins_file.hpp"
#include "bas/tween_system.hpp"
#include "bas/camera_shake.hpp"
#include "bas/localization.hpp"
#include "bas/state_machine.hpp"
#include "bas/modern_state_system.hpp"
#include "bas/game_commands.hpp"
#include "bas/modern_features.hpp"
#include "bas/advanced_features.hpp"
#include "bas/post_processing.hpp"
#include "bas/streaming.hpp"
#include "bas/enhanced_events.hpp"
#include "bas/screen_transitions.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <raylib.h>
#include <raylib.h>

#if defined(_WIN32) && defined(_DEBUG)
#include <windows.h>
#include <cstdio>
#endif
#include <string>
#include <cstring>
#include <algorithm>
#include <filesystem>
#include <unordered_set>

void print_usage(const char* program_name) {
    std::cout << "BASIC + Raylib Interpreter v1.0" << std::endl;
    std::cout << "Usage: " << program_name << " [options] <file.bas>" << std::endl;
    std::cout << "       " << program_name << " --repl" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cerr << "  --repl          Start interactive REPL (Read-Eval-Print Loop)" << std::endl;
    std::cerr << "  --debug, -d     Enable debug mode with detailed error information" << std::endl;
    std::cerr << "  --verbose, -v   Show warnings and additional information" << std::endl;
    std::cerr << "  --dialect <name>  Select dialect: classic | agklite" << std::endl;
    std::cerr << "                   Default is permissive (agklite features ON)." << std::endl;
    std::cerr << "                   Use --dialect classic to enforce strict classic." << std::endl;
    std::cerr << "  --agk             [deprecated - will be removed] Alias for --dialect agklite" << std::endl;
    std::cerr << "  --help, -h      Show this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cerr << "  " << program_name << " game.bas" << std::endl;
    std::cerr << "  " << program_name << " --debug test.bas" << std::endl;
    std::cerr << "  " << program_name << " --repl" << std::endl;
    std::cerr << "  " << program_name << " --verbose demo.bas" << std::endl;
}

int main(int argc, char* argv[]) {
    bool debug_mode = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-d") == 0) {
            debug_mode = true;
            break;
        }
    }

    if (debug_mode) {
        std::cerr << "[DEBUG] Application starting..." << std::endl;
    }
#if defined(_WIN32) && defined(_DEBUG)
    if (AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()) {
        FILE* f_dummy;
        freopen_s(&f_dummy, "CONOUT$", "w", stdout);
        freopen_s(&f_dummy, "CONOUT$", "w", stderr);
        freopen_s(&f_dummy, "CONIN$", "r", stdin);
    }
#endif
    bool verbose_mode = false;
    bool repl_mode = false;
    // Permissive by default: single-line IF and AGK-like loops enabled
    bool agk_mode = true;
    std::string dialect;
    std::string filename;
    bool suppressDialectWarn = false;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-d") == 0) {
            // Already handled above, so just skip
            continue;
        } else if (strcmp(argv[i], "--repl") == 0) {
            repl_mode = true;
        } else if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
            verbose_mode = true;
        } else if (strncmp(argv[i], "--dialect=", 10) == 0) {
            dialect = std::string(argv[i] + 10);
        } else if (strcmp(argv[i], "--dialect") == 0 && i + 1 < argc) {
            dialect = std::string(argv[++i]);
        } else if (strcmp(argv[i], "--agk") == 0) { // deprecated alias - removed
            std::cerr << "Warning: --agk flag is deprecated. Use --dialect agklite instead." << std::endl;
            dialect = "agklite";
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-") == 0) {
            // Support stdin input with "-"
            filename = "-";
        } else if (argv[i][0] != '-') {
            filename = argv[i];
        } else {
            std::cerr << "Unknown option: " << argv[i] << std::endl;
            print_usage(argv[0]);
            return 64;
        }
    }
    
    // REPL Mode
    if (repl_mode) {
        std::cout << "CyberBasic REPL v1.0" << std::endl;
        std::cout << "Type 'exit' or 'quit' to exit, 'help' for help" << std::endl;
        std::cout << std::endl;
        
        // Setup runtime
        bas::FunctionRegistry R;
        bas::register_builtins(R);
        bas::register_object_constructors(R);
        bas::register_raylib_bindings(R);
        bas::register_raymath_functions(R);
        bas::register_game_systems_bindings(R);
        bas::register_navigation_functions(R);
        bas::register_physics_functions(R);
        bas::register_ai_functions(R);
        bas::register_graphics_functions(R);
        bas::register_networking_functions(R);
        bas::register_audio_functions(R);
        bas::register_camera3d_functions(R);
        bas::register_lighting3d_functions(R);
        bas::register_models3d_functions(R);
        bas::register_level_editor_functions(R);
        bas::register_asset_pipeline_functions(R);
        bas::register_sprite_animation_functions(R);
        bas::register_gui_functions(R);
        
        // Setup namespace registry
        bas::NamespaceRegistry namespace_registry;
        bas::register_raylib_namespaces(namespace_registry);
        bas::register_game_namespaces(namespace_registry);
        bas::set_namespace_registry(&namespace_registry);
        
        std::string line;
        std::string multi_line;
        bool in_multiline = false;
        
        while (true) {
            if (!in_multiline) {
                std::cout << "> ";
            } else {
                std::cout << "... ";
            }
            
            if (!std::getline(std::cin, line)) {
                break; // EOF
            }
            
            if (line.empty() && !in_multiline) {
                continue;
            }
            
            // Handle special commands
            std::string line_upper = line;
            std::transform(line_upper.begin(), line_upper.end(), line_upper.begin(), 
                         [](unsigned char c) { return std::toupper(c); });
            
            if (line_upper == "EXIT" || line_upper == "QUIT") {
                std::cout << "Goodbye!" << std::endl;
                break;
            }
            
            if (line_upper == "HELP") {
                std::cout << "REPL Commands:" << std::endl;
                std::cout << "  exit, quit  - Exit REPL" << std::endl;
                std::cout << "  help        - Show this help" << std::endl;
                std::cout << "  clear       - Clear screen" << std::endl;
                std::cout << std::endl;
                std::cout << "You can type BASIC code directly:" << std::endl;
                std::cout << "  > VAR x = 10" << std::endl;
                std::cout << "  > PRINT x" << std::endl;
                std::cout << "  > VAR vec = Vector3(1, 2, 3)" << std::endl;
                std::cout << "  > vec.length()" << std::endl;
                continue;
            }
            
            if (line_upper == "CLEAR") {
                #ifdef _WIN32
                system("cls");
                #else
                system("clear");
                #endif
                continue;
            }
            
            // Accumulate multi-line input
            if (!multi_line.empty()) {
                multi_line += "\n" + line;
            } else {
                multi_line = line;
            }
            
            // Check if line ends with continuation (backslash or incomplete statement)
            if (!line.empty() && (line.back() == '\\' || line.back() == '_')) {
                in_multiline = true;
                multi_line.pop_back(); // Remove continuation character
                continue;
            }
            
            // Try to parse and execute
            try {
                bas::Lexer lex(multi_line);
                auto toks = lex.lex();
                bas::Diag diag;
                diag.set_debug_mode(debug_mode);
                bas::Parser ps(std::move(toks), diag, agk_mode);
                bas::Program prog = ps.parse();
                
                if (diag.has_errors()) {
                    diag.print_errors();
                    multi_line.clear();
                    in_multiline = false;
                    continue;
                }
                
                // Execute
                int rc = bas::interpret(prog, R, debug_mode);
                if (rc != 0 && debug_mode) {
                    std::cerr << "Execution returned code: " << rc << std::endl;
                }
                
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
            
            multi_line.clear();
            in_multiline = false;
        }
        
        return 0;
    }
    
    if (debug_mode) std::cerr << "[DEBUG] Filename to execute: " << filename << std::endl;

    if (filename.empty()) {
        std::cerr << "Error: No input file specified" << std::endl;
        print_usage(argv[0]);
        return 64;
    }
    
    if (verbose_mode) {
        std::cout << "BASIC + Raylib Interpreter v1.0" << std::endl;
        if (debug_mode) std::cerr << "Debug mode: ENABLED" << std::endl;
        std::cout << "Loading: " << (filename == "-" ? "stdin" : filename) << std::endl;
        std::cout << std::endl;
    }
    
    // Read from stdin if filename is "-", otherwise read from file
    std::string src;
    if (filename == "-") {
        if (debug_mode) std::cerr << "[DEBUG] Reading from stdin..." << std::endl;
        std::string line;
        while (std::getline(std::cin, line)) {
            src += line + "\n";
        }
    } else {
        // Open and read the source file
        if (debug_mode) std::cerr << "[DEBUG] Attempting to open file..." << std::endl;
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            if (debug_mode) std::cerr << "[DEBUG] Failed to open file." << std::endl;
            std::cerr << "Error: Cannot open file '" << filename << "'" << std::endl;
            std::cerr << "  Check if the file exists and you have read permissions" << std::endl;
            return 66;
        }
        src = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }
    if (src.empty()) {
        std::cerr << "Error: File '" << filename << "' is empty" << std::endl;
        return 65;
    }
    
        if (debug_mode) {
        std::cerr << "Source file loaded: " << src.length() << " characters" << std::endl;
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
        if (debug_mode) std::cerr << "Phase 1: Lexical Analysis..." << std::endl;
    bas::Lexer lx(std::move(src));
    auto toks = lx.lex();
    
    if (debug_mode) {
        std::cerr << "  Tokens generated: " << toks.size() << std::endl;
        if (verbose_mode) {
            std::cerr << "  First few tokens:" << std::endl;
            size_t max_tokens = toks.size() < 10 ? toks.size() : 10;
            for (size_t i = 0; i < max_tokens; i++) {
                std::cerr << "    " << i << ": " << toks[i].lex << " (line " << toks[i].line << ")" << std::endl;
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
            std::cerr << "Auto-detected dialect: AGK/DBPro (single-line IF)" << std::endl;
        }
    }
    if (autodetected_agk && verbose_mode && !suppressDialectWarn) {
        std::cerr << "WARNING: Permissive mode engaged due to single-line IF. Use #pragma NOWARN DIALECT to suppress this message." << std::endl;
    }
    
    // Parsing
        if (debug_mode) std::cerr << "Phase 2: Parsing..." << std::endl;
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
        std::cerr << "  Parse tree created successfully" << std::endl;
        std::cerr << "  Statements: " << prog.stmts.size() << std::endl;
        std::cerr << "  Dialect: " << (agk_mode? "Permissive" : "Classic (strict)") << std::endl;
    }
    
    // Show warnings if any
    if (verbose_mode && diag.diagnostics.size() > 0) {
        std::cout << "Warnings during compilation:" << std::endl;
        diag.print_warnings();
    }
    
    // Expand IMPORT statements before runtime
        if (debug_mode) std::cerr << "Phase 3: Resolving imports..." << std::endl;
    auto expand_imports = [&](auto&& self, bas::Program& program, const std::filesystem::path& baseDir, std::unordered_set<std::string>& loaded)->bool{
        std::vector<std::unique_ptr<bas::Stmt>> result;
        result.reserve(program.stmts.size());
        for (auto& up : program.stmts) {
            if (auto imp = dynamic_cast<bas::ImportStmt*>(up.get())) {
                std::filesystem::path rel(imp->path);
                std::filesystem::path full = rel.is_absolute() ? rel : (baseDir / rel);
                std::error_code ec;
                std::filesystem::path canon = std::filesystem::weakly_canonical(full, ec);
                if (ec) canon = full.lexically_normal();
                std::string canonStr = canon.string();
                if (verbose_mode) {
                    std::cerr << "  IMPORT: '" << imp->path << "' -> " << canonStr << std::endl;
                }
                if (loaded.find(canonStr) != loaded.end()) {
                    // Already loaded; skip
                    continue;
                }
                // Load file
                std::ifstream fin(canon, std::ios::binary);
                if (!fin.good()) {
                    std::cerr << "Error: Cannot open imported file '" << canonStr << "'" << std::endl;
                    return false;
                }
                std::string src2((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
                bas::Lexer lx2(std::move(src2));
                auto toks2 = lx2.lex();
                bas::Diag d2; d2.set_debug_mode(debug_mode);
                bas::Parser ps2(std::move(toks2), d2, agk_mode);
                bas::Program p2 = ps2.parse();
                if (d2.has_errors()) {
                    std::cerr << "Parsing failed in imported file: " << canonStr << std::endl;
                    d2.print_errors();
                    return false;
                }
                // Mark loaded and recursively expand its imports
                loaded.insert(canonStr);
                if (!self(self, p2, canon.parent_path(), loaded)) return false;
                // Splice imported statements into result
                for (auto& s2 : p2.stmts) {
                    result.push_back(std::move(s2));
                }
            } else {
                result.push_back(std::move(up));
            }
        }
        program.stmts = std::move(result);
        return true;
    };
    {
        std::unordered_set<std::string> loaded;
        // Consider the main file as loaded to prevent accidental self-import if used
        std::error_code ec;
        std::filesystem::path mainPath = std::filesystem::weakly_canonical(std::filesystem::path(filename), ec);
        if (ec) mainPath = std::filesystem::path(filename).lexically_normal();
        loaded.insert(mainPath.string());
        std::filesystem::path baseDir = mainPath.parent_path();
        if (!expand_imports(expand_imports, prog, baseDir, loaded)) {
            return 65;
        }
        if (debug_mode) {
            std::cerr << "  After imports, statements: " << prog.stmts.size() << std::endl;
        }
    }

    // Runtime setup
        if (debug_mode) std::cerr << "Phase 3: Runtime Setup..." << std::endl;
    bas::FunctionRegistry R;
    
    // Create and register namespace registry for dot notation
    bas::NamespaceRegistry namespace_registry;
    bas::register_raylib_namespaces(namespace_registry);
    bas::set_namespace_registry(&namespace_registry);
    
    // Create and register type registry for user-defined types
    bas::TypeRegistry type_registry;
    bas::set_type_registry(&type_registry);
    
    // Core BASIC functions
    bas::register_builtins(R);
    
    // Enums, dictionaries, and states
    bas::register_enums_and_dicts(R);
    
    // Enhanced dot notation system
    bas::register_dot_notation_enhancements(R);
    
    // Advanced features (coroutines, testing, profiling, etc.)
    bas::register_advanced_features(R);
    
    // Object constructors (Vector3, Camera3D, Color, etc.)
    bas::register_object_constructors(R);
    
    // Raylib graphics and multimedia
    bas::register_raylib_bindings(R);
    bas::register_raymath_functions(R);
    
    // Game programming systems
    bas::register_game_systems_bindings(R);
    bas::register_navigation_functions(R);
    bas::register_physics_functions(R);
    bas::register_ai_functions(R);
    bas::register_graphics_functions(R);
    bas::register_networking_functions(R);
    bas::register_audio_functions(R);
    bas::register_camera3d_functions(R);
    bas::register_lighting3d_functions(R);
    bas::register_models3d_functions(R);
    
    // Enhanced game development systems
    bas::register_level_editor_functions(R);
    bas::register_asset_pipeline_functions(R);
    bas::register_sprite_animation_functions(R);
    bas::register_gui_functions(R);
    
    // New modular game systems
    bas::register_sprite_system(R);
    bas::register_timer_system(R);
    bas::register_input_events(R);
    bas::register_animation_system(R);
    bas::register_scene_entity_system(R);
    bas::register_ecs_system(R);  // Enhanced ECS system
    bas::register_camera_system(R);
    bas::register_collision_system(R);
    bas::register_game_loop(R);
    bas::register_game_helpers(R);
    
    // Modern game development helpers
    bas::register_asset_manager(R);
    bas::register_save_load(R);
    bas::register_profiling(R);
    bas::register_debug_viz(R);
    
    // JSON and File I/O support
    bas::register_json_functions(R);
    bas::register_file_functions(R);
    
    // New AGK2-style features
    bas::register_tween_system(R);
    bas::register_camera_shake(R);
    bas::register_localization(R);
    bas::register_state_machine(R);
    bas::register_modern_state_system(R);  // Modern BASIC-style state system
    bas::register_game_commands(R);  // High-level game commands
    bas::register_modern_features(R);  // Modern features (Sets, high-level networking/file I/O)
    bas::register_game_advanced_features(R);  // Advanced game features (macros, particles, dialogue, etc.)
    bas::register_post_processing(R);
    bas::register_streaming(R);
    bas::register_enhanced_events(R);
    bas::register_screen_transitions(R);
    
    bas::set_event_registry(&R);
    
    if (debug_mode) {
        std::cerr << "  Built-in functions registered: " << R.size() << std::endl;
    }
    
    // Execution
        if (debug_mode) {
        std::cerr << "Phase 4: Execution..." << std::endl;
    }
    
    std::string error_message;
    bool had_error = false;
    int exit_code = 0;
    
    try {
        int rc = bas::interpret(prog, R, debug_mode);
        if (rc != 0) {
            error_message = "Runtime error: Program returned exit code " + std::to_string(rc);
            std::cerr << error_message << std::endl;
            had_error = true;
            exit_code = 70;
        }
    } catch (const std::exception& e) {
        error_message = std::string("Fatal runtime error: ") + e.what();
        std::cerr << std::endl;
        std::cerr << error_message << std::endl;
        if (debug_mode) {
            std::cerr << "Exception type: " << typeid(e).name() << std::endl;
        }
        had_error = true;
        exit_code = 70;
    } catch (...) {
        error_message = "Unknown fatal runtime error occurred";
        std::cerr << std::endl;
        std::cerr << error_message << std::endl;
        had_error = true;
        exit_code = 70;
    }
    
    // If a window is open, keep it open until user closes it
    // This allows the user to see the program output or error messages
    if (IsWindowReady()) {
        // If there was an error, display it in the window
        if (had_error && !error_message.empty()) {
            // Keep window open and show error message
            while (!WindowShouldClose()) {
                BeginDrawing();
                ClearBackground(BLACK);
                
                // Draw error message
                int y = 20;
                int line_height = 25;
                std::string display_msg = "ERROR:";
                DrawText(display_msg.c_str(), 10, y, 20, RED);
                y += line_height;
                
                // Split error message into lines if too long
                size_t max_width = 70;
                size_t pos = 0;
                while (pos < error_message.length()) {
                    size_t end = pos + max_width;
                    if (end < error_message.length()) {
                        // Try to break at a space
                        size_t space_pos = error_message.rfind(' ', end);
                        if (space_pos != std::string::npos && space_pos > pos) {
                            end = space_pos;
                        }
                    }
                    std::string line = error_message.substr(pos, end - pos);
                    DrawText(line.c_str(), 10, y, 18, WHITE);
                    y += line_height;
                    pos = (end < error_message.length() && error_message[end] == ' ') ? end + 1 : end;
                }
                
                DrawText("Press ESC or close window to exit", 10, y + 10, 16, YELLOW);
                
                EndDrawing();
                
                // Check for ESC key
                if (IsKeyPressed(KEY_ESCAPE)) {
                    break;
                }
            }
        } else {
            // No error - just keep window open until user closes it
            while (!WindowShouldClose()) {
                BeginDrawing();
                ClearBackground(BLACK);
                
                // Show a simple message that program completed
                DrawText("Program completed successfully.", 10, 10, 20, GREEN);
                DrawText("Press ESC or close window to exit", 10, 40, 16, WHITE);
                
                EndDrawing();
                
                // Check for ESC key
                if (IsKeyPressed(KEY_ESCAPE)) {
                    break;
                }
            }
        }
        
        // Clean up window
        CloseWindow();
    }
    
    return exit_code;
}

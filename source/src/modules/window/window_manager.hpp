#pragma once

#include "bas/runtime.hpp"
#include <string>
#include <vector>

namespace bas {

/**
 * Window Manager - Centralized window state and configuration management
 * Provides a future-proof, modular approach to window handling
 */
class WindowManager {
public:
    struct WindowConfig {
        int width = 800;
        int height = 600;
        std::string title = "CyberBasic Application";
        bool fullscreen = false;
        bool resizable = true;
        bool vsync = true;
        bool msaa = false;
        int msaa_samples = 4;
        bool high_dpi = false;
        float opacity = 1.0f;
        int min_width = 320;
        int min_height = 240;
        int max_width = 0;  // 0 = no limit
        int max_height = 0; // 0 = no limit
        int monitor = 0;
        bool borderless = false;
        bool always_on_top = false;
        bool transparent = false;
    };

    struct WindowState {
        bool initialized = false;
        bool should_close = false;
        bool focused = false;
        bool minimized = false;
        bool maximized = false;
        bool hidden = false;
        bool resized = false;
        int current_width = 0;
        int current_height = 0;
        int position_x = 0;
        int position_y = 0;
        int current_monitor = 0;
        float current_opacity = 1.0f;
    };

private:
    WindowConfig config_;
    WindowState state_;
    std::vector<std::string> event_log_;
    bool debug_mode_ = false;

public:
    WindowManager() = default;
    ~WindowManager() = default;

    // === INITIALIZATION ===
    bool initialize(const WindowConfig& config);
    void shutdown();
    
    // === CONFIGURATION ===
    void set_config(const WindowConfig& config) { config_ = config; }
    const WindowConfig& get_config() const { return config_; }
    
    // === STATE QUERIES ===
    const WindowState& get_state() const { return state_; }
    bool is_ready() const { return state_.initialized; }
    bool should_close() const { return state_.should_close; }
    bool is_fullscreen() const;
    bool is_minimized() const { return state_.minimized; }
    bool is_maximized() const { return state_.maximized; }
    bool is_focused() const { return state_.focused; }
    bool is_hidden() const { return state_.hidden; }
    bool was_resized() const { return state_.resized; }
    
    // === WINDOW CONTROL ===
    void set_title(const std::string& title);
    void set_position(int x, int y);
    void set_size(int width, int height);
    void set_opacity(float opacity);
    void set_monitor(int monitor);
    void set_min_size(int width, int height);
    void set_max_size(int width, int height);
    
    // === WINDOW STATE CONTROL ===
    void toggle_fullscreen();
    void toggle_borderless();
    void maximize();
    void minimize();
    void restore();
    void focus();
    void show();
    void hide();
    
    // === MONITOR INFO ===
    int get_monitor_count() const;
    int get_current_monitor() const;
    std::pair<int, int> get_monitor_size(int monitor) const;
    std::pair<int, int> get_monitor_position(int monitor) const;
    int get_monitor_refresh_rate(int monitor) const;
    std::string get_monitor_name(int monitor) const;
    
    // === SCREEN INFO ===
    int get_screen_width() const;
    int get_screen_height() const;
    int get_render_width() const;
    int get_render_height() const;
    std::pair<int, int> get_window_position() const;
    std::pair<float, float> get_window_scale_dpi() const;
    
    // === EVENT HANDLING ===
    void update(); // Call this every frame to update state
    void enable_event_waiting();
    void disable_event_waiting();
    
    // === CLIPBOARD ===
    void set_clipboard_text(const std::string& text);
    std::string get_clipboard_text() const;
    
    // === CURSOR CONTROL ===
    void show_cursor();
    void hide_cursor();
    void enable_cursor();
    void disable_cursor();
    bool is_cursor_hidden() const;
    bool is_cursor_on_screen() const;
    
    // === DEBUG AND UTILITIES ===
    void enable_debug_mode() { debug_mode_ = true; }
    void disable_debug_mode() { debug_mode_ = false; }
    const std::vector<std::string>& get_event_log() const { return event_log_; }
    void clear_event_log() { event_log_.clear(); }
    
    // === VALIDATION ===
    bool validate_config(const WindowConfig& config) const;
    std::string get_last_error() const;

private:
    void log_event(const std::string& event);
    void update_state();
    std::string last_error_;
};

// Global window manager instance
extern std::unique_ptr<WindowManager> g_window_manager;

// === NATIVE FUNCTION DECLARATIONS ===
// Window initialization
Value window_init(const std::vector<Value>& args);
Value window_close(const std::vector<Value>& args);

// Window state queries
Value window_should_close(const std::vector<Value>& args);
Value window_is_ready(const std::vector<Value>& args);
Value window_is_fullscreen(const std::vector<Value>& args);
Value window_is_minimized(const std::vector<Value>& args);
Value window_is_maximized(const std::vector<Value>& args);
Value window_is_focused(const std::vector<Value>& args);
Value window_is_hidden(const std::vector<Value>& args);
Value window_was_resized(const std::vector<Value>& args);

// Window control
Value window_set_title(const std::vector<Value>& args);
Value window_set_position(const std::vector<Value>& args);
Value window_set_size(const std::vector<Value>& args);
Value window_set_opacity(const std::vector<Value>& args);
Value window_toggle_fullscreen(const std::vector<Value>& args);
Value window_maximize(const std::vector<Value>& args);
Value window_minimize(const std::vector<Value>& args);
Value window_restore(const std::vector<Value>& args);

// Screen and monitor info
Value window_get_screen_width(const std::vector<Value>& args);
Value window_get_screen_height(const std::vector<Value>& args);
Value window_get_monitor_count(const std::vector<Value>& args);
Value window_get_monitor_name(const std::vector<Value>& args);

// Cursor control
Value cursor_show(const std::vector<Value>& args);
Value cursor_hide(const std::vector<Value>& args);
Value cursor_enable(const std::vector<Value>& args);
Value cursor_disable(const std::vector<Value>& args);
Value cursor_is_hidden(const std::vector<Value>& args);
Value cursor_is_on_screen(const std::vector<Value>& args);

// Clipboard
Value clipboard_set_text(const std::vector<Value>& args);
Value clipboard_get_text(const std::vector<Value>& args);

// Function registration
void register_window_functions(FunctionRegistry& registry);

} // namespace bas

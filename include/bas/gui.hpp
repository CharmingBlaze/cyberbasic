#pragma once

// CyberBasic GUI System - Modular and Easy to Use
// This header provides a clean, unified interface to the GUI system

#include "gui_types.hpp"
#include "gui_control.hpp"
#include "gui_controls.hpp"
#include "gui_style.hpp"
#include "gui_manager.hpp"
#include "gui_layout.hpp"
#include "runtime.hpp"

namespace bas {

// Convenience functions for common GUI operations
namespace Gui {

// Quick control creation with sensible defaults
inline int Button(const std::string& name, int x, int y, int width = 100, int height = 30, const std::string& text = "") {
    return GuiManager::create_button(name, x, y, width, height, text);
}

inline int Label(const std::string& name, int x, int y, int width = 100, int height = 20, const std::string& text = "") {
    return GuiManager::create_label(name, x, y, width, height, text);
}

inline int TextBox(const std::string& name, int x, int y, int width = 200, int height = 25, const std::string& text = "") {
    return GuiManager::create_textbox(name, x, y, width, height, text);
}

inline int Slider(const std::string& name, int x, int y, int width = 200, int height = 20, double min_val = 0, double max_val = 100, double current_val = 50) {
    return GuiManager::create_slider(name, x, y, width, height, min_val, max_val, current_val);
}

inline int CheckBox(const std::string& name, int x, int y, int width = 150, int height = 20, const std::string& text = "", bool checked = false) {
    return GuiManager::create_checkbox(name, x, y, width, height, text, checked);
}

inline int ComboBox(const std::string& name, int x, int y, int width = 150, int height = 25) {
    return GuiManager::create_combobox(name, x, y, width, height);
}

inline int ProgressBar(const std::string& name, int x, int y, int width = 200, int height = 20, double min_val = 0, double max_val = 100, double current_val = 0) {
    return GuiManager::create_progressbar(name, x, y, width, height, min_val, max_val, current_val);
}

inline int ListView(const std::string& name, int x, int y, int width = 200, int height = 150) {
    return GuiManager::create_listview(name, x, y, width, height);
}

inline int Window(const std::string& name, int x, int y, int width = 400, int height = 300, const std::string& title = "") {
    return GuiManager::create_windowbox(name, x, y, width, height, title);
}

inline int Panel(const std::string& name, int x, int y, int width = 200, int height = 150) {
    return GuiManager::create_panel(name, x, y, width, height);
}

// Quick property setters
inline void SetText(int control_id, const std::string& text) {
    GuiManager::set_control_text(control_id, text);
}

inline void SetText(const std::string& name, const std::string& text) {
    GuiManager::set_control_text(name, text);
}

inline std::string GetText(int control_id) {
    return GuiManager::get_control_text(control_id);
}

inline std::string GetText(const std::string& name) {
    return GuiManager::get_control_text(name);
}

inline void SetValue(int control_id, const std::string& value) {
    GuiManager::set_control_value(control_id, value);
}

inline void SetValue(const std::string& name, const std::string& value) {
    GuiManager::set_control_value(name, value);
}

inline std::string GetValue(int control_id) {
    return GuiManager::get_control_value(control_id);
}

inline std::string GetValue(const std::string& name) {
    return GuiManager::get_control_value(name);
}

inline void SetPosition(int control_id, int x, int y) {
    GuiManager::set_control_position(control_id, x, y);
}

inline void SetPosition(const std::string& name, int x, int y) {
    GuiManager::set_control_position(name, x, y);
}

inline void SetSize(int control_id, int width, int height) {
    GuiManager::set_control_size(control_id, width, height);
}

inline void SetSize(const std::string& name, int width, int height) {
    GuiManager::set_control_size(name, width, height);
}

inline void SetVisible(int control_id, bool visible) {
    GuiManager::set_control_visible(control_id, visible);
}

inline void SetVisible(const std::string& name, bool visible) {
    GuiManager::set_control_visible(name, visible);
}

inline void SetEnabled(int control_id, bool enabled) {
    GuiManager::set_control_enabled(control_id, enabled);
}

inline void SetEnabled(const std::string& name, bool enabled) {
    GuiManager::set_control_enabled(name, enabled);
}

// System functions
inline bool Initialize() {
    return GuiManager::initialize();
}

inline void Shutdown() {
    GuiManager::shutdown();
}

inline void Update() {
    GuiManager::update_all_controls();
}

inline void Draw() {
    GuiManager::draw_all_controls();
}

// Style functions
inline void UseDefaultStyle() {
    GuiStyleManager::use_default_style();
}

inline void UseDarkStyle() {
    GuiStyleManager::use_dark_style();
}

inline void UseLightStyle() {
    GuiStyleManager::use_light_style();
}

inline void UseGameStyle() {
    GuiStyleManager::use_game_style();
}

// Layout functions
inline int CreateHorizontalLayout(int x, int y, int width, int height) {
    return GuiLayoutManager::create_horizontal_layout(Rectangle(x, y, width, height));
}

inline int CreateVerticalLayout(int x, int y, int width, int height) {
    return GuiLayoutManager::create_vertical_layout(Rectangle(x, y, width, height));
}

inline int CreateGridLayout(int x, int y, int width, int height, int columns, int rows) {
    return GuiLayoutManager::create_grid_layout(Rectangle(x, y, width, height), columns, rows);
}

inline void AddToLayout(int layout_id, int control_id) {
    GuiLayoutManager::add_control_to_layout(layout_id, control_id);
}

inline void UpdateLayout(int layout_id) {
    GuiLayoutManager::update_layout(layout_id);
}

} // namespace Gui

// Function registration
void register_gui_functions(FunctionRegistry& registry);

} // namespace bas

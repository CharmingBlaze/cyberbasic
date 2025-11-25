#pragma once
#include "gui_types.hpp"
#include <unordered_map>

namespace bas {

// GUI Style System
class GuiStyle {
private:
    std::unordered_map<std::string, Color> colors;
    std::unordered_map<std::string, int> properties;
    
public:
    GuiStyle();
    
    // Color management
    void set_color(const std::string& property, const Color& color);
    void set_color(const std::string& property, int r, int g, int b, int a = 255);
    Color get_color(const std::string& property) const;
    bool has_color(const std::string& property) const;
    
    // Property management
    void set_property(const std::string& property, int value);
    int get_property(const std::string& property) const;
    bool has_property(const std::string& property) const;
    
    // Style presets
    void load_default_style();
    void load_dark_style();
    void load_light_style();
    void load_game_style();
    
    // Style persistence
    bool load_from_file(const std::string& filename);
    bool save_to_file(const std::string& filename) const;
    
    // Style copying
    void copy_from(const GuiStyle& other);
    void reset();
    
    // Utility
    std::vector<std::string> get_all_color_properties() const;
    std::vector<std::string> get_all_style_properties() const;
};

// Global Style Manager
class GuiStyleManager {
private:
    static GuiStyle default_style;
    static GuiStyle current_style;
    static std::unordered_map<std::string, GuiStyle> named_styles;
    
public:
    // Global style management
    static void set_global_color(const std::string& property, const Color& color);
    static void set_global_color(const std::string& property, int r, int g, int b, int a = 255);
    static Color get_global_color(const std::string& property);
    
    static void set_global_property(const std::string& property, int value);
    static int get_global_property(const std::string& property);
    
    // Style switching
    static void use_default_style();
    static void use_dark_style();
    static void use_light_style();
    static void use_game_style();
    static void use_custom_style(const GuiStyle& style);
    
    // Named styles
    static void save_named_style(const std::string& name, const GuiStyle& style);
    static bool load_named_style(const std::string& name);
    static bool has_named_style(const std::string& name);
    static void remove_named_style(const std::string& name);
    
    // Style persistence
    static bool load_global_style_from_file(const std::string& filename);
    static bool save_global_style_to_file(const std::string& filename);
    
    // Current style access
    static const GuiStyle& get_current_style() { return current_style; }
    static GuiStyle& get_current_style_mutable() { return current_style; }
};

} // namespace bas

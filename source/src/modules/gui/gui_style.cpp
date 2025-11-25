#include "bas/gui_style.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace bas {

// GuiStyle Implementation
GuiStyle::GuiStyle() {
    load_default_style();
}

void GuiStyle::set_color(const std::string& property, const Color& color) {
    colors[property] = color;
}

void GuiStyle::set_color(const std::string& property, int r, int g, int b, int a) {
    colors[property] = Color(r, g, b, a);
}

Color GuiStyle::get_color(const std::string& property) const {
    auto it = colors.find(property);
    if (it != colors.end()) {
        return it->second;
    }
    return Color(0, 0, 0, 255); // Default black
}

bool GuiStyle::has_color(const std::string& property) const {
    return colors.find(property) != colors.end();
}

void GuiStyle::set_property(const std::string& property, int value) {
    properties[property] = value;
}

int GuiStyle::get_property(const std::string& property) const {
    auto it = properties.find(property);
    if (it != properties.end()) {
        return it->second;
    }
    return 0; // Default value
}

bool GuiStyle::has_property(const std::string& property) const {
    return properties.find(property) != properties.end();
}

void GuiStyle::load_default_style() {
    // Default color scheme
    set_color("background", 240, 240, 240, 255);
    set_color("foreground", 0, 0, 0, 255);
    set_color("border", 128, 128, 128, 255);
    set_color("button_normal", 200, 200, 200, 255);
    set_color("button_hover", 180, 180, 180, 255);
    set_color("button_pressed", 160, 160, 160, 255);
    set_color("text", 0, 0, 0, 255);
    set_color("text_disabled", 128, 128, 128, 255);
    
    // Default properties
    set_property("border_width", 1);
    set_property("font_size", 10);
    set_property("padding", 5);
    set_property("margin", 2);
}

void GuiStyle::load_dark_style() {
    // Dark color scheme
    set_color("background", 32, 32, 32, 255);
    set_color("foreground", 255, 255, 255, 255);
    set_color("border", 64, 64, 64, 255);
    set_color("button_normal", 64, 64, 64, 255);
    set_color("button_hover", 80, 80, 80, 255);
    set_color("button_pressed", 96, 96, 96, 255);
    set_color("text", 255, 255, 255, 255);
    set_color("text_disabled", 128, 128, 128, 255);
    
    // Properties
    set_property("border_width", 1);
    set_property("font_size", 10);
    set_property("padding", 5);
    set_property("margin", 2);
}

void GuiStyle::load_light_style() {
    // Light color scheme
    set_color("background", 255, 255, 255, 255);
    set_color("foreground", 0, 0, 0, 255);
    set_color("border", 200, 200, 200, 255);
    set_color("button_normal", 240, 240, 240, 255);
    set_color("button_hover", 220, 220, 220, 255);
    set_color("button_pressed", 200, 200, 200, 255);
    set_color("text", 0, 0, 0, 255);
    set_color("text_disabled", 160, 160, 160, 255);
    
    // Properties
    set_property("border_width", 1);
    set_property("font_size", 10);
    set_property("padding", 5);
    set_property("margin", 2);
}

void GuiStyle::load_game_style() {
    // Game-style color scheme
    set_color("background", 20, 20, 40, 255);
    set_color("foreground", 255, 255, 255, 255);
    set_color("border", 100, 100, 200, 255);
    set_color("button_normal", 40, 40, 80, 255);
    set_color("button_hover", 60, 60, 120, 255);
    set_color("button_pressed", 80, 80, 160, 255);
    set_color("text", 255, 255, 255, 255);
    set_color("text_disabled", 128, 128, 128, 255);
    
    // Properties
    set_property("border_width", 2);
    set_property("font_size", 12);
    set_property("padding", 8);
    set_property("margin", 4);
}

bool GuiStyle::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    reset(); // Start with a clean style
    std::string line, current_section;
    
    while (std::getline(file, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);

        if (line.empty() || line[0] == '#') continue; // Skip empty lines and comments

        if (line[0] == '[' && line.back() == ']') {
            current_section = line.substr(1, line.length() - 2);
        } else {
            std::stringstream ss(line);
            std::string key, value;
            if (std::getline(ss, key, '=') && std::getline(ss, value)) {
                // Trim key and value
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);

                if (current_section == "Colors") {
                    std::stringstream vss(value);
                    std::string component;
                    int r, g, b, a = 255;
                    if (std::getline(vss, component, ',') && (r = std::stoi(component)) >= 0 &&
                        std::getline(vss, component, ',') && (g = std::stoi(component)) >= 0 &&
                        std::getline(vss, component, ',') && (b = std::stoi(component)) >= 0) {
                        // Optional alpha
                        if (std::getline(vss, component, ',')) a = std::stoi(component);
                        set_color(key, r, g, b, a);
                    }
                } else if (current_section == "Properties") {
                    try {
                        set_property(key, std::stoi(value));
                    } catch (...) { /* ignore invalid values */ }
                }
            }
        }
    }
    return true;
}

bool GuiStyle::save_to_file(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << "# CyberBasic GUI Style File" << std::endl;

    // Write Colors
    file << "\n[Colors]" << std::endl;
    for (const auto& pair : colors) {
        const auto& c = pair.second;
        file << pair.first << " = " << c.r << "," << c.g << "," << c.b << "," << c.a << std::endl;
    }

    // Write Properties
    file << "\n[Properties]" << std::endl;
    for (const auto& pair : properties) {
        file << pair.first << " = " << pair.second << std::endl;
    }

    return true;
}

void GuiStyle::copy_from(const GuiStyle& other) {
    colors = other.colors;
    properties = other.properties;
}

void GuiStyle::reset() {
    colors.clear();
    properties.clear();
    load_default_style();
}

std::vector<std::string> GuiStyle::get_all_color_properties() const {
    std::vector<std::string> result;
    for (const auto& pair : colors) {
        result.push_back(pair.first);
    }
    return result;
}

std::vector<std::string> GuiStyle::get_all_style_properties() const {
    std::vector<std::string> result;
    for (const auto& pair : properties) {
        result.push_back(pair.first);
    }
    return result;
}

// GuiStyleManager Implementation
GuiStyle GuiStyleManager::default_style;
GuiStyle GuiStyleManager::current_style;
std::unordered_map<std::string, GuiStyle> GuiStyleManager::named_styles;

void GuiStyleManager::set_global_color(const std::string& property, const Color& color) {
    current_style.set_color(property, color);
}

void GuiStyleManager::set_global_color(const std::string& property, int r, int g, int b, int a) {
    current_style.set_color(property, r, g, b, a);
}

Color GuiStyleManager::get_global_color(const std::string& property) {
    return current_style.get_color(property);
}

void GuiStyleManager::set_global_property(const std::string& property, int value) {
    current_style.set_property(property, value);
}

int GuiStyleManager::get_global_property(const std::string& property) {
    return current_style.get_property(property);
}

void GuiStyleManager::use_default_style() {
    current_style.copy_from(default_style);
}

void GuiStyleManager::use_dark_style() {
    current_style.load_dark_style();
}

void GuiStyleManager::use_light_style() {
    current_style.load_light_style();
}

void GuiStyleManager::use_game_style() {
    current_style.load_game_style();
}

void GuiStyleManager::use_custom_style(const GuiStyle& style) {
    current_style.copy_from(style);
}

void GuiStyleManager::save_named_style(const std::string& name, const GuiStyle& style) {
    named_styles[name] = style;
}

bool GuiStyleManager::load_named_style(const std::string& name) {
    auto it = named_styles.find(name);
    if (it != named_styles.end()) {
        current_style.copy_from(it->second);
        return true;
    }
    return false;
}

bool GuiStyleManager::has_named_style(const std::string& name) {
    return named_styles.find(name) != named_styles.end();
}

void GuiStyleManager::remove_named_style(const std::string& name) {
    named_styles.erase(name);
}

bool GuiStyleManager::load_global_style_from_file(const std::string& filename) {
    return current_style.load_from_file(filename);
}

bool GuiStyleManager::save_global_style_to_file(const std::string& filename) {
    return current_style.save_to_file(filename);
}

} // namespace bas
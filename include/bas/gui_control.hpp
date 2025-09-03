#pragma once
#include "gui_types.hpp"
#include "value.hpp"

namespace bas {

// Base GUI Control Class
class GuiControl {
protected:
    int id;
    std::string name;
    GuiControlType type;
    Rectangle bounds;
    GuiControlState state;
    bool visible;
    bool enabled;
    std::string text;
    Color textColor;
    Color backgroundColor;
    Color borderColor;
    int fontSize;
    GuiTextAlignment textAlignment;
    GuiTextAlignmentVertical textAlignmentVertical;
    GuiTextWrapMode textWrapMode;
    GuiEventCallback eventCallback;
    
public:
    GuiControl(int control_id, const std::string& control_name, GuiControlType control_type, 
               const Rectangle& control_bounds);
    virtual ~GuiControl() = default;
    
    // Getters
    int get_id() const { return id; }
    const std::string& get_name() const { return name; }
    GuiControlType get_type() const { return type; }
    const Rectangle& get_bounds() const { return bounds; }
    GuiControlState get_state() const { return state; }
    bool is_visible() const { return visible; }
    bool is_enabled() const { return enabled; }
    const std::string& get_text() const { return text; }
    Color get_text_color() const { return textColor; }
    Color get_background_color() const { return backgroundColor; }
    Color get_border_color() const { return borderColor; }
    int get_font_size() const { return fontSize; }
    GuiTextAlignment get_text_alignment() const { return textAlignment; }
    GuiTextAlignmentVertical get_text_alignment_vertical() const { return textAlignmentVertical; }
    GuiTextWrapMode get_text_wrap_mode() const { return textWrapMode; }
    
    // Setters
    void set_bounds(const Rectangle& new_bounds) { bounds = new_bounds; }
    void set_state(GuiControlState new_state) { state = new_state; }
    void set_visible(bool vis) { visible = vis; }
    void set_enabled(bool en) { enabled = en; }
    void set_text(const std::string& new_text) { text = new_text; }
    void set_text_color(const Color& color) { textColor = color; }
    void set_background_color(const Color& color) { backgroundColor = color; }
    void set_border_color(const Color& color) { borderColor = color; }
    void set_font_size(int size) { fontSize = size; }
    void set_text_alignment(GuiTextAlignment alignment) { textAlignment = alignment; }
    void set_text_alignment_vertical(GuiTextAlignmentVertical alignment) { textAlignmentVertical = alignment; }
    void set_text_wrap_mode(GuiTextWrapMode mode) { textWrapMode = mode; }
    void set_event_callback(GuiEventCallback callback) { eventCallback = callback; }
    
    // Virtual methods
    virtual bool update() = 0;  // Returns true if control was interacted with
    virtual void draw() = 0;
    virtual void set_value(const std::string& /*value*/) {}
    virtual std::string get_value() const { return text; }
    
    // Event handling
    void trigger_event(GuiEventType event_type, const std::string& data = "") {
        if (eventCallback) {
            eventCallback(GuiEvent(event_type, id, data));
        }
    }
    
    // Utility methods
    bool contains_point(double x, double y) const {
        return x >= bounds.x && x <= bounds.x + bounds.width &&
               y >= bounds.y && y <= bounds.y + bounds.height;
    }
    
    void move_to(double x, double y) {
        bounds.x = x;
        bounds.y = y;
    }
    
    void resize(double width, double height) {
        bounds.width = width;
        bounds.height = height;
    }
};

} // namespace bas

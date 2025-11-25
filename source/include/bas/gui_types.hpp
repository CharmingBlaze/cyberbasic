#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

namespace bas {

// Forward declarations
struct Rectangle;
struct Color;
struct Vector2;

// GUI Control Types
enum class GuiControlType {
    BUTTON,
    LABEL,
    TEXTBOX,
    SLIDER,
    SLIDERBAR,
    CHECKBOX,
    COMBOBOX,
    DROPDOWNBOX,
    TOGGLE,
    TOGGLEGROUP,
    TOGGLESLIDER,
    VALUEBOX,
    SPINNER,
    PROGRESSBAR,
    STATUSBAR,
    LISTVIEW,
    COLORPICKER,
    MESSAGEBOX,
    TEXTINPUTBOX,
    WINDOWBOX,
    GROUPBOX,
    LINE,
    PANEL,
    SCROLLPANEL,
    TABBAR,
    DUMMYREC,
    GRID
};

// GUI Control State
enum class GuiControlState {
    NORMAL,
    FOCUSED,
    PRESSED,
    DISABLED
};

// GUI Text Alignment
enum class GuiTextAlignment {
    LEFT,
    CENTER,
    RIGHT
};

// GUI Text Alignment Vertical
enum class GuiTextAlignmentVertical {
    TOP,
    MIDDLE,
    BOTTOM
};

// GUI Text Wrap Mode
enum class GuiTextWrapMode {
    NONE,
    CHAR,
    WORD
};

// Basic GUI Types
struct Rectangle {
    double x, y, width, height;
    Rectangle(double px = 0, double py = 0, double w = 0, double h = 0) 
        : x(px), y(py), width(w), height(h) {}
};

struct Color {
    unsigned char r, g, b, a;
    Color(unsigned char red = 0, unsigned char green = 0, unsigned char blue = 0, unsigned char alpha = 255) 
        : r(red), g(green), b(blue), a(alpha) {}
};

struct Vector2 {
    double x, y;
    Vector2(double px = 0, double py = 0) : x(px), y(py) {}
};

// Event Types
enum class GuiEventType {
    CLICK,
    HOVER,
    FOCUS,
    BLUR,
    TEXT_CHANGED,
    VALUE_CHANGED,
    SELECTION_CHANGED,
    DROPDOWN_OPENED,
    DROPDOWN_CLOSED,
    WINDOW_CLOSED
};

struct GuiEvent {
    GuiEventType type;
    int control_id;
    std::string data;
    
    GuiEvent(GuiEventType t, int id, const std::string& d = "") 
        : type(t), control_id(id), data(d) {}
};

// Event Callback Type
using GuiEventCallback = std::function<void(const GuiEvent&)>;

} // namespace bas

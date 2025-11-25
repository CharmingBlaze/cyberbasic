#pragma once
#include "gui_control.hpp"

namespace bas {

// Basic Controls
class GuiButton : public GuiControl {
public:
    GuiButton(int id, const std::string& name, const Rectangle& bounds);
    bool update() override;
    void draw() override;
};

class GuiLabel : public GuiControl {
public:
    GuiLabel(int id, const std::string& name, const Rectangle& bounds);
    bool update() override;
    void draw() override;
};

class GuiTextBox : public GuiControl {
private:
    int maxLength;
    bool editMode;
    int cursorPosition;
    
public:
    GuiTextBox(int id, const std::string& name, const Rectangle& bounds, int max_len = 256);
    bool update() override;
    void draw() override;
    void set_value(const std::string& value) override;
    std::string get_value() const override;
    void set_edit_mode(bool edit) { editMode = edit; }
    bool is_edit_mode() const { return editMode; }
};

class GuiSlider : public GuiControl {
private:
    double minValue;
    double maxValue;
    double currentValue;
    
public:
    GuiSlider(int id, const std::string& name, const Rectangle& bounds, 
              double min_val, double max_val, double current_val);
    bool update() override;
    void draw() override;
    void set_value(const std::string& value) override;
    std::string get_value() const override;
    double get_numeric_value() const { return currentValue; }
    void set_numeric_value(double value) { currentValue = value; }
};

class GuiCheckBox : public GuiControl {
private:
    bool checked;
    
public:
    GuiCheckBox(int id, const std::string& name, const Rectangle& bounds, bool is_checked = false);
    bool update() override;
    void draw() override;
    void set_value(const std::string& value) override;
    std::string get_value() const override;
    bool is_checked() const { return checked; }
    void set_checked(bool check) { checked = check; }
};

class GuiComboBox : public GuiControl {
private:
    std::vector<std::string> items;
    int selectedIndex;
    bool dropdownOpen;
    
public:
    GuiComboBox(int id, const std::string& name, const Rectangle& bounds);
    bool update() override;
    void draw() override;
    void set_value(const std::string& value) override;
    std::string get_value() const override;
    void add_item(const std::string& item);
    void remove_item(int index);
    void clear_items();
    int get_selected_index() const { return selectedIndex; }
    void set_selected_index(int index);
    const std::vector<std::string>& get_items() const { return items; }
};

class GuiProgressBar : public GuiControl {
private:
    double minValue;
    double maxValue;
    double currentValue;
    
public:
    GuiProgressBar(int id, const std::string& name, const Rectangle& bounds,
                   double min_val, double max_val, double current_val);
    bool update() override;
    void draw() override;
    void set_value(const std::string& value) override;
    std::string get_value() const override;
    double get_numeric_value() const { return currentValue; }
    void set_numeric_value(double value) { currentValue = value; }
};

class GuiListView : public GuiControl {
private:
    std::vector<std::string> items;
    int selectedIndex;
    int scrollIndex;
    int itemsToShow;
    
public:
    GuiListView(int id, const std::string& name, const Rectangle& bounds);
    bool update() override;
    void draw() override;
    void set_value(const std::string& value) override;
    std::string get_value() const override;
    void add_item(const std::string& item);
    void remove_item(int index);
    void clear_items();
    int get_selected_index() const { return selectedIndex; }
    void set_selected_index(int index);
    const std::vector<std::string>& get_items() const { return items; }
};

// Container Controls
class GuiWindowBox : public GuiControl {
private:
    bool resizable;
    bool isDragging;
    Vector2 dragOffset;
    bool closeButton;
    bool draggable;
    
public:
    GuiWindowBox(int id, const std::string& name, const Rectangle& bounds);
    bool update() override;
    void draw() override;
    void set_close_button(bool show) { closeButton = show; }
    void set_draggable(bool drag) { draggable = drag; }
    void set_resizable(bool resize) { resizable = resize; }
};

class GuiPanel : public GuiControl {
public:
    GuiPanel(int id, const std::string& name, const Rectangle& bounds);
    bool update() override;
    void draw() override;
};

class GuiScrollPanel : public GuiControl {
private:
    Rectangle content;
    Vector2 scroll;
    
public:
    GuiScrollPanel(int id, const std::string& name, const Rectangle& bounds, const Rectangle& content_bounds);
    bool update() override;
    void draw() override;
    void set_content_bounds(const Rectangle& content_bounds) { content = content_bounds; }
    Rectangle get_content_bounds() const { return content; }
    Vector2 get_scroll() const { return scroll; }
    void set_scroll(const Vector2& new_scroll) { scroll = new_scroll; }
};

} // namespace bas

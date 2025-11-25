#include "bas/gui_controls.hpp"
#include "raylib.h"
#include <stdexcept>
#include <iostream>

namespace bas {

// Helper function to convert bas::Rectangle to raylib Rectangle
static ::Rectangle to_raylib_rect(const Rectangle& rect) {
    return {static_cast<float>(rect.x), static_cast<float>(rect.y), 
            static_cast<float>(rect.width), static_cast<float>(rect.height)};
}



// GuiButton Implementation
GuiButton::GuiButton(int id, const std::string& name, const Rectangle& bounds) 
    : GuiControl(id, name, GuiControlType::BUTTON, bounds) {
}

bool GuiButton::update() {
    if (!visible || !enabled) return false;
    
    ::Rectangle raylib_bounds = to_raylib_rect(bounds);
    
    // Simple button implementation using basic Raylib
    bool clicked = false;
    if (::IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        ::Vector2 mouse_pos = ::GetMousePosition();
        if (mouse_pos.x >= raylib_bounds.x && mouse_pos.x <= raylib_bounds.x + raylib_bounds.width &&
            mouse_pos.y >= raylib_bounds.y && mouse_pos.y <= raylib_bounds.y + raylib_bounds.height) {
            clicked = true;
        }
    }
    if (clicked) {
        trigger_event(GuiEventType::CLICK);
        return true;
    }
    return false;
}

void GuiButton::draw() {
    if (!visible) return;
    
    ::Rectangle raylib_bounds = to_raylib_rect(bounds);
    
    // Draw button using basic Raylib
    ::DrawRectangleRec(raylib_bounds, ::LIGHTGRAY);
    ::DrawRectangleLinesEx(raylib_bounds, 2, ::DARKGRAY);
    ::DrawText(text.c_str(), static_cast<int>(raylib_bounds.x + 5), 
               static_cast<int>(raylib_bounds.y + 5), 10, ::BLACK);
}

// GuiLabel Implementation
GuiLabel::GuiLabel(int id, const std::string& name, const Rectangle& bounds) 
    : GuiControl(id, name, GuiControlType::LABEL, bounds) {
}

bool GuiLabel::update() {
    // Labels don't interact, just return false
    return false;
}

void GuiLabel::draw() {
    if (!visible) return;
    
    ::Rectangle raylib_bounds = to_raylib_rect(bounds);
    
    // Draw label using basic Raylib
    ::DrawText(text.c_str(), static_cast<int>(raylib_bounds.x), 
               static_cast<int>(raylib_bounds.y), 10, ::BLACK);
}

// GuiTextBox Implementation
GuiTextBox::GuiTextBox(int id, const std::string& name, const Rectangle& bounds, int max_len) 
    : GuiControl(id, name, GuiControlType::TEXTBOX, bounds), maxLength(max_len), editMode(false), cursorPosition(0) {
}

bool GuiTextBox::update() {
    if (!visible || !enabled) {
        if (editMode) {
            editMode = false;
            trigger_event(GuiEventType::BLUR);
        }
        return false;
    }

    ::Rectangle raylib_bounds = to_raylib_rect(bounds);
    ::Vector2 mouse_pos = ::GetMousePosition();
    bool mouse_over = ::CheckCollisionPointRec(mouse_pos, raylib_bounds);

    if (::IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (mouse_over) {
            if (!editMode) {
                editMode = true;
                cursorPosition = text.length(); // Move cursor to end
                trigger_event(GuiEventType::FOCUS);
            }
        } else {
            if (editMode) {
                editMode = false;
                trigger_event(GuiEventType::BLUR);
            }
        }
    }

    if (editMode) {
        // Get pressed character (unicode)
        int key = ::GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (text.length() < static_cast<size_t>(maxLength))) {
                text.insert(cursorPosition, 1, static_cast<char>(key));
                cursorPosition++;
                trigger_event(GuiEventType::VALUE_CHANGED);
            }
            key = ::GetCharPressed(); // Check next character in queue
        }

        // Handle backspace (repeatedly)
        if (::IsKeyPressedRepeat(KEY_BACKSPACE) || ::IsKeyPressed(KEY_BACKSPACE)) {
            if (cursorPosition > 0) {
                text.erase(cursorPosition - 1, 1);
                cursorPosition--;
                trigger_event(GuiEventType::VALUE_CHANGED);
            }
        }

        // Handle Enter or Escape to lose focus
        if (::IsKeyPressed(KEY_ENTER) || ::IsKeyPressed(KEY_ESCAPE)) {
            editMode = false;
            trigger_event(GuiEventType::BLUR);
        }

        // Handle cursor movement
        if (::IsKeyPressed(KEY_LEFT)) {
            if (cursorPosition > 0) cursorPosition--;
        }
        if (::IsKeyPressed(KEY_RIGHT)) {
            if (cursorPosition < static_cast<int>(text.length())) cursorPosition++;
        }

        return true; // Input was processed
    }

    return false;
}

void GuiTextBox::draw() {
    if (!visible) return;
    
    ::Rectangle raylib_bounds = to_raylib_rect(bounds);
    
    // Draw textbox using basic raylib
    ::DrawRectangleRec(raylib_bounds, ::WHITE);
    ::DrawRectangleLinesEx(raylib_bounds, 1, editMode ? ::BLACK : ::DARKGRAY);
    ::DrawText(text.c_str(), static_cast<int>(raylib_bounds.x + 5), 
               static_cast<int>(raylib_bounds.y + (bounds.height / 2) - 5), 10, ::BLACK);

    // Draw blinking cursor
    if (editMode) {
        // Use a simple timer for blinking
        if (((int)(::GetTime() * 2.0f)) % 2 == 0) {
            // Measure text width up to cursor to find cursor's x position
            std::string text_before_cursor = text.substr(0, cursorPosition);
            int text_width = ::MeasureText(text_before_cursor.c_str(), 10);
            ::DrawRectangle(static_cast<int>(raylib_bounds.x + 5 + text_width), 
                            static_cast<int>(raylib_bounds.y + 4), 1, 
                            static_cast<int>(raylib_bounds.height - 8), ::BLACK);
        }
    }
}

void GuiTextBox::set_value(const std::string& value) {
    text = value;
    if (static_cast<int>(text.length()) > maxLength) {
        text = text.substr(0, maxLength);
    }
}

std::string GuiTextBox::get_value() const {
    return text;
}

// GuiSlider Implementation
GuiSlider::GuiSlider(int id, const std::string& name, const Rectangle& bounds, 
                     double min_val, double max_val, double current_val) 
    : GuiControl(id, name, GuiControlType::SLIDER, bounds), 
      minValue(min_val), maxValue(max_val), currentValue(current_val) {
}

bool GuiSlider::update() {
    if (!visible || !enabled) return false;

    ::Rectangle raylib_bounds = to_raylib_rect(bounds);
    ::Vector2 mouse_pos = ::GetMousePosition();

    if (::IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (::CheckCollisionPointRec(mouse_pos, raylib_bounds)) {
            double new_value = ((mouse_pos.x - raylib_bounds.x) / raylib_bounds.width) * (maxValue - minValue) + minValue;
            
            if (new_value < minValue) new_value = minValue;
            if (new_value > maxValue) new_value = maxValue;

            if (new_value != currentValue) {
                currentValue = new_value;
                trigger_event(GuiEventType::VALUE_CHANGED);
                return true;
            }
        }
    }
    return false;
}

void GuiSlider::draw() {
    if (!visible) return;
    ::Rectangle raylib_bounds = to_raylib_rect(bounds);
    
    // Draw slider bar
    ::DrawRectangleRec({raylib_bounds.x, raylib_bounds.y + raylib_bounds.height / 2 - 2, raylib_bounds.width, 4}, ::LIGHTGRAY);
    
    // Draw filled part
    double progress = (currentValue - minValue) / (maxValue - minValue);
    float filled_width = static_cast<float>(progress * raylib_bounds.width);
    ::DrawRectangleRec({raylib_bounds.x, raylib_bounds.y + raylib_bounds.height / 2 - 2, filled_width, 4}, ::BLUE);

    // Draw slider handle
    ::Rectangle handle_rect = { raylib_bounds.x + filled_width - 5, raylib_bounds.y, 10, raylib_bounds.height };
    ::DrawRectangleRec(handle_rect, ::DARKBLUE);

    // Draw value text
    std::string val_str = std::to_string(static_cast<int>(currentValue));
    ::DrawText(val_str.c_str(), static_cast<int>(raylib_bounds.x + raylib_bounds.width + 5), static_cast<int>(raylib_bounds.y), 10, ::BLACK);
}

void GuiSlider::set_value(const std::string& value) {
    try {
        currentValue = std::stod(value);
        if (currentValue < minValue) currentValue = minValue;
        if (currentValue > maxValue) currentValue = maxValue;
    } catch (...) {
        // Invalid value, keep current
    }
}

std::string GuiSlider::get_value() const {
    return std::to_string(currentValue);
}

// GuiCheckBox Implementation
GuiCheckBox::GuiCheckBox(int id, const std::string& name, const Rectangle& bounds, bool is_checked) 
    : GuiControl(id, name, GuiControlType::CHECKBOX, bounds), checked(is_checked) {
}

bool GuiCheckBox::update() {
    if (!visible || !enabled) return false;

    ::Rectangle raylib_bounds = to_raylib_rect(bounds);
    ::Vector2 mouse_pos = ::GetMousePosition();

    if (::IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (::CheckCollisionPointRec(mouse_pos, raylib_bounds)) {
            checked = !checked;
            trigger_event(GuiEventType::VALUE_CHANGED);
            return true;
        }
    }
    return false;
}

void GuiCheckBox::draw() {
    if (!visible) return;
    
    ::Rectangle raylib_bounds = to_raylib_rect(bounds);
    
    // Draw checkbox
    ::DrawRectangleLinesEx(raylib_bounds, 1, ::DARKGRAY);
    if (checked) {
        ::DrawRectangleRec({raylib_bounds.x + 2, raylib_bounds.y + 2, raylib_bounds.width - 4, raylib_bounds.height - 4}, ::BLUE);
    }

    // Draw text label beside the checkbox
    ::DrawText(text.c_str(), static_cast<int>(raylib_bounds.x + raylib_bounds.width + 5), 
               static_cast<int>(raylib_bounds.y), 10, ::BLACK);
}

void GuiCheckBox::set_value(const std::string& value) {
    checked = (value == "true" || value == "1" || value == "yes");
}

std::string GuiCheckBox::get_value() const {
    return checked ? "true" : "false";
}

// GuiComboBox Implementation
GuiComboBox::GuiComboBox(int id, const std::string& name, const Rectangle& bounds) 
    : GuiControl(id, name, GuiControlType::COMBOBOX, bounds), selectedIndex(0), dropdownOpen(false) {
}

bool GuiComboBox::update() {
    if (!visible || !enabled) {
        dropdownOpen = false;
        return false;
    }

    ::Rectangle raylib_bounds = to_raylib_rect(bounds);
    ::Vector2 mouse_pos = ::GetMousePosition();
    bool mouse_over_main = ::CheckCollisionPointRec(mouse_pos, raylib_bounds);

    if (::IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (mouse_over_main) {
            dropdownOpen = !dropdownOpen;
            trigger_event(dropdownOpen ? GuiEventType::DROPDOWN_OPENED : GuiEventType::DROPDOWN_CLOSED);
            return true;
        } else if (dropdownOpen) {
            // Check for selection in dropdown list
            ::Rectangle dropdown_bounds = { raylib_bounds.x, raylib_bounds.y + raylib_bounds.height, raylib_bounds.width, static_cast<float>(items.size() * 20) };
            if (::CheckCollisionPointRec(mouse_pos, dropdown_bounds)) {
                int hover_index = static_cast<int>((mouse_pos.y - (raylib_bounds.y + raylib_bounds.height)) / 20);
                if (hover_index >= 0 && hover_index < static_cast<int>(items.size())) {
                    if (selectedIndex != hover_index) {
                        selectedIndex = hover_index;
                        trigger_event(GuiEventType::VALUE_CHANGED);
                    }
                    dropdownOpen = false;
                    trigger_event(GuiEventType::DROPDOWN_CLOSED);
                    return true;
                }
            } else {
                // Clicked outside, close dropdown
                dropdownOpen = false;
                trigger_event(GuiEventType::DROPDOWN_CLOSED);
            }
        }
    }
    return false;
}

void GuiComboBox::draw() {
    if (!visible) return;
    
    ::Rectangle raylib_bounds = to_raylib_rect(bounds);
    
    // Draw main combo box
    ::DrawRectangleRec(raylib_bounds, ::LIGHTGRAY);
    ::DrawRectangleLinesEx(raylib_bounds, 1, ::DARKGRAY);
    std::string display_text = (selectedIndex >= 0 && selectedIndex < static_cast<int>(items.size())) ? items[selectedIndex] : "";
    ::DrawText(display_text.c_str(), static_cast<int>(raylib_bounds.x + 5), 
               static_cast<int>(raylib_bounds.y + raylib_bounds.height / 2 - 5), 10, ::BLACK);

    // Draw dropdown arrow
    ::DrawTriangle({ raylib_bounds.x + raylib_bounds.width - 15, raylib_bounds.y + raylib_bounds.height / 2 - 2 },
                   { raylib_bounds.x + raylib_bounds.width - 5, raylib_bounds.y + raylib_bounds.height / 2 - 2 },
                   { raylib_bounds.x + raylib_bounds.width - 10, raylib_bounds.y + raylib_bounds.height / 2 + 3 }, ::BLACK);

    // Draw dropdown list if open
    if (dropdownOpen) {
        ::Rectangle dropdown_bounds = { raylib_bounds.x, raylib_bounds.y + raylib_bounds.height, raylib_bounds.width, static_cast<float>(items.size() * 20) };
        ::DrawRectangleRec(dropdown_bounds, ::WHITE);
        ::DrawRectangleLinesEx(dropdown_bounds, 1, ::DARKGRAY);

        for (size_t i = 0; i < items.size(); ++i) {
            ::Rectangle item_bounds = { dropdown_bounds.x, dropdown_bounds.y + (i * 20), dropdown_bounds.width, 20 };
            if (::CheckCollisionPointRec(::GetMousePosition(), item_bounds)) {
                ::DrawRectangleRec(item_bounds, ::LIGHTGRAY);
            }
            if (static_cast<int>(i) == selectedIndex) {
                ::DrawRectangleLinesEx(item_bounds, 1, ::BLUE);
            }
            ::DrawText(items[i].c_str(), static_cast<int>(item_bounds.x + 5), static_cast<int>(item_bounds.y + 5), 10, ::BLACK);
        }
    }
}

void GuiComboBox::set_value(const std::string& value) {
    // Try to find the item by value
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] == value) {
            selectedIndex = static_cast<int>(i);
            return;
        }
    }
}

std::string GuiComboBox::get_value() const {
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(items.size())) {
        return items[selectedIndex];
    }
    return "";
}

void GuiComboBox::add_item(const std::string& item) {
    items.push_back(item);
}

void GuiComboBox::remove_item(int index) {
    if (index >= 0 && index < static_cast<int>(items.size())) {
        items.erase(items.begin() + index);
        if (selectedIndex >= static_cast<int>(items.size())) {
            selectedIndex = static_cast<int>(items.size()) - 1;
        }
    }
}

void GuiComboBox::clear_items() {
    items.clear();
    selectedIndex = 0;
}

void GuiComboBox::set_selected_index(int index) {
    if (index >= 0 && index < static_cast<int>(items.size())) {
        selectedIndex = index;
    }
}

// GuiProgressBar Implementation
GuiProgressBar::GuiProgressBar(int id, const std::string& name, const Rectangle& bounds,
                               double min_val, double max_val, double current_val) 
    : GuiControl(id, name, GuiControlType::PROGRESSBAR, bounds), 
      minValue(min_val), maxValue(max_val), currentValue(current_val) {
}

bool GuiProgressBar::update() {
    // Progress bars don't interact, just return false
    return false;
}

void GuiProgressBar::draw() {
    if (!visible) return;

    ::Rectangle raylib_bounds = to_raylib_rect(bounds);

    // Draw background
    ::DrawRectangleRec(raylib_bounds, ::LIGHTGRAY);
    ::DrawRectangleLinesEx(raylib_bounds, 1, ::DARKGRAY);

    // Draw progress bar fill
    float progress = (currentValue - minValue) / (maxValue - minValue);
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;
    ::Rectangle progress_bounds = { raylib_bounds.x + 2, raylib_bounds.y + 2, (raylib_bounds.width - 4) * progress, raylib_bounds.height - 4 };
    ::DrawRectangleRec(progress_bounds, ::BLUE);

    // Draw text
    std::string progress_text = std::to_string(static_cast<int>(currentValue));
    int text_width = ::MeasureText(progress_text.c_str(), 10);
    ::DrawText(progress_text.c_str(), static_cast<int>(raylib_bounds.x + raylib_bounds.width / 2 - text_width / 2), static_cast<int>(raylib_bounds.y + raylib_bounds.height / 2 - 5), 10, ::BLACK);
}

void GuiProgressBar::set_value(const std::string& value) {
    try {
        currentValue = std::stod(value);
        if (currentValue < minValue) currentValue = minValue;
        if (currentValue > maxValue) currentValue = maxValue;
    } catch (...) {
        // Invalid value, keep current
    }
}

std::string GuiProgressBar::get_value() const {
    return std::to_string(currentValue);
}

// GuiListView Implementation
GuiListView::GuiListView(int id, const std::string& name, const Rectangle& bounds) 
    : GuiControl(id, name, GuiControlType::LISTVIEW, bounds), selectedIndex(0), scrollIndex(0), itemsToShow(5) {
}

bool GuiListView::update() {
    if (!visible || !enabled) return false;

    ::Rectangle raylib_bounds = to_raylib_rect(bounds);
    ::Vector2 mouse_pos = ::GetMousePosition();
    bool mouse_over = ::CheckCollisionPointRec(mouse_pos, raylib_bounds);

    if (mouse_over) {
        // Handle mouse wheel scrolling
        float wheel_move = ::GetMouseWheelMove();
        if (wheel_move != 0) {
            scrollIndex -= static_cast<int>(wheel_move);
            if (scrollIndex < 0) scrollIndex = 0;
            if (scrollIndex > static_cast<int>(items.size()) - itemsToShow) {
                scrollIndex = static_cast<int>(items.size()) - itemsToShow;
            }
        }

        // Handle item selection
        if (::IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int hover_index = scrollIndex + static_cast<int>((mouse_pos.y - raylib_bounds.y) / 20);
            if (hover_index >= 0 && hover_index < static_cast<int>(items.size())) {
                if (selectedIndex != hover_index) {
                    selectedIndex = hover_index;
                    trigger_event(GuiEventType::VALUE_CHANGED);
                    return true;
                }
            }
        }
    }
    return false;
}

void GuiListView::draw() {
    if (!visible) return;
    
    ::Rectangle raylib_bounds = to_raylib_rect(bounds);
    itemsToShow = static_cast<int>(raylib_bounds.height / 20);

    // Draw list view background
    ::DrawRectangleRec(raylib_bounds, ::WHITE);
    ::DrawRectangleLinesEx(raylib_bounds, 1, ::DARKGRAY);

    // Begin scissoring to draw only within the bounds
    ::BeginScissorMode(static_cast<int>(raylib_bounds.x), static_cast<int>(raylib_bounds.y), static_cast<int>(raylib_bounds.width), static_cast<int>(raylib_bounds.height));

    // Draw items
    for (int i = 0; i < static_cast<int>(items.size()); ++i) {
        if (i >= scrollIndex && i < scrollIndex + itemsToShow) {
            ::Rectangle item_bounds = { raylib_bounds.x, raylib_bounds.y + (i - scrollIndex) * 20, raylib_bounds.width, 20 };
            
            // Draw selection highlight
            if (i == selectedIndex) {
                ::DrawRectangleRec(item_bounds, ::LIGHTGRAY);
            }

            ::DrawText(items[i].c_str(), static_cast<int>(item_bounds.x + 5), static_cast<int>(item_bounds.y + 5), 10, ::BLACK);
        }
    }

    ::EndScissorMode();
}

void GuiListView::set_value(const std::string& value) {
    // Try to find the item by value
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] == value) {
            selectedIndex = static_cast<int>(i);
            return;
        }
    }
}

std::string GuiListView::get_value() const {
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(items.size())) {
        return items[selectedIndex];
    }
    return "";
}

void GuiListView::add_item(const std::string& item) {
    items.push_back(item);
}

void GuiListView::remove_item(int index) {
    if (index >= 0 && index < static_cast<int>(items.size())) {
        items.erase(items.begin() + index);
        if (selectedIndex >= static_cast<int>(items.size())) {
            selectedIndex = static_cast<int>(items.size()) - 1;
        }
    }
}

void GuiListView::clear_items() {
    items.clear();
    selectedIndex = 0;
    scrollIndex = 0;
}

void GuiListView::set_selected_index(int index) {
    if (index >= 0 && index < static_cast<int>(items.size())) {
        selectedIndex = index;
    }
}

// GuiWindowBox Implementation
GuiWindowBox::GuiWindowBox(int id, const std::string& name, const Rectangle& bounds) 
    : GuiControl(id, name, GuiControlType::WINDOWBOX, bounds), resizable(false), isDragging(false), dragOffset({0, 0}), closeButton(true), draggable(true) {
}

bool GuiWindowBox::update() {
    if (!visible || !enabled) {
        if (isDragging) isDragging = false;
        return false;
    }

    ::Vector2 mouse_pos = ::GetMousePosition();
    ::Rectangle title_bar_bounds = { static_cast<float>(bounds.x), static_cast<float>(bounds.y), static_cast<float>(bounds.width), 25.0f };

    if (draggable) {
        if (::IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (::CheckCollisionPointRec(mouse_pos, title_bar_bounds)) {
                isDragging = true;
                dragOffset = { mouse_pos.x - bounds.x, mouse_pos.y - bounds.y };
            }
        }

        if (isDragging) {
            if (::IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                bounds.x = mouse_pos.x - dragOffset.x;
                bounds.y = mouse_pos.y - dragOffset.y;
            } else {
                isDragging = false;
            }
            return true; // Window is being moved
        }
    }

    // Handle close button
    if (closeButton) {
        ::Rectangle close_button_bounds = { static_cast<float>(bounds.x + bounds.width - 20), static_cast<float>(bounds.y + 5), 15, 15 };
        if (::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ::CheckCollisionPointRec(mouse_pos, close_button_bounds)) {
            visible = false; // Or trigger a close event
            trigger_event(GuiEventType::CLICK);
            return true;
        }
    }

    return false;
}

void GuiWindowBox::draw() {
    if (!visible) return;

    ::Rectangle raylib_bounds = to_raylib_rect(bounds);

    // Draw window body
    ::DrawRectangleRec(raylib_bounds, {245, 245, 245, 255}); // A light grey
    ::DrawRectangleLinesEx(raylib_bounds, 1, ::DARKGRAY);

    // Draw title bar
    ::Rectangle title_bar_bounds = { raylib_bounds.x, raylib_bounds.y, raylib_bounds.width, 25.0f };
    ::DrawRectangleRec(title_bar_bounds, ::GRAY);
    ::DrawText(text.c_str(), static_cast<int>(raylib_bounds.x + 5), static_cast<int>(raylib_bounds.y + 8), 10, ::WHITE);

    // Draw close button
    if (closeButton) {
        ::Rectangle close_button_bounds = { raylib_bounds.x + raylib_bounds.width - 20, raylib_bounds.y + 5, 15, 15 };
        bool mouse_over_close = ::CheckCollisionPointRec(::GetMousePosition(), close_button_bounds);
        ::DrawRectangleRec(close_button_bounds, mouse_over_close ? ::RED : ::MAROON);
        ::DrawText("x", static_cast<int>(close_button_bounds.x + 4), static_cast<int>(close_button_bounds.y + 2), 10, ::WHITE);
    }
}

// GuiPanel Implementation
GuiPanel::GuiPanel(int id, const std::string& name, const Rectangle& bounds) 
    : GuiControl(id, name, GuiControlType::PANEL, bounds) {
}

bool GuiPanel::update() {
    // Panels don't interact, just return false
    return false;
}

void GuiPanel::draw() {
    if (!visible) return;
    
    ::Rectangle raylib_bounds = to_raylib_rect(bounds);
    
    // Draw panel using basic raylib
    ::DrawRectangleRec(raylib_bounds, ::LIGHTGRAY);
    ::DrawRectangleLinesEx(raylib_bounds, 1, ::DARKGRAY);
}

// GuiScrollPanel Implementation
GuiScrollPanel::GuiScrollPanel(int id, const std::string& name, const Rectangle& bounds, const Rectangle& content_bounds) 
    : GuiControl(id, name, GuiControlType::SCROLLPANEL, bounds), content(content_bounds), scroll(0, 0) {
}

bool GuiScrollPanel::update() {
    if (!visible || !enabled) return false;
    
    // Use basic drawing for now - implement proper scroll panel later
    return false;
}

void GuiScrollPanel::draw() {
    if (!visible) return;
    
    ::Rectangle raylib_bounds = to_raylib_rect(bounds);
    
    // Draw a simple rectangle for now
    ::DrawRectangleRec(raylib_bounds, ::LIGHTGRAY);
    ::DrawText(text.c_str(), static_cast<int>(raylib_bounds.x + 5), 
               static_cast<int>(raylib_bounds.y + 5), 10, ::BLACK);
}

} // namespace bas
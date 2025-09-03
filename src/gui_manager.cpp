#include "bas/gui_manager.hpp"
#include "bas/gui_controls.hpp"
#include "raylib.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>

namespace bas {

// Static member initialization
int GuiManager::next_control_id = 1;
std::unordered_map<int, std::shared_ptr<GuiControl>> GuiManager::controls;
std::unordered_map<std::string, int> GuiManager::control_lookup;
std::vector<int> GuiManager::control_order;
int GuiManager::focused_control_id = -1;
bool GuiManager::system_initialized = false;
GuiEventCallback GuiManager::global_event_callback;

// System management
bool GuiManager::initialize() {
    if (system_initialized) return true;
    
    // Initialize raygui if needed
    // Note: raygui doesn't require explicit initialization
    
    system_initialized = true;
    std::cout << "GuiManager initialized." << std::endl;
    return true;
}

void GuiManager::shutdown() {
    if (!system_initialized) return;
    
    destroy_all_controls();
    system_initialized = false;
    std::cout << "GuiManager shutdown." << std::endl;
}

// Control management
int GuiManager::create_button(const std::string& name, int x, int y, int width, int height, const std::string& text) {
    if (!system_initialized) throw std::runtime_error("GuiManager not initialized");
    
    int id = next_control_id++;
    Rectangle bounds(x, y, width, height);
    auto control = std::make_shared<GuiButton>(id, name, bounds);
    control->set_text(text);
    
    controls[id] = control;
    control_lookup[name] = id;
    control_order.push_back(id);
    
    return id;
}

int GuiManager::create_label(const std::string& name, int x, int y, int width, int height, const std::string& text) {
    if (!system_initialized) throw std::runtime_error("GuiManager not initialized");
    
    int id = next_control_id++;
    Rectangle bounds(x, y, width, height);
    auto control = std::make_shared<GuiLabel>(id, name, bounds);
    control->set_text(text);
    
    controls[id] = control;
    control_lookup[name] = id;
    control_order.push_back(id);
    
    return id;
}

int GuiManager::create_textbox(const std::string& name, int x, int y, int width, int height, const std::string& text, int max_length) {
    if (!system_initialized) throw std::runtime_error("GuiManager not initialized");
    
    int id = next_control_id++;
    Rectangle bounds(x, y, width, height);
    auto control = std::make_shared<GuiTextBox>(id, name, bounds, max_length);
    control->set_text(text);
    
    controls[id] = control;
    control_lookup[name] = id;
    control_order.push_back(id);
    
    return id;
}

int GuiManager::create_slider(const std::string& name, int x, int y, int width, int height, double min_val, double max_val, double current_val) {
    if (!system_initialized) throw std::runtime_error("GuiManager not initialized");
    
    int id = next_control_id++;
    Rectangle bounds(x, y, width, height);
    auto control = std::make_shared<GuiSlider>(id, name, bounds, min_val, max_val, current_val);
    
    controls[id] = control;
    control_lookup[name] = id;
    control_order.push_back(id);
    
    return id;
}

int GuiManager::create_checkbox(const std::string& name, int x, int y, int width, int height, const std::string& text, bool checked) {
    if (!system_initialized) throw std::runtime_error("GuiManager not initialized");
    
    int id = next_control_id++;
    Rectangle bounds(x, y, width, height);
    auto control = std::make_shared<GuiCheckBox>(id, name, bounds, checked);
    control->set_text(text);
    
    controls[id] = control;
    control_lookup[name] = id;
    control_order.push_back(id);
    
    return id;
}

int GuiManager::create_combobox(const std::string& name, int x, int y, int width, int height) {
    if (!system_initialized) throw std::runtime_error("GuiManager not initialized");
    
    int id = next_control_id++;
    Rectangle bounds(x, y, width, height);
    auto control = std::make_shared<GuiComboBox>(id, name, bounds);
    
    controls[id] = control;
    control_lookup[name] = id;
    control_order.push_back(id);
    
    return id;
}

int GuiManager::create_progressbar(const std::string& name, int x, int y, int width, int height, double min_val, double max_val, double current_val) {
    if (!system_initialized) throw std::runtime_error("GuiManager not initialized");
    
    int id = next_control_id++;
    Rectangle bounds(x, y, width, height);
    auto control = std::make_shared<GuiProgressBar>(id, name, bounds, min_val, max_val, current_val);
    
    controls[id] = control;
    control_lookup[name] = id;
    control_order.push_back(id);
    
    return id;
}

int GuiManager::create_listview(const std::string& name, int x, int y, int width, int height) {
    if (!system_initialized) throw std::runtime_error("GuiManager not initialized");
    
    int id = next_control_id++;
    Rectangle bounds(x, y, width, height);
    auto control = std::make_shared<GuiListView>(id, name, bounds);
    
    controls[id] = control;
    control_lookup[name] = id;
    control_order.push_back(id);
    
    return id;
}

int GuiManager::create_windowbox(const std::string& name, int x, int y, int width, int height, const std::string& title) {
    if (!system_initialized) throw std::runtime_error("GuiManager not initialized");
    
    int id = next_control_id++;
    Rectangle bounds(x, y, width, height);
    auto control = std::make_shared<GuiWindowBox>(id, name, bounds);
    control->set_text(title);
    
    controls[id] = control;
    control_lookup[name] = id;
    control_order.push_back(id);
    
    return id;
}

int GuiManager::create_panel(const std::string& name, int x, int y, int width, int height) {
    if (!system_initialized) throw std::runtime_error("GuiManager not initialized");
    
    int id = next_control_id++;
    Rectangle bounds(x, y, width, height);
    auto control = std::make_shared<GuiPanel>(id, name, bounds);
    
    controls[id] = control;
    control_lookup[name] = id;
    control_order.push_back(id);
    
    return id;
}

int GuiManager::create_scrollpanel(const std::string& name, int x, int y, int width, int height, int content_width, int content_height) {
    if (!system_initialized) throw std::runtime_error("GuiManager not initialized");
    
    int id = next_control_id++;
    Rectangle bounds(x, y, width, height);
    Rectangle content_bounds(0, 0, content_width, content_height);
    auto control = std::make_shared<GuiScrollPanel>(id, name, bounds, content_bounds);
    
    controls[id] = control;
    control_lookup[name] = id;
    control_order.push_back(id);
    
    return id;
}

// Control access
std::shared_ptr<GuiControl> GuiManager::get_control(int control_id) {
    auto it = controls.find(control_id);
    if (it != controls.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<GuiControl> GuiManager::get_control(const std::string& name) {
    auto it = control_lookup.find(name);
    if (it != control_lookup.end()) {
        return get_control(it->second);
    }
    return nullptr;
}

void GuiManager::destroy_control(int control_id) {
    auto it = controls.find(control_id);
    if (it != controls.end()) {
        controls.erase(it);
        
        // Remove from lookup
        for (auto lookup_it = control_lookup.begin(); lookup_it != control_lookup.end(); ++lookup_it) {
            if (lookup_it->second == control_id) {
                control_lookup.erase(lookup_it);
                break;
            }
        }
        
        // Remove from order
        control_order.erase(std::remove(control_order.begin(), control_order.end(), control_id), control_order.end());
        
        if (focused_control_id == control_id) {
            focused_control_id = -1;
        }
    }
}

void GuiManager::destroy_control(const std::string& name) {
    auto it = control_lookup.find(name);
    if (it != control_lookup.end()) {
        destroy_control(it->second);
    }
}

void GuiManager::destroy_all_controls() {
    controls.clear();
    control_lookup.clear();
    control_order.clear();
    focused_control_id = -1;
}

// Control properties
void GuiManager::set_control_text(int control_id, const std::string& text) {
    auto control = get_control(control_id);
    if (control) {
        control->set_text(text);
    }
}

void GuiManager::set_control_text(const std::string& name, const std::string& text) {
    auto control = get_control(name);
    if (control) {
        control->set_text(text);
    }
}

std::string GuiManager::get_control_text(int control_id) {
    auto control = get_control(control_id);
    if (control) {
        return control->get_text();
    }
    return "";
}

std::string GuiManager::get_control_text(const std::string& name) {
    auto control = get_control(name);
    if (control) {
        return control->get_text();
    }
    return "";
}

void GuiManager::set_control_value(int control_id, const std::string& value) {
    auto control = get_control(control_id);
    if (control) {
        control->set_value(value);
    }
}

void GuiManager::set_control_value(const std::string& name, const std::string& value) {
    auto control = get_control(name);
    if (control) {
        control->set_value(value);
    }
}

std::string GuiManager::get_control_value(int control_id) {
    auto control = get_control(control_id);
    if (control) {
        return control->get_value();
    }
    return "";
}

std::string GuiManager::get_control_value(const std::string& name) {
    auto control = get_control(name);
    if (control) {
        return control->get_value();
    }
    return "";
}

void GuiManager::set_control_position(int control_id, int x, int y) {
    auto control = get_control(control_id);
    if (control) {
        control->move_to(x, y);
    }
}

void GuiManager::set_control_position(const std::string& name, int x, int y) {
    auto control = get_control(name);
    if (control) {
        control->move_to(x, y);
    }
}

void GuiManager::set_control_size(int control_id, int width, int height) {
    auto control = get_control(control_id);
    if (control) {
        control->resize(width, height);
    }
}

void GuiManager::set_control_size(const std::string& name, int width, int height) {
    auto control = get_control(name);
    if (control) {
        control->resize(width, height);
    }
}

void GuiManager::set_control_visible(int control_id, bool visible) {
    auto control = get_control(control_id);
    if (control) {
        control->set_visible(visible);
    }
}

void GuiManager::set_control_visible(const std::string& name, bool visible) {
    auto control = get_control(name);
    if (control) {
        control->set_visible(visible);
    }
}

void GuiManager::set_control_enabled(int control_id, bool enabled) {
    auto control = get_control(control_id);
    if (control) {
        control->set_enabled(enabled);
    }
}

void GuiManager::set_control_enabled(const std::string& name, bool enabled) {
    auto control = get_control(name);
    if (control) {
        control->set_enabled(enabled);
    }
}

// System update and rendering
void GuiManager::update_all_controls() {
    if (!system_initialized) return;
    
    for (int control_id : control_order) {
        auto control = get_control(control_id);
        if (control && control->is_visible() && control->is_enabled()) {
            control->update();
        }
    }
}

void GuiManager::draw_all_controls() {
    if (!system_initialized) return;
    
    for (int control_id : control_order) {
        auto control = get_control(control_id);
        if (control && control->is_visible()) {
            control->draw();
        }
    }
}

void GuiManager::set_focused_control(int control_id) {
    focused_control_id = control_id;
}

// Specialized control methods
void GuiManager::add_combobox_item(int control_id, const std::string& item) {
    auto control = get_control(control_id);
    if (control && control->get_type() == GuiControlType::COMBOBOX) {
        auto combobox = std::dynamic_pointer_cast<GuiComboBox>(control);
        if (combobox) {
            combobox->add_item(item);
        }
    }
}

void GuiManager::add_combobox_item(const std::string& name, const std::string& item) {
    auto control = get_control(name);
    if (control && control->get_type() == GuiControlType::COMBOBOX) {
        auto combobox = std::dynamic_pointer_cast<GuiComboBox>(control);
        if (combobox) {
            combobox->add_item(item);
        }
    }
}

void GuiManager::clear_combobox_items(int control_id) {
    auto control = get_control(control_id);
    if (control && control->get_type() == GuiControlType::COMBOBOX) {
        auto combobox = std::dynamic_pointer_cast<GuiComboBox>(control);
        if (combobox) {
            combobox->clear_items();
        }
    }
}

void GuiManager::clear_combobox_items(const std::string& name) {
    auto control = get_control(name);
    if (control && control->get_type() == GuiControlType::COMBOBOX) {
        auto combobox = std::dynamic_pointer_cast<GuiComboBox>(control);
        if (combobox) {
            combobox->clear_items();
        }
    }
}

int GuiManager::get_combobox_selected_index(int control_id) {
    auto control = get_control(control_id);
    if (control && control->get_type() == GuiControlType::COMBOBOX) {
        auto combobox = std::dynamic_pointer_cast<GuiComboBox>(control);
        if (combobox) {
            return combobox->get_selected_index();
        }
    }
    return -1;
}

int GuiManager::get_combobox_selected_index(const std::string& name) {
    auto control = get_control(name);
    if (control && control->get_type() == GuiControlType::COMBOBOX) {
        auto combobox = std::dynamic_pointer_cast<GuiComboBox>(control);
        if (combobox) {
            return combobox->get_selected_index();
        }
    }
    return -1;
}

void GuiManager::set_combobox_selected_index(int control_id, int index) {
    auto control = get_control(control_id);
    if (control && control->get_type() == GuiControlType::COMBOBOX) {
        auto combobox = std::dynamic_pointer_cast<GuiComboBox>(control);
        if (combobox) {
            combobox->set_selected_index(index);
        }
    }
}

void GuiManager::set_combobox_selected_index(const std::string& name, int index) {
    auto control = get_control(name);
    if (control && control->get_type() == GuiControlType::COMBOBOX) {
        auto combobox = std::dynamic_pointer_cast<GuiComboBox>(control);
        if (combobox) {
            combobox->set_selected_index(index);
        }
    }
}

void GuiManager::add_listview_item(int control_id, const std::string& item) {
    auto control = get_control(control_id);
    if (control && control->get_type() == GuiControlType::LISTVIEW) {
        auto listview = std::dynamic_pointer_cast<GuiListView>(control);
        if (listview) {
            listview->add_item(item);
        }
    }
}

void GuiManager::add_listview_item(const std::string& name, const std::string& item) {
    auto control = get_control(name);
    if (control && control->get_type() == GuiControlType::LISTVIEW) {
        auto listview = std::dynamic_pointer_cast<GuiListView>(control);
        if (listview) {
            listview->add_item(item);
        }
    }
}

void GuiManager::clear_listview_items(int control_id) {
    auto control = get_control(control_id);
    if (control && control->get_type() == GuiControlType::LISTVIEW) {
        auto listview = std::dynamic_pointer_cast<GuiListView>(control);
        if (listview) {
            listview->clear_items();
        }
    }
}

void GuiManager::clear_listview_items(const std::string& name) {
    auto control = get_control(name);
    if (control && control->get_type() == GuiControlType::LISTVIEW) {
        auto listview = std::dynamic_pointer_cast<GuiListView>(control);
        if (listview) {
            listview->clear_items();
        }
    }
}

int GuiManager::get_listview_selected_index(int control_id) {
    auto control = get_control(control_id);
    if (control && control->get_type() == GuiControlType::LISTVIEW) {
        auto listview = std::dynamic_pointer_cast<GuiListView>(control);
        if (listview) {
            return listview->get_selected_index();
        }
    }
    return -1;
}

int GuiManager::get_listview_selected_index(const std::string& name) {
    auto control = get_control(name);
    if (control && control->get_type() == GuiControlType::LISTVIEW) {
        auto listview = std::dynamic_pointer_cast<GuiListView>(control);
        if (listview) {
            return listview->get_selected_index();
        }
    }
    return -1;
}

void GuiManager::set_listview_selected_index(int control_id, int index) {
    auto control = get_control(control_id);
    if (control && control->get_type() == GuiControlType::LISTVIEW) {
        auto listview = std::dynamic_pointer_cast<GuiListView>(control);
        if (listview) {
            listview->set_selected_index(index);
        }
    }
}

void GuiManager::set_listview_selected_index(const std::string& name, int index) {
    auto control = get_control(name);
    if (control && control->get_type() == GuiControlType::LISTVIEW) {
        auto listview = std::dynamic_pointer_cast<GuiListView>(control);
        if (listview) {
            listview->set_selected_index(index);
        }
    }
}

bool GuiManager::is_checkbox_checked(int control_id) {
    auto control = get_control(control_id);
    if (control && control->get_type() == GuiControlType::CHECKBOX) {
        auto checkbox = std::dynamic_pointer_cast<GuiCheckBox>(control);
        if (checkbox) {
            return checkbox->is_checked();
        }
    }
    return false;
}

bool GuiManager::is_checkbox_checked(const std::string& name) {
    auto control = get_control(name);
    if (control && control->get_type() == GuiControlType::CHECKBOX) {
        auto checkbox = std::dynamic_pointer_cast<GuiCheckBox>(control);
        if (checkbox) {
            return checkbox->is_checked();
        }
    }
    return false;
}

void GuiManager::set_checkbox_checked(int control_id, bool checked) {
    auto control = get_control(control_id);
    if (control && control->get_type() == GuiControlType::CHECKBOX) {
        auto checkbox = std::dynamic_pointer_cast<GuiCheckBox>(control);
        if (checkbox) {
            checkbox->set_checked(checked);
        }
    }
}

void GuiManager::set_checkbox_checked(const std::string& name, bool checked) {
    auto control = get_control(name);
    if (control && control->get_type() == GuiControlType::CHECKBOX) {
        auto checkbox = std::dynamic_pointer_cast<GuiCheckBox>(control);
        if (checkbox) {
            checkbox->set_checked(checked);
        }
    }
}

double GuiManager::get_slider_value(int control_id) {
    auto control = get_control(control_id);
    if (control && control->get_type() == GuiControlType::SLIDER) {
        auto slider = std::dynamic_pointer_cast<GuiSlider>(control);
        if (slider) {
            return slider->get_numeric_value();
        }
    }
    return 0.0;
}

double GuiManager::get_slider_value(const std::string& name) {
    auto control = get_control(name);
    if (control && control->get_type() == GuiControlType::SLIDER) {
        auto slider = std::dynamic_pointer_cast<GuiSlider>(control);
        if (slider) {
            return slider->get_numeric_value();
        }
    }
    return 0.0;
}

void GuiManager::set_slider_value(int control_id, double value) {
    auto control = get_control(control_id);
    if (control && control->get_type() == GuiControlType::SLIDER) {
        auto slider = std::dynamic_pointer_cast<GuiSlider>(control);
        if (slider) {
            slider->set_numeric_value(value);
        }
    }
}

void GuiManager::set_slider_value(const std::string& name, double value) {
    auto control = get_control(name);
    if (control && control->get_type() == GuiControlType::SLIDER) {
        auto slider = std::dynamic_pointer_cast<GuiSlider>(control);
        if (slider) {
            slider->set_numeric_value(value);
        }
    }
}

double GuiManager::get_progressbar_value(int control_id) {
    auto control = get_control(control_id);
    if (control && control->get_type() == GuiControlType::PROGRESSBAR) {
        auto progressbar = std::dynamic_pointer_cast<GuiProgressBar>(control);
        if (progressbar) {
            return progressbar->get_numeric_value();
        }
    }
    return 0.0;
}

double GuiManager::get_progressbar_value(const std::string& name) {
    auto control = get_control(name);
    if (control && control->get_type() == GuiControlType::PROGRESSBAR) {
        auto progressbar = std::dynamic_pointer_cast<GuiProgressBar>(control);
        if (progressbar) {
            return progressbar->get_numeric_value();
        }
    }
    return 0.0;
}

void GuiManager::set_progressbar_value(int control_id, double value) {
    auto control = get_control(control_id);
    if (control && control->get_type() == GuiControlType::PROGRESSBAR) {
        auto progressbar = std::dynamic_pointer_cast<GuiProgressBar>(control);
        if (progressbar) {
            progressbar->set_numeric_value(value);
        }
    }
}

void GuiManager::set_progressbar_value(const std::string& name, double value) {
    auto control = get_control(name);
    if (control && control->get_type() == GuiControlType::PROGRESSBAR) {
        auto progressbar = std::dynamic_pointer_cast<GuiProgressBar>(control);
        if (progressbar) {
            progressbar->set_numeric_value(value);
        }
    }
}

// Utility functions
bool GuiManager::is_control_clicked(int control_id) {
    auto control = get_control(control_id);
    if (control) {
        return control->update();
    }
    return false;
}

bool GuiManager::is_control_hovered(int control_id) {
    auto control = get_control(control_id);
    if (control) {
        ::Vector2 mouse_pos = ::GetMousePosition();
        return control->contains_point(mouse_pos.x, mouse_pos.y);
    }
    return false;
}

void GuiManager::bring_control_to_front(int control_id) {
    auto it = std::find(control_order.begin(), control_order.end(), control_id);
    if (it != control_order.end()) {
        control_order.erase(it);
        control_order.push_back(control_id);
    }
}

void GuiManager::send_control_to_back(int control_id) {
    auto it = std::find(control_order.begin(), control_order.end(), control_id);
    if (it != control_order.end()) {
        control_order.erase(it);
        control_order.insert(control_order.begin(), control_id);
    }
}

std::vector<int> GuiManager::get_all_control_ids() {
    return control_order;
}

int GuiManager::get_control_count() {
    return static_cast<int>(controls.size());
}

void GuiManager::set_global_event_callback(GuiEventCallback callback) {
    global_event_callback = callback;
}

} // namespace bas

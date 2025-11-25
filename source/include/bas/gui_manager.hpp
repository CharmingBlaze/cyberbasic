#pragma once
#include "gui_control.hpp"
#include "gui_style.hpp"
#include "gui_types.hpp"
#include <unordered_map>
#include <vector>
#include <memory>

namespace bas {

// GUI Manager - Handles control lifecycle and rendering
class GuiManager {
private:
    static int next_control_id;
    static std::unordered_map<int, std::shared_ptr<GuiControl>> controls;
    static std::unordered_map<std::string, int> control_lookup;
    static std::vector<int> control_order;  // For drawing order
    static int focused_control_id;
    static bool system_initialized;
    static GuiEventCallback global_event_callback;
    
public:
    // System management
    static bool initialize();
    static void shutdown();
    static bool is_initialized() { return system_initialized; }
    
    // Control management
    static int create_button(const std::string& name, int x, int y, int width, int height, const std::string& text = "");
    static int create_label(const std::string& name, int x, int y, int width, int height, const std::string& text = "");
    static int create_textbox(const std::string& name, int x, int y, int width, int height, const std::string& text = "", int max_length = 256);
    static int create_slider(const std::string& name, int x, int y, int width, int height, double min_val, double max_val, double current_val);
    static int create_checkbox(const std::string& name, int x, int y, int width, int height, const std::string& text = "", bool checked = false);
    static int create_combobox(const std::string& name, int x, int y, int width, int height);
    static int create_progressbar(const std::string& name, int x, int y, int width, int height, double min_val, double max_val, double current_val);
    static int create_listview(const std::string& name, int x, int y, int width, int height);
    static int create_windowbox(const std::string& name, int x, int y, int width, int height, const std::string& title = "");
    static int create_panel(const std::string& name, int x, int y, int width, int height);
    static int create_scrollpanel(const std::string& name, int x, int y, int width, int height, int content_width, int content_height);
    
    // Control access
    static std::shared_ptr<GuiControl> get_control(int control_id);
    static std::shared_ptr<GuiControl> get_control(const std::string& name);
    static void destroy_control(int control_id);
    static void destroy_control(const std::string& name);
    static void destroy_all_controls();
    
    // Control properties
    static void set_control_text(int control_id, const std::string& text);
    static void set_control_text(const std::string& name, const std::string& text);
    static std::string get_control_text(int control_id);
    static std::string get_control_text(const std::string& name);
    static void set_control_value(int control_id, const std::string& value);
    static void set_control_value(const std::string& name, const std::string& value);
    static std::string get_control_value(int control_id);
    static std::string get_control_value(const std::string& name);
    static void set_control_position(int control_id, int x, int y);
    static void set_control_position(const std::string& name, int x, int y);
    static void set_control_size(int control_id, int width, int height);
    static void set_control_size(const std::string& name, int width, int height);
    static void set_control_visible(int control_id, bool visible);
    static void set_control_visible(const std::string& name, bool visible);
    static void set_control_enabled(int control_id, bool enabled);
    static void set_control_enabled(const std::string& name, bool enabled);
    
    // Specialized control methods
    static void add_combobox_item(int control_id, const std::string& item);
    static void add_combobox_item(const std::string& name, const std::string& item);
    static void clear_combobox_items(int control_id);
    static void clear_combobox_items(const std::string& name);
    static int get_combobox_selected_index(int control_id);
    static int get_combobox_selected_index(const std::string& name);
    static void set_combobox_selected_index(int control_id, int index);
    static void set_combobox_selected_index(const std::string& name, int index);
    
    static void add_listview_item(int control_id, const std::string& item);
    static void add_listview_item(const std::string& name, const std::string& item);
    static void clear_listview_items(int control_id);
    static void clear_listview_items(const std::string& name);
    static int get_listview_selected_index(int control_id);
    static int get_listview_selected_index(const std::string& name);
    static void set_listview_selected_index(int control_id, int index);
    static void set_listview_selected_index(const std::string& name, int index);
    
    static bool is_checkbox_checked(int control_id);
    static bool is_checkbox_checked(const std::string& name);
    static void set_checkbox_checked(int control_id, bool checked);
    static void set_checkbox_checked(const std::string& name, bool checked);
    
    static double get_slider_value(int control_id);
    static double get_slider_value(const std::string& name);
    static void set_slider_value(int control_id, double value);
    static void set_slider_value(const std::string& name, double value);
    
    static double get_progressbar_value(int control_id);
    static double get_progressbar_value(const std::string& name);
    static void set_progressbar_value(int control_id, double value);
    static void set_progressbar_value(const std::string& name, double value);
    
    // System update and rendering
    static void update_all_controls();
    static void draw_all_controls();
    static void set_focused_control(int control_id);
    static int get_focused_control() { return focused_control_id; }
    
    // Event handling
    static void set_global_event_callback(GuiEventCallback callback);
    static void set_control_event_callback(int control_id, GuiEventCallback callback);
    static void set_control_event_callback(const std::string& name, GuiEventCallback callback);
    
    // Utility functions
    static bool is_control_clicked(int control_id);
    static bool is_control_hovered(int control_id);
    static void bring_control_to_front(int control_id);
    static void send_control_to_back(int control_id);
    static std::vector<int> get_all_control_ids();
    static int get_control_count();
    
    // Layout helpers
    static void arrange_controls_horizontal(const std::vector<int>& control_ids, int start_x, int start_y, int spacing = 5);
    static void arrange_controls_vertical(const std::vector<int>& control_ids, int start_x, int start_y, int spacing = 5);
    static void center_control(int control_id, int container_width, int container_height);
    static void center_control(const std::string& name, int container_width, int container_height);
};

} // namespace bas

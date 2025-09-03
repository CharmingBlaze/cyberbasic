#include "bas/gui.hpp"
#include "bas/runtime.hpp"

namespace bas {

void register_gui_functions(FunctionRegistry& R) {
    // System functions
    R.add("INITGUI", NativeFn{"INITGUI", 0, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("INITGUI: expected 0 args");
        bool success = Gui::Initialize();
        return Value::from_bool(success);
    }});
    
    R.add("SHUTDOWNGUI", NativeFn{"SHUTDOWNGUI", 0, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("SHUTDOWNGUI: expected 0 args");
        Gui::Shutdown();
        return Value::from_bool(true);
    }});
    
    R.add("UPDATEGUI", NativeFn{"UPDATEGUI", 0, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("UPDATEGUI: expected 0 args");
        Gui::Update();
        return Value::from_bool(true);
    }});
    
    R.add("DRAWGUI", NativeFn{"DRAWGUI", 0, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("DRAWGUI: expected 0 args");
        Gui::Draw();
        return Value::from_bool(true);
    }});
    
    // Control creation functions
    R.add("CREATEBUTTON", NativeFn{"CREATEBUTTON", 6, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("CREATEBUTTON: expected 6 args");
        std::string name = args[0].as_string();
        int x = args[1].as_int();
        int y = args[2].as_int();
        int width = args[3].as_int();
        int height = args[4].as_int();
        std::string text = args[5].as_string();
        int button_id = Gui::Button(name, x, y, width, height, text);
        return Value::from_int(button_id);
    }});
    
    R.add("CREATELABEL", NativeFn{"CREATELABEL", 6, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("CREATELABEL: expected 6 args");
        std::string name = args[0].as_string();
        int x = args[1].as_int();
        int y = args[2].as_int();
        int width = args[3].as_int();
        int height = args[4].as_int();
        std::string text = args[5].as_string();
        int label_id = Gui::Label(name, x, y, width, height, text);
        return Value::from_int(label_id);
    }});
    
    R.add("CREATETEXTBOX", NativeFn{"CREATETEXTBOX", 6, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("CREATETEXTBOX: expected 6 args");
        std::string name = args[0].as_string();
        int x = args[1].as_int();
        int y = args[2].as_int();
        int width = args[3].as_int();
        int height = args[4].as_int();
        std::string text = args[5].as_string();
        int textbox_id = Gui::TextBox(name, x, y, width, height, text);
        return Value::from_int(textbox_id);
    }});
    
    R.add("CREATESLIDER", NativeFn{"CREATESLIDER", 8, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("CREATESLIDER: expected 8 args");
        std::string name = args[0].as_string();
        int x = args[1].as_int();
        int y = args[2].as_int();
        int width = args[3].as_int();
        int height = args[4].as_int();
        double min_val = args[5].as_number();
        double max_val = args[6].as_number();
        double current_val = args[7].as_number();
        int slider_id = Gui::Slider(name, x, y, width, height, min_val, max_val, current_val);
        return Value::from_int(slider_id);
    }});
    
    R.add("CREATECHECKBOX", NativeFn{"CREATECHECKBOX", 7, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("CREATECHECKBOX: expected 7 args");
        std::string name = args[0].as_string();
        int x = args[1].as_int();
        int y = args[2].as_int();
        int width = args[3].as_int();
        int height = args[4].as_int();
        std::string text = args[5].as_string();
        bool checked = args[6].as_bool();
        int checkbox_id = Gui::CheckBox(name, x, y, width, height, text, checked);
        return Value::from_int(checkbox_id);
    }});
    
    R.add("CREATECOMBOBOX", NativeFn{"CREATECOMBOBOX", 5, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("CREATECOMBOBOX: expected 5 args");
        std::string name = args[0].as_string();
        int x = args[1].as_int();
        int y = args[2].as_int();
        int width = args[3].as_int();
        int height = args[4].as_int();
        int combobox_id = Gui::ComboBox(name, x, y, width, height);
        return Value::from_int(combobox_id);
    }});
    
    R.add("CREATEPROGRESSBAR", NativeFn{"CREATEPROGRESSBAR", 8, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("CREATEPROGRESSBAR: expected 8 args");
        std::string name = args[0].as_string();
        int x = args[1].as_int();
        int y = args[2].as_int();
        int width = args[3].as_int();
        int height = args[4].as_int();
        double min_val = args[5].as_number();
        double max_val = args[6].as_number();
        double current_val = args[7].as_number();
        int progressbar_id = Gui::ProgressBar(name, x, y, width, height, min_val, max_val, current_val);
        return Value::from_int(progressbar_id);
    }});
    
    R.add("CREATELISTVIEW", NativeFn{"CREATELISTVIEW", 5, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("CREATELISTVIEW: expected 5 args");
        std::string name = args[0].as_string();
        int x = args[1].as_int();
        int y = args[2].as_int();
        int width = args[3].as_int();
        int height = args[4].as_int();
        int listview_id = Gui::ListView(name, x, y, width, height);
        return Value::from_int(listview_id);
    }});
    
    R.add("CREATEWINDOW", NativeFn{"CREATEWINDOW", 6, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("CREATEWINDOW: expected 6 args");
        std::string name = args[0].as_string();
        int x = args[1].as_int();
        int y = args[2].as_int();
        int width = args[3].as_int();
        int height = args[4].as_int();
        std::string title = args[5].as_string();
        int window_id = Gui::Window(name, x, y, width, height, title);
        return Value::from_int(window_id);
    }});
    
    R.add("CREATEPANEL", NativeFn{"CREATEPANEL", 5, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("CREATEPANEL: expected 5 args");
        std::string name = args[0].as_string();
        int x = args[1].as_int();
        int y = args[2].as_int();
        int width = args[3].as_int();
        int height = args[4].as_int();
        int panel_id = Gui::Panel(name, x, y, width, height);
        return Value::from_int(panel_id);
    }});
    
    // Control property functions
    R.add("SETCONTROLTEXT", NativeFn{"SETCONTROLTEXT", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETCONTROLTEXT: expected 2 args");
        int control_id = args[0].as_int();
        std::string text = args[1].as_string();
        Gui::SetText(control_id, text);
        return Value::from_bool(true);
    }});
    
    R.add("GETCONTROLTEXT", NativeFn{"GETCONTROLTEXT", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCONTROLTEXT: expected 1 arg");
        int control_id = args[0].as_int();
        std::string text = Gui::GetText(control_id);
        return Value::from_string(text);
    }});
    
    R.add("SETCONTROLVALUE", NativeFn{"SETCONTROLVALUE", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETCONTROLVALUE: expected 2 args");
        int control_id = args[0].as_int();
        std::string value = args[1].as_string();
        Gui::SetValue(control_id, value);
        return Value::from_bool(true);
    }});
    
    R.add("GETCONTROLVALUE", NativeFn{"GETCONTROLVALUE", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCONTROLVALUE: expected 1 arg");
        int control_id = args[0].as_int();
        std::string value = Gui::GetValue(control_id);
        return Value::from_string(value);
    }});
    
    R.add("SETCONTROLPOSITION", NativeFn{"SETCONTROLPOSITION", 3, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETCONTROLPOSITION: expected 3 args");
        int control_id = args[0].as_int();
        int x = args[1].as_int();
        int y = args[2].as_int();
        Gui::SetPosition(control_id, x, y);
        return Value::from_bool(true);
    }});
    
    R.add("SETCONTROLSIZE", NativeFn{"SETCONTROLSIZE", 3, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETCONTROLSIZE: expected 3 args");
        int control_id = args[0].as_int();
        int width = args[1].as_int();
        int height = args[2].as_int();
        Gui::SetSize(control_id, width, height);
        return Value::from_bool(true);
    }});
    
    R.add("SETCONTROLVISIBLE", NativeFn{"SETCONTROLVISIBLE", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETCONTROLVISIBLE: expected 2 args");
        int control_id = args[0].as_int();
        bool visible = args[1].as_bool();
        Gui::SetVisible(control_id, visible);
        return Value::from_bool(true);
    }});
    
    R.add("SETCONTROLENABLED", NativeFn{"SETCONTROLENABLED", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETCONTROLENABLED: expected 2 args");
        int control_id = args[0].as_int();
        bool enabled = args[1].as_bool();
        Gui::SetEnabled(control_id, enabled);
        return Value::from_bool(true);
    }});
    
    // Specialized control functions
    R.add("ADDCOMBOBOXITEM", NativeFn{"ADDCOMBOBOXITEM", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ADDCOMBOBOXITEM: expected 2 args");
        int control_id = args[0].as_int();
        std::string item = args[1].as_string();
        GuiManager::add_combobox_item(control_id, item);
        return Value::from_bool(true);
    }});
    
    R.add("CLEARCOMBOBOXITEMS", NativeFn{"CLEARCOMBOBOXITEMS", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("CLEARCOMBOBOXITEMS: expected 1 arg");
        int control_id = args[0].as_int();
        GuiManager::clear_combobox_items(control_id);
        return Value::from_bool(true);
    }});
    
    R.add("GETCOMBOBOXSELECTED", NativeFn{"GETCOMBOBOXSELECTED", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCOMBOBOXSELECTED: expected 1 arg");
        int control_id = args[0].as_int();
        int selected = GuiManager::get_combobox_selected_index(control_id);
        return Value::from_int(selected);
    }});
    
    R.add("SETCOMBOBOXSELECTED", NativeFn{"SETCOMBOBOXSELECTED", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETCOMBOBOXSELECTED: expected 2 args");
        int control_id = args[0].as_int();
        int index = args[1].as_int();
        GuiManager::set_combobox_selected_index(control_id, index);
        return Value::from_bool(true);
    }});
    
    R.add("ADDLISTVIEWITEM", NativeFn{"ADDLISTVIEWITEM", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ADDLISTVIEWITEM: expected 2 args");
        int control_id = args[0].as_int();
        std::string item = args[1].as_string();
        GuiManager::add_listview_item(control_id, item);
        return Value::from_bool(true);
    }});
    
    R.add("CLEARLISTVIEWITEMS", NativeFn{"CLEARLISTVIEWITEMS", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("CLEARLISTVIEWITEMS: expected 1 arg");
        int control_id = args[0].as_int();
        GuiManager::clear_listview_items(control_id);
        return Value::from_bool(true);
    }});
    
    R.add("GETLISTVIEWSELECTED", NativeFn{"GETLISTVIEWSELECTED", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETLISTVIEWSELECTED: expected 1 arg");
        int control_id = args[0].as_int();
        int selected = GuiManager::get_listview_selected_index(control_id);
        return Value::from_int(selected);
    }});
    
    R.add("SETLISTVIEWSELECTED", NativeFn{"SETLISTVIEWSELECTED", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETLISTVIEWSELECTED: expected 2 args");
        int control_id = args[0].as_int();
        int index = args[1].as_int();
        GuiManager::set_listview_selected_index(control_id, index);
        return Value::from_bool(true);
    }});
    
    R.add("ISCHECKBOXCHECKED", NativeFn{"ISCHECKBOXCHECKED", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISCHECKBOXCHECKED: expected 1 arg");
        int control_id = args[0].as_int();
        bool checked = GuiManager::is_checkbox_checked(control_id);
        return Value::from_bool(checked);
    }});
    
    R.add("SETCHECKBOXCHECKED", NativeFn{"SETCHECKBOXCHECKED", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETCHECKBOXCHECKED: expected 2 args");
        int control_id = args[0].as_int();
        bool checked = args[1].as_bool();
        GuiManager::set_checkbox_checked(control_id, checked);
        return Value::from_bool(true);
    }});
    
    R.add("GETSLIDERVALUE", NativeFn{"GETSLIDERVALUE", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETSLIDERVALUE: expected 1 arg");
        int control_id = args[0].as_int();
        double value = GuiManager::get_slider_value(control_id);
        return Value::from_number(value);
    }});
    
    R.add("SETSLIDERVALUE", NativeFn{"SETSLIDERVALUE", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETSLIDERVALUE: expected 2 args");
        int control_id = args[0].as_int();
        double value = args[1].as_number();
        GuiManager::set_slider_value(control_id, value);
        return Value::from_bool(true);
    }});
    
    R.add("GETPROGRESSBARVALUE", NativeFn{"GETPROGRESSBARVALUE", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETPROGRESSBARVALUE: expected 1 arg");
        int control_id = args[0].as_int();
        double value = GuiManager::get_progressbar_value(control_id);
        return Value::from_number(value);
    }});
    
    R.add("SETPROGRESSBARVALUE", NativeFn{"SETPROGRESSBARVALUE", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETPROGRESSBARVALUE: expected 2 args");
        int control_id = args[0].as_int();
        double value = args[1].as_number();
        GuiManager::set_progressbar_value(control_id, value);
        return Value::from_bool(true);
    }});
    
    // Style functions
    R.add("USEDEFAULTSTYLE", NativeFn{"USEDEFAULTSTYLE", 0, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("USEDEFAULTSTYLE: expected 0 args");
        Gui::UseDefaultStyle();
        return Value::from_bool(true);
    }});
    
    R.add("USEDARKSTYLE", NativeFn{"USEDARKSTYLE", 0, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("USEDARKSTYLE: expected 0 args");
        Gui::UseDarkStyle();
        return Value::from_bool(true);
    }});
    
    R.add("USELIGHTSTYLE", NativeFn{"USELIGHTSTYLE", 0, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("USELIGHTSTYLE: expected 0 args");
        Gui::UseLightStyle();
        return Value::from_bool(true);
    }});
    
    R.add("USEGAMESTYLE", NativeFn{"USEGAMESTYLE", 0, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("USEGAMESTYLE: expected 0 args");
        Gui::UseGameStyle();
        return Value::from_bool(true);
    }});
    
    // Layout functions
    R.add("CREATEHORIZONTALLAYOUT", NativeFn{"CREATEHORIZONTALLAYOUT", 4, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("CREATEHORIZONTALLAYOUT: expected 4 args");
        int x = args[0].as_int();
        int y = args[1].as_int();
        int width = args[2].as_int();
        int height = args[3].as_int();
        int layout_id = Gui::CreateHorizontalLayout(x, y, width, height);
        return Value::from_int(layout_id);
    }});
    
    R.add("CREATEVERTICALLAYOUT", NativeFn{"CREATEVERTICALLAYOUT", 4, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("CREATEVERTICALLAYOUT: expected 4 args");
        int x = args[0].as_int();
        int y = args[1].as_int();
        int width = args[2].as_int();
        int height = args[3].as_int();
        int layout_id = Gui::CreateVerticalLayout(x, y, width, height);
        return Value::from_int(layout_id);
    }});
    
    R.add("CREATEGRIDLAYOUT", NativeFn{"CREATEGRIDLAYOUT", 6, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("CREATEGRIDLAYOUT: expected 6 args");
        int x = args[0].as_int();
        int y = args[1].as_int();
        int width = args[2].as_int();
        int height = args[3].as_int();
        int columns = args[4].as_int();
        int rows = args[5].as_int();
        int layout_id = Gui::CreateGridLayout(x, y, width, height, columns, rows);
        return Value::from_int(layout_id);
    }});
    
    R.add("ADDTOLAYOUT", NativeFn{"ADDTOLAYOUT", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ADDTOLAYOUT: expected 2 args");
        int layout_id = args[0].as_int();
        int control_id = args[1].as_int();
        Gui::AddToLayout(layout_id, control_id);
        return Value::from_bool(true);
    }});
    
    R.add("UPDATELAYOUT", NativeFn{"UPDATELAYOUT", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UPDATELAYOUT: expected 1 arg");
        int layout_id = args[0].as_int();
        Gui::UpdateLayout(layout_id);
        return Value::from_bool(true);
    }});
    
    // Control destruction
    R.add("DESTROYCONTROL", NativeFn{"DESTROYCONTROL", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("DESTROYCONTROL: expected 1 arg");
        int control_id = args[0].as_int();
        GuiManager::destroy_control(control_id);
        return Value::from_bool(true);
    }});
    
    R.add("DESTROYALLCONTROLS", NativeFn{"DESTROYALLCONTROLS", 0, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("DESTROYALLCONTROLS: expected 0 args");
        GuiManager::destroy_all_controls();
        return Value::from_bool(true);
    }});
}

} // namespace bas

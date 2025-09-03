#include "bas/level_editor.hpp"
#include "bas/runtime.hpp"

namespace bas {

void register_level_editor_functions(FunctionRegistry& R) {
    
    // === LEVEL EDITOR FUNCTIONS ===
    
    R.add("CREATELEVEL", NativeFn{"CREATELEVEL", 3, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("CREATELEVEL: expected 3 args");
        std::string name = args[0].as_string();
        double width = args[1].as_number();
        double height = args[2].as_number();
        int level_id = LevelEditor::create_level(name, width, height);
        return Value::from_int(level_id);
    }});
    
    R.add("ADDLEVELLAYER", NativeFn{"ADDLEVELLAYER", 3, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("ADDLEVELLAYER: expected 3 args");
        int level_id = args[0].as_int();
        std::string layer_name = args[1].as_string();
        int depth = args[2].as_int();
        LevelEditor::add_level_layer(level_id, layer_name, depth);
        return Value::nil();
    }});
    
    R.add("PLACELEVELOBJECT", NativeFn{"PLACELEVELOBJECT", 6, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("PLACELEVELOBJECT: expected 6 args");
        int level_id = args[0].as_int();
        std::string obj_name = args[1].as_string();
        std::string obj_type = args[2].as_string();
        double x = args[3].as_number();
        double y = args[4].as_number();
        double z = args[5].as_number();
        LevelEditor::place_level_object(level_id, obj_name, obj_type, x, y, z);
        return Value::nil();
    }});
    
    R.add("SAVELEVEL", NativeFn{"SAVELEVEL", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SAVELEVEL: expected 2 args");
        int level_id = args[0].as_int();
        std::string filename = args[1].as_string();
        bool success = LevelEditor::save_level(level_id, filename);
        return Value::from_bool(success);
    }});
    
    R.add("LOADLEVEL", NativeFn{"LOADLEVEL", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADLEVEL: expected 1 arg");
        std::string filename = args[1].as_string();
        int level_id = LevelEditor::load_level(filename);
        return Value::from_int(level_id);
    }});
    
    R.add("GETLEVELWIDTH", NativeFn{"GETLEVELWIDTH", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETLEVELWIDTH: expected 1 arg");
        int level_id = args[0].as_int();
        auto level = LevelEditor::get_level(level_id);
        if (level) {
            return Value::from_number(level->get_width());
        }
        return Value::from_number(0.0);
    }});
    
    R.add("GETLEVELHEIGHT", NativeFn{"GETLEVELHEIGHT", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETLEVELHEIGHT: expected 1 arg");
        int level_id = args[0].as_int();
        auto level = LevelEditor::get_level(level_id);
        if (level) {
            return Value::from_number(level->get_height());
        }
        return Value::from_number(0.0);
    }});
    
    R.add("GETLEVELNAME", NativeFn{"GETLEVELNAME", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETLEVELNAME: expected 1 arg");
        int level_id = args[0].as_int();
        auto level = LevelEditor::get_level(level_id);
        if (level) {
            return Value::from_string(level->get_name());
        }
        return Value::from_string("");
    }});
    
    R.add("DESTROYLEVEL", NativeFn{"DESTROYLEVEL", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("DESTROYLEVEL: expected 1 arg");
        int level_id = args[0].as_int();
        LevelEditor::destroy_level(level_id);
        return Value::nil();
    }});
    
    R.add("SETLEVELPROPERTY", NativeFn{"SETLEVELPROPERTY", 3, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETLEVELPROPERTY: expected 3 args");
        int level_id = args[0].as_int();
        std::string key = args[1].as_string();
        std::string value = args[2].as_string();
        auto level = LevelEditor::get_level(level_id);
        if (level) {
            level->set_property(key, value);
        }
        return Value::nil();
    }});
    
    R.add("GETLEVELPROPERTY", NativeFn{"GETLEVELPROPERTY", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETLEVELPROPERTY: expected 2 args");
        int level_id = args[0].as_int();
        std::string key = args[1].as_string();
        auto level = LevelEditor::get_level(level_id);
        if (level) {
            return Value::from_string(level->get_property(key));
        }
        return Value::from_string("");
    }});
}

} // namespace bas

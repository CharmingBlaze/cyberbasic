#include "bas/level_editor.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace bas {

// Static member definitions
int LevelEditor::next_level_id = 1;
std::unordered_map<int, std::shared_ptr<Level>> LevelEditor::levels;

void Level::add_layer(const std::string& layer_name, int depth) {
    auto layer = std::make_shared<LevelLayer>(layer_name, depth);
    layers.push_back(layer);
}

void Level::place_object(const std::string& obj_name, const std::string& obj_type, 
                        double x, double y, double z, const std::string& layer_name) {
    auto obj = std::make_shared<LevelObject>(obj_name, obj_type, x, y, z);
    
    // Find the layer or create default layer
    auto layer = get_layer(layer_name);
    if (!layer) {
        add_layer(layer_name, 0);
        layer = get_layer(layer_name);
    }
    
    if (layer) {
        layer->objects.push_back(obj);
    }
}

std::shared_ptr<LevelLayer> Level::get_layer(const std::string& name) const {
    for (auto& layer : layers) {
        if (layer->name == name) {
            return layer;
        }
    }
    return nullptr;
}

void Level::set_property(const std::string& key, const std::string& value) {
    properties[key] = value;
}

std::string Level::get_property(const std::string& key) const {
    auto it = properties.find(key);
    return (it != properties.end()) ? it->second : "";
}

bool Level::save_to_file(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "LEVEL " << name << " " << width << " " << height << std::endl;
    
    // Save properties
    for (const auto& prop : properties) {
        file << "PROPERTY " << prop.first << " " << prop.second << std::endl;
    }
    
    // Save layers and objects
    for (const auto& layer : layers) {
        file << "LAYER " << layer->name << " " << layer->depth << std::endl;
        for (const auto& obj : layer->objects) {
            file << "OBJECT " << obj->name << " " << obj->type << " " 
                 << obj->x << " " << obj->y << " " << obj->z << std::endl;
        }
    }
    
    file.close();
    return true;
}

std::shared_ptr<Level> Level::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return nullptr;
    }
    
    std::string line;
    std::string name;
    double width, height;
    
    // Read level header
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token; // "LEVEL"
        iss >> name >> width >> height;
    }
    
    auto level = std::make_shared<Level>(0, name, width, height);
    std::shared_ptr<LevelLayer> current_layer = nullptr;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;
        
        if (token == "PROPERTY") {
            std::string key, value;
            iss >> key >> value;
            level->set_property(key, value);
        }
        else if (token == "LAYER") {
            std::string layer_name;
            int depth;
            iss >> layer_name >> depth;
            level->add_layer(layer_name, depth);
            current_layer = level->get_layer(layer_name);
        }
        else if (token == "OBJECT" && current_layer) {
            std::string obj_name, obj_type;
            double x, y, z;
            iss >> obj_name >> obj_type >> x >> y >> z;
            auto obj = std::make_shared<LevelObject>(obj_name, obj_type, x, y, z);
            current_layer->objects.push_back(obj);
        }
    }
    
    file.close();
    return level;
}

// LevelEditor implementation
int LevelEditor::create_level(const std::string& name, double width, double height) {
    int id = next_level_id++;
    auto level = std::make_shared<Level>(id, name, width, height);
    levels[id] = level;
    return id;
}

void LevelEditor::add_level_layer(int level_id, const std::string& layer_name, int depth) {
    auto it = levels.find(level_id);
    if (it != levels.end()) {
        it->second->add_layer(layer_name, depth);
    }
}

void LevelEditor::place_level_object(int level_id, const std::string& obj_name, 
                                    const std::string& obj_type, double x, double y, double z) {
    auto it = levels.find(level_id);
    if (it != levels.end()) {
        it->second->place_object(obj_name, obj_type, x, y, z);
    }
}

bool LevelEditor::save_level(int level_id, const std::string& filename) {
    auto it = levels.find(level_id);
    if (it != levels.end()) {
        return it->second->save_to_file(filename);
    }
    return false;
}

int LevelEditor::load_level(const std::string& filename) {
    auto level = Level::load_from_file(filename);
    if (level) {
        int id = next_level_id++;
        level = std::make_shared<Level>(id, level->get_name(), level->get_width(), level->get_height());
        levels[id] = level;
        return id;
    }
    return -1;
}

std::shared_ptr<Level> LevelEditor::get_level(int level_id) {
    auto it = levels.find(level_id);
    return (it != levels.end()) ? it->second : nullptr;
}

void LevelEditor::destroy_level(int level_id) {
    levels.erase(level_id);
}

} // namespace bas

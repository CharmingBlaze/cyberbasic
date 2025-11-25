#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "value.hpp"

namespace bas {

struct LevelObject {
    std::string name;
    std::string type;
    double x, y, z;
    std::unordered_map<std::string, std::string> properties;
    
    LevelObject(const std::string& n, const std::string& t, double px, double py, double pz)
        : name(n), type(t), x(px), y(py), z(pz) {}
};

struct LevelLayer {
    std::string name;
    int depth;
    std::vector<std::shared_ptr<LevelObject>> objects;
    
    LevelLayer(const std::string& n, int d) : name(n), depth(d) {}
};

class Level {
private:
    int id;
    std::string name;
    double width, height;
    std::vector<std::shared_ptr<LevelLayer>> layers;
    std::unordered_map<std::string, std::string> properties;
    
public:
    Level(int level_id, const std::string& level_name, double w, double h)
        : id(level_id), name(level_name), width(w), height(h) {}
    
    int get_id() const { return id; }
    const std::string& get_name() const { return name; }
    double get_width() const { return width; }
    double get_height() const { return height; }
    
    void add_layer(const std::string& layer_name, int depth);
    void place_object(const std::string& obj_name, const std::string& obj_type, 
                     double x, double y, double z, const std::string& layer_name = "default");
    
    const std::vector<std::shared_ptr<LevelLayer>>& get_layers() const { return layers; }
    std::shared_ptr<LevelLayer> get_layer(const std::string& name) const;
    
    void set_property(const std::string& key, const std::string& value);
    std::string get_property(const std::string& key) const;
    
    bool save_to_file(const std::string& filename) const;
    static std::shared_ptr<Level> load_from_file(const std::string& filename);
};

class LevelEditor {
private:
    static int next_level_id;
    static std::unordered_map<int, std::shared_ptr<Level>> levels;
    
public:
    static int create_level(const std::string& name, double width, double height);
    static void add_level_layer(int level_id, const std::string& layer_name, int depth);
    static void place_level_object(int level_id, const std::string& obj_name, 
                                  const std::string& obj_type, double x, double y, double z);
    static bool save_level(int level_id, const std::string& filename);
    static int load_level(const std::string& filename);
    static std::shared_ptr<Level> get_level(int level_id);
    static void destroy_level(int level_id);
};

} // namespace bas

#pragma once

#include "value.hpp"
#include <unordered_map>
#include <string>

namespace bas {
namespace postfx_registry {
    // Post-processing effect registry
    extern std::unordered_map<int, std::string> effects;
    extern int next_effect_id;
    
    int create_effect(const std::string& name, const std::string& type, int intensity);
    void destroy_effect(int id);
    void enable_effect(int id, bool enabled);
    bool is_effect_enabled(int id);
    void set_effect_intensity(int id, int intensity);
    int get_effect_count();
    void clear_all_effects();
}
} // namespace bas


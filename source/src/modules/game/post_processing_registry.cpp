#include "bas/post_processing_registry.hpp"
#include <unordered_map>
#include <string>

namespace bas {
namespace postfx_registry {

std::unordered_map<int, std::string> effects;
int next_effect_id = 1;

int create_effect(const std::string& name, const std::string& type, int intensity) {
    int id = next_effect_id++;
    effects[id] = name + ":" + type + ":" + std::to_string(intensity);
    return id;
}

void destroy_effect(int id) {
    effects.erase(id);
}

void enable_effect(int id, bool enabled) {
    // Effects are tracked by ID, enabled state is managed by the post-processing system
    (void)id; (void)enabled;
}

bool is_effect_enabled(int id) {
    return effects.find(id) != effects.end();
}

void set_effect_intensity(int id, int intensity) {
    // Update intensity in the effect string
    if (effects.find(id) != effects.end()) {
        std::string& effect = effects[id];
        size_t last_colon = effect.find_last_of(':');
        if (last_colon != std::string::npos) {
            effect = effect.substr(0, last_colon + 1) + std::to_string(intensity);
        }
    }
}

int get_effect_count() {
    return static_cast<int>(effects.size());
}

void clear_all_effects() {
    effects.clear();
}

} // namespace postfx_registry
} // namespace bas


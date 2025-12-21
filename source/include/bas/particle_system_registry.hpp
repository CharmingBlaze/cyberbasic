#pragma once

#include "value.hpp"
#include <unordered_map>
#include <string>

namespace bas {
namespace particle_registry {
    // Particle system data
    struct ParticleSystemData {
        std::string name;
        int maxParticles;
        float x, y;
        float velX, velY;
        int r, g, b, a;
        float minSize, maxSize;
        float minLifetime, maxLifetime;
        float emissionRate;
        bool enabled;
        std::string type; // "fire", "explosion", "smoke", etc.
    };
    
    extern std::unordered_map<int, ParticleSystemData> systems;
    extern int next_system_id;
    
    int create_system(const std::string& name, int maxParticles);
    void destroy_system(int id);
    void update_system(int id, float deltaTime);
    void render_system(int id);
    void set_position(int id, float x, float y);
    void set_velocity(int id, float velX, float velY);
    void set_color(int id, int r, int g, int b, int a);
    void set_size(int id, float minSize, float maxSize);
    void set_lifetime(int id, float minLifetime, float maxLifetime);
    void set_emission_rate(int id, float rate);
    void enable_system(int id, bool enabled);
    bool is_system_enabled(int id);
    int create_effect(const std::string& type, float x, float y, float width = 0, float height = 0);
    int get_system_count();
    void clear_all_systems();
}
} // namespace bas


#include "bas/particle_system_registry.hpp"
#include <unordered_map>
#include <string>

namespace bas {
namespace particle_registry {

std::unordered_map<int, ParticleSystemData> systems;
int next_system_id = 1;

int create_system(const std::string& name, int maxParticles) {
    int id = next_system_id++;
    ParticleSystemData ps;
    ps.name = name;
    ps.maxParticles = maxParticles;
    ps.x = ps.y = 0.0f;
    ps.velX = ps.velY = 0.0f;
    ps.r = ps.g = ps.b = 255;
    ps.a = 255;
    ps.minSize = ps.maxSize = 1.0f;
    ps.minLifetime = ps.maxLifetime = 1.0f;
    ps.emissionRate = 10.0f;
    ps.enabled = true;
    ps.type = "generic";
    systems[id] = ps;
    return id;
}

void destroy_system(int id) {
    systems.erase(id);
}

void update_system(int id, float deltaTime) {
    // Particle system update logic would go here
    (void)id; (void)deltaTime;
}

void render_system(int id) {
    // Particle system rendering logic would go here
    (void)id;
}

void set_position(int id, float x, float y) {
    if (systems.find(id) != systems.end()) {
        systems[id].x = x;
        systems[id].y = y;
    }
}

void set_velocity(int id, float velX, float velY) {
    if (systems.find(id) != systems.end()) {
        systems[id].velX = velX;
        systems[id].velY = velY;
    }
}

void set_color(int id, int r, int g, int b, int a) {
    if (systems.find(id) != systems.end()) {
        systems[id].r = r;
        systems[id].g = g;
        systems[id].b = b;
        systems[id].a = a;
    }
}

void set_size(int id, float minSize, float maxSize) {
    if (systems.find(id) != systems.end()) {
        systems[id].minSize = minSize;
        systems[id].maxSize = maxSize;
    }
}

void set_lifetime(int id, float minLifetime, float maxLifetime) {
    if (systems.find(id) != systems.end()) {
        systems[id].minLifetime = minLifetime;
        systems[id].maxLifetime = maxLifetime;
    }
}

void set_emission_rate(int id, float rate) {
    if (systems.find(id) != systems.end()) {
        systems[id].emissionRate = rate;
    }
}

void enable_system(int id, bool enabled) {
    if (systems.find(id) != systems.end()) {
        systems[id].enabled = enabled;
    }
}

bool is_system_enabled(int id) {
    if (systems.find(id) != systems.end()) {
        return systems[id].enabled;
    }
    return false;
}

int create_effect(const std::string& type, float x, float y, float width, float height) {
    int id = next_system_id++;
    ParticleSystemData ps;
    ps.name = type;
    ps.type = type;
    ps.x = x;
    ps.y = y;
    ps.maxParticles = 100;
    
    // Set defaults based on effect type
    if (type == "fire") {
        ps.r = 255; ps.g = 100; ps.b = 0; ps.a = 255;
        ps.emissionRate = 20.0f;
    } else if (type == "explosion") {
        ps.r = 255; ps.g = 200; ps.b = 0; ps.a = 255;
        ps.emissionRate = 50.0f;
    } else if (type == "smoke") {
        ps.r = 100; ps.g = 100; ps.b = 100; ps.a = 200;
        ps.emissionRate = 10.0f;
    } else if (type == "sparkle") {
        ps.r = 255; ps.g = 255; ps.b = 255; ps.a = 255;
        ps.emissionRate = 30.0f;
    } else if (type == "rain" || type == "snow") {
        ps.r = 200; ps.g = 200; ps.b = 255; ps.a = 255;
        ps.emissionRate = 100.0f;
    }
    
    ps.velX = ps.velY = 0.0f;
    ps.minSize = ps.maxSize = 1.0f;
    ps.minLifetime = ps.maxLifetime = 1.0f;
    ps.enabled = true;
    systems[id] = ps;
    return id;
}

int get_system_count() {
    return static_cast<int>(systems.size());
}

void clear_all_systems() {
    systems.clear();
}

} // namespace particle_registry
} // namespace bas


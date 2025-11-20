#include "bas/graphics.hpp"
#include "bas/runtime.hpp"
#include <algorithm>
#include <cmath>
#include <random>

namespace bas {

// Global graphics system instance
std::unique_ptr<GraphicsSystem> g_graphics_system;

// GraphicsSystem implementation
GraphicsSystem::GraphicsSystem() 
    : next_particle_system_id(0), next_post_effect_id(0), next_shader_id(0), delta_time(0.0f) {
}

GraphicsSystem::~GraphicsSystem() = default;

// Particle system management
int GraphicsSystem::create_particle_system(const std::string& name, ParticleSystemType type) {
    auto system = std::make_unique<ParticleSystem>(next_particle_system_id++, name, type);
    int id = system->id;
    
    // Set default properties based on type
    switch (type) {
        case ParticleSystemType::FIRE:
            system->emission_rate = 50.0f;
            system->particle_life_min = 0.5f;
            system->particle_life_max = 1.5f;
            system->velocity_min_y = -100.0f;
            system->velocity_max_y = -50.0f;
            system->acceleration_y = -50.0f;
            system->color_r = 1.0f;
            system->color_g = 0.3f;
            system->color_b = 0.0f;
            system->color_a = 0.8f;
            break;
        case ParticleSystemType::SMOKE:
            system->emission_rate = 20.0f;
            system->particle_life_min = 2.0f;
            system->particle_life_max = 4.0f;
            system->velocity_min_y = -30.0f;
            system->velocity_max_y = -10.0f;
            system->color_r = 0.3f;
            system->color_g = 0.3f;
            system->color_b = 0.3f;
            system->color_a = 0.6f;
            break;
        case ParticleSystemType::EXPLOSION:
            system->emission_rate = 200.0f;
            system->particle_life_min = 0.2f;
            system->particle_life_max = 0.8f;
            system->velocity_min_x = -200.0f;
            system->velocity_max_x = 200.0f;
            system->velocity_min_y = -200.0f;
            system->velocity_max_y = 200.0f;
            system->color_r = 1.0f;
            system->color_g = 0.8f;
            system->color_b = 0.0f;
            system->color_a = 1.0f;
            system->loop = false;
            break;
        case ParticleSystemType::RAIN:
            system->emission_rate = 100.0f;
            system->particle_life_min = 3.0f;
            system->particle_life_max = 5.0f;
            system->velocity_min_x = -10.0f;
            system->velocity_max_x = 10.0f;
            system->velocity_min_y = -300.0f;
            system->velocity_max_y = -200.0f;
            system->color_r = 0.5f;
            system->color_g = 0.7f;
            system->color_b = 1.0f;
            system->color_a = 0.8f;
            break;
        case ParticleSystemType::SNOW:
            system->emission_rate = 30.0f;
            system->particle_life_min = 5.0f;
            system->particle_life_max = 8.0f;
            system->velocity_min_x = -20.0f;
            system->velocity_max_x = 20.0f;
            system->velocity_min_y = -50.0f;
            system->velocity_max_y = -20.0f;
            system->color_r = 1.0f;
            system->color_g = 1.0f;
            system->color_b = 1.0f;
            system->color_a = 0.9f;
            break;
        case ParticleSystemType::SPARKLE:
            system->emission_rate = 10.0f;
            system->particle_life_min = 1.0f;
            system->particle_life_max = 2.0f;
            system->velocity_min_x = -30.0f;
            system->velocity_max_x = 30.0f;
            system->velocity_min_y = -30.0f;
            system->velocity_max_y = 30.0f;
            system->color_r = 1.0f;
            system->color_g = 1.0f;
            system->color_b = 0.0f;
            system->color_a = 1.0f;
            break;
        default:
            break;
    }
    
    particle_systems.push_back(std::move(system));
    return id;
}

void GraphicsSystem::remove_particle_system(int system_id) {
    particle_systems.erase(std::remove_if(particle_systems.begin(), particle_systems.end(),
        [system_id](const std::unique_ptr<ParticleSystem>& system) {
            return system->id == system_id;
        }), particle_systems.end());
}

ParticleSystem* GraphicsSystem::get_particle_system(int system_id) {
    auto it = std::find_if(particle_systems.begin(), particle_systems.end(),
        [system_id](const std::unique_ptr<ParticleSystem>& system) {
            return system->id == system_id;
        });
    return (it != particle_systems.end()) ? it->get() : nullptr;
}

void GraphicsSystem::set_particle_system_position(int system_id, float x, float y) {
    ParticleSystem* system = get_particle_system(system_id);
    if (system) {
        // Store position for emission (would be used in emit_particle)
        (void)x; (void)y; // Suppress unused parameter warnings
        system->particles.reserve(static_cast<size_t>(system->max_particles));
    }
}

void GraphicsSystem::set_particle_system_emission_rate(int system_id, float rate) {
    ParticleSystem* system = get_particle_system(system_id);
    if (system) {
        system->emission_rate = rate;
    }
}

void GraphicsSystem::set_particle_system_life_time(int system_id, float life_time) {
    ParticleSystem* system = get_particle_system(system_id);
    if (system) {
        system->life_time = life_time;
    }
}

void GraphicsSystem::set_particle_system_max_particles(int system_id, int max_particles) {
    ParticleSystem* system = get_particle_system(system_id);
    if (system) {
        system->max_particles = static_cast<float>(max_particles);
        system->particles.reserve(static_cast<size_t>(max_particles));
    }
}

void GraphicsSystem::set_particle_system_velocity_range(int system_id, float min_x, float max_x, float min_y, float max_y) {
    ParticleSystem* system = get_particle_system(system_id);
    if (system) {
        system->velocity_min_x = min_x;
        system->velocity_max_x = max_x;
        system->velocity_min_y = min_y;
        system->velocity_max_y = max_y;
    }
}

void GraphicsSystem::set_particle_system_size_range(int system_id, float min_size, float max_size) {
    ParticleSystem* system = get_particle_system(system_id);
    if (system) {
        system->particle_size_min = min_size;
        system->particle_size_max = max_size;
    }
}

void GraphicsSystem::set_particle_system_color(int system_id, float r, float g, float b, float a) {
    ParticleSystem* system = get_particle_system(system_id);
    if (system) {
        system->color_r = r;
        system->color_g = g;
        system->color_b = b;
        system->color_a = a;
    }
}

void GraphicsSystem::set_particle_system_loop(int system_id, bool loop) {
    ParticleSystem* system = get_particle_system(system_id);
    if (system) {
        system->loop = loop;
    }
}

void GraphicsSystem::set_particle_system_active(int system_id, bool active) {
    ParticleSystem* system = get_particle_system(system_id);
    if (system) {
        system->active = active;
    }
}

// Post-processing effects
int GraphicsSystem::create_post_effect(const std::string& name, PostProcessType type) {
    auto effect = std::make_unique<PostProcessEffect>(next_post_effect_id++, name, type);
    int id = effect->id;
    
    // Set default parameters based on type
    switch (type) {
        case PostProcessType::BLUR:
            effect->parameters["radius"] = 5.0f;
            break;
        case PostProcessType::SHARPEN:
            effect->parameters["strength"] = 1.0f;
            break;
        case PostProcessType::BLOOM:
            effect->parameters["threshold"] = 0.7f;
            effect->parameters["intensity"] = 1.0f;
            break;
        case PostProcessType::VIGNETTE:
            effect->parameters["radius"] = 0.5f;
            effect->parameters["softness"] = 0.3f;
            break;
        case PostProcessType::CHROMATIC_ABERRATION:
            effect->parameters["strength"] = 0.01f;
            break;
        default:
            break;
    }
    
    post_effects.push_back(std::move(effect));
    return id;
}

void GraphicsSystem::remove_post_effect(int effect_id) {
    post_effects.erase(std::remove_if(post_effects.begin(), post_effects.end(),
        [effect_id](const std::unique_ptr<PostProcessEffect>& effect) {
            return effect->id == effect_id;
        }), post_effects.end());
}

PostProcessEffect* GraphicsSystem::get_post_effect(int effect_id) {
    auto it = std::find_if(post_effects.begin(), post_effects.end(),
        [effect_id](const std::unique_ptr<PostProcessEffect>& effect) {
            return effect->id == effect_id;
        });
    return (it != post_effects.end()) ? it->get() : nullptr;
}

void GraphicsSystem::set_post_effect_active(int effect_id, bool active) {
    PostProcessEffect* effect = get_post_effect(effect_id);
    if (effect) {
        effect->active = active;
    }
}

void GraphicsSystem::set_post_effect_intensity(int effect_id, float intensity) {
    PostProcessEffect* effect = get_post_effect(effect_id);
    if (effect) {
        effect->intensity = intensity;
    }
}

void GraphicsSystem::set_post_effect_parameter(int effect_id, const std::string& name, float value) {
    PostProcessEffect* effect = get_post_effect(effect_id);
    if (effect) {
        effect->parameters[name] = value;
    }
}

// Shader management
int GraphicsSystem::create_shader(const std::string& name, ShaderType type, const std::string& source) {
    auto shader = std::make_unique<Shader>(next_shader_id++, name, type);
    shader->source_code = source;
    int id = shader->id;
    shaders.push_back(std::move(shader));
    return id;
}

void GraphicsSystem::remove_shader(int shader_id) {
    shaders.erase(std::remove_if(shaders.begin(), shaders.end(),
        [shader_id](const std::unique_ptr<Shader>& shader) {
            return shader->id == shader_id;
        }), shaders.end());
}

Shader* GraphicsSystem::get_shader(int shader_id) {
    auto it = std::find_if(shaders.begin(), shaders.end(),
        [shader_id](const std::unique_ptr<Shader>& shader) {
            return shader->id == shader_id;
        });
    return (it != shaders.end()) ? it->get() : nullptr;
}

bool GraphicsSystem::compile_shader(int shader_id) {
    Shader* shader = get_shader(shader_id);
    if (shader) {
        return compile_shader(*shader);
    }
    return false;
}

void GraphicsSystem::use_shader(int shader_id) {
    Shader* shader = get_shader(shader_id);
    if (shader && shader->compiled) {
        use_shader(*shader);
    }
}

void GraphicsSystem::set_shader_uniform_float(int shader_id, const std::string& name, float value) {
    Shader* shader = get_shader(shader_id);
    if (shader) {
        set_shader_uniform(*shader, name, value);
    }
}

void GraphicsSystem::set_shader_uniform_int(int shader_id, const std::string& name, int value) {
    Shader* shader = get_shader(shader_id);
    if (shader) {
        set_shader_uniform(*shader, name, value);
    }
}

// System update and rendering
void GraphicsSystem::update(float delta_time) {
    this->delta_time = delta_time;
    
    // Update particle systems
    for (auto& system : particle_systems) {
        if (system->active) {
            update_particle_system(*system);
        }
    }
}

void GraphicsSystem::render() {
    render_particle_systems();
    apply_post_processing();
}

void GraphicsSystem::render_particle_systems() {
    for (auto& system : particle_systems) {
        if (system->active) {
            for (const auto& particle : system->particles) {
                if (particle.active) {
                    render_particle(particle);
                }
            }
        }
    }
}

void GraphicsSystem::apply_post_processing() {
    for (auto& effect : post_effects) {
        if (effect->active) {
            apply_post_effect(*effect);
        }
    }
}

// Utility functions
int GraphicsSystem::get_particle_system_count() const {
    return static_cast<int>(particle_systems.size());
}

int GraphicsSystem::get_post_effect_count() const {
    return static_cast<int>(post_effects.size());
}

int GraphicsSystem::get_shader_count() const {
    return static_cast<int>(shaders.size());
}

void GraphicsSystem::clear_all_particle_systems() {
    particle_systems.clear();
}

void GraphicsSystem::clear_all_post_effects() {
    post_effects.clear();
}

void GraphicsSystem::clear_all_shaders() {
    shaders.clear();
}

// Pre-built effects
int GraphicsSystem::create_fire_effect(float x, float y) {
    int id = create_particle_system("fire", ParticleSystemType::FIRE);
    set_particle_system_position(id, x, y);
    return id;
}

int GraphicsSystem::create_explosion_effect(float x, float y) {
    int id = create_particle_system("explosion", ParticleSystemType::EXPLOSION);
    set_particle_system_position(id, x, y);
    return id;
}

int GraphicsSystem::create_rain_effect() {
    return create_particle_system("rain", ParticleSystemType::RAIN);
}

int GraphicsSystem::create_snow_effect() {
    return create_particle_system("snow", ParticleSystemType::SNOW);
}

int GraphicsSystem::create_sparkle_effect(float x, float y) {
    int id = create_particle_system("sparkle", ParticleSystemType::SPARKLE);
    set_particle_system_position(id, x, y);
    return id;
}

// Private helper methods
void GraphicsSystem::update_particle_system(ParticleSystem& system) {
    // Emit new particles
    if (system.emission_rate > 0) {
        system.emission_timer += delta_time;
        float emission_interval = 1.0f / system.emission_rate;
        
        while (system.emission_timer >= emission_interval && 
               static_cast<int>(system.particles.size()) < static_cast<int>(system.max_particles)) {
            emit_particle(system, 0, 0); // Position will be set by the system
            system.emission_timer -= emission_interval;
        }
    }
    
    // Update existing particles
    for (auto& particle : system.particles) {
        if (particle.active) {
            update_particle(particle, delta_time);
        }
    }
    
    // Remove dead particles
    system.particles.erase(
        std::remove_if(system.particles.begin(), system.particles.end(),
            [](const Particle& particle) { return !particle.active; }),
        system.particles.end());
}

void GraphicsSystem::emit_particle(ParticleSystem& system, float x, float y) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    Particle particle;
    particle.x = x;
    particle.y = y;
    
    // Random velocity
    std::uniform_real_distribution<float> vel_x_dist(system.velocity_min_x, system.velocity_max_x);
    std::uniform_real_distribution<float> vel_y_dist(system.velocity_min_y, system.velocity_max_y);
    particle.velocity_x = vel_x_dist(gen);
    particle.velocity_y = vel_y_dist(gen);
    
    // Random life
    std::uniform_real_distribution<float> life_dist(system.particle_life_min, system.particle_life_max);
    particle.max_life = life_dist(gen);
    particle.life = particle.max_life;
    
    // Random size
    std::uniform_real_distribution<float> size_dist(system.particle_size_min, system.particle_size_max);
    particle.size = size_dist(gen);
    
    // Color with variation
    std::uniform_real_distribution<float> color_var(-system.color_variation, system.color_variation);
    particle.color_r = std::max(0.0f, std::min(1.0f, system.color_r + color_var(gen)));
    particle.color_g = std::max(0.0f, std::min(1.0f, system.color_g + color_var(gen)));
    particle.color_b = std::max(0.0f, std::min(1.0f, system.color_b + color_var(gen)));
    particle.color_a = system.color_a;
    
    particle.active = true;
    system.particles.push_back(particle);
}

void GraphicsSystem::update_particle(Particle& particle, float dt) {
    // Update position
    particle.x += particle.velocity_x * dt;
    particle.y += particle.velocity_y * dt;
    
    // Update velocity
    particle.velocity_x += particle.acceleration_x * dt;
    particle.velocity_y += particle.acceleration_y * dt;
    
    // Update rotation
    particle.rotation += particle.angular_velocity * dt;
    
    // Update life
    particle.life -= dt;
    if (particle.life <= 0) {
        particle.active = false;
    }
    
    // Fade out (use the particle's original alpha)
    float life_ratio = particle.life / particle.max_life;
    particle.color_a = particle.color_a * life_ratio;
}

void GraphicsSystem::render_particle(const Particle& particle) {
    // This would use Raylib's drawing functions
    // For now, it's a placeholder
    (void)particle; // Suppress unused parameter warning
}

void GraphicsSystem::apply_post_effect(const PostProcessEffect& effect) {
    // This would apply the post-processing effect
    // For now, it's a placeholder
    (void)effect; // Suppress unused parameter warning
}

void GraphicsSystem::render_to_texture() {
    // This would render the scene to a texture for post-processing
    // For now, it's a placeholder
}

void GraphicsSystem::render_fullscreen_quad() {
    // This would render a fullscreen quad for post-processing
    // For now, it's a placeholder
}

bool GraphicsSystem::compile_shader(Shader& shader) {
    // This would compile the shader using Raylib's shader functions
    // For now, it's a placeholder
    shader.compiled = true;
    (void)shader; // Suppress unused parameter warning
    return true;
}

void GraphicsSystem::use_shader(const Shader& shader) {
    // This would use the shader for rendering
    // For now, it's a placeholder
    (void)shader; // Suppress unused parameter warning
}

void GraphicsSystem::set_shader_uniform(const Shader& shader, const std::string& name, float value) {
    // This would set a float uniform in the shader
    // For now, it's a placeholder
    (void)shader; (void)name; (void)value; // Suppress unused parameter warnings
}

void GraphicsSystem::set_shader_uniform(const Shader& shader, const std::string& name, int value) {
    // This would set an int uniform in the shader
    // For now, it's a placeholder
    (void)shader; (void)name; (void)value; // Suppress unused parameter warnings
}

// Native function implementations
Value graphics_init_system(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_graphics_system) {
        g_graphics_system = std::make_unique<GraphicsSystem>();
    }
    return Value::nil();
}

Value graphics_create_particle_system(const std::vector<Value>& args) {
    if (args.size() != 2 || !g_graphics_system) return Value::from_int(-1);
    
    std::string name = args[0].as_string();
    int type = args[1].as_int();
    
    ParticleSystemType system_type = static_cast<ParticleSystemType>(type);
    int system_id = g_graphics_system->create_particle_system(name, system_type);
    return Value::from_int(system_id);
}

Value graphics_remove_particle_system(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_graphics_system) return Value::nil();
    
    int system_id = args[0].as_int();
    g_graphics_system->remove_particle_system(system_id);
    return Value::nil();
}

Value graphics_set_particle_system_position(const std::vector<Value>& args) {
    if (args.size() != 3 || !g_graphics_system) return Value::nil();
    
    int system_id = args[0].as_int();
    float x = static_cast<float>(args[1].as_number());
    float y = static_cast<float>(args[2].as_number());
    
    g_graphics_system->set_particle_system_position(system_id, x, y);
    return Value::nil();
}

Value graphics_set_particle_system_emission_rate(const std::vector<Value>& args) {
    if (args.size() != 2 || !g_graphics_system) return Value::nil();
    
    int system_id = args[0].as_int();
    float rate = static_cast<float>(args[1].as_number());
    
    g_graphics_system->set_particle_system_emission_rate(system_id, rate);
    return Value::nil();
}

Value graphics_set_particle_system_color(const std::vector<Value>& args) {
    if (args.size() != 5 || !g_graphics_system) return Value::nil();
    
    int system_id = args[0].as_int();
    float r = static_cast<float>(args[1].as_number());
    float g = static_cast<float>(args[2].as_number());
    float b = static_cast<float>(args[3].as_number());
    float a = static_cast<float>(args[4].as_number());
    
    g_graphics_system->set_particle_system_color(system_id, r, g, b, a);
    return Value::nil();
}

Value graphics_create_fire_effect(const std::vector<Value>& args) {
    if (args.size() != 2 || !g_graphics_system) return Value::from_int(-1);
    
    float x = static_cast<float>(args[0].as_number());
    float y = static_cast<float>(args[1].as_number());
    
    int effect_id = g_graphics_system->create_fire_effect(x, y);
    return Value::from_int(effect_id);
}

Value graphics_create_explosion_effect(const std::vector<Value>& args) {
    if (args.size() != 2 || !g_graphics_system) return Value::from_int(-1);
    
    float x = static_cast<float>(args[0].as_number());
    float y = static_cast<float>(args[1].as_number());
    
    int effect_id = g_graphics_system->create_explosion_effect(x, y);
    return Value::from_int(effect_id);
}

Value graphics_create_post_effect(const std::vector<Value>& args) {
    if (args.size() != 2 || !g_graphics_system) return Value::from_int(-1);
    
    std::string name = args[0].as_string();
    int type = args[1].as_int();
    
    PostProcessType effect_type = static_cast<PostProcessType>(type);
    int effect_id = g_graphics_system->create_post_effect(name, effect_type);
    return Value::from_int(effect_id);
}

Value graphics_update_graphics_system(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_graphics_system) return Value::nil();
    
    float delta_time = static_cast<float>(args[0].as_number());
    g_graphics_system->update(delta_time);
    return Value::nil();
}

Value graphics_render_graphics_system(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_graphics_system) return Value::nil();
    
    g_graphics_system->render();
    return Value::nil();
}

Value graphics_get_particle_system_count(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_graphics_system) return Value::from_int(0);
    
    int count = g_graphics_system->get_particle_system_count();
    return Value::from_int(count);
}

void register_graphics_functions(FunctionRegistry& registry) {
    registry.add("INITGRAPHICS", NativeFn{"INITGRAPHICS", 0, graphics_init_system});
    registry.add("CREATEPARTICLESYSTEM", NativeFn{"CREATEPARTICLESYSTEM", 2, graphics_create_particle_system});
    registry.add("REMOVEPARTICLESYSTEM", NativeFn{"REMOVEPARTICLESYSTEM", 1, graphics_remove_particle_system});
    registry.add("SETPARTICLESYSTEMPOSITION", NativeFn{"SETPARTICLESYSTEMPOSITION", 3, graphics_set_particle_system_position});
    registry.add("SETPARTICLESYSTEMEMISSIONRATE", NativeFn{"SETPARTICLESYSTEMEMISSIONRATE", 2, graphics_set_particle_system_emission_rate});
    registry.add("SETPARTICLESYSTEMCOLOR", NativeFn{"SETPARTICLESYSTEMCOLOR", 5, graphics_set_particle_system_color});
    registry.add("CREATEFIREEFFECT", NativeFn{"CREATEFIREEFFECT", 2, graphics_create_fire_effect});
    registry.add("CREATEEXPLOSIONEFFECT", NativeFn{"CREATEEXPLOSIONEFFECT", 2, graphics_create_explosion_effect});
    registry.add("CREATEPOSTEFFECT", NativeFn{"CREATEPOSTEFFECT", 2, graphics_create_post_effect});
    registry.add("UPDATEGRAPHICSSYSTEM", NativeFn{"UPDATEGRAPHICSSYSTEM", 1, graphics_update_graphics_system});
    registry.add("RENDERGRAPHICSSYSTEM", NativeFn{"RENDERGRAPHICSSYSTEM", 0, graphics_render_graphics_system});
    registry.add("GETPARTICLESYSTEMCOUNT", NativeFn{"GETPARTICLESYSTEMCOUNT", 0, graphics_get_particle_system_count});
}

} // namespace bas

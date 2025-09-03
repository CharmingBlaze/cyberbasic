#pragma once

#include "value.hpp"
#include "runtime.hpp"
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include <cmath>

namespace bas {

// Forward declarations
struct Particle;
struct ParticleSystem;
struct PostProcessEffect;
class GraphicsSystem;

// Particle structure (2D)
struct Particle {
    float x, y;
    float velocity_x, velocity_y;
    float acceleration_x, acceleration_y;
    float life;
    float max_life;
    float size;
    float rotation;
    float angular_velocity;
    float color_r, color_g, color_b, color_a;
    bool active;
    
    Particle() : x(0), y(0), velocity_x(0), velocity_y(0), acceleration_x(0), acceleration_y(0),
                 life(0), max_life(1.0f), size(1.0f), rotation(0), angular_velocity(0),
                 color_r(1.0f), color_g(1.0f), color_b(1.0f), color_a(1.0f), active(false) {}
};

// Particle structure (3D)
struct Particle3D {
    float x, y, z;
    float velocity_x, velocity_y, velocity_z;
    float acceleration_x, acceleration_y, acceleration_z;
    float life;
    float max_life;
    float size;
    float rotation_x, rotation_y, rotation_z;
    float angular_velocity_x, angular_velocity_y, angular_velocity_z;
    float color_r, color_g, color_b, color_a;
    bool active;
    
    Particle3D() : x(0), y(0), z(0), velocity_x(0), velocity_y(0), velocity_z(0),
                   acceleration_x(0), acceleration_y(0), acceleration_z(0),
                   life(0), max_life(1.0f), size(1.0f), rotation_x(0), rotation_y(0), rotation_z(0),
                   angular_velocity_x(0), angular_velocity_y(0), angular_velocity_z(0),
                   color_r(1.0f), color_g(1.0f), color_b(1.0f), color_a(1.0f), active(false) {}
};

// Particle system types
enum class ParticleSystemType {
    FIRE,
    SMOKE,
    EXPLOSION,
    RAIN,
    SNOW,
    SPARKLE,
    CUSTOM
};

// Particle system (2D)
struct ParticleSystem {
    int id;
    std::string name;
    ParticleSystemType type;
    std::vector<Particle> particles;
    float emission_rate;
    float emission_timer;
    float life_time;
    float max_particles;
    float particle_life_min, particle_life_max;
    float particle_size_min, particle_size_max;
    float velocity_min_x, velocity_max_x;
    float velocity_min_y, velocity_max_y;
    float acceleration_x, acceleration_y;
    float color_r, color_g, color_b, color_a;
    float color_variation;
    bool loop;
    bool active;
    
    ParticleSystem(int id, const std::string& name, ParticleSystemType type) 
        : id(id), name(name), type(type), emission_rate(10.0f), emission_timer(0),
          life_time(0), max_particles(100), particle_life_min(1.0f), particle_life_max(2.0f),
          particle_size_min(1.0f), particle_size_max(3.0f), velocity_min_x(-50.0f), velocity_max_x(50.0f),
          velocity_min_y(-50.0f), velocity_max_y(50.0f), acceleration_x(0), acceleration_y(0),
          color_r(1.0f), color_g(1.0f), color_b(1.0f), color_a(1.0f), color_variation(0.2f),
          loop(true), active(true) {}
};

// Particle system (3D)
struct ParticleSystem3D {
    int id;
    std::string name;
    ParticleSystemType type;
    std::vector<Particle3D> particles;
    float emission_rate;
    float emission_timer;
    float life_time;
    float max_particles;
    float particle_life_min, particle_life_max;
    float particle_size_min, particle_size_max;
    float velocity_min_x, velocity_max_x;
    float velocity_min_y, velocity_max_y;
    float velocity_min_z, velocity_max_z;
    float acceleration_x, acceleration_y, acceleration_z;
    float color_r, color_g, color_b, color_a;
    float color_variation;
    bool loop;
    bool active;
    
    ParticleSystem3D(int id, const std::string& name, ParticleSystemType type) 
        : id(id), name(name), type(type), emission_rate(10.0f), emission_timer(0),
          life_time(0), max_particles(100), particle_life_min(1.0f), particle_life_max(2.0f),
          particle_size_min(1.0f), particle_size_max(3.0f), velocity_min_x(-50.0f), velocity_max_x(50.0f),
          velocity_min_y(-50.0f), velocity_max_y(50.0f), velocity_min_z(-50.0f), velocity_max_z(50.0f),
          acceleration_x(0), acceleration_y(0), acceleration_z(0),
          color_r(1.0f), color_g(1.0f), color_b(1.0f), color_a(1.0f), color_variation(0.2f),
          loop(true), active(true) {}
};

// Post-processing effect types
enum class PostProcessType {
    BLUR,
    SHARPEN,
    GRAYSCALE,
    SEPIA,
    INVERT,
    BLOOM,
    VIGNETTE,
    CHROMATIC_ABERRATION,
    CUSTOM
};

// Post-processing effect
struct PostProcessEffect {
    int id;
    std::string name;
    PostProcessType type;
    bool active;
    float intensity;
    std::unordered_map<std::string, float> parameters;
    
    PostProcessEffect(int id, const std::string& name, PostProcessType type)
        : id(id), name(name), type(type), active(true), intensity(1.0f) {}
};

// Shader types
enum class ShaderType {
    VERTEX,
    FRAGMENT,
    COMPUTE,
    GEOMETRY
};

// Shader
struct Shader {
    int id;
    std::string name;
    ShaderType type;
    std::string source_code;
    bool compiled;
    std::unordered_map<std::string, int> uniforms;
    
    Shader(int id, const std::string& name, ShaderType type)
        : id(id), name(name), type(type), compiled(false) {}
};

// Graphics system
class GraphicsSystem {
private:
    std::vector<std::unique_ptr<ParticleSystem>> particle_systems;
    std::vector<std::unique_ptr<ParticleSystem3D>> particle_systems3d;
    std::vector<std::unique_ptr<PostProcessEffect>> post_effects;
    std::vector<std::unique_ptr<Shader>> shaders;
    int next_particle_system_id;
    int next_post_effect_id;
    int next_shader_id;
    float delta_time;
    
    // Particle system management (2D)
    void update_particle_system(ParticleSystem& system);
    void emit_particle(ParticleSystem& system, float x, float y);
    void update_particle(Particle& particle, float dt);
    void render_particle(const Particle& particle);
    
    // Particle system management (3D)
    void update_particle_system_3d(ParticleSystem3D& system);
    void emit_particle_3d(ParticleSystem3D& system, float x, float y, float z);
    void update_particle_3d(Particle3D& particle, float dt);
    void render_particle_3d(const Particle3D& particle);
    
    // Post-processing
    void apply_post_effect(const PostProcessEffect& effect);
    void render_to_texture();
    void render_fullscreen_quad();
    
    // Shader management
    bool compile_shader(Shader& shader);
    void use_shader(const Shader& shader);
    void set_shader_uniform(const Shader& shader, const std::string& name, float value);
    void set_shader_uniform(const Shader& shader, const std::string& name, int value);
    
public:
    GraphicsSystem();
    ~GraphicsSystem();
    
    // Particle system management (2D)
    int create_particle_system(const std::string& name, ParticleSystemType type);
    void remove_particle_system(int system_id);
    ParticleSystem* get_particle_system(int system_id);
    
    // Particle system properties (2D)
    void set_particle_system_position(int system_id, float x, float y);
    void set_particle_system_emission_rate(int system_id, float rate);
    void set_particle_system_life_time(int system_id, float life_time);
    void set_particle_system_max_particles(int system_id, int max_particles);
    void set_particle_system_velocity_range(int system_id, float min_x, float max_x, float min_y, float max_y);
    void set_particle_system_size_range(int system_id, float min_size, float max_size);
    void set_particle_system_color(int system_id, float r, float g, float b, float a);
    void set_particle_system_loop(int system_id, bool loop);
    void set_particle_system_active(int system_id, bool active);
    
    // Particle system management (3D)
    int create_particle_system_3d(const std::string& name, ParticleSystemType type);
    void remove_particle_system_3d(int system_id);
    ParticleSystem3D* get_particle_system_3d(int system_id);
    
    // Particle system properties (3D)
    void set_particle_system_position_3d(int system_id, float x, float y, float z);
    void set_particle_system_emission_rate_3d(int system_id, float rate);
    void set_particle_system_life_time_3d(int system_id, float life_time);
    void set_particle_system_max_particles_3d(int system_id, int max_particles);
    void set_particle_system_velocity_range_3d(int system_id, float min_x, float max_x, float min_y, float max_y, float min_z, float max_z);
    void set_particle_system_size_range_3d(int system_id, float min_size, float max_size);
    void set_particle_system_color_3d(int system_id, float r, float g, float b, float a);
    void set_particle_system_loop_3d(int system_id, bool loop);
    void set_particle_system_active_3d(int system_id, bool active);
    
    // Post-processing effects
    int create_post_effect(const std::string& name, PostProcessType type);
    void remove_post_effect(int effect_id);
    PostProcessEffect* get_post_effect(int effect_id);
    void set_post_effect_active(int effect_id, bool active);
    void set_post_effect_intensity(int effect_id, float intensity);
    void set_post_effect_parameter(int effect_id, const std::string& name, float value);
    
    // Shader management
    int create_shader(const std::string& name, ShaderType type, const std::string& source);
    void remove_shader(int shader_id);
    Shader* get_shader(int shader_id);
    bool compile_shader(int shader_id);
    void use_shader(int shader_id);
    void set_shader_uniform_float(int shader_id, const std::string& name, float value);
    void set_shader_uniform_int(int shader_id, const std::string& name, int value);
    
    // System update and rendering
    void update(float delta_time);
    void render();
    void render_particle_systems();
    void render_particle_systems_3d();
    void apply_post_processing();
    
    // Utility functions (2D)
    int get_particle_system_count() const;
    int get_post_effect_count() const;
    int get_shader_count() const;
    void clear_all_particle_systems();
    void clear_all_post_effects();
    void clear_all_shaders();
    
    // Utility functions (3D)
    int get_particle_system_3d_count() const;
    void clear_all_particle_systems_3d();
    
    // Pre-built effects (2D)
    int create_fire_effect(float x, float y);
    int create_explosion_effect(float x, float y);
    int create_rain_effect();
    int create_snow_effect();
    int create_sparkle_effect(float x, float y);
    
    // Pre-built effects (3D)
    int create_fire_effect_3d(float x, float y, float z);
    int create_explosion_effect_3d(float x, float y, float z);
    int create_rain_effect_3d();
    int create_snow_effect_3d();
    int create_sparkle_effect_3d(float x, float y, float z);
};

// Global graphics system instance
extern std::unique_ptr<GraphicsSystem> g_graphics_system;

// Native function declarations
void register_graphics_functions(FunctionRegistry& registry);
void register_3d_camera_system_bindings(FunctionRegistry& registry);
void register_animation_system_bindings(FunctionRegistry& registry);
void register_advanced_3d_features_bindings(FunctionRegistry& registry);
void register_particle_system_bindings(FunctionRegistry& registry);
void register_post_processing_effects_bindings(FunctionRegistry& registry);
void register_game_development_apis_bindings(FunctionRegistry& registry);

} // namespace bas

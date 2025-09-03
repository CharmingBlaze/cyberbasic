#pragma once

#include "value.hpp"
#include "runtime.hpp"
#include "vector3d.hpp"
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include <cmath>

namespace bas {
struct Light3D;
class LightingSystem3D;

// Light types
enum class LightType3D {
    DIRECTIONAL,    // Directional light (like sun)
    POINT,          // Point light (like light bulb)
    SPOT,           // Spot light (like flashlight)
    AREA            // Area light (like panel light)
};

// Shadow types
enum class ShadowType {
    NONE,           // No shadows
    HARD,           // Hard shadows
    SOFT,           // Soft shadows
    PCF,            // Percentage Closer Filtering
    VSM             // Variance Shadow Maps
};

// Light3D structure
struct Light3D {
    int id;
    std::string name;
    LightType3D type;
    
    // Position and direction
    Vector3D position;
    Vector3D direction;
    Vector3D target;
    
    // Color properties
    float color_r, color_g, color_b;
    float intensity;
    float ambient_intensity;
    
    // Attenuation (for point and spot lights)
    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;
    
    // Spot light specific
    float inner_cone_angle;    // Inner cone angle in degrees
    float outer_cone_angle;    // Outer cone angle in degrees
    float falloff_exponent;
    
    // Area light specific
    float width;
    float height;
    
    // Shadow properties
    ShadowType shadow_type;
    bool cast_shadows;
    float shadow_bias;
    float shadow_normal_offset;
    int shadow_map_size;
    float shadow_far_plane;
    
    // Animation properties
    bool animated;
    float animation_speed;
    Vector3D animation_center;
    float animation_radius;
    float animation_angle;
    
    // State
    bool enabled;
    bool visible;
    
    Light3D(int id, const std::string& name, LightType3D type) 
        : id(id), name(name), type(type), position(0, 0, 0), direction(0, -1, 0), target(0, 0, 0),
          color_r(1.0f), color_g(1.0f), color_b(1.0f), intensity(1.0f), ambient_intensity(0.1f),
          constant_attenuation(1.0f), linear_attenuation(0.09f), quadratic_attenuation(0.032f),
          inner_cone_angle(30.0f), outer_cone_angle(45.0f), falloff_exponent(1.0f),
          width(1.0f), height(1.0f), shadow_type(ShadowType::SOFT), cast_shadows(false),
          shadow_bias(0.001f), shadow_normal_offset(0.0f), shadow_map_size(1024),
          shadow_far_plane(100.0f), animated(false), animation_speed(1.0f),
          animation_center(0, 0, 0), animation_radius(5.0f), animation_angle(0),
          enabled(true), visible(true) {}
};

// Lighting system manager
class LightingSystem3D {
private:
    std::vector<std::unique_ptr<Light3D>> lights;
    int next_light_id;
    
    // Global lighting properties
    Vector3D ambient_color;
    float ambient_intensity;
    bool global_shadows_enabled;
    int max_lights;
    
    // Shadow mapping
    std::vector<int> shadow_map_textures;
    int shadow_map_size;
    float shadow_bias;
    
    // Update methods
    void update_light_animation(Light3D& light, float delta_time);
    void update_shadow_maps();
    void render_shadow_map(const Light3D& light);
    
    // Utility methods
    float calculate_attenuation(const Light3D& light, float distance);
    Vector3D calculate_light_direction(const Light3D& light, const Vector3D& point);
    float calculate_spot_cone(const Light3D& light, const Vector3D& point);
    
public:
    LightingSystem3D();
    ~LightingSystem3D();
    
    // Light management
    int create_light(const std::string& name, LightType3D type);
    void remove_light(int light_id);
    Light3D* get_light(int light_id);
    
    // Light properties
    void set_light_position(int light_id, float x, float y, float z);
    void set_light_direction(int light_id, float x, float y, float z);
    void set_light_target(int light_id, float x, float y, float z);
    void set_light_color(int light_id, float r, float g, float b);
    void set_light_intensity(int light_id, float intensity);
    void set_light_ambient_intensity(int light_id, float intensity);
    
    // Attenuation (point and spot lights)
    void set_light_attenuation(int light_id, float constant, float linear, float quadratic);
    
    // Spot light specific
    void set_spot_cone_angles(int light_id, float inner_angle, float outer_angle);
    void set_spot_falloff(int light_id, float exponent);
    
    // Area light specific
    void set_area_light_size(int light_id, float width, float height);
    
    // Shadow properties
    void set_light_shadows(int light_id, bool cast_shadows, ShadowType shadow_type);
    void set_shadow_properties(int light_id, float bias, float normal_offset, int map_size);
    void set_shadow_far_plane(int light_id, float far_plane);
    
    // Animation
    void set_light_animation(int light_id, bool animated, float speed, float radius);
    void set_animation_center(int light_id, float x, float y, float z);
    
    // State
    void set_light_enabled(int light_id, bool enabled);
    void set_light_visible(int light_id, bool visible);
    
    // Global lighting
    void set_ambient_lighting(float r, float g, float b, float intensity);
    void set_global_shadows(bool enabled);
    void set_max_lights(int max_lights);
    
    // Shadow mapping
    void set_shadow_map_size(int size);
    void set_shadow_bias(float bias);
    
    // System update
    void update(float delta_time);
    void update_light(int light_id, float delta_time);
    
    // Rendering
    void render_lights();
    void render_shadows();
    void apply_lighting();
    
    // Utility functions
    Vector3D get_light_position(int light_id) const;
    Vector3D get_light_direction(int light_id) const;
    Vector3D get_light_color(int light_id) const;
    float get_light_intensity(int light_id) const;
    bool is_light_enabled(int light_id) const;
    
    // Light count
    int get_light_count() const;
    void clear_all_lights();
    
    // Pre-built light setups
    int create_sun_light(const std::string& name, float x, float y, float z);
    int create_point_light(const std::string& name, float x, float y, float z);
    int create_spot_light(const std::string& name, float x, float y, float z);
    int create_area_light(const std::string& name, float x, float y, float z);
    
    // Lighting calculations
    Vector3D calculate_lighting(const Vector3D& point, const Vector3D& normal, const Vector3D& view_direction) const;
    float calculate_shadow_factor(const Vector3D& point, int light_id) const;
    
    // Environment lighting
    void set_skybox_lighting(bool enabled);
    void set_environment_map(const std::string& file_path);
    void set_environment_intensity(float intensity);
};

// Global lighting system instance
extern std::unique_ptr<LightingSystem3D> g_lighting_system_3d;

// Native function declarations
void register_lighting3d_functions(FunctionRegistry& registry);

} // namespace bas

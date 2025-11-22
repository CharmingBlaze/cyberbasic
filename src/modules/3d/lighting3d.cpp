#include "bas/lighting3d.hpp"
#include "bas/runtime.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numbers>

namespace bas {

// Global lighting system instance
std::unique_ptr<LightingSystem3D> g_lighting_system_3d;

// LightingSystem3D implementation
LightingSystem3D::LightingSystem3D() 
    : next_light_id(0), ambient_color(0.2f, 0.2f, 0.3f), ambient_intensity(0.1f),
      global_shadows_enabled(true), max_lights(8), shadow_map_size(1024), shadow_bias(0.001f) {
}

LightingSystem3D::~LightingSystem3D() = default;

int LightingSystem3D::create_light(const std::string& name, LightType3D type) {
    auto light = std::make_unique<Light3D>(next_light_id++, name, type);
    int id = light->id;
    lights.push_back(std::move(light));
    return id;
}

void LightingSystem3D::remove_light(int light_id) {
    lights.erase(std::remove_if(lights.begin(), lights.end(),
        [light_id](const std::unique_ptr<Light3D>& light) {
            return light->id == light_id;
        }), lights.end());
}

Light3D* LightingSystem3D::get_light(int light_id) {
    auto it = std::find_if(lights.begin(), lights.end(),
        [light_id](const std::unique_ptr<Light3D>& light) {
            return light->id == light_id;
        });
    return (it != lights.end()) ? it->get() : nullptr;
}

void LightingSystem3D::set_light_position(int light_id, float x, float y, float z) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->position = Vector3D(x, y, z);
    }
}

void LightingSystem3D::set_light_direction(int light_id, float x, float y, float z) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->direction = Vector3D(x, y, z).normalized();
    }
}

void LightingSystem3D::set_light_target(int light_id, float x, float y, float z) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->target = Vector3D(x, y, z);
        light->direction = (light->target - light->position).normalized();
    }
}

void LightingSystem3D::set_light_color(int light_id, float r, float g, float b) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->color_r = r;
        light->color_g = g;
        light->color_b = b;
    }
}

void LightingSystem3D::set_light_intensity(int light_id, float intensity) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->intensity = intensity;
    }
}

void LightingSystem3D::set_light_ambient_intensity(int light_id, float intensity) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->ambient_intensity = intensity;
    }
}

void LightingSystem3D::set_light_attenuation(int light_id, float constant, float linear, float quadratic) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->constant_attenuation = constant;
        light->linear_attenuation = linear;
        light->quadratic_attenuation = quadratic;
    }
}

void LightingSystem3D::set_spot_cone_angles(int light_id, float inner_angle, float outer_angle) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->inner_cone_angle = inner_angle;
        light->outer_cone_angle = outer_angle;
    }
}

void LightingSystem3D::set_spot_falloff(int light_id, float exponent) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->falloff_exponent = exponent;
    }
}

void LightingSystem3D::set_area_light_size(int light_id, float width, float height) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->width = width;
        light->height = height;
    }
}

void LightingSystem3D::set_light_shadows(int light_id, bool cast_shadows, ShadowType shadow_type) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->cast_shadows = cast_shadows;
        light->shadow_type = shadow_type;
    }
}

void LightingSystem3D::set_shadow_properties(int light_id, float bias, float normal_offset, int map_size) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->shadow_bias = bias;
        light->shadow_normal_offset = normal_offset;
        light->shadow_map_size = map_size;
    }
}

void LightingSystem3D::set_shadow_far_plane(int light_id, float far_plane) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->shadow_far_plane = far_plane;
    }
}

void LightingSystem3D::set_light_animation(int light_id, bool animated, float speed, float radius) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->animated = animated;
        light->animation_speed = speed;
        light->animation_radius = radius;
    }
}

void LightingSystem3D::set_animation_center(int light_id, float x, float y, float z) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->animation_center = Vector3D(x, y, z);
    }
}

void LightingSystem3D::set_light_enabled(int light_id, bool enabled) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->enabled = enabled;
    }
}

void LightingSystem3D::set_light_visible(int light_id, bool visible) {
    Light3D* light = get_light(light_id);
    if (light) {
        light->visible = visible;
    }
}

void LightingSystem3D::set_ambient_lighting(float r, float g, float b, float intensity) {
    ambient_color = Vector3D(r, g, b);
    ambient_intensity = intensity;
}

void LightingSystem3D::set_global_shadows(bool enabled) {
    global_shadows_enabled = enabled;
}

void LightingSystem3D::set_max_lights(int max_lights) {
    this->max_lights = max_lights;
}

void LightingSystem3D::set_shadow_map_size(int size) {
    shadow_map_size = size;
}

void LightingSystem3D::set_shadow_bias(float bias) {
    shadow_bias = bias;
}

void LightingSystem3D::update(float delta_time) {
    for (auto& light : lights) {
        if (light->enabled && light->animated) {
            update_light_animation(*light, delta_time);
        }
    }
    
    if (global_shadows_enabled) {
        update_shadow_maps();
    }
}

void LightingSystem3D::update_light(int light_id, float delta_time) {
    Light3D* light = get_light(light_id);
    if (light && light->enabled && light->animated) {
        update_light_animation(*light, delta_time);
    }
}

void LightingSystem3D::update_light_animation(Light3D& light, float delta_time) {
    if (!light.animated) return;
    
    light.animation_angle += light.animation_speed * delta_time;
    
    // Circular animation
    float x = light.animation_center.x + std::cos(light.animation_angle) * light.animation_radius;
    float z = light.animation_center.z + std::sin(light.animation_angle) * light.animation_radius;
    
    light.position = Vector3D(x, light.animation_center.y, z);
}

void LightingSystem3D::update_shadow_maps() {
    // Simplified shadow map update
    // In a real implementation, this would render shadow maps for each light
    for (const auto& light : lights) {
        if (light->enabled && light->cast_shadows) {
            render_shadow_map(*light);
        }
    }
}

void LightingSystem3D::render_shadow_map(const Light3D& light) {
    // Simplified shadow map rendering
    // In a real implementation, this would render the scene from the light's perspective
    (void)light; // Suppress unused parameter warning
}

void LightingSystem3D::render_lights() {
    // Simplified light rendering
    // In a real implementation, this would render light volumes or apply deferred lighting
}

void LightingSystem3D::render_shadows() {
    // Simplified shadow rendering
    // In a real implementation, this would apply shadow maps to the scene
}

void LightingSystem3D::apply_lighting() {
    // Simplified lighting application
    // In a real implementation, this would apply all lights to the scene
}

Vector3D LightingSystem3D::get_light_position(int light_id) const {
    const Light3D* light = const_cast<LightingSystem3D*>(this)->get_light(light_id);
    return light ? light->position : Vector3D(0, 0, 0);
}

Vector3D LightingSystem3D::get_light_direction(int light_id) const {
    const Light3D* light = const_cast<LightingSystem3D*>(this)->get_light(light_id);
    return light ? light->direction : Vector3D(0, -1, 0);
}

Vector3D LightingSystem3D::get_light_color(int light_id) const {
    const Light3D* light = const_cast<LightingSystem3D*>(this)->get_light(light_id);
    return light ? Vector3D(light->color_r, light->color_g, light->color_b) : Vector3D(1, 1, 1);
}

float LightingSystem3D::get_light_intensity(int light_id) const {
    const Light3D* light = const_cast<LightingSystem3D*>(this)->get_light(light_id);
    return light ? light->intensity : 1.0f;
}

bool LightingSystem3D::is_light_enabled(int light_id) const {
    const Light3D* light = const_cast<LightingSystem3D*>(this)->get_light(light_id);
    return light ? light->enabled : false;
}

int LightingSystem3D::get_light_count() const {
    return static_cast<int>(lights.size());
}

void LightingSystem3D::clear_all_lights() {
    lights.clear();
}

int LightingSystem3D::create_sun_light(const std::string& name, float x, float y, float z) {
    int id = create_light(name, LightType3D::DIRECTIONAL);
    set_light_direction(id, x, y, z);
    set_light_color(id, 1.0f, 0.95f, 0.8f);
    set_light_intensity(id, 1.0f);
    set_light_shadows(id, true, ShadowType::SOFT);
    return id;
}

int LightingSystem3D::create_point_light(const std::string& name, float x, float y, float z) {
    int id = create_light(name, LightType3D::POINT);
    set_light_position(id, x, y, z);
    set_light_color(id, 1.0f, 1.0f, 1.0f);
    set_light_intensity(id, 1.0f);
    set_light_attenuation(id, 1.0f, 0.09f, 0.032f);
    set_light_shadows(id, true, ShadowType::SOFT);
    return id;
}

int LightingSystem3D::create_spot_light(const std::string& name, float x, float y, float z) {
    int id = create_light(name, LightType3D::SPOT);
    set_light_position(id, x, y, z);
    set_light_direction(id, 0, -1, 0);
    set_light_color(id, 1.0f, 1.0f, 1.0f);
    set_light_intensity(id, 1.0f);
    set_spot_cone_angles(id, 30.0f, 45.0f);
    set_light_attenuation(id, 1.0f, 0.09f, 0.032f);
    set_light_shadows(id, true, ShadowType::SOFT);
    return id;
}

int LightingSystem3D::create_area_light(const std::string& name, float x, float y, float z) {
    int id = create_light(name, LightType3D::AREA);
    set_light_position(id, x, y, z);
    set_light_color(id, 1.0f, 1.0f, 1.0f);
    set_light_intensity(id, 1.0f);
    set_area_light_size(id, 2.0f, 2.0f);
    set_light_shadows(id, true, ShadowType::SOFT);
    return id;
}

Vector3D LightingSystem3D::calculate_lighting(const Vector3D& point, const Vector3D& normal, const Vector3D& view_direction) const {
    Vector3D total_lighting = ambient_color * ambient_intensity;
    
    for (const auto& light : lights) {
        if (!light->enabled) continue;
        
        Vector3D light_direction = const_cast<LightingSystem3D*>(this)->calculate_light_direction(*light, point);
        float distance = (light->position - point).length();
        float attenuation = const_cast<LightingSystem3D*>(this)->calculate_attenuation(*light, distance);
        
        // Diffuse lighting
        float diffuse = std::max(0.0f, normal.dot(light_direction));
        Vector3D light_color = Vector3D(light->color_r, light->color_g, light->color_b);
        Vector3D diffuse_contribution = light_color * light->intensity * diffuse * attenuation;
        
        // Specular lighting (simplified)
        Vector3D reflect_direction = light_direction - normal * 2.0f * normal.dot(light_direction);
        float specular = std::pow(std::max(0.0f, view_direction.dot(reflect_direction)), 32.0f);
        Vector3D specular_contribution = light_color * light->intensity * specular * attenuation * 0.5f;
        
        total_lighting = total_lighting + diffuse_contribution + specular_contribution;
    }
    
    return total_lighting;
}

float LightingSystem3D::calculate_shadow_factor(const Vector3D& point, int light_id) const {
    (void)point; // Suppress unused parameter warning
    const Light3D* light = const_cast<LightingSystem3D*>(this)->get_light(light_id);
    if (!light || !light->cast_shadows) return 1.0f;
    
    // Simplified shadow calculation
    // In a real implementation, this would sample the shadow map
    return 1.0f;
}

void LightingSystem3D::set_skybox_lighting(bool enabled) {
    (void)enabled; // Suppress unused parameter warning
    // Simplified implementation
}

void LightingSystem3D::set_environment_map(const std::string& file_path) {
    (void)file_path; // Suppress unused parameter warning
    // Simplified implementation
}

void LightingSystem3D::set_environment_intensity(float intensity) {
    (void)intensity; // Suppress unused parameter warning
    // Simplified implementation
}

float LightingSystem3D::calculate_attenuation(const Light3D& light, float distance) {
    if (light.type == LightType3D::DIRECTIONAL) {
        return 1.0f; // No attenuation for directional lights
    }
    
    return 1.0f / (light.constant_attenuation + 
                   light.linear_attenuation * distance + 
                   light.quadratic_attenuation * distance * distance);
}

Vector3D LightingSystem3D::calculate_light_direction(const Light3D& light, const Vector3D& point) {
    switch (light.type) {
        case LightType3D::DIRECTIONAL:
            return Vector3D(-light.direction.x, -light.direction.y, -light.direction.z);
        case LightType3D::POINT:
        case LightType3D::SPOT:
        case LightType3D::AREA:
            return (light.position - point).normalized();
        default:
            return Vector3D(0, -1, 0);
    }
}

float LightingSystem3D::calculate_spot_cone(const Light3D& light, const Vector3D& point) {
    if (light.type != LightType3D::SPOT) return 1.0f;
    
    Vector3D light_direction = (light.position - point).normalized();
    float cos_angle = light.direction.dot(light_direction);
    float cos_inner = std::cos(light.inner_cone_angle * std::numbers::pi_v<float> / 180.0f);
    float cos_outer = std::cos(light.outer_cone_angle * std::numbers::pi_v<float> / 180.0f);
    
    if (cos_angle < cos_outer) return 0.0f;
    if (cos_angle > cos_inner) return 1.0f;
    
    float spot_factor = (cos_angle - cos_outer) / (cos_inner - cos_outer);
    return std::pow(spot_factor, light.falloff_exponent);
}

// Native function implementations
Value lighting3d_init_system(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_lighting_system_3d) {
        g_lighting_system_3d = std::make_unique<LightingSystem3D>();
    }
    return Value::nil();
}

Value lighting3d_create_light(const std::vector<Value>& args) {
    if (args.size() < 2) return Value::nil();
    
    std::string name = args[0].as_string();
    int type = static_cast<int>(args[1].as_int());
    
    if (!g_lighting_system_3d) return Value::nil();
    
    LightType3D light_type = static_cast<LightType3D>(type);
    int id = g_lighting_system_3d->create_light(name, light_type);
    return Value::from_int(id);
}

Value lighting3d_set_position(const std::vector<Value>& args) {
    if (args.size() < 4) return Value::nil();
    
    int light_id = static_cast<int>(args[0].as_int());
    float x = args[1].as_number();
    float y = args[2].as_number();
    float z = args[3].as_number();
    
    if (g_lighting_system_3d) {
        g_lighting_system_3d->set_light_position(light_id, x, y, z);
    }
    return Value::nil();
}

Value lighting3d_set_color(const std::vector<Value>& args) {
    if (args.size() < 4) return Value::nil();
    
    int light_id = static_cast<int>(args[0].as_int());
    float r = args[1].as_number();
    float g = args[2].as_number();
    float b = args[3].as_number();
    
    if (g_lighting_system_3d) {
        g_lighting_system_3d->set_light_color(light_id, r, g, b);
    }
    return Value::nil();
}

Value lighting3d_set_intensity(const std::vector<Value>& args) {
    if (args.size() < 2) return Value::nil();
    
    int light_id = static_cast<int>(args[0].as_int());
    float intensity = args[1].as_number();
    
    if (g_lighting_system_3d) {
        g_lighting_system_3d->set_light_intensity(light_id, intensity);
    }
    return Value::nil();
}

Value lighting3d_set_enabled(const std::vector<Value>& args) {
    if (args.size() < 2) return Value::nil();
    
    int light_id = static_cast<int>(args[0].as_int());
    bool enabled = args[1].as_bool();
    
    if (g_lighting_system_3d) {
        g_lighting_system_3d->set_light_enabled(light_id, enabled);
    }
    return Value::nil();
}

Value lighting3d_update(const std::vector<Value>& args) {
    if (args.size() < 1) return Value::nil();
    
    float delta_time = args[0].as_number();
    
    if (g_lighting_system_3d) {
        g_lighting_system_3d->update(delta_time);
    }
    return Value::nil();
}

Value lighting3d_get_count(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    
    if (g_lighting_system_3d) {
        return Value::from_int(g_lighting_system_3d->get_light_count());
    }
    return Value::from_int(0);
}

// Function registration
void register_lighting3d_functions(FunctionRegistry& registry) {
    registry.add("INITLIGHTING3D", NativeFn{"INITLIGHTING3D", 0, lighting3d_init_system});
    registry.add("CREATELIGHT3D", NativeFn{"CREATELIGHT3D", 2, lighting3d_create_light});
    registry.add("SETLIGHT3DPOSITION", NativeFn{"SETLIGHT3DPOSITION", 4, lighting3d_set_position});
    registry.add("SETLIGHT3DCOLOR", NativeFn{"SETLIGHT3DCOLOR", 4, lighting3d_set_color});
    registry.add("SETLIGHT3DINTENSITY", NativeFn{"SETLIGHT3DINTENSITY", 2, lighting3d_set_intensity});
    registry.add("SETLIGHT3DENABLED", NativeFn{"SETLIGHT3DENABLED", 2, lighting3d_set_enabled});
    registry.add("UPDATELIGHTING3D", NativeFn{"UPDATELIGHTING3D", 1, lighting3d_update});
    registry.add("GETLIGHT3DCOUNT", NativeFn{"GETLIGHT3DCOUNT", 0, lighting3d_get_count});
}

} // namespace bas

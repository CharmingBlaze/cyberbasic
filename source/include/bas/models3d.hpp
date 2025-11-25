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
struct Model3D;
struct Material3D;
struct Animation3D;
class ModelSystem3D;

#include "vector3d.hpp"

// Material types
enum class MaterialType {
    LAMBERT,        // Lambert shading
    PHONG,          // Phong shading
    BLINN_PHONG,    // Blinn-Phong shading
    PBR,            // Physically Based Rendering
    EMISSIVE        // Emissive material
};

// Material3D structure
struct Material3D {
    int id;
    std::string name;
    MaterialType type;
    
    // Base properties
    float color_r, color_g, color_b, color_a;
    std::string diffuse_texture;
    std::string normal_texture;
    std::string specular_texture;
    std::string emissive_texture;
    
    // PBR properties
    float metallic;
    float roughness;
    float ao;           // Ambient occlusion
    float clearcoat;
    float clearcoat_roughness;
    
    // Lighting properties
    float shininess;
    float specular_strength;
    float emissive_strength;
    
    // Transparency
    bool transparent;
    float alpha_cutoff;
    bool alpha_blend;
    
    // Culling
    bool cull_face;
    bool double_sided;
    
    Material3D(int id, const std::string& name, MaterialType type) 
        : id(id), name(name), type(type), color_r(1.0f), color_g(1.0f), color_b(1.0f), color_a(1.0f),
          metallic(0.0f), roughness(0.5f), ao(1.0f), clearcoat(0.0f), clearcoat_roughness(0.0f),
          shininess(32.0f), specular_strength(1.0f), emissive_strength(0.0f),
          transparent(false), alpha_cutoff(0.5f), alpha_blend(false),
          cull_face(true), double_sided(false) {}
};

// Animation types
enum class AnimationType {
    TRANSLATION,
    ROTATION,
    SCALE,
    MORPH_TARGET,
    SKELETAL
};

// Animation3D structure
struct Animation3D {
    int id;
    std::string name;
    AnimationType type;
    
    // Animation properties
    float duration;
    float current_time;
    bool loop;
    bool playing;
    float speed;
    
    // Keyframes (simplified - would be more complex in real implementation)
    std::vector<float> keyframe_times;
    std::vector<Vector3D> keyframe_values;
    
    // Skeletal animation specific
    std::vector<std::string> bone_names;
    std::vector<std::vector<Vector3D>> bone_translations;
    std::vector<std::vector<Vector3D>> bone_rotations;
    std::vector<std::vector<Vector3D>> bone_scales;
    
    Animation3D(int id, const std::string& name, AnimationType type) 
        : id(id), name(name), type(type), duration(1.0f), current_time(0.0f),
          loop(false), playing(false), speed(1.0f) {}
};

// Model3D structure
struct Model3D {
    int id;
    std::string name;
    std::string file_path;
    
    // Transform properties
    Vector3D position;
    Vector3D rotation;
    Vector3D scale;
    
    // Model properties
    bool loaded;
    bool visible;
    bool cast_shadows;
    bool receive_shadows;
    
    // Materials
    std::vector<int> material_ids;
    int active_material_id;
    
    // Animations
    std::vector<int> animation_ids;
    int active_animation_id;
    
    // LOD (Level of Detail)
    std::vector<std::string> lod_meshes;
    int current_lod;
    float lod_distance;
    
    // Bounding box
    Vector3D bounding_box_min;
    Vector3D bounding_box_max;
    
    // Physics
    bool has_physics;
    int physics_body_id;
    
    Model3D(int id, const std::string& name, const std::string& file_path) 
        : id(id), name(name), file_path(file_path), position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1),
          loaded(false), visible(true), cast_shadows(true), receive_shadows(true),
          active_material_id(-1), active_animation_id(-1), current_lod(0), lod_distance(100.0f),
          bounding_box_min(0, 0, 0), bounding_box_max(0, 0, 0),
          has_physics(false), physics_body_id(-1) {}
};

// Model system manager
class ModelSystem3D {
private:
    std::vector<std::unique_ptr<Model3D>> models;
    std::vector<std::unique_ptr<Material3D>> materials;
    std::vector<std::unique_ptr<Animation3D>> animations;
    
    int next_model_id;
    int next_material_id;
    int next_animation_id;
    
    // Model loading
    bool load_model_from_file(Model3D& model);
    void calculate_bounding_box(Model3D& model);
    void setup_lod_system(Model3D& model);
    
    // Animation system
    void update_animation(Animation3D& animation, float delta_time);
    Vector3D interpolate_keyframes(const std::vector<float>& times, const std::vector<Vector3D>& values, float time);
    
    // Material system
    void apply_material(const Material3D& material);
    void load_texture(const std::string& file_path);
    
public:
    ModelSystem3D();
    ~ModelSystem3D();
    
    // Model management
    int load_model(const std::string& name, const std::string& file_path);
    void unload_model(int model_id);
    Model3D* get_model(int model_id);
    
    // Model properties
    void set_model_position(int model_id, float x, float y, float z);
    void set_model_rotation(int model_id, float x, float y, float z);
    void set_model_scale(int model_id, float x, float y, float z);
    void set_model_visible(int model_id, bool visible);
    void set_model_shadows(int model_id, bool cast_shadows, bool receive_shadows);
    
    // Material management
    int create_material(const std::string& name, MaterialType type);
    void remove_material(int material_id);
    Material3D* get_material(int material_id);
    
    // Material properties
    void set_material_color(int material_id, float r, float g, float b, float a);
    void set_material_texture(int material_id, const std::string& texture_type, const std::string& file_path);
    void set_material_pbr_properties(int material_id, float metallic, float roughness, float ao);
    void set_material_transparency(int material_id, bool transparent, float alpha_cutoff);
    void set_material_emissive(int material_id, float strength);
    
    // Model-Material association
    void set_model_material(int model_id, int material_id);
    void set_model_materials(int model_id, const std::vector<int>& material_ids);
    
    // Animation management
    int create_animation(const std::string& name, AnimationType type);
    void remove_animation(int animation_id);
    Animation3D* get_animation(int animation_id);
    
    // Animation properties
    void set_animation_duration(int animation_id, float duration);
    void set_animation_loop(int animation_id, bool loop);
    void set_animation_speed(int animation_id, float speed);
    void add_animation_keyframe(int animation_id, float time, float x, float y, float z);
    
    // Model-Animation association
    void set_model_animation(int model_id, int animation_id);
    void set_model_animations(int model_id, const std::vector<int>& animation_ids);
    
    // Animation control
    void play_animation(int animation_id);
    void pause_animation(int animation_id);
    void stop_animation(int animation_id);
    void set_animation_time(int animation_id, float time);
    
    // LOD system
    void set_model_lod(int model_id, int lod_level);
    void set_lod_distance(int model_id, float distance);
    void add_lod_mesh(int model_id, const std::string& mesh_file);
    
    // Physics integration
    void set_model_physics(int model_id, bool has_physics, int physics_body_id);
    
    // System update
    void update(float delta_time);
    void update_model(int model_id, float delta_time);
    
    // Rendering
    void render_model(int model_id);
    void render_all_models();
    void cull_models(const Vector3D& camera_position, float cull_distance);
    
    // Utility functions
    Vector3D get_model_position(int model_id) const;
    Vector3D get_model_rotation(int model_id) const;
    Vector3D get_model_scale(int model_id) const;
    bool is_model_visible(int model_id) const;
    bool is_model_loaded(int model_id) const;
    
    // Model count
    int get_model_count() const;
    int get_material_count() const;
    int get_animation_count() const;
    void clear_all_models();
    void clear_all_materials();
    void clear_all_animations();
    
    // Pre-built model setups
    int create_cube_model(const std::string& name, float size);
    int create_sphere_model(const std::string& name, float radius);
    int create_plane_model(const std::string& name, float width, float height);
    int create_cylinder_model(const std::string& name, float radius, float height);
    
    // Pre-built material setups
    int create_metal_material(const std::string& name);
    int create_plastic_material(const std::string& name);
    int create_glass_material(const std::string& name);
    int create_emissive_material(const std::string& name);
    
    // Model utilities
    Vector3D get_model_bounding_box_min(int model_id) const;
    Vector3D get_model_bounding_box_max(int model_id) const;
    float get_model_bounding_radius(int model_id) const;
    bool is_point_inside_model(int model_id, const Vector3D& point) const;
};

// Global model system instance
extern std::unique_ptr<ModelSystem3D> g_model_system_3d;

// Native function declarations
void register_models3d_functions(FunctionRegistry& registry);

} // namespace bas

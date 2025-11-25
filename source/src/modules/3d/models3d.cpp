#include "bas/models3d.hpp"
#include "bas/runtime.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>

namespace bas {

std::unique_ptr<ModelSystem3D> g_model_system_3d;

ModelSystem3D::ModelSystem3D() : next_model_id(1), next_material_id(1), next_animation_id(1) {}
ModelSystem3D::~ModelSystem3D() = default;

// Model management
int ModelSystem3D::load_model(const std::string& name, const std::string& file_path) {
    auto model = std::make_unique<Model3D>(next_model_id++, name, file_path);
    int id = model->id;
    
    // Try to load the model
    if (load_model_from_file(*model)) {
        model->loaded = true;
        calculate_bounding_box(*model);
        setup_lod_system(*model);
    }
    
    models.push_back(std::move(model));
    return id;
}

void ModelSystem3D::unload_model(int model_id) {
    models.erase(std::remove_if(models.begin(), models.end(),
        [model_id](const std::unique_ptr<Model3D>& model) {
            return model->id == model_id;
        }), models.end());
}

Model3D* ModelSystem3D::get_model(int model_id) {
    auto it = std::find_if(models.begin(), models.end(),
        [model_id](const std::unique_ptr<Model3D>& model) {
            return model->id == model_id;
        });
    return (it != models.end()) ? it->get() : nullptr;
}

// Model properties
void ModelSystem3D::set_model_position(int model_id, float x, float y, float z) {
    Model3D* model = get_model(model_id);
    if (model) model->position = Vector3D(x, y, z);
}

void ModelSystem3D::set_model_rotation(int model_id, float x, float y, float z) {
    Model3D* model = get_model(model_id);
    if (model) model->rotation = Vector3D(x, y, z);
}

void ModelSystem3D::set_model_scale(int model_id, float x, float y, float z) {
    Model3D* model = get_model(model_id);
    if (model) model->scale = Vector3D(x, y, z);
}

void ModelSystem3D::set_model_visible(int model_id, bool visible) {
    Model3D* model = get_model(model_id);
    if (model) model->visible = visible;
}

void ModelSystem3D::set_model_shadows(int model_id, bool cast_shadows, bool receive_shadows) {
    Model3D* model = get_model(model_id);
    if (model) {
        model->cast_shadows = cast_shadows;
        model->receive_shadows = receive_shadows;
    }
}

// Material management
int ModelSystem3D::create_material(const std::string& name, MaterialType type) {
    auto material = std::make_unique<Material3D>(next_material_id++, name, type);
    int id = material->id;
    materials.push_back(std::move(material));
    return id;
}

void ModelSystem3D::remove_material(int material_id) {
    materials.erase(std::remove_if(materials.begin(), materials.end(),
        [material_id](const std::unique_ptr<Material3D>& material) {
            return material->id == material_id;
        }), materials.end());
}

Material3D* ModelSystem3D::get_material(int material_id) {
    auto it = std::find_if(materials.begin(), materials.end(),
        [material_id](const std::unique_ptr<Material3D>& material) {
            return material->id == material_id;
        });
    return (it != materials.end()) ? it->get() : nullptr;
}

// Material properties
void ModelSystem3D::set_material_color(int material_id, float r, float g, float b, float a) {
    Material3D* material = get_material(material_id);
    if (material) {
        material->color_r = r;
        material->color_g = g;
        material->color_b = b;
        material->color_a = a;
    }
}

void ModelSystem3D::set_material_texture(int material_id, const std::string& texture_type, const std::string& file_path) {
    Material3D* material = get_material(material_id);
    if (material) {
        if (texture_type == "diffuse") material->diffuse_texture = file_path;
        else if (texture_type == "normal") material->normal_texture = file_path;
        else if (texture_type == "specular") material->specular_texture = file_path;
        else if (texture_type == "emissive") material->emissive_texture = file_path;
    }
}

void ModelSystem3D::set_material_pbr_properties(int material_id, float metallic, float roughness, float ao) {
    Material3D* material = get_material(material_id);
    if (material) {
        material->metallic = metallic;
        material->roughness = roughness;
        material->ao = ao;
    }
}

void ModelSystem3D::set_material_transparency(int material_id, bool transparent, float alpha_cutoff) {
    Material3D* material = get_material(material_id);
    if (material) {
        material->transparent = transparent;
        material->alpha_cutoff = alpha_cutoff;
    }
}

void ModelSystem3D::set_material_emissive(int material_id, float strength) {
    Material3D* material = get_material(material_id);
    if (material) material->emissive_strength = strength;
}

// Model-Material association
void ModelSystem3D::set_model_material(int model_id, int material_id) {
    Model3D* model = get_model(model_id);
    if (model) {
        model->material_ids.clear();
        model->material_ids.push_back(material_id);
        model->active_material_id = material_id;
    }
}

void ModelSystem3D::set_model_materials(int model_id, const std::vector<int>& material_ids) {
    Model3D* model = get_model(model_id);
    if (model) {
        model->material_ids = material_ids;
        if (!material_ids.empty()) {
            model->active_material_id = material_ids[0];
        }
    }
}

// Animation management
int ModelSystem3D::create_animation(const std::string& name, AnimationType type) {
    auto animation = std::make_unique<Animation3D>(next_animation_id++, name, type);
    int id = animation->id;
    animations.push_back(std::move(animation));
    return id;
}

void ModelSystem3D::remove_animation(int animation_id) {
    animations.erase(std::remove_if(animations.begin(), animations.end(),
        [animation_id](const std::unique_ptr<Animation3D>& animation) {
            return animation->id == animation_id;
        }), animations.end());
}

Animation3D* ModelSystem3D::get_animation(int animation_id) {
    auto it = std::find_if(animations.begin(), animations.end(),
        [animation_id](const std::unique_ptr<Animation3D>& animation) {
            return animation->id == animation_id;
        });
    return (it != animations.end()) ? it->get() : nullptr;
}

// Animation properties
void ModelSystem3D::set_animation_duration(int animation_id, float duration) {
    Animation3D* animation = get_animation(animation_id);
    if (animation) animation->duration = duration;
}

void ModelSystem3D::set_animation_loop(int animation_id, bool loop) {
    Animation3D* animation = get_animation(animation_id);
    if (animation) animation->loop = loop;
}

void ModelSystem3D::set_animation_speed(int animation_id, float speed) {
    Animation3D* animation = get_animation(animation_id);
    if (animation) animation->speed = speed;
}

void ModelSystem3D::add_animation_keyframe(int animation_id, float time, float x, float y, float z) {
    Animation3D* animation = get_animation(animation_id);
    if (animation) {
        animation->keyframe_times.push_back(time);
        animation->keyframe_values.push_back(Vector3D(x, y, z));
    }
}

// Model-Animation association
void ModelSystem3D::set_model_animation(int model_id, int animation_id) {
    Model3D* model = get_model(model_id);
    if (model) {
        model->animation_ids.clear();
        model->animation_ids.push_back(animation_id);
        model->active_animation_id = animation_id;
    }
}

void ModelSystem3D::set_model_animations(int model_id, const std::vector<int>& animation_ids) {
    Model3D* model = get_model(model_id);
    if (model) {
        model->animation_ids = animation_ids;
        if (!animation_ids.empty()) {
            model->active_animation_id = animation_ids[0];
        }
    }
}

// Animation control
void ModelSystem3D::play_animation(int animation_id) {
    Animation3D* animation = get_animation(animation_id);
    if (animation) {
        animation->playing = true;
        animation->current_time = 0.0f;
    }
}

void ModelSystem3D::pause_animation(int animation_id) {
    Animation3D* animation = get_animation(animation_id);
    if (animation) animation->playing = false;
}

void ModelSystem3D::stop_animation(int animation_id) {
    Animation3D* animation = get_animation(animation_id);
    if (animation) {
        animation->playing = false;
        animation->current_time = 0.0f;
    }
}

void ModelSystem3D::set_animation_time(int animation_id, float time) {
    Animation3D* animation = get_animation(animation_id);
    if (animation) animation->current_time = time;
}

// LOD system
void ModelSystem3D::set_model_lod(int model_id, int lod_level) {
    Model3D* model = get_model(model_id);
    if (model && lod_level >= 0 && lod_level < static_cast<int>(model->lod_meshes.size())) {
        model->current_lod = lod_level;
    }
}

void ModelSystem3D::set_lod_distance(int model_id, float distance) {
    Model3D* model = get_model(model_id);
    if (model) model->lod_distance = distance;
}

void ModelSystem3D::add_lod_mesh(int model_id, const std::string& mesh_file) {
    Model3D* model = get_model(model_id);
    if (model) model->lod_meshes.push_back(mesh_file);
}

// Physics integration
void ModelSystem3D::set_model_physics(int model_id, bool has_physics, int physics_body_id) {
    Model3D* model = get_model(model_id);
    if (model) {
        model->has_physics = has_physics;
        model->physics_body_id = physics_body_id;
    }
}

// System update
void ModelSystem3D::update(float delta_time) {
    for (auto& model : models) {
        update_model(model->id, delta_time);
    }
}

void ModelSystem3D::update_model(int model_id, float delta_time) {
    Model3D* model = get_model(model_id);
    if (!model) return;
    
    // Update animations
    if (model->active_animation_id != -1) {
        Animation3D* animation = get_animation(model->active_animation_id);
        if (animation) {
            update_animation(*animation, delta_time);
            
            // Apply animation to model
            if (animation->type == AnimationType::TRANSLATION) {
                Vector3D translation = interpolate_keyframes(animation->keyframe_times, animation->keyframe_values, animation->current_time);
                model->position = model->position + translation;
            } else if (animation->type == AnimationType::ROTATION) {
                Vector3D rotation = interpolate_keyframes(animation->keyframe_times, animation->keyframe_values, animation->current_time);
                model->rotation = model->rotation + rotation;
            } else if (animation->type == AnimationType::SCALE) {
                Vector3D scale = interpolate_keyframes(animation->keyframe_times, animation->keyframe_values, animation->current_time);
                model->scale = model->scale + scale;
            }
        }
    }
}

// Rendering
void ModelSystem3D::render_model(int model_id) {
    Model3D* model = get_model(model_id);
    if (!model || !model->visible || !model->loaded) return;
    
    // Apply material
    if (model->active_material_id != -1) {
        Material3D* material = get_material(model->active_material_id);
        if (material) apply_material(*material);
    }
    
    // Render model (would integrate with Raylib here)
    // DrawModel(model->raylib_model, model->position, model->scale, model->rotation);
}

void ModelSystem3D::render_all_models() {
    for (auto& model : models) {
        render_model(model->id);
    }
}

void ModelSystem3D::cull_models(const Vector3D& camera_position, float cull_distance) {
    for (auto& model : models) {
        float distance = (model->position - camera_position).length();
        model->visible = distance <= cull_distance;
    }
}

// Utility functions
Vector3D ModelSystem3D::get_model_position(int model_id) const {
    const Model3D* model = const_cast<ModelSystem3D*>(this)->get_model(model_id);
    return model ? model->position : Vector3D(0, 0, 0);
}

Vector3D ModelSystem3D::get_model_rotation(int model_id) const {
    const Model3D* model = const_cast<ModelSystem3D*>(this)->get_model(model_id);
    return model ? model->rotation : Vector3D(0, 0, 0);
}

Vector3D ModelSystem3D::get_model_scale(int model_id) const {
    const Model3D* model = const_cast<ModelSystem3D*>(this)->get_model(model_id);
    return model ? model->scale : Vector3D(1, 1, 1);
}

bool ModelSystem3D::is_model_visible(int model_id) const {
    const Model3D* model = const_cast<ModelSystem3D*>(this)->get_model(model_id);
    return model ? model->visible : false;
}

bool ModelSystem3D::is_model_loaded(int model_id) const {
    const Model3D* model = const_cast<ModelSystem3D*>(this)->get_model(model_id);
    return model ? model->loaded : false;
}

// Model count
int ModelSystem3D::get_model_count() const {
    return static_cast<int>(models.size());
}

int ModelSystem3D::get_material_count() const {
    return static_cast<int>(materials.size());
}

int ModelSystem3D::get_animation_count() const {
    return static_cast<int>(animations.size());
}

void ModelSystem3D::clear_all_models() {
    models.clear();
}

void ModelSystem3D::clear_all_materials() {
    materials.clear();
}

void ModelSystem3D::clear_all_animations() {
    animations.clear();
}

// Pre-built model setups
int ModelSystem3D::create_cube_model(const std::string& name, float size) {
    auto model = std::make_unique<Model3D>(next_model_id++, name, "cube");
    model->loaded = true;
    model->scale = Vector3D(size, size, size);
    model->bounding_box_min = Vector3D(-size/2, -size/2, -size/2);
    model->bounding_box_max = Vector3D(size/2, size/2, size/2);
    
    int id = model->id;
    models.push_back(std::move(model));
    return id;
}

int ModelSystem3D::create_sphere_model(const std::string& name, float radius) {
    auto model = std::make_unique<Model3D>(next_model_id++, name, "sphere");
    model->loaded = true;
    model->scale = Vector3D(radius, radius, radius);
    model->bounding_box_min = Vector3D(-radius, -radius, -radius);
    model->bounding_box_max = Vector3D(radius, radius, radius);
    
    int id = model->id;
    models.push_back(std::move(model));
    return id;
}

int ModelSystem3D::create_plane_model(const std::string& name, float width, float height) {
    auto model = std::make_unique<Model3D>(next_model_id++, name, "plane");
    model->loaded = true;
    model->scale = Vector3D(width, 1, height);
    model->bounding_box_min = Vector3D(-width/2, 0, -height/2);
    model->bounding_box_max = Vector3D(width/2, 0, height/2);
    
    int id = model->id;
    models.push_back(std::move(model));
    return id;
}

int ModelSystem3D::create_cylinder_model(const std::string& name, float radius, float height) {
    auto model = std::make_unique<Model3D>(next_model_id++, name, "cylinder");
    model->loaded = true;
    model->scale = Vector3D(radius, height, radius);
    model->bounding_box_min = Vector3D(-radius, -height/2, -radius);
    model->bounding_box_max = Vector3D(radius, height/2, radius);
    
    int id = model->id;
    models.push_back(std::move(model));
    return id;
}

// Pre-built material setups
int ModelSystem3D::create_metal_material(const std::string& name) {
    int material_id = create_material(name, MaterialType::PBR);
    set_material_pbr_properties(material_id, 0.9f, 0.1f, 1.0f);
    set_material_color(material_id, 0.8f, 0.8f, 0.9f, 1.0f);
    return material_id;
}

int ModelSystem3D::create_plastic_material(const std::string& name) {
    int material_id = create_material(name, MaterialType::PBR);
    set_material_pbr_properties(material_id, 0.0f, 0.5f, 1.0f);
    set_material_color(material_id, 0.8f, 0.2f, 0.2f, 1.0f);
    return material_id;
}

int ModelSystem3D::create_glass_material(const std::string& name) {
    int material_id = create_material(name, MaterialType::PBR);
    set_material_pbr_properties(material_id, 0.0f, 0.0f, 1.0f);
    set_material_color(material_id, 0.9f, 0.9f, 1.0f, 0.7f);
    set_material_transparency(material_id, true, 0.5f);
    return material_id;
}

int ModelSystem3D::create_emissive_material(const std::string& name) {
    int material_id = create_material(name, MaterialType::EMISSIVE);
    set_material_color(material_id, 1.0f, 1.0f, 1.0f, 1.0f);
    set_material_emissive(material_id, 2.0f);
    return material_id;
}

// Model utilities
Vector3D ModelSystem3D::get_model_bounding_box_min(int model_id) const {
    const Model3D* model = const_cast<ModelSystem3D*>(this)->get_model(model_id);
    return model ? model->bounding_box_min : Vector3D(0, 0, 0);
}

Vector3D ModelSystem3D::get_model_bounding_box_max(int model_id) const {
    const Model3D* model = const_cast<ModelSystem3D*>(this)->get_model(model_id);
    return model ? model->bounding_box_max : Vector3D(0, 0, 0);
}

float ModelSystem3D::get_model_bounding_radius(int model_id) const {
    const Model3D* model = const_cast<ModelSystem3D*>(this)->get_model(model_id);
    if (!model) return 0.0f;
    
    Vector3D size = model->bounding_box_max - model->bounding_box_min;
    return size.length() / 2.0f;
}

bool ModelSystem3D::is_point_inside_model(int model_id, const Vector3D& point) const {
    const Model3D* model = const_cast<ModelSystem3D*>(this)->get_model(model_id);
    if (!model) return false;
    
    return point.x >= model->bounding_box_min.x && point.x <= model->bounding_box_max.x &&
           point.y >= model->bounding_box_min.y && point.y <= model->bounding_box_max.y &&
           point.z >= model->bounding_box_min.z && point.z <= model->bounding_box_max.z;
}

// Private helper methods
bool ModelSystem3D::load_model_from_file(Model3D& model) {
    // Placeholder for actual model loading
    // In a real implementation, this would load from .obj, .fbx, .gltf, etc.
    std::ifstream file(model.file_path);
    if (file.good()) {
        file.close();
        return true;
    }
    return false;
}

void ModelSystem3D::calculate_bounding_box(Model3D& model) {
    // Placeholder for bounding box calculation
    // In a real implementation, this would analyze the mesh vertices
    model.bounding_box_min = Vector3D(-1, -1, -1);
    model.bounding_box_max = Vector3D(1, 1, 1);
}

void ModelSystem3D::setup_lod_system(Model3D& model) {
    // Placeholder for LOD setup
    // In a real implementation, this would create different detail levels
    model.lod_meshes.push_back(model.file_path);
    model.current_lod = 0;
}

void ModelSystem3D::update_animation(Animation3D& animation, float delta_time) {
    if (!animation.playing) return;
    
    animation.current_time += delta_time * animation.speed;
    
    if (animation.current_time >= animation.duration) {
        if (animation.loop) {
            animation.current_time = 0.0f;
        } else {
            animation.current_time = animation.duration;
            animation.playing = false;
        }
    }
}

Vector3D ModelSystem3D::interpolate_keyframes(const std::vector<float>& times, const std::vector<Vector3D>& values, float time) {
    if (times.empty() || values.empty()) return Vector3D(0, 0, 0);
    if (times.size() != values.size()) return Vector3D(0, 0, 0);
    
    // Find the two keyframes to interpolate between
    for (size_t i = 0; i < times.size() - 1; ++i) {
        if (time >= times[i] && time <= times[i + 1]) {
            float t = (time - times[i]) / (times[i + 1] - times[i]);
            return values[i] + (values[i + 1] - values[i]) * t;
        }
    }
    
    // Return the last keyframe if time is beyond the animation
    return values.back();
}

void ModelSystem3D::apply_material(const Material3D& material) {
    // Placeholder for material application
    // In a real implementation, this would set shader uniforms and textures
    (void)material; // Suppress unused parameter warning
}

void ModelSystem3D::load_texture(const std::string& file_path) {
    // Placeholder for texture loading
    // In a real implementation, this would load and cache textures
    (void)file_path; // Suppress unused parameter warning
}

// Native function implementations
Value models3d_load_model(const std::vector<Value>& args) {
    if (!g_model_system_3d) g_model_system_3d = std::make_unique<ModelSystem3D>();
    std::string name = args[0].as_string();
    std::string file_path = args[1].as_string();
    return Value::from_int(g_model_system_3d->load_model(name, file_path));
}

Value models3d_unload_model(const std::vector<Value>& args) {
    if (!g_model_system_3d) return Value::nil();
    g_model_system_3d->unload_model(args[0].as_int());
    return Value::nil();
}

Value models3d_set_position(const std::vector<Value>& args) {
    if (!g_model_system_3d) return Value::nil();
    g_model_system_3d->set_model_position(args[0].as_int(), args[1].as_number(), args[2].as_number(), args[3].as_number());
    return Value::nil();
}

Value models3d_set_rotation(const std::vector<Value>& args) {
    if (!g_model_system_3d) return Value::nil();
    g_model_system_3d->set_model_rotation(args[0].as_int(), args[1].as_number(), args[2].as_number(), args[3].as_number());
    return Value::nil();
}

Value models3d_set_scale(const std::vector<Value>& args) {
    if (!g_model_system_3d) return Value::nil();
    g_model_system_3d->set_model_scale(args[0].as_int(), args[1].as_number(), args[2].as_number(), args[3].as_number());
    return Value::nil();
}

Value models3d_set_visible(const std::vector<Value>& args) {
    if (!g_model_system_3d) return Value::nil();
    g_model_system_3d->set_model_visible(args[0].as_int(), args[1].as_bool());
    return Value::nil();
}

Value models3d_create_material(const std::vector<Value>& args) {
    if (!g_model_system_3d) return Value::nil();
    std::string name = args[0].as_string();
    std::string type_str = args[1].as_string();
    
    MaterialType type = MaterialType::LAMBERT;
    if (type_str == "PBR") type = MaterialType::PBR;
    else if (type_str == "PHONG") type = MaterialType::PHONG;
    else if (type_str == "BLINN_PHONG") type = MaterialType::BLINN_PHONG;
    else if (type_str == "EMISSIVE") type = MaterialType::EMISSIVE;
    
    return Value::from_int(g_model_system_3d->create_material(name, type));
}

Value models3d_set_material_color(const std::vector<Value>& args) {
    if (!g_model_system_3d) return Value::nil();
    g_model_system_3d->set_material_color(args[0].as_int(), args[1].as_number(), args[2].as_number(), args[3].as_number(), args[4].as_number());
    return Value::nil();
}

Value models3d_set_model_material(const std::vector<Value>& args) {
    if (!g_model_system_3d) return Value::nil();
    g_model_system_3d->set_model_material(args[0].as_int(), args[1].as_int());
    return Value::nil();
}

Value models3d_create_animation(const std::vector<Value>& args) {
    if (!g_model_system_3d) return Value::nil();
    std::string name = args[0].as_string();
    std::string type_str = args[1].as_string();
    
    AnimationType type = AnimationType::TRANSLATION;
    if (type_str == "ROTATION") type = AnimationType::ROTATION;
    else if (type_str == "SCALE") type = AnimationType::SCALE;
    else if (type_str == "MORPH_TARGET") type = AnimationType::MORPH_TARGET;
    else if (type_str == "SKELETAL") type = AnimationType::SKELETAL;
    
    return Value::from_int(g_model_system_3d->create_animation(name, type));
}

Value models3d_play_animation(const std::vector<Value>& args) {
    if (!g_model_system_3d) return Value::nil();
    g_model_system_3d->play_animation(args[0].as_int());
    return Value::nil();
}

Value models3d_update(const std::vector<Value>& args) {
    if (!g_model_system_3d) return Value::nil();
    g_model_system_3d->update(args[0].as_number());
    return Value::nil();
}

Value models3d_render_model(const std::vector<Value>& args) {
    if (!g_model_system_3d) return Value::nil();
    g_model_system_3d->render_model(args[0].as_int());
    return Value::nil();
}

Value models3d_create_cube(const std::vector<Value>& args) {
    if (!g_model_system_3d) g_model_system_3d = std::make_unique<ModelSystem3D>();
    std::string name = args[0].as_string();
    float size = args[1].as_number();
    return Value::from_int(g_model_system_3d->create_cube_model(name, size));
}

Value models3d_create_sphere(const std::vector<Value>& args) {
    if (!g_model_system_3d) g_model_system_3d = std::make_unique<ModelSystem3D>();
    std::string name = args[0].as_string();
    float radius = args[1].as_number();
    return Value::from_int(g_model_system_3d->create_sphere_model(name, radius));
}

Value models3d_get_position(const std::vector<Value>& args) {
    if (!g_model_system_3d) return Value::nil();
    Vector3D pos = g_model_system_3d->get_model_position(args[0].as_int());
    Value::Array pos_array;
    pos_array.push_back(Value::from_number(pos.x));
    pos_array.push_back(Value::from_number(pos.y));
    pos_array.push_back(Value::from_number(pos.z));
    return Value::from_array(pos_array);
}

Value models3d_get_count(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_model_system_3d) return Value::from_int(0);
    return Value::from_int(g_model_system_3d->get_model_count());
}

void register_models3d_functions(FunctionRegistry& registry) {
    registry.add("LOADMODEL3D", NativeFn{"LOADMODEL3D", 2, models3d_load_model});
    registry.add("UNLOADMODEL3D", NativeFn{"UNLOADMODEL3D", 1, models3d_unload_model});
    registry.add("SETMODEL3DPOSITION", NativeFn{"SETMODEL3DPOSITION", 4, models3d_set_position});
    registry.add("SETMODEL3DROTATION", NativeFn{"SETMODEL3DROTATION", 4, models3d_set_rotation});
    registry.add("SETMODEL3DSCALE", NativeFn{"SETMODEL3DSCALE", 4, models3d_set_scale});
    registry.add("SETMODEL3DVISIBLE", NativeFn{"SETMODEL3DVISIBLE", 2, models3d_set_visible});
    registry.add("CREATEMATERIAL3D", NativeFn{"CREATEMATERIAL3D", 2, models3d_create_material});
    registry.add("SETMATERIAL3DCOLOR", NativeFn{"SETMATERIAL3DCOLOR", 5, models3d_set_material_color});
    registry.add("SETMODEL3DMATERIAL", NativeFn{"SETMODEL3DMATERIAL", 2, models3d_set_model_material});
    registry.add("CREATEANIMATION3D", NativeFn{"CREATEANIMATION3D", 2, models3d_create_animation});
    registry.add("PLAYANIMATION3D", NativeFn{"PLAYANIMATION3D", 1, models3d_play_animation});
    registry.add("UPDATEMODELS3D", NativeFn{"UPDATEMODELS3D", 1, models3d_update});
    registry.add("RENDERMODEL3D", NativeFn{"RENDERMODEL3D", 1, models3d_render_model});
    registry.add("CREATECUBE3D", NativeFn{"CREATECUBE3D", 2, models3d_create_cube});
    registry.add("CREATESPHERE3D", NativeFn{"CREATESPHERE3D", 2, models3d_create_sphere});
    registry.add("GETMODEL3DPOSITION", NativeFn{"GETMODEL3DPOSITION", 1, models3d_get_position});
    registry.add("GETMODELS3DCOUNT", NativeFn{"GETMODELS3DCOUNT", 0, models3d_get_count});
}

} // namespace bas

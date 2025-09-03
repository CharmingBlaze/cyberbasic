#include "bas/camera3d.hpp"
#include "bas/runtime.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace bas {

// Global camera system instance
std::unique_ptr<CameraSystem3D> g_camera_system_3d;

// CameraSystem3D implementation
CameraSystem3D::CameraSystem3D() 
    : active_camera_id(-1), next_camera_id(0), input_movement(0, 0, 0), 
      input_rotation(0, 0, 0), input_zoom(0), input_enabled(true) {
}

CameraSystem3D::~CameraSystem3D() = default;

int CameraSystem3D::create_camera(const std::string& name, CameraMode3D mode) {
    auto camera = std::make_unique<Camera3D>(next_camera_id++, name, mode);
    int id = camera->id;
    cameras.push_back(std::move(camera));
    
    if (active_camera_id == -1) {
        active_camera_id = id;
    }
    
    return id;
}

void CameraSystem3D::remove_camera(int camera_id) {
    cameras.erase(std::remove_if(cameras.begin(), cameras.end(),
        [camera_id](const std::unique_ptr<Camera3D>& camera) {
            return camera->id == camera_id;
        }), cameras.end());
    
    if (active_camera_id == camera_id) {
        active_camera_id = cameras.empty() ? -1 : cameras[0]->id;
    }
}

Camera3D* CameraSystem3D::get_camera(int camera_id) {
    auto it = std::find_if(cameras.begin(), cameras.end(),
        [camera_id](const std::unique_ptr<Camera3D>& camera) {
            return camera->id == camera_id;
        });
    return (it != cameras.end()) ? it->get() : nullptr;
}

Camera3D* CameraSystem3D::get_active_camera() {
    return get_camera(active_camera_id);
}

void CameraSystem3D::set_active_camera(int camera_id) {
    if (get_camera(camera_id)) {
        active_camera_id = camera_id;
    }
}

void CameraSystem3D::set_camera_position(int camera_id, float x, float y, float z) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->position = Vector3D(x, y, z);
        camera->target_position = Vector3D(x, y, z);
    }
}

void CameraSystem3D::set_camera_target(int camera_id, float x, float y, float z) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->target = Vector3D(x, y, z);
    }
}

void CameraSystem3D::set_camera_fov(int camera_id, float fov) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->fov = fov;
    }
}

void CameraSystem3D::set_camera_near_far(int camera_id, float near_plane, float far_plane) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->near_plane = near_plane;
        camera->far_plane = far_plane;
    }
}

void CameraSystem3D::set_camera_aspect_ratio(int camera_id, float aspect_ratio) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->aspect_ratio = aspect_ratio;
    }
}

void CameraSystem3D::set_camera_move_speed(int camera_id, float speed) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->move_speed = speed;
    }
}

void CameraSystem3D::set_camera_rotation_speed(int camera_id, float speed) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->rotation_speed = speed;
    }
}

void CameraSystem3D::set_camera_zoom_speed(int camera_id, float speed) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->zoom_speed = speed;
    }
}

void CameraSystem3D::set_camera_constraint(int camera_id, MovementConstraint constraint) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->constraint = constraint;
    }
}

void CameraSystem3D::set_orbit_distance(int camera_id, float distance) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->orbit_distance = std::max(camera->min_orbit_distance, 
                                         std::min(camera->max_orbit_distance, distance));
    }
}

void CameraSystem3D::set_orbit_angles(int camera_id, float angle_x, float angle_y) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->orbit_angle_x = angle_x;
        camera->orbit_angle_y = std::max(-89.0f, std::min(89.0f, angle_y));
    }
}

void CameraSystem3D::set_orbit_limits(int camera_id, float min_distance, float max_distance) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->min_orbit_distance = min_distance;
        camera->max_orbit_distance = max_distance;
        camera->orbit_distance = std::max(min_distance, std::min(max_distance, camera->orbit_distance));
    }
}

void CameraSystem3D::set_target_offset(int camera_id, float x, float y, float z) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->target_offset = Vector3D(x, y, z);
    }
}

void CameraSystem3D::set_follow_distance(int camera_id, float distance) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->follow_distance = distance;
    }
}

void CameraSystem3D::set_follow_height(int camera_id, float height) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->follow_height = height;
    }
}

void CameraSystem3D::add_spline_point(int camera_id, float x, float y, float z) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->spline_points.push_back(Vector3D(x, y, z));
    }
}

void CameraSystem3D::set_spline_duration(int camera_id, float duration) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->spline_duration = duration;
    }
}

void CameraSystem3D::set_spline_loop(int camera_id, bool loop) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->spline_loop = loop;
    }
}

void CameraSystem3D::start_spline_animation(int camera_id) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->spline_time = 0;
    }
}

void CameraSystem3D::stop_spline_animation(int camera_id) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->spline_time = camera->spline_duration;
    }
}

void CameraSystem3D::set_input_movement(float x, float y, float z) {
    input_movement = Vector3D(x, y, z);
}

void CameraSystem3D::set_input_rotation(float x, float y, float z) {
    input_rotation = Vector3D(x, y, z);
}

void CameraSystem3D::set_input_zoom(float zoom) {
    input_zoom = zoom;
}

void CameraSystem3D::enable_input(bool enabled) {
    input_enabled = enabled;
}

void CameraSystem3D::set_mouse_sensitivity(int camera_id, float x, float y) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->mouse_sensitivity_x = x;
        camera->mouse_sensitivity_y = y;
    }
}

void CameraSystem3D::set_gamepad_sensitivity(int camera_id, float sensitivity) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->gamepad_sensitivity = sensitivity;
    }
}

void CameraSystem3D::set_smoothing(int camera_id, bool enabled, float factor) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->smoothing_enabled = enabled;
        camera->smoothing_factor = factor;
    }
}

void CameraSystem3D::set_collision(int camera_id, bool enabled, float radius) {
    Camera3D* camera = get_camera(camera_id);
    if (camera) {
        camera->collision_enabled = enabled;
        camera->collision_radius = radius;
    }
}

void CameraSystem3D::add_collision_point(float x, float y, float z) {
    // Add collision point to all cameras (simplified implementation)
    for (auto& camera : cameras) {
        camera->collision_points.push_back(Vector3D(x, y, z));
    }
}

void CameraSystem3D::clear_collision_points() {
    for (auto& camera : cameras) {
        camera->collision_points.clear();
    }
}

void CameraSystem3D::update(float delta_time) {
    if (!input_enabled) return;
    
    Camera3D* active_camera = get_active_camera();
    if (active_camera) {
        update_camera(active_camera->id, delta_time);
    }
}

void CameraSystem3D::update_camera(int camera_id, float delta_time) {
    Camera3D* camera = get_camera(camera_id);
    if (!camera) return;
    
    switch (camera->mode) {
        case CameraMode3D::FPS:
            update_fps_camera(*camera, delta_time);
            break;
        case CameraMode3D::TPS:
            update_tps_camera(*camera, delta_time);
            break;
        case CameraMode3D::ORBIT:
            update_orbit_camera(*camera, delta_time);
            break;
        case CameraMode3D::FREE:
            update_free_camera(*camera, delta_time);
            break;
        case CameraMode3D::CINEMATIC:
            update_cinematic_camera(*camera, delta_time);
            break;
    }
    
    update_camera_vectors(*camera);
    
    if (camera->smoothing_enabled) {
        apply_smoothing(*camera, delta_time);
    }
}

void CameraSystem3D::update_camera_vectors(Camera3D& camera) {
    // Update forward vector
    camera.forward = (camera.target - camera.position).normalized();
    
    // Update right vector
    camera.right = camera.forward.cross(camera.up).normalized();
    
    // Update up vector
    camera.up = camera.right.cross(camera.forward).normalized();
}

void CameraSystem3D::update_fps_camera(Camera3D& camera, float delta_time) {
    // Apply rotation
    camera.orbit_angle_x += input_rotation.x * camera.mouse_sensitivity_x * delta_time;
    camera.orbit_angle_y += input_rotation.y * camera.mouse_sensitivity_y * delta_time;
    
    // Clamp vertical angle
    camera.orbit_angle_y = std::max(-89.0f, std::min(89.0f, camera.orbit_angle_y));
    
    // Calculate new target based on rotation
    float rad_x = camera.orbit_angle_x * M_PI / 180.0f;
    float rad_y = camera.orbit_angle_y * M_PI / 180.0f;
    
    Vector3D new_target = camera.position + Vector3D(
        std::cos(rad_y) * std::sin(rad_x),
        std::sin(rad_y),
        std::cos(rad_y) * std::cos(rad_x)
    );
    
    camera.target = new_target;
    
    // Apply movement
    Vector3D movement = Vector3D(0, 0, 0);
    movement = movement + camera.right * input_movement.x * camera.move_speed * delta_time;
    movement = movement + camera.up * input_movement.y * camera.move_speed * delta_time;
    movement = movement + camera.forward * input_movement.z * camera.move_speed * delta_time;
    
    Vector3D new_position = camera.position + movement;
    
    // Apply constraints
    if (camera.constraint == MovementConstraint::GROUND_ONLY) {
        new_position.y = camera.position.y; // Keep Y position
    }
    
    // Check collision
    if (!camera.collision_enabled || !check_collision(camera, new_position)) {
        camera.target_position = new_position;
    }
}

void CameraSystem3D::update_tps_camera(Camera3D& camera, float delta_time) {
    // Apply rotation
    camera.orbit_angle_x += input_rotation.x * camera.mouse_sensitivity_x * delta_time;
    camera.orbit_angle_y += input_rotation.y * camera.mouse_sensitivity_y * delta_time;
    
    // Clamp vertical angle
    camera.orbit_angle_y = std::max(-89.0f, std::min(89.0f, camera.orbit_angle_y));
    
    // Calculate new position based on target and rotation
    float rad_x = camera.orbit_angle_x * M_PI / 180.0f;
    float rad_y = camera.orbit_angle_y * M_PI / 180.0f;
    
    Vector3D offset = Vector3D(
        std::cos(rad_y) * std::sin(rad_x) * camera.follow_distance,
        std::sin(rad_y) * camera.follow_distance + camera.follow_height,
        std::cos(rad_y) * std::cos(rad_x) * camera.follow_distance
    );
    
    Vector3D new_position = camera.target + camera.target_offset + offset;
    
    // Check collision
    if (!camera.collision_enabled || !check_collision(camera, new_position)) {
        camera.target_position = new_position;
    }
}

void CameraSystem3D::update_orbit_camera(Camera3D& camera, float delta_time) {
    // Apply rotation
    camera.orbit_angle_x += input_rotation.x * camera.mouse_sensitivity_x * delta_time;
    camera.orbit_angle_y += input_rotation.y * camera.mouse_sensitivity_y * delta_time;
    
    // Apply zoom
    camera.orbit_distance += input_zoom * camera.zoom_speed * delta_time;
    camera.orbit_distance = std::max(camera.min_orbit_distance, 
                                    std::min(camera.max_orbit_distance, camera.orbit_distance));
    
    // Clamp vertical angle
    camera.orbit_angle_y = std::max(-89.0f, std::min(89.0f, camera.orbit_angle_y));
    
    // Calculate new position
    float rad_x = camera.orbit_angle_x * M_PI / 180.0f;
    float rad_y = camera.orbit_angle_y * M_PI / 180.0f;
    
    Vector3D new_position = camera.target + Vector3D(
        std::cos(rad_y) * std::sin(rad_x) * camera.orbit_distance,
        std::sin(rad_y) * camera.orbit_distance,
        std::cos(rad_y) * std::cos(rad_x) * camera.orbit_distance
    );
    
    camera.target_position = new_position;
}

void CameraSystem3D::update_free_camera(Camera3D& camera, float delta_time) {
    // Apply rotation
    camera.orbit_angle_x += input_rotation.x * camera.mouse_sensitivity_x * delta_time;
    camera.orbit_angle_y += input_rotation.y * camera.mouse_sensitivity_y * delta_time;
    
    // Clamp vertical angle
    camera.orbit_angle_y = std::max(-89.0f, std::min(89.0f, camera.orbit_angle_y));
    
    // Calculate new target based on rotation
    float rad_x = camera.orbit_angle_x * M_PI / 180.0f;
    float rad_y = camera.orbit_angle_y * M_PI / 180.0f;
    
    Vector3D new_target = camera.position + Vector3D(
        std::cos(rad_y) * std::sin(rad_x),
        std::sin(rad_y),
        std::cos(rad_y) * std::cos(rad_x)
    );
    
    camera.target = new_target;
    
    // Apply movement
    Vector3D movement = Vector3D(0, 0, 0);
    movement = movement + camera.right * input_movement.x * camera.move_speed * delta_time;
    movement = movement + camera.up * input_movement.y * camera.move_speed * delta_time;
    movement = movement + camera.forward * input_movement.z * camera.move_speed * delta_time;
    
    Vector3D new_position = camera.position + movement;
    
    // Check collision
    if (!camera.collision_enabled || !check_collision(camera, new_position)) {
        camera.target_position = new_position;
    }
}

void CameraSystem3D::update_cinematic_camera(Camera3D& camera, float delta_time) {
    if (camera.spline_points.size() < 2) return;
    
    camera.spline_time += delta_time;
    
    if (camera.spline_time >= camera.spline_duration) {
        if (camera.spline_loop) {
            camera.spline_time = 0;
        } else {
            camera.spline_time = camera.spline_duration;
        }
    }
    
    float t = camera.spline_time / camera.spline_duration;
    Vector3D new_position = calculate_spline_position(camera.spline_points, t);
    camera.target_position = new_position;
}

Vector3D CameraSystem3D::calculate_spline_position(const std::vector<Vector3D>& points, float t) {
    if (points.size() < 2) return Vector3D(0, 0, 0);
    
    // Simple linear interpolation between points
    float scaled_t = t * (points.size() - 1);
    int index = static_cast<int>(scaled_t);
    float local_t = scaled_t - index;
    
    if (index >= static_cast<int>(points.size()) - 1) {
        return points.back();
    }
    
    Vector3D p1 = points[index];
    Vector3D p2 = points[index + 1];
    
    return p1 + (p2 - p1) * local_t;
}

bool CameraSystem3D::check_collision(const Camera3D& camera, const Vector3D& new_position) {
    if (!camera.collision_enabled) return false;
    
    for (const auto& point : camera.collision_points) {
        float distance = (new_position - point).length();
        if (distance < camera.collision_radius) {
            return true;
        }
    }
    
    return false;
}

void CameraSystem3D::apply_smoothing(Camera3D& camera, float delta_time) {
    float lerp_factor = camera.smoothing_factor * delta_time;
    camera.position = camera.position + (camera.target_position - camera.position) * lerp_factor;
}

Vector3D CameraSystem3D::get_camera_position(int camera_id) const {
    const Camera3D* camera = const_cast<CameraSystem3D*>(this)->get_camera(camera_id);
    return camera ? camera->position : Vector3D(0, 0, 0);
}

Vector3D CameraSystem3D::get_camera_target(int camera_id) const {
    const Camera3D* camera = const_cast<CameraSystem3D*>(this)->get_camera(camera_id);
    return camera ? camera->target : Vector3D(0, 0, 0);
}

Vector3D CameraSystem3D::get_camera_forward(int camera_id) const {
    const Camera3D* camera = const_cast<CameraSystem3D*>(this)->get_camera(camera_id);
    return camera ? camera->forward : Vector3D(0, 0, -1);
}

Vector3D CameraSystem3D::get_camera_right(int camera_id) const {
    const Camera3D* camera = const_cast<CameraSystem3D*>(this)->get_camera(camera_id);
    return camera ? camera->right : Vector3D(1, 0, 0);
}

Vector3D CameraSystem3D::get_camera_up(int camera_id) const {
    const Camera3D* camera = const_cast<CameraSystem3D*>(this)->get_camera(camera_id);
    return camera ? camera->up : Vector3D(0, 1, 0);
}

float CameraSystem3D::get_camera_fov(int camera_id) const {
    const Camera3D* camera = const_cast<CameraSystem3D*>(this)->get_camera(camera_id);
    return camera ? camera->fov : 60.0f;
}

int CameraSystem3D::get_camera_count() const {
    return static_cast<int>(cameras.size());
}

void CameraSystem3D::clear_all_cameras() {
    cameras.clear();
    active_camera_id = -1;
}

int CameraSystem3D::create_fps_camera(const std::string& name, float x, float y, float z) {
    int id = create_camera(name, CameraMode3D::FPS);
    set_camera_position(id, x, y, z);
    set_camera_constraint(id, MovementConstraint::FLYING);
    return id;
}

int CameraSystem3D::create_tps_camera(const std::string& name, float x, float y, float z) {
    int id = create_camera(name, CameraMode3D::TPS);
    set_camera_position(id, x, y, z);
    set_camera_target(id, x, y, z - 1);
    return id;
}

int CameraSystem3D::create_orbit_camera(const std::string& name, float x, float y, float z, float distance) {
    int id = create_camera(name, CameraMode3D::ORBIT);
    set_camera_target(id, x, y, z);
    set_orbit_distance(id, distance);
    return id;
}

int CameraSystem3D::create_free_camera(const std::string& name, float x, float y, float z) {
    int id = create_camera(name, CameraMode3D::FREE);
    set_camera_position(id, x, y, z);
    set_camera_constraint(id, MovementConstraint::FLYING);
    return id;
}

int CameraSystem3D::create_cinematic_camera(const std::string& name) {
    int id = create_camera(name, CameraMode3D::CINEMATIC);
    set_spline_duration(id, 10.0f);
    set_spline_loop(id, false);
    return id;
}

// Native function implementations
Value camera3d_init_system(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_camera_system_3d) {
        g_camera_system_3d = std::make_unique<CameraSystem3D>();
    }
    return Value::nil();
}

Value camera3d_create_camera(const std::vector<Value>& args) {
    if (args.size() < 2) return Value::nil();
    
    std::string name = args[0].as_string();
    int mode = args[1].as_int();
    
    if (!g_camera_system_3d) return Value::nil();
    
    CameraMode3D camera_mode = static_cast<CameraMode3D>(mode);
    int id = g_camera_system_3d->create_camera(name, camera_mode);
    return Value::number(id);
}

Value camera3d_set_position(const std::vector<Value>& args) {
    if (args.size() < 4) return Value::nil();
    
    int camera_id = args[0].as_int();
    float x = args[1].as_number();
    float y = args[2].as_number();
    float z = args[3].as_number();
    
    if (g_camera_system_3d) {
        g_camera_system_3d->set_camera_position(camera_id, x, y, z);
    }
    return Value::nil();
}

Value camera3d_set_target(const std::vector<Value>& args) {
    if (args.size() < 4) return Value::nil();
    
    int camera_id = args[0].as_int();
    float x = args[1].as_number();
    float y = args[2].as_number();
    float z = args[3].as_number();
    
    if (g_camera_system_3d) {
        g_camera_system_3d->set_camera_target(camera_id, x, y, z);
    }
    return Value::nil();
}

Value camera3d_set_fov(const std::vector<Value>& args) {
    if (args.size() < 2) return Value::nil();
    
    int camera_id = args[0].as_int();
    float fov = args[1].as_number();
    
    if (g_camera_system_3d) {
        g_camera_system_3d->set_camera_fov(camera_id, fov);
    }
    return Value::nil();
}

Value camera3d_set_active(const std::vector<Value>& args) {
    if (args.size() < 1) return Value::nil();
    
    int camera_id = args[0].as_int();
    
    if (g_camera_system_3d) {
        g_camera_system_3d->set_active_camera(camera_id);
    }
    return Value::nil();
}

Value camera3d_update(const std::vector<Value>& args) {
    if (args.size() < 1) return Value::nil();
    
    float delta_time = args[0].as_number();
    
    if (g_camera_system_3d) {
        g_camera_system_3d->update(delta_time);
    }
    return Value::nil();
}

Value camera3d_get_position(const std::vector<Value>& args) {
    if (args.size() < 1) return Value::nil();
    
    int camera_id = args[0].as_int();
    
    if (g_camera_system_3d) {
        Vector3D pos = g_camera_system_3d->get_camera_position(camera_id);
        return Value::array({Value::number(pos.x), Value::number(pos.y), Value::number(pos.z)});
    }
    return Value::nil();
}

Value camera3d_get_target(const std::vector<Value>& args) {
    if (args.size() < 1) return Value::nil();
    
    int camera_id = args[0].as_int();
    
    if (g_camera_system_3d) {
        Vector3D target = g_camera_system_3d->get_camera_target(camera_id);
        return Value::array({Value::number(target.x), Value::number(target.y), Value::number(target.z)});
    }
    return Value::nil();
}

Value camera3d_get_count(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    
    if (g_camera_system_3d) {
        return Value::number(g_camera_system_3d->get_camera_count());
    }
    return Value::number(0);
}

// Function registration
void register_camera3d_functions(FunctionRegistry& registry) {
    registry.add("INITCAMERA3D", NativeFn{"INITCAMERA3D", 0, camera3d_init_system});
    registry.add("CREATECAMERA3D", NativeFn{"CREATECAMERA3D", 2, camera3d_create_camera});
    registry.add("SETCAMERA3DPOSITION", NativeFn{"SETCAMERA3DPOSITION", 4, camera3d_set_position});
    registry.add("SETCAMERA3DTARGET", NativeFn{"SETCAMERA3DTARGET", 4, camera3d_set_target});
    registry.add("SETCAMERA3DFOV", NativeFn{"SETCAMERA3DFOV", 2, camera3d_set_fov});
    registry.add("SETCAMERA3DACTIVE", NativeFn{"SETCAMERA3DACTIVE", 1, camera3d_set_active});
    registry.add("UPDATECAMERA3D", NativeFn{"UPDATECAMERA3D", 1, camera3d_update});
    registry.add("GETCAMERA3DPOSITION", NativeFn{"GETCAMERA3DPOSITION", 1, camera3d_get_position});
    registry.add("GETCAMERA3DTARGET", NativeFn{"GETCAMERA3DTARGET", 1, camera3d_get_target});
    registry.add("GETCAMERA3DCOUNT", NativeFn{"GETCAMERA3DCOUNT", 0, camera3d_get_count});
}

} // namespace bas

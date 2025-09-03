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

// Forward declarations
struct Camera3D;
class CameraSystem3D;

// Camera modes
enum class CameraMode3D {
    FPS,        // First Person Shooter
    TPS,        // Third Person Shooter
    ORBIT,      // Orbit around target
    FREE,       // Free camera
    CINEMATIC   // Cinematic camera with spline paths
};

// Camera movement constraints
enum class MovementConstraint {
    NONE,           // No constraints
    GROUND_ONLY,    // Can only move on ground plane
    FLYING,         // Can move in all directions
    CONSTRAINED     // Custom constraints
};

// Camera3D structure
struct Camera3D {
    int id;
    std::string name;
    CameraMode3D mode;
    
    // Position and orientation
    Vector3D position;
    Vector3D target;
    Vector3D up;
    Vector3D forward;
    Vector3D right;
    
    // Camera properties
    float fov;              // Field of view in degrees
    float near_plane;       // Near clipping plane
    float far_plane;        // Far clipping plane
    float aspect_ratio;     // Aspect ratio (width/height)
    
    // Movement properties
    float move_speed;       // Movement speed
    float rotation_speed;   // Rotation speed
    float zoom_speed;       // Zoom speed
    MovementConstraint constraint;
    
    // Orbit camera specific
    float orbit_distance;   // Distance from target
    float orbit_angle_x;    // Horizontal angle
    float orbit_angle_y;    // Vertical angle
    float min_orbit_distance;
    float max_orbit_distance;
    
    // TPS camera specific
    Vector3D target_offset; // Offset from target
    float follow_distance;  // Distance to follow target
    float follow_height;    // Height offset from target
    
    // Cinematic camera specific
    std::vector<Vector3D> spline_points;
    float spline_time;
    float spline_duration;
    bool spline_loop;
    
    // Input sensitivity
    float mouse_sensitivity_x;
    float mouse_sensitivity_y;
    float gamepad_sensitivity;
    
    // Smoothing
    bool smoothing_enabled;
    float smoothing_factor;
    Vector3D target_position;
    Vector3D target_rotation;
    
    // Collision detection
    bool collision_enabled;
    float collision_radius;
    std::vector<Vector3D> collision_points;
    
    Camera3D(int id, const std::string& name, CameraMode3D mode) 
        : id(id), name(name), mode(mode), position(0, 0, 0), target(0, 0, -1), up(0, 1, 0),
          forward(0, 0, -1), right(1, 0, 0), fov(60.0f), near_plane(0.1f), far_plane(1000.0f),
          aspect_ratio(16.0f/9.0f), move_speed(10.0f), rotation_speed(2.0f), zoom_speed(5.0f),
          constraint(MovementConstraint::FLYING), orbit_distance(10.0f), orbit_angle_x(0),
          orbit_angle_y(0), min_orbit_distance(1.0f), max_orbit_distance(100.0f),
          target_offset(0, 2, 0), follow_distance(5.0f), follow_height(2.0f),
          spline_time(0), spline_duration(10.0f), spline_loop(false),
          mouse_sensitivity_x(0.002f), mouse_sensitivity_y(0.002f), gamepad_sensitivity(1.0f),
          smoothing_enabled(true), smoothing_factor(0.1f), target_position(0, 0, 0),
          target_rotation(0, 0, 0), collision_enabled(false), collision_radius(0.5f) {}
};

// Camera system manager
class CameraSystem3D {
private:
    std::vector<std::unique_ptr<Camera3D>> cameras;
    int active_camera_id;
    int next_camera_id;
    
    // Input handling
    Vector3D input_movement;
    Vector3D input_rotation;
    float input_zoom;
    bool input_enabled;
    
    // Update methods
    void update_camera_vectors(Camera3D& camera);
    void update_fps_camera(Camera3D& camera, float delta_time);
    void update_tps_camera(Camera3D& camera, float delta_time);
    void update_orbit_camera(Camera3D& camera, float delta_time);
    void update_free_camera(Camera3D& camera, float delta_time);
    void update_cinematic_camera(Camera3D& camera, float delta_time);
    
    // Utility methods
    Vector3D calculate_spline_position(const std::vector<Vector3D>& points, float t);
    bool check_collision(const Camera3D& camera, const Vector3D& new_position);
    void apply_smoothing(Camera3D& camera, float delta_time);
    
public:
    CameraSystem3D();
    ~CameraSystem3D();
    
    // Camera management
    int create_camera(const std::string& name, CameraMode3D mode);
    void remove_camera(int camera_id);
    Camera3D* get_camera(int camera_id);
    Camera3D* get_active_camera();
    void set_active_camera(int camera_id);
    
    // Camera properties
    void set_camera_position(int camera_id, float x, float y, float z);
    void set_camera_target(int camera_id, float x, float y, float z);
    void set_camera_fov(int camera_id, float fov);
    void set_camera_near_far(int camera_id, float near_plane, float far_plane);
    void set_camera_aspect_ratio(int camera_id, float aspect_ratio);
    
    // Movement properties
    void set_camera_move_speed(int camera_id, float speed);
    void set_camera_rotation_speed(int camera_id, float speed);
    void set_camera_zoom_speed(int camera_id, float speed);
    void set_camera_constraint(int camera_id, MovementConstraint constraint);
    
    // Orbit camera specific
    void set_orbit_distance(int camera_id, float distance);
    void set_orbit_angles(int camera_id, float angle_x, float angle_y);
    void set_orbit_limits(int camera_id, float min_distance, float max_distance);
    
    // TPS camera specific
    void set_target_offset(int camera_id, float x, float y, float z);
    void set_follow_distance(int camera_id, float distance);
    void set_follow_height(int camera_id, float height);
    
    // Cinematic camera specific
    void add_spline_point(int camera_id, float x, float y, float z);
    void set_spline_duration(int camera_id, float duration);
    void set_spline_loop(int camera_id, bool loop);
    void start_spline_animation(int camera_id);
    void stop_spline_animation(int camera_id);
    
    // Input handling
    void set_input_movement(float x, float y, float z);
    void set_input_rotation(float x, float y, float z);
    void set_input_zoom(float zoom);
    void enable_input(bool enabled);
    
    // Sensitivity settings
    void set_mouse_sensitivity(int camera_id, float x, float y);
    void set_gamepad_sensitivity(int camera_id, float sensitivity);
    
    // Smoothing
    void set_smoothing(int camera_id, bool enabled, float factor);
    
    // Collision detection
    void set_collision(int camera_id, bool enabled, float radius);
    void add_collision_point(float x, float y, float z);
    void clear_collision_points();
    
    // System update
    void update(float delta_time);
    void update_camera(int camera_id, float delta_time);
    
    // Utility functions
    Vector3D get_camera_position(int camera_id) const;
    Vector3D get_camera_target(int camera_id) const;
    Vector3D get_camera_forward(int camera_id) const;
    Vector3D get_camera_right(int camera_id) const;
    Vector3D get_camera_up(int camera_id) const;
    float get_camera_fov(int camera_id) const;
    
    // Camera count
    int get_camera_count() const;
    void clear_all_cameras();
    
    // Pre-built camera setups
    int create_fps_camera(const std::string& name, float x, float y, float z);
    int create_tps_camera(const std::string& name, float x, float y, float z);
    int create_orbit_camera(const std::string& name, float x, float y, float z, float distance);
    int create_free_camera(const std::string& name, float x, float y, float z);
    int create_cinematic_camera(const std::string& name);
};

// Global camera system instance
extern std::unique_ptr<CameraSystem3D> g_camera_system_3d;

// Native function declarations
void register_camera3d_functions(FunctionRegistry& registry);

} // namespace bas

#pragma once

#include "value.hpp"
#include "runtime.hpp"
#include <vector>
#include <memory>
#include <functional>
#include <cmath>

namespace bas {

// Forward declarations
struct Vector2D;
struct Vector3D;
struct RigidBody;
struct PhysicsJoint;
class PhysicsWorld;

// 2D Vector
struct Vector2D {
    float x, y;
    Vector2D(float x = 0, float y = 0) : x(x), y(y) {}
    Vector2D operator+(const Vector2D& other) const { return Vector2D(x + other.x, y + other.y); }
    Vector2D operator-(const Vector2D& other) const { return Vector2D(x - other.x, y - other.y); }
    Vector2D operator*(float scalar) const { return Vector2D(x * scalar, y * scalar); }
    float length() const { return std::sqrt(x * x + y * y); }
    Vector2D normalized() const { float len = length(); return len > 0 ? Vector2D(x / len, y / len) : Vector2D(0, 0); }
};

// 3D Vector
struct Vector3D {
    float x, y, z;
    Vector3D(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    Vector3D operator+(const Vector3D& other) const { return Vector3D(x + other.x, y + other.y, z + other.z); }
    Vector3D operator-(const Vector3D& other) const { return Vector3D(x - other.x, y - other.y, z - other.z); }
    Vector3D operator*(float scalar) const { return Vector3D(x * scalar, y * scalar, z * scalar); }
    float length() const { return std::sqrt(x * x + y * y + z * z); }
    Vector3D normalized() const { float len = length(); return len > 0 ? Vector3D(x / len, y / len, z / len) : Vector3D(0, 0, 0); }
};

// Physics body types
enum class BodyType {
    STATIC,
    DYNAMIC,
    KINEMATIC
};

// Collision shapes
enum class ShapeType {
    CIRCLE,
    RECTANGLE,
    POLYGON,
    // 3D shapes
    SPHERE,
    BOX,
    CAPSULE,
    MESH
};

// Rigid body structure
struct RigidBody {
    int id;
    Vector2D position;       // 2D position
    Vector3D position3d;     // 3D position
    Vector2D velocity;       // 2D velocity
    Vector3D velocity3d;     // 3D velocity
    Vector2D acceleration;   // 2D acceleration
    Vector3D acceleration3d; // 3D acceleration
    float rotation;          // 2D rotation (around Z axis)
    Vector3D rotation3d;     // 3D rotation (Euler angles)
    float angular_velocity;  // 2D angular velocity
    Vector3D angular_velocity3d; // 3D angular velocity
    float mass;
    float friction;
    float restitution; // Bounciness
    float density;
    BodyType type;
    ShapeType shape;
    float radius; // For circle/sphere
    Vector2D size; // For rectangle
    Vector3D size3d; // For box
    float height; // For capsule
    std::vector<Vector2D> vertices; // For 2D polygon
    std::vector<Vector3D> vertices3d; // For 3D mesh
    bool is_sleeping;
    float sleep_threshold;
    bool is_3d;              // Flag to indicate 2D or 3D body
    
    RigidBody(int id, bool is_3d = false) : id(id), position(0, 0), position3d(0, 0, 0),
                       velocity(0, 0), velocity3d(0, 0, 0), acceleration(0, 0), acceleration3d(0, 0, 0),
                       rotation(0), rotation3d(0, 0, 0), angular_velocity(0), angular_velocity3d(0, 0, 0),
                       mass(1.0f), friction(0.5f), restitution(0.3f), density(1.0f), type(BodyType::DYNAMIC),
                       shape(is_3d ? ShapeType::SPHERE : ShapeType::CIRCLE), radius(10.0f), size(20, 20), 
                       size3d(20, 20, 20), height(40.0f), is_sleeping(false), sleep_threshold(0.1f), is_3d(is_3d) {}
};

// Physics joint types
enum class JointType {
    PIN,
    SPRING,
    DISTANCE,
    REVOLUTE,
    PRISMATIC,
    // 3D joint types
    BALL_SOCKET,
    HINGE,
    SLIDER,
    UNIVERSAL,
    FIXED
};

// Physics joint structure
struct PhysicsJoint {
    int id;
    JointType type;
    int body_a_id;
    int body_b_id;
    Vector2D anchor_a;       // 2D anchor point
    Vector3D anchor_a3d;     // 3D anchor point
    Vector2D anchor_b;       // 2D anchor point
    Vector3D anchor_b3d;     // 3D anchor point
    float stiffness;
    float damping;
    float rest_length;
    float max_force;
    bool active;
    bool is_3d;              // Flag to indicate 2D or 3D joint
    
    PhysicsJoint(int id, JointType type, int body_a, int body_b, bool is_3d = false) 
        : id(id), type(type), body_a_id(body_a), body_b_id(body_b),
          anchor_a(0, 0), anchor_a3d(0, 0, 0), anchor_b(0, 0), anchor_b3d(0, 0, 0),
          stiffness(100.0f), damping(10.0f), rest_length(0), max_force(1000.0f), 
          active(true), is_3d(is_3d) {}
};

// Collision detection result
struct CollisionResult {
    bool collided;
    int body_a_id;
    int body_b_id;
    Vector2D contact_point;
    Vector2D normal;
    float penetration;
    float restitution;
    float friction;
};

// Physics world class
class PhysicsWorld {
private:
    std::vector<std::unique_ptr<RigidBody>> bodies;
    std::vector<std::unique_ptr<PhysicsJoint>> joints;
    Vector2D gravity;
    float time_step;
    int iterations;
    int next_body_id;
    int next_joint_id;
    
    // Collision detection
    bool check_circle_circle(const RigidBody& a, const RigidBody& b, CollisionResult& result);
    bool check_rectangle_rectangle(const RigidBody& a, const RigidBody& b, CollisionResult& result);
    bool check_circle_rectangle(const RigidBody& a, const RigidBody& b, CollisionResult& result);
    
    // Collision resolution
    void resolve_collision(const CollisionResult& collision);
    void apply_impulse(RigidBody& body, const Vector2D& impulse, const Vector2D& contact_point);
    
    // Joint resolution
    void resolve_joints();
    void resolve_pin_joint(PhysicsJoint& joint);
    void resolve_spring_joint(PhysicsJoint& joint);
    void resolve_distance_joint(PhysicsJoint& joint);
    
public:
    PhysicsWorld();
    ~PhysicsWorld();
    
    // World settings
    void set_gravity(float x, float y);
    void set_time_step(float step);
    void set_iterations(int iter);
    
    // Body management
    int create_body(BodyType type, float x, float y);
    int create_body_3d(BodyType type, float x, float y, float z);
    void remove_body(int body_id);
    RigidBody* get_body(int body_id);
    
    // Body properties (2D)
    void set_body_position(int body_id, float x, float y);
    void set_body_velocity(int body_id, float x, float y);
    void set_body_mass(int body_id, float mass);
    void set_body_friction(int body_id, float friction);
    void set_body_restitution(int body_id, float restitution);
    void set_body_density(int body_id, float density);
    
    // Body properties (3D)
    void set_body_position_3d(int body_id, float x, float y, float z);
    void set_body_velocity_3d(int body_id, float x, float y, float z);
    void set_body_rotation_3d(int body_id, float x, float y, float z);
    void set_body_angular_velocity_3d(int body_id, float x, float y, float z);
    
    // Shape management (2D)
    void set_circle_shape(int body_id, float radius);
    void set_rectangle_shape(int body_id, float width, float height);
    void set_polygon_shape(int body_id, const std::vector<Vector2D>& vertices);
    
    // Shape management (3D)
    void set_sphere_shape(int body_id, float radius);
    void set_box_shape(int body_id, float width, float height, float depth);
    void set_capsule_shape(int body_id, float radius, float height);
    void set_mesh_shape(int body_id, const std::vector<Vector3D>& vertices);
    
    // Forces and impulses (2D)
    void apply_force(int body_id, float x, float y);
    void apply_impulse(int body_id, float x, float y);
    void apply_torque(int body_id, float torque);
    void apply_force_at_point(int body_id, float force_x, float force_y, float point_x, float point_y);
    
    // Forces and impulses (3D)
    void apply_force_3d(int body_id, float x, float y, float z);
    void apply_impulse_3d(int body_id, float x, float y, float z);
    void apply_torque_3d(int body_id, float x, float y, float z);
    void apply_force_at_point_3d(int body_id, float force_x, float force_y, float force_z, 
                                 float point_x, float point_y, float point_z);
    
    // Joint management (2D)
    int create_pin_joint(int body_a, int body_b, float anchor_x, float anchor_y);
    int create_spring_joint(int body_a, int body_b, float stiffness, float damping);
    int create_distance_joint(int body_a, int body_b, float distance);
    
    // Joint management (3D)
    int create_ball_socket_joint(int body_a, int body_b, float anchor_x, float anchor_y, float anchor_z);
    int create_hinge_joint(int body_a, int body_b, float anchor_x, float anchor_y, float anchor_z, 
                          float axis_x, float axis_y, float axis_z);
    int create_slider_joint(int body_a, int body_b, float axis_x, float axis_y, float axis_z);
    int create_fixed_joint(int body_a, int body_b);
    
    void remove_joint(int joint_id);
    PhysicsJoint* get_joint(int joint_id);
    
    // Simulation
    void step();
    void update(float delta_time);
    
    // Collision detection
    std::vector<CollisionResult> get_collisions();
    bool check_collision(int body_a_id, int body_b_id);
    
    // 3D collision detection
    bool check_sphere_sphere(const RigidBody& a, const RigidBody& b, CollisionResult& result);
    bool check_box_box(const RigidBody& a, const RigidBody& b, CollisionResult& result);
    bool check_sphere_box(const RigidBody& a, const RigidBody& b, CollisionResult& result);
    
    // Utility functions (2D)
    Vector2D get_body_position(int body_id);
    Vector2D get_body_velocity(int body_id);
    float get_body_rotation(int body_id);
    
    // Utility functions (3D)
    Vector3D get_body_position_3d(int body_id);
    Vector3D get_body_velocity_3d(int body_id);
    Vector3D get_body_rotation_3d(int body_id);
    
    // General utility functions
    int get_body_count();
    int get_joint_count();
};

// Global physics world instance
extern std::unique_ptr<PhysicsWorld> g_physics_world;

// Native function declarations
void register_physics_functions(FunctionRegistry& registry);

} // namespace bas

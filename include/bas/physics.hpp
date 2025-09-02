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
    POLYGON
};

// Rigid body structure
struct RigidBody {
    int id;
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
    float rotation;
    float angular_velocity;
    float mass;
    float friction;
    float restitution; // Bounciness
    float density;
    BodyType type;
    ShapeType shape;
    float radius; // For circle
    Vector2D size; // For rectangle
    std::vector<Vector2D> vertices; // For polygon
    bool is_sleeping;
    float sleep_threshold;
    
    RigidBody(int id) : id(id), position(0, 0), velocity(0, 0), acceleration(0, 0),
                       rotation(0), angular_velocity(0), mass(1.0f), friction(0.5f),
                       restitution(0.3f), density(1.0f), type(BodyType::DYNAMIC),
                       shape(ShapeType::CIRCLE), radius(10.0f), size(20, 20),
                       is_sleeping(false), sleep_threshold(0.1f) {}
};

// Physics joint types
enum class JointType {
    PIN,
    SPRING,
    DISTANCE,
    REVOLUTE,
    PRISMATIC
};

// Physics joint structure
struct PhysicsJoint {
    int id;
    JointType type;
    int body_a_id;
    int body_b_id;
    Vector2D anchor_a;
    Vector2D anchor_b;
    float stiffness;
    float damping;
    float rest_length;
    float max_force;
    bool active;
    
    PhysicsJoint(int id, JointType type, int body_a, int body_b) 
        : id(id), type(type), body_a_id(body_a), body_b_id(body_b),
          anchor_a(0, 0), anchor_b(0, 0), stiffness(100.0f), damping(10.0f),
          rest_length(0), max_force(1000.0f), active(true) {}
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
    void remove_body(int body_id);
    RigidBody* get_body(int body_id);
    
    // Body properties
    void set_body_position(int body_id, float x, float y);
    void set_body_velocity(int body_id, float x, float y);
    void set_body_mass(int body_id, float mass);
    void set_body_friction(int body_id, float friction);
    void set_body_restitution(int body_id, float restitution);
    void set_body_density(int body_id, float density);
    
    // Shape management
    void set_circle_shape(int body_id, float radius);
    void set_rectangle_shape(int body_id, float width, float height);
    void set_polygon_shape(int body_id, const std::vector<Vector2D>& vertices);
    
    // Forces and impulses
    void apply_force(int body_id, float x, float y);
    void apply_impulse(int body_id, float x, float y);
    void apply_torque(int body_id, float torque);
    void apply_force_at_point(int body_id, float force_x, float force_y, float point_x, float point_y);
    
    // Joint management
    int create_pin_joint(int body_a, int body_b, float anchor_x, float anchor_y);
    int create_spring_joint(int body_a, int body_b, float stiffness, float damping);
    int create_distance_joint(int body_a, int body_b, float distance);
    void remove_joint(int joint_id);
    PhysicsJoint* get_joint(int joint_id);
    
    // Simulation
    void step();
    void update(float delta_time);
    
    // Collision detection
    std::vector<CollisionResult> get_collisions();
    bool check_collision(int body_a_id, int body_b_id);
    
    // Utility functions
    Vector2D get_body_position(int body_id);
    Vector2D get_body_velocity(int body_id);
    float get_body_rotation(int body_id);
    int get_body_count();
    int get_joint_count();
};

// Global physics world instance
extern std::unique_ptr<PhysicsWorld> g_physics_world;

// Native function declarations
void register_physics_functions(FunctionRegistry& registry);

} // namespace bas

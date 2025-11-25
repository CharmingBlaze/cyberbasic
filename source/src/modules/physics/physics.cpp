#include "bas/physics.hpp"
#include "bas/runtime.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace bas {

// Global physics world instance
std::unique_ptr<PhysicsWorld> g_physics_world;

// PhysicsWorld implementation
PhysicsWorld::PhysicsWorld() 
    : gravity(0, 9.81f), time_step(1.0f/60.0f), iterations(10), 
      next_body_id(0), next_joint_id(0) {
}

PhysicsWorld::~PhysicsWorld() = default;

void PhysicsWorld::set_gravity(float x, float y) {
    gravity = Vector2D(x, y);
}

void PhysicsWorld::set_time_step(float step) {
    time_step = step;
}

void PhysicsWorld::set_iterations(int iter) {
    iterations = iter;
}

int PhysicsWorld::create_body(BodyType type, float x, float y) {
    auto body = std::make_unique<RigidBody>(next_body_id++, false);
    body->position = Vector2D(x, y);
    body->type = type;
    
    if (type == BodyType::STATIC) {
        body->mass = 0.0f; // Infinite mass for static bodies
    }
    
    int id = body->id;
    bodies.push_back(std::move(body));
    return id;
}

int PhysicsWorld::create_body_3d(BodyType type, float x, float y, float z) {
    auto body = std::make_unique<RigidBody>(next_body_id++, true);
    body->position3d = Vector3D(x, y, z);
    body->type = type;
    
    if (type == BodyType::STATIC) {
        body->mass = 0.0f; // Infinite mass for static bodies
    }
    
    int id = body->id;
    bodies.push_back(std::move(body));
    return id;
}

void PhysicsWorld::remove_body(int body_id) {
    bodies.erase(std::remove_if(bodies.begin(), bodies.end(),
        [body_id](const std::unique_ptr<RigidBody>& body) {
            return body->id == body_id;
        }), bodies.end());
}

RigidBody* PhysicsWorld::get_body(int body_id) {
    auto it = std::find_if(bodies.begin(), bodies.end(),
        [body_id](const std::unique_ptr<RigidBody>& body) {
            return body->id == body_id;
        });
    return (it != bodies.end()) ? it->get() : nullptr;
}

void PhysicsWorld::set_body_position(int body_id, float x, float y) {
    RigidBody* body = get_body(body_id);
    if (body) {
        body->position = Vector2D(x, y);
    }
}

void PhysicsWorld::set_body_velocity(int body_id, float x, float y) {
    RigidBody* body = get_body(body_id);
    if (body) {
        body->velocity = Vector2D(x, y);
    }
}

void PhysicsWorld::set_body_mass(int body_id, float mass) {
    RigidBody* body = get_body(body_id);
    if (body && body->type != BodyType::STATIC) {
        body->mass = mass;
    }
}

void PhysicsWorld::set_body_friction(int body_id, float friction) {
    RigidBody* body = get_body(body_id);
    if (body) {
        body->friction = friction;
    }
}

void PhysicsWorld::set_body_restitution(int body_id, float restitution) {
    RigidBody* body = get_body(body_id);
    if (body) {
        body->restitution = restitution;
    }
}

void PhysicsWorld::set_body_density(int body_id, float density) {
    RigidBody* body = get_body(body_id);
    if (body) {
        body->density = density;
    }
}

// 3D body property methods
void PhysicsWorld::set_body_position_3d(int body_id, float x, float y, float z) {
    RigidBody* body = get_body(body_id);
    if (body && body->is_3d) {
        body->position3d = Vector3D(x, y, z);
    }
}

void PhysicsWorld::set_body_velocity_3d(int body_id, float x, float y, float z) {
    RigidBody* body = get_body(body_id);
    if (body && body->is_3d) {
        body->velocity3d = Vector3D(x, y, z);
    }
}

void PhysicsWorld::set_body_rotation_3d(int body_id, float x, float y, float z) {
    RigidBody* body = get_body(body_id);
    if (body && body->is_3d) {
        body->rotation3d = Vector3D(x, y, z);
    }
}

void PhysicsWorld::set_body_angular_velocity_3d(int body_id, float x, float y, float z) {
    RigidBody* body = get_body(body_id);
    if (body && body->is_3d) {
        body->angular_velocity3d = Vector3D(x, y, z);
    }
}

void PhysicsWorld::set_circle_shape(int body_id, float radius) {
    RigidBody* body = get_body(body_id);
    if (body) {
        body->shape = ShapeType::CIRCLE;
        body->radius = radius;
    }
}

void PhysicsWorld::set_rectangle_shape(int body_id, float width, float height) {
    RigidBody* body = get_body(body_id);
    if (body) {
        body->shape = ShapeType::RECTANGLE;
        body->size = Vector2D(width, height);
    }
}

void PhysicsWorld::set_polygon_shape(int body_id, const std::vector<Vector2D>& vertices) {
    RigidBody* body = get_body(body_id);
    if (body) {
        body->shape = ShapeType::POLYGON;
        body->vertices = vertices;
    }
}

// 3D shape methods
void PhysicsWorld::set_sphere_shape(int body_id, float radius) {
    RigidBody* body = get_body(body_id);
    if (body && body->is_3d) {
        body->shape = ShapeType::SPHERE;
        body->radius = radius;
    }
}

void PhysicsWorld::set_box_shape(int body_id, float width, float height, float depth) {
    RigidBody* body = get_body(body_id);
    if (body && body->is_3d) {
        body->shape = ShapeType::BOX;
        body->size3d = Vector3D(width, height, depth);
    }
}

void PhysicsWorld::set_capsule_shape(int body_id, float radius, float height) {
    RigidBody* body = get_body(body_id);
    if (body && body->is_3d) {
        body->shape = ShapeType::CAPSULE;
        body->radius = radius;
        body->height = height;
    }
}

void PhysicsWorld::set_mesh_shape(int body_id, const std::vector<Vector3D>& vertices) {
    RigidBody* body = get_body(body_id);
    if (body && body->is_3d) {
        body->shape = ShapeType::MESH;
        body->vertices3d = vertices;
    }
}

void PhysicsWorld::apply_force(int body_id, float x, float y) {
    RigidBody* body = get_body(body_id);
    if (body && body->type == BodyType::DYNAMIC) {
        body->acceleration = body->acceleration + Vector2D(x, y) * (1.0f / body->mass);
    }
}

void PhysicsWorld::apply_impulse(int body_id, float x, float y) {
    RigidBody* body = get_body(body_id);
    if (body && body->type == BodyType::DYNAMIC) {
        body->velocity = body->velocity + Vector2D(x, y) * (1.0f / body->mass);
    }
}

void PhysicsWorld::apply_torque(int body_id, float torque) {
    RigidBody* body = get_body(body_id);
    if (body && body->type == BodyType::DYNAMIC) {
        body->angular_velocity += torque * (1.0f / body->mass);
    }
}

void PhysicsWorld::apply_force_at_point(int body_id, float force_x, float force_y, float point_x, float point_y) {
    RigidBody* body = get_body(body_id);
    if (body && body->type == BodyType::DYNAMIC) {
        Vector2D force(force_x, force_y);
        Vector2D point(point_x, point_y);
        Vector2D r = point - body->position;
        
        // Apply linear force
        body->acceleration = body->acceleration + force * (1.0f / body->mass);
        
        // Apply angular force (torque)
        float torque = r.x * force.y - r.y * force.x;
        body->angular_velocity += torque * (1.0f / body->mass);
    }
}

// 3D force methods
void PhysicsWorld::apply_force_3d(int body_id, float x, float y, float z) {
    RigidBody* body = get_body(body_id);
    if (body && body->type == BodyType::DYNAMIC && body->is_3d) {
        body->acceleration3d = body->acceleration3d + Vector3D(x, y, z) * (1.0f / body->mass);
    }
}

void PhysicsWorld::apply_impulse_3d(int body_id, float x, float y, float z) {
    RigidBody* body = get_body(body_id);
    if (body && body->type == BodyType::DYNAMIC && body->is_3d) {
        body->velocity3d = body->velocity3d + Vector3D(x, y, z) * (1.0f / body->mass);
    }
}

void PhysicsWorld::apply_torque_3d(int body_id, float x, float y, float z) {
    RigidBody* body = get_body(body_id);
    if (body && body->type == BodyType::DYNAMIC && body->is_3d) {
        body->angular_velocity3d = body->angular_velocity3d + Vector3D(x, y, z) * (1.0f / body->mass);
    }
}

void PhysicsWorld::apply_force_at_point_3d(int body_id, float force_x, float force_y, float force_z, 
                                           float point_x, float point_y, float point_z) {
    RigidBody* body = get_body(body_id);
    if (body && body->type == BodyType::DYNAMIC && body->is_3d) {
        Vector3D force(force_x, force_y, force_z);
        Vector3D point(point_x, point_y, point_z);
        Vector3D r = point - body->position3d;
        
        // Apply linear force
        body->acceleration3d = body->acceleration3d + force * (1.0f / body->mass);
        
        // Apply angular force (torque) - cross product
        Vector3D torque = Vector3D(
            r.y * force.z - r.z * force.y,
            r.z * force.x - r.x * force.z,
            r.x * force.y - r.y * force.x
        );
        body->angular_velocity3d = body->angular_velocity3d + torque * (1.0f / body->mass);
    }
}

int PhysicsWorld::create_pin_joint(int body_a, int body_b, float anchor_x, float anchor_y) {
    auto joint = std::make_unique<PhysicsJoint>(next_joint_id++, JointType::PIN, body_a, body_b);
    joint->anchor_a = Vector2D(anchor_x, anchor_y);
    joint->anchor_b = Vector2D(anchor_x, anchor_y);
    
    int id = joint->id;
    joints.push_back(std::move(joint));
    return id;
}

int PhysicsWorld::create_spring_joint(int body_a, int body_b, float stiffness, float damping) {
    auto joint = std::make_unique<PhysicsJoint>(next_joint_id++, JointType::SPRING, body_a, body_b);
    joint->stiffness = stiffness;
    joint->damping = damping;
    
    // Calculate rest length
    RigidBody* body_a_ptr = get_body(body_a);
    RigidBody* body_b_ptr = get_body(body_b);
    if (body_a_ptr && body_b_ptr) {
        joint->rest_length = (body_b_ptr->position - body_a_ptr->position).length();
    }
    
    int id = joint->id;
    joints.push_back(std::move(joint));
    return id;
}

int PhysicsWorld::create_distance_joint(int body_a, int body_b, float distance) {
    auto joint = std::make_unique<PhysicsJoint>(next_joint_id++, JointType::DISTANCE, body_a, body_b);
    joint->rest_length = distance;
    
    int id = joint->id;
    joints.push_back(std::move(joint));
    return id;
}

// 3D joint methods
int PhysicsWorld::create_ball_socket_joint(int body_a, int body_b, float anchor_x, float anchor_y, float anchor_z) {
    auto joint = std::make_unique<PhysicsJoint>(next_joint_id++, JointType::BALL_SOCKET, body_a, body_b, true);
    joint->anchor_a3d = Vector3D(anchor_x, anchor_y, anchor_z);
    joint->anchor_b3d = Vector3D(anchor_x, anchor_y, anchor_z);
    
    int id = joint->id;
    joints.push_back(std::move(joint));
    return id;
}

int PhysicsWorld::create_hinge_joint(int body_a, int body_b, float anchor_x, float anchor_y, float anchor_z, 
                                    float axis_x, float axis_y, float axis_z) {
    auto joint = std::make_unique<PhysicsJoint>(next_joint_id++, JointType::HINGE, body_a, body_b, true);
    joint->anchor_a3d = Vector3D(anchor_x, anchor_y, anchor_z);
    joint->anchor_b3d = Vector3D(anchor_x, anchor_y, anchor_z);
    // Store axis in anchor_b3d for now (could add separate axis field later)
    joint->anchor_b3d = Vector3D(axis_x, axis_y, axis_z);
    
    int id = joint->id;
    joints.push_back(std::move(joint));
    return id;
}

int PhysicsWorld::create_slider_joint(int body_a, int body_b, float axis_x, float axis_y, float axis_z) {
    auto joint = std::make_unique<PhysicsJoint>(next_joint_id++, JointType::SLIDER, body_a, body_b, true);
    // Store axis in anchor_a3d for now
    joint->anchor_a3d = Vector3D(axis_x, axis_y, axis_z);
    
    int id = joint->id;
    joints.push_back(std::move(joint));
    return id;
}

int PhysicsWorld::create_fixed_joint(int body_a, int body_b) {
    auto joint = std::make_unique<PhysicsJoint>(next_joint_id++, JointType::FIXED, body_a, body_b, true);
    
    int id = joint->id;
    joints.push_back(std::move(joint));
    return id;
}

void PhysicsWorld::remove_joint(int joint_id) {
    joints.erase(std::remove_if(joints.begin(), joints.end(),
        [joint_id](const std::unique_ptr<PhysicsJoint>& joint) {
            return joint->id == joint_id;
        }), joints.end());
}

PhysicsJoint* PhysicsWorld::get_joint(int joint_id) {
    auto it = std::find_if(joints.begin(), joints.end(),
        [joint_id](const std::unique_ptr<PhysicsJoint>& joint) {
            return joint->id == joint_id;
        });
    return (it != joints.end()) ? it->get() : nullptr;
}

void PhysicsWorld::step() {
    // Update positions and velocities
    for (auto& body : bodies) {
        if (body->type == BodyType::DYNAMIC && !body->is_sleeping) {
            // Apply gravity
            body->acceleration = body->acceleration + gravity;
            
            // Update velocity
            body->velocity = body->velocity + body->acceleration * time_step;
            
            // Update position
            body->position = body->position + body->velocity * time_step;
            
            // Update rotation
            body->rotation += body->angular_velocity * time_step;
            
            // Reset acceleration
            body->acceleration = Vector2D(0, 0);
            
            // Check for sleeping
            if (body->velocity.length() < body->sleep_threshold) {
                body->is_sleeping = true;
            }
        }
    }
    
    // Resolve joints
    resolve_joints();
    
    // Collision detection and resolution
    std::vector<CollisionResult> collisions = get_collisions();
    for (const auto& collision : collisions) {
        resolve_collision(collision);
    }
}

void PhysicsWorld::update(float delta_time) {
    // Simple time step management
    float accumulator = 0.0f;
    accumulator += delta_time;
    
    while (accumulator >= time_step) {
        step();
        accumulator -= time_step;
    }
}

bool PhysicsWorld::check_circle_circle(const RigidBody& a, const RigidBody& b, CollisionResult& result) {
    Vector2D distance = b.position - a.position;
    float distance_length = distance.length();
    float min_distance = a.radius + b.radius;
    
    if (distance_length < min_distance) {
        result.collided = true;
        result.body_a_id = a.id;
        result.body_b_id = b.id;
        result.penetration = min_distance - distance_length;
        result.normal = distance.normalized();
        result.contact_point = a.position + result.normal * a.radius;
        result.restitution = std::min(a.restitution, b.restitution);
        result.friction = std::sqrt(a.friction * b.friction);
        return true;
    }
    
    return false;
}

bool PhysicsWorld::check_rectangle_rectangle(const RigidBody& a, const RigidBody& b, CollisionResult& result) {
    // AABB collision detection
    float a_left = a.position.x - a.size.x / 2;
    float a_right = a.position.x + a.size.x / 2;
    float a_top = a.position.y - a.size.y / 2;
    float a_bottom = a.position.y + a.size.y / 2;
    
    float b_left = b.position.x - b.size.x / 2;
    float b_right = b.position.x + b.size.x / 2;
    float b_top = b.position.y - b.size.y / 2;
    float b_bottom = b.position.y + b.size.y / 2;
    
    if (a_left < b_right && a_right > b_left && a_top < b_bottom && a_bottom > b_top) {
        result.collided = true;
        result.body_a_id = a.id;
        result.body_b_id = b.id;
        
        // Calculate penetration and normal
        float overlap_x = std::min(a_right - b_left, b_right - a_left);
        float overlap_y = std::min(a_bottom - b_top, b_bottom - a_top);
        
        if (overlap_x < overlap_y) {
            result.penetration = overlap_x;
            result.normal = Vector2D(a.position.x < b.position.x ? -1 : 1, 0);
        } else {
            result.penetration = overlap_y;
            result.normal = Vector2D(0, a.position.y < b.position.y ? -1 : 1);
        }
        
        result.contact_point = (a.position + b.position) * 0.5f;
        result.restitution = std::min(a.restitution, b.restitution);
        result.friction = std::sqrt(a.friction * b.friction);
        return true;
    }
    
    return false;
}

bool PhysicsWorld::check_circle_rectangle(const RigidBody& circle, const RigidBody& rect, CollisionResult& result) {
    // Find closest point on rectangle to circle center
    float closest_x = std::max(rect.position.x - rect.size.x / 2, 
                              std::min(circle.position.x, rect.position.x + rect.size.x / 2));
    float closest_y = std::max(rect.position.y - rect.size.y / 2, 
                              std::min(circle.position.y, rect.position.y + rect.size.y / 2));
    
    Vector2D closest_point(closest_x, closest_y);
    Vector2D distance = circle.position - closest_point;
    float distance_length = distance.length();
    
    if (distance_length < circle.radius) {
        result.collided = true;
        result.body_a_id = circle.id;
        result.body_b_id = rect.id;
        result.penetration = circle.radius - distance_length;
        result.normal = distance.normalized();
        result.contact_point = closest_point;
        result.restitution = std::min(circle.restitution, rect.restitution);
        result.friction = std::sqrt(circle.friction * rect.friction);
        return true;
    }
    
    return false;
}

// 3D collision detection methods
bool PhysicsWorld::check_sphere_sphere(const RigidBody& a, const RigidBody& b, CollisionResult& result) {
    Vector3D distance = b.position3d - a.position3d;
    float distance_length = distance.length();
    float min_distance = a.radius + b.radius;
    
    if (distance_length < min_distance) {
        result.collided = true;
        result.body_a_id = a.id;
        result.body_b_id = b.id;
        result.penetration = min_distance - distance_length;
        result.normal = Vector2D(distance.x, distance.y).normalized();
        result.contact_point = Vector2D(a.position3d.x, a.position3d.y) + result.normal * a.radius; // Use 2D contact point for now
        result.restitution = std::min(a.restitution, b.restitution);
        result.friction = std::sqrt(a.friction * b.friction);
        return true;
    }
    
    return false;
}

bool PhysicsWorld::check_box_box(const RigidBody& a, const RigidBody& b, CollisionResult& result) {
    // 3D AABB collision detection
    float a_min_x = a.position3d.x - a.size3d.x / 2;
    float a_max_x = a.position3d.x + a.size3d.x / 2;
    float a_min_y = a.position3d.y - a.size3d.y / 2;
    float a_max_y = a.position3d.y + a.size3d.y / 2;
    float a_min_z = a.position3d.z - a.size3d.z / 2;
    float a_max_z = a.position3d.z + a.size3d.z / 2;
    
    float b_min_x = b.position3d.x - b.size3d.x / 2;
    float b_max_x = b.position3d.x + b.size3d.x / 2;
    float b_min_y = b.position3d.y - b.size3d.y / 2;
    float b_max_y = b.position3d.y + b.size3d.y / 2;
    float b_min_z = b.position3d.z - b.size3d.z / 2;
    float b_max_z = b.position3d.z + b.size3d.z / 2;
    
    if (a_max_x >= b_min_x && a_min_x <= b_max_x &&
        a_max_y >= b_min_y && a_min_y <= b_max_y &&
        a_max_z >= b_min_z && a_min_z <= b_max_z) {
        
        result.collided = true;
        result.body_a_id = a.id;
        result.body_b_id = b.id;
        result.penetration = std::min({a_max_x - b_min_x, b_max_x - a_min_x,
                                      a_max_y - b_min_y, b_max_y - a_min_y,
                                      a_max_z - b_min_z, b_max_z - a_min_z});
        result.normal = Vector2D(0, 1); // Default normal for now
        result.contact_point = Vector2D((a.position3d.x + b.position3d.x) / 2, 
                                       (a.position3d.y + b.position3d.y) / 2);
        result.restitution = std::min(a.restitution, b.restitution);
        result.friction = std::sqrt(a.friction * b.friction);
        return true;
    }
    
    return false;
}

bool PhysicsWorld::check_sphere_box(const RigidBody& sphere, const RigidBody& box, CollisionResult& result) {
    // Find closest point on box to sphere center
    float closest_x = std::max(box.position3d.x - box.size3d.x / 2, 
                              std::min(sphere.position3d.x, box.position3d.x + box.size3d.x / 2));
    float closest_y = std::max(box.position3d.y - box.size3d.y / 2, 
                              std::min(sphere.position3d.y, box.position3d.y + box.size3d.y / 2));
    float closest_z = std::max(box.position3d.z - box.size3d.z / 2, 
                              std::min(sphere.position3d.z, box.position3d.z + box.size3d.z / 2));
    
    Vector3D closest_point(closest_x, closest_y, closest_z);
    Vector3D distance = sphere.position3d - closest_point;
    float distance_length = distance.length();
    
    if (distance_length < sphere.radius) {
        result.collided = true;
        result.body_a_id = sphere.id;
        result.body_b_id = box.id;
        result.penetration = sphere.radius - distance_length;
        result.normal = Vector2D(distance.x, distance.y).normalized();
        result.contact_point = Vector2D(closest_x, closest_y); // Use 2D contact point for now
        result.restitution = std::min(sphere.restitution, box.restitution);
        result.friction = std::sqrt(sphere.friction * box.friction);
        return true;
    }
    
    return false;
}

std::vector<CollisionResult> PhysicsWorld::get_collisions() {
    std::vector<CollisionResult> collisions;
    
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = i + 1; j < bodies.size(); ++j) {
            CollisionResult result;
            bool collided = false;
            
            // Determine collision type and check
            if (bodies[i]->shape == ShapeType::CIRCLE && bodies[j]->shape == ShapeType::CIRCLE) {
                collided = check_circle_circle(*bodies[i], *bodies[j], result);
            } else if (bodies[i]->shape == ShapeType::RECTANGLE && bodies[j]->shape == ShapeType::RECTANGLE) {
                collided = check_rectangle_rectangle(*bodies[i], *bodies[j], result);
            } else if (bodies[i]->shape == ShapeType::CIRCLE && bodies[j]->shape == ShapeType::RECTANGLE) {
                collided = check_circle_rectangle(*bodies[i], *bodies[j], result);
            } else if (bodies[i]->shape == ShapeType::RECTANGLE && bodies[j]->shape == ShapeType::CIRCLE) {
                collided = check_circle_rectangle(*bodies[j], *bodies[i], result);
            }
            
            if (collided) {
                collisions.push_back(result);
            }
        }
    }
    
    return collisions;
}

bool PhysicsWorld::check_collision(int body_a_id, int body_b_id) {
    RigidBody* body_a = get_body(body_a_id);
    RigidBody* body_b = get_body(body_b_id);
    
    if (!body_a || !body_b) return false;
    
    CollisionResult result;
    bool collided = false;
    
    if (body_a->shape == ShapeType::CIRCLE && body_b->shape == ShapeType::CIRCLE) {
        collided = check_circle_circle(*body_a, *body_b, result);
    } else if (body_a->shape == ShapeType::RECTANGLE && body_b->shape == ShapeType::RECTANGLE) {
        collided = check_rectangle_rectangle(*body_a, *body_b, result);
    } else if (body_a->shape == ShapeType::CIRCLE && body_b->shape == ShapeType::RECTANGLE) {
        collided = check_circle_rectangle(*body_a, *body_b, result);
    } else if (body_a->shape == ShapeType::RECTANGLE && body_b->shape == ShapeType::CIRCLE) {
        collided = check_circle_rectangle(*body_b, *body_a, result);
    }
    
    return collided;
}

void PhysicsWorld::resolve_collision(const CollisionResult& collision) {
    RigidBody* body_a = get_body(collision.body_a_id);
    RigidBody* body_b = get_body(collision.body_b_id);
    
    if (!body_a || !body_b) return;
    
    // Separate bodies
    float total_mass = body_a->mass + body_b->mass;
    if (total_mass > 0) {
        float separation_a = collision.penetration * (body_b->mass / total_mass);
        float separation_b = collision.penetration * (body_a->mass / total_mass);
        
        if (body_a->type == BodyType::DYNAMIC) {
            body_a->position = body_a->position - collision.normal * separation_a;
        }
        if (body_b->type == BodyType::DYNAMIC) {
            body_b->position = body_b->position + collision.normal * separation_b;
        }
    }
    
    // Calculate relative velocity
    Vector2D relative_velocity = body_b->velocity - body_a->velocity;
    float velocity_along_normal = relative_velocity.x * collision.normal.x + relative_velocity.y * collision.normal.y;
    
    // Don't resolve if velocities are separating
    if (velocity_along_normal > 0) return;
    
    // Calculate restitution
    float restitution = collision.restitution;
    
    // Calculate impulse scalar
    float impulse_scalar = -(1 + restitution) * velocity_along_normal;
    impulse_scalar /= (1.0f / body_a->mass + 1.0f / body_b->mass);
    
    // Apply impulse
    Vector2D impulse = collision.normal * impulse_scalar;
    
    if (body_a->type == BodyType::DYNAMIC) {
        body_a->velocity = body_a->velocity - impulse * (1.0f / body_a->mass);
    }
    if (body_b->type == BodyType::DYNAMIC) {
        body_b->velocity = body_b->velocity + impulse * (1.0f / body_b->mass);
    }
}

void PhysicsWorld::resolve_joints() {
    for (auto& joint : joints) {
        if (!joint->active) continue;
        
        switch (joint->type) {
            case JointType::PIN:
                resolve_pin_joint(*joint);
                break;
            case JointType::SPRING:
                resolve_spring_joint(*joint);
                break;
            case JointType::DISTANCE:
                resolve_distance_joint(*joint);
                break;
            default:
                break;
        }
    }
}

void PhysicsWorld::resolve_pin_joint(PhysicsJoint& joint) {
    RigidBody* body_a = get_body(joint.body_a_id);
    RigidBody* body_b = get_body(joint.body_b_id);
    
    if (!body_a || !body_b) return;
    
    // Pin joint keeps two points together
    Vector2D world_anchor_a = body_a->position + joint.anchor_a;
    Vector2D world_anchor_b = body_b->position + joint.anchor_b;
    Vector2D error = world_anchor_b - world_anchor_a;
    
    // Apply correction
    float correction_factor = 0.5f;
    if (body_a->type == BodyType::DYNAMIC) {
        body_a->position = body_a->position + error * correction_factor;
    }
    if (body_b->type == BodyType::DYNAMIC) {
        body_b->position = body_b->position - error * correction_factor;
    }
}

void PhysicsWorld::resolve_spring_joint(PhysicsJoint& joint) {
    RigidBody* body_a = get_body(joint.body_a_id);
    RigidBody* body_b = get_body(joint.body_b_id);
    
    if (!body_a || !body_b) return;
    
    Vector2D distance = body_b->position - body_a->position;
    float current_length = distance.length();
    float error = current_length - joint.rest_length;
    
    if (current_length > 0) {
        Vector2D direction = distance.normalized();
        Vector2D force = direction * error * joint.stiffness;
        
        // Apply spring force
        if (body_a->type == BodyType::DYNAMIC) {
            body_a->acceleration = body_a->acceleration + force * (1.0f / body_a->mass);
        }
        if (body_b->type == BodyType::DYNAMIC) {
            body_b->acceleration = body_b->acceleration - force * (1.0f / body_b->mass);
        }
        
        // Apply damping
        Vector2D relative_velocity = body_b->velocity - body_a->velocity;
        float damping_force = relative_velocity.x * direction.x + relative_velocity.y * direction.y;
        Vector2D damping = direction * damping_force * joint.damping;
        
        if (body_a->type == BodyType::DYNAMIC) {
            body_a->acceleration = body_a->acceleration + damping * (1.0f / body_a->mass);
        }
        if (body_b->type == BodyType::DYNAMIC) {
            body_b->acceleration = body_b->acceleration - damping * (1.0f / body_b->mass);
        }
    }
}

void PhysicsWorld::resolve_distance_joint(PhysicsJoint& joint) {
    RigidBody* body_a = get_body(joint.body_a_id);
    RigidBody* body_b = get_body(joint.body_b_id);
    
    if (!body_a || !body_b) return;
    
    Vector2D distance = body_b->position - body_a->position;
    float current_length = distance.length();
    float error = current_length - joint.rest_length;
    
    if (current_length > 0) {
        Vector2D direction = distance.normalized();
        Vector2D correction = direction * error * 0.5f;
        
        // Apply distance constraint
        if (body_a->type == BodyType::DYNAMIC) {
            body_a->position = body_a->position + correction;
        }
        if (body_b->type == BodyType::DYNAMIC) {
            body_b->position = body_b->position - correction;
        }
    }
}

Vector2D PhysicsWorld::get_body_position(int body_id) {
    RigidBody* body = get_body(body_id);
    return body ? body->position : Vector2D(0, 0);
}

Vector2D PhysicsWorld::get_body_velocity(int body_id) {
    RigidBody* body = get_body(body_id);
    return body ? body->velocity : Vector2D(0, 0);
}

float PhysicsWorld::get_body_rotation(int body_id) {
    RigidBody* body = get_body(body_id);
    return body ? body->rotation : 0.0f;
}

// 3D getter methods
Vector3D PhysicsWorld::get_body_position_3d(int body_id) {
    RigidBody* body = get_body(body_id);
    return (body && body->is_3d) ? body->position3d : Vector3D(0, 0, 0);
}

Vector3D PhysicsWorld::get_body_velocity_3d(int body_id) {
    RigidBody* body = get_body(body_id);
    return (body && body->is_3d) ? body->velocity3d : Vector3D(0, 0, 0);
}

Vector3D PhysicsWorld::get_body_rotation_3d(int body_id) {
    RigidBody* body = get_body(body_id);
    return (body && body->is_3d) ? body->rotation3d : Vector3D(0, 0, 0);
}

int PhysicsWorld::get_body_count() {
    return static_cast<int>(bodies.size());
}

int PhysicsWorld::get_joint_count() {
    return static_cast<int>(joints.size());
}

// Native function implementations
Value physics_init_world(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_physics_world) {
        g_physics_world = std::make_unique<PhysicsWorld>();
    }
    return Value::nil();
}

Value physics_set_gravity(const std::vector<Value>& args) {
    if (args.size() != 2 || !g_physics_world) return Value::nil();
    
    float x = static_cast<float>(args[0].as_number());
    float y = static_cast<float>(args[1].as_number());
    
    g_physics_world->set_gravity(x, y);
    return Value::nil();
}

Value physics_create_body(const std::vector<Value>& args) {
    if (args.size() != 3 || !g_physics_world) return Value::from_int(-1);
    
    int type = static_cast<int>(args[0].as_int()); // 0=static, 1=dynamic, 2=kinematic
    float x = static_cast<float>(args[1].as_number());
    float y = static_cast<float>(args[2].as_number());
    
    BodyType body_type = static_cast<BodyType>(type);
    int body_id = g_physics_world->create_body(body_type, x, y);
    return Value::from_int(body_id);
}

Value physics_remove_body(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_physics_world) return Value::nil();
    
    int body_id = static_cast<int>(args[0].as_int());
    g_physics_world->remove_body(body_id);
    return Value::nil();
}

Value physics_set_body_position(const std::vector<Value>& args) {
    if (args.size() != 3 || !g_physics_world) return Value::nil();
    
    int body_id = static_cast<int>(args[0].as_int());
    float x = static_cast<float>(args[1].as_number());
    float y = static_cast<float>(args[2].as_number());
    
    g_physics_world->set_body_position(body_id, x, y);
    return Value::nil();
}

Value physics_set_body_velocity(const std::vector<Value>& args) {
    if (args.size() != 3 || !g_physics_world) return Value::nil();
    
    int body_id = static_cast<int>(args[0].as_int());
    float x = static_cast<float>(args[1].as_number());
    float y = static_cast<float>(args[2].as_number());
    
    g_physics_world->set_body_velocity(body_id, x, y);
    return Value::nil();
}

Value physics_set_circle_shape(const std::vector<Value>& args) {
    if (args.size() != 2 || !g_physics_world) return Value::nil();
    
    int body_id = static_cast<int>(args[0].as_int());
    float radius = static_cast<float>(args[1].as_number());
    
    g_physics_world->set_circle_shape(body_id, radius);
    return Value::nil();
}

Value physics_set_rectangle_shape(const std::vector<Value>& args) {
    if (args.size() != 3 || !g_physics_world) return Value::nil();
    
    int body_id = static_cast<int>(args[0].as_int());
    float width = static_cast<float>(args[1].as_number());
    float height = static_cast<float>(args[2].as_number());
    
    g_physics_world->set_rectangle_shape(body_id, width, height);
    return Value::nil();
}

Value physics_apply_force(const std::vector<Value>& args) {
    if (args.size() != 3 || !g_physics_world) return Value::nil();
    
    int body_id = static_cast<int>(args[0].as_int());
    float x = static_cast<float>(args[1].as_number());
    float y = static_cast<float>(args[2].as_number());
    
    g_physics_world->apply_force(body_id, x, y);
    return Value::nil();
}

Value physics_apply_impulse(const std::vector<Value>& args) {
    if (args.size() != 3 || !g_physics_world) return Value::nil();
    
    int body_id = static_cast<int>(args[0].as_int());
    float x = static_cast<float>(args[1].as_number());
    float y = static_cast<float>(args[2].as_number());
    
    g_physics_world->apply_impulse(body_id, x, y);
    return Value::nil();
}

Value physics_create_spring_joint(const std::vector<Value>& args) {
    if (args.size() != 4 || !g_physics_world) return Value::from_int(-1);
    
    int body_a = static_cast<int>(args[0].as_int());
    int body_b = static_cast<int>(args[1].as_int());
    float stiffness = static_cast<float>(args[2].as_number());
    float damping = static_cast<float>(args[3].as_number());
    
    int joint_id = g_physics_world->create_spring_joint(body_a, body_b, stiffness, damping);
    return Value::from_int(joint_id);
}

Value physics_step(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_physics_world) return Value::nil();
    
    g_physics_world->step();
    return Value::nil();
}

Value physics_get_body_position(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_physics_world) return Value::from_number(0);
    
    int body_id = static_cast<int>(args[0].as_int());
    Vector2D pos = g_physics_world->get_body_position(body_id);
    return Value::from_number(pos.x); // Return x coordinate
}

Value physics_check_collision(const std::vector<Value>& args) {
    if (args.size() != 2 || !g_physics_world) return Value::from_bool(false);
    
    int body_a = static_cast<int>(args[0].as_int());
    int body_b = static_cast<int>(args[1].as_int());
    
    bool collided = g_physics_world->check_collision(body_a, body_b);
    return Value::from_bool(collided);
}

void register_physics_functions(FunctionRegistry& registry) {
    registry.add("INITPHYSICS", NativeFn{"INITPHYSICS", 0, physics_init_world});
    registry.add("SETPHYSICSGRAVITY", NativeFn{"SETPHYSICSGRAVITY", 2, physics_set_gravity});
    registry.add("CREATEPHYSICSBODY", NativeFn{"CREATEPHYSICSBODY", 3, physics_create_body});
    registry.add("REMOVEPHYSICSBODY", NativeFn{"REMOVEPHYSICSBODY", 1, physics_remove_body});
    registry.add("SETPHYSICSBODYPOSITION", NativeFn{"SETPHYSICSBODYPOSITION", 3, physics_set_body_position});
    registry.add("SETPHYSICSBODYVELOCITY", NativeFn{"SETPHYSICSBODYVELOCITY", 3, physics_set_body_velocity});
    registry.add("SETPHYSICSCIRCLESHAPE", NativeFn{"SETPHYSICSCIRCLESHAPE", 2, physics_set_circle_shape});
    registry.add("SETPHYSICSRECTANGLESHAPE", NativeFn{"SETPHYSICSRECTANGLESHAPE", 3, physics_set_rectangle_shape});
    registry.add("APPLYPHYSICSFORCE", NativeFn{"APPLYPHYSICSFORCE", 3, physics_apply_force});
    registry.add("APPLYPHYSICSIMPULSE", NativeFn{"APPLYPHYSICSIMPULSE", 3, physics_apply_impulse});
    registry.add("CREATEPHYSICSSPRINGJOINT", NativeFn{"CREATEPHYSICSSPRINGJOINT", 4, physics_create_spring_joint});
    registry.add("PHYSICSSTEP", NativeFn{"PHYSICSSTEP", 0, physics_step});
    registry.add("GETPHYSICSBODYPOSITION", NativeFn{"GETPHYSICSBODYPOSITION", 1, physics_get_body_position});
    registry.add("CHECKPHYSICSCOLLISION", NativeFn{"CHECKPHYSICSCOLLISION", 2, physics_check_collision});
}

} // namespace bas

#pragma once

#include "value.hpp"
#include "runtime.hpp"
#include <vector>
#include <memory>
#include <functional>

namespace bas {

// Forward declarations
struct Point2D;
struct Node;
class Pathfinder;
class NavigationMesh;

// 2D Point structure
struct Point2D {
    float x, y;
    Point2D(float x = 0, float y = 0) : x(x), y(y) {}
    bool operator==(const Point2D& other) const {
        return x == other.x && y == other.y;
    }
};

// 3D Point structure
struct Point3D {
    float x, y, z;
    Point3D(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    bool operator==(const Point3D& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

// A* Pathfinding Node (2D)
struct Node {
    Point2D position;
    float g_cost;  // Distance from start
    float h_cost;  // Distance to end (heuristic)
    float f_cost;  // Total cost (g + h)
    std::shared_ptr<Node> parent;
    bool walkable;
    
    Node(Point2D pos, bool walk = true) 
        : position(pos), g_cost(0), h_cost(0), f_cost(0), 
          parent(nullptr), walkable(walk) {}
};

// A* Pathfinding Node (3D)
struct Node3D {
    Point3D position;
    float g_cost;  // Distance from start
    float h_cost;  // Distance to end (heuristic)
    float f_cost;  // Total cost (g + h)
    std::shared_ptr<Node3D> parent;
    bool walkable;
    
    Node3D(Point3D pos, bool walk = true) 
        : position(pos), g_cost(0), h_cost(0), f_cost(0), 
          parent(nullptr), walkable(walk) {}
};

// Pathfinding result (2D)
struct Path {
    std::vector<Point2D> waypoints;
    bool valid;
    float total_distance;
    
    Path() : valid(false), total_distance(0) {}
    void add_waypoint(const Point2D& point);
    Point2D get_next_waypoint(int current_index) const;
    bool is_complete(int current_index) const;
};

// Pathfinding result (3D)
struct Path3D {
    std::vector<Point3D> waypoints;
    bool valid;
    float total_distance;
    
    Path3D() : valid(false), total_distance(0) {}
    void add_waypoint(const Point3D& point);
    Point3D get_next_waypoint(int current_index) const;
    bool is_complete(int current_index) const;
};

// A* Pathfinder class (2D)
class Pathfinder {
private:
    std::vector<std::vector<std::shared_ptr<Node>>> grid;
    int grid_width, grid_height;
    float cell_size;
    
    float calculate_distance(const Point2D& a, const Point2D& b) const;
    std::vector<std::shared_ptr<Node>> get_neighbors(std::shared_ptr<Node> node) const;
    std::vector<Point2D> reconstruct_path(std::shared_ptr<Node> end_node) const;
    
public:
    Pathfinder(int width, int height, float cell_size = 32.0f);
    void set_obstacle(int x, int y, bool is_obstacle);
    void set_obstacle_rect(float x, float y, float width, float height, bool is_obstacle);
    Path find_path(const Point2D& start, const Point2D& end) const;
    void clear_obstacles();
};

// A* Pathfinder class (3D)
class Pathfinder3D {
private:
    std::vector<std::vector<std::vector<std::shared_ptr<Node3D>>>> grid;
    int grid_width, grid_height, grid_depth;
    float cell_size;
    
    float calculate_distance(const Point3D& a, const Point3D& b) const;
    std::vector<std::shared_ptr<Node3D>> get_neighbors(std::shared_ptr<Node3D> node) const;
    std::vector<Point3D> reconstruct_path(std::shared_ptr<Node3D> end_node) const;
    
public:
    Pathfinder3D(int width, int height, int depth, float cell_size = 32.0f);
    void set_obstacle(int x, int y, int z, bool is_obstacle);
    void set_obstacle_box(float x, float y, float z, float width, float height, float depth, bool is_obstacle);
    Path3D find_path(const Point3D& start, const Point3D& end) const;
    void clear_obstacles();
};

// Navigation Mesh for complex environments (2D)
class NavigationMesh {
private:
    std::vector<Point2D> vertices;
    std::vector<std::vector<int>> triangles;
    std::vector<bool> triangle_walkable;
    
public:
    void add_vertex(const Point2D& vertex);
    void add_triangle(int v1, int v2, int v3, bool walkable = true);
    Path find_path(const Point2D& start, const Point2D& end) const;
    Point2D get_random_point() const;
    bool is_point_walkable(const Point2D& point) const;
};

// Navigation Mesh for complex environments (3D)
class NavigationMesh3D {
private:
    std::vector<Point3D> vertices;
    std::vector<std::vector<int>> triangles;
    std::vector<bool> triangle_walkable;
    
public:
    void add_vertex(const Point3D& vertex);
    void add_triangle(int v1, int v2, int v3, bool walkable = true);
    Path3D find_path(const Point3D& start, const Point3D& end) const;
    Point3D get_random_point() const;
    bool is_point_walkable(const Point3D& point) const;
};

// Navigation system manager
class NavigationSystem {
private:
    std::unique_ptr<Pathfinder> pathfinder;
    std::unique_ptr<Pathfinder3D> pathfinder3d;
    std::unique_ptr<NavigationMesh> navmesh;
    std::unique_ptr<NavigationMesh3D> navmesh3d;
    std::vector<Path> active_paths;
    std::vector<Path3D> active_paths3d;
    
public:
    NavigationSystem();
    ~NavigationSystem();
    
    // Grid-based pathfinding (2D)
    void initialize_grid(int width, int height, float cell_size = 32.0f);
    void add_obstacle(float x, float y, float width, float height);
    void remove_obstacle(float x, float y, float width, float height);
    Path find_path(float start_x, float start_y, float end_x, float end_y);
    
    // Grid-based pathfinding (3D)
    void initialize_grid_3d(int width, int height, int depth, float cell_size = 32.0f);
    void add_obstacle_3d(float x, float y, float z, float width, float height, float depth);
    void remove_obstacle_3d(float x, float y, float z, float width, float height, float depth);
    Path3D find_path_3d(float start_x, float start_y, float start_z, float end_x, float end_y, float end_z);
    
    // Navigation mesh (2D)
    void create_navmesh();
    void add_navmesh_vertex(float x, float y);
    void add_navmesh_triangle(int v1, int v2, int v3, bool walkable = true);
    Path find_navmesh_path(float start_x, float start_y, float end_x, float end_y);
    
    // Navigation mesh (3D)
    void create_navmesh_3d();
    void add_navmesh_vertex_3d(float x, float y, float z);
    void add_navmesh_triangle_3d(int v1, int v2, int v3, bool walkable = true);
    Path3D find_navmesh_path_3d(float start_x, float start_y, float start_z, float end_x, float end_y, float end_z);
    
    // Path management (2D)
    int create_path(float start_x, float start_y, float end_x, float end_y);
    Point2D get_next_waypoint(int path_id, int current_index);
    bool is_path_complete(int path_id, int current_index);
    void remove_path(int path_id);
    
    // Path management (3D)
    int create_path_3d(float start_x, float start_y, float start_z, float end_x, float end_y, float end_z);
    Point3D get_next_waypoint_3d(int path_id, int current_index);
    bool is_path_complete_3d(int path_id, int current_index);
    void remove_path_3d(int path_id);
    
    // Utility functions (2D)
    float calculate_distance(float x1, float y1, float x2, float y2) const;
    Point2D get_random_walkable_point() const;
    bool is_point_walkable(float x, float y) const;
    
    // Utility functions (3D)
    float calculate_distance_3d(float x1, float y1, float z1, float x2, float y2, float z2) const;
    Point3D get_random_walkable_point_3d() const;
    bool is_point_walkable_3d(float x, float y, float z) const;
};

// Native function declarations
void register_navigation_functions(FunctionRegistry& registry);

} // namespace bas

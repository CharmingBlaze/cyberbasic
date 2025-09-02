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

// A* Pathfinding Node
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

// Pathfinding result
struct Path {
    std::vector<Point2D> waypoints;
    bool valid;
    float total_distance;
    
    Path() : valid(false), total_distance(0) {}
    void add_waypoint(const Point2D& point);
    Point2D get_next_waypoint(int current_index) const;
    bool is_complete(int current_index) const;
};

// A* Pathfinder class
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

// Navigation Mesh for complex environments
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

// Navigation system manager
class NavigationSystem {
private:
    std::unique_ptr<Pathfinder> pathfinder;
    std::unique_ptr<NavigationMesh> navmesh;
    std::vector<Path> active_paths;
    
public:
    NavigationSystem();
    ~NavigationSystem();
    
    // Grid-based pathfinding
    void initialize_grid(int width, int height, float cell_size = 32.0f);
    void add_obstacle(float x, float y, float width, float height);
    void remove_obstacle(float x, float y, float width, float height);
    Path find_path(float start_x, float start_y, float end_x, float end_y);
    
    // Navigation mesh
    void create_navmesh();
    void add_navmesh_vertex(float x, float y);
    void add_navmesh_triangle(int v1, int v2, int v3, bool walkable = true);
    Path find_navmesh_path(float start_x, float start_y, float end_x, float end_y);
    
    // Path management
    int create_path(float start_x, float start_y, float end_x, float end_y);
    Point2D get_next_waypoint(int path_id, int current_index);
    bool is_path_complete(int path_id, int current_index);
    void remove_path(int path_id);
    
    // Utility functions
    float calculate_distance(float x1, float y1, float x2, float y2) const;
    Point2D get_random_walkable_point() const;
    bool is_point_walkable(float x, float y) const;
};

// Native function declarations
void register_navigation_functions(FunctionRegistry& registry);

} // namespace bas

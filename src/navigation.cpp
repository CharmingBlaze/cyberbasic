#include "bas/navigation.hpp"
#include "bas/runtime.hpp"
#include <algorithm>
#include <cmath>
#include <queue>
#include <unordered_set>
#include <random>

namespace bas {

// Helper function for distance calculation
static float calculate_distance(const Point2D& a, const Point2D& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Path implementation
void Path::add_waypoint(const Point2D& point) {
    waypoints.push_back(point);
    if (waypoints.size() > 1) {
        total_distance += calculate_distance(waypoints[waypoints.size()-2], waypoints.back());
    }
}

Point2D Path::get_next_waypoint(int current_index) const {
    if (current_index < 0 || current_index >= static_cast<int>(waypoints.size())) {
        return Point2D(0, 0);
    }
    return waypoints[current_index];
}

bool Path::is_complete(int current_index) const {
    return current_index >= static_cast<int>(waypoints.size()) - 1;
}

// Pathfinder implementation
Pathfinder::Pathfinder(int width, int height, float cell_size) 
    : grid_width(width), grid_height(height), cell_size(cell_size) {
    grid.resize(height);
    for (int y = 0; y < height; ++y) {
        grid[y].resize(width);
        for (int x = 0; x < width; ++x) {
            grid[y][x] = std::make_shared<Node>(Point2D(x * cell_size, y * cell_size));
        }
    }
}

void Pathfinder::set_obstacle(int x, int y, bool is_obstacle) {
    if (x >= 0 && x < grid_width && y >= 0 && y < grid_height) {
        grid[y][x]->walkable = !is_obstacle;
    }
}

void Pathfinder::set_obstacle_rect(float x, float y, float width, float height, bool is_obstacle) {
    int start_x = static_cast<int>(x / cell_size);
    int start_y = static_cast<int>(y / cell_size);
    int end_x = static_cast<int>((x + width) / cell_size);
    int end_y = static_cast<int>((y + height) / cell_size);
    
    for (int py = start_y; py <= end_y && py < grid_height; ++py) {
        for (int px = start_x; px <= end_x && px < grid_width; ++px) {
            set_obstacle(px, py, is_obstacle);
        }
    }
}

float Pathfinder::calculate_distance(const Point2D& a, const Point2D& b) const {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

std::vector<std::shared_ptr<Node>> Pathfinder::get_neighbors(std::shared_ptr<Node> node) const {
    std::vector<std::shared_ptr<Node>> neighbors;
    int x = static_cast<int>(node->position.x / cell_size);
    int y = static_cast<int>(node->position.y / cell_size);
    
    // 8-directional movement
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            
            int nx = x + dx;
            int ny = y + dy;
            
            if (nx >= 0 && nx < grid_width && ny >= 0 && ny < grid_height) {
                auto neighbor = grid[ny][nx];
                if (neighbor->walkable) {
                    neighbors.push_back(neighbor);
                }
            }
        }
    }
    
    return neighbors;
}

std::vector<Point2D> Pathfinder::reconstruct_path(std::shared_ptr<Node> end_node) const {
    std::vector<Point2D> path;
    auto current = end_node;
    
    while (current != nullptr) {
        path.push_back(current->position);
        current = current->parent;
    }
    
    std::reverse(path.begin(), path.end());
    return path;
}

Path Pathfinder::find_path(const Point2D& start, const Point2D& end) const {
    Path result;
    
    // Convert world coordinates to grid coordinates
    int start_x = static_cast<int>(start.x / cell_size);
    int start_y = static_cast<int>(start.y / cell_size);
    int end_x = static_cast<int>(end.x / cell_size);
    int end_y = static_cast<int>(end.y / cell_size);
    
    if (start_x < 0 || start_x >= grid_width || start_y < 0 || start_y >= grid_height ||
        end_x < 0 || end_x >= grid_width || end_y < 0 || end_y >= grid_height) {
        return result; // Invalid coordinates
    }
    
    auto start_node = grid[start_y][start_x];
    auto end_node = grid[end_y][end_x];
    
    if (!start_node->walkable || !end_node->walkable) {
        return result; // Start or end is not walkable
    }
    
    // A* algorithm
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, 
                       std::function<bool(const std::shared_ptr<Node>&, const std::shared_ptr<Node>&)>> 
        open_set([](const auto& a, const auto& b) { return a->f_cost > b->f_cost; });
    
    std::unordered_set<std::shared_ptr<Node>> open_set_hash;
    std::unordered_set<std::shared_ptr<Node>> closed_set;
    
    start_node->g_cost = 0;
    start_node->h_cost = calculate_distance(start_node->position, end_node->position);
    start_node->f_cost = start_node->g_cost + start_node->h_cost;
    
    open_set.push(start_node);
    open_set_hash.insert(start_node);
    
    while (!open_set.empty()) {
        auto current = open_set.top();
        open_set.pop();
        open_set_hash.erase(current);
        closed_set.insert(current);
        
        if (current == end_node) {
            result.waypoints = reconstruct_path(current);
            result.valid = true;
            result.total_distance = current->g_cost;
            return result;
        }
        
        auto neighbors = get_neighbors(current);
        for (auto neighbor : neighbors) {
            if (closed_set.find(neighbor) != closed_set.end()) {
                continue;
            }
            
            float tentative_g_cost = current->g_cost + calculate_distance(current->position, neighbor->position);
            
            if (open_set_hash.find(neighbor) == open_set_hash.end()) {
                neighbor->parent = current;
                neighbor->g_cost = tentative_g_cost;
                neighbor->h_cost = calculate_distance(neighbor->position, end_node->position);
                neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;
                
                open_set.push(neighbor);
                open_set_hash.insert(neighbor);
            } else if (tentative_g_cost < neighbor->g_cost) {
                neighbor->parent = current;
                neighbor->g_cost = tentative_g_cost;
                neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;
            }
        }
    }
    
    return result; // No path found
}

void Pathfinder::clear_obstacles() {
    for (int y = 0; y < grid_height; ++y) {
        for (int x = 0; x < grid_width; ++x) {
            grid[y][x]->walkable = true;
        }
    }
}

// NavigationMesh implementation
void NavigationMesh::add_vertex(const Point2D& vertex) {
    vertices.push_back(vertex);
}

void NavigationMesh::add_triangle(int v1, int v2, int v3, bool walkable) {
    if (v1 >= 0 && v1 < static_cast<int>(vertices.size()) &&
        v2 >= 0 && v2 < static_cast<int>(vertices.size()) &&
        v3 >= 0 && v3 < static_cast<int>(vertices.size())) {
        triangles.push_back({v1, v2, v3});
        triangle_walkable.push_back(walkable);
    }
}

Path NavigationMesh::find_path(const Point2D& start, const Point2D& end) const {
    Path result;
    // Simplified implementation - in a real system, this would use
    // more sophisticated mesh-based pathfinding algorithms
    result.waypoints = {start, end};
    result.valid = true;
    result.total_distance = std::sqrt((end.x - start.x) * (end.x - start.x) + 
                                     (end.y - start.y) * (end.y - start.y));
    return result;
}

Point2D NavigationMesh::get_random_point() const {
    if (vertices.empty()) return Point2D(0, 0);
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, vertices.size() - 1);
    
    return vertices[dis(gen)];
}

bool NavigationMesh::is_point_walkable(const Point2D& point) const {
    (void)point; // Suppress unused parameter warning
    // Simplified implementation - would need proper point-in-triangle testing
    return true;
}

// NavigationSystem implementation
NavigationSystem::NavigationSystem() = default;
NavigationSystem::~NavigationSystem() = default;

void NavigationSystem::initialize_grid(int width, int height, float cell_size) {
    pathfinder = std::make_unique<Pathfinder>(width, height, cell_size);
}

void NavigationSystem::add_obstacle(float x, float y, float width, float height) {
    if (pathfinder) {
        pathfinder->set_obstacle_rect(x, y, width, height, true);
    }
}

void NavigationSystem::remove_obstacle(float x, float y, float width, float height) {
    if (pathfinder) {
        pathfinder->set_obstacle_rect(x, y, width, height, false);
    }
}

Path NavigationSystem::find_path(float start_x, float start_y, float end_x, float end_y) {
    if (pathfinder) {
        return pathfinder->find_path(Point2D(start_x, start_y), Point2D(end_x, end_y));
    }
    return Path();
}

void NavigationSystem::create_navmesh() {
    navmesh = std::make_unique<NavigationMesh>();
}

void NavigationSystem::add_navmesh_vertex(float x, float y) {
    if (navmesh) {
        navmesh->add_vertex(Point2D(x, y));
    }
}

void NavigationSystem::add_navmesh_triangle(int v1, int v2, int v3, bool walkable) {
    if (navmesh) {
        navmesh->add_triangle(v1, v2, v3, walkable);
    }
}

Path NavigationSystem::find_navmesh_path(float start_x, float start_y, float end_x, float end_y) {
    if (navmesh) {
        return navmesh->find_path(Point2D(start_x, start_y), Point2D(end_x, end_y));
    }
    return Path();
}

int NavigationSystem::create_path(float start_x, float start_y, float end_x, float end_y) {
    Path path = find_path(start_x, start_y, end_x, end_y);
    if (path.valid) {
        active_paths.push_back(path);
        return static_cast<int>(active_paths.size() - 1);
    }
    return -1;
}

Point2D NavigationSystem::get_next_waypoint(int path_id, int current_index) {
    if (path_id >= 0 && path_id < static_cast<int>(active_paths.size())) {
        return active_paths[path_id].get_next_waypoint(current_index);
    }
    return Point2D(0, 0);
}

bool NavigationSystem::is_path_complete(int path_id, int current_index) {
    if (path_id >= 0 && path_id < static_cast<int>(active_paths.size())) {
        return active_paths[path_id].is_complete(current_index);
    }
    return true;
}

void NavigationSystem::remove_path(int path_id) {
    if (path_id >= 0 && path_id < static_cast<int>(active_paths.size())) {
        active_paths.erase(active_paths.begin() + path_id);
    }
}

float NavigationSystem::calculate_distance(float x1, float y1, float x2, float y2) const {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

Point2D NavigationSystem::get_random_walkable_point() const {
    if (navmesh) {
        return navmesh->get_random_point();
    }
    return Point2D(0, 0);
}

bool NavigationSystem::is_point_walkable(float x, float y) const {
    if (navmesh) {
        return navmesh->is_point_walkable(Point2D(x, y));
    }
    return true;
}

// Global navigation system instance
static std::unique_ptr<NavigationSystem> g_nav_system;

// Native function implementations
Value nav_init_grid(const std::vector<Value>& args) {
    if (args.size() != 3) return Value::nil();
    
    if (!g_nav_system) {
        g_nav_system = std::make_unique<NavigationSystem>();
    }
    
    int width = args[0].as_int();
    int height = args[1].as_int();
    float cell_size = static_cast<float>(args[2].as_number());
    
    g_nav_system->initialize_grid(width, height, cell_size);
    return Value::nil();
}

Value nav_add_obstacle(const std::vector<Value>& args) {
    if (args.size() != 4 || !g_nav_system) return Value::nil();
    
    float x = static_cast<float>(args[0].as_number());
    float y = static_cast<float>(args[1].as_number());
    float width = static_cast<float>(args[2].as_number());
    float height = static_cast<float>(args[3].as_number());
    
    g_nav_system->add_obstacle(x, y, width, height);
    return Value::nil();
}

Value nav_remove_obstacle(const std::vector<Value>& args) {
    if (args.size() != 4 || !g_nav_system) return Value::nil();
    
    float x = static_cast<float>(args[0].as_number());
    float y = static_cast<float>(args[1].as_number());
    float width = static_cast<float>(args[2].as_number());
    float height = static_cast<float>(args[3].as_number());
    
    g_nav_system->remove_obstacle(x, y, width, height);
    return Value::nil();
}

Value nav_find_path(const std::vector<Value>& args) {
    if (args.size() != 4 || !g_nav_system) return Value::nil();
    
    float start_x = static_cast<float>(args[0].as_number());
    float start_y = static_cast<float>(args[1].as_number());
    float end_x = static_cast<float>(args[2].as_number());
    float end_y = static_cast<float>(args[3].as_number());
    
    Path path = g_nav_system->find_path(start_x, start_y, end_x, end_y);
    return Value::from_bool(path.valid);
}

Value nav_create_path(const std::vector<Value>& args) {
    if (args.size() != 4 || !g_nav_system) return Value::from_int(-1);
    
    float start_x = static_cast<float>(args[0].as_number());
    float start_y = static_cast<float>(args[1].as_number());
    float end_x = static_cast<float>(args[2].as_number());
    float end_y = static_cast<float>(args[3].as_number());
    
    int path_id = g_nav_system->create_path(start_x, start_y, end_x, end_y);
    return Value::from_int(path_id);
}

Value nav_get_next_waypoint(const std::vector<Value>& args) {
    if (args.size() != 2 || !g_nav_system) return Value::from_number(0);
    
    int path_id = args[0].as_int();
    int current_index = args[1].as_int();
    
    Point2D waypoint = g_nav_system->get_next_waypoint(path_id, current_index);
    return Value::from_number(waypoint.x); // Return x coordinate (could be enhanced to return both)
}

Value nav_is_path_complete(const std::vector<Value>& args) {
    if (args.size() != 2 || !g_nav_system) return Value::from_bool(true);
    
    int path_id = args[0].as_int();
    int current_index = args[1].as_int();
    
    return Value::from_bool(g_nav_system->is_path_complete(path_id, current_index));
}

Value nav_remove_path(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_nav_system) return Value::nil();
    
    int path_id = args[0].as_int();
    g_nav_system->remove_path(path_id);
    return Value::nil();
}

Value nav_calculate_distance(const std::vector<Value>& args) {
    if (args.size() != 4 || !g_nav_system) return Value::from_number(0);
    
    float x1 = static_cast<float>(args[0].as_number());
    float y1 = static_cast<float>(args[1].as_number());
    float x2 = static_cast<float>(args[2].as_number());
    float y2 = static_cast<float>(args[3].as_number());
    
    float distance = g_nav_system->calculate_distance(x1, y1, x2, y2);
    return Value::from_number(distance);
}

void register_navigation_functions(FunctionRegistry& registry) {
    registry.add("INITNAVGRID", NativeFn{"INITNAVGRID", 3, nav_init_grid});
    registry.add("ADDNAVOBSTACLE", NativeFn{"ADDNAVOBSTACLE", 4, nav_add_obstacle});
    registry.add("REMOVENAVOBSTACLE", NativeFn{"REMOVENAVOBSTACLE", 4, nav_remove_obstacle});
    registry.add("FINDNAVPATH", NativeFn{"FINDNAVPATH", 4, nav_find_path});
    registry.add("CREATENAVPATH", NativeFn{"CREATENAVPATH", 4, nav_create_path});
    registry.add("GETNEXTWAYPOINT", NativeFn{"GETNEXTWAYPOINT", 2, nav_get_next_waypoint});
    registry.add("ISNAVPATHCOMPLETE", NativeFn{"ISNAVPATHCOMPLETE", 2, nav_is_path_complete});
    registry.add("REMOVENAVPATH", NativeFn{"REMOVENAVPATH", 1, nav_remove_path});
    registry.add("NAVDISTANCE", NativeFn{"NAVDISTANCE", 4, nav_calculate_distance});
}

} // namespace bas

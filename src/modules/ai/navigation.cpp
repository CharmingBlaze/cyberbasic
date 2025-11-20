#include "bas/navigation.hpp"
#include "bas/runtime.hpp"
#include <algorithm>
#include <cmath>
#include <queue>
#include <unordered_set>
#include <random>

namespace bas {

// Helper function for distance calculation (2D)
static float calculate_distance(const Point2D& a, const Point2D& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Helper function for distance calculation (3D)
static float calculate_distance_3d(const Point3D& a, const Point3D& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
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

// Path3D implementation
void Path3D::add_waypoint(const Point3D& point) {
    waypoints.push_back(point);
    if (waypoints.size() > 1) {
        total_distance += calculate_distance_3d(waypoints[waypoints.size()-2], waypoints.back());
    }
}

Point3D Path3D::get_next_waypoint(int current_index) const {
    if (current_index < 0 || current_index >= static_cast<int>(waypoints.size())) {
        return Point3D(0, 0, 0);
    }
    return waypoints[current_index];
}

bool Path3D::is_complete(int current_index) const {
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

// Pathfinder3D implementation
Pathfinder3D::Pathfinder3D(int width, int height, int depth, float cell_size) 
    : grid_width(width), grid_height(height), grid_depth(depth), cell_size(cell_size) {
    grid.resize(depth);
    for (int z = 0; z < depth; ++z) {
        grid[z].resize(height);
        for (int y = 0; y < height; ++y) {
            grid[z][y].resize(width);
            for (int x = 0; x < width; ++x) {
                Point3D pos(x * cell_size, y * cell_size, z * cell_size);
                grid[z][y][x] = std::make_shared<Node3D>(pos);
            }
        }
    }
}

void Pathfinder3D::set_obstacle(int x, int y, int z, bool is_obstacle) {
    if (x >= 0 && x < grid_width && y >= 0 && y < grid_height && z >= 0 && z < grid_depth) {
        grid[z][y][x]->walkable = !is_obstacle;
    }
}

void Pathfinder3D::set_obstacle_box(float x, float y, float z, float width, float height, float depth, bool is_obstacle) {
    int start_x = static_cast<int>(x / cell_size);
    int start_y = static_cast<int>(y / cell_size);
    int start_z = static_cast<int>(z / cell_size);
    int end_x = static_cast<int>((x + width) / cell_size);
    int end_y = static_cast<int>((y + height) / cell_size);
    int end_z = static_cast<int>((z + depth) / cell_size);
    
    for (int gz = start_z; gz <= end_z && gz < grid_depth; ++gz) {
        for (int gy = start_y; gy <= end_y && gy < grid_height; ++gy) {
            for (int gx = start_x; gx <= end_x && gx < grid_width; ++gx) {
                set_obstacle(gx, gy, gz, is_obstacle);
            }
        }
    }
}

Path3D Pathfinder3D::find_path(const Point3D& start, const Point3D& end) const {
    Path3D path;
    
    // Convert world coordinates to grid coordinates
    int start_x = static_cast<int>(start.x / cell_size);
    int start_y = static_cast<int>(start.y / cell_size);
    int start_z = static_cast<int>(start.z / cell_size);
    int end_x = static_cast<int>(end.x / cell_size);
    int end_y = static_cast<int>(end.y / cell_size);
    int end_z = static_cast<int>(end.z / cell_size);
    
    if (start_x < 0 || start_x >= grid_width || start_y < 0 || start_y >= grid_height || 
        start_z < 0 || start_z >= grid_depth || end_x < 0 || end_x >= grid_width || 
        end_y < 0 || end_y >= grid_height || end_z < 0 || end_z >= grid_depth) {
        return path;
    }
    
    auto start_node = grid[start_z][start_y][start_x];
    auto end_node = grid[end_z][end_y][end_x];
    
    if (!start_node->walkable || !end_node->walkable) {
        return path;
    }
    
    // A* algorithm implementation (simplified for 3D)
    std::priority_queue<std::shared_ptr<Node3D>, std::vector<std::shared_ptr<Node3D>>, 
                       std::function<bool(const std::shared_ptr<Node3D>&, const std::shared_ptr<Node3D>&)>> 
        open_set([](const std::shared_ptr<Node3D>& a, const std::shared_ptr<Node3D>& b) {
            return a->f_cost > b->f_cost;
        });
    
    std::unordered_set<std::shared_ptr<Node3D>> closed_set;
    
    start_node->g_cost = 0;
    start_node->h_cost = calculate_distance_3d(start_node->position, end_node->position);
    start_node->f_cost = start_node->g_cost + start_node->h_cost;
    
    open_set.push(start_node);
    
    while (!open_set.empty()) {
        auto current = open_set.top();
        open_set.pop();
        
        if (current == end_node) {
            // Reconstruct path
            std::vector<Point3D> waypoints;
            auto node = current;
            while (node) {
                waypoints.push_back(node->position);
                node = node->parent;
            }
            std::reverse(waypoints.begin(), waypoints.end());
            
            for (const auto& waypoint : waypoints) {
                path.add_waypoint(waypoint);
            }
            path.valid = true;
            break;
        }
        
        closed_set.insert(current);
        
        auto neighbors = get_neighbors(current);
        for (auto neighbor : neighbors) {
            if (closed_set.find(neighbor) != closed_set.end() || !neighbor->walkable) {
                continue;
            }
            
            float tentative_g_cost = current->g_cost + calculate_distance_3d(current->position, neighbor->position);
            
            bool in_open_set = false;
            std::vector<std::shared_ptr<Node3D>> temp_open_set;
            while (!open_set.empty()) {
                auto node = open_set.top();
                open_set.pop();
                if (node == neighbor) {
                    in_open_set = true;
                    if (tentative_g_cost < neighbor->g_cost) {
                        neighbor->parent = current;
                        neighbor->g_cost = tentative_g_cost;
                        neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;
                    }
                }
                temp_open_set.push_back(node);
            }
            
            for (auto node : temp_open_set) {
                open_set.push(node);
            }
            
            if (!in_open_set) {
                neighbor->parent = current;
                neighbor->g_cost = tentative_g_cost;
                neighbor->h_cost = calculate_distance_3d(neighbor->position, end_node->position);
                neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;
                open_set.push(neighbor);
            }
        }
    }
    
    return path;
}

void Pathfinder3D::clear_obstacles() {
    for (int z = 0; z < grid_depth; ++z) {
        for (int y = 0; y < grid_height; ++y) {
            for (int x = 0; x < grid_width; ++x) {
                grid[z][y][x]->walkable = true;
            }
        }
    }
}

float Pathfinder3D::calculate_distance(const Point3D& a, const Point3D& b) const {
    return calculate_distance_3d(a, b);
}

std::vector<std::shared_ptr<Node3D>> Pathfinder3D::get_neighbors(std::shared_ptr<Node3D> node) const {
    std::vector<std::shared_ptr<Node3D>> neighbors;
    
    int x = static_cast<int>(node->position.x / cell_size);
    int y = static_cast<int>(node->position.y / cell_size);
    int z = static_cast<int>(node->position.z / cell_size);
    
    // Check 26 neighbors (3D grid)
    for (int dz = -1; dz <= 1; ++dz) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == 0 && dy == 0 && dz == 0) continue;
                
                int nx = x + dx;
                int ny = y + dy;
                int nz = z + dz;
                
                if (nx >= 0 && nx < grid_width && ny >= 0 && ny < grid_height && nz >= 0 && nz < grid_depth) {
                    neighbors.push_back(grid[nz][ny][nx]);
                }
            }
        }
    }
    
    return neighbors;
}

std::vector<Point3D> Pathfinder3D::reconstruct_path(std::shared_ptr<Node3D> end_node) const {
    std::vector<Point3D> path;
    auto node = end_node;
    while (node) {
        path.push_back(node->position);
        node = node->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
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

// NavigationMesh3D implementation
void NavigationMesh3D::add_vertex(const Point3D& vertex) {
    vertices.push_back(vertex);
}

void NavigationMesh3D::add_triangle(int v1, int v2, int v3, bool walkable) {
    if (v1 >= 0 && v1 < static_cast<int>(vertices.size()) &&
        v2 >= 0 && v2 < static_cast<int>(vertices.size()) &&
        v3 >= 0 && v3 < static_cast<int>(vertices.size())) {
        triangles.push_back({v1, v2, v3});
        triangle_walkable.push_back(walkable);
    }
}

Path3D NavigationMesh3D::find_path(const Point3D& start, const Point3D& end) const {
    Path3D path;
    // Simplified implementation - would need proper 3D navigation mesh pathfinding
    path.add_waypoint(start);
    path.add_waypoint(end);
    path.valid = true;
    return path;
}

Point3D NavigationMesh3D::get_random_point() const {
    if (vertices.empty()) {
        return Point3D(0, 0, 0);
    }
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(vertices.size()) - 1);
    
    return vertices[dis(gen)];
}

bool NavigationMesh3D::is_point_walkable(const Point3D& point) const {
    (void)point; // Suppress unused parameter warning
    // Simplified implementation - would need proper point-in-triangle testing for 3D
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

// 3D NavigationSystem methods
void NavigationSystem::initialize_grid_3d(int width, int height, int depth, float cell_size) {
    pathfinder3d = std::make_unique<Pathfinder3D>(width, height, depth, cell_size);
}

void NavigationSystem::add_obstacle_3d(float x, float y, float z, float width, float height, float depth) {
    if (pathfinder3d) {
        pathfinder3d->set_obstacle_box(x, y, z, width, height, depth, true);
    }
}

void NavigationSystem::remove_obstacle_3d(float x, float y, float z, float width, float height, float depth) {
    if (pathfinder3d) {
        pathfinder3d->set_obstacle_box(x, y, z, width, height, depth, false);
    }
}

Path3D NavigationSystem::find_path_3d(float start_x, float start_y, float start_z, float end_x, float end_y, float end_z) {
    if (pathfinder3d) {
        return pathfinder3d->find_path(Point3D(start_x, start_y, start_z), Point3D(end_x, end_y, end_z));
    }
    return Path3D();
}

void NavigationSystem::create_navmesh_3d() {
    navmesh3d = std::make_unique<NavigationMesh3D>();
}

void NavigationSystem::add_navmesh_vertex_3d(float x, float y, float z) {
    if (navmesh3d) {
        navmesh3d->add_vertex(Point3D(x, y, z));
    }
}

void NavigationSystem::add_navmesh_triangle_3d(int v1, int v2, int v3, bool walkable) {
    if (navmesh3d) {
        navmesh3d->add_triangle(v1, v2, v3, walkable);
    }
}

Path3D NavigationSystem::find_navmesh_path_3d(float start_x, float start_y, float start_z, float end_x, float end_y, float end_z) {
    if (navmesh3d) {
        return navmesh3d->find_path(Point3D(start_x, start_y, start_z), Point3D(end_x, end_y, end_z));
    }
    return Path3D();
}

int NavigationSystem::create_path_3d(float start_x, float start_y, float start_z, float end_x, float end_y, float end_z) {
    Path3D path = find_path_3d(start_x, start_y, start_z, end_x, end_y, end_z);
    if (path.valid) {
        active_paths3d.push_back(path);
        return static_cast<int>(active_paths3d.size()) - 1;
    }
    return -1;
}

Point3D NavigationSystem::get_next_waypoint_3d(int path_id, int current_index) {
    if (path_id >= 0 && path_id < static_cast<int>(active_paths3d.size())) {
        return active_paths3d[path_id].get_next_waypoint(current_index);
    }
    return Point3D(0, 0, 0);
}

bool NavigationSystem::is_path_complete_3d(int path_id, int current_index) {
    if (path_id >= 0 && path_id < static_cast<int>(active_paths3d.size())) {
        return active_paths3d[path_id].is_complete(current_index);
    }
    return true;
}

void NavigationSystem::remove_path_3d(int path_id) {
    if (path_id >= 0 && path_id < static_cast<int>(active_paths3d.size())) {
        active_paths3d.erase(active_paths3d.begin() + path_id);
    }
}

float NavigationSystem::calculate_distance_3d(float x1, float y1, float z1, float x2, float y2, float z2) const {
    return std::sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));
}

Point3D NavigationSystem::get_random_walkable_point_3d() const {
    if (navmesh3d) {
        return navmesh3d->get_random_point();
    }
    return Point3D(0, 0, 0);
}

bool NavigationSystem::is_point_walkable_3d(float x, float y, float z) const {
    if (navmesh3d) {
        return navmesh3d->is_point_walkable(Point3D(x, y, z));
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

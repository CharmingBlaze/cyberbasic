#pragma once

#include "value.hpp"
#include "runtime.hpp"
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>

namespace bas {

// Forward declarations
class AIState;
class BehaviorTree;
class AISystem;

// AI State types
enum class StateType {
    IDLE,
    PATROL,
    CHASE,
    ATTACK,
    FLEE,
    SEARCH,
    CUSTOM
};

// Behavior Tree node types
enum class NodeType {
    SEQUENCE,
    SELECTOR,
    PARALLEL,
    CONDITION,
    ACTION,
    DECORATOR
};

// Behavior Tree node result
enum class NodeResult {
    SUCCESS,
    FAILURE,
    RUNNING
};

// AI State class
class AIState {
private:
    std::string name;
    StateType type;
    std::function<NodeResult()> enter_function;
    std::function<NodeResult()> update_function;
    std::function<void()> exit_function;
    bool is_active;
    
public:
    AIState(const std::string& name, StateType type);
    ~AIState() = default;
    
    // State management
    void set_enter_function(std::function<NodeResult()> func);
    void set_update_function(std::function<NodeResult()> func);
    void set_exit_function(std::function<void()> func);
    
    // State execution
    NodeResult enter();
    NodeResult update();
    void exit();
    
    // Getters
    const std::string& get_name() const { return name; }
    StateType get_type() const { return type; }
    bool get_is_active() const { return is_active; }
    void set_is_active(bool active) { is_active = active; }
};

// Behavior Tree Node
class BehaviorNode {
private:
    std::string name;
    NodeType type;
    std::vector<std::shared_ptr<BehaviorNode>> children;
    std::function<NodeResult()> action_function;
    std::function<bool()> condition_function;
    NodeResult last_result;
    
public:
    BehaviorNode(const std::string& name, NodeType type);
    ~BehaviorNode() = default;
    
    // Node management
    void add_child(std::shared_ptr<BehaviorNode> child);
    void remove_child(std::shared_ptr<BehaviorNode> child);
    void set_action_function(std::function<NodeResult()> func);
    void set_condition_function(std::function<bool()> func);
    
    // Node execution
    NodeResult execute();
    NodeResult get_last_result() const { return last_result; }
    
    // Node type specific execution methods
    NodeResult execute_sequence();
    NodeResult execute_selector();
    NodeResult execute_parallel();
    NodeResult execute_condition();
    NodeResult execute_action();
    NodeResult execute_decorator();
    
    // Getters
    const std::string& get_name() const { return name; }
    NodeType get_type() const { return type; }
    const std::vector<std::shared_ptr<BehaviorNode>>& get_children() const { return children; }
};

// Behavior Tree
class BehaviorTree {
private:
    std::shared_ptr<BehaviorNode> root;
    std::string name;
    bool is_running;
    
public:
    BehaviorTree(const std::string& name);
    ~BehaviorTree() = default;
    
    // Tree management
    void set_root(std::shared_ptr<BehaviorNode> root_node);
    std::shared_ptr<BehaviorNode> create_node(const std::string& name, NodeType type);
    
    // Tree execution
    NodeResult execute();
    void start();
    void stop();
    
    // Getters
    const std::string& get_name() const { return name; }
    bool get_is_running() const { return is_running; }
};

// AI Agent
class AIAgent {
private:
    int id;
    std::string name;
    std::vector<std::shared_ptr<AIState>> states;
    std::shared_ptr<AIState> current_state;
    std::shared_ptr<BehaviorTree> behavior_tree;
    std::unordered_map<std::string, Value> blackboard;
    float position_x, position_y;
    float target_x, target_y;
    float speed;
    float health;
    float max_health;
    bool is_alive;
    
public:
    AIAgent(int id, const std::string& name);
    ~AIAgent() = default;
    
    // State management
    void add_state(std::shared_ptr<AIState> state);
    void remove_state(const std::string& state_name);
    void set_current_state(const std::string& state_name);
    std::shared_ptr<AIState> get_current_state() const { return current_state; }
    
    // Behavior tree management
    void set_behavior_tree(std::shared_ptr<BehaviorTree> tree);
    std::shared_ptr<BehaviorTree> get_behavior_tree() const { return behavior_tree; }
    
    // Blackboard (shared memory)
    void set_blackboard_value(const std::string& key, const Value& value);
    Value get_blackboard_value(const std::string& key) const;
    bool has_blackboard_value(const std::string& key) const;
    void remove_blackboard_value(const std::string& key);
    
    // Agent properties
    void set_position(float x, float y);
    void get_position(float& x, float& y) const;
    void set_target(float x, float y);
    void get_target(float& x, float& y) const;
    void set_speed(float speed);
    float get_speed() const { return speed; }
    void set_health(float health);
    float get_health() const { return health; }
    void set_max_health(float max_health);
    float get_max_health() const { return max_health; }
    void set_alive(bool alive);
    bool get_alive() const { return is_alive; }
    
    // Agent execution
    void update(float delta_time);
    
    // Getters
    int get_id() const { return id; }
    const std::string& get_name() const { return name; }
};

// AI System
class AISystem {
private:
    std::vector<std::unique_ptr<AIAgent>> agents;
    std::vector<std::shared_ptr<BehaviorTree>> behavior_trees;
    int next_agent_id;
    float update_interval;
    float last_update_time;
    
public:
    AISystem();
    ~AISystem() = default;
    
    // Agent management
    int create_agent(const std::string& name);
    void remove_agent(int agent_id);
    AIAgent* get_agent(int agent_id);
    
    // Behavior tree management
    std::shared_ptr<BehaviorTree> create_behavior_tree(const std::string& name);
    void remove_behavior_tree(const std::string& name);
    std::shared_ptr<BehaviorTree> get_behavior_tree(const std::string& name);
    
    // System update
    void update(float delta_time);
    void set_update_interval(float interval);
    
    // Utility functions
    int get_agent_count() const;
    std::vector<AIAgent*> get_agents_in_radius(float x, float y, float radius);
    AIAgent* get_nearest_agent(float x, float y);
    
    // Pre-built behaviors
    std::shared_ptr<BehaviorTree> create_patrol_behavior(float start_x, float start_y, float end_x, float end_y);
    std::shared_ptr<BehaviorTree> create_chase_behavior(int target_agent_id);
    std::shared_ptr<BehaviorTree> create_attack_behavior(int target_agent_id, float attack_range);
    std::shared_ptr<BehaviorTree> create_flee_behavior(int threat_agent_id, float flee_distance);
};

// Global AI system instance
extern std::unique_ptr<AISystem> g_ai_system;

// Native function declarations
void register_ai_functions(FunctionRegistry& registry);

} // namespace bas

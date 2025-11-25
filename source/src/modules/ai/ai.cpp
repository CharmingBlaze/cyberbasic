#include "bas/ai.hpp"
#include "bas/runtime.hpp"
#include <algorithm>
#include <cmath>

namespace bas {

// Global AI system instance
std::unique_ptr<AISystem> g_ai_system;

// AIState implementation
AIState::AIState(const std::string& name, StateType type) 
    : name(name), type(type), is_active(false) {
}

void AIState::set_enter_function(std::function<NodeResult()> func) {
    enter_function = func;
}

void AIState::set_update_function(std::function<NodeResult()> func) {
    update_function = func;
}

void AIState::set_exit_function(std::function<void()> func) {
    exit_function = func;
}

NodeResult AIState::enter() {
    is_active = true;
    if (enter_function) {
        return enter_function();
    }
    return NodeResult::SUCCESS;
}

NodeResult AIState::update() {
    if (update_function) {
        return update_function();
    }
    return NodeResult::SUCCESS;
}

void AIState::exit() {
    is_active = false;
    if (exit_function) {
        exit_function();
    }
}

// BehaviorNode implementation
BehaviorNode::BehaviorNode(const std::string& name, NodeType type) 
    : name(name), type(type), last_result(NodeResult::FAILURE) {
}

void BehaviorNode::add_child(std::shared_ptr<BehaviorNode> child) {
    children.push_back(child);
}

void BehaviorNode::remove_child(std::shared_ptr<BehaviorNode> child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

void BehaviorNode::set_action_function(std::function<NodeResult()> func) {
    action_function = func;
}

void BehaviorNode::set_condition_function(std::function<bool()> func) {
    condition_function = func;
}

NodeResult BehaviorNode::execute() {
    switch (type) {
        case NodeType::SEQUENCE:
            return execute_sequence();
        case NodeType::SELECTOR:
            return execute_selector();
        case NodeType::PARALLEL:
            return execute_parallel();
        case NodeType::CONDITION:
            return execute_condition();
        case NodeType::ACTION:
            return execute_action();
        case NodeType::DECORATOR:
            return execute_decorator();
        default:
            return NodeResult::FAILURE;
    }
}

NodeResult BehaviorNode::execute_sequence() {
    for (auto& child : children) {
        NodeResult result = child->execute();
        if (result != NodeResult::SUCCESS) {
            last_result = result;
            return result;
        }
    }
    last_result = NodeResult::SUCCESS;
    return NodeResult::SUCCESS;
}

NodeResult BehaviorNode::execute_selector() {
    for (auto& child : children) {
        NodeResult result = child->execute();
        if (result != NodeResult::FAILURE) {
            last_result = result;
            return result;
        }
    }
    last_result = NodeResult::FAILURE;
    return NodeResult::FAILURE;
}

NodeResult BehaviorNode::execute_parallel() {
    int success_count = 0;
    int failure_count = 0;
    
    for (auto& child : children) {
        NodeResult result = child->execute();
        if (result == NodeResult::SUCCESS) {
            success_count++;
        } else if (result == NodeResult::FAILURE) {
            failure_count++;
        }
    }
    
    if (success_count == static_cast<int>(children.size())) {
        last_result = NodeResult::SUCCESS;
        return NodeResult::SUCCESS;
    } else if (failure_count == static_cast<int>(children.size())) {
        last_result = NodeResult::FAILURE;
        return NodeResult::FAILURE;
    } else {
        last_result = NodeResult::RUNNING;
        return NodeResult::RUNNING;
    }
}

NodeResult BehaviorNode::execute_condition() {
    if (condition_function) {
        bool result = condition_function();
        last_result = result ? NodeResult::SUCCESS : NodeResult::FAILURE;
        return last_result;
    }
    last_result = NodeResult::FAILURE;
    return NodeResult::FAILURE;
}

NodeResult BehaviorNode::execute_action() {
    if (action_function) {
        last_result = action_function();
        return last_result;
    }
    last_result = NodeResult::FAILURE;
    return NodeResult::FAILURE;
}

NodeResult BehaviorNode::execute_decorator() {
    if (!children.empty()) {
        NodeResult result = children[0]->execute();
        // Decorator can modify the result (e.g., invert, repeat, etc.)
        last_result = result;
        return result;
    }
    last_result = NodeResult::FAILURE;
    return NodeResult::FAILURE;
}

// BehaviorTree implementation
BehaviorTree::BehaviorTree(const std::string& name) 
    : name(name), is_running(false) {
}

void BehaviorTree::set_root(std::shared_ptr<BehaviorNode> root_node) {
    root = root_node;
}

std::shared_ptr<BehaviorNode> BehaviorTree::create_node(const std::string& name, NodeType type) {
    return std::make_shared<BehaviorNode>(name, type);
}

NodeResult BehaviorTree::execute() {
    if (root && is_running) {
        return root->execute();
    }
    return NodeResult::FAILURE;
}

void BehaviorTree::start() {
    is_running = true;
}

void BehaviorTree::stop() {
    is_running = false;
}

// AIAgent implementation
AIAgent::AIAgent(int id, const std::string& name) 
    : id(id), name(name), position_x(0), position_y(0), target_x(0), target_y(0),
      speed(100.0f), health(100.0f), max_health(100.0f), is_alive(true) {
}

void AIAgent::add_state(std::shared_ptr<AIState> state) {
    states.push_back(state);
}

void AIAgent::remove_state(const std::string& state_name) {
    states.erase(std::remove_if(states.begin(), states.end(),
        [&state_name](const std::shared_ptr<AIState>& state) {
            return state->get_name() == state_name;
        }), states.end());
}

void AIAgent::set_current_state(const std::string& state_name) {
    if (current_state) {
        current_state->exit();
    }
    
    auto it = std::find_if(states.begin(), states.end(),
        [&state_name](const std::shared_ptr<AIState>& state) {
            return state->get_name() == state_name;
        });
    
    if (it != states.end()) {
        current_state = *it;
        current_state->enter();
    }
}

void AIAgent::set_behavior_tree(std::shared_ptr<BehaviorTree> tree) {
    behavior_tree = tree;
}

void AIAgent::set_blackboard_value(const std::string& key, const Value& value) {
    blackboard[key] = value;
}

Value AIAgent::get_blackboard_value(const std::string& key) const {
    auto it = blackboard.find(key);
    return (it != blackboard.end()) ? it->second : Value::nil();
}

bool AIAgent::has_blackboard_value(const std::string& key) const {
    return blackboard.find(key) != blackboard.end();
}

void AIAgent::remove_blackboard_value(const std::string& key) {
    blackboard.erase(key);
}

void AIAgent::set_position(float x, float y) {
    position_x = x;
    position_y = y;
}

void AIAgent::get_position(float& x, float& y) const {
    x = position_x;
    y = position_y;
}

void AIAgent::set_target(float x, float y) {
    target_x = x;
    target_y = y;
}

void AIAgent::get_target(float& x, float& y) const {
    x = target_x;
    y = target_y;
}

void AIAgent::set_speed(float speed) {
    this->speed = speed;
}

void AIAgent::set_health(float health) {
    this->health = std::max(0.0f, std::min(health, max_health));
}

void AIAgent::set_max_health(float max_health) {
    this->max_health = max_health;
    if (health > max_health) {
        health = max_health;
    }
}

void AIAgent::set_alive(bool alive) {
    is_alive = alive;
}

void AIAgent::update(float delta_time) {
    if (!is_alive) return;
    
    // Update current state
    if (current_state) {
        NodeResult result = current_state->update();
        (void)result; // Suppress unused variable warning
        // Handle state transitions based on result
    }
    
    // Update behavior tree
    if (behavior_tree) {
        behavior_tree->execute();
    }
    
    // Move towards target
    float dx = target_x - position_x;
    float dy = target_y - position_y;
    float distance = std::sqrt(dx * dx + dy * dy);
    
    if (distance > 1.0f) {
        float move_distance = speed * delta_time;
        if (move_distance > distance) {
            move_distance = distance;
        }
        
        float move_x = (dx / distance) * move_distance;
        float move_y = (dy / distance) * move_distance;
        
        position_x += move_x;
        position_y += move_y;
    }
}

// AISystem implementation
AISystem::AISystem() 
    : next_agent_id(0), update_interval(1.0f/60.0f), last_update_time(0.0f) {
}

int AISystem::create_agent(const std::string& name) {
    auto agent = std::make_unique<AIAgent>(next_agent_id++, name);
    int id = agent->get_id();
    agents.push_back(std::move(agent));
    return id;
}

void AISystem::remove_agent(int agent_id) {
    agents.erase(std::remove_if(agents.begin(), agents.end(),
        [agent_id](const std::unique_ptr<AIAgent>& agent) {
            return agent->get_id() == agent_id;
        }), agents.end());
}

AIAgent* AISystem::get_agent(int agent_id) {
    auto it = std::find_if(agents.begin(), agents.end(),
        [agent_id](const std::unique_ptr<AIAgent>& agent) {
            return agent->get_id() == agent_id;
        });
    return (it != agents.end()) ? it->get() : nullptr;
}

std::shared_ptr<BehaviorTree> AISystem::create_behavior_tree(const std::string& name) {
    auto tree = std::make_shared<BehaviorTree>(name);
    behavior_trees.push_back(tree);
    return tree;
}

void AISystem::remove_behavior_tree(const std::string& name) {
    behavior_trees.erase(std::remove_if(behavior_trees.begin(), behavior_trees.end(),
        [&name](const std::shared_ptr<BehaviorTree>& tree) {
            return tree->get_name() == name;
        }), behavior_trees.end());
}

std::shared_ptr<BehaviorTree> AISystem::get_behavior_tree(const std::string& name) {
    auto it = std::find_if(behavior_trees.begin(), behavior_trees.end(),
        [&name](const std::shared_ptr<BehaviorTree>& tree) {
            return tree->get_name() == name;
        });
    return (it != behavior_trees.end()) ? *it : nullptr;
}

void AISystem::update(float delta_time) {
    last_update_time += delta_time;
    
    if (last_update_time >= update_interval) {
        for (auto& agent : agents) {
            agent->update(delta_time);
        }
        last_update_time = 0.0f;
    }
}

void AISystem::set_update_interval(float interval) {
    update_interval = interval;
}

int AISystem::get_agent_count() const {
    return static_cast<int>(agents.size());
}

std::vector<AIAgent*> AISystem::get_agents_in_radius(float x, float y, float radius) {
    std::vector<AIAgent*> result;
    
    for (auto& agent : agents) {
        float ax, ay;
        agent->get_position(ax, ay);
        float distance = std::sqrt((ax - x) * (ax - x) + (ay - y) * (ay - y));
        if (distance <= radius) {
            result.push_back(agent.get());
        }
    }
    
    return result;
}

AIAgent* AISystem::get_nearest_agent(float x, float y) {
    AIAgent* nearest = nullptr;
    float nearest_distance = std::numeric_limits<float>::max();
    
    for (auto& agent : agents) {
        float ax, ay;
        agent->get_position(ax, ay);
        float distance = std::sqrt((ax - x) * (ax - x) + (ay - y) * (ay - y));
        if (distance < nearest_distance) {
            nearest_distance = distance;
            nearest = agent.get();
        }
    }
    
    return nearest;
}

std::shared_ptr<BehaviorTree> AISystem::create_patrol_behavior(float start_x, float start_y, float end_x, float end_y) {
    auto tree = create_behavior_tree("patrol");
    auto root = tree->create_node("patrol_root", NodeType::SEQUENCE);
    
    // Create patrol nodes
    auto move_to_start = tree->create_node("move_to_start", NodeType::ACTION);
    auto move_to_end = tree->create_node("move_to_end", NodeType::ACTION);
    
    // Set up patrol logic
    move_to_start->set_action_function([start_x, start_y]() {
        // Move to start position
        return NodeResult::SUCCESS;
    });
    
    move_to_end->set_action_function([end_x, end_y]() {
        // Move to end position
        return NodeResult::SUCCESS;
    });
    
    root->add_child(move_to_start);
    root->add_child(move_to_end);
    tree->set_root(root);
    
    return tree;
}

std::shared_ptr<BehaviorTree> AISystem::create_chase_behavior(int target_agent_id) {
    auto tree = create_behavior_tree("chase");
    auto root = tree->create_node("chase_root", NodeType::SEQUENCE);
    
    auto chase_target = tree->create_node("chase_target", NodeType::ACTION);
    chase_target->set_action_function([target_agent_id]() {
        // Chase target agent
        return NodeResult::SUCCESS;
    });
    
    root->add_child(chase_target);
    tree->set_root(root);
    
    return tree;
}

std::shared_ptr<BehaviorTree> AISystem::create_attack_behavior(int target_agent_id, float attack_range) {
    auto tree = create_behavior_tree("attack");
    auto root = tree->create_node("attack_root", NodeType::SEQUENCE);
    
    auto check_range = tree->create_node("check_range", NodeType::CONDITION);
    auto attack = tree->create_node("attack", NodeType::ACTION);
    
    check_range->set_condition_function([target_agent_id, attack_range]() {
        // Check if target is in range
        return true; // Simplified
    });
    
    attack->set_action_function([target_agent_id]() {
        // Attack target
        return NodeResult::SUCCESS;
    });
    
    root->add_child(check_range);
    root->add_child(attack);
    tree->set_root(root);
    
    return tree;
}

std::shared_ptr<BehaviorTree> AISystem::create_flee_behavior(int threat_agent_id, float flee_distance) {
    auto tree = create_behavior_tree("flee");
    auto root = tree->create_node("flee_root", NodeType::SEQUENCE);
    
    auto flee = tree->create_node("flee", NodeType::ACTION);
    flee->set_action_function([threat_agent_id, flee_distance]() {
        // Flee from threat
        return NodeResult::SUCCESS;
    });
    
    root->add_child(flee);
    tree->set_root(root);
    
    return tree;
}

// Native function implementations
Value ai_init_system(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_ai_system) {
        g_ai_system = std::make_unique<AISystem>();
    }
    return Value::nil();
}

Value ai_create_agent(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_ai_system) return Value::from_int(-1);
    
    std::string name = args[0].as_string();
    int agent_id = g_ai_system->create_agent(name);
    return Value::from_int(agent_id);
}

Value ai_remove_agent(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_ai_system) return Value::nil();
    
    int agent_id = static_cast<int>(args[0].as_int());
    g_ai_system->remove_agent(agent_id);
    return Value::nil();
}

Value ai_set_agent_position(const std::vector<Value>& args) {
    if (args.size() != 3 || !g_ai_system) return Value::nil();
    
    int agent_id = static_cast<int>(args[0].as_int());
    float x = static_cast<float>(args[1].as_number());
    float y = static_cast<float>(args[2].as_number());
    
    AIAgent* agent = g_ai_system->get_agent(agent_id);
    if (agent) {
        agent->set_position(x, y);
    }
    return Value::nil();
}

Value ai_set_agent_target(const std::vector<Value>& args) {
    if (args.size() != 3 || !g_ai_system) return Value::nil();
    
    int agent_id = static_cast<int>(args[0].as_int());
    float x = static_cast<float>(args[1].as_number());
    float y = static_cast<float>(args[2].as_number());
    
    AIAgent* agent = g_ai_system->get_agent(agent_id);
    if (agent) {
        agent->set_target(x, y);
    }
    return Value::nil();
}

Value ai_set_agent_speed(const std::vector<Value>& args) {
    if (args.size() != 2 || !g_ai_system) return Value::nil();
    
    int agent_id = static_cast<int>(args[0].as_int());
    float speed = static_cast<float>(args[1].as_number());
    
    AIAgent* agent = g_ai_system->get_agent(agent_id);
    if (agent) {
        agent->set_speed(speed);
    }
    return Value::nil();
}

Value ai_get_agent_position(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_ai_system) return Value::from_number(0);
    
    int agent_id = static_cast<int>(args[0].as_int());
    AIAgent* agent = g_ai_system->get_agent(agent_id);
    if (agent) {
        float x, y;
        agent->get_position(x, y);
        return Value::from_number(x); // Return x coordinate
    }
    return Value::from_number(0);
}

Value ai_get_agent_x(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_ai_system) return Value::from_number(0);
    
    int agent_id = static_cast<int>(args[0].as_int());
    AIAgent* agent = g_ai_system->get_agent(agent_id);
    if (agent) {
        float x, y;
        agent->get_position(x, y);
        return Value::from_number(x);
    }
    return Value::from_number(0);
}

Value ai_get_agent_y(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_ai_system) return Value::from_number(0);
    
    int agent_id = static_cast<int>(args[0].as_int());
    AIAgent* agent = g_ai_system->get_agent(agent_id);
    if (agent) {
        float x, y;
        agent->get_position(x, y);
        return Value::from_number(y);
    }
    return Value::from_number(0);
}

Value ai_create_patrol_behavior(const std::vector<Value>& args) {
    if (args.size() != 4 || !g_ai_system) return Value::from_bool(false);
    
    float start_x = static_cast<float>(args[0].as_number());
    float start_y = static_cast<float>(args[1].as_number());
    float end_x = static_cast<float>(args[2].as_number());
    float end_y = static_cast<float>(args[3].as_number());
    
    auto tree = g_ai_system->create_patrol_behavior(start_x, start_y, end_x, end_y);
    return Value::from_bool(tree != nullptr);
}

Value ai_set_agent_behavior(const std::vector<Value>& args) {
    if (args.size() != 2 || !g_ai_system) return Value::nil();
    
    int agent_id = static_cast<int>(args[0].as_int());
    std::string behavior_name = args[1].as_string();
    
    AIAgent* agent = g_ai_system->get_agent(agent_id);
    auto tree = g_ai_system->get_behavior_tree(behavior_name);
    
    if (agent && tree) {
        agent->set_behavior_tree(tree);
    }
    return Value::nil();
}

Value ai_update_system(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_ai_system) return Value::nil();
    
    float delta_time = static_cast<float>(args[0].as_number());
    g_ai_system->update(delta_time);
    return Value::nil();
}

Value ai_get_agent_count(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_ai_system) return Value::from_int(0);
    
    int count = g_ai_system->get_agent_count();
    return Value::from_int(count);
}

void register_ai_functions(FunctionRegistry& registry) {
    registry.add("INITAISYSTEM", NativeFn{"INITAISYSTEM", 0, ai_init_system});
    registry.add("CREATEAIAGENT", NativeFn{"CREATEAIAGENT", 1, ai_create_agent});
    registry.add("REMOVEAIAGENT", NativeFn{"REMOVEAIAGENT", 1, ai_remove_agent});
    registry.add("SETAIAGENTPOSITION", NativeFn{"SETAIAGENTPOSITION", 3, ai_set_agent_position});
    registry.add("SETAIAGENTTARGET", NativeFn{"SETAIAGENTTARGET", 3, ai_set_agent_target});
    registry.add("SETAIAGENTSPEED", NativeFn{"SETAIAGENTSPEED", 2, ai_set_agent_speed});
    registry.add("GETAIAGENTPOSITION", NativeFn{"GETAIAGENTPOSITION", 1, ai_get_agent_position});
    registry.add("GETAIAGENTX", NativeFn{"GETAIAGENTX", 1, ai_get_agent_x});
    registry.add("GETAIAGENTY", NativeFn{"GETAIAGENTY", 1, ai_get_agent_y});
    registry.add("CREATEPATROLBEHAVIOR", NativeFn{"CREATEPATROLBEHAVIOR", 4, ai_create_patrol_behavior});
    registry.add("SETAIAGENTBEHAVIOR", NativeFn{"SETAIAGENTBEHAVIOR", 2, ai_set_agent_behavior});
    registry.add("UPDATEAISYSTEM", NativeFn{"UPDATEAISYSTEM", 1, ai_update_system});
    registry.add("GETAIAGENTCOUNT", NativeFn{"GETAIAGENTCOUNT", 0, ai_get_agent_count});
}

} // namespace bas

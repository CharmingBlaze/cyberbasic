#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include "bas/ast.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>
#include <iostream>

namespace bas {

// Enhanced state system data structures
struct StateHookData {
    enum Type { Enter, Exit, Update };
    Type type;
    std::vector<std::string> code; // Stored as code strings for execution
};

struct TransitionData {
    std::string fromState;
    std::string toState;
    std::string condition; // Condition code as string
    int priority{0};
    bool enabled{true};
};

struct StateData {
    std::string name;
    std::string animation;
    double animationBlend{0.0};
    double waitTime{0.0};
    double elapsedTime{0.0};
    bool enabled{true};
    std::vector<StateHookData> hooks;
    std::vector<TransitionData> transitions;
    std::vector<std::string> body; // State body code
};

struct StateSystemData {
    std::string name;
    std::vector<StateData> states;
    std::string currentState;
    std::string previousState;
    std::unordered_map<std::string, int> stateIndexMap; // name -> index
    int attachedEntityId{0}; // Entity this system is attached to
    bool enabled{true};
};

static std::unordered_map<std::string, StateSystemData> g_state_systems;
static std::unordered_map<int, std::string> g_entity_systems; // entityId -> systemName

// Helper: Execute code string
static Value execute_code(const std::string& code, FunctionRegistry& R) {
    // This would need to parse and execute the code
    // For now, return nil - full implementation would integrate with interpreter
    return Value::nil();
}

// STATE name ... END STATE
static Value state_decl_handler(const std::vector<Value>& args) {
    // This will be called from parser/interpreter when STATE block is parsed
    // For now, return nil
    return Value::nil();
}

// TRANSITION from TO to WHEN condition
static Value transition_decl_handler(const std::vector<Value>& args) {
    if (args.size() < 3) {
        return Value::nil();
    }
    
    std::string fromState = args[0].as_string();
    std::string toState = args[1].as_string();
    std::string condition = args.size() > 2 ? args[2].as_string() : "TRUE";
    int priority = args.size() > 3 ? static_cast<int>(args[3].as_int()) : 0;
    
    // Store transition in current state system context
    // This would be set by parser/interpreter context
    return Value::nil();
}

// ON ENTER/EXIT/UPDATE ... END ON
static Value state_hook_handler(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::nil();
    }
    
    std::string hookType = args[0].as_string(); // "ENTER", "EXIT", "UPDATE"
    std::string code = args[1].as_string();
    
    // Store hook in current state context
    return Value::nil();
}

// DEFINE STATE SYSTEM name ... END SYSTEM
static Value state_system_decl_handler(const std::vector<Value>& args) {
    if (args.empty()) {
        return Value::nil();
    }
    
    std::string systemName = args[0].as_string();
    
    StateSystemData& system = g_state_systems[systemName];
    system.name = systemName;
    system.currentState = "";
    system.previousState = "";
    
    Value::Map obj;
    obj["_type"] = Value::from_string("StateSystem");
    obj["name"] = Value::from_string(systemName);
    
    return Value::from_map(std::move(obj));
}

// ATTACH SYSTEM systemName TO entity
static Value attach_system_handler(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::nil();
    }
    
    std::string systemName = args[0].as_string();
    
    // Get entity ID
    int entityId = 0;
    if (args[1].is_map()) {
        const auto& map = args[1].as_map();
        auto idIt = map.find("_id");
        if (idIt != map.end() && idIt->second.is_int()) {
            entityId = idIt->second.as_int();
        }
    } else if (args[1].is_int()) {
        entityId = args[1].as_int();
    }
    
    if (entityId > 0) {
        g_entity_systems[entityId] = systemName;
        auto systemIt = g_state_systems.find(systemName);
        if (systemIt != g_state_systems.end()) {
            systemIt->second.attachedEntityId = entityId;
        }
    }
    
    return Value::nil();
}

// ADD STATE stateName TO systemName
static Value add_state_handler(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::nil();
    }
    
    std::string stateName = args[0].as_string();
    std::string systemName = args[1].as_string();
    
    auto systemIt = g_state_systems.find(systemName);
    if (systemIt == g_state_systems.end()) {
        return Value::nil();
    }
    
    StateData newState;
    newState.name = stateName;
    newState.enabled = true;
    
    systemIt->second.states.push_back(newState);
    systemIt->second.stateIndexMap[stateName] = static_cast<int>(systemIt->second.states.size() - 1);
    
    return Value::nil();
}

// REMOVE STATE stateName FROM systemName
static Value remove_state_handler(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::nil();
    }
    
    std::string stateName = args[0].as_string();
    std::string systemName = args[1].as_string();
    
    auto systemIt = g_state_systems.find(systemName);
    if (systemIt == g_state_systems.end()) {
        return Value::nil();
    }
    
    auto indexIt = systemIt->second.stateIndexMap.find(stateName);
    if (indexIt == systemIt->second.stateIndexMap.end()) {
        return Value::nil();
    }
    
    int index = indexIt->second;
    systemIt->second.states.erase(systemIt->second.states.begin() + index);
    systemIt->second.stateIndexMap.erase(indexIt);
    
    // Rebuild index map
    systemIt->second.stateIndexMap.clear();
    for (size_t i = 0; i < systemIt->second.states.size(); ++i) {
        systemIt->second.stateIndexMap[systemIt->second.states[i].name] = static_cast<int>(i);
    }
    
    return Value::nil();
}

// ENABLE/DISABLE STATE stateName
static Value enable_disable_state_handler(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::nil();
    }
    
    bool enable = args[0].as_bool();
    std::string stateName = args[1].as_string();
    std::string systemName = args.size() > 2 ? args[2].as_string() : "";
    
    if (systemName.empty()) {
        // Enable/disable in all systems
        for (auto& pair : g_state_systems) {
            auto indexIt = pair.second.stateIndexMap.find(stateName);
            if (indexIt != pair.second.stateIndexMap.end()) {
                pair.second.states[indexIt->second].enabled = enable;
            }
        }
    } else {
        auto systemIt = g_state_systems.find(systemName);
        if (systemIt != g_state_systems.end()) {
            auto indexIt = systemIt->second.stateIndexMap.find(stateName);
            if (indexIt != systemIt->second.stateIndexMap.end()) {
                systemIt->second.states[indexIt->second].enabled = enable;
            }
        }
    }
    
    return Value::nil();
}

// DEBUG STATES [systemName]
static Value debug_states_handler(const std::vector<Value>& args) {
    std::string systemName = args.empty() ? "" : args[0].as_string();
    
    if (systemName.empty()) {
        // Debug all systems
        for (const auto& pair : g_state_systems) {
            std::cout << "State System: " << pair.first << std::endl;
            std::cout << "  Current State: " << pair.second.currentState << std::endl;
            std::cout << "  States: ";
            for (const auto& state : pair.second.states) {
                std::cout << state.name << " ";
            }
            std::cout << std::endl;
        }
    } else {
        auto systemIt = g_state_systems.find(systemName);
        if (systemIt != g_state_systems.end()) {
            const auto& system = systemIt->second;
            std::cout << "State System: " << system.name << std::endl;
            std::cout << "  Current State: " << system.currentState << std::endl;
            std::cout << "  Previous State: " << system.previousState << std::endl;
            std::cout << "  States:" << std::endl;
            for (const auto& state : system.states) {
                std::cout << "    - " << state.name;
                if (!state.enabled) std::cout << " (DISABLED)";
                std::cout << std::endl;
                if (!state.animation.empty()) {
                    std::cout << "      Animation: " << state.animation << std::endl;
                }
                std::cout << "      Transitions:" << std::endl;
                for (const auto& trans : state.transitions) {
                    std::cout << "        -> " << trans.toState << " WHEN " << trans.condition;
                    if (trans.priority > 0) std::cout << " (priority: " << trans.priority << ")";
                    std::cout << std::endl;
                }
            }
        }
    }
    
    return Value::nil();
}

// EXPORT SYSTEM systemName TO filePath
static Value export_system_handler(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::nil();
    }
    
    std::string systemName = args[0].as_string();
    std::string filePath = args[1].as_string();
    
    auto systemIt = g_state_systems.find(systemName);
    if (systemIt == g_state_systems.end()) {
        return Value::nil();
    }
    
    // Export to JSON (would use JSON.stringify in full implementation)
    // For now, just return nil
    return Value::nil();
}

// IMPORT SYSTEM systemName FROM filePath
static Value import_system_handler(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::nil();
    }
    
    std::string systemName = args[0].as_string();
    std::string filePath = args[1].as_string();
    
    // Import from JSON (would use JSON.parse in full implementation)
    // For now, just return nil
    return Value::nil();
}

// Update state system (called each frame)
static Value update_state_system_handler(const std::vector<Value>& args) {
    if (args.empty()) {
        return Value::nil();
    }
    
    std::string systemName = args[0].as_string();
    double deltaTime = args.size() > 1 ? args[1].as_number() : 0.016;
    
    auto systemIt = g_state_systems.find(systemName);
    if (systemIt == g_state_systems.end() || !systemIt->second.enabled) {
        return Value::nil();
    }
    
    StateSystemData& system = systemIt->second;
    
    if (system.currentState.empty()) {
        return Value::nil();
    }
    
    auto stateIndexIt = system.stateIndexMap.find(system.currentState);
    if (stateIndexIt == system.stateIndexMap.end()) {
        return Value::nil();
    }
    
    StateData& currentState = system.states[stateIndexIt->second];
    
    if (!currentState.enabled) {
        return Value::nil();
    }
    
    // Update elapsed time
    currentState.elapsedTime += deltaTime;
    
    // Check transitions
    for (const auto& trans : currentState.transitions) {
        if (!trans.enabled) continue;
        
        // Evaluate condition (would need full expression evaluation)
        // For now, check if condition is "TRUE" or simple checks
        bool shouldTransition = false;
        if (trans.condition == "TRUE" || trans.condition == "true") {
            shouldTransition = true;
        }
        // In full implementation, would evaluate condition expression
        
        if (shouldTransition && currentState.elapsedTime >= currentState.waitTime) {
            // Transition to new state
            system.previousState = system.currentState;
            system.currentState = trans.toState;
            
            // Execute ON EXIT hook for old state
            for (const auto& hook : currentState.hooks) {
                if (hook.type == StateHookData::Exit) {
                    // Execute hook code
                }
            }
            
            // Execute ON ENTER hook for new state
            auto newStateIndexIt = system.stateIndexMap.find(trans.toState);
            if (newStateIndexIt != system.stateIndexMap.end()) {
                StateData& newState = system.states[newStateIndexIt->second];
                newState.elapsedTime = 0.0;
                
                for (const auto& hook : newState.hooks) {
                    if (hook.type == StateHookData::Enter) {
                        // Execute hook code
                    }
                }
            }
            
            break;
        }
    }
    
    // Execute ON UPDATE hook
    for (const auto& hook : currentState.hooks) {
        if (hook.type == StateHookData::Update) {
            // Execute hook code
        }
    }
    
    return Value::nil();
}

// Register modern state system functions
void register_modern_state_system(FunctionRegistry& R) {
    R.add("STATE_DECL", NativeFn{"STATE_DECL", -1, state_decl_handler});
    R.add("TRANSITION_DECL", NativeFn{"TRANSITION_DECL", -1, transition_decl_handler});
    R.add("STATE_HOOK", NativeFn{"STATE_HOOK", -1, state_hook_handler});
    R.add("DEFINE_STATE_SYSTEM", NativeFn{"DEFINE_STATE_SYSTEM", 1, state_system_decl_handler});
    R.add("ATTACH_SYSTEM", NativeFn{"ATTACH_SYSTEM", 2, attach_system_handler});
    R.add("ADD_STATE", NativeFn{"ADD_STATE", 2, add_state_handler});
    R.add("REMOVE_STATE", NativeFn{"REMOVE_STATE", 2, remove_state_handler});
    R.add("ENABLE_DISABLE_STATE", NativeFn{"ENABLE_DISABLE_STATE", 3, enable_disable_state_handler});
    R.add("DEBUG_STATES", NativeFn{"DEBUG_STATES", 1, debug_states_handler});
    R.add("EXPORT_SYSTEM", NativeFn{"EXPORT_SYSTEM", 2, export_system_handler});
    R.add("IMPORT_SYSTEM", NativeFn{"IMPORT_SYSTEM", 2, import_system_handler});
    R.add("UPDATE_STATE_SYSTEM", NativeFn{"UPDATE_STATE_SYSTEM", 2, update_state_system_handler});
}

} // namespace bas


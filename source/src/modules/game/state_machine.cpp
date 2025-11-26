#include "bas/state_machine.hpp"
#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <functional>

namespace bas {

// State machine data
struct StateMachineData {
    std::string currentState;
    std::string previousState;
    std::unordered_map<std::string, std::string> states;  // state name -> handler function name
    std::vector<std::string> stateStack;  // For push/pop operations
    int id;
    
    StateMachineData() : id(0) {}
};

static std::unordered_map<int, StateMachineData> g_state_machines;
static int g_next_state_machine_id = 1;

// StateMachine.create([initialState]) -> StateMachine
static Value statemachine_create(const std::vector<Value>& args) {
    int id = g_next_state_machine_id++;
    StateMachineData& sm = g_state_machines[id];
    sm.id = id;
    sm.currentState = args.size() > 0 ? args[0].as_string() : "";
    sm.previousState = "";
    
    Value::Map obj;
    obj["_type"] = Value::from_string("StateMachine");
    obj["_id"] = Value::from_int(id);
    obj["currentState"] = Value::from_string(sm.currentState);
    obj["previousState"] = Value::from_string(sm.previousState);
    
    return Value::from_map(std::move(obj));
}

// StateMachine.addState(stateMachine, stateName, handlerFunction)
static Value statemachine_addState(const std::vector<Value>& args) {
    if (args.size() < 3 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    int id = static_cast<int>(idIt->second.as_int());
    auto smIt = g_state_machines.find(id);
    if (smIt == g_state_machines.end()) {
        return Value::nil();
    }
    
    std::string stateName = args[1].as_string();
    std::string handlerName = args[2].as_string();
    
    smIt->second.states[stateName] = handlerName;
    
    return Value::nil();
}

// StateMachine.setState(stateMachine, newState)
static Value statemachine_setState(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    int id = static_cast<int>(idIt->second.as_int());
    auto smIt = g_state_machines.find(id);
    if (smIt == g_state_machines.end()) {
        return Value::nil();
    }
    
    std::string newState = args[1].as_string();
    smIt->second.previousState = smIt->second.currentState;
    smIt->second.currentState = newState;
    
    return Value::nil();
}

// StateMachine.pushState(stateMachine, newState)
static Value statemachine_pushState(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    int id = static_cast<int>(idIt->second.as_int());
    auto smIt = g_state_machines.find(id);
    if (smIt == g_state_machines.end()) {
        return Value::nil();
    }
    
    // Push current state to stack
    if (!smIt->second.currentState.empty()) {
        smIt->second.stateStack.push_back(smIt->second.currentState);
    }
    
    std::string newState = args[1].as_string();
    smIt->second.previousState = smIt->second.currentState;
    smIt->second.currentState = newState;
    
    return Value::nil();
}

// StateMachine.popState(stateMachine)
static Value statemachine_popState(const std::vector<Value>& args) {
    if (args.size() < 1 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    int id = static_cast<int>(idIt->second.as_int());
    auto smIt = g_state_machines.find(id);
    if (smIt == g_state_machines.end() || smIt->second.stateStack.empty()) {
        return Value::nil();
    }
    
    smIt->second.previousState = smIt->second.currentState;
    smIt->second.currentState = smIt->second.stateStack.back();
    smIt->second.stateStack.pop_back();
    
    return Value::nil();
}

// StateMachine.getState(stateMachine) -> string
static Value statemachine_getState(const std::vector<Value>& args) {
    if (args.size() < 1 || !args[0].is_map()) {
        return Value::from_string("");
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::from_string("");
    }
    
    int id = static_cast<int>(idIt->second.as_int());
    auto smIt = g_state_machines.find(id);
    if (smIt == g_state_machines.end()) {
        return Value::from_string("");
    }
    
    return Value::from_string(smIt->second.currentState);
}

// StateMachine.getPreviousState(stateMachine) -> string
static Value statemachine_getPreviousState(const std::vector<Value>& args) {
    if (args.size() < 1 || !args[0].is_map()) {
        return Value::from_string("");
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::from_string("");
    }
    
    int id = static_cast<int>(idIt->second.as_int());
    auto smIt = g_state_machines.find(id);
    if (smIt == g_state_machines.end()) {
        return Value::from_string("");
    }
    
    return Value::from_string(smIt->second.previousState);
}

// StateMachine.isState(stateMachine, stateName) -> bool
static Value statemachine_isState(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::from_bool(false);
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::from_bool(false);
    }
    
    int id = static_cast<int>(idIt->second.as_int());
    auto smIt = g_state_machines.find(id);
    if (smIt == g_state_machines.end()) {
        return Value::from_bool(false);
    }
    
    std::string stateName = args[1].as_string();
    return Value::from_bool(smIt->second.currentState == stateName);
}

void register_state_machine(FunctionRegistry& registry) {
    registry.add("STATEMACHINE_CREATE", NativeFn{"STATEMACHINE_CREATE", 1, statemachine_create});
    registry.add("STATEMACHINE_ADDSTATE", NativeFn{"STATEMACHINE_ADDSTATE", 3, statemachine_addState});
    registry.add("STATEMACHINE_SETSTATE", NativeFn{"STATEMACHINE_SETSTATE", 2, statemachine_setState});
    registry.add("STATEMACHINE_PUSHSTATE", NativeFn{"STATEMACHINE_PUSHSTATE", 2, statemachine_pushState});
    registry.add("STATEMACHINE_POPSTATE", NativeFn{"STATEMACHINE_POPSTATE", 1, statemachine_popState});
    registry.add("STATEMACHINE_GETSTATE", NativeFn{"STATEMACHINE_GETSTATE", 1, statemachine_getState});
    registry.add("STATEMACHINE_GETPREVIOUSSTATE", NativeFn{"STATEMACHINE_GETPREVIOUSSTATE", 1, statemachine_getPreviousState});
    registry.add("STATEMACHINE_ISSTATE", NativeFn{"STATEMACHINE_ISSTATE", 2, statemachine_isState});
}

}


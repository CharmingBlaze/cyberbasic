#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

namespace bas {

// ===== ENUM SYSTEM =====

struct EnumData {
    std::string name;
    std::unordered_map<std::string, long long> values;
    long long nextValue{0};
};

static std::unordered_map<std::string, EnumData> g_enums;

// Enum(name, value1, value2, ...) - Create enum with values
static Value enum_constructor(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string enumName = args[0].as_string();
    EnumData& enumData = g_enums[enumName];
    enumData.name = enumName;
    enumData.nextValue = 0;
    enumData.values.clear();
    
    // Add enum values
    for (size_t i = 1; i < args.size(); ++i) {
        if (args[i].is_string()) {
            std::string valueName = args[i].as_string();
            enumData.values[valueName] = enumData.nextValue++;
        }
    }
    
    // Return enum object
    Value::Map enumObj;
    enumObj["_type"] = Value::from_string("Enum");
    enumObj["_name"] = Value::from_string(enumName);
    Value::Map valuesMap;
    for (const auto& pair : enumData.values) {
        valuesMap[pair.first] = Value::from_int(pair.second);
    }
    enumObj["_values"] = Value::from_map(std::move(valuesMap));
    
    return Value::from_map(std::move(enumObj));
}

// Enum.getValue(enum, valueName) -> int
static Value enum_getValue(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map() || !args[1].is_string()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto nameIt = map.find("_name");
    if (nameIt == map.end() || !nameIt->second.is_string()) {
        return Value::nil();
    }
    
    std::string enumName = nameIt->second.as_string();
    auto enumIt = g_enums.find(enumName);
    if (enumIt == g_enums.end()) {
        return Value::nil();
    }
    
    std::string valueName = args[1].as_string();
    auto valueIt = enumIt->second.values.find(valueName);
    if (valueIt == enumIt->second.values.end()) {
        return Value::nil();
    }
    
    return Value::from_int(valueIt->second);
}

// Enum.getName(enum, value) -> string
static Value enum_getName(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto nameIt = map.find("_name");
    if (nameIt == map.end() || !nameIt->second.is_string()) {
        return Value::nil();
    }
    
    std::string enumName = nameIt->second.as_string();
    auto enumIt = g_enums.find(enumName);
    if (enumIt == g_enums.end()) {
        return Value::nil();
    }
    
    long long value = args[1].as_int();
    for (const auto& pair : enumIt->second.values) {
        if (pair.second == value) {
            return Value::from_string(pair.first);
        }
    }
    
    return Value::nil();
}

// Enum.hasValue(enum, valueName) -> bool
static Value enum_hasValue(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map() || !args[1].is_string()) {
        return Value::from_bool(false);
    }
    
    const auto& map = args[0].as_map();
    auto nameIt = map.find("_name");
    if (nameIt == map.end() || !nameIt->second.is_string()) {
        return Value::from_bool(false);
    }
    
    std::string enumName = nameIt->second.as_string();
    auto enumIt = g_enums.find(enumName);
    if (enumIt == g_enums.end()) {
        return Value::from_bool(false);
    }
    
    std::string valueName = args[1].as_string();
    return Value::from_bool(enumIt->second.values.count(valueName) > 0);
}

// ===== ENHANCED DICTIONARY SYSTEM =====

// Dictionary() or Dictionary(key1, value1, key2, value2, ...) - Create dictionary
static Value dictionary_constructor(const std::vector<Value>& args) {
    Value::Map dict;
    dict["_type"] = Value::from_string("Dictionary");
    
    // If even number of args, treat as key-value pairs
    if (args.size() % 2 == 0 && args.size() > 0) {
        for (size_t i = 0; i < args.size(); i += 2) {
            std::string key = args[i].as_string();
            dict[key] = args[i + 1];
        }
    }
    
    return Value::from_map(std::move(dict));
}

// Dictionary.set(dict, key, value) -> dict
static Value dictionary_set(const std::vector<Value>& args) {
    if (args.size() < 3 || !args[0].is_map()) {
        return Value::nil();
    }
    
    Value::Map dict = args[0].as_map();
    std::string key = args[1].as_string();
    dict[key] = args[2];
    return Value::from_map(std::move(dict));
}

// Dictionary.get(dict, key, [default]) -> value
static Value dictionary_get(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& dict = args[0].as_map();
    std::string key = args[1].as_string();
    auto it = dict.find(key);
    if (it != dict.end()) {
        return it->second;
    }
    
    if (args.size() >= 3) {
        return args[2]; // default value
    }
    
    return Value::nil();
}

// Dictionary.has(dict, key) -> bool
static Value dictionary_has(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::from_bool(false);
    }
    
    const auto& dict = args[0].as_map();
    std::string key = args[1].as_string();
    return Value::from_bool(dict.count(key) > 0);
}

// Dictionary.remove(dict, key) -> dict
static Value dictionary_remove(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    Value::Map dict = args[0].as_map();
    std::string key = args[1].as_string();
    dict.erase(key);
    return Value::from_map(std::move(dict));
}

// Dictionary.keys(dict) -> array
static Value dictionary_keys(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::from_array({});
    }
    
    const auto& dict = args[0].as_map();
    Value::Array keys;
    keys.reserve(dict.size());
    
    for (const auto& pair : dict) {
        // Skip internal type markers
        if (pair.first != "_type" && pair.first != "_name" && pair.first != "_id") {
            keys.push_back(Value::from_string(pair.first));
        }
    }
    
    return Value::from_array(std::move(keys));
}

// Dictionary.values(dict) -> array
static Value dictionary_values(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::from_array({});
    }
    
    const auto& dict = args[0].as_map();
    Value::Array values;
    values.reserve(dict.size());
    
    for (const auto& pair : dict) {
        // Skip internal type markers
        if (pair.first != "_type" && pair.first != "_name" && pair.first != "_id") {
            values.push_back(pair.second);
        }
    }
    
    return Value::from_array(std::move(values));
}

// Dictionary.size(dict) -> int
static Value dictionary_size(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::from_int(0);
    }
    
    const auto& dict = args[0].as_map();
    size_t count = 0;
    for (const auto& pair : dict) {
        // Skip internal type markers
        if (pair.first != "_type" && pair.first != "_name" && pair.first != "_id") {
            count++;
        }
    }
    
    return Value::from_int(static_cast<long long>(count));
}

// Dictionary.clear(dict) -> empty dict
static Value dictionary_clear(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::from_map({});
    }
    
    Value::Map dict;
    dict["_type"] = Value::from_string("Dictionary");
    return Value::from_map(std::move(dict));
}

// Dictionary.merge(dict1, dict2) -> merged dict
static Value dictionary_merge(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map() || !args[1].is_map()) {
        return Value::nil();
    }
    
    Value::Map result = args[0].as_map();
    const auto& dict2 = args[1].as_map();
    
    for (const auto& pair : dict2) {
        // Skip internal type markers
        if (pair.first != "_type" && pair.first != "_name" && pair.first != "_id") {
            result[pair.first] = pair.second;
        }
    }
    
    return Value::from_map(std::move(result));
}

// ===== STATE SYSTEM =====

struct StateData {
    std::string name;
    std::string handlerFunction;
    Value::Map data;
    bool active{false};
};

static std::unordered_map<std::string, StateData> g_states;
static std::string g_current_state;

// State.create(name, [handlerFunction]) -> State
static Value state_create(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string stateName = args[0].as_string();
    StateData& state = g_states[stateName];
    state.name = stateName;
    state.handlerFunction = args.size() > 1 ? args[1].as_string() : "";
    state.active = false;
    
    Value::Map stateObj;
    stateObj["_type"] = Value::from_string("State");
    stateObj["_name"] = Value::from_string(stateName);
    stateObj["handler"] = Value::from_string(state.handlerFunction);
    stateObj["active"] = Value::from_bool(state.active);
    stateObj["data"] = Value::from_map(state.data);
    
    return Value::from_map(std::move(stateObj));
}

// State.setActive(state, active) -> state
static Value state_setActive(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto nameIt = map.find("_name");
    if (nameIt == map.end() || !nameIt->second.is_string()) {
        return args[0];
    }
    
    std::string stateName = nameIt->second.as_string();
    auto stateIt = g_states.find(stateName);
    if (stateIt == g_states.end()) {
        return args[0];
    }
    
    bool active = args[1].as_bool();
    stateIt->second.active = active;
    
    if (active) {
        g_current_state = stateName;
    }
    
    Value::Map updated = map;
    updated["active"] = Value::from_bool(active);
    
    return Value::from_map(std::move(updated));
}

// State.getCurrent() -> string
static Value state_getCurrent(const std::vector<Value>& args) {
    return Value::from_string(g_current_state);
}

// State.setData(state, key, value) -> state
static Value state_setData(const std::vector<Value>& args) {
    if (args.size() < 3 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto nameIt = map.find("_name");
    if (nameIt == map.end() || !nameIt->second.is_string()) {
        return args[0];
    }
    
    std::string stateName = nameIt->second.as_string();
    auto stateIt = g_states.find(stateName);
    if (stateIt == g_states.end()) {
        return args[0];
    }
    
    std::string key = args[1].as_string();
    stateIt->second.data[key] = args[2];
    
    Value::Map updated = map;
    Value::Map dataCopy = stateIt->second.data;
    updated["data"] = Value::from_map(std::move(dataCopy));
    
    return Value::from_map(std::move(updated));
}

// State.getData(state, key, [default]) -> value
static Value state_getData(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto nameIt = map.find("_name");
    if (nameIt == map.end() || !nameIt->second.is_string()) {
        return Value::nil();
    }
    
    std::string stateName = nameIt->second.as_string();
    auto stateIt = g_states.find(stateName);
    if (stateIt == g_states.end()) {
        return Value::nil();
    }
    
    std::string key = args[1].as_string();
    auto dataIt = stateIt->second.data.find(key);
    if (dataIt != stateIt->second.data.end()) {
        return dataIt->second;
    }
    
    if (args.size() >= 3) {
        return args[2]; // default value
    }
    
    return Value::nil();
}

// Register all functions
void register_enums_and_dicts(FunctionRegistry& R) {
    // Enum functions
    R.add("ENUM", NativeFn{"ENUM", -1, enum_constructor});
    R.add("ENUM_GETVALUE", NativeFn{"ENUM_GETVALUE", 2, enum_getValue});
    R.add("ENUM_GETNAME", NativeFn{"ENUM_GETNAME", 2, enum_getName});
    R.add("ENUM_HASVALUE", NativeFn{"ENUM_HASVALUE", 2, enum_hasValue});
    
    // Dictionary functions
    R.add("DICTIONARY", NativeFn{"DICTIONARY", -1, dictionary_constructor});
    R.add("DICTIONARY_SET", NativeFn{"DICTIONARY_SET", 3, dictionary_set});
    R.add("DICTIONARY_GET", NativeFn{"DICTIONARY_GET", -1, dictionary_get});
    R.add("DICTIONARY_HAS", NativeFn{"DICTIONARY_HAS", 2, dictionary_has});
    R.add("DICTIONARY_REMOVE", NativeFn{"DICTIONARY_REMOVE", 2, dictionary_remove});
    R.add("DICTIONARY_KEYS", NativeFn{"DICTIONARY_KEYS", 1, dictionary_keys});
    R.add("DICTIONARY_VALUES", NativeFn{"DICTIONARY_VALUES", 1, dictionary_values});
    R.add("DICTIONARY_SIZE", NativeFn{"DICTIONARY_SIZE", 1, dictionary_size});
    R.add("DICTIONARY_CLEAR", NativeFn{"DICTIONARY_CLEAR", 1, dictionary_clear});
    R.add("DICTIONARY_MERGE", NativeFn{"DICTIONARY_MERGE", 2, dictionary_merge});
    
    // State functions
    R.add("STATE_CREATE", NativeFn{"STATE_CREATE", -1, state_create});
    R.add("STATE_SETACTIVE", NativeFn{"STATE_SETACTIVE", 2, state_setActive});
    R.add("STATE_GETCURRENT", NativeFn{"STATE_GETCURRENT", 0, state_getCurrent});
    R.add("STATE_SETDATA", NativeFn{"STATE_SETDATA", 3, state_setData});
    R.add("STATE_GETDATA", NativeFn{"STATE_GETDATA", -1, state_getData});
}

} // namespace bas





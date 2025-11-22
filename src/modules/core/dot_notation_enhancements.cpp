#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

namespace bas {

// ===== PROPERTY DESCRIPTORS =====

struct PropertyDescriptor {
    Value value;
    std::function<Value()> getter;
    std::function<void(const Value&)> setter;
    bool writable{true};
    bool enumerable{true};
    bool configurable{true};
    bool hasGetter{false};
    bool hasSetter{false};
};

static std::unordered_map<std::string, std::unordered_map<std::string, PropertyDescriptor>> g_property_descriptors;

// Object.defineProperty(obj, prop, descriptor) - Define property with descriptor
static Value object_defineProperty(const std::vector<Value>& args) {
    if (args.size() < 3 || !args[0].is_map() || !args[1].is_string() || !args[2].is_map()) {
        return Value::from_bool(false);
    }
    
    const auto& objMap = args[0].as_map();
    std::string objId;
    auto idIt = objMap.find("_id");
    if (idIt != objMap.end() && idIt->second.is_int()) {
        objId = std::to_string(idIt->second.as_int());
    } else {
        // Use object pointer as ID
        objId = std::to_string(reinterpret_cast<intptr_t>(&objMap));
    }
    
    std::string propName = args[1].as_string();
    const auto& descriptor = args[2].as_map();
    
    PropertyDescriptor desc;
    
    // Get value
    auto valueIt = descriptor.find("value");
    if (valueIt != descriptor.end()) {
        desc.value = valueIt->second;
    }
    
    // Get getter
    auto getterIt = descriptor.find("get");
    if (getterIt != descriptor.end() && getterIt->second.is_map()) {
        // In a real implementation, this would store a function reference
        desc.hasGetter = true;
    }
    
    // Get setter
    auto setterIt = descriptor.find("set");
    if (setterIt != descriptor.end() && setterIt->second.is_map()) {
        desc.hasSetter = true;
    }
    
    // Get flags
    auto writableIt = descriptor.find("writable");
    if (writableIt != descriptor.end()) {
        desc.writable = writableIt->second.as_bool();
    }
    
    auto enumerableIt = descriptor.find("enumerable");
    if (enumerableIt != descriptor.end()) {
        desc.enumerable = enumerableIt->second.as_bool();
    }
    
    auto configurableIt = descriptor.find("configurable");
    if (configurableIt != descriptor.end()) {
        desc.configurable = configurableIt->second.as_bool();
    }
    
    g_property_descriptors[objId][propName] = desc;
    
    return Value::from_bool(true);
}

// Object.getOwnPropertyDescriptor(obj, prop) -> descriptor
static Value object_getOwnPropertyDescriptor(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map() || !args[1].is_string()) {
        return Value::nil();
    }
    
    const auto& objMap = args[0].as_map();
    std::string objId;
    auto idIt = objMap.find("_id");
    if (idIt != objMap.end() && idIt->second.is_int()) {
        objId = std::to_string(idIt->second.as_int());
    } else {
        objId = std::to_string(reinterpret_cast<intptr_t>(&objMap));
    }
    
    std::string propName = args[1].as_string();
    auto objIt = g_property_descriptors.find(objId);
    if (objIt == g_property_descriptors.end()) {
        return Value::nil();
    }
    
    auto propIt = objIt->second.find(propName);
    if (propIt == objIt->second.end()) {
        return Value::nil();
    }
    
    const PropertyDescriptor& desc = propIt->second;
    Value::Map descriptor;
    descriptor["value"] = desc.value;
    descriptor["writable"] = Value::from_bool(desc.writable);
    descriptor["enumerable"] = Value::from_bool(desc.enumerable);
    descriptor["configurable"] = Value::from_bool(desc.configurable);
    descriptor["hasGetter"] = Value::from_bool(desc.hasGetter);
    descriptor["hasSetter"] = Value::from_bool(desc.hasSetter);
    
    return Value::from_map(std::move(descriptor));
}

// ===== COMPUTED PROPERTIES =====

// Object.defineComputed(obj, prop, getter, [setter]) -> bool
static Value object_defineComputed(const std::vector<Value>& args) {
    if (args.size() < 3 || !args[0].is_map() || !args[1].is_string()) {
        return Value::from_bool(false);
    }
    
    const auto& objMap = args[0].as_map();
    std::string objId;
    auto idIt = objMap.find("_id");
    if (idIt != objMap.end() && idIt->second.is_int()) {
        objId = std::to_string(idIt->second.as_int());
    } else {
        objId = std::to_string(reinterpret_cast<intptr_t>(&objMap));
    }
    
    std::string propName = args[1].as_string();
    PropertyDescriptor desc;
    desc.hasGetter = true;
    desc.hasSetter = args.size() > 3;
    desc.value = args[2]; // Store getter function
    
    if (args.size() > 3) {
        desc.value = args[3]; // Store setter function
    }
    
    g_property_descriptors[objId][propName] = desc;
    
    return Value::from_bool(true);
}

// ===== PROPERTY OBSERVERS =====

struct PropertyObserver {
    std::string property;
    std::function<void(const Value&, const Value&)> callback;
};

static std::unordered_map<std::string, std::vector<PropertyObserver>> g_property_observers;

// Object.watch(obj, prop, callback) -> bool
static Value object_watch(const std::vector<Value>& args) {
    if (args.size() < 3 || !args[0].is_map() || !args[1].is_string()) {
        return Value::from_bool(false);
    }
    
    const auto& objMap = args[0].as_map();
    auto idIt = objMap.find("_id");
    if (idIt == objMap.end() || !idIt->second.is_int()) {
        return Value::from_bool(false);
    }
    
    std::string objId = std::to_string(idIt->second.as_int());
    std::string propName = args[1].as_string();
    
    // In a real implementation, this would store the callback
    PropertyObserver obs;
    obs.property = propName;
    g_property_observers[objId].push_back(obs);
    
    return Value::from_bool(true);
}

// Object.unwatch(obj, prop) -> bool
static Value object_unwatch(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map() || !args[1].is_string()) {
        return Value::from_bool(false);
    }
    
    const auto& objMap = args[0].as_map();
    auto idIt = objMap.find("_id");
    if (idIt == objMap.end() || !idIt->second.is_int()) {
        return Value::from_bool(false);
    }
    
    std::string objId = std::to_string(idIt->second.as_int());
    std::string propName = args[1].as_string();
    
    auto obsIt = g_property_observers.find(objId);
    if (obsIt == g_property_observers.end()) {
        return Value::from_bool(false);
    }
    
    auto& observers = obsIt->second;
    observers.erase(
        std::remove_if(observers.begin(), observers.end(),
            [&propName](const PropertyObserver& obs) { return obs.property == propName; }),
        observers.end()
    );
    
    return Value::from_bool(true);
}

// ===== BETTER METHOD CHAINING =====

// Helper to check if method should return self for chaining
static bool is_chainable_method(const std::string& methodName) {
    // Methods that typically return self for chaining
    static const std::vector<std::string> chainableMethods = {
        "SET", "ADD", "REMOVE", "CLEAR", "UPDATE", "MOVE", "SCALE", "ROTATE",
        "NORMALIZE", "TRANSFORM", "PUSH", "POP", "SHIFT", "UNSHIFT"
    };
    
    std::string upper = methodName;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    
    for (const auto& chainable : chainableMethods) {
        if (upper.find(chainable) != std::string::npos) {
            return true;
        }
    }
    
    return false;
}

// ===== PROPERTY SUGGESTIONS =====

// Get available properties for an object (for better error messages)
static Value::Array get_available_properties(const Value& obj) {
    Value::Array props;
    
    if (obj.is_map()) {
        const auto& map = obj.as_map();
        for (const auto& pair : map) {
            // Skip internal properties
            if (pair.first[0] != '_' || pair.first == "_type") {
                props.push_back(Value::from_string(pair.first));
            }
        }
    }
    
    return props;
}

// Find similar property names (fuzzy matching)
static Value::Array find_similar_properties(const std::string& target, const Value::Array& available) {
    Value::Array suggestions;
    std::string targetUpper;
    targetUpper.reserve(target.size());
    for (char c : target) {
        targetUpper.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
    }
    
    for (const auto& prop : available) {
        if (!prop.is_string()) continue;
        std::string propUpper;
        propUpper.reserve(prop.as_string().size());
        for (char c : prop.as_string()) {
            propUpper.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
        }
        
        // Check similarity
        bool similar = false;
        
        // Starts with target
        if (propUpper.find(targetUpper) == 0) {
            similar = true;
        }
        // Target starts with property
        else if (targetUpper.find(propUpper) == 0) {
            similar = true;
        }
        // Contains target
        else if (propUpper.find(targetUpper) != std::string::npos) {
            similar = true;
        }
        // Levenshtein-like: check if most characters match
        else {
            size_t matches = 0;
            size_t minLen = std::min(targetUpper.size(), propUpper.size());
            for (size_t i = 0; i < minLen; ++i) {
                if (targetUpper[i] == propUpper[i]) matches++;
            }
            if (matches >= minLen / 2) {
                similar = true;
            }
        }
        
        if (similar) {
            suggestions.push_back(prop);
            if (suggestions.size() >= 5) break; // Limit to 5 suggestions
        }
    }
    
    return suggestions;
}

// Object.getProperties(obj) -> array of property names
static Value object_getProperties(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::from_array({});
    }
    
    return Value::from_array(get_available_properties(args[0]));
}

// Object.hasProperty(obj, prop) -> bool
static Value object_hasProperty(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map() || !args[1].is_string()) {
        return Value::from_bool(false);
    }
    
    const auto& map = args[0].as_map();
    std::string propName = args[1].as_string();
    std::string propUpper;
    propUpper.reserve(propName.size());
    for (char c : propName) {
        propUpper.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
    }
    
    return Value::from_bool(map.count(propUpper) > 0);
}

// Object.freeze(obj) -> obj (make object immutable)
static Value object_freeze(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return args.empty() ? Value::nil() : args[0];
    }
    
    // In a real implementation, this would mark the object as frozen
    // For now, just return the object
    return args[0];
}

// Object.seal(obj) -> obj (prevent adding new properties)
static Value object_seal(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return args.empty() ? Value::nil() : args[0];
    }
    
    // In a real implementation, this would mark the object as sealed
    return args[0];
}

// Object.keys(obj) -> array of keys
static Value object_keys(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::from_array({});
    }
    
    const auto& map = args[0].as_map();
    Value::Array keys;
    keys.reserve(map.size());
    
    for (const auto& pair : map) {
        // Skip internal properties
        if (pair.first[0] != '_' || pair.first == "_type") {
            keys.push_back(Value::from_string(pair.first));
        }
    }
    
    return Value::from_array(std::move(keys));
}

// Object.values(obj) -> array of values
static Value object_values(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::from_array({});
    }
    
    const auto& map = args[0].as_map();
    Value::Array values;
    values.reserve(map.size());
    
    for (const auto& pair : map) {
        // Skip internal properties
        if (pair.first[0] != '_' || pair.first == "_type") {
            values.push_back(pair.second);
        }
    }
    
    return Value::from_array(std::move(values));
}

// Object.entries(obj) -> array of [key, value] pairs
static Value object_entries(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::from_array({});
    }
    
    const auto& map = args[0].as_map();
    Value::Array entries;
    entries.reserve(map.size());
    
    for (const auto& pair : map) {
        // Skip internal properties
        if (pair.first[0] != '_' || pair.first == "_type") {
            Value::Map entry;
            entry["key"] = Value::from_string(pair.first);
            entry["value"] = pair.second;
            entries.push_back(Value::from_map(std::move(entry)));
        }
    }
    
    return Value::from_array(std::move(entries));
}

// Object.assign(target, ...sources) -> target (copy properties)
static Value object_assign(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return args.empty() ? Value::nil() : args[0];
    }
    
    Value::Map result = args[0].as_map();
    
    for (size_t i = 1; i < args.size(); ++i) {
        if (!args[i].is_map()) continue;
        const auto& source = args[i].as_map();
        for (const auto& pair : source) {
            // Skip internal properties
            if (pair.first[0] != '_' || pair.first == "_type") {
                result[pair.first] = pair.second;
            }
        }
    }
    
    return Value::from_map(std::move(result));
}

// Object.create(proto, [properties]) -> new object
static Value object_create(const std::vector<Value>& args) {
    Value::Map obj;
    obj["_type"] = Value::from_string("Object");
    
    // Set prototype if provided
    if (args.size() > 0 && args[0].is_map()) {
        obj["_proto"] = args[0];
    }
    
    // Add properties if provided
    if (args.size() > 1 && args[1].is_map()) {
        const auto& properties = args[1].as_map();
        for (const auto& pair : properties) {
            obj[pair.first] = pair.second;
        }
    }
    
    return Value::from_map(std::move(obj));
}

// ===== ENHANCED METHOD RESOLUTION =====

// Object.getMethod(obj, methodName) -> method or nil
static Value object_getMethod(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map() || !args[1].is_string()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    std::string methodName = args[1].as_string();
    std::string methodUpper;
    methodUpper.reserve(methodName.size());
    for (char c : methodName) {
        methodUpper.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
    }
    
    // Check if method exists as property
    auto it = map.find(methodUpper);
    if (it != map.end() && it->second.is_map()) {
        const auto& methodMap = it->second.as_map();
        auto typeIt = methodMap.find("_type");
        if (typeIt != map.end() && typeIt->second.is_string() && 
            typeIt->second.as_string() == "Method") {
            return it->second;
        }
    }
    
    // Check object type for method
    auto typeIt = map.find("_type");
    if (typeIt != map.end() && typeIt->second.is_string()) {
        std::string objType = typeIt->second.as_string();
        // In a real implementation, would look up method in registry
        // For now, return nil
    }
    
    return Value::nil();
}

// ===== REGISTER ALL FUNCTIONS =====

void register_dot_notation_enhancements(FunctionRegistry& R) {
    // Property descriptors
    R.add("OBJECT_DEFINEPROPERTY", NativeFn{"OBJECT_DEFINEPROPERTY", 3, object_defineProperty});
    R.add("OBJECT_GETOWNPROPERTYDESCRIPTOR", NativeFn{"OBJECT_GETOWNPROPERTYDESCRIPTOR", 2, object_getOwnPropertyDescriptor});
    R.add("OBJECT_DEFINECOMPUTED", NativeFn{"OBJECT_DEFINECOMPUTED", -1, object_defineComputed});
    
    // Property observers
    R.add("OBJECT_WATCH", NativeFn{"OBJECT_WATCH", 3, object_watch});
    R.add("OBJECT_UNWATCH", NativeFn{"OBJECT_UNWATCH", 2, object_unwatch});
    
    // Object utilities
    R.add("OBJECT_GETPROPERTIES", NativeFn{"OBJECT_GETPROPERTIES", 1, object_getProperties});
    R.add("OBJECT_HASPROPERTY", NativeFn{"OBJECT_HASPROPERTY", 2, object_hasProperty});
    R.add("OBJECT_FREEZE", NativeFn{"OBJECT_FREEZE", 1, object_freeze});
    R.add("OBJECT_SEAL", NativeFn{"OBJECT_SEAL", 1, object_seal});
    R.add("OBJECT_KEYS", NativeFn{"OBJECT_KEYS", 1, object_keys});
    R.add("OBJECT_VALUES", NativeFn{"OBJECT_VALUES", 1, object_values});
    R.add("OBJECT_ENTRIES", NativeFn{"OBJECT_ENTRIES", 1, object_entries});
    R.add("OBJECT_ASSIGN", NativeFn{"OBJECT_ASSIGN", -1, object_assign});
    R.add("OBJECT_CREATE", NativeFn{"OBJECT_CREATE", -1, object_create});
    R.add("OBJECT_GETMETHOD", NativeFn{"OBJECT_GETMETHOD", 2, object_getMethod});
}

} // namespace bas

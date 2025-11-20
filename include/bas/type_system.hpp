#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "value.hpp"

namespace bas {

// Type system for user-defined types and type checking
struct TypeInfo {
    std::string name;
    std::string parentType; // For inheritance
    bool hasParent{false};
    std::vector<std::pair<std::string, std::string>> fields; // field name -> type name
    std::unordered_map<std::string, std::string> methods; // method name -> signature
    
    [[nodiscard]] bool isSubtypeOf(const std::string& otherType) const {
        if (name == otherType) return true;
        if (hasParent && parentType == otherType) return true;
        return false;
    }
};

class TypeRegistry {
public:
    void registerType(const TypeInfo& type) {
        types_[type.name] = type;
    }
    
    [[nodiscard]] TypeInfo* getType(const std::string& name) {
        auto it = types_.find(name);
        return it != types_.end() ? &it->second : nullptr;
    }
    
    [[nodiscard]] const TypeInfo* getType(const std::string& name) const {
        auto it = types_.find(name);
        return it != types_.end() ? &it->second : nullptr;
    }
    
    [[nodiscard]] bool hasType(const std::string& name) const {
        return types_.find(name) != types_.end();
    }
    
    [[nodiscard]] Value createInstance(const std::string& typeName) const {
        const TypeInfo* type = getType(typeName);
        if (!type) {
            return Value::nil();
        }
        
        Value::Map obj;
        obj["_type"] = Value::from_string(typeName);
        
        // Initialize fields with default values
        for (const auto& [fieldName, fieldType] : type->fields) {
            obj[fieldName] = getDefaultValue(fieldType);
        }
        
        // If has parent, copy parent fields
        if (type->hasParent) {
            const TypeInfo* parent = getType(type->parentType);
            if (parent) {
                for (const auto& [fieldName, fieldType] : parent->fields) {
                    if (obj.find(fieldName) == obj.end()) {
                        obj[fieldName] = getDefaultValue(fieldType);
                    }
                }
            }
        }
        
        return Value::from_map(std::move(obj));
    }
    
    [[nodiscard]] bool isSubtype(const std::string& subtype, const std::string& supertype) const {
        const TypeInfo* sub = getType(subtype);
        if (!sub) return false;
        return sub->isSubtypeOf(supertype);
    }
    
private:
    std::unordered_map<std::string, TypeInfo> types_;
    
    [[nodiscard]] static Value getDefaultValue(const std::string& typeName) {
        // Map type names to default values
        if (typeName == "INTEGER" || typeName == "INT") {
            return Value::from_int(0);
        } else if (typeName == "DOUBLE" || typeName == "SINGLE" || typeName == "FLOAT") {
            return Value::from_number(0.0);
        } else if (typeName == "STRING") {
            return Value::from_string("");
        } else if (typeName == "BOOLEAN" || typeName == "BOOL") {
            return Value::from_bool(false);
        } else {
            // For user-defined types, return nil (will be handled by createInstance)
            return Value::nil();
        }
    }
};

// Global type registry access
TypeRegistry* get_type_registry();
void set_type_registry(TypeRegistry* registry);

} // namespace bas


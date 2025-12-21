#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <cctype>
#include "value.hpp"

namespace bas {

// Helper: Normalize identifier to lowercase for case-insensitive matching
inline std::string normalize_type_name(const std::string& s) {
  std::string r;
  r.reserve(s.size());
  for (char c : s) {
    r.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
  }
  return r;
}

// Type system for user-defined types and type checking
struct TypeInfo {
    std::string name;
    std::string parentType; // For inheritance
    bool hasParent{false};
    std::vector<std::pair<std::string, std::string>> fields; // field name -> type name
    std::unordered_map<std::string, std::string> methods; // method name -> signature
    
    [[nodiscard]] bool isSubtypeOf(const std::string& otherType) const {
        std::string normalized_other = normalize_type_name(otherType);
        if (normalize_type_name(name) == normalized_other) return true;
        if (hasParent && normalize_type_name(parentType) == normalized_other) return true;
        return false;
    }
};

class TypeRegistry {
public:
    void registerType(const TypeInfo& type) {
        // Normalize type name for case-insensitive storage
        std::string normalized_name = normalize_type_name(type.name);
        TypeInfo normalized_type = type;
        normalized_type.name = normalized_name;
        
        // Normalize parent type name if present
        if (normalized_type.hasParent) {
            normalized_type.parentType = normalize_type_name(type.parentType);
        }
        
        // Normalize field names
        std::vector<std::pair<std::string, std::string>> normalized_fields;
        for (const auto& [fieldName, fieldType] : type.fields) {
            normalized_fields.emplace_back(normalize_type_name(fieldName), normalize_type_name(fieldType));
        }
        normalized_type.fields = normalized_fields;
        
        // Normalize method names
        std::unordered_map<std::string, std::string> normalized_methods;
        for (const auto& [methodName, signature] : type.methods) {
            normalized_methods[normalize_type_name(methodName)] = signature;
        }
        normalized_type.methods = normalized_methods;
        
        types_[normalized_name] = normalized_type;
    }
    
    [[nodiscard]] TypeInfo* getType(const std::string& name) {
        std::string normalized = normalize_type_name(name);
        auto it = types_.find(normalized);
        return it != types_.end() ? &it->second : nullptr;
    }
    
    [[nodiscard]] const TypeInfo* getType(const std::string& name) const {
        std::string normalized = normalize_type_name(name);
        auto it = types_.find(normalized);
        return it != types_.end() ? &it->second : nullptr;
    }
    
    [[nodiscard]] bool hasType(const std::string& name) const {
        std::string normalized = normalize_type_name(name);
        return types_.find(normalized) != types_.end();
    }
    
    [[nodiscard]] Value createInstance(const std::string& typeName) const {
        std::string normalized = normalize_type_name(typeName);
        const TypeInfo* type = getType(normalized);
        if (!type) {
            return Value::nil();
        }
        
        Value::Map obj;
        obj[normalize_type_name("_type")] = Value::from_string(normalized);
        
        // Initialize fields with default values (field names already normalized)
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
        std::string normalized_sub = normalize_type_name(subtype);
        std::string normalized_super = normalize_type_name(supertype);
        const TypeInfo* sub = getType(normalized_sub);
        if (!sub) return false;
        return sub->isSubtypeOf(normalized_super);
    }
    
private:
    std::unordered_map<std::string, TypeInfo> types_;
    
    [[nodiscard]] static Value getDefaultValue(const std::string& typeName) {
        // Normalize type name for case-insensitive comparison
        std::string normalized = normalize_type_name(typeName);
        
        // Map type names to default values (all comparisons normalized)
        if (normalized == "integer" || normalized == "int") {
            return Value::from_int(0);
        } else if (normalized == "double" || normalized == "single" || normalized == "float") {
            return Value::from_number(0.0);
        } else if (normalized == "string") {
            return Value::from_string("");
        } else if (normalized == "boolean" || normalized == "bool") {
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


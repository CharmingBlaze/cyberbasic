#pragma once

// Include STL headers at global scope BEFORE opening namespace bas
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <filesystem>
#include <functional>

// Include bas headers (they properly open/close their own namespaces)
#include "bas/runtime.hpp"
#include "bas/namespace_registry.hpp"
#include "bas/type_system.hpp"

namespace bas {

// Forward declaration
using NativeFunctionCallback = std::function<Value(const std::vector<Value>&)>;

/**
 * @brief Registry for native function callbacks that can be bound to YAML functions
 * 
 * This allows YAML modules to reference pre-compiled C++ functions by name.
 */
class NativeFunctionRegistry {
public:
    /**
     * @brief Register a native function callback by name
     */
    void register_native(const std::string& name, NativeFunctionCallback callback);
    
    /**
     * @brief Get a native function callback by name
     */
    [[nodiscard]] NativeFunctionCallback* find_native(const std::string& name);
    
    /**
     * @brief Check if a native function is registered
     */
    [[nodiscard]] bool has_native(const std::string& name) const;
    
private:
    std::unordered_map<std::string, NativeFunctionCallback> natives_;
};

/**
 * @brief Runtime YAML module loader for dynamic module loading
 * 
 * Loads YAML module files at runtime and registers:
 * - Functions
 * - Constructors
 * - Methods
 * - Namespaces
 * - Types
 * - Metadata
 * 
 * All keys are normalized using normalize_identifier() for case-insensitivity.
 */
class YamlModuleLoader {
public:
    struct LoadOptions {
        bool strict_mode;      // Strict validation mode
        bool allow_override;   // Allow overriding existing functions
        bool verbose;          // Verbose error reporting
        
        // Default constructor with default values
        LoadOptions() : strict_mode(false), allow_override(false), verbose(false) {}
    };
    
    YamlModuleLoader(FunctionRegistry& registry, 
                     NamespaceRegistry* namespace_registry = nullptr,
                     TypeRegistry* type_registry = nullptr,
                     NativeFunctionRegistry* native_registry = nullptr,
                     LoadOptions options = LoadOptions());
    
    /**
     * @brief Load a single YAML module file
     * @param filepath Path to the YAML file
     * @return true on success, false on error
     */
    bool load_module(const std::filesystem::path& filepath);
    
    /**
     * @brief Load all YAML modules from a directory
     * @param directory Path to directory containing .yaml files
     * @return Number of modules loaded
     */
    size_t load_modules_from_directory(const std::filesystem::path& directory);
    
    /**
     * @brief Get list of loaded module names
     */
    [[nodiscard]] std::vector<std::string> get_loaded_modules() const;
    
    /**
     * @brief Check if a module is loaded
     */
    [[nodiscard]] bool is_loaded(const std::string& module_name) const;
    
    /**
     * @brief Clear all loaded modules (does not unregister functions)
     */
    void clear_loaded_modules();
    
    /**
     * @brief Get module metadata for a loaded module
     */
    struct ModuleMetadata {
        std::string name;
        std::string version;
        std::string author;
        std::string description;
        std::vector<std::string> dependencies;
    };
    
    /**
     * @brief Get metadata for a loaded module
     */
    [[nodiscard]] ModuleMetadata get_module_metadata(const std::string& module_name) const;
    
    /**
     * @brief Get all loaded module names with metadata
     */
    [[nodiscard]] std::vector<ModuleMetadata> get_all_module_metadata() const;
    
    /**
     * @brief Validate a YAML module file without loading it
     * @return true if valid, false otherwise. Error message in error_out if provided.
     */
    bool validate_module_file(const std::filesystem::path& filepath, std::string* error_out = nullptr) const;
    
    /**
     * @brief Set load options
     */
    void set_options(const LoadOptions& options) { options_ = options; }
    
    /**
     * @brief Get current load options
     */
    [[nodiscard]] const LoadOptions& get_options() const { return options_; }

private:
    // Internal YAML parsing (simple parser for our needs)
    struct YamlNode {
        enum Type { Scalar, Sequence, Mapping, Null };
        Type type = Null;
        std::string scalar;
        std::vector<YamlNode> sequence;
        std::unordered_map<std::string, YamlNode> mapping;
        
        [[nodiscard]] bool is_scalar() const { return type == Scalar; }
        [[nodiscard]] bool is_sequence() const { return type == Sequence; }
        [[nodiscard]] bool is_mapping() const { return type == Mapping; }
        [[nodiscard]] bool is_null() const { return type == Null; }
        
        [[nodiscard]] const YamlNode& operator[](const std::string& key) const;
        [[nodiscard]] bool has(const std::string& key) const;
    };
    
    // Simple YAML parser (handles basic YAML structure)
    YamlNode parse_yaml_file(const std::filesystem::path& filepath);
    YamlNode parse_yaml_content(const std::string& content, size_t& pos);
    std::string parse_scalar(const std::string& content, size_t& pos);
    void skip_whitespace(const std::string& content, size_t& pos);
    void skip_line(const std::string& content, size_t& pos);
    
    // Module processing
    bool process_module(const YamlNode& module, const std::filesystem::path& filepath);
    void process_imports(const YamlNode& module, const std::filesystem::path& base_path);
    void process_functions(const YamlNode& module);
    void process_constructors(const YamlNode& module);
    void process_methods(const YamlNode& module);
    void process_namespaces(const YamlNode& module);
    void process_types(const YamlNode& module);
    void process_constants(const YamlNode& module);
    
    // Registration helpers
    void register_function(const YamlNode& func_node);
    void register_constructor(const YamlNode& ctor_node);
    void register_method(const YamlNode& method_node);
    void register_namespace(const YamlNode& ns_node);
    void register_type(const YamlNode& type_node);
    void register_constant(const YamlNode& const_node);
    
    // Helper to create native function from YAML
    NativeFn create_native_function(const YamlNode& func_node);
    
    // Constructor generation
    NativeFn create_constructor(const YamlNode& ctor_node);
    Value create_constructor_object(const YamlNode& ctor_node, const std::vector<Value>& args);
    
    // Schema validation
    bool validate_function_schema(const YamlNode& func_node, std::string& error) const;
    bool validate_constructor_schema(const YamlNode& ctor_node, std::string& error) const;
    bool validate_namespace_schema(const YamlNode& ns_node, std::string& error) const;
    bool validate_type_schema(const YamlNode& type_node, std::string& error) const;
    
    // Normalization helper (uses shared normalize_identifier)
    static std::string normalize_key(const std::string& key) {
        return normalize_identifier(key);
    }
    
    FunctionRegistry& registry_;
    NamespaceRegistry* namespace_registry_;
    TypeRegistry* type_registry_;
    NativeFunctionRegistry* native_registry_;
    LoadOptions options_;
    std::unordered_set<std::string> loaded_modules_;
    std::set<std::filesystem::path> processed_files_; // Prevent circular imports (using set instead of unordered_set because filesystem::path doesn't have std::hash)
    std::unordered_map<std::string, ModuleMetadata> module_metadata_; // Module metadata cache
};

/**
 * @brief Helper function to populate NativeFunctionRegistry from FunctionRegistry
 * 
 * This allows YAML modules to bind to existing built-in functions by name.
 * Useful for creating aliases or exposing functions with different names.
 */
void populate_native_registry_from_function_registry(
    NativeFunctionRegistry& native_registry,
    FunctionRegistry& function_registry);

} // namespace bas


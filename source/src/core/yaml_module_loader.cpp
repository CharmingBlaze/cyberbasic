// Include standard headers at global scope BEFORE any bas headers that open namespace bas
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <functional>

// yaml_module_loader.hpp already includes runtime.hpp, so we don't need to include it again
#include "bas/yaml_module_loader.hpp"

namespace bas {

// Version comparison helpers
namespace {
    struct Version {
        int major = 0;
        int minor = 0;
        int patch = 0;
        
        bool operator<(const Version& other) const {
            if (major != other.major) return major < other.major;
            if (minor != other.minor) return minor < other.minor;
            return patch < other.patch;
        }
        
        bool operator==(const Version& other) const {
            return major == other.major && minor == other.minor && patch == other.patch;
        }
        
        bool operator<=(const Version& other) const {
            return *this < other || *this == other;
        }
        
        bool operator>=(const Version& other) const {
            return !(*this < other);
        }
        
        bool operator>(const Version& other) const {
            return !(*this <= other);
        }
    };
    
    Version parse_version(const std::string& version_str) {
        Version v;
        std::regex version_regex(R"((\d+)\.(\d+)(?:\.(\d+))?)");
        std::smatch match;
        if (std::regex_search(version_str, match, version_regex)) {
            v.major = std::stoi(match[1].str());
            v.minor = std::stoi(match[2].str());
            if (match[3].matched) {
                v.patch = std::stoi(match[3].str());
            }
        }
        return v;
    }
    
    bool check_version_constraint(const std::string& constraint, const std::string& version) {
        if (constraint.empty()) return true;
        
        Version target = parse_version(version);
        
        // Parse constraint: ">= 1.2" or "1.2" or ">=1.2"
        std::string trimmed = constraint;
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        
        if (trimmed.empty()) return true;
        
        // Check for operators
        if (trimmed.find(">=") == 0) {
            Version required = parse_version(trimmed.substr(2));
            return target >= required;
        } else if (trimmed.find("<=") == 0) {
            Version required = parse_version(trimmed.substr(2));
            return target <= required;
        } else if (trimmed.find(">") == 0) {
            Version required = parse_version(trimmed.substr(1));
            return target > required;
        } else if (trimmed.find("<") == 0) {
            Version required = parse_version(trimmed.substr(1));
            return target < required;
        } else if (trimmed.find("==") == 0 || trimmed.find("=") == 0) {
            Version required = parse_version(trimmed.substr(trimmed.find("=") == 0 ? 1 : 2));
            return target == required;
        } else {
            // No operator - exact match
            Version required = parse_version(trimmed);
            return target == required;
        }
    }
}

// NativeFunctionRegistry implementation
void NativeFunctionRegistry::register_native(const std::string& name, NativeFunctionCallback callback) {
    std::string normalized = normalize_identifier(name);
    natives_[normalized] = std::move(callback);
}

NativeFunctionCallback* NativeFunctionRegistry::find_native(const std::string& name) {
    std::string normalized = normalize_identifier(name);
    auto it = natives_.find(normalized);
    return (it != natives_.end()) ? &it->second : nullptr;
}

bool NativeFunctionRegistry::has_native(const std::string& name) const {
    std::string normalized = normalize_identifier(name);
    return natives_.find(normalized) != natives_.end();
}

YamlModuleLoader::YamlModuleLoader(FunctionRegistry& registry,
                                   NamespaceRegistry* namespace_registry,
                                   TypeRegistry* type_registry,
                                   NativeFunctionRegistry* native_registry,
                                   LoadOptions options)
    : registry_(registry)
    , namespace_registry_(namespace_registry)
    , type_registry_(type_registry)
    , native_registry_(native_registry)
    , options_(options)
{
}

bool YamlModuleLoader::load_module(const std::filesystem::path& filepath) {
    if (!std::filesystem::exists(filepath)) {
        return false;
    }
    
    // Check if already processed (prevent circular imports)
    auto canonical = std::filesystem::canonical(filepath);
    if (processed_files_.find(canonical) != processed_files_.end()) {
        return true; // Already loaded
    }
    
    processed_files_.insert(canonical);
    
    // Parse YAML file
    YamlNode module = parse_yaml_file(filepath);
    if (module.is_null()) {
        return false;
    }
    
    // Extract module name from filename
    std::string module_name = normalize_key(filepath.stem().string());
    
    // Extract and store module metadata
    ModuleMetadata metadata;
    metadata.name = module_name;
    if (module.has("module")) {
        metadata.name = normalize_key(module["module"].scalar);
    }
    if (module.has("version")) {
        metadata.version = module["version"].scalar;
    }
    if (module.has("author")) {
        metadata.author = module["author"].scalar;
    }
    if (module.has("description")) {
        metadata.description = module["description"].scalar;
    }
    if (module.has("dependencies")) {
        const auto& deps = module["dependencies"];
        if (deps.is_sequence()) {
            for (const auto& dep : deps.sequence) {
                if (dep.is_scalar()) {
                    metadata.dependencies.push_back(normalize_key(dep.scalar));
                } else if (dep.is_mapping() && dep.has("name")) {
                    // Dependency with version constraint: {name: "rmath", version: ">= 1.2"}
                    std::string dep_name = normalize_key(dep["name"].scalar);
                    std::string dep_version = dep.has("version") ? dep["version"].scalar : "";
                    metadata.dependencies.push_back(dep_name + (dep_version.empty() ? "" : " " + dep_version));
                }
            }
        }
    }
    
    // Validate dependencies exist and check version constraints (in strict mode)
    if (options_.strict_mode) {
        for (const auto& dep_str : metadata.dependencies) {
            // Parse dependency: "rmath >= 1.2" or just "rmath"
            size_t space_pos = dep_str.find(' ');
            std::string dep_name = space_pos != std::string::npos ? dep_str.substr(0, space_pos) : dep_str;
            std::string version_constraint = space_pos != std::string::npos ? dep_str.substr(space_pos + 1) : "";
            
            if (loaded_modules_.find(dep_name) == loaded_modules_.end()) {
                // Check if dependency file exists
                std::filesystem::path dep_path = filepath.parent_path() / (dep_name + ".yaml");
                if (!std::filesystem::exists(dep_path)) {
                    throw std::runtime_error("Module '" + module_name + "' requires dependency '" + dep_name + "' which is not loaded and file not found");
                }
            } else if (!version_constraint.empty()) {
                // Check version constraint
                auto it = module_metadata_.find(dep_name);
                if (it != module_metadata_.end() && !it->second.version.empty()) {
                    if (!check_version_constraint(version_constraint, it->second.version)) {
                        throw std::runtime_error("Module '" + module_name + "' requires dependency '" + dep_name + "' " + version_constraint + " but found version " + it->second.version);
                    }
                }
            }
        }
    }
    
    // Load dependencies first (if not already loaded)
    for (const auto& dep_str : metadata.dependencies) {
        // Parse dependency name (before space)
        size_t space_pos = dep_str.find(' ');
        std::string dep_name = space_pos != std::string::npos ? dep_str.substr(0, space_pos) : dep_str;
        
        if (loaded_modules_.find(dep_name) == loaded_modules_.end()) {
            std::filesystem::path dep_path = filepath.parent_path() / (dep_name + ".yaml");
            if (std::filesystem::exists(dep_path)) {
                load_module(dep_path);
            } else if (options_.strict_mode) {
                throw std::runtime_error("Module '" + module_name + "' requires dependency '" + dep_name + "' but file not found: " + dep_path.string());
            }
        }
    }
    
    loaded_modules_.insert(module_name);
    module_metadata_[module_name] = metadata;
    
    // Process the module
    return process_module(module, filepath);
}

size_t YamlModuleLoader::load_modules_from_directory(const std::filesystem::path& directory) {
    if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory)) {
        return 0;
    }
    
    size_t count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".yaml") {
            if (load_module(entry.path())) {
                count++;
            }
        }
    }
    
    return count;
}

std::vector<std::string> YamlModuleLoader::get_loaded_modules() const {
    return std::vector<std::string>(loaded_modules_.begin(), loaded_modules_.end());
}

bool YamlModuleLoader::is_loaded(const std::string& module_name) const {
    return loaded_modules_.find(normalize_key(module_name)) != loaded_modules_.end();
}

void YamlModuleLoader::clear_loaded_modules() {
    loaded_modules_.clear();
    processed_files_.clear();
}

// Simple YAML parser implementation
YamlModuleLoader::YamlNode YamlModuleLoader::parse_yaml_file(const std::filesystem::path& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return YamlNode{};
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    
    size_t pos = 0;
    return parse_yaml_content(content, pos);
}

YamlModuleLoader::YamlNode YamlModuleLoader::parse_yaml_content(const std::string& content, size_t& pos) {
    YamlNode node;
    skip_whitespace(content, pos);
    
    if (pos >= content.size()) {
        return node; // Null node
    }
    
    // Top-level is always a mapping
    node.type = YamlNode::Mapping;
    
    while (pos < content.size()) {
        // Skip leading whitespace (but track indentation)
        size_t line_start = pos;
        while (pos < content.size() && content[pos] != '\n' && std::isspace(static_cast<unsigned char>(content[pos]))) {
            pos++;
        }
        
        // Check for end of file or empty line
        if (pos >= content.size() || content[pos] == '\n') {
            if (pos < content.size()) pos++; // Skip newline
            continue;
        }
        
        // Check for comment
        if (content[pos] == '#') {
            skip_line(content, pos);
            continue;
        }
        
        // Parse key
        std::string key = parse_scalar(content, pos);
        if (key.empty()) {
            skip_line(content, pos);
            continue;
        }
        
        skip_whitespace(content, pos);
        
        // Expect colon
        if (pos >= content.size() || content[pos] != ':') {
            skip_line(content, pos);
            continue;
        }
        pos++; // Skip colon
        
        skip_whitespace(content, pos);
        
        // Check if value is on same line (inline scalar)
        if (pos < content.size() && content[pos] != '\n') {
            // Inline scalar value
            YamlNode value;
            value.type = YamlNode::Scalar;
            value.scalar = parse_scalar(content, pos);
            node.mapping[normalize_key(key)] = value;
            skip_line(content, pos);
        } else {
            // Value on next line(s) - could be sequence, mapping, or multi-line scalar
            if (pos < content.size() && content[pos] == '\n') {
                pos++; // Skip newline
            }
            
            skip_whitespace(content, pos);
            
            // Check for sequence (starts with '-')
            if (pos < content.size() && content[pos] == '-') {
                YamlNode value;
                value.type = YamlNode::Sequence;
                
                while (pos < content.size()) {
                    // Skip leading whitespace and '-'
                    while (pos < content.size() && (std::isspace(static_cast<unsigned char>(content[pos])) || content[pos] == '-')) {
                        if (content[pos] == '-') {
                            pos++;
                            skip_whitespace(content, pos);
                            break;
                        }
                        pos++;
                    }
                    
                    if (pos >= content.size() || content[pos] == '\n') {
                        if (pos < content.size()) pos++;
                        continue;
                    }
                    
                    if (content[pos] == '#') {
                        skip_line(content, pos);
                        continue;
                    }
                    
                    // Parse sequence item - detect type by first character
                    YamlNode item;
                    size_t item_start = pos;
                    
                    // Check if this is a mapping (starts with key: or {)
                    bool is_mapping = false;
                    if (pos < content.size()) {
                        if (content[pos] == '{') {
                            // Inline mapping
                            is_mapping = true;
                        } else if (content[pos] == '[') {
                            // Inline sequence
                            is_mapping = false; // Will be handled as sequence
                        } else if (content[pos] == '"' || content[pos] == '\'') {
                            // Quoted scalar
                            is_mapping = false;
                        } else if (std::isalnum(static_cast<unsigned char>(content[pos])) || content[pos] == '_' || content[pos] == '-') {
                            // Could be a mapping key (identifier followed by ':')
                            // Look ahead to see if there's a colon on this line (before newline or comment)
                            size_t lookahead = pos;
                            bool found_colon = false;
                            while (lookahead < content.size() && content[lookahead] != '\n' && content[lookahead] != '#') {
                                if (content[lookahead] == ':') {
                                    // Found colon - check if it's followed by space/newline/value
                                    lookahead++;
                                    skip_whitespace(content, lookahead);
                                    // If colon is followed by value (not end of line), it's a mapping
                                    if (lookahead < content.size() && content[lookahead] != '\n' && content[lookahead] != '#') {
                                        found_colon = true;
                                        is_mapping = true;
                                    }
                                    break;
                                }
                                // Stop if we hit whitespace that's not part of the key
                                if (std::isspace(static_cast<unsigned char>(content[lookahead])) && content[lookahead] != ' ') {
                                    break;
                                }
                                lookahead++;
                            }
                            // If no colon found, it's a scalar
                            if (!found_colon) {
                                is_mapping = false;
                            }
                        }
                    }
                    
                    if (is_mapping) {
                        // Parse as mapping (could be inline or multi-line)
                        if (content[pos] == '{') {
                            // Inline mapping - parse until closing brace
                            pos++; // Skip '{'
                            item = parse_yaml_content(content, pos);
                        } else {
                            // Multi-line mapping - parse from current position
                            item = parse_yaml_content(content, pos);
                        }
                    } else if (pos < content.size() && content[pos] == '[') {
                        // Inline sequence
                        pos++; // Skip '['
                        item.type = YamlNode::Sequence;
                        // Parse sequence items until ']'
                        while (pos < content.size() && content[pos] != ']') {
                            skip_whitespace(content, pos);
                            if (pos >= content.size() || content[pos] == ']') break;
                            
                            YamlNode seq_item;
                            if (content[pos] == '"' || content[pos] == '\'') {
                                seq_item.type = YamlNode::Scalar;
                                seq_item.scalar = parse_scalar(content, pos);
                            } else {
                                seq_item.type = YamlNode::Scalar;
                                seq_item.scalar = parse_scalar(content, pos);
                            }
                            item.sequence.push_back(seq_item);
                            
                            // Skip comma if present
                            skip_whitespace(content, pos);
                            if (pos < content.size() && content[pos] == ',') {
                                pos++;
                            }
                        }
                        if (pos < content.size() && content[pos] == ']') {
                            pos++; // Skip ']'
                        }
                    } else {
                        // Scalar item
                        item.type = YamlNode::Scalar;
                        item.scalar = parse_scalar(content, pos);
                    }
                    
                    if (!item.is_null()) {
                        value.sequence.push_back(item);
                    }
                    
                    // Skip to next line
                    skip_line(content, pos);
                    
                    // Check if next line continues sequence (starts with '-')
                    skip_whitespace(content, pos);
                    if (pos >= content.size() || content[pos] != '-') {
                        break; // End of sequence
                    }
                }
                
                node.mapping[normalize_key(key)] = value;
            } else {
                // Could be nested mapping or empty
                size_t saved_pos = pos;
                YamlNode value = parse_yaml_content(content, pos);
                if (!value.is_null() && (value.is_mapping() || value.is_scalar())) {
                    node.mapping[normalize_key(key)] = value;
                } else {
                    // Empty value
                    pos = saved_pos;
                    YamlNode empty_value;
                    empty_value.type = YamlNode::Scalar;
                    empty_value.scalar = "";
                    node.mapping[normalize_key(key)] = empty_value;
                }
            }
        }
    }
    
    return node;
}

std::string YamlModuleLoader::parse_scalar(const std::string& content, size_t& pos) {
    skip_whitespace(content, pos);
    if (pos >= content.size()) return "";
    
    std::string result;
    
    // Handle quoted strings
    if (content[pos] == '"' || content[pos] == '\'') {
        char quote = content[pos];
        pos++;
        while (pos < content.size() && content[pos] != quote) {
            if (content[pos] == '\\' && pos + 1 < content.size()) {
                pos++;
                if (content[pos] == 'n') result += '\n';
                else if (content[pos] == 't') result += '\t';
                else if (content[pos] == 'r') result += '\r';
                else if (content[pos] == '\\') result += '\\';
                else result += content[pos];
            } else {
                result += content[pos];
            }
            pos++;
        }
        if (pos < content.size()) pos++; // Skip closing quote
    } else {
        // Unquoted scalar
        while (pos < content.size() && content[pos] != ':' && content[pos] != '\n' && 
               content[pos] != '#' && content[pos] != '[' && content[pos] != '{') {
            if (content[pos] == '\\' && pos + 1 < content.size() && content[pos + 1] == '\n') {
                pos += 2;
                skip_whitespace(content, pos);
            } else {
                result += content[pos];
                pos++;
            }
        }
    }
    
    // Trim whitespace
    while (!result.empty() && std::isspace(static_cast<unsigned char>(result.back()))) {
        result.pop_back();
    }
    
    return result;
}

void YamlModuleLoader::skip_whitespace(const std::string& content, size_t& pos) {
    while (pos < content.size() && std::isspace(static_cast<unsigned char>(content[pos])) && content[pos] != '\n') {
        pos++;
    }
}

void YamlModuleLoader::skip_line(const std::string& content, size_t& pos) {
    while (pos < content.size() && content[pos] != '\n') {
        pos++;
    }
    if (pos < content.size()) pos++; // Skip newline
}

const YamlModuleLoader::YamlNode& YamlModuleLoader::YamlNode::operator[](const std::string& key) const {
    static YamlNode null_node;
    if (type != Mapping) return null_node;
    std::string normalized_key = normalize_identifier(key);
    auto it = mapping.find(normalized_key);
    return (it != mapping.end()) ? it->second : null_node;
}

bool YamlModuleLoader::YamlNode::has(const std::string& key) const {
    if (type != Mapping) return false;
    std::string normalized_key = normalize_identifier(key);
    return mapping.find(normalized_key) != mapping.end();
}

// Module processing
bool YamlModuleLoader::process_module(const YamlNode& module, const std::filesystem::path& filepath) {
    if (!module.is_mapping()) {
        return false;
    }
    
    // Process imports first
    if (module.has("imports")) {
        process_imports(module["imports"], filepath.parent_path());
    }
    
    // Process functions
    if (module.has("functions")) {
        process_functions(module["functions"]);
    }
    
    // Process constructors
    if (module.has("constructors")) {
        process_constructors(module["constructors"]);
    }
    
    // Process methods
    if (module.has("methods")) {
        process_methods(module["methods"]);
    }
    
    // Process namespaces
    if (module.has("namespaces")) {
        process_namespaces(module["namespaces"]);
    }
    
    // Process types
    if (module.has("types")) {
        process_types(module["types"]);
    }
    
    // Process constants
    if (module.has("constants")) {
        process_constants(module["constants"]);
    }
    
    return true;
}

void YamlModuleLoader::process_imports(const YamlNode& imports, const std::filesystem::path& base_path) {
    if (!imports.is_sequence()) return;
    
    for (const auto& import_node : imports.sequence) {
        if (import_node.is_scalar()) {
            std::filesystem::path import_path = base_path / import_node.scalar;
            load_module(import_path);
        }
    }
}

void YamlModuleLoader::process_functions(const YamlNode& functions) {
    if (!functions.is_sequence()) return;
    
    for (const auto& func_node : functions.sequence) {
        if (func_node.is_mapping()) {
            register_function(func_node);
        }
    }
}

void YamlModuleLoader::process_constructors(const YamlNode& constructors) {
    if (!constructors.is_sequence()) return;
    
    for (const auto& ctor_node : constructors.sequence) {
        if (ctor_node.is_mapping()) {
            register_constructor(ctor_node);
        }
    }
}

void YamlModuleLoader::process_methods(const YamlNode& methods) {
    if (!methods.is_sequence()) return;
    
    for (const auto& method_node : methods.sequence) {
        if (method_node.is_mapping()) {
            register_method(method_node);
        }
    }
}

void YamlModuleLoader::process_namespaces(const YamlNode& namespaces) {
    if (!namespaces.is_mapping() || !namespace_registry_) return;
    
    for (const auto& [ns_name, ns_node] : namespaces.mapping) {
        if (ns_node.is_mapping()) {
            register_namespace(ns_node);
        }
    }
}

void YamlModuleLoader::process_types(const YamlNode& types) {
    if (!types.is_sequence() || !type_registry_) return;
    
    for (const auto& type_node : types.sequence) {
        if (type_node.is_mapping()) {
            register_type(type_node);
        }
    }
}

void YamlModuleLoader::process_constants(const YamlNode& constants) {
    if (!constants.is_sequence()) return;
    
    for (const auto& const_node : constants.sequence) {
        if (const_node.is_mapping()) {
            register_constant(const_node);
        }
    }
}

// Registration implementations
void YamlModuleLoader::register_function(const YamlNode& func_node) {
    // Validate schema
    std::string error;
    if (!validate_function_schema(func_node, error)) {
        if (options_.strict_mode) {
            throw std::runtime_error("Invalid function schema: " + error);
        } else if (options_.verbose) {
            std::cerr << "Warning: Invalid function schema: " << error << std::endl;
        }
        return;
    }
    
    std::string name = normalize_key(func_node["name"].scalar);
    
    // Check for duplicate registration (strict mode)
    if (options_.strict_mode && registry_.find(name) != nullptr) {
        if (!options_.allow_override && !(func_node.has("allow_override") && func_node["allow_override"].scalar == "true")) {
            throw std::runtime_error("Duplicate function registration: " + name + " (use allow_override: true to override)");
        }
    }
    
    NativeFn fn = create_native_function(func_node);
    
    // Check for allow_override policy
    bool allow_override = options_.allow_override || 
                         (func_node.has("allow_override") && func_node["allow_override"].scalar == "true");
    
    if (allow_override) {
        registry_.add_with_policy(name, fn, true);
    } else {
        registry_.add(name, fn);
    }
}

void YamlModuleLoader::register_constructor(const YamlNode& ctor_node) {
    // Validate schema
    std::string error;
    if (!validate_constructor_schema(ctor_node, error)) {
        throw std::runtime_error("Invalid constructor schema: " + error);
    }
    
    std::string name = normalize_key(ctor_node["name"].scalar);
    NativeFn fn = create_constructor(ctor_node);
    
    // Constructors are registered as functions
    bool allow_override = ctor_node.has("allow_override") && 
                          ctor_node["allow_override"].scalar == "true";
    
    if (allow_override) {
        registry_.add_with_policy(name, fn, true);
    } else {
        registry_.add(name, fn);
    }
}

void YamlModuleLoader::register_method(const YamlNode& method_node) {
    // Methods are registered as functions with namespace.method format
    if (!method_node.has("name") || !method_node.has("namespace")) return;
    
    std::string ns_name = normalize_key(method_node["namespace"].scalar);
    std::string method_name = normalize_key(method_node["name"].scalar);
    std::string full_name = ns_name + "." + method_name;
    
    // Create function node with full name
    YamlNode func_node = method_node;
    func_node.mapping[normalize_key("name")] = YamlNode{YamlNode::Scalar, full_name};
    
    register_function(func_node);
    
    // Also register in namespace registry if available
    if (namespace_registry_) {
        // Namespace registry will handle method registration
        // This is a placeholder for future enhancement
    }
}

void YamlModuleLoader::register_namespace(const YamlNode& ns_node) {
    if (!namespace_registry_) return;
    
    // Validate schema
    std::string error;
    if (!validate_namespace_schema(ns_node, error)) {
        throw std::runtime_error("Invalid namespace schema: " + error);
    }
    
    std::string ns_name = normalize_key(ns_node["name"].scalar);
    
    // Build method map: method_name -> function_name
    std::unordered_map<std::string, std::string> methods;
    
    // Register namespace methods with metadata
    if (ns_node.has("methods")) {
        const auto& methods_node = ns_node["methods"];
        if (methods_node.is_sequence()) {
            for (const auto& method_node : methods_node.sequence) {
                if (method_node.is_mapping() && method_node.has("name")) {
                    std::string method_name = normalize_key(method_node["name"].scalar);
                    std::string function_name;
                    
                    // Get function name from method node
                    if (method_node.has("function")) {
                        function_name = normalize_key(method_node["function"].scalar);
                    } else {
                        // Default: namespace_method format
                        function_name = ns_name + "_" + method_name;
                    }
                    
                    methods[method_name] = function_name;
                    
                    // Store method metadata for autocomplete/documentation
                    // This creates a method object with metadata that can be queried later
                    if (namespace_registry_) {
                        Value::Map method_meta;
                        method_meta[normalize_identifier("_type")] = Value::from_string("method");
                        method_meta[normalize_identifier("_namespace")] = Value::from_string(ns_name);
                        method_meta[normalize_identifier("_method")] = Value::from_string(method_name);
                        method_meta[normalize_identifier("_function")] = Value::from_string(function_name);
                        
                        // Add optional metadata
                        if (method_node.has("description")) {
                            method_meta[normalize_identifier("description")] = Value::from_string(method_node["description"].scalar);
                        }
                        if (method_node.has("args")) {
                            std::string args_str = method_node["args"].is_scalar() ? 
                                                  method_node["args"].scalar : "";
                            method_meta[normalize_identifier("args")] = Value::from_string(args_str);
                        }
                        if (method_node.has("returns")) {
                            method_meta[normalize_identifier("returns")] = Value::from_string(method_node["returns"].scalar);
                        }
                        
                        // Store in a metadata map (could be extended to query later)
                        // For now, this metadata is available in the method object structure
                    }
                }
            }
        } else if (methods_node.is_mapping()) {
            // Methods as mapping: method_name: function_name
            for (const auto& [method_name, func_node] : methods_node.mapping) {
                std::string normalized_method = normalize_key(method_name);
                std::string function_name = func_node.is_scalar() ? 
                                            normalize_key(func_node.scalar) : 
                                            normalized_method;
                methods[normalized_method] = function_name;
            }
        }
    }
    
    // Register the namespace
    if (!methods.empty()) {
        namespace_registry_->register_namespace(ns_name, methods);
    }
}

void YamlModuleLoader::register_type(const YamlNode& type_node) {
    if (!type_registry_) return;
    
    // Validate schema
    std::string error;
    if (!validate_type_schema(type_node, error)) {
        throw std::runtime_error("Invalid type schema: " + error);
    }
    
    std::string type_name = normalize_key(type_node["name"].scalar);
    
    TypeInfo type_info;
    type_info.name = type_name;
    
    // Parse parent type
    if (type_node.has("extends")) {
        type_info.hasParent = true;
        type_info.parentType = normalize_key(type_node["extends"].scalar);
    }
    
    // Parse fields
    if (type_node.has("fields")) {
        const YamlNode& fields = type_node["fields"];
        if (fields.is_sequence()) {
            for (const YamlNode& field_node : fields.sequence) {
                if (field_node.is_mapping() && field_node.has("name") && field_node.has("type")) {
                    std::string field_name = normalize_key(field_node["name"].scalar);
                    std::string field_type = normalize_key(field_node["type"].scalar);
                    type_info.fields.emplace_back(field_name, field_type);
                }
            }
        }
    }
    
    // Parse methods
    if (type_node.has("methods")) {
        const YamlNode& methods = type_node["methods"];
        if (methods.is_sequence()) {
            for (const YamlNode& method_node : methods.sequence) {
                if (method_node.is_mapping() && method_node.has("name")) {
                    std::string method_name = normalize_key(method_node["name"].scalar);
                    std::string signature = method_node.has("signature") ? 
                                           method_node["signature"].scalar : "";
                    type_info.methods[method_name] = signature;
                }
            }
        }
    }
    
    type_registry_->registerType(type_info);
}

void YamlModuleLoader::register_constant(const YamlNode& const_node) {
    if (!const_node.has("name") || !const_node.has("value")) return;
    
    std::string name = normalize_key(const_node["name"].scalar);
    std::string value_str = const_node["value"].scalar;
    
    // Parse value based on type
    Value value;
    if (const_node.has("type")) {
        std::string type = normalize_key(const_node["type"].scalar);
        if (type == "int" || type == "integer") {
            value = Value::from_int(std::stoll(value_str));
        } else if (type == "float" || type == "double") {
            value = Value::from_number(std::stod(value_str));
        } else if (type == "bool" || type == "boolean") {
            value = Value::from_bool(value_str == "true" || value_str == "TRUE" || value_str == "1");
        } else {
            value = Value::from_string(value_str);
        }
    } else {
        // Try to infer type
        if (value_str == "true" || value_str == "TRUE" || value_str == "false" || value_str == "FALSE") {
            value = Value::from_bool(value_str == "true" || value_str == "TRUE");
        } else {
            try {
                value = Value::from_int(std::stoll(value_str));
            } catch (...) {
                try {
                    value = Value::from_number(std::stod(value_str));
                } catch (...) {
                    value = Value::from_string(value_str);
                }
            }
        }
    }
    
    // Register as a function that returns the constant value
    NativeFn const_fn{name, 0, [value](const std::vector<Value>&) -> Value {
        return value;
    }};
    
    registry_.add(name, const_fn);
}

NativeFn YamlModuleLoader::create_native_function(const YamlNode& func_node) {
    std::string name = normalize_key(func_node["name"].scalar);
    int arity = -1;
    
    // Parse arity from args
    if (func_node.has("args")) {
        const auto& args_node = func_node["args"];
        if (args_node.is_sequence()) {
            arity = static_cast<int>(args_node.sequence.size());
        } else if (args_node.is_scalar() && args_node.scalar == "variadic") {
            arity = -1; // Variadic
        }
    }
    
    // Try to bind to native function (explicit 'native' field)
    if (func_node.has("native")) {
        std::string native_name = normalize_key(func_node["native"].scalar);
        if (native_registry_) {
            auto* callback = native_registry_->find_native(native_name);
            if (callback) {
                return NativeFn{name, arity, *callback};
            }
        }
        // In strict mode, missing native bindings are errors
        if (options_.strict_mode) {
            throw std::runtime_error("Native function '" + native_name + "' not found in registry for function '" + name + "'");
        } else {
            // In non-strict mode, create a placeholder that warns at runtime
            return NativeFn{name, arity, [name, native_name](const std::vector<Value>&) -> Value {
                throw std::runtime_error("Function '" + name + "' requires native binding '" + native_name + "' which is not registered");
            }};
        }
    }
    
    // Try to bind via raylib_name/map_to field (for compatibility with code generation specs)
    // If raylib_name is present, try to find the function in the registry by its BASIC name
    // This allows YAML specs with raylib_name to work with runtime loading
    if (func_node.has("raylib_name") || func_node.has("map_to")) {
        // The function should already be registered in FunctionRegistry by code generation
        // Try to find it and use its callback
        const NativeFn* existing = registry_.find(name);
        if (existing) {
            return NativeFn{name, existing->arity, existing->fn};
        }
        // If not found, try to find via raylib_name in native registry (in case it was registered differently)
        std::string raylib_name = func_node.has("raylib_name") ? 
                                 normalize_key(func_node["raylib_name"].scalar) :
                                 normalize_key(func_node["map_to"].scalar);
        if (native_registry_) {
            auto* callback = native_registry_->find_native(raylib_name);
            if (callback) {
                return NativeFn{name, arity, *callback};
            }
            // Also try the BASIC name (normalized)
            callback = native_registry_->find_native(name);
            if (callback) {
                return NativeFn{name, arity, *callback};
            }
        }
    }
    
    // Try to bind to existing function in registry (for aliases)
    if (func_node.has("alias")) {
        std::string alias_name = normalize_key(func_node["alias"].scalar);
        const NativeFn* existing = registry_.find(alias_name);
        if (existing) {
            return NativeFn{name, existing->arity, existing->fn};
        }
        throw std::runtime_error("Alias function '" + alias_name + "' not found for function '" + name + "'");
    }
    
    // No binding found - create placeholder
    return NativeFn{name, arity, [name](const std::vector<Value>&) -> Value {
        throw std::runtime_error("Function '" + name + "' has no native binding. "
                                "Register it in NativeFunctionRegistry or use 'native'/'raylib_name' field in YAML.");
    }};
}

NativeFn YamlModuleLoader::create_constructor(const YamlNode& ctor_node) {
    std::string name = normalize_key(ctor_node["name"].scalar);
    int arity = -1;
    
    // Parse arity from fields
    if (ctor_node.has("fields")) {
        const auto& fields_node = ctor_node["fields"];
        if (fields_node.is_sequence()) {
            arity = static_cast<int>(fields_node.sequence.size());
        }
    }
    
    // Create constructor function
    return NativeFn{name, arity, [this, ctor_node](const std::vector<Value>& args) -> Value {
        return create_constructor_object(ctor_node, args);
    }};
}

Value YamlModuleLoader::create_constructor_object(const YamlNode& ctor_node, const std::vector<Value>& args) {
    std::string type_name = normalize_key(ctor_node["name"].scalar);
    
    Value::Map obj;
    obj[normalize_key("_type")] = Value::from_string(type_name);
    
    // Parse fields and assign values
    if (ctor_node.has("fields")) {
        const auto& fields_node = ctor_node["fields"];
        if (fields_node.is_sequence()) {
            for (size_t i = 0; i < fields_node.sequence.size() && i < args.size(); ++i) {
                const auto& field_node = fields_node.sequence[i];
                if (field_node.is_mapping() && field_node.has("name")) {
                    std::string field_name = normalize_key(field_node["name"].scalar);
                    obj[field_name] = args[i];
                } else if (field_node.is_scalar()) {
                    std::string field_name = normalize_key(field_node.scalar);
                    obj[field_name] = args[i];
                }
            }
            
            // Set default values for remaining fields
            for (size_t i = args.size(); i < fields_node.sequence.size(); ++i) {
                const auto& field_node = fields_node.sequence[i];
                if (field_node.is_mapping()) {
                    std::string field_name = normalize_key(field_node["name"].scalar);
                    Value default_value = Value::nil();
                    
                    if (field_node.has("default")) {
                        const auto& default_node = field_node["default"];
                        if (default_node.is_scalar()) {
                            std::string default_str = default_node.scalar;
                            // Parse default value
                            if (field_node.has("type")) {
                                std::string type = normalize_key(field_node["type"].scalar);
                                if (type == "int" || type == "integer") {
                                    default_value = Value::from_int(std::stoll(default_str));
                                } else if (type == "float" || type == "double") {
                                    default_value = Value::from_number(std::stod(default_str));
                                } else if (type == "bool" || type == "boolean") {
                                    default_value = Value::from_bool(default_str == "true" || default_str == "TRUE");
                                } else {
                                    default_value = Value::from_string(default_str);
                                }
                            } else {
                                default_value = Value::from_string(default_str);
                            }
                        }
                    }
                    
                    obj[field_name] = default_value;
                }
            }
        }
    }
    
    return Value::from_map(std::move(obj));
}

// Schema validation
bool YamlModuleLoader::validate_function_schema(const YamlNode& func_node, std::string& error) const {
    if (!func_node.is_mapping()) {
        error = "Function must be a mapping";
        return false;
    }
    
    if (!func_node.has("name")) {
        error = "Function missing required 'name' field";
        return false;
    }
    
    if (!func_node["name"].is_scalar()) {
        error = "Function 'name' must be a scalar";
        return false;
    }
    
    // Either 'native' or 'alias' should be present for runtime loading
    if (!func_node.has("native") && !func_node.has("alias")) {
        // This is OK - it will create a placeholder
    }
    
    return true;
}

bool YamlModuleLoader::validate_constructor_schema(const YamlNode& ctor_node, std::string& error) const {
    if (!ctor_node.is_mapping()) {
        error = "Constructor must be a mapping";
        return false;
    }
    
    if (!ctor_node.has("name")) {
        error = "Constructor missing required 'name' field";
        return false;
    }
    
    if (!ctor_node["name"].is_scalar()) {
        error = "Constructor 'name' must be a scalar";
        return false;
    }
    
    return true;
}

bool YamlModuleLoader::validate_namespace_schema(const YamlNode& ns_node, std::string& error) const {
    if (!ns_node.is_mapping()) {
        error = "Namespace must be a mapping";
        return false;
    }
    
    if (!ns_node.has("name")) {
        error = "Namespace missing required 'name' field";
        return false;
    }
    
    if (!ns_node["name"].is_scalar()) {
        error = "Namespace 'name' must be a scalar";
        return false;
    }
    
    return true;
}

bool YamlModuleLoader::validate_type_schema(const YamlNode& type_node, std::string& error) const {
    if (!type_node.is_mapping()) {
        error = "Type must be a mapping";
        return false;
    }
    
    if (!type_node.has("name")) {
        error = "Type missing required 'name' field";
        return false;
    }
    
    if (!type_node["name"].is_scalar()) {
        error = "Type 'name' must be a scalar";
        return false;
    }
    
    return true;
}

// Helper to populate native registry from function registry
void populate_native_registry_from_function_registry(
    NativeFunctionRegistry& native_registry,
    FunctionRegistry& function_registry) {
    // This is a placeholder - in a full implementation, you would iterate
    // through the FunctionRegistry and register each function in the native registry.
    // For now, YAML modules can use the "alias" field to reference existing functions.
    
    // Note: FunctionRegistry doesn't expose iteration, so we can't automatically
    // populate. Users should explicitly register functions they want to expose
    // to YAML modules, or use the "alias" mechanism in YAML.
}

// Module metadata access
YamlModuleLoader::ModuleMetadata YamlModuleLoader::get_module_metadata(const std::string& module_name) const {
    std::string normalized = normalize_key(module_name);
    auto it = module_metadata_.find(normalized);
    if (it != module_metadata_.end()) {
        return it->second;
    }
    return ModuleMetadata{normalized, "", "", "", {}};
}

std::vector<YamlModuleLoader::ModuleMetadata> YamlModuleLoader::get_all_module_metadata() const {
    std::vector<ModuleMetadata> result;
    result.reserve(module_metadata_.size());
    for (const auto& [name, metadata] : module_metadata_) {
        result.push_back(metadata);
    }
    return result;
}

// Module validation without loading
bool YamlModuleLoader::validate_module_file(const std::filesystem::path& filepath, std::string* error_out) const {
    if (!std::filesystem::exists(filepath)) {
        if (error_out) *error_out = "File does not exist: " + filepath.string();
        return false;
    }
    
    try {
        YamlNode module = const_cast<YamlModuleLoader*>(this)->parse_yaml_file(filepath);
        if (module.is_null()) {
            if (error_out) *error_out = "Failed to parse YAML file";
            return false;
        }
        
        // Validate required fields
        if (!module.has("module") && !module.has("functions") && !module.has("constructors") && 
            !module.has("namespaces") && !module.has("types")) {
            if (error_out) *error_out = "Module has no content (functions, constructors, namespaces, or types)";
            return false;
        }
        
        // Validate functions if present
        if (module.has("functions")) {
            const auto& functions = module["functions"];
            if (functions.is_sequence()) {
                for (const auto& func_node : functions.sequence) {
                    if (func_node.is_mapping()) {
                        std::string error;
                        if (!validate_function_schema(func_node, error)) {
                            if (error_out) *error_out = "Function validation error: " + error;
                            return false;
                        }
                    }
                }
            }
        }
        
        return true;
    } catch (const std::exception& e) {
        if (error_out) *error_out = std::string("Validation error: ") + e.what();
        return false;
    }
}

} // namespace bas


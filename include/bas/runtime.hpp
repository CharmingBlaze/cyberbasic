#pragma once
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include <cctype>
#include "value.hpp"
#include "ast.hpp"

namespace bas {

// Native function entry for the runtime registry.
struct NativeFn {
  std::string name; 
  int arity; 
  std::function<Value(const std::vector<Value>&)> fn;
  
  [[nodiscard]] Value operator()(const std::vector<Value>& args) const { 
    return fn(args); 
  }
};

// Registry for native functions callable from BASIC.
class FunctionRegistry {
public:
  void add(const std::string& name, const NativeFn& fn){
    std::string upper_name;
    upper_name.reserve(name.size());
    for (char c : name) upper_name += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    // Duplicate detection: do not allow silent overwrite of existing functions
    if (fns.find(upper_name) != fns.end()) {
      throw std::runtime_error(std::string("Duplicate native function registration: ") + upper_name);
    }
    fns[upper_name] = fn;
  }
  
  // Add with collision policy for generated bindings
  void add_with_policy(const std::string& name, const NativeFn& fn, bool allow_override = false){
    std::string upper_name;
    upper_name.reserve(name.size());
    for (char c : name) upper_name += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    if (fns.find(upper_name) != fns.end()) {
      if (!allow_override) {
        throw std::runtime_error(std::string("Duplicate native function registration: ") + upper_name);
      }
      // Log override in debug builds
      #ifdef DEBUG
      std::cerr << "Warning: Overriding function " << upper_name << std::endl;
      #endif
    }
    fns[upper_name] = fn;
  }
  
  [[nodiscard]] const NativeFn* find(const std::string& name) const {
    std::string upper_name;
    upper_name.reserve(name.size());
    for (char c : name) upper_name += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    auto it=fns.find(upper_name); 
    return it==fns.end() ? nullptr : &it->second;
  }
  
  [[nodiscard]] size_t size() const noexcept { return fns.size(); }
private:
  std::unordered_map<std::string, NativeFn> fns;
};

void register_builtins(FunctionRegistry&);
void register_object_constructors(FunctionRegistry&);
void register_raylib_bindings(FunctionRegistry&);

// Member access hooks allow subsystems (e.g., ECS) to resolve custom dot-notation.
using MemberReadHook = std::function<std::optional<Value>(const Value&, const std::string&)>;
using MemberWriteHook = std::function<bool(const Value&, const std::string&, const Value&)>;

void register_member_read_hook(MemberReadHook hook);
void register_member_write_hook(MemberWriteHook hook);
std::optional<Value> try_resolve_member(const Value& object, const std::string& member);
bool try_assign_member(const Value& object, const std::string& member, const Value& value);

// Enhanced Game Development Systems
void register_level_editor_functions(FunctionRegistry&);
void register_asset_pipeline_functions(FunctionRegistry&);
void register_sprite_animation_functions(FunctionRegistry&);
void register_gui_functions(FunctionRegistry&);
void register_enums_and_dicts(FunctionRegistry&);
void register_dot_notation_enhancements(FunctionRegistry&);
void register_advanced_features(FunctionRegistry&);

// Call a native function by name with arguments.
[[nodiscard]] Value call(FunctionRegistry&, const std::string& name, const std::vector<Value>&);

// Interpret a parsed program. Returns 0 on success, non-zero on runtime error.
[[nodiscard]] int interpret(const Program&, FunctionRegistry&, bool debug_mode);

// Set the global namespace registry for the interpreter
class NamespaceRegistry;
void set_namespace_registry(NamespaceRegistry* registry);

} // namespace bas

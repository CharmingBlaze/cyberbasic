#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <cctype>
#include "value.hpp"
#include "ast.hpp"

namespace bas {

// Native function entry for the runtime registry.
struct NativeFn {
  std::string name; int arity; std::function<Value(const std::vector<Value>&)> fn;
  Value operator()(const std::vector<Value>& args) const { return fn(args); }
};

// Registry for native functions callable from BASIC.
class FunctionRegistry {
public:
  void add(const std::string& name, const NativeFn& fn){
    std::string upper_name;
    for (char c : name) upper_name += toupper(c);
    // Duplicate detection: do not allow silent overwrite of existing functions
    if (fns.find(upper_name) != fns.end()) {
      throw std::runtime_error(std::string("Duplicate native function registration: ") + upper_name);
    }
    fns[upper_name] = fn;
  }
  
  // Add with collision policy for generated bindings
  void add_with_policy(const std::string& name, const NativeFn& fn, bool allow_override = false){
    std::string upper_name;
    for (char c : name) upper_name += toupper(c);
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
  const NativeFn* find(const std::string& name) const {
    std::string upper_name;
    for (char c : name) upper_name += toupper(c);
    auto it=fns.find(upper_name); return it==fns.end()?nullptr:&it->second;
  }
  
  size_t size() const { return fns.size(); }
private:
  std::unordered_map<std::string, NativeFn> fns;
};

void register_builtins(FunctionRegistry&);
void register_raylib_bindings(FunctionRegistry&);

// Enhanced Game Development Systems
void register_level_editor_functions(FunctionRegistry&);
void register_asset_pipeline_functions(FunctionRegistry&);
void register_sprite_animation_functions(FunctionRegistry&);
void register_gui_functions(FunctionRegistry&);

// Call a native function by name with arguments.
Value call(FunctionRegistry&, const std::string& name, const std::vector<Value>&);

// Interpret a parsed program. Returns 0 on success, non-zero on runtime error.
int interpret(const Program&, FunctionRegistry&);

} // namespace bas

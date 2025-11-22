#include "bas/ast.hpp"
#include "bas/runtime.hpp"
#include "bas/namespace_registry.hpp"
#include "bas/type_system.hpp"
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <stdexcept>
#include <cmath>
#include <cctype>
#include <memory>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <locale>
#include <cstdlib>

using namespace bas;

// Global namespace registry (initialized by runtime)
static NamespaceRegistry* g_namespace_registry = nullptr;

// Helper function to convert any Value to string
static std::string value_to_string_safe(const Value& v){
  if(auto s = std::get_if<std::string>(&v.v)) return *s;
  if(auto b = std::get_if<bool>(&v.v)) return *b?"TRUE":"FALSE";
  if(auto d = std::get_if<double>(&v.v)) { std::ostringstream os; os.imbue(std::locale::classic()); os<<*d; return os.str(); }
  if(auto i = std::get_if<long long>(&v.v)) { std::ostringstream os; os.imbue(std::locale::classic()); os<<*i; return os.str(); }
  return std::string{};
}

namespace {
// Exception classes for control flow
struct ReturnSignal { Value v; };
struct BreakSignal {};
struct ContinueSignal {};
struct ExitSignal { std::string target; }; // "FOR", "WHILE", "SUB", "FUNCTION"

// Env struct must be defined before helper functions that use Env::up()
struct Env {
  std::unordered_map<std::string, Value> vars;
  std::unordered_set<std::string> consts;
  std::unordered_set<std::string> declared;
  std::unordered_set<std::string> globals_here; // names in this scope that should bind to root env
  bool strict{false};
  const Env* parent{nullptr};
  [[nodiscard]] static std::string up(const std::string& s){ 
    std::string r; 
    r.reserve(s.size()); 
    for(char c:s) r.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c)))); 
    return r; 
  }
  bool is_declared_here(const std::string& ukey) const { return declared.find(ukey) != declared.end(); }
  bool is_declared(const std::string& n) const {
    auto key = up(n);
    if(is_declared_here(key)) return true;
    return parent ? parent->is_declared(n) : false;
  }
  void declare(const std::string& n){ declared.insert(up(n)); }
  bool is_global_here(const std::string& ukey) const { return globals_here.find(ukey) != globals_here.end(); }
  Env* root(){ Env* r = this; while(r->parent) r = const_cast<Env*>(r->parent); return r; }
  const Env* root() const { auto* r = this; while(r->parent) r = r->parent; return r; }
  Value get(const std::string& n) const {
    auto key = up(n);
    // If marked GLOBAL in this scope, read from root env
    if(is_global_here(key)){
      const Env* r = root();
      auto it = r->vars.find(key);
      if(it!=r->vars.end()) return it->second;
      if(strict && !r->is_declared(n)){
        throw std::runtime_error("Use of undeclared variable '" + key + "'");
      }
      return Value::nil();
    }
    auto it = vars.find(key);
    if(it!=vars.end()) return it->second;
    if(parent) return parent->get(n);
    if(strict && !is_declared(n)){
      throw std::runtime_error("Use of undeclared variable '" + key + "'");
    }
    return Value::nil();
  }
  bool is_const_here(const std::string& ukey) const { return consts.find(ukey) != consts.end(); }
  bool is_const(const std::string& n) const {
    auto key = up(n);
    if(is_const_here(key)) return true;
    return parent ? parent->is_const(n) : false;
  }
  void define_const(const std::string& n, Value v){
    auto key = up(n);
    // Defining a const shades any parent value; local const wins
    vars[key] = std::move(v);
    consts.insert(key);
    declare(n);
  }
  void set(const std::string& n, Value v){
    auto key = up(n);
    if(is_global_here(key)){
      // Route assignment to root env but enforce const/strict using current scope's rules
      Env* r = root();
      if(r->is_const(n)){
        throw std::runtime_error("Assignment to constant '" + key + "'");
      }
      if(strict && !r->is_declared(n)){
        throw std::runtime_error("Assignment to undeclared variable '" + key + "'");
      }
      r->vars[key] = std::move(v);
      return;
    }
    if(is_const(key)){
      throw std::runtime_error("Assignment to constant '" + key + "'");
    }
    if(strict && !is_declared(n)){
      throw std::runtime_error("Assignment to undeclared variable '" + key + "'");
    }
    vars[key]=std::move(v);
  }
};

// Helper function for deep property access
std::optional<Value> get_deep_property(const Value& obj, const std::string& path) {
    if (!obj.is_map()) return std::nullopt;
    const auto& map = obj.as_map();
    
    // Split path by dots for nested access
    size_t dotPos = path.find('.');
    if (dotPos == std::string::npos) {
        // Simple property access
        std::string key = Env::up(path);
        auto it = map.find(key);
        if (it != map.end()) {
            return it->second;
        }
        // Case-insensitive search
        for (const auto& pair : map) {
            if (Env::up(pair.first) == key) {
                return pair.second;
            }
        }
        return std::nullopt;
    }
    
    // Nested access: get first part, then recurse
    std::string first = path.substr(0, dotPos);
    std::string rest = path.substr(dotPos + 1);
    std::string key = Env::up(first);
    auto it = map.find(key);
    if (it == map.end()) {
        // Case-insensitive search
        for (const auto& pair : map) {
            if (Env::up(pair.first) == key) {
                return get_deep_property(pair.second, rest);
            }
        }
        return std::nullopt;
    }
    return get_deep_property(it->second, rest);
}

// Helper function for deep property assignment
bool set_deep_property(Value& obj, const std::string& path, const Value& val) {
    if (!obj.is_map()) return false;
    auto& map = obj.as_map();
    
    // Split path by dots for nested access
    size_t dotPos = path.find('.');
    if (dotPos == std::string::npos) {
        // Simple property assignment
        map[Env::up(path)] = val;
        return true;
    }
    
    // Nested assignment: get/create first part, then recurse
    std::string first = path.substr(0, dotPos);
    std::string rest = path.substr(dotPos + 1);
    std::string key = Env::up(first);
    
    auto it = map.find(key);
    if (it == map.end() || !it->second.is_map()) {
        // Create nested map
        Value::Map nested;
        map[key] = Value::from_map(std::move(nested));
        it = map.find(key);
    }
    
    return set_deep_property(it->second, rest, val);
}

// Global storage for subroutines and functions
static std::unordered_map<std::string, const SubDecl*> g_subs;
static std::unordered_map<std::string, const FunctionDecl*> g_funcs;

// Forward declarations
static Value eval(Env& env, FunctionRegistry& R, const Expr* e, bool debug_mode);
static void exec(Env& env, FunctionRegistry& R, const Stmt* s, bool debug_mode);


static double to_num(const Value& v) {
    return v.as_number();
}
static void call_sub(Env& caller, FunctionRegistry& R, const std::string& name, const std::vector<Value>& args, bool debug_mode);
static Value call_func(Env& caller, FunctionRegistry& R, const std::string& name, 
                       const std::vector<Value>& args, 
                       const std::map<std::string, Value>& namedArgs,
                       bool debug_mode);

// Helper functions
static bool truthy(const Value& v) {
    if (auto b = std::get_if<bool>(&v.v)) return *b;
    if (auto d = std::get_if<double>(&v.v)) return *d != 0.0;
    if (auto i = std::get_if<long long>(&v.v)) return *i != 0;
    if (auto s = std::get_if<std::string>(&v.v)) return !s->empty();
    return false;
}

static int cmp_values(const Value& a, const Value& b) {
    // Try to compare as numbers first
    try {
        double da = a.as_number();
        double db = b.as_number();
        if (da < db) return -1;
        if (da > db) return 1;
        return 0;
    } catch (...) {
        // If not numbers, compare as strings
        try {
            const std::string& sa = a.as_string();
            const std::string& sb = b.as_string();
            if (sa < sb) return -1;
            if (sa > sb) return 1;
            return 0;
        } catch (...) {
            return 0; // Equal if can't compare
        }
    }
}
static Value make_array_from_sizes(Env& env, FunctionRegistry& R, const std::vector<std::unique_ptr<Expr>>& sizes, size_t depth, bool debug_mode);

// Evaluate size expressions to a concrete vector<size_t>
static std::vector<size_t> eval_sizes_to_vec(Env& env, FunctionRegistry& R,
                                            const std::vector<std::unique_ptr<Expr>>& sizes, bool debug_mode){
  std::vector<size_t> out; out.reserve(sizes.size());
  for(auto& e : sizes){
    long long n = eval(env, R, e.get(), debug_mode).as_int();
    if(n < 0) n = 0;
    out.push_back(static_cast<size_t>(n));
  }
  return out;
}

// Build an array initialized with nils using concrete size vector
static Value make_array_from_lenvec(const std::vector<size_t>& lens, size_t depth){
  size_t n = depth < lens.size() ? lens[depth] : 0;
  Value::Array arr; arr.resize(n, Value::nil());
  if(depth + 1 < lens.size()){
    for(size_t i = 0; i < n; ++i){
      arr[i] = make_array_from_lenvec(lens, depth + 1);
    }
  }
  return Value::from_array(std::move(arr));
}

// Copy overlapping elements from src into a newly allocated array with shape `lens`.
// For deeper dimensions, recurse; values are copied by Value (which is variant, so copy is fine).
static Value redim_preserve_copy(const Value& src, const std::vector<size_t>& lens, size_t depth){
  Value dest = make_array_from_lenvec(lens, depth);
  if(!src.is_array()) return dest;
  const auto& sarr = src.as_array();
  auto& darr = dest.as_array();
  const size_t ncopy = std::min(darr.size(), sarr.size());
  if(depth + 1 < lens.size()){
    for(size_t i = 0; i < ncopy; ++i){
      darr[i] = redim_preserve_copy(sarr[i], lens, depth + 1);
    }
  } else {
    for(size_t i = 0; i < ncopy; ++i){
      darr[i] = sarr[i];
    }
  }
  return dest;
}

// Compute total number of elements for a shape, with overflow/limit guard
static bool safe_total_elems(const std::vector<size_t>& lens, size_t& outTotal, size_t cap = 50'000'000){
  // cap default ~50M cells to avoid runaway allocations
  size_t prod = 1;
  for(size_t d : lens){
    if (d > 0 && prod > cap / d) { 
      outTotal = cap; 
      return false; 
    }
    prod *= d;
  }
  outTotal = prod;
  return true;
}

// Allocate a nested array based on a vector of size expressions.
// sizes: list of dimension sizes expressions, evaluated at each depth.
// depth: current dimension index.
static Value make_array_from_sizes(Env& env, FunctionRegistry& R,
                                   const std::vector<std::unique_ptr<Expr>>& sizes,
                                   size_t depth, bool debug_mode){
  long long nll = 0;
  if(depth < sizes.size()){
    nll = eval(env, R, sizes[depth].get(), debug_mode).as_int();
  }
  if(nll < 0) nll = 0;
  size_t n = static_cast<size_t>(nll);
  Value::Array arr;
  arr.resize(n, Value::nil());
  if(depth + 1 < sizes.size()){
    for(size_t i = 0; i < arr.size(); ++i){
      arr[i] = make_array_from_sizes(env, R, sizes, depth + 1, debug_mode);
    }
  }
  return Value::from_array(std::move(arr));
}

static Value eval(Env& env, FunctionRegistry& R, const Expr* e, bool debug_mode){
    (void)env; (void)R; // Suppress unused parameter warnings
  if (debug_mode) std::cerr << "eval: " << typeid(*e).name() << std::endl;
  if(auto lit = dynamic_cast<const Literal*>(e)){
    switch(lit->tok.kind){
      case Tok::Number: return Value::from_number(std::stod(lit->tok.lex));
      case Tok::String: return Value::from_string(lit->tok.lex);
      case Tok::True: return Value::from_bool(true);
      case Tok::False: return Value::from_bool(false);
      default: return Value::nil();
    }
  }
  if(auto i = dynamic_cast<const Variable*>(e)) {
    return env.get(i->name);
  }
  if(auto u = dynamic_cast<const Unary*>(e)){
    Value r = eval(env, R, u->right.get(), debug_mode);
    if(u->op==Tok::Minus) return Value::from_number(-to_num(r));
    if(u->op==Tok::Plus) return Value::from_number(+to_num(r));
    if(u->op==Tok::Not) return Value::from_bool(!truthy(r));
    throw std::runtime_error("invalid unary op");
  }
  if(auto b = dynamic_cast<const Binary*>(e)){
    Value L = eval(env, R, b->left.get(), debug_mode);
    Value right_val = eval(env, R, b->right.get(), debug_mode);
        if (debug_mode) std::cerr << "binary op L: " << value_to_string_safe(L) << ", R: " << value_to_string_safe(right_val) << std::endl;

    switch(b->op){
      case Tok::Plus: {
        if (std::holds_alternative<std::string>(L.v) || std::holds_alternative<std::string>(right_val.v)) {
          return Value::from_string(value_to_string_safe(L) + value_to_string_safe(right_val));
        } else {
          return Value::from_number(to_num(L) + to_num(right_val));
        }
      }
      case Tok::Minus: return Value::from_number(to_num(L) - to_num(right_val));
      case Tok::Star: return Value::from_number(to_num(L) * to_num(right_val));
      case Tok::Slash: return Value::from_number(to_num(L) / to_num(right_val));
      case Tok::IntDiv: {
        long long lhs = static_cast<long long>(to_num(L));
        long long rhs = static_cast<long long>(to_num(right_val));
        if(rhs == 0) throw std::runtime_error("Integer division by zero");
        return Value::from_int(lhs / rhs);
      }
      case Tok::Power: return Value::from_number(std::pow(to_num(L), to_num(right_val)));
      case Tok::Mod: {
        double rhs = to_num(right_val);
        if(rhs == 0.0) throw std::runtime_error("Modulo by zero");
        return Value::from_number(std::fmod(to_num(L), rhs));
      }
      case Tok::Eq:  return Value::from_bool(cmp_values(L,right_val) == 0);
      case Tok::Neq: return Value::from_bool(cmp_values(L,right_val) != 0);
      case Tok::Lt:  return Value::from_bool(cmp_values(L,right_val) <  0);
      case Tok::Lte: return Value::from_bool(cmp_values(L,right_val) <= 0);
      case Tok::Gt:  return Value::from_bool(cmp_values(L,right_val) >  0);
      case Tok::Gte: return Value::from_bool(cmp_values(L,right_val) >= 0);
      case Tok::And: return Value::from_bool(truthy(L) && truthy(right_val));
      case Tok::Xor: return Value::from_bool(truthy(L) != truthy(right_val));
      case Tok::Or:  return Value::from_bool(truthy(L) || truthy(right_val));
      default: break;
    }
    throw std::runtime_error("invalid binary op");
  }
  if(auto arr = dynamic_cast<const ArrayLiteral*>(e)){
    Value::Array a; a.reserve(arr->elements.size());
    for(auto& el : arr->elements) a.push_back(eval(env,R,el.get(), debug_mode));
    return Value::from_array(std::move(a));
  }
  if(auto idx = dynamic_cast<const Index*>(e)){
    Value base = eval(env,R,idx->target.get(), debug_mode);
    Value indexVal = eval(env,R,idx->index.get(), debug_mode);
    
    // Array indexing
    if(base.is_array()){
      long long i = indexVal.as_int();
      auto const& arrRef = base.as_array();
      if(i < 0 || (size_t)i >= arrRef.size()) return Value::nil();
      return arrRef[(size_t)i];
    }
    
    // String indexing (character access)
    if(base.is_string()){
      long long i = indexVal.as_int();
      const auto& str = base.as_string();
      if(i < 0 || (size_t)i >= str.size()) return Value::nil();
      return Value::from_string(std::string(1, str[(size_t)i]));
    }
    
    // Map indexing (key access)
    if(base.is_map()){
      const auto& map = base.as_map();
      std::string key;
      if(indexVal.is_string()){
        key = Env::up(indexVal.as_string());
      } else {
        key = Env::up(indexVal.as_string()); // Convert to string
      }
      auto it = map.find(key);
      if(it != map.end()) return it->second;
      // Case-insensitive search
      for(const auto& pair : map){
        if(Env::up(pair.first) == key) return pair.second;
      }
      return Value::nil();
    }
    
    return Value::nil();
  }
  if(auto c = dynamic_cast<const Call*>(e)){
    std::vector<Value> args; args.reserve(c->args.size());
    for(auto& a : c->args) args.push_back(eval(env, R, a.get(), debug_mode));
    
    // Evaluate named arguments
    std::map<std::string, Value> namedArgs;
    for(const auto& na : c->namedArgs){
      Value val = eval(env, R, na.value.get(), debug_mode);
      namedArgs[Env::up(na.name)] = val;
    }
    
    // Check user-defined subs first
    auto it = g_subs.find(Env::up(c->callee));
    if(it!=g_subs.end()){
      call_sub(env, R, c->callee, args, debug_mode);
      return Value::nil();
    }
    auto itf = g_funcs.find(Env::up(c->callee));
    if(itf!=g_funcs.end()){
      return call_func(env, R, c->callee, args, namedArgs, debug_mode);
    }
    return call(R, c->callee, args);
  }
  if(auto ma = dynamic_cast<const MemberAccess*>(e)){
    Value obj = eval(env, R, ma->object.get(), debug_mode);
    
    // Try hooks first (for ECS, etc.)
    if (auto hookValue = try_resolve_member(obj, ma->member); hookValue.has_value()) {
      return *hookValue;
    }
    
    // Check if object is a map (object)
    if(obj.is_map()){
      const auto& map = obj.as_map();
      std::string member_upper = Env::up(ma->member);
      
      // Check property descriptors first (for computed properties/getters)
      std::string objId;
      auto idIt = map.find("_id");
      if (idIt != map.end() && idIt->second.is_int()) {
        objId = std::to_string(idIt->second.as_int());
        // In a real implementation, would check g_property_descriptors here for getters
      }
      
      // Try direct access first
      auto it = map.find(member_upper);
      if(it != map.end()){
        Value propValue = it->second;
        // If property is a function, return it as a method
        if (propValue.is_map()) {
          const auto& propMap = propValue.as_map();
          auto typeIt = propMap.find("_type");
          if (typeIt != propMap.end() && typeIt->second.is_string() && 
              typeIt->second.as_string() == "Method") {
            return propValue;
          }
        }
        return propValue;
      }
      
      // Try case-insensitive search
      for (const auto& pair : map) {
        std::string keyUpper;
        keyUpper.reserve(pair.first.size());
        for (char c : pair.first) {
          keyUpper.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
        }
        if (keyUpper == member_upper) {
          return pair.second;
        }
      }
      
      // Try deep property access (nested maps)
      if (auto deep = get_deep_property(obj, ma->member)) {
        return *deep;
      }
      
      // Check if this is a namespace object trying to access a method
      if(g_namespace_registry){
        auto type_it = map.find("_type");
        if(type_it != map.end() && type_it->second.is_string() && 
           type_it->second.as_string() == "Namespace"){
          auto name_it = map.find("_name");
          if(name_it != map.end() && name_it->second.is_string()){
            std::string ns_name = name_it->second.as_string();
            std::string method_name = member_upper;
            std::string func_name = g_namespace_registry->resolve_method(ns_name, method_name);
            if(!func_name.empty()){
              // Return a method object that can be called
              Value::Map method_obj;
              method_obj["_type"] = Value::from_string("Method");
              method_obj["_namespace"] = Value::from_string(ns_name);
              method_obj["_method"] = Value::from_string(method_name);
              method_obj["_function"] = Value::from_string(func_name);
              return Value::from_map(std::move(method_obj));
            }
          }
        }
      }
      
      // Check if this is an object type (Vector3, Color, etc.) trying to access a method
      auto type_it = map.find("_type");
      if(type_it != map.end() && type_it->second.is_string()){
        std::string obj_type = type_it->second.as_string();
        // Try to resolve as object method: TYPE_METHODNAME
        std::string method_func = obj_type + "_" + member_upper;
        const auto* fn = R.find(method_func);
        if(fn){
          // Return a method object
          Value::Map method_obj;
          method_obj["_type"] = Value::from_string("Method");
          method_obj["_object"] = obj; // Store reference to object
          method_obj["_method"] = Value::from_string(member_upper);
          method_obj["_function"] = Value::from_string(method_func);
          return Value::from_map(std::move(method_obj));
        }
      }
      
      // Property not found - provide helpful error message with suggestions
      if(debug_mode){
        std::string obj_type = "object";
        auto type_it = map.find("_type");
        if(type_it != map.end() && type_it->second.is_string()){
          obj_type = type_it->second.as_string();
        }
        
        // Get available properties for suggestions
        Value::Array availableProps;
        for (const auto& pair : map) {
          if (pair.first[0] != '_' || pair.first == "_type") {
            availableProps.push_back(Value::from_string(pair.first));
          }
        }
        
        // Find similar property names (fuzzy match with better algorithm)
        Value::Array suggestions;
        std::string memberUpper = Env::up(ma->member);
        for (const auto& prop : availableProps) {
          if (prop.is_string()) {
            std::string propUpper = Env::up(prop.as_string());
            // Check if member is similar to property
            bool similar = false;
            
            // Exact prefix match
            if (propUpper.find(memberUpper) == 0 || memberUpper.find(propUpper) == 0) {
              similar = true;
            }
            // Contains match
            else if (propUpper.find(memberUpper) != std::string::npos || 
                     memberUpper.find(propUpper) != std::string::npos) {
              similar = true;
            }
            // Character similarity (simple Levenshtein-like)
            else {
              size_t matches = 0;
              size_t minLen = std::min(memberUpper.size(), propUpper.size());
              for (size_t i = 0; i < minLen; ++i) {
                if (memberUpper[i] == propUpper[i]) matches++;
              }
              if (matches >= minLen / 2) {
                similar = true;
              }
            }
            
            if (similar) {
              suggestions.push_back(prop);
              if (suggestions.size() >= 5) break;
            }
          }
        }
        
        std::cerr << "Warning: Property '" << ma->member << "' not found on " << obj_type;
        if (!suggestions.empty()) {
          std::cerr << "\n  Did you mean: ";
          for (size_t i = 0; i < suggestions.size() && i < 5; ++i) {
            if (i > 0) std::cerr << ", ";
            std::cerr << suggestions[i].as_string();
          }
        }
        std::cerr << std::endl;
      }
      return Value::nil();
    }
    
    // Check if object is a string trying to access a method
    if(obj.is_string()){
      std::string method_func = "STRING_" + Env::up(ma->member);
      const auto* fn = R.find(method_func);
      if(fn){
        Value::Map method_obj;
        method_obj["_type"] = Value::from_string("Method");
        method_obj["_object"] = obj;
        method_obj["_method"] = Value::from_string(Env::up(ma->member));
        method_obj["_function"] = Value::from_string(method_func);
        return Value::from_map(std::move(method_obj));
      }
    }
    
    // Check if object is an array trying to access a method or property
    if(obj.is_array()){
      // Try array methods first
      std::string method_func = "ARRAY_" + Env::up(ma->member);
      const auto* fn = R.find(method_func);
      if(fn){
        Value::Map method_obj;
        method_obj["_type"] = Value::from_string("Method");
        method_obj["_object"] = obj;
        method_obj["_method"] = Value::from_string(Env::up(ma->member));
        method_obj["_function"] = Value::from_string(method_func);
        return Value::from_map(std::move(method_obj));
      }
      // Try array properties (length, etc.)
      if (ma->member == "length" || ma->member == "LENGTH" || ma->member == "size" || ma->member == "SIZE") {
        return Value::from_int(static_cast<long long>(obj.as_array().size()));
      }
    }
    
    // Check if object is a string trying to access a method or property
    if(obj.is_string()){
      // Try string methods first
      std::string method_func = "STRING_" + Env::up(ma->member);
      const auto* fn = R.find(method_func);
      if(fn){
        Value::Map method_obj;
        method_obj["_type"] = Value::from_string("Method");
        method_obj["_object"] = obj;
        method_obj["_method"] = Value::from_string(Env::up(ma->member));
        method_obj["_function"] = Value::from_string(method_func);
        return Value::from_map(std::move(method_obj));
      }
      // Try string properties (length, etc.)
      if (ma->member == "length" || ma->member == "LENGTH" || ma->member == "size" || ma->member == "SIZE") {
        return Value::from_int(static_cast<long long>(obj.as_string().size()));
      }
    }
    
    // If object is a variable name, try to resolve as namespace
    if(auto var = dynamic_cast<const Variable*>(ma->object.get())){
      if(g_namespace_registry && g_namespace_registry->has_namespace(var->name)){
        // Create namespace object on-the-fly and access member
        Value ns_obj = g_namespace_registry->create_namespace_object(var->name);
        const auto& ns_map = ns_obj.as_map();
        auto member_it = ns_map.find(Env::up(ma->member));
        if(member_it != ns_map.end()){
          return member_it->second;
        }
      }
    }
    
    return Value::nil();
  }
  if(auto mc = dynamic_cast<const MethodCall*>(e)){
    Value obj = eval(env, R, mc->object.get(), debug_mode);
    std::vector<Value> args; args.reserve(mc->args.size());
    for(auto& a : mc->args) args.push_back(eval(env, R, a.get(), debug_mode));
    
    std::string func_name;
    
    // Check if object is a namespace or method object
    if(obj.is_map()){
      const auto& map = obj.as_map();
      auto type_it = map.find("_type");
      
      if(type_it != map.end() && type_it->second.is_string()){
        std::string obj_type = type_it->second.as_string();
        
        if(obj_type == "Lambda"){
          // Lambda function call - in a real implementation, this would execute the lambda body
          // For now, return nil as placeholder
          // TODO: Implement full lambda execution with closure
          return Value::nil();
        }
        else if(obj_type == "Method"){
          // Direct method call - get function name from method object
          auto func_it = map.find("_function");
          if(func_it != map.end() && func_it->second.is_string()){
            func_name = func_it->second.as_string();
            // If method has object reference, prepend it to args
            auto obj_it = map.find("_object");
            if(obj_it != map.end()){
              std::vector<Value> new_args;
              new_args.push_back(obj_it->second);
              new_args.insert(new_args.end(), args.begin(), args.end());
              args = std::move(new_args);
            }
          }
        } else if(obj_type == "Namespace" && g_namespace_registry){
          // Namespace.method() - resolve method
          auto name_it = map.find("_name");
          if(name_it != map.end() && name_it->second.is_string()){
            std::string ns_name = name_it->second.as_string();
            func_name = g_namespace_registry->resolve_method(ns_name, mc->method);
          }
        }
      }
    }
    
    // Store original object for method chaining
    Value originalObj = obj;
    bool shouldChain = false;
    
    // Check if method should return self for chaining
    // First check method name from MethodCall
    std::string methodNameUpper = Env::up(mc->method);
    static const std::vector<std::string> chainablePrefixes = {
      "SET", "ADD", "REMOVE", "CLEAR", "UPDATE", "MOVE", "SCALE", 
      "ROTATE", "NORMALIZE", "TRANSFORM", "PUSH", "POP", "SHIFT", "UNSHIFT",
      "APPEND", "INSERT", "DELETE", "MODIFY", "CHANGE", "PREPEND"
    };
    for (const auto& prefix : chainablePrefixes) {
      if (methodNameUpper.find(prefix) == 0) {
        shouldChain = true;
        break;
      }
    }
    
    // Also check if object is a method object with chainable method
    if (!shouldChain && obj.is_map()) {
      const auto& objMap = obj.as_map();
      auto methodIt = objMap.find("_method");
      if (methodIt != objMap.end() && methodIt->second.is_string()) {
        std::string methodName = Env::up(methodIt->second.as_string());
        for (const auto& prefix : chainablePrefixes) {
          if (methodName.find(prefix) == 0) {
            shouldChain = true;
            // Get the original object from the method object
            auto objIt = objMap.find("_object");
            if (objIt != objMap.end()) {
              originalObj = objIt->second;
            }
            break;
          }
        }
      }
    }
    
    // If we have a function name, call it
    if(!func_name.empty()){
      Value result;
      // Try user-defined subs first
      auto it = g_subs.find(Env::up(func_name));
      if(it != g_subs.end()){
        call_sub(env, R, func_name, args, debug_mode);
        result = Value::nil();
      }
      // Try user-defined functions
      else {
        auto itf = g_funcs.find(Env::up(func_name));
        if(itf != g_funcs.end()){
          result = call_func(env, R, func_name, args, {}, debug_mode);
        } else {
          // Try native function
          result = call(R, func_name, args);
        }
      }
      
      // Enhanced method chaining: if method is chainable and result is nil, return original object
      if (shouldChain && result.is_nil()) {
        return originalObj;
      }
      return result;
    }
    
    // Fallback: try method name directly (for global methods)
    std::string method_name = Env::up(mc->method);
    Value result;
    auto it = g_subs.find(method_name);
    if(it != g_subs.end()){
      call_sub(env, R, method_name, args, debug_mode);
      result = Value::nil();
    } else {
      auto itf = g_funcs.find(method_name);
      if(itf != g_funcs.end()){
        result = call_func(env, R, method_name, args, {}, debug_mode);
      } else {
        result = call(R, method_name, args);
      }
    }
    
    // For chainable methods, return original object if result is nil
    if (shouldChain && result.is_nil()) {
      return originalObj;
    }
    return result;
  }
  // Extension expression types
  if (auto lambda = dynamic_cast<const LambdaExpr*>(e)) {
    // Lambda expressions - create a callable function object with closure
    Value::Map funcObj;
    funcObj["_type"] = Value::from_string("Lambda");
    funcObj["_paramCount"] = Value::from_int(static_cast<long long>(lambda->params.size()));
    
    // Store closure (captured environment)
    Value::Map closure;
    for (const auto& pair : env.vars) {
      closure[pair.first] = pair.second;
    }
    funcObj["_closure"] = Value::from_map(std::move(closure));
    
    // Store lambda AST (in a real implementation, this would be serialized)
    // For now, we'll store a reference that can be looked up
    funcObj["_lambdaId"] = Value::from_int(static_cast<long long>(reinterpret_cast<intptr_t>(lambda)));
    
    return Value::from_map(std::move(funcObj));
  }
  if (auto interp = dynamic_cast<const InterpolatedString*>(e)) {
    // String interpolation - concatenate parts
    std::ostringstream oss;
    for (const auto& part : interp->parts) {
      Value val = eval(env, R, part.get(), debug_mode);
      if (val.is_string()) {
        oss << val.as_string();
      } else {
        try {
          oss << val.as_number();
        } catch (...) {
          try {
            oss << val.as_int();
          } catch (...) {
            try {
              oss << (val.as_bool() ? "TRUE" : "FALSE");
            } catch (...) {
              oss << "NIL";
            }
          }
        }
      }
    }
    return Value::from_string(oss.str());
  }
  if (auto map = dynamic_cast<const MapLiteral*>(e)) {
    // Map/dictionary literal
    Value::Map result;
    for (const auto& [keyExpr, valueExpr] : map->entries) {
      Value keyVal = eval(env, R, keyExpr.get(), debug_mode);
      Value valueVal = eval(env, R, valueExpr.get(), debug_mode);
      std::string key;
      if (keyVal.is_string()) {
        key = keyVal.as_string();
      } else {
        key = std::to_string(to_num(keyVal));
      }
      result[key] = valueVal;
    }
    return Value::from_map(std::move(result));
  }
  if (auto to = dynamic_cast<const TypeOfExpr*>(e)) {
    // TYPEOF expression
    Value val = eval(env, R, to->value.get(), debug_mode);
    std::string typeName;
    if (val.is_string()) typeName = "STRING";
    else if (val.is_array()) typeName = "ARRAY";
    else if (val.is_map()) {
      const auto& map = val.as_map();
      auto it = map.find("_type");
      if (it != map.end() && it->second.is_string()) {
        typeName = it->second.as_string();
      } else {
        typeName = "MAP";
      }
    }
    else if (val.is_nil()) typeName = "NIL";
    else {
      // Try to determine if it's a number or int
      try {
        (void)val.as_int();
        typeName = "INTEGER";
      } catch (...) {
        try {
          val.as_number();
          typeName = "NUMBER";
        } catch (...) {
          try {
            val.as_bool();
            typeName = "BOOLEAN";
          } catch (...) {
            typeName = "UNKNOWN";
          }
        }
      }
    }
    return Value::from_string(typeName);
  }
  if (auto gp = dynamic_cast<const GetPropertiesExpr*>(e)) {
    // GETPROPERTIES expression
    Value val = eval(env, R, gp->value.get(), debug_mode);
    if (!val.is_map()) {
      return Value::from_array({});
    }
    const auto& map = val.as_map();
    std::vector<Value> props;
    for (const auto& [key, value] : map) {
      if (key != "_type" && key != "_object" && key != "_method" && key != "_function") {
        props.push_back(Value::from_string(key));
      }
    }
    return Value::from_array(std::move(props));
  }
  if (auto gm = dynamic_cast<const GetMethodsExpr*>(e)) {
    // GETMETHODS expression
    Value val = eval(env, R, gm->value.get(), debug_mode);
    if (!val.is_map()) {
      return Value::from_array({});
    }
    const auto& map = val.as_map();
    std::vector<Value> methods;
    
    // Check if it's a user-defined type
    auto typeIt = map.find("_type");
    if (typeIt != map.end() && typeIt->second.is_string()) {
      std::string typeName = typeIt->second.as_string();
      TypeRegistry* typeReg = get_type_registry();
      if (typeReg) {
        const TypeInfo* typeInfo = typeReg->getType(typeName);
        if (typeInfo) {
          for (const auto& [methodName, signature] : typeInfo->methods) {
            methods.push_back(Value::from_string(methodName));
          }
        }
      }
    }
    
    return Value::from_array(std::move(methods));
  }
  if (auto sc = dynamic_cast<const SuperCall*>(e)) {
    // SUPER call - resolve to parent type method
    // Look for "this" in current environment
    Value thisObj = env.get("this");
    if (!thisObj.is_map()) {
      throw std::runtime_error("SUPER: can only be called from within a method");
    }
    
    const auto& objMap = thisObj.as_map();
    auto typeIt = objMap.find("_type");
    if (typeIt == objMap.end() || !typeIt->second.is_string()) {
      throw std::runtime_error("SUPER: object has no type");
    }
    
    std::string typeName = typeIt->second.as_string();
    TypeRegistry* typeReg = get_type_registry();
    if (!typeReg) {
      throw std::runtime_error("SUPER: TypeRegistry not available");
    }
    
    const TypeInfo* typeInfo = typeReg->getType(typeName);
    if (!typeInfo || !typeInfo->hasParent) {
      throw std::runtime_error("SUPER: type " + typeName + " has no parent");
    }
    
    // Build method name: PARENTTYPE_METHODNAME
    std::string parentType = typeInfo->parentType;
    std::string methodName = Env::up(sc->method);
    std::string methodFunc = parentType + "_" + methodName;
    
    // Evaluate arguments
    std::vector<Value> args;
    args.push_back(thisObj); // Prepend 'this' as first argument
    for (const auto& arg : sc->args) {
      args.push_back(eval(env, R, arg.get(), debug_mode));
    }
    
    // Try to call parent method
    const auto* fn = R.find(methodFunc);
    if (fn) {
      return (*fn)(args);
    }
    
    // Try as user-defined function
    auto itf = g_funcs.find(methodFunc);
    if (itf != g_funcs.end()) {
      return call_func(env, R, methodFunc, args, {}, debug_mode);
    }
    
    throw std::runtime_error("SUPER: parent method " + methodFunc + " not found");
  }
  
  // New modern features
  if (auto range = dynamic_cast<const RangeLiteral*>(e)) {
    Value start_val = eval(env, R, range->start.get(), debug_mode);
    Value end_val = eval(env, R, range->end.get(), debug_mode);
    
    // Create array from range
    Value::Array arr;
    if (start_val.is_string() && end_val.is_string()) {
      // String range: "a".."z"
      std::string start_str = start_val.as_string();
      std::string end_str = end_val.as_string();
      if (start_str.length() == 1 && end_str.length() == 1) {
        char start_ch = start_str[0];
        char end_ch = end_str[0];
        if (start_ch <= end_ch) {
          for (char c = start_ch; c <= end_ch; ++c) {
            arr.push_back(Value::from_string(std::string(1, c)));
          }
        }
      }
    } else {
      // Numeric range: 1..10
      long long start = start_val.as_int();
      long long end = end_val.as_int();
      if (start <= end) {
        for (long long i = start; i <= end; ++i) {
          arr.push_back(Value::from_int(i));
        }
      } else {
        for (long long i = start; i >= end; --i) {
          arr.push_back(Value::from_int(i));
        }
      }
    }
    return Value::from_array(std::move(arr));
  }
  
  if (auto comp = dynamic_cast<const ArrayComprehension*>(e)) {
    Value collection_val = eval(env, R, comp->collection.get(), debug_mode);
    Value::Array result;
    
    if (collection_val.is_array()) {
      const auto& coll = collection_val.as_array();
      Env local; local.parent = &env; local.strict = env.strict;
      local.declare(comp->var);
      
      for (const auto& item : coll) {
        local.set(comp->var, item);
        
        // Check condition if present
        if (comp->condition) {
          Value cond_val = eval(local, R, comp->condition.get(), debug_mode);
          if (!truthy(cond_val)) continue;
        }
        
        // Evaluate expression
        Value expr_val = eval(local, R, comp->expr.get(), debug_mode);
        result.push_back(expr_val);
      }
    }
    return Value::from_array(std::move(result));
  }
  
  if (auto null_safe = dynamic_cast<const NullSafeAccess*>(e)) {
    Value obj = eval(env, R, null_safe->object.get(), debug_mode);
    if (obj.is_nil()) return Value::nil();
    
    if (null_safe->isIndex) {
      // ?[index]
      Value idx_val = eval(env, R, null_safe->index.get(), debug_mode);
      if (obj.is_array()) {
        long long idx = idx_val.as_int();
        const auto& arr = obj.as_array();
        if (idx >= 0 && (size_t)idx < arr.size()) {
          return arr[(size_t)idx];
        }
      } else if (obj.is_map()) {
        // Support map indexing with string keys
        if (idx_val.is_string()) {
          const auto& map = obj.as_map();
          std::string key = Env::up(idx_val.as_string());
          auto it = map.find(key);
          if (it != map.end()) return it->second;
        } else if (idx_val.is_int()) {
          // Try numeric key as string
          const auto& map = obj.as_map();
          std::string key = std::to_string(idx_val.as_int());
          auto it = map.find(key);
          if (it != map.end()) return it->second;
        }
      } else if (obj.is_string()) {
        // String indexing
        long long idx = idx_val.as_int();
        const std::string& str = obj.as_string();
        if (idx >= 0 && (size_t)idx < str.size()) {
          return Value::from_string(std::string(1, str[(size_t)idx]));
        }
      }
      return Value::nil();
    } else {
      // ?.member - enhanced with hooks and deep property access
      // Try hooks first (for ECS, etc.)
      if (auto hookValue = try_resolve_member(obj, null_safe->member); hookValue.has_value()) {
        return *hookValue;
      }
      
      if (obj.is_map()) {
        const auto& map = obj.as_map();
        std::string member_upper = Env::up(null_safe->member);
        
        // Try direct access first
        auto it = map.find(member_upper);
        if (it != map.end()) return it->second;
        
        // Try deep property access (nested maps)
        if (auto deep = get_deep_property(obj, null_safe->member)) {
          return *deep;
        }
        
        // Check if this is an object type trying to access a method
        auto type_it = map.find("_type");
        if(type_it != map.end() && type_it->second.is_string()){
          std::string obj_type = type_it->second.as_string();
          // Try to resolve as object method: TYPE_METHODNAME
          std::string method_func = obj_type + "_" + member_upper;
          const auto* fn = R.find(method_func);
          if(fn){
            // Return a method object
            Value::Map method_obj;
            method_obj["_type"] = Value::from_string("Method");
            method_obj["_object"] = obj;
            method_obj["_method"] = Value::from_string(member_upper);
            method_obj["_function"] = Value::from_string(method_func);
            return Value::from_map(std::move(method_obj));
          }
        }
      } else if (obj.is_string()) {
        // String methods
        std::string method_func = "STRING_" + Env::up(null_safe->member);
        const auto* fn = R.find(method_func);
        if(fn){
          Value::Map method_obj;
          method_obj["_type"] = Value::from_string("Method");
          method_obj["_object"] = obj;
          method_obj["_method"] = Value::from_string(Env::up(null_safe->member));
          method_obj["_function"] = Value::from_string(method_func);
          return Value::from_map(std::move(method_obj));
        }
        // String properties
        if (null_safe->member == "length" || null_safe->member == "LENGTH" || 
            null_safe->member == "size" || null_safe->member == "SIZE") {
          return Value::from_int(static_cast<long long>(obj.as_string().size()));
        }
      } else if (obj.is_array()) {
        // Array methods
        std::string method_func = "ARRAY_" + Env::up(null_safe->member);
        const auto* fn = R.find(method_func);
        if(fn){
          Value::Map method_obj;
          method_obj["_type"] = Value::from_string("Method");
          method_obj["_object"] = obj;
          method_obj["_method"] = Value::from_string(Env::up(null_safe->member));
          method_obj["_function"] = Value::from_string(method_func);
          return Value::from_map(std::move(method_obj));
        }
        // Array properties
        if (null_safe->member == "length" || null_safe->member == "LENGTH" || 
            null_safe->member == "size" || null_safe->member == "SIZE") {
          return Value::from_int(static_cast<long long>(obj.as_array().size()));
        }
      }
      return Value::nil();
    }
  }
  
  if (auto null_coalesce = dynamic_cast<const NullCoalesceExpr*>(e)) {
    Value left = eval(env, R, null_coalesce->left.get(), debug_mode);
    if (!left.is_nil()) return left;
    return eval(env, R, null_coalesce->right.get(), debug_mode);
  }
  
  if (auto tuple = dynamic_cast<const TupleLiteral*>(e)) {
    // Tuples are represented as arrays
    Value::Array arr;
    for (const auto& elem : tuple->elements) {
      arr.push_back(eval(env, R, elem.get(), debug_mode));
    }
    return Value::from_array(std::move(arr));
  }
  
  if (auto match = dynamic_cast<const MatchExpr*>(e)) {
    Value value = eval(env, R, match->value.get(), debug_mode);
    
    for (const auto& c : match->cases) {
      Value pattern = eval(env, R, c.pattern.get(), debug_mode);
      
      // Enhanced pattern matching
      bool matches = false;
      
      // Exact match
      if (cmp_values(value, pattern) == 0) {
        matches = true;
      }
      // Range matching (if pattern is a range object)
      else if (pattern.is_map()) {
        const auto& map = pattern.as_map();
        auto typeIt = map.find("_type");
        if (typeIt != map.end() && typeIt->second.is_string() && typeIt->second.as_string() == "Range") {
          // Check if value is within range
          if (value.is_number() || value.is_int()) {
            double val = value.as_number();
            auto startIt = map.find("start");
            auto endIt = map.find("end");
            if (startIt != map.end() && endIt != map.end()) {
              double start = startIt->second.as_number();
              double end = endIt->second.as_number();
              matches = (val >= start && val <= end);
            }
          }
        }
      }
      // Array matching (check if value is in array)
      else if (pattern.is_array()) {
        const auto& arr = pattern.as_array();
        for (const auto& item : arr) {
          if (cmp_values(value, item) == 0) {
            matches = true;
            break;
          }
        }
      }
      
      if (matches) {
        return eval(env, R, c.result.get(), debug_mode);
      }
    }
    
    if (match->defaultCase) {
      return eval(env, R, match->defaultCase.get(), debug_mode);
    }
    
    return Value::nil();
  }
  
  if (auto spread = dynamic_cast<const SpreadExpr*>(e)) {
    Value val = eval(env, R, spread->value.get(), debug_mode);
    // Spread is handled at the call site (array/map literals)
    return val;
  }
  
  throw std::runtime_error("unknown expr");
  return Value::nil(); // This line will never be reached, but satisfies the compiler
}

static void exec(Env& env, FunctionRegistry& R, const Stmt* s, bool debug_mode){
  (void)env; (void)R; (void)s; // Suppress unused parameter warnings
  if(auto ox = dynamic_cast<const OptionExplicit*>(s)){
    env.strict = ox->enabled;
    return;
  }
  if(auto ld = dynamic_cast<const LocalDecl*>(s)){
    // Declare locals and create a local slot (nil) so they shadow outer/global
    for(const auto& name : ld->names){
      env.declare(name);
      env.vars[Env::up(name)] = Value::nil();
    }
    return;
  }
  if(auto gd = dynamic_cast<const GlobalDecl*>(s)){
    // Mark names as global in this scope; declare at root if in strict mode
    for(const auto& name : gd->names){
      const std::string key = Env::up(name);
      env.globals_here.insert(key);
      Env* r = env.root();
      if(env.strict && !r->is_declared(name)){
        r->declare(name);
      }
    }
    return;
  }
      if(auto p = dynamic_cast<const Print*>(s)){
      Value v = eval(env,R,p->value.get(), debug_mode);
      call(R, "PRINT", {v});
      return;
    }
  if(auto l = dynamic_cast<const Let*>(s)){
    env.declare(l->name);
    env.set(l->name, eval(env,R,l->value.get(), debug_mode)); return;
  }
  if(auto cd = dynamic_cast<const ConstDecl*>(s)){
    Value v = eval(env,R,cd->value.get(), debug_mode);
    env.define_const(cd->name, std::move(v));
    return;
  }
  if(dynamic_cast<const ImportStmt*>(s)){
    // Imports are expanded before interpretation; nothing to do at runtime
    return;
  }
  if(auto a = dynamic_cast<const Assign*>(s)){
    env.set(a->name, eval(env,R,a->value.get(), debug_mode)); return;
  }
  if(auto e = dynamic_cast<const ExprStmt*>(s)){
    (void)eval(env,R,e->expr.get(), debug_mode); return;
  }
  if(auto c = dynamic_cast<const CallStmt*>(s)){
    std::vector<Value> args; for(auto& x:c->args) args.push_back(eval(env,R,x.get(), debug_mode));
    auto it = g_subs.find(Env::up(c->name));
    if(it!=g_subs.end()) { call_sub(env,R,c->name,args, debug_mode); return; }
    auto itf = g_funcs.find(Env::up(c->name));
    if(itf!=g_funcs.end()) { (void)call_func(env,R,c->name,args, {}, debug_mode); return; }
    (void)call(R,c->name,args); return;
  }
  if(auto w = dynamic_cast<const WhileWend*>(s)){
    while(truthy(eval(env,R,w->cond.get(), debug_mode))){
      try {
        for(auto& ss : w->body) exec(env,R,ss.get(), debug_mode);
      } catch(const BreakSignal&) {
        break; // Exit the loop
      } catch(const ExitSignal& es) {
        if(es.target == "WHILE") break;
        throw; // Re-throw if not for this loop type
      } catch(const ContinueSignal&) {
        // Continue to next iteration
      }
    }
    return;
  }
  if(auto f = dynamic_cast<const ForNext*>(s)){
    // Initialize loop variable
    Value start_val = eval(env, R, f->init.get(), debug_mode);
    if (debug_mode) std::cerr << "FOR start: " << value_to_string_safe(start_val) << std::endl;
    Value limit_val = eval(env, R, f->limit.get(), debug_mode);
    if (debug_mode) std::cerr << "FOR limit: " << value_to_string_safe(limit_val) << std::endl;
    Value step_val = f->step ? eval(env, R, f->step.get(), debug_mode) : Value::from_number(1.0);
    if (debug_mode) std::cerr << "FOR step: " << value_to_string_safe(step_val) << std::endl;
    double init = start_val.as_number();
    double limit = limit_val.as_number();
    double step = step_val.as_number();
    if(debug_mode){
      std::cerr << "[FOR] var=" << f->var
                << " init=" << value_to_string_safe(start_val)
                << " limit=" << value_to_string_safe(limit_val)
                << " step=" << value_to_string_safe(step_val) << std::endl;
    }
    env.declare(f->var);
    env.set(f->var, Value::from_number(init));
    auto cond = [&](double v){ return step >= 0 ? (v <= limit) : (v >= limit); };
    while(true){
      double cur;
      try {
        cur = env.get(f->var).as_number();
      } catch(const std::exception& e){
        if(debug_mode){
          std::cerr << "[FOR] non-numeric loop var '" << f->var << "' encountered: " << e.what() << std::endl;
        }
        throw; // rethrow
      }
      if(!cond(cur)) break;
      try {
        for(auto& ss : f->body) exec(env,R,ss.get(), debug_mode);
      } catch(const BreakSignal&) {
        break; // Exit the loop
      } catch(const ExitSignal& es) {
        if(es.target == "FOR") break;
        throw; // Re-throw if not for this loop type
      } catch(const ContinueSignal&) {
        // fallthrough to step update
      }
      env.set(f->var, Value::from_number(env.get(f->var).as_number() + step));
    }
    return;
  }
  if(auto d = dynamic_cast<const DoLoop*>(s)){
    while(true){
      try{
        for(auto& ss : d->body) exec(env,R,ss.get(), debug_mode);
      } catch(const BreakSignal&){
        break;
      } catch(const ExitSignal& es) {
        if(es.target == "DO" || es.target == "LOOP") break;
        throw; // Re-throw if not for this loop type
      } catch(const ContinueSignal&){
        // continue next iteration
      }
    }
    return;
  }
  if(auto ru = dynamic_cast<const RepeatUntil*>(s)){
    while(true){
      try{
        for(auto& ss : ru->body) exec(env,R,ss.get(), debug_mode);
      } catch(const BreakSignal&){
        break;
      } catch(const ExitSignal& es) {
        if(es.target == "REPEAT" || es.target == "UNTIL") break;
        throw; // Re-throw if not for this loop type
      } catch(const ContinueSignal&){
        // after continue, still evaluate condition for post-test semantics
      }
      if(truthy(eval(env,R,ru->cond.get(), debug_mode))) break;
    }
    return;
  }
  if(auto i = dynamic_cast<const IfThenEndIf*>(s)){
    if(truthy(eval(env,R,i->cond.get(), debug_mode))){ for(auto& ss:i->body) exec(env,R,ss.get(), debug_mode); }
    return;
  }
  if(auto ic = dynamic_cast<const IfChain*>(s)){
    bool ran = false;
    for(const auto& br : ic->branches){
      if(truthy(eval(env, R, br.cond.get(), debug_mode))){
        for(const auto& ss : br.body) exec(env, R, ss.get(), debug_mode);
        ran = true;
        break;
      }
    }
    if(!ran && ic->hasElse){
      for(const auto& ss : ic->elseBody) exec(env, R, ss.get(), debug_mode);
    }
    return;
  }
  if(auto r = dynamic_cast<const Return*>(s)){
    Value rv = r->value ? eval(env,R,r->value.get(), debug_mode) : Value::nil();
    throw ReturnSignal{rv};
  }
  if(dynamic_cast<const Break*>(s)){
    throw BreakSignal{};
  }
  if(dynamic_cast<const Continue*>(s)){
    throw ContinueSignal{};
  }
  if(auto e = dynamic_cast<const Exit*>(s)){
    throw ExitSignal{e->target};
  }
  if(auto d = dynamic_cast<const Dim*>(s)){
    Value v = make_array_from_sizes(env, R, d->sizes, 0, debug_mode);
    env.declare(d->name);
    env.set(d->name, std::move(v));
    return;
  }
  if(auto rd = dynamic_cast<const Redim*>(s)){
    if(env.is_const(rd->name)){
      throw std::runtime_error("REDIM on constant '" + Env::up(rd->name) + "'");
    }
    if(rd->preserve){
      // Evaluate sizes once, construct new array and copy overlap
      std::vector<size_t> lens = eval_sizes_to_vec(env, R, rd->sizes, debug_mode);
      size_t total = 0;
      // Debug: print sizes and note PRESERVE path
      if(debug_mode){
        std::ostringstream oss; oss << "[REDIM] PRESERVE sizes=[";
        for(size_t i=0;i<lens.size();++i){ if(i) oss<<","; oss<<lens[i]; }
        oss << "]";
        std::cerr << oss.str() << std::endl;
      }
      if(!safe_total_elems(lens, total)){
        std::ostringstream oss; oss << "REDIM PRESERVE too large. sizes=[";
        for(size_t i=0;i<lens.size();++i){ if(i) oss<<","; oss<<lens[i]; }
        oss << "] total(capped)=" << total;
        throw std::runtime_error(oss.str());
      }
      if(debug_mode) std::cerr << "[REDIM] PRESERVE total elements=" << total << std::endl;
      Value oldv = env.get(rd->name);
      Value newv;
      if(oldv.is_array()){
        if(debug_mode) std::cerr << "[REDIM] copying overlap from existing array" << std::endl;
        newv = redim_preserve_copy(oldv, lens, 0);
      } else {
        if(debug_mode) std::cerr << "[REDIM] variable not array; creating new with lens" << std::endl;
        newv = make_array_from_lenvec(lens, 0);
      }
      env.set(rd->name, std::move(newv));
    } else {
      // Reinitialize array to new sizes (no PRESERVE semantics)
      std::vector<size_t> lens = eval_sizes_to_vec(env, R, rd->sizes, debug_mode);
      size_t total = 0;
      if(debug_mode){
        std::ostringstream oss; oss << "[REDIM] sizes=[";
        for(size_t i=0;i<lens.size();++i){ if(i) oss<<","; oss<<lens[i]; }
        oss << "]";
        std::cerr << oss.str() << std::endl;
      }
      if(!safe_total_elems(lens, total)){
        std::ostringstream oss; oss << "REDIM too large. sizes=[";
        for(size_t i=0;i<lens.size();++i){ if(i) oss<<","; oss<<lens[i]; }
        oss << "] total(capped)=" << total;
        throw std::runtime_error(oss.str());
      }
      if(debug_mode) std::cerr << "[REDIM] total elements=" << total << std::endl;
      // We still want to support expression re-evaluation semantics for non-preserve path;
      // however, we validated shape is reasonable using lens.
      Value v = make_array_from_sizes(env, R, rd->sizes, 0, debug_mode);
      env.set(rd->name, std::move(v));
    }
    return;
  }
  if(auto ai = dynamic_cast<const AssignIndex*>(s)){
    if(env.is_const(ai->name)){
      throw std::runtime_error("Indexed assignment to constant '" + Env::up(ai->name) + "'");
    }
    Value target = env.get(ai->name);
    if(!target.is_array()) target = Value::from_array({});
    // Traverse/create nested arrays according to indices except last
    Value* current = &target;
    for(size_t d=0; d+1<ai->indices.size(); ++d){
      long long idx = eval(env, R, ai->indices[d].get(), debug_mode).as_int();
      if(idx < 0) return; // ignore
      auto& arr = current->as_array();
      if((size_t)idx >= arr.size()) arr.resize((size_t)idx+1, Value::nil());
      if(!arr[(size_t)idx].is_array()) arr[(size_t)idx] = Value::from_array({});
      current = &arr[(size_t)idx];
    }
    // Set final element
    if(ai->indices.empty()) return; // nothing to do
    long long last = eval(env, R, ai->indices.back().get(), debug_mode).as_int();
    if(last < 0) return;
    auto& arr = current->as_array();
    if((size_t)last >= arr.size()) arr.resize((size_t)last+1, Value::nil());
    arr[(size_t)last] = eval(env, R, ai->value.get(), debug_mode);
    env.set(ai->name, std::move(target));
    return;
  }
  if(auto am = dynamic_cast<const AssignMember*>(s)){
    Value obj = eval(env, R, am->object.get(), debug_mode);
    Value val = eval(env, R, am->value.get(), debug_mode);
    
    // Try hooks first (for ECS, etc.)
    if(try_assign_member(obj, am->member, val)){
      if(auto var = dynamic_cast<const Variable*>(am->object.get())){
        env.set(var->name, obj);
      }
      return;
    }
    
    // Check if object is a map (object)
    if(obj.is_map()){
      auto& map = obj.as_map();
      std::string memberUpper = Env::up(am->member);
      
      // Check property descriptors for setters
      std::string objId;
      auto idIt = map.find("_id");
      if (idIt != map.end() && idIt->second.is_int()) {
        objId = std::to_string(idIt->second.as_int());
        // In a real implementation, would check g_property_descriptors here for setters
        // If setter exists, call it instead of direct assignment
      }
      
      // Check if property is writable (from descriptor)
      // In a real implementation, would check descriptor.writable flag
      
      // Try deep property assignment
      if (set_deep_property(obj, am->member, val)) {
        // If the object came from a variable, update it
        if(auto var = dynamic_cast<const Variable*>(am->object.get())){
          env.set(var->name, obj);
        }
        return;
      }
      
      // Try case-insensitive assignment
      bool found = false;
      for (auto& pair : map) {
        std::string keyUpper;
        keyUpper.reserve(pair.first.size());
        for (char c : pair.first) {
          keyUpper.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
        }
        if (keyUpper == memberUpper) {
          pair.second = val;
          found = true;
          break;
        }
      }
      
      if (!found) {
        // Fallback to simple assignment
        map[memberUpper] = val;
      }
      
      // If the object came from a variable, update it
      if(auto var = dynamic_cast<const Variable*>(am->object.get())){
        env.set(var->name, obj);
      }
      return;
    }
    
    // If not a map, create one
    Value::Map new_obj;
    new_obj[Env::up(am->member)] = val;
    Value new_value = Value::from_map(std::move(new_obj));
    
    if(auto var = dynamic_cast<const Variable*>(am->object.get())){
      env.set(var->name, new_value);
    }
    return;
  }
  if(auto sc = dynamic_cast<const SelectCaseStmt*>(s)){
    // Evaluate selector once
    Value sel = eval(env, R, sc->selector.get(), debug_mode);
    // First pass: try non-ELSE branches
    for(const auto& br : sc->branches){
      if(br.isElse) continue;
      bool doRun = false;
      if(br.isRel){
        Value rv = eval(env, R, br.relExpr.get(), debug_mode);
        int c = cmp_values(sel, rv);
        switch(br.relOp){
          case Tok::Eq:  doRun = (c == 0); break;
          case Tok::Neq: doRun = (c != 0); break;
          case Tok::Lt:  doRun = (c <  0); break;
          case Tok::Lte: doRun = (c <= 0); break;
          case Tok::Gt:  doRun = (c >  0); break;
          case Tok::Gte: doRun = (c >= 0); break;
          default: break;
        }
      } else {
        // Evaluate case values, first equal match wins
        for(const auto& vexpr : br.values){
          Value cv = eval(env, R, vexpr.get(), debug_mode);
          if(cmp_values(sel, cv) == 0){ doRun = true; break; }
        }
      }
      if(doRun){
        for(const auto& ss : br.body) exec(env, R, ss.get(), debug_mode);
        return;
      }
    }
    // Second pass: ELSE
    for(const auto& br : sc->branches){
      if(br.isElse){
        for(const auto& ss : br.body) exec(env, R, ss.get(), debug_mode);
        break;
      }
    }
    return;
  }
  if(dynamic_cast<const SubDecl*>(s)){
    // SubDecl statements are handled during program collection, not execution
    return;
  }
  if(dynamic_cast<const FunctionDecl*>(s)){
    // FunctionDecl statements are handled during program collection, not execution
    return;
  }
  
  // Extension statement types (to be implemented)
  if (auto td = dynamic_cast<const TypeDecl*>(s)) {
    // Type declaration - register type
    TypeRegistry* typeReg = get_type_registry();
    if (typeReg) {
      TypeInfo typeInfo;
      typeInfo.name = td->name;
      typeInfo.hasParent = td->hasParent;
      if (td->hasParent) {
        typeInfo.parentType = td->parentType;
      }
      for (const auto& field : td->fields) {
        typeInfo.fields.emplace_back(field.name, field.hasType ? field.typeName : "ANY");
      }
      typeReg->registerType(typeInfo);
      // Register constructor
      std::string ctorName = td->name;
      R.add(ctorName, NativeFn{ctorName, -1, [typeReg, ctorName](const std::vector<Value>& args) {
        return typeReg->createInstance(ctorName);
      }});
    }
    return;
  }
  if (auto fe = dynamic_cast<const ForEach*>(s)) {
    Value collection = eval(env, R, fe->collection.get(), debug_mode);
    if (collection.is_array()) {
      for (const auto& item : collection.as_array()) {
        env.set(fe->var, item);
        for (const auto& bodyStmt : fe->body) {
          exec(env, R, bodyStmt.get(), debug_mode);
        }
      }
    } else if (collection.is_map()) {
      for (const auto& [key, value] : collection.as_map()) {
        Value::Map pair;
        pair["key"] = Value::from_string(key);
        pair["value"] = value;
        env.set(fe->var, Value::from_map(std::move(pair)));
        for (const auto& bodyStmt : fe->body) {
          exec(env, R, bodyStmt.get(), debug_mode);
        }
      }
    }
    return;
  }
  if (auto ast = dynamic_cast<const AssertStmt*>(s)) {
    Value condition = eval(env, R, ast->condition.get(), debug_mode);
    if (!truthy(condition)) {
      std::string message = "Assertion failed";
      if (ast->message) {
        Value msgVal = eval(env, R, ast->message.get(), debug_mode);
        if (msgVal.is_string()) message = msgVal.as_string();
      }
      throw std::runtime_error(message);
    }
    return;
  }
  if (auto bp = dynamic_cast<const BreakpointStmt*>(s)) {
    if (debug_mode) {
      std::cerr << "[BREAKPOINT] Execution paused" << std::endl;
    }
    return;
  }
  if (auto using_block = dynamic_cast<const UsingBlock*>(s)) {
    // Evaluate resource
    Value resource = eval(env, R, using_block->resource.get(), debug_mode);
    Env local; local.parent = &env; local.strict = env.strict;
    local.declare(using_block->varName);
    local.set(using_block->varName, resource);
    
    try {
      // Execute body
      for (auto& stmt : using_block->body) {
        exec(local, R, stmt.get(), debug_mode);
      }
    } catch (...) {
      // Re-throw - cleanup would happen here if needed
      throw;
    }
    // Automatic cleanup (if resource has cleanup method, call it)
    return;
  }
  if (auto enum_decl = dynamic_cast<const EnumDecl*>(s)) {
    // Register enum values as constants
    for (size_t i = 0; i < enum_decl->values.size(); ++i) {
      std::string enum_value = enum_decl->name + "_" + enum_decl->values[i];
      env.define_const(enum_value, Value::from_int(static_cast<long long>(i)));
    }
    return;
  }
  if (auto union_decl = dynamic_cast<const UnionDecl*>(s)) {
    // Register union type (for now, just store the declaration)
    // Full implementation would require type checking
    return;
  }
  if (auto destr = dynamic_cast<const DestructureAssign*>(s)) {
    Value value = eval(env, R, destr->value.get(), debug_mode);
    if (value.is_array()) {
      const auto& arr = value.as_array();
      for (size_t i = 0; i < destr->names.size() && i < arr.size(); ++i) {
        env.declare(destr->names[i]);
        env.set(destr->names[i], arr[i]);
      }
    } else if (value.is_map()) {
      const auto& map = value.as_map();
      for (const auto& name : destr->names) {
        std::string key = Env::up(name);
        auto it = map.find(key);
        env.declare(name);
        env.set(name, it != map.end() ? it->second : Value::nil());
      }
    }
    return;
  }
  if (auto dp = dynamic_cast<const DebugPrintStmt*>(s)) {
    if (debug_mode) {
      Value val = eval(env, R, dp->value.get(), debug_mode);
      std::cerr << "[DEBUG] " << (val.is_string() ? val.as_string() : std::to_string(to_num(val))) << std::endl;
    }
    return;
  }
  if (auto try_catch = dynamic_cast<const TryCatchStmt*>(s)) {
    // Execute try block
    try {
      for (auto& stmt : try_catch->tryBody) {
        exec(env, R, stmt.get(), debug_mode);
      }
    } catch (const std::exception& e) {
      // Catch block
      if (try_catch->hasCatch) {
        Env catchEnv; catchEnv.parent = &env; catchEnv.strict = env.strict;
        if (!try_catch->catchVar.empty()) {
          catchEnv.declare(try_catch->catchVar);
          Value::Map errorObj;
          errorObj["_type"] = Value::from_string("Error");
          errorObj["message"] = Value::from_string(e.what());
          catchEnv.set(try_catch->catchVar, Value::from_map(std::move(errorObj)));
        }
        for (auto& stmt : try_catch->catchBody) {
          exec(catchEnv, R, stmt.get(), debug_mode);
        }
      } else {
        // No catch block, re-throw
        throw;
      }
    } catch (...) {
      // Catch any other exception
      if (try_catch->hasCatch) {
        Env catchEnv; catchEnv.parent = &env; catchEnv.strict = env.strict;
        if (!try_catch->catchVar.empty()) {
          catchEnv.declare(try_catch->catchVar);
          Value::Map errorObj;
          errorObj["_type"] = Value::from_string("Error");
          errorObj["message"] = Value::from_string("Unknown error");
          catchEnv.set(try_catch->catchVar, Value::from_map(std::move(errorObj)));
        }
        for (auto& stmt : try_catch->catchBody) {
          exec(catchEnv, R, stmt.get(), debug_mode);
        }
      } else {
        throw;
      }
    }
    
    // Finally block (always executes)
    if (try_catch->hasFinally) {
      for (auto& stmt : try_catch->finallyBody) {
        exec(env, R, stmt.get(), debug_mode);
      }
    }
    return;
  }
  if (auto throw_stmt = dynamic_cast<const ThrowStmt*>(s)) {
    Value error = eval(env, R, throw_stmt->error.get(), debug_mode);
    std::string errorMsg;
    if (error.is_string()) {
      errorMsg = error.as_string();
    } else if (error.is_map()) {
      const auto& map = error.as_map();
      auto msgIt = map.find("message");
      if (msgIt != map.end() && msgIt->second.is_string()) {
        errorMsg = msgIt->second.as_string();
      } else {
        errorMsg = "Error thrown";
      }
    } else {
      errorMsg = "Error thrown";
    }
    throw std::runtime_error(errorMsg);
  }
}

static void call_sub(Env& caller, FunctionRegistry& R, const std::string& name, const std::vector<Value>& args, bool debug_mode){
  auto it = g_subs.find(Env::up(name));
  if(it==g_subs.end()) return;
  const SubDecl* sd = it->second;
  Env local; local.parent = &caller; local.strict = caller.strict;
  // Bind parameters
  size_t n = std::min(sd->params.size(), args.size());
  for(size_t i=0;i<n;++i){
    local.declare(sd->params[i]);
    local.set(sd->params[i], args[i]);
  }
  try{
    for(auto& s : sd->body) exec(local, R, s.get(), debug_mode);
  } catch(const ReturnSignal&){ /* swallow */ }
  catch(const ExitSignal& es) {
    if(es.target == "SUB") return; // Exit the sub
    throw; // Re-throw if not for SUB
  }
}

static Value call_func(Env& caller, FunctionRegistry& R, const std::string& name, 
                       const std::vector<Value>& args, 
                       const std::map<std::string, Value>& namedArgs,
                       bool debug_mode){
  auto it = g_funcs.find(Env::up(name));
  if(it==g_funcs.end()) return Value::nil();
  const FunctionDecl* fd = it->second;
  Env local; local.parent = &caller; local.strict = caller.strict;
  
  // Build parameter map from positional and named arguments
  std::map<std::string, Value> paramValues;
  
  // First, process positional arguments
  size_t positionalCount = std::min(fd->params.size(), args.size());
  for(size_t i=0; i<positionalCount; ++i){
    paramValues[Env::up(fd->params[i].name)] = args[i];
  }
  
  // Then, process named arguments (override positional)
  for(const auto& [name, value] : namedArgs){
    paramValues[name] = value;
  }
  
  // Finally, fill in defaults for missing parameters
  for(size_t i=0; i<fd->params.size(); ++i){
    std::string paramName = Env::up(fd->params[i].name);
    if(paramValues.find(paramName) == paramValues.end()){
      if(fd->params[i].hasDefault){
        Value defaultVal = eval(local, R, fd->params[i].defaultValue.get(), debug_mode);
        paramValues[paramName] = defaultVal;
      } else {
        paramValues[paramName] = Value::nil();
      }
    }
  }
  
  // Set all parameters in local environment
  for(size_t i=0; i<fd->params.size(); ++i){
    std::string paramName = Env::up(fd->params[i].name);
    local.declare(fd->params[i].name);
    local.set(fd->params[i].name, paramValues[paramName]);
  }
  
  // Execute function body
  Value returnValue = Value::nil();
  try{
    for(auto& s : fd->body) exec(local, R, s.get(), debug_mode);
  } catch(const ReturnSignal& rs){ 
    returnValue = rs.v;
  } catch(const ExitSignal& es) {
    if(es.target == "FUNCTION") return Value::nil(); // Exit the function
    throw; // Re-throw if not for FUNCTION
  }
  // Check return type if specified
  if(fd->hasReturnType && !fd->returnType.empty()){
    // Basic type checking - could be enhanced
    std::string expectedType = Env::up(fd->returnType);
    std::string actualType;
    if(returnValue.is_string()) actualType = "STRING";
    else if(returnValue.is_array()) actualType = "ARRAY";
    else if(returnValue.is_map()) actualType = "MAP";
    else if(returnValue.is_nil()) actualType = "NIL";
    else {
      try {
        (void)returnValue.as_int();
        actualType = "INTEGER";
      } catch(...) {
        try {
          (void)returnValue.as_number();
          actualType = "NUMBER";
        } catch(...) {
          try {
            (void)returnValue.as_bool();
            actualType = "BOOLEAN";
          } catch(...) {
            actualType = "UNKNOWN";
          }
        }
      }
    }
    // Type checking is lenient for now - could be made strict
    if(debug_mode && expectedType != actualType && expectedType != "NIL" && actualType != "NIL"){
      std::cerr << "Warning: Return type mismatch in " << name << ": expected " 
                << expectedType << ", got " << actualType << std::endl;
    }
  }
  return returnValue;
}

} // namespace

void bas::set_namespace_registry(NamespaceRegistry* registry) {
  g_namespace_registry = registry;
}

int bas::interpret(const Program& prog, FunctionRegistry& R, bool debug_mode){
  try{
    Env env;
    g_subs.clear();
    g_funcs.clear();
    // Collect subs
    for(auto& s : prog.stmts){
      if(auto sd = dynamic_cast<SubDecl*>(s.get())){
        g_subs[Env::up(sd->name)] = sd;
        continue;
      }
      if(auto fd = dynamic_cast<FunctionDecl*>(s.get())){
        g_funcs[Env::up(fd->name)] = fd;
      }
    }
    // Execute non-sub statements
    for(auto& s : prog.stmts){
      if(dynamic_cast<SubDecl*>(s.get())) continue;
      if(dynamic_cast<FunctionDecl*>(s.get())) continue;
      exec(env, R, s.get(), debug_mode);
    }
    return 0;
  } catch(const std::exception& e){
    std::cerr << "Runtime exception: " << e.what() << std::endl;
    return 1;
  }
}

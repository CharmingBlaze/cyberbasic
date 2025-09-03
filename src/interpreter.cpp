#include "bas/ast.hpp"
#include "bas/runtime.hpp"
#include <unordered_map>
#include <unordered_set>
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

namespace {
// Exception classes for control flow
struct ReturnSignal { Value v; };
struct BreakSignal {};
struct ContinueSignal {};

// Simple verbose flag controlled by environment variable BAS_VERBOSE (non-zero => verbose)
static bool is_verbose(){
  const char* v = std::getenv("BAS_VERBOSE");
  if(!v) return false;
  return v[0] != '\0' && v[0] != '0';
}
struct Env {
  std::unordered_map<std::string, Value> vars;
  std::unordered_set<std::string> consts;
  const Env* parent{nullptr};
  static std::string up(const std::string& s){ std::string r; r.reserve(s.size()); for(char c:s) r.push_back((char)std::toupper((unsigned char)c)); return r; }
  Value get(const std::string& n) const {
    auto key = up(n);
    auto it = vars.find(key);
    if(it!=vars.end()) return it->second;
    if(parent) return parent->get(n);
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
  }
  void set(const std::string& n, Value v){
    auto key = up(n);
    if(is_const(key)){
      throw std::runtime_error("Assignment to constant '" + key + "'");
    }
    vars[key]=std::move(v);
  }
};

// Global storage for subroutines and functions
static std::unordered_map<std::string, const SubDecl*> g_subs;
static std::unordered_map<std::string, const FunctionDecl*> g_funcs;

// Forward declarations
static Value eval(Env& env, FunctionRegistry& R, const Expr* e);
static void exec(Env& env, FunctionRegistry& R, const Stmt* s);
static void call_sub(Env& caller, FunctionRegistry& R, const std::string& name, const std::vector<Value>& args);
static Value call_func(Env& caller, FunctionRegistry& R, const std::string& name, const std::vector<Value>& args);

// Helper functions
static bool truthy(const Value& v);
static int cmp_values(const Value& a, const Value& b);
static Value make_array_from_sizes(Env& env, FunctionRegistry& R, const std::vector<std::unique_ptr<Expr>>& sizes, size_t depth);

// Evaluate size expressions to a concrete vector<size_t>
static std::vector<size_t> eval_sizes_to_vec(Env& env, FunctionRegistry& R,
                                            const std::vector<std::unique_ptr<Expr>>& sizes){
  std::vector<size_t> out; out.reserve(sizes.size());
  for(auto& e : sizes){
    long long n = eval(env, R, e.get()).as_int();
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



static double to_num(const Value& v){ return v.as_number(); }
static bool truthy(const Value& v){ return v.as_bool(); }

// Stringify a Value similar to PRINT semantics (nil -> empty string)
static std::string value_to_string(const Value& v){
  if(auto s = std::get_if<std::string>(&v.v)) return *s;
  if(auto b = std::get_if<bool>(&v.v)) return *b?"TRUE":"FALSE";
  if(auto i = std::get_if<long long>(&v.v)) { std::ostringstream os; os.imbue(std::locale::classic()); os<<*i; return os.str(); }
  if(auto d = std::get_if<double>(&v.v)) { std::ostringstream os; os.imbue(std::locale::classic()); os<<*d; return os.str(); }
  return std::string{};
}

// String and generic comparison helpers (lexicographic for strings)
static int cmp_strings(const std::string& a, const std::string& b){
  if(a==b) return 0;
  return (a<b) ? -1 : 1;
}

static constexpr const char* kErrMixedCmp = "Type error: cannot compare string with non-string";

static int cmp_values(const Value& lhs, const Value& rhs){
  const bool ls = std::holds_alternative<std::string>(lhs.v);
  const bool rs = std::holds_alternative<std::string>(rhs.v);
  if(ls && rs){
    return cmp_strings(std::get<std::string>(lhs.v), std::get<std::string>(rhs.v));
  }
  if(ls ^ rs){
    throw std::runtime_error(kErrMixedCmp);
  }
  auto to_num_safe = [](const Value& v)->double{
    if(auto d = std::get_if<double>(&v.v)) return *d;
    if(auto i = std::get_if<long long>(&v.v)) return static_cast<double>(*i);
    if(auto b = std::get_if<bool>(&v.v)) return *b?1.0:0.0;
    if(std::holds_alternative<std::monostate>(v.v)) return 0.0;
    // Should not reach here because strings handled above
    throw std::runtime_error("Type error: non-numeric comparison");
  };
  const double a = to_num_safe(lhs);
  const double b = to_num_safe(rhs);
  if(a == b) return 0;
  return (a < b) ? -1 : 1;
}

// Allocate a nested array based on a vector of size expressions.
// sizes: list of dimension sizes expressions, evaluated at each depth.
// depth: current dimension index.
static Value make_array_from_sizes(Env& env, FunctionRegistry& R,
                                   const std::vector<std::unique_ptr<Expr>>& sizes,
                                   size_t depth){
  long long nll = 0;
  if(depth < sizes.size()){
    nll = eval(env, R, sizes[depth].get()).as_int();
  }
  if(nll < 0) nll = 0;
  size_t n = static_cast<size_t>(nll);
  Value::Array arr;
  arr.resize(n, Value::nil());
  if(depth + 1 < sizes.size()){
    for(size_t i = 0; i < arr.size(); ++i){
      arr[i] = make_array_from_sizes(env, R, sizes, depth + 1);
    }
  }
  return Value::from_array(std::move(arr));
}

static Value eval(Env& env, FunctionRegistry& R, const Expr* e){
  if(auto lit = dynamic_cast<const Literal*>(e)){
    switch(lit->tok.kind){
      case Tok::Number: return Value::from_number(std::stod(lit->tok.lex));
      case Tok::String: return Value::from_string(lit->tok.lex);
      case Tok::True: return Value::from_bool(true);
      case Tok::False: return Value::from_bool(false);
      default: return Value::nil();
    }
  }
  if(auto v = dynamic_cast<const Variable*>(e)){
    return env.get(v->name);
  }
  if(auto u = dynamic_cast<const Unary*>(e)){
    Value r = eval(env, R, u->right.get());
    if(u->op==Tok::Minus) return Value::from_number(-to_num(r));
    if(u->op==Tok::Plus) return Value::from_number(+to_num(r));
    if(u->op==Tok::Not) return Value::from_bool(!truthy(r));
    throw std::runtime_error("invalid unary op");
  }
  if(auto b = dynamic_cast<const Binary*>(e)){
    Value L = eval(env, R, b->left.get());
    Value Rv = eval(env, R, b->right.get());
    switch(b->op){
      case Tok::Plus: {
        const bool ls = std::holds_alternative<std::string>(L.v);
        const bool rs = std::holds_alternative<std::string>(Rv.v);
        if(ls || rs){
          const std::string sa = ls ? std::get<std::string>(L.v) : value_to_string(L);
          const std::string sb = rs ? std::get<std::string>(Rv.v) : value_to_string(Rv);
          return Value::from_string(sa + sb);
        }
        return Value::from_number(to_num(L)+to_num(Rv));
      }
      case Tok::Minus: return Value::from_number(to_num(L)-to_num(Rv));
      case Tok::Star: return Value::from_number(to_num(L)*to_num(Rv));
      case Tok::Slash: return Value::from_number(to_num(L)/to_num(Rv));
      case Tok::Mod: {
        double rhs = to_num(Rv);
        if(rhs == 0.0) throw std::runtime_error("Modulo by zero");
        return Value::from_number(std::fmod(to_num(L), rhs));
      }
      case Tok::Eq:  return Value::from_bool(cmp_values(L,Rv) == 0);
      case Tok::Neq: return Value::from_bool(cmp_values(L,Rv) != 0);
      case Tok::Lt:  return Value::from_bool(cmp_values(L,Rv) <  0);
      case Tok::Lte: return Value::from_bool(cmp_values(L,Rv) <= 0);
      case Tok::Gt:  return Value::from_bool(cmp_values(L,Rv) >  0);
      case Tok::Gte: return Value::from_bool(cmp_values(L,Rv) >= 0);
      case Tok::And: return Value::from_bool(truthy(L) && truthy(Rv));
      case Tok::Or:  return Value::from_bool(truthy(L) || truthy(Rv));
      default: break;
    }
    throw std::runtime_error("invalid binary op");
  }
  if(auto arr = dynamic_cast<const ArrayLiteral*>(e)){
    Value::Array a; a.reserve(arr->elements.size());
    for(auto& el : arr->elements) a.push_back(eval(env,R,el.get()));
    return Value::from_array(std::move(a));
  }
  if(auto idx = dynamic_cast<const Index*>(e)){
    Value base = eval(env,R,idx->target.get());
    long long i = eval(env,R,idx->index.get()).as_int();
    auto const& arrRef = base.as_array();
    if(i < 0 || (size_t)i >= arrRef.size()) return Value::nil();
    return arrRef[(size_t)i];
  }
  if(auto c = dynamic_cast<const Call*>(e)){
    std::vector<Value> args; args.reserve(c->args.size());
    for(auto& a : c->args) args.push_back(eval(env, R, a.get()));
    // Check user-defined subs first
    auto it = g_subs.find(Env::up(c->callee));
    if(it!=g_subs.end()){
      call_sub(env, R, c->callee, args);
      return Value::nil();
    }
    auto itf = g_funcs.find(Env::up(c->callee));
    if(itf!=g_funcs.end()){
      return call_func(env, R, c->callee, args);
    }
    return call(R, c->callee, args);
  }
  throw std::runtime_error("unknown expr");
}

static void exec(Env& env, FunctionRegistry& R, const Stmt* s){
  if(auto p = dynamic_cast<const Print*>(s)){
    Value v = eval(env,R,p->value.get());
    call(R, "PRINT", {v});
    return;
  }
  if(auto l = dynamic_cast<const Let*>(s)){
    env.set(l->name, eval(env,R,l->value.get())); return;
  }
  if(auto cd = dynamic_cast<const ConstDecl*>(s)){
    Value v = eval(env,R,cd->value.get());
    env.define_const(cd->name, std::move(v));
    return;
  }
  if(auto a = dynamic_cast<const Assign*>(s)){
    env.set(a->name, eval(env,R,a->value.get())); return;
  }
  if(auto e = dynamic_cast<const ExprStmt*>(s)){
    (void)eval(env,R,e->expr.get()); return;
  }
  if(auto c = dynamic_cast<const CallStmt*>(s)){
    std::vector<Value> args; for(auto& x:c->args) args.push_back(eval(env,R,x.get()));
    auto it = g_subs.find(Env::up(c->name));
    if(it!=g_subs.end()) { call_sub(env,R,c->name,args); return; }
    auto itf = g_funcs.find(Env::up(c->name));
    if(itf!=g_funcs.end()) { (void)call_func(env,R,c->name,args); return; }
    (void)call(R,c->name,args); return;
  }
  if(auto w = dynamic_cast<const WhileWend*>(s)){
    while(truthy(eval(env,R,w->cond.get()))){
      try {
        for(auto& ss : w->body) exec(env,R,ss.get());
      } catch(const BreakSignal&) {
        break; // Exit the loop
      } catch(const ContinueSignal&) {
        // Continue to next iteration
      }
    }
    return;
  }
  if(auto f = dynamic_cast<const ForNext*>(s)){
    // Initialize loop variable
    double init = eval(env,R,f->init.get()).as_number();
    double limit = eval(env,R,f->limit.get()).as_number();
    double step = f->step ? eval(env,R,f->step.get()).as_number() : 1.0;
    env.set(f->var, Value::from_number(init));
    auto cond = [&](double v){ return step >= 0 ? (v <= limit) : (v >= limit); };
    while(cond(env.get(f->var).as_number())){
      try {
        for(auto& ss : f->body) exec(env,R,ss.get());
      } catch(const BreakSignal&) {
        break; // Exit the loop
      } catch(const ContinueSignal&) {
        // fallthrough to step update
      }
      double v = env.get(f->var).as_number();
      env.set(f->var, Value::from_number(v + step));
    }
    return;
  }
  if(auto d = dynamic_cast<const DoLoop*>(s)){
    while(true){
      try{
        for(auto& ss : d->body) exec(env,R,ss.get());
      } catch(const BreakSignal&){
        break;
      } catch(const ContinueSignal&){
        // continue next iteration
      }
    }
    return;
  }
  if(auto ru = dynamic_cast<const RepeatUntil*>(s)){
    while(true){
      try{
        for(auto& ss : ru->body) exec(env,R,ss.get());
      } catch(const BreakSignal&){
        break;
      } catch(const ContinueSignal&){
        // after continue, still evaluate condition for post-test semantics
      }
      if(truthy(eval(env,R,ru->cond.get()))) break;
    }
    return;
  }
  if(auto i = dynamic_cast<const IfThenEndIf*>(s)){
    if(truthy(eval(env,R,i->cond.get()))){ for(auto& ss:i->body) exec(env,R,ss.get()); }
    return;
  }
  if(auto ic = dynamic_cast<const IfChain*>(s)){
    bool ran = false;
    for(const auto& br : ic->branches){
      if(truthy(eval(env, R, br.cond.get()))){
        for(const auto& ss : br.body) exec(env, R, ss.get());
        ran = true;
        break;
      }
    }
    if(!ran && ic->hasElse){
      for(const auto& ss : ic->elseBody) exec(env, R, ss.get());
    }
    return;
  }
  if(auto r = dynamic_cast<const Return*>(s)){
    Value rv = r->value ? eval(env,R,r->value.get()) : Value::nil();
    throw ReturnSignal{rv};
  }
  if(dynamic_cast<const Break*>(s)){
    throw BreakSignal{};
  }
  if(dynamic_cast<const Continue*>(s)){
    throw ContinueSignal{};
  }
  if(auto d = dynamic_cast<const Dim*>(s)){
    Value v = make_array_from_sizes(env, R, d->sizes, 0);
    env.set(d->name, std::move(v));
    return;
  }
  if(auto rd = dynamic_cast<const Redim*>(s)){
    if(env.is_const(rd->name)){
      throw std::runtime_error("REDIM on constant '" + Env::up(rd->name) + "'");
    }
    if(rd->preserve){
      // Evaluate sizes once, construct new array and copy overlap
      std::vector<size_t> lens = eval_sizes_to_vec(env, R, rd->sizes);
      size_t total = 0;
      // Debug: print sizes and note PRESERVE path
      if(is_verbose()){
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
      if(is_verbose()) std::cerr << "[REDIM] PRESERVE total elements=" << total << std::endl;
      Value oldv = env.get(rd->name);
      Value newv;
      if(oldv.is_array()){
        if(is_verbose()) std::cerr << "[REDIM] copying overlap from existing array" << std::endl;
        newv = redim_preserve_copy(oldv, lens, 0);
      } else {
        if(is_verbose()) std::cerr << "[REDIM] variable not array; creating new with lens" << std::endl;
        newv = make_array_from_lenvec(lens, 0);
      }
      env.set(rd->name, std::move(newv));
    } else {
      // Reinitialize array to new sizes (no PRESERVE semantics)
      std::vector<size_t> lens = eval_sizes_to_vec(env, R, rd->sizes);
      size_t total = 0;
      if(is_verbose()){
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
      if(is_verbose()) std::cerr << "[REDIM] total elements=" << total << std::endl;
      // We still want to support expression re-evaluation semantics for non-preserve path;
      // however, we validated shape is reasonable using lens.
      Value v = make_array_from_sizes(env, R, rd->sizes, 0);
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
      long long idx = eval(env, R, ai->indices[d].get()).as_int();
      if(idx < 0) return; // ignore
      auto& arr = current->as_array();
      if((size_t)idx >= arr.size()) arr.resize((size_t)idx+1, Value::nil());
      if(!arr[(size_t)idx].is_array()) arr[(size_t)idx] = Value::from_array({});
      current = &arr[(size_t)idx];
    }
    // Set final element
    if(ai->indices.empty()) return; // nothing to do
    long long last = eval(env, R, ai->indices.back().get()).as_int();
    if(last < 0) return;
    auto& arr = current->as_array();
    if((size_t)last >= arr.size()) arr.resize((size_t)last+1, Value::nil());
    arr[(size_t)last] = eval(env, R, ai->value.get());
    env.set(ai->name, std::move(target));
    return;
  }
  if(auto sc = dynamic_cast<const SelectCaseStmt*>(s)){
    // Evaluate selector once
    Value sel = eval(env, R, sc->selector.get());
    // First pass: try non-ELSE branches
    for(const auto& br : sc->branches){
      if(br.isElse) continue;
      bool doRun = false;
      if(br.isRel){
        Value rv = eval(env, R, br.relExpr.get());
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
          Value cv = eval(env, R, vexpr.get());
          if(cmp_values(sel, cv) == 0){ doRun = true; break; }
        }
      }
      if(doRun){
        for(const auto& ss : br.body) exec(env, R, ss.get());
        return;
      }
    }
    // Second pass: ELSE
    for(const auto& br : sc->branches){
      if(br.isElse){
        for(const auto& ss : br.body) exec(env, R, ss.get());
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
}

static void call_sub(Env& caller, FunctionRegistry& R, const std::string& name, const std::vector<Value>& args){
  auto it = g_subs.find(Env::up(name));
  if(it==g_subs.end()) return;
  const SubDecl* sd = it->second;
  Env local; local.parent = &caller;
  // Bind parameters
  size_t n = std::min(sd->params.size(), args.size());
  for(size_t i=0;i<n;++i) local.set(sd->params[i], args[i]);
  try{
    for(auto& s : sd->body) exec(local, R, s.get());
  } catch(const ReturnSignal&){ /* swallow */ }
}

static Value call_func(Env& caller, FunctionRegistry& R, const std::string& name, const std::vector<Value>& args){
  auto it = g_funcs.find(Env::up(name));
  if(it==g_funcs.end()) return Value::nil();
  const FunctionDecl* fd = it->second;
  Env local; local.parent = &caller;
  size_t n = std::min(fd->params.size(), args.size());
  for(size_t i=0;i<n;++i) local.set(fd->params[i], args[i]);
  try{
    for(auto& s : fd->body) exec(local, R, s.get());
  } catch(const ReturnSignal& rs){ return rs.v; }
  return Value::nil();
}



} // namespace

int bas::interpret(const Program& prog, FunctionRegistry& R){
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
      exec(env, R, s.get());
    }
    return 0;
  } catch(const std::exception& e){
    std::cerr << "Runtime exception: " << e.what() << std::endl;
    return 1;
  }
}

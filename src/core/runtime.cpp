#include "bas/runtime.hpp"
#include <stdexcept>

using namespace bas;

[[nodiscard]] Value bas::call(FunctionRegistry& R, const std::string& name, const std::vector<Value>& args){
  const NativeFn* f = R.find(name);
  if(!f) throw std::runtime_error("Unknown function: "+name);
  if(f->arity>=0 && static_cast<int>(args.size())!=f->arity)
    throw std::runtime_error(name+": expected "+std::to_string(f->arity)+" args");
  return (*f)(args);
}

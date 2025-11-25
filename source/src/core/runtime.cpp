#include "bas/runtime.hpp"
#include <stdexcept>

using namespace bas;

namespace {
using namespace bas;

static std::vector<MemberReadHook>& member_read_hooks() {
  static std::vector<MemberReadHook> hooks;
  return hooks;
}

static std::vector<MemberWriteHook>& member_write_hooks() {
  static std::vector<MemberWriteHook> hooks;
  return hooks;
}
} // namespace

void bas::register_member_read_hook(MemberReadHook hook) {
  member_read_hooks().push_back(std::move(hook));
}

void bas::register_member_write_hook(MemberWriteHook hook) {
  member_write_hooks().push_back(std::move(hook));
}

std::optional<Value> bas::try_resolve_member(const Value& object, const std::string& member) {
  for (auto it = member_read_hooks().rbegin(); it != member_read_hooks().rend(); ++it) {
    if (auto result = (*it)(object, member)) {
      return result;
    }
  }
  return std::nullopt;
}

bool bas::try_assign_member(const Value& object, const std::string& member, const Value& value) {
  for (auto it = member_write_hooks().rbegin(); it != member_write_hooks().rend(); ++it) {
    if ((*it)(object, member, value)) {
      return true;
    }
  }
  return false;
}

[[nodiscard]] Value bas::call(FunctionRegistry& R, const std::string& name, const std::vector<Value>& args){
  const NativeFn* f = R.find(name);
  if(!f) throw std::runtime_error("Unknown function: "+name);
  if(f->arity>=0 && static_cast<int>(args.size())!=f->arity)
    throw std::runtime_error(name+": expected "+std::to_string(f->arity)+" args");
  return (*f)(args);
}

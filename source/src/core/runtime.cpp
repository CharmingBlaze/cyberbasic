#include "bas/runtime.hpp"
#include <stdexcept>
#include <cctype>

using namespace bas;

// Helper: Normalize identifier to lowercase for case-insensitive matching
static std::string normalize_identifier(const std::string& s) {
  std::string r;
  r.reserve(s.size());
  for (char c : s) {
    r.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
  }
  return r;
}

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
  // Normalize member name for case-insensitive matching
  std::string normalized = normalize_identifier(member);
  for (auto it = member_read_hooks().rbegin(); it != member_read_hooks().rend(); ++it) {
    if (auto result = (*it)(object, normalized)) {
      return result;
    }
  }
  return std::nullopt;
}

bool bas::try_assign_member(const Value& object, const std::string& member, const Value& value) {
  // Normalize member name for case-insensitive matching
  std::string normalized = normalize_identifier(member);
  for (auto it = member_write_hooks().rbegin(); it != member_write_hooks().rend(); ++it) {
    if ((*it)(object, normalized, value)) {
      return true;
    }
  }
  return false;
}

[[nodiscard]] Value bas::call(FunctionRegistry& R, const std::string& name, const std::vector<Value>& args){
  // Normalize function name for case-insensitive lookup
  // (FunctionRegistry::find() also normalizes, but we normalize here for consistency and clarity)
  std::string normalized = normalize_identifier(name);
  const NativeFn* f = R.find(normalized);
  if(!f) throw std::runtime_error("Unknown function: "+name);
  if(f->arity>=0 && static_cast<int>(args.size())!=f->arity)
    throw std::runtime_error(name+": expected "+std::to_string(f->arity)+" args");
  return (*f)(args);
}

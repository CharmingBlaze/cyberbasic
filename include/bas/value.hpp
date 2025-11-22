#pragma once
#include <variant>
#include <string>
#include <stdexcept>
#include <vector>
#include <map>

namespace bas {
// Dynamically-typed value used by the interpreter.
struct Value {
  using Array = std::vector<Value>;
  using Map = std::map<std::string, Value>;
  using V = std::variant<std::monostate, double, long long, bool, std::string, Array, Map>;
  V v;
  [[nodiscard]] static Value nil() noexcept { return Value{std::monostate{}}; }
  [[nodiscard]] static Value from_number(double d) noexcept { return Value{d}; }
  [[nodiscard]] static Value from_int(long long i) noexcept { return Value{i}; }
  [[nodiscard]] static Value from_bool(bool b) noexcept { return Value{b}; }
  [[nodiscard]] static Value from_string(std::string s) { return Value{std::move(s)}; }
  [[nodiscard]] static Value from_array(Array a) { return Value{std::move(a)}; }
  [[nodiscard]] static Value from_map(Map m) { return Value{std::move(m)}; }

  [[nodiscard]] constexpr bool is_nil() const noexcept { return std::holds_alternative<std::monostate>(v); }
  [[nodiscard]] constexpr bool is_string() const noexcept { return std::holds_alternative<std::string>(v); }
  [[nodiscard]] constexpr bool is_bool() const noexcept { return std::holds_alternative<bool>(v); }
  [[nodiscard]] constexpr bool is_int() const noexcept { return std::holds_alternative<long long>(v); }
  [[nodiscard]] constexpr bool is_number() const noexcept {
    return std::holds_alternative<double>(v) || std::holds_alternative<long long>(v);
  }
  [[nodiscard]] double as_number() const {
    if (auto p=std::get_if<double>(&v)) return *p;
    if (auto q=std::get_if<long long>(&v)) return static_cast<double>(*q);
    throw std::runtime_error("Expected number");
  }
  [[nodiscard]] long long as_int() const {
    if (auto q=std::get_if<long long>(&v)) return *q;
    if (auto p=std::get_if<double>(&v)) return static_cast<long long>(*p);
    throw std::runtime_error("Expected int");
  }
  [[nodiscard]] bool as_bool() const {
    if (auto p=std::get_if<bool>(&v)) return *p;
    if (auto d=std::get_if<double>(&v)) return *d!=0.0;
    if (auto i=std::get_if<long long>(&v)) return *i!=0;
    if (auto s=std::get_if<std::string>(&v)) return !s->empty();
    return false;
  }
  [[nodiscard]] const std::string& as_string() const {
    if (auto s=std::get_if<std::string>(&v)) return *s;
    throw std::runtime_error("Expected string");
  }
  [[nodiscard]] constexpr bool is_array() const noexcept { return std::holds_alternative<Array>(v); }
  [[nodiscard]] const Array& as_array() const {
    if (auto a=std::get_if<Array>(&v)) return *a;
    throw std::runtime_error("Expected array");
  }
  [[nodiscard]] Array& as_array() {
    if (auto a=std::get_if<Array>(&v)) return *a;
    throw std::runtime_error("Expected array");
  }
  [[nodiscard]] constexpr bool is_map() const noexcept { return std::holds_alternative<Map>(v); }
  [[nodiscard]] const Map& as_map() const {
    if (auto m=std::get_if<Map>(&v)) return *m;
    throw std::runtime_error("Expected map");
  }
  [[nodiscard]] Map& as_map() {
    if (auto m=std::get_if<Map>(&v)) return *m;
    throw std::runtime_error("Expected map");
  }
};
} // namespace bas

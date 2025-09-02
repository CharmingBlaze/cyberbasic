#pragma once
#include <variant>
#include <string>
#include <stdexcept>
#include <vector>

namespace bas {
// Dynamically-typed value used by the interpreter.
struct Value {
  using Array = std::vector<Value>;
  using V = std::variant<std::monostate, double, long long, bool, std::string, Array>;
  V v;
  static Value nil(){ return Value{std::monostate{}}; }
  static Value from_number(double d){ return Value{d}; }
  static Value from_int(long long i){ return Value{i}; }
  static Value from_bool(bool b){ return Value{b}; }
  static Value from_string(std::string s){ return Value{std::move(s)}; }
  static Value from_array(Array a){ return Value{std::move(a)}; }

  bool is_nil() const { return std::holds_alternative<std::monostate>(v); }
  double as_number() const {
    if (auto p=std::get_if<double>(&v)) return *p;
    if (auto q=std::get_if<long long>(&v)) return static_cast<double>(*q);
    throw std::runtime_error("Expected number");
  }
  long long as_int() const {
    if (auto q=std::get_if<long long>(&v)) return *q;
    if (auto p=std::get_if<double>(&v)) return static_cast<long long>(*p);
    throw std::runtime_error("Expected int");
  }
  bool as_bool() const {
    if (auto p=std::get_if<bool>(&v)) return *p;
    if (auto d=std::get_if<double>(&v)) return *d!=0.0;
    if (auto i=std::get_if<long long>(&v)) return *i!=0;
    if (auto s=std::get_if<std::string>(&v)) return !s->empty();
    return false;
  }
  const std::string& as_string() const {
    if (auto s=std::get_if<std::string>(&v)) return *s;
    throw std::runtime_error("Expected string");
  }
  bool is_array() const { return std::holds_alternative<Array>(v); }
  const Array& as_array() const {
    if (auto a=std::get_if<Array>(&v)) return *a;
    throw std::runtime_error("Expected array");
  }
  Array& as_array() {
    if (auto a=std::get_if<Array>(&v)) return *a;
    throw std::runtime_error("Expected array");
  }
};
} // namespace bas

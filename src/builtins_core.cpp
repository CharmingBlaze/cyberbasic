#include "bas/builtins.hpp"
#include "bas/input.hpp"
#include <iostream>
#include <random>
#include <chrono>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <thread>
#include <locale>
#include <cmath>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <map>
#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

using namespace bas;

static std::string to_string_value(const Value& v){
  if(auto s = std::get_if<std::string>(&v.v)) return *s;
  if(auto b = std::get_if<bool>(&v.v)) return *b?"TRUE":"FALSE";
  if(auto d = std::get_if<double>(&v.v)) { std::ostringstream os; os.imbue(std::locale::classic()); os<<*d; return os.str(); }
  if(auto i = std::get_if<long long>(&v.v)) { std::ostringstream os; os.imbue(std::locale::classic()); os<<*i; return os.str(); }
  return std::string{};
}

// Forward declarations for category registrars implemented in other compilation units
void register_builtins_console(FunctionRegistry&);
void register_builtins_graphics(FunctionRegistry&);
void register_builtins_audio(FunctionRegistry&);

void bas::register_builtins(FunctionRegistry& R){
  // Initialize input subsystem (safe to call multiple times)
  bas::input::init();

  // Core I/O

  R.add("INPUT", NativeFn{"INPUT", -1, [](const std::vector<Value>& a){
    if(!a.empty()) std::cout << to_string_value(a[0]);
    std::string line;
    std::getline(std::cin, line);
    return Value::from_string(line);
  }});

  // String helpers
  R.add("STR", NativeFn{"STR", 1, [](const std::vector<Value>& a){
    return Value::from_string(to_string_value(a[0]));
  }});

  R.add("VAL", NativeFn{"VAL", 1, [](const std::vector<Value>& a){
    if(std::holds_alternative<double>(a[0].v)) return a[0];
    if(std::holds_alternative<long long>(a[0].v)) return Value::from_number(static_cast<double>(std::get<long long>(a[0].v)));
    if(std::holds_alternative<std::string>(a[0].v)) return Value::from_number(std::stod(std::get<std::string>(a[0].v)));
    if(std::holds_alternative<bool>(a[0].v)) return Value::from_number(std::get<bool>(a[0].v)?1.0:0.0);
    return Value::from_number(0.0);
  }});

  R.add("LEN", NativeFn{"LEN", 1, [](const std::vector<Value>& a){
    if (std::holds_alternative<std::string>(a[0].v)) {
      return Value::from_int(static_cast<long long>(a[0].as_string().size()));
    }
    if (a[0].is_array()) {
      const auto& arr = a[0].as_array();
      return Value::from_int(static_cast<long long>(arr.size()));
    }
    return Value::from_int(0);
  }});

  R.add("SUBSTR", NativeFn{"SUBSTR", 3, [](const std::vector<Value>& a){
    const std::string& s = a[0].as_string();
    long long start = a[1].as_int();
    long long len = a[2].as_int();
    if(start < 0) start = 0;
    if(start > static_cast<long long>(s.size())) start = static_cast<long long>(s.size());
    long long end = start + len;
    if(end > static_cast<long long>(s.size())) end = static_cast<long long>(s.size());
    return Value::from_string(s.substr(static_cast<size_t>(start), static_cast<size_t>(end-start)));
  }});

  // Random/time
  R.add("RANDOM", NativeFn{"RANDOM", -1, [](const std::vector<Value>& a){
    static std::mt19937_64 eng{std::random_device{}()};
    if(a.empty()){
      std::uniform_real_distribution<double> dist(0.0,1.0); return Value::from_number(dist(eng));
    }
    long long n = a[0].as_int(); if(n<=0) return Value::from_int(0);
    std::uniform_int_distribution<long long> dist(0,n-1); return Value::from_int(dist(eng));
  }});
  R.add("RND", NativeFn{"RND", -1, [](const std::vector<Value>& a){
    static std::mt19937_64 eng{std::random_device{}()};
    if(a.empty()){
      std::uniform_real_distribution<double> dist(0.0,1.0); return Value::from_number(dist(eng));
    }
    long long n = a[0].as_int(); if(n<=0) return Value::from_int(0);
    std::uniform_int_distribution<long long> dist(0,n-1); return Value::from_int(dist(eng));
  }});

  R.add("RNDINT", NativeFn{"RNDINT", 2, [](const std::vector<Value>& a){
      static std::mt19937_64 eng{std::random_device{}()};
      long long lo = a[0].as_int();
      long long hi = a[1].as_int();
      if (lo > hi) std::swap(lo, hi);
      std::uniform_int_distribution<long long> dist(lo, hi);
      return Value::from_int(dist(eng));
  }});

  R.add("SEED", NativeFn{"SEED", 1, [](const std::vector<Value>& a){
    static std::mt19937_64 eng; eng.seed(static_cast<uint64_t>(a[0].as_int())); return Value::nil();
  }});
  R.add("RANDOMIZE", NativeFn{"RANDOMIZE", 1, [](const std::vector<Value>& a){
    static std::mt19937_64 eng; eng.seed(static_cast<uint64_t>(a[0].as_int())); return Value::nil();
  }});

  R.add("TIME", NativeFn{"TIME", 0, [](const std::vector<Value>&){
    using clock = std::chrono::system_clock;
    auto now = clock::now().time_since_epoch();
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(now).count();
    return Value::from_int(static_cast<long long>(secs));
  }});

  // Assertions
  R.add("ASSERT", NativeFn{"ASSERT", 1, [](const std::vector<Value>& a){
    if(!a[0].as_bool()) throw std::runtime_error("ASSERT failed");
    return Value::nil();
  }});

  // Arrays (immutable ops returning new arrays)
  R.add("ARRAY", NativeFn{"ARRAY", 1, [](const std::vector<Value>& a){
    long long nll = a[0].as_int();
    if(nll < 0) nll = 0;
    size_t n = static_cast<size_t>(nll);
    bas::Value::Array arr; arr.resize(n, Value::nil());
    return Value::from_array(std::move(arr));
  }});

  R.add("PUSH", NativeFn{"PUSH", 2, [](const std::vector<Value>& a){
    if(!a[0].is_array()) throw std::runtime_error("PUSH: first argument must be an array");
    bas::Value::Array copy = a[0].as_array();
    copy.push_back(a[1]);
    return Value::from_array(std::move(copy));
  }});

  R.add("APPEND", NativeFn{"APPEND", 2, [](const std::vector<Value>& a){
    if(!a[0].is_array()) throw std::runtime_error("APPEND: first argument must be an array");
    bas::Value::Array copy = a[0].as_array();
    copy.push_back(a[1]);
    return Value::from_array(std::move(copy));
  }});

  R.add("POP", NativeFn{"POP", 1, [](const std::vector<Value>& a){
    if(!a[0].is_array()) throw std::runtime_error("POP: argument must be an array");
    bas::Value::Array copy = a[0].as_array();
    if(!copy.empty()) copy.pop_back();
    return Value::from_array(std::move(copy));
  }});

  R.add("INSERT", NativeFn{"INSERT", 3, [](const std::vector<Value>& a){
    if(!a[0].is_array()) throw std::runtime_error("INSERT: first argument must be an array");
    bas::Value::Array copy = a[0].as_array();
    long long idx = a[1].as_int();
    if(idx < 0) idx = 0;
    if((size_t)idx > copy.size()) idx = (long long)copy.size();
    copy.insert(copy.begin() + (size_t)idx, a[2]);
    return Value::from_array(std::move(copy));
  }});

  R.add("REMOVE", NativeFn{"REMOVE", 2, [](const std::vector<Value>& a){
    if(!a[0].is_array()) throw std::runtime_error("REMOVE: first arg must be an array");
    auto arr = a[0].as_array();
    long long idx = a[1].as_int();
    if(idx < 0 || idx >= static_cast<long long>(arr.size())) throw std::runtime_error("REMOVE: index out of bounds");
    arr.erase(arr.begin() + idx);
    return Value::from_array(arr);
  }});

  R.add("SORT", NativeFn{"SORT", 1, [](const std::vector<Value>& a){
    if (!a[0].is_array()) throw std::runtime_error("SORT: argument must be an array");
    auto arr = a[0].as_array();
    if (arr.empty()) return Value::from_array(arr);

    std::sort(arr.begin(), arr.end(), [](const Value& v1, const Value& v2) {
        bool v1_is_num = std::holds_alternative<double>(v1.v) || std::holds_alternative<long long>(v1.v);
        bool v2_is_num = std::holds_alternative<double>(v2.v) || std::holds_alternative<long long>(v2.v);
        bool v1_is_str = std::holds_alternative<std::string>(v1.v);
        bool v2_is_str = std::holds_alternative<std::string>(v2.v);

        if (v1_is_num && v2_is_num) {
            return v1.as_number() < v2.as_number();
        } else if (v1_is_str && v2_is_str) {
            return v1.as_string() < v2.as_string();
        } else {
            throw std::runtime_error("SORT: all array elements must be of the same comparable type (all numbers or all strings)");
        }
    });

    return Value::from_array(arr);
  }});

  // Map functions
  R.add("MAP_CREATE", NativeFn{"MAP_CREATE", 0, [](const std::vector<Value>&){
    return Value::from_map({});
  }});

  R.add("MAP_SET", NativeFn{"MAP_SET", 3, [](const std::vector<Value>& a){
    if (!a[0].is_map()) throw std::runtime_error("MAP_SET: first argument must be a map");
    auto map = a[0].as_map(); // copy
    map[a[1].as_string()] = a[2];
    return Value::from_map(map);
  }});

  R.add("MAP_GET", NativeFn{"MAP_GET", -1, [](const std::vector<Value>& a){
    if (a.size() < 2) throw std::runtime_error("MAP_GET: requires at least 2 arguments");
    if (!a[0].is_map()) throw std::runtime_error("MAP_GET: first argument must be a map");
    const auto& map = a[0].as_map();
    const auto& key = a[1].as_string();
    auto it = map.find(key);
    if (it != map.end()) {
        return it->second;
    }
    if (a.size() >= 3) {
        return a[2]; // default value
    }
    throw std::runtime_error("MAP_GET: key not found and no default value provided");
  }});

  R.add("MAP_HAS", NativeFn{"MAP_HAS", 2, [](const std::vector<Value>& a){
    if (!a[0].is_map()) throw std::runtime_error("MAP_HAS: first argument must be a map");
    const auto& map = a[0].as_map();
    return Value::from_bool(map.count(a[1].as_string()) > 0);
  }});

  R.add("MAP_REMOVE", NativeFn{"MAP_REMOVE", 2, [](const std::vector<Value>& a){
    if (!a[0].is_map()) throw std::runtime_error("MAP_REMOVE: first argument must be a map");
    auto map = a[0].as_map(); // copy
    map.erase(a[1].as_string());
    return Value::from_map(map);
  }});

  R.add("MAP_KEYS", NativeFn{"MAP_KEYS", 1, [](const std::vector<Value>& a){
    if (!a[0].is_map()) throw std::runtime_error("MAP_KEYS: argument must be a map");
    const auto& map = a[0].as_map();
    std::vector<Value> keys;
    keys.reserve(map.size());
    for(const auto& pair : map) {
        keys.push_back(Value::from_string(pair.first));
    }
    return Value::from_array(keys);
  }});

  R.add("MAP_VALUES", NativeFn{"MAP_VALUES", 1, [](const std::vector<Value>& a){
    if (!a[0].is_map()) throw std::runtime_error("MAP_VALUES: argument must be a map");
    const auto& map = a[0].as_map();
    std::vector<Value> values;
    values.reserve(map.size());
    for(const auto& pair : map) {
        values.push_back(pair.second);
    }
    return Value::from_array(values);
  }});

  // File I/O
  R.add("FILEEXISTS", NativeFn{"FILEEXISTS", 1, [](const std::vector<Value>& a){
    std::string filename = a[0].as_string();
    std::ifstream file(filename);
    return Value::from_bool(file.good());
  }});

  R.add("FILESIZE", NativeFn{"FILESIZE", 1, [](const std::vector<Value>& a){
    std::string filename = a[0].as_string();
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if(!file.is_open()) return Value::from_number(0.0);
    return Value::from_number(static_cast<double>(file.tellg()));
  }});

  R.add("READALL", NativeFn{"READALL", 1, [](const std::vector<Value>& a){
    std::ifstream f(a[0].as_string(), std::ios::binary);
    if(!f.is_open()) return Value::from_string("");
    std::ostringstream ss; ss << f.rdbuf();
    return Value::from_string(ss.str());
  }});

  R.add("WRITEALL", NativeFn{"WRITEALL", 2, [](const std::vector<Value>& a){
    std::ofstream f(a[0].as_string(), std::ios::binary);
    if(!f.is_open()) return Value::from_bool(false);
    const std::string& data = a[1].as_string();
    f.write(data.data(), static_cast<std::streamsize>(data.size()));
    return Value::from_bool(static_cast<bool>(f));
  }});

  // String manipulation
  R.add("LEFT", NativeFn{"LEFT", 2, [](const std::vector<Value>& a){
    std::string str = a[0].as_string();
    int count = static_cast<int>(a[1].as_number());
    if(count <= 0) return Value::from_string("");
    if(count >= static_cast<int>(str.length())) return Value::from_string(str);
    return Value::from_string(str.substr(0, count));
  }});

  R.add("RIGHT", NativeFn{"RIGHT", 2, [](const std::vector<Value>& a){
    std::string str = a[0].as_string();
    int count = static_cast<int>(a[1].as_number());
    if(count <= 0) return Value::from_string("");
    if(count >= static_cast<int>(str.length())) return Value::from_string(str);
    return Value::from_string(str.substr(str.length() - count));
  }});

  R.add("MID", NativeFn{"MID", 3, [](const std::vector<Value>& a){
    std::string str = a[0].as_string();
    int start = static_cast<int>(a[1].as_number());
    int count = static_cast<int>(a[2].as_number());
    if(start < 0 || count <= 0) return Value::from_string("");
    if(start >= static_cast<int>(str.length())) return Value::from_string("");
    size_t startPos = static_cast<size_t>(start);
    size_t endPos = std::min(startPos + static_cast<size_t>(count), str.length());
    return Value::from_string(str.substr(startPos, endPos - startPos));
  }});

  R.add("UCASE", NativeFn{"UCASE", 1, [](const std::vector<Value>& a){
    std::string str = a[0].as_string();
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return Value::from_string(str);
  }});

  R.add("LCASE", NativeFn{"LCASE", 1, [](const std::vector<Value>& a){
    std::string str = a[0].as_string();
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return Value::from_string(str);
  }});

  R.add("LTRIM", NativeFn{"LTRIM", 1, [](const std::vector<Value>& a){
    const std::string& s = a[0].as_string();
    size_t i = 0; while(i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    return Value::from_string(s.substr(i));
  }});

  R.add("RTRIM", NativeFn{"RTRIM", 1, [](const std::vector<Value>& a){
    const std::string& s = a[0].as_string();
    if(s.empty()) return Value::from_string("");
    size_t j = s.size(); while(j > 0 && std::isspace(static_cast<unsigned char>(s[j-1]))) --j;
    return Value::from_string(s.substr(0, j));
  }});

  R.add("TRIM", NativeFn{"TRIM", 1, [](const std::vector<Value>& a){
    const std::string& s = a[0].as_string();
    size_t i = 0; while(i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    size_t j = s.size(); while(j > i && std::isspace(static_cast<unsigned char>(s[j-1]))) --j;
    return Value::from_string(s.substr(i, j - i));
  }});

  R.add("INSTR", NativeFn{"INSTR", -1, [](const std::vector<Value>& a){
    if(a.size() < 2) throw std::runtime_error("INSTR: need at least 2 args");
    const std::string& s = a[0].as_string();
    const std::string& sub = a[1].as_string();
    long long start = 1;
    if(a.size() >= 3) start = a[2].as_int();
    if(start < 1) start = 1;
    size_t pos = s.find(sub, static_cast<size_t>(start - 1));
    if(pos == std::string::npos) return Value::from_int(0);
    return Value::from_int(static_cast<long long>(pos + 1));
  }});

  R.add("REPLACE", NativeFn{"REPLACE", 3, [](const std::vector<Value>& a){
    std::string str = a[0].as_string();
    const std::string& from = a[1].as_string();
    const std::string& to = a[2].as_string();
    if (from.empty()) return Value::from_string(str);
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return Value::from_string(str);
  }});

  R.add("CHR", NativeFn{"CHR", 1, [](const std::vector<Value>& a){
    long long code = a[0].as_int();
    return Value::from_string(std::string(1, static_cast<char>(code)));
  }});

  R.add("ASC", NativeFn{"ASC", 1, [](const std::vector<Value>& a){
    const std::string& s = a[0].as_string();
    if (s.empty()) return Value::from_int(0);
    return Value::from_int(static_cast<unsigned char>(s[0]));
  }});

  R.add("SPLIT", NativeFn{"SPLIT", 2, [](const std::vector<Value>& a){
    const std::string& str = a[0].as_string();
    const std::string& delim = a[1].as_string();
    std::vector<Value> result;
    if (delim.empty()) {
        for (char c : str) {
            result.push_back(Value::from_string(std::string(1, c)));
        }
    } else {
        size_t start = 0;
        size_t end = str.find(delim);
        while (end != std::string::npos) {
            result.push_back(Value::from_string(str.substr(start, end - start)));
            start = end + delim.length();
            end = str.find(delim, start);
        }
        result.push_back(Value::from_string(str.substr(start)));
    }
    return Value::from_array(result);
  }});

  R.add("JOIN", NativeFn{"JOIN", 2, [](const std::vector<Value>& a){
      if (!a[0].is_array()) throw std::runtime_error("JOIN: first argument must be an array");
      const auto& arr = a[0].as_array();
      const std::string& delim = a[1].as_string();
      std::stringstream ss;
      for (size_t i = 0; i < arr.size(); ++i) {
          ss << to_string_value(arr[i]);
          if (i < arr.size() - 1) {
              ss << delim;
          }
      }
      return Value::from_string(ss.str());
  }});

  R.add("FORMAT", NativeFn{"FORMAT", -1, [](const std::vector<Value>& a){
    if (a.empty()) throw std::runtime_error("FORMAT: requires a format string");
    const std::string& fmt = a[0].as_string();
    std::stringstream result;
    size_t arg_idx = 1;

    for (size_t i = 0; i < fmt.length(); ++i) {
        if (fmt[i] == '%') {
            if (i + 1 < fmt.length()) {
                i++; // move to specifier
                if (fmt[i] == '%') {
                    result << '%';
                } else {
                    if (arg_idx >= a.size()) {
                        throw std::runtime_error("FORMAT: not enough arguments for format string");
                    }
                    const auto& val = a[arg_idx++];
                    char buffer[512];
                    switch (fmt[i]) {
                        case 's':
                            snprintf(buffer, sizeof(buffer), "%s", to_string_value(val).c_str());
                            result << buffer;
                            break;
                        case 'd':
                        case 'i':
                            snprintf(buffer, sizeof(buffer), "%lld", val.as_int());
                            result << buffer;
                            break;
                        case 'f':
                            snprintf(buffer, sizeof(buffer), "%f", val.as_number());
                            result << buffer;
                            break;
                        default: // just print the char if specifier is unknown
                            result << '%' << fmt[i];
                            break;
                    }
                }
            } else {
                result << '%' ; // trailing %
            }
        } else {
            result << fmt[i];
        }
    }
    return Value::from_string(result.str());
  }});

  // Math & system
  R.add("SGN", NativeFn{"SGN", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    return Value::from_number((num > 0) - (num < 0));
  }});

  R.add_with_policy("MIN", NativeFn{"MIN", 2, [](const std::vector<Value>& a){
    double n1 = a[0].as_number();
    double n2 = a[1].as_number();
    return Value::from_number(std::min(n1, n2));
  }}, true);

  R.add_with_policy("MAX", NativeFn{"MAX", 2, [](const std::vector<Value>& a){
    double n1 = a[0].as_number();
    double n2 = a[1].as_number();
    return Value::from_number(std::max(n1, n2));
  }}, true);

  R.add_with_policy("CLAMP", NativeFn{"CLAMP", 3, [](const std::vector<Value>& a){
    double val = a[0].as_number();
    double min_val = a[1].as_number();
    double max_val = a[2].as_number();
    return Value::from_number(std::max(min_val, std::min(val, max_val)));
  }}, true);

  R.add("ABS", NativeFn{"ABS", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    return Value::from_number(std::abs(num));
  }});
  R.add("SIN", NativeFn{"SIN", 1, [](const std::vector<Value>& a){
    double angle = a[0].as_number();
    return Value::from_number(std::sin(angle));
  }});
  R.add("COS", NativeFn{"COS", 1, [](const std::vector<Value>& a){
    double angle = a[0].as_number();
    return Value::from_number(std::cos(angle));
  }});
  R.add("TAN", NativeFn{"TAN", 1, [](const std::vector<Value>& a){
    double angle = a[0].as_number();
    return Value::from_number(std::tan(angle));
  }});
  R.add("ATAN", NativeFn{"ATAN", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    return Value::from_number(std::atan(num));
  }});
  R.add("ASIN", NativeFn{"ASIN", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    if (num < -1.0 || num > 1.0) throw std::runtime_error("ASIN: argument out of range [-1, 1]");
    return Value::from_number(std::asin(num));
  }});
  R.add("ACOS", NativeFn{"ACOS", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    if (num < -1.0 || num > 1.0) throw std::runtime_error("ACOS: argument out of range [-1, 1]");
    return Value::from_number(std::acos(num));
  }});
  R.add("ATAN2", NativeFn{"ATAN2", 2, [](const std::vector<Value>& a){
    double y = a[0].as_number();
    double x = a[1].as_number();
    return Value::from_number(std::atan2(y, x));
  }});
  R.add("SQRT", NativeFn{"SQRT", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    if(num < 0) throw std::runtime_error("SQRT: cannot take square root of negative number");
    return Value::from_number(std::sqrt(num));
  }});
  R.add("POW", NativeFn{"POW", 2, [](const std::vector<Value>& a){
    double base = a[0].as_number();
    double exponent = a[1].as_number();
    return Value::from_number(std::pow(base, exponent));
  }});
  R.add("LOG", NativeFn{"LOG", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    if(num <= 0) throw std::runtime_error("LOG: cannot take log of non-positive number");
    return Value::from_number(std::log(num));
  }});
  R.add("EXP", NativeFn{"EXP", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    return Value::from_number(std::exp(num));
  }});
  R.add("INT", NativeFn{"INT", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    return Value::from_int(static_cast<long long>(std::floor(num)));
  }});
  R.add("FIX", NativeFn{"FIX", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    return Value::from_int(static_cast<long long>(num));
  }});
  R.add("ROUND", NativeFn{"ROUND", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    return Value::from_int(static_cast<long long>(std::round(num)));
  }});

  R.add_with_policy("LERP", NativeFn{"LERP", 3, [](const std::vector<Value>& a){
      double v0 = a[0].as_number();
      double v1 = a[1].as_number();
      double t = a[2].as_number();
      return Value::from_number(v0 + t * (v1 - v0));
  }}, true);

  R.add("SLEEP", NativeFn{"SLEEP", 1, [](const std::vector<Value>& a){
    double seconds = a[0].as_number();
    if(seconds > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(seconds * 1000)));
    }
    return Value::nil();
  }});
  
  // WAIT and DELAY are aliases for SLEEP (common BASIC commands)
  R.add("WAIT", NativeFn{"WAIT", 1, [](const std::vector<Value>& a){
    double seconds = a[0].as_number();
    if(seconds > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(seconds * 1000)));
    }
    return Value::nil();
  }});
  
  R.add("DELAY", NativeFn{"DELAY", 1, [](const std::vector<Value>& a){
    double seconds = a[0].as_number();
    if(seconds > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(seconds * 1000)));
    }
    return Value::nil();
  }});

  R.add("BEEP", NativeFn{"BEEP", 0, [](const std::vector<Value>&){
    std::cout << "\a" << std::flush;
    return Value::nil();
  }});

  // System functions
  R.add("SHELL", NativeFn{"SHELL", 1, [](const std::vector<Value>& a){
    std::string cmd = a[0].as_string();
    int result = std::system(cmd.c_str());
    return Value::from_int(static_cast<long long>(result));
  }});

  // Clipboard functions (platform-specific, simplified)
  R.add("CLIPBOARD_GET", NativeFn{"CLIPBOARD_GET", 0, [](const std::vector<Value>&){
    // Platform-specific clipboard access would go here
    // For now, return empty string
    return Value::from_string("");
  }});

  R.add("CLIPBOARD_SET", NativeFn{"CLIPBOARD_SET", 1, [](const std::vector<Value>& a){
    std::string text = a[0].as_string();
    // Platform-specific clipboard set would go here
    (void)text; // Suppress unused warning
    return Value::nil();
  }});

  // Environment variables
  R.add("ENV_GET", NativeFn{"ENV_GET", 1, [](const std::vector<Value>& a){
    std::string key = a[0].as_string();
    const char* val = std::getenv(key.c_str());
    return Value::from_string(val ? val : "");
  }});

  R.add("ENV_SET", NativeFn{"ENV_SET", 2, [](const std::vector<Value>& a){
    std::string key = a[0].as_string();
    std::string val = a[1].as_string();
#ifdef _WIN32
    _putenv_s(key.c_str(), val.c_str());
#else
    setenv(key.c_str(), val.c_str(), 1);
#endif
    return Value::nil();
  }});

  // File operations
  R.add("FILE_DELETE", NativeFn{"FILE_DELETE", 1, [](const std::vector<Value>& a){
    std::string path = a[0].as_string();
    std::remove(path.c_str());
    return Value::nil();
  }});

  R.add("FILE_COPY", NativeFn{"FILE_COPY", 2, [](const std::vector<Value>& a){
    std::string src = a[0].as_string();
    std::string dst = a[1].as_string();
    std::ifstream srcFile(src, std::ios::binary);
    std::ofstream dstFile(dst, std::ios::binary);
    if(srcFile && dstFile){
      dstFile << srcFile.rdbuf();
    }
    return Value::nil();
  }});

  // File handle operations (simple implementation using file paths)
  // Note: Full #n file handle support would require parser changes
  R.add("EOF", NativeFn{"EOF", 1, [](const std::vector<Value>& a){
    std::string path = a[0].as_string();
    std::ifstream file(path);
    if(!file.is_open()) return Value::from_bool(true);
    file.seekg(0, std::ios::end);
    std::streampos end = file.tellg();
    file.seekg(0, std::ios::cur);
    std::streampos cur = file.tellg();
    return Value::from_bool(cur >= end);
  }});

  R.add("LOF", NativeFn{"LOF", 1, [](const std::vector<Value>& a){
    std::string path = a[0].as_string();
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if(!file.is_open()) return Value::from_number(0.0);
    return Value::from_number(static_cast<double>(file.tellg()));
  }});

  R.add("SEEK", NativeFn{"SEEK", 2, [](const std::vector<Value>& a){
    std::string path = a[0].as_string();
    long long pos = a[1].as_int();
    // Note: This is a simplified implementation
    // Full file handle support would maintain open file streams
    return Value::nil();
  }});

  // Directory operations
  R.add("DIR_CREATE", NativeFn{"DIR_CREATE", 1, [](const std::vector<Value>& a){
    std::string path = a[0].as_string();
#ifdef _WIN32
    _mkdir(path.c_str());
#else
    mkdir(path.c_str(), 0755);
#endif
    return Value::nil();
  }});

  R.add("DIR_DELETE", NativeFn{"DIR_DELETE", 1, [](const std::vector<Value>& a){
    std::string path = a[0].as_string();
#ifdef _WIN32
    _rmdir(path.c_str());
#else
    rmdir(path.c_str());
#endif
    return Value::nil();
  }});

  R.add("DIR_LIST", NativeFn{"DIR_LIST", 1, [](const std::vector<Value>& a){
    std::string path = a[0].as_string();
    std::vector<Value> files;
    // Simplified - would need proper directory iteration
    return Value::from_array(files);
  }});

  R.add("CHDIR", NativeFn{"CHDIR", 1, [](const std::vector<Value>& a){
    std::string path = a[0].as_string();
    std::filesystem::current_path(path);
    return Value::nil();
  }});

  R.add("CURDIR", NativeFn{"CURDIR", 0, [](const std::vector<Value>&){
    return Value::from_string(std::filesystem::current_path().string());
  }});

  R.add("PATH_JOIN", NativeFn{"PATH_JOIN", 2, [](const std::vector<Value>& a){
    std::string a_path = a[0].as_string();
    std::string b_path = a[1].as_string();
    return Value::from_string((std::filesystem::path(a_path) / b_path).string());
  }});

  // Date/Time functions
  R.add("DATE_NOW", NativeFn{"DATE_NOW", 0, [](const std::vector<Value>&){
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
    return Value::from_string(buffer);
  }});

  R.add("UNIX_TIME", NativeFn{"UNIX_TIME", 0, [](const std::vector<Value>&){
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    return Value::from_int(static_cast<long long>(time));
  }});

  // Timer functions (simple implementation)
  static std::map<std::string, std::chrono::steady_clock::time_point> timers;
  R.add("TIMER_START", NativeFn{"TIMER_START", 1, [](const std::vector<Value>& a){
    std::string id = a[0].as_string();
    timers[id] = std::chrono::steady_clock::now();
    return Value::nil();
  }});

  R.add("TIMER_ELAPSED", NativeFn{"TIMER_ELAPSED", 1, [](const std::vector<Value>& a){
    std::string id = a[0].as_string();
    auto it = timers.find(id);
    if(it == timers.end()) return Value::from_number(0.0);
    auto elapsed = std::chrono::steady_clock::now() - it->second;
    auto seconds = std::chrono::duration<double>(elapsed).count();
    return Value::from_number(seconds);
  }});

  // Delegate to other categories
  register_builtins_console(R);
  register_builtins_graphics(R);
  register_builtins_audio(R);
}

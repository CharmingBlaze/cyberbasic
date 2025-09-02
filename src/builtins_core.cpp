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
  R.add("PRINT", NativeFn{"PRINT", 1, [](const std::vector<Value>& a){
    std::cout << to_string_value(a[0]) << std::endl; return Value::nil();
  }});

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

  R.add("SEED", NativeFn{"SEED", 1, [](const std::vector<Value>& a){
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
    if(!a[0].is_array()) throw std::runtime_error("REMOVE: first argument must be an array");
    bas::Value::Array copy = a[0].as_array();
    long long idx = a[1].as_int();
    if(idx >= 0 && (size_t)idx < copy.size()) copy.erase(copy.begin() + (size_t)idx);
    return Value::from_array(std::move(copy));
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

  // Math & system
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

  R.add("SLEEP", NativeFn{"SLEEP", 1, [](const std::vector<Value>& a){
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

  // Delegate to other categories
  register_builtins_console(R);
  register_builtins_graphics(R);
  register_builtins_audio(R);
}

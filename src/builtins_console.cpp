#include "bas/builtins.hpp"
#include <iostream>
#include <sstream>
#include <locale>

using namespace bas;

// Helper function to convert any Value to string
static std::string to_string_value(const Value& v){
  if(auto s = std::get_if<std::string>(&v.v)) return *s;
  if(auto b = std::get_if<bool>(&v.v)) return *b?"TRUE":"FALSE";
  if(auto d = std::get_if<double>(&v.v)) { std::ostringstream os; os.imbue(std::locale::classic()); os<<*d; return os.str(); }
  if(auto i = std::get_if<long long>(&v.v)) { std::ostringstream os; os.imbue(std::locale::classic()); os<<*i; return os.str(); }
  return "NIL";
}

static void print_value(const Value& v) {
    std::cout << to_string_value(v);
}

void register_builtins_console(FunctionRegistry& R) {
    // PRINT ...
    R.add("PRINT", NativeFn{"PRINT", -1, [](const std::vector<Value>& a){
        for (size_t i = 0; i < a.size(); ++i) {
            print_value(a[i]);
            if (i < a.size() - 1) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
        return Value::nil();
    }});
  // SETCURSORVISIBLE(flag)
  R.add("SETCURSORVISIBLE", NativeFn{"SETCURSORVISIBLE", 1, [](const std::vector<Value>& a){
    bool show = a[0].as_bool();
    if(show) std::cout << "\x1b[?25h"; else std::cout << "\x1b[?25l";
    return Value::nil();
  }});

  // INK(r,g,b): set ANSI color (best-effort). Values 0-255, mapped to 38;2;r;g;bm
  R.add("INK", NativeFn{"INK", 3, [](const std::vector<Value>& a){
    int r = (int)a[0].as_int(); int g=(int)a[1].as_int(); int b=(int)a[2].as_int();
    if(r<0) { r=0; } if(r>255) { r=255; }
    if(g<0) { g=0; } if(g>255) { g=255; }
    if(b<0) { b=0; } if(b>255) { b=255; }
    std::cout << "\x1b[38;2;" << r << ";" << g << ";" << b << "m";
    return Value::nil();
  }});
}

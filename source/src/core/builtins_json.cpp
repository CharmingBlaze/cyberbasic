#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>

using bas::Value;
using bas::FunctionRegistry;
using bas::NativeFn;

namespace bas {

// Simple JSON parser - handles basic JSON structures
static Value parse_json_value(const std::string& json, size_t& pos) {
    // Skip whitespace
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r')) {
        pos++;
    }
    if (pos >= json.size()) return Value::nil();
    
    char c = json[pos];
    
    // Parse string
    if (c == '"') {
        pos++; // skip opening quote
        std::string str;
        while (pos < json.size() && json[pos] != '"') {
            if (json[pos] == '\\' && pos + 1 < json.size()) {
                pos++;
                if (json[pos] == 'n') str += '\n';
                else if (json[pos] == 't') str += '\t';
                else if (json[pos] == 'r') str += '\r';
                else if (json[pos] == '\\') str += '\\';
                else if (json[pos] == '"') str += '"';
                else str += json[pos];
            } else {
                str += json[pos];
            }
            pos++;
        }
        if (pos < json.size()) pos++; // skip closing quote
        return Value::from_string(str);
    }
    
    // Parse number
    if ((c >= '0' && c <= '9') || c == '-' || c == '+') {
        std::string numStr;
        bool isFloat = false;
        while (pos < json.size() && ((json[pos] >= '0' && json[pos] <= '9') || json[pos] == '.' || json[pos] == '-' || json[pos] == '+' || json[pos] == 'e' || json[pos] == 'E')) {
            if (json[pos] == '.') isFloat = true;
            numStr += json[pos];
            pos++;
        }
        if (isFloat) {
            return Value::from_number(std::stod(numStr));
        } else {
            return Value::from_int(std::stoll(numStr));
        }
    }
    
    // Parse boolean
    if (json.substr(pos, 4) == "true") {
        pos += 4;
        return Value::from_bool(true);
    }
    if (json.substr(pos, 5) == "false") {
        pos += 5;
        return Value::from_bool(false);
    }
    
    // Parse null
    if (json.substr(pos, 4) == "null") {
        pos += 4;
        return Value::nil();
    }
    
    // Parse array
    if (c == '[') {
        pos++; // skip '['
        Value::Array arr;
        bool first = true;
        while (pos < json.size() && json[pos] != ']') {
            if (!first) {
                if (json[pos] == ',') pos++;
                else break;
            }
            first = false;
            Value val = parse_json_value(json, pos);
            if (!val.is_nil() || pos < json.size()) {
                arr.push_back(val);
            }
        }
        if (pos < json.size() && json[pos] == ']') pos++;
        return Value::from_array(std::move(arr));
    }
    
    // Parse object
    if (c == '{') {
        pos++; // skip '{'
        Value::Map map;
        bool first = true;
        while (pos < json.size() && json[pos] != '}') {
            if (!first) {
                if (json[pos] == ',') pos++;
                else break;
            }
            first = false;
            
            // Parse key
            while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
            if (pos >= json.size() || json[pos] != '"') break;
            pos++; // skip quote
            std::string key;
            while (pos < json.size() && json[pos] != '"') {
                if (json[pos] == '\\' && pos + 1 < json.size()) {
                    pos++;
                    key += json[pos];
                } else {
                    key += json[pos];
                }
                pos++;
            }
            if (pos < json.size()) pos++; // skip closing quote
            
            // Skip colon
            while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == ':')) pos++;
            
            // Parse value
            Value val = parse_json_value(json, pos);
            map[key] = val;
        }
        if (pos < json.size() && json[pos] == '}') pos++;
        return Value::from_map(std::move(map));
    }
    
    return Value::nil();
}

// Simple JSON stringifier
static std::string stringify_json_value(const Value& v, int indent = 0) {
    std::ostringstream oss;
    std::string indentStr(indent * 2, ' ');
    
    if (v.is_string()) {
        oss << '"';
        std::string str = v.as_string();
        for (char c : str) {
            if (c == '"') oss << "\\\"";
            else if (c == '\\') oss << "\\\\";
            else if (c == '\n') oss << "\\n";
            else if (c == '\t') oss << "\\t";
            else if (c == '\r') oss << "\\r";
            else oss << c;
        }
        oss << '"';
    } else if (v.is_array()) {
        oss << '[';
        const auto& arr = v.as_array();
        for (size_t i = 0; i < arr.size(); i++) {
            if (i > 0) oss << ',';
            oss << stringify_json_value(arr[i], indent);
        }
        oss << ']';
    } else if (v.is_map()) {
        oss << '{';
        const auto& map = v.as_map();
        bool first = true;
        for (const auto& [key, value] : map) {
            if (!first) oss << ',';
            first = false;
            oss << '"' << key << "\":" << stringify_json_value(value, indent);
        }
        oss << '}';
    } else if (v.is_nil()) {
        oss << "null";
    } else {
        try {
            oss << v.as_int();
        } catch (...) {
            try {
                oss << std::fixed << std::setprecision(6) << v.as_number();
            } catch (...) {
                try {
                    oss << (v.as_bool() ? "true" : "false");
                } catch (...) {
                    oss << "null";
                }
            }
        }
    }
    return oss.str();
}

void register_json_functions(FunctionRegistry& R) {
    R.add("JSON.PARSE", NativeFn{"JSON.PARSE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("JSON.PARSE: expected 1 argument");
        if (!args[0].is_string()) throw std::runtime_error("JSON.PARSE: expected string argument");
        
        std::string json = args[0].as_string();
        size_t pos = 0;
        Value result = parse_json_value(json, pos);
        return result;
    }});
    
    R.add("JSON.STRINGIFY", NativeFn{"JSON.STRINGIFY", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("JSON.STRINGIFY: expected 1 argument");
        
        std::string json = stringify_json_value(args[0]);
        return Value::from_string(json);
    }});
}

} // namespace bas


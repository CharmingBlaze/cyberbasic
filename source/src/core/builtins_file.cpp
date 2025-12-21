#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <cctype>

using bas::Value;
using bas::FunctionRegistry;
using bas::NativeFn;

namespace bas {

void register_file_functions(FunctionRegistry& R) {
    // Path object constructor
    R.add(normalize_identifier("PATH"), NativeFn{normalize_identifier("PATH"), 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PATH: expected 1 argument");
        if (!args[0].is_string()) throw std::runtime_error("PATH: expected string argument");
        
        std::string pathStr = args[0].as_string();
        Value::Map pathObj;
        pathObj[normalize_identifier("_type")] = Value::from_string(normalize_identifier("Path"));
        pathObj[normalize_identifier("path")] = Value::from_string(pathStr);
        
        // Add path methods as properties
        std::filesystem::path p(pathStr);
        pathObj[normalize_identifier("exists")] = Value::from_bool(std::filesystem::exists(p));
        pathObj[normalize_identifier("isfile")] = Value::from_bool(std::filesystem::is_regular_file(p));
        pathObj[normalize_identifier("isdirectory")] = Value::from_bool(std::filesystem::is_directory(p));
        pathObj[normalize_identifier("extension")] = Value::from_string(p.extension().string());
        pathObj[normalize_identifier("stem")] = Value::from_string(p.stem().string());
        pathObj[normalize_identifier("filename")] = Value::from_string(p.filename().string());
        pathObj[normalize_identifier("parent")] = Value::from_string(p.parent_path().string());
        
        return Value::from_map(std::move(pathObj));
    }});
    
    // Path.exists() method
    R.add(normalize_identifier("PATH_EXISTS"), NativeFn{normalize_identifier("PATH_EXISTS"), 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PATH_EXISTS: expected 1 argument");
        if (!args[0].is_map()) throw std::runtime_error("PATH_EXISTS: expected Path object");
        
        const auto& pathObj = args[0].as_map();
        auto it = pathObj.find(normalize_identifier("path"));
        if (it == pathObj.end()) return Value::from_bool(false);
        
        std::filesystem::path p(it->second.as_string());
        return Value::from_bool(std::filesystem::exists(p));
    }});
    
    // File.readText() method
    R.add(normalize_identifier("FILE_READTEXT"), NativeFn{normalize_identifier("FILE_READTEXT"), 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("FILE_READTEXT: expected 1 argument");
        
        std::string path;
        if (args[0].is_string()) {
            path = args[0].as_string();
        } else if (args[0].is_map()) {
            const auto& pathObj = args[0].as_map();
            auto it = pathObj.find(normalize_identifier("path"));
            if (it == pathObj.end()) throw std::runtime_error("FILE_READTEXT: Path object missing path property");
            path = it->second.as_string();
        } else {
            throw std::runtime_error("FILE_READTEXT: expected string or Path object");
        }
        
        std::ifstream file(path);
        if (!file.is_open()) throw std::runtime_error("FILE_READTEXT: could not open file: " + path);
        
        std::ostringstream oss;
        oss << file.rdbuf();
        return Value::from_string(oss.str());
    }});
    
    // File.writeText() method
    R.add(normalize_identifier("FILE_WRITETEXT"), NativeFn{normalize_identifier("FILE_WRITETEXT"), 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("FILE_WRITETEXT: expected 2 arguments");
        if (!args[1].is_string()) throw std::runtime_error("FILE_WRITETEXT: expected string content");
        
        std::string path;
        if (args[0].is_string()) {
            path = args[0].as_string();
        } else if (args[0].is_map()) {
            const auto& pathObj = args[0].as_map();
            auto it = pathObj.find(normalize_identifier("path"));
            if (it == pathObj.end()) throw std::runtime_error("FILE_WRITETEXT: Path object missing path property");
            path = it->second.as_string();
        } else {
            throw std::runtime_error("FILE_WRITETEXT: expected string or Path object");
        }
        
        std::ofstream file(path);
        if (!file.is_open()) throw std::runtime_error("FILE_WRITETEXT: could not open file for writing: " + path);
        
        file << args[1].as_string();
        return Value::nil();
    }});
    
    // File object constructor
    R.add(normalize_identifier("FILE"), NativeFn{normalize_identifier("FILE"), 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("FILE: expected 1 argument");
        if (!args[0].is_string()) throw std::runtime_error("FILE: expected string argument");
        
        std::string pathStr = args[0].as_string();
        Value::Map fileObj;
        fileObj[normalize_identifier("_type")] = Value::from_string(normalize_identifier("File"));
        fileObj[normalize_identifier("path")] = Value::from_string(pathStr);
        fileObj[normalize_identifier("exists")] = Value::from_bool(std::filesystem::exists(std::filesystem::path(pathStr)));
        
        return Value::from_map(std::move(fileObj));
    }});
}

} // namespace bas


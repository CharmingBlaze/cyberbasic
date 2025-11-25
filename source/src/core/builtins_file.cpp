#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

using bas::Value;
using bas::FunctionRegistry;
using bas::NativeFn;

namespace bas {

void register_file_functions(FunctionRegistry& R) {
    // Path object constructor
    R.add("PATH", NativeFn{"PATH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PATH: expected 1 argument");
        if (!args[0].is_string()) throw std::runtime_error("PATH: expected string argument");
        
        std::string pathStr = args[0].as_string();
        Value::Map pathObj;
        pathObj["_type"] = Value::from_string("Path");
        pathObj["path"] = Value::from_string(pathStr);
        
        // Add path methods as properties
        std::filesystem::path p(pathStr);
        pathObj["exists"] = Value::from_bool(std::filesystem::exists(p));
        pathObj["isFile"] = Value::from_bool(std::filesystem::is_regular_file(p));
        pathObj["isDirectory"] = Value::from_bool(std::filesystem::is_directory(p));
        pathObj["extension"] = Value::from_string(p.extension().string());
        pathObj["stem"] = Value::from_string(p.stem().string());
        pathObj["filename"] = Value::from_string(p.filename().string());
        pathObj["parent"] = Value::from_string(p.parent_path().string());
        
        return Value::from_map(std::move(pathObj));
    }});
    
    // Path.exists() method
    R.add("PATH_EXISTS", NativeFn{"PATH_EXISTS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PATH_EXISTS: expected 1 argument");
        if (!args[0].is_map()) throw std::runtime_error("PATH_EXISTS: expected Path object");
        
        const auto& pathObj = args[0].as_map();
        auto it = pathObj.find("path");
        if (it == pathObj.end()) return Value::from_bool(false);
        
        std::filesystem::path p(it->second.as_string());
        return Value::from_bool(std::filesystem::exists(p));
    }});
    
    // File.readText() method
    R.add("FILE_READTEXT", NativeFn{"FILE_READTEXT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("FILE_READTEXT: expected 1 argument");
        
        std::string path;
        if (args[0].is_string()) {
            path = args[0].as_string();
        } else if (args[0].is_map()) {
            const auto& pathObj = args[0].as_map();
            auto it = pathObj.find("path");
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
    R.add("FILE_WRITETEXT", NativeFn{"FILE_WRITETEXT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("FILE_WRITETEXT: expected 2 arguments");
        if (!args[1].is_string()) throw std::runtime_error("FILE_WRITETEXT: expected string content");
        
        std::string path;
        if (args[0].is_string()) {
            path = args[0].as_string();
        } else if (args[0].is_map()) {
            const auto& pathObj = args[0].as_map();
            auto it = pathObj.find("path");
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
    R.add("FILE", NativeFn{"FILE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("FILE: expected 1 argument");
        if (!args[0].is_string()) throw std::runtime_error("FILE: expected string argument");
        
        std::string pathStr = args[0].as_string();
        Value::Map fileObj;
        fileObj["_type"] = Value::from_string("File");
        fileObj["path"] = Value::from_string(pathStr);
        fileObj["exists"] = Value::from_bool(std::filesystem::exists(std::filesystem::path(pathStr)));
        
        return Value::from_map(std::move(fileObj));
    }});
}

} // namespace bas


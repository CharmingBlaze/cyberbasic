#include "bas/localization.hpp"
#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include "bas/builtins_file.hpp"
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>

namespace bas {

// Localization data
static std::unordered_map<std::string, std::unordered_map<std::string, std::string>> g_loc_strings;
static std::string g_current_language = "en";

// LOC.loadTable(filePath) - Load CSV/JSON localization table
static Value loc_loadTable(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::from_bool(false);
    }
    
    std::string filePath = args[0].as_string();
    
    // Try to read file
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return Value::from_bool(false);
    }
    
    std::string line;
    bool firstLine = true;
    std::vector<std::string> languages;
    
    // Read CSV format: key,en,es,fr,...
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;
        
        // Simple CSV parsing (handles quoted strings)
        bool inQuotes = false;
        std::string current;
        for (char c : line) {
            if (c == '"') {
                inQuotes = !inQuotes;
            } else if (c == ',' && !inQuotes) {
                tokens.push_back(current);
                current.clear();
            } else {
                current += c;
            }
        }
        if (!current.empty()) {
            tokens.push_back(current);
        }
        
        if (tokens.empty()) continue;
        
        if (firstLine) {
            // First line contains language codes
            for (size_t i = 1; i < tokens.size(); ++i) {
                languages.push_back(tokens[i]);
            }
            firstLine = false;
        } else {
            // Subsequent lines contain key-value pairs
            if (tokens.size() < 2) continue;
            
            std::string key = tokens[0];
            for (size_t i = 1; i < tokens.size() && i - 1 < languages.size(); ++i) {
                std::string lang = languages[i - 1];
                std::string value = tokens[i];
                g_loc_strings[lang][key] = value;
            }
        }
    }
    
    return Value::from_bool(true);
}

// LOC.setLanguage(languageCode) - Set current language
static Value loc_setLanguage(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::nil();
    }
    
    g_current_language = args[0].as_string();
    return Value::nil();
}

// LOC.get(key) -> string - Get localized string
static Value loc_get(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::from_string("");
    }
    
    std::string key = args[0].as_string();
    
    // Try current language first
    auto langIt = g_loc_strings.find(g_current_language);
    if (langIt != g_loc_strings.end()) {
        auto keyIt = langIt->second.find(key);
        if (keyIt != langIt->second.end()) {
            return Value::from_string(keyIt->second);
        }
    }
    
    // Fallback to English
    if (g_current_language != "en") {
        auto enIt = g_loc_strings.find("en");
        if (enIt != g_loc_strings.end()) {
            auto keyIt = enIt->second.find(key);
            if (keyIt != enIt->second.end()) {
                return Value::from_string(keyIt->second);
            }
        }
    }
    
    // Return key if not found
    return Value::from_string(key);
}

// LOC.getLanguage() -> string - Get current language code
static Value loc_getLanguage(const std::vector<Value>& args) {
    (void)args;
    return Value::from_string(g_current_language);
}

// LOC.hasKey(key) -> bool - Check if key exists
static Value loc_hasKey(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::from_bool(false);
    }
    
    std::string key = args[0].as_string();
    auto langIt = g_loc_strings.find(g_current_language);
    if (langIt != g_loc_strings.end()) {
        return Value::from_bool(langIt->second.find(key) != langIt->second.end());
    }
    
    return Value::from_bool(false);
}

void register_localization(FunctionRegistry& registry) {
    registry.add("LOC_LOADTABLE", NativeFn{"LOC_LOADTABLE", 1, loc_loadTable});
    registry.add("LOC_SETLANGUAGE", NativeFn{"LOC_SETLANGUAGE", 1, loc_setLanguage});
    registry.add("LOC_GET", NativeFn{"LOC_GET", 1, loc_get});
    registry.add("LOC_GETLANGUAGE", NativeFn{"LOC_GETLANGUAGE", 0, loc_getLanguage});
    registry.add("LOC_HASKEY", NativeFn{"LOC_HASKEY", 1, loc_hasKey});
}

}


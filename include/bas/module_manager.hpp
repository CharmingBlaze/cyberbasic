#pragma once

#include "bas/module_base.hpp"
#include "bas/runtime.hpp"
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

namespace bas {

/**
 * @brief Manages loading, registration, and lifecycle of modules
 */
class ModuleManager {
public:
    ModuleManager() = default;
    ~ModuleManager() {
        shutdown_all();
    }

    /**
     * @brief Register a module with the manager
     * @param module Shared pointer to module instance
     */
    void register_module(std::shared_ptr<Module> module) {
        if (!module) return;
        
        std::string name = module->name();
        modules_[name] = module;
        
        // Register module's functions
        module->register_functions(registry_);
        
        // Initialize module
        module->initialize();
    }

    /**
     * @brief Get the function registry (used by modules)
     */
    [[nodiscard]] FunctionRegistry& get_registry() {
        return registry_;
    }
    
    /**
     * @brief Get the function registry (const version)
     */
    [[nodiscard]] const FunctionRegistry& get_registry() const {
        return registry_;
    }

    /**
     * @brief Shutdown all modules
     */
    void shutdown_all() {
        for (auto& [name, module] : modules_) {
            if (module) {
                module->shutdown();
            }
        }
        modules_.clear();
    }

    /**
     * @brief Get a module by name
     */
    [[nodiscard]] std::shared_ptr<Module> get_module(const std::string& name) {
        auto it = modules_.find(name);
        return (it != modules_.end()) ? it->second : nullptr;
    }

    /**
     * @brief Check if a module is loaded
     */
    [[nodiscard]] bool is_loaded(const std::string& name) const {
        return modules_.find(name) != modules_.end();
    }

    /**
     * @brief Get list of loaded module names
     */
    [[nodiscard]] std::vector<std::string> get_loaded_modules() const {
        std::vector<std::string> names;
        names.reserve(modules_.size());
        for (const auto& [name, _] : modules_) {
            names.push_back(name);
        }
        return names;
    }

private:
    FunctionRegistry registry_;
    std::unordered_map<std::string, std::shared_ptr<Module>> modules_;
};

} // namespace bas


#pragma once

#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

namespace bas {

/**
 * @brief Represents a namespace with methods that can be called via dot notation
 * 
 * A namespace is essentially an object with methods. When you call
 * `Window.init()`, it resolves to calling the function registered
 * as "WINDOW_INIT" or "INITWINDOW".
 */
class NamespaceRegistry {
public:
    /**
     * @brief Register a namespace with a name and list of method mappings
     * 
     * @param namespace_name Name of the namespace (e.g., "Window")
     * @param methods Map of method names to function names in registry
     * 
     * Example:
     *   register_namespace("Window", {
     *       {"init", "INITWINDOW"},
     *       {"close", "CLOSEWINDOW"},
     *       {"shouldClose", "WINDOWSHOULDCLOSE"}
     *   });
     */
    void register_namespace(
        const std::string& namespace_name,
        const std::unordered_map<std::string, std::string>& methods
    );

    /**
     * @brief Create a namespace object that can be accessed in BASIC
     * 
     * @param namespace_name Name of the namespace
     * @return Value representing the namespace object
     */
    [[nodiscard]] Value create_namespace_object(const std::string& namespace_name) const;

    /**
     * @brief Resolve a method call like "Window.init" to the actual function name
     * 
     * @param namespace_name Name of the namespace
     * @param method_name Name of the method
     * @return Function name in registry, or empty string if not found
     */
    [[nodiscard]] std::string resolve_method(
        const std::string& namespace_name,
        const std::string& method_name
    ) const;

    /**
     * @brief Check if a namespace exists
     */
    [[nodiscard]] bool has_namespace(const std::string& namespace_name) const;

    /**
     * @brief Get all registered namespace names
     */
    [[nodiscard]] std::vector<std::string> get_namespaces() const;

private:
    // Map namespace name -> (method name -> function name)
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> namespaces_;
};

/**
 * @brief Register all raylib namespaces
 * 
 * This function sets up all the namespace mappings for raylib functions,
 * making them accessible via dot notation like Window.init(), Graphics.drawRectangle(), etc.
 */
void register_raylib_namespaces(NamespaceRegistry& registry);
void register_game_namespaces(NamespaceRegistry& registry);

} // namespace bas


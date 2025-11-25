#pragma once

#include "bas/runtime.hpp"
#include <string>

namespace bas {

/**
 * @brief Base class for all CyberBasic modules
 * 
 * All feature modules should inherit from this class and implement
 * the required virtual functions.
 */
class Module {
public:
    virtual ~Module() = default;

    /**
     * @brief Register all functions provided by this module
     * @param registry The function registry to register functions with
     */
    virtual void register_functions(FunctionRegistry& registry) = 0;

    /**
     * @brief Initialize the module (called after registration)
     * Override if module needs initialization
     */
    virtual void initialize() {}

    /**
     * @brief Shutdown the module (called before unloading)
     * Override if module needs cleanup
     */
    virtual void shutdown() {}

    /**
     * @brief Get the module name
     * @return Module name as C-string
     */
    [[nodiscard]] virtual const char* name() const = 0;

    /**
     * @brief Get the module version
     * @return Version string (default: "1.0.0")
     */
    [[nodiscard]] virtual const char* version() const { return "1.0.0"; }

    /**
     * @brief Check if module is optional
     * @return true if module can be disabled, false if required
     */
    [[nodiscard]] virtual constexpr bool is_optional() const noexcept { return true; }
};

} // namespace bas


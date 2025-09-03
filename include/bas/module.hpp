#ifndef BAS_MODULE_HPP
#define BAS_MODULE_HPP

// Forward declaration of the FunctionRegistry to avoid including the full header.
namespace bas {
    class FunctionRegistry;
}

// The ModuleRegistrar struct provides a stable ABI for modules to register themselves.
// It passes a reference to the core FunctionRegistry.
struct ModuleRegistrar {
    bas::FunctionRegistry& registry;
    // Future members can be added here for ABI evolution without breaking old modules.
};

// Each module MUST export these C-style functions.

/**
 * @brief The entry point for registering the module's functions.
 * The core executable calls this after loading the module.
 * @param registrar A reference to the registrar object, containing the function registry.
 * @return True on successful registration, false otherwise.
 */
extern "C" bool bas_module_register_v1(ModuleRegistrar& registrar);

/**
 * @brief Provides information about the module (name, version, etc.) as a JSON or YAML string.
 * This allows the core to query module metadata without full registration.
 * @return A null-terminated C-string with module information.
 */
extern "C" const char* bas_module_info_v1();

#endif // BAS_MODULE_HPP

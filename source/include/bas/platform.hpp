#ifndef BAS_PLATFORM_HPP
#define BAS_PLATFORM_HPP

#include <string>
#include <filesystem>

namespace bas {

// Abstract handle for a dynamically loaded library.
using ModuleHandle = void*;

// Abstract handle for a function symbol loaded from a library.
using FunctionHandle = void*;

/**
 * @brief Loads a dynamic library from the given path.
 * @param path The platform-specific path to the library (.dll, .so, .dylib).
 * @return A handle to the loaded library, or nullptr on failure.
 */
ModuleHandle load_library(const std::filesystem::path& path);

/**
 * @brief Unloads a previously loaded dynamic library.
 * @param handle The handle returned by load_library.
 */
void unload_library(ModuleHandle handle);

/**
 * @brief Retrieves a function pointer from a loaded library.
 * @param handle The handle to the library.
 * @param function_name The name of the function to retrieve.
 * @return A handle to the function, or nullptr if not found.
 */
FunctionHandle get_function(ModuleHandle handle, const std::string& function_name);

} // namespace bas

#endif // BAS_PLATFORM_HPP

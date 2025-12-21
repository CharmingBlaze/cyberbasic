#include "bas/module_base.hpp"
#include "bas/networking.hpp"
#include "bas/runtime.hpp"

// Forward declaration for advanced networking
namespace bas {
    void register_advanced_networking_functions(FunctionRegistry& registry);
}

namespace bas {

/**
 * Networking Module
 * Provides networking and communication functions
 */
class NetworkingModule : public Module {
public:
    const char* name() const override { return "networking"; }
    
    void register_functions(FunctionRegistry& registry) override {
        register_networking_functions(registry);
        register_advanced_networking_functions(registry);
    }
    
    void initialize() override {}
    void shutdown() override {}
};

extern "C" Module* create_networking_module() {
    return new NetworkingModule();
}

} // namespace bas


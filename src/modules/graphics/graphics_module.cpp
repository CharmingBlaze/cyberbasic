#include "bas/module_base.hpp"
#include "bas/graphics.hpp"
#include "bas/runtime.hpp"

namespace bas {

/**
 * Graphics Module
 * Provides high-level graphics functions and utilities
 */
class GraphicsModule : public Module {
public:
    const char* name() const override { return "graphics"; }
    
    void register_functions(FunctionRegistry& registry) override {
        register_graphics_functions(registry);
    }
    
    void initialize() override {
        // Graphics module initialization if needed
    }
    
    void shutdown() override {
        // Graphics module cleanup if needed
    }
};

// Factory function to create module instance
extern "C" Module* create_graphics_module() {
    return new GraphicsModule();
}

} // namespace bas


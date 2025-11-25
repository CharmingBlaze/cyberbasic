#include "bas/module_base.hpp"
#include "bas/raymath.hpp"
#include "bas/runtime.hpp"

namespace bas {

/**
 * Raymath Module
 * Provides raymath mathematical functions
 */
class RaymathModule : public Module {
public:
    const char* name() const override { return "raymath"; }
    
    void register_functions(FunctionRegistry& registry) override {
        register_raymath_functions(registry);
    }
    
    void initialize() override {}
    void shutdown() override {}
};

extern "C" Module* create_raymath_module() {
    return new RaymathModule();
}

} // namespace bas


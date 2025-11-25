#include "bas/module_base.hpp"
#include "bas/physics.hpp"
#include "bas/runtime.hpp"

namespace bas {

/**
 * Physics Module
 * Provides physics simulation functions
 */
class PhysicsModule : public Module {
public:
    const char* name() const override { return "physics"; }
    
    void register_functions(FunctionRegistry& registry) override {
        register_physics_functions(registry);
    }
    
    void initialize() override {}
    void shutdown() override {}
};

extern "C" Module* create_physics_module() {
    return new PhysicsModule();
}

} // namespace bas


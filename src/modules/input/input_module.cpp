#include "bas/module_base.hpp"
#include "bas/input.hpp"
#include "bas/runtime.hpp"

namespace bas {

/**
 * Input Module
 * Provides input handling functions
 */
class InputModule : public Module {
public:
    const char* name() const override { return "input"; }
    
    void register_functions(FunctionRegistry& registry) override {
        // Input functions are registered elsewhere, but this provides the module interface
        // If there's a register_input_functions, call it here
    }
    
    void initialize() override {}
    void shutdown() override {}
};

// Factory function to create module instance
extern "C" Module* create_input_module() {
    return new InputModule();
}

} // namespace bas


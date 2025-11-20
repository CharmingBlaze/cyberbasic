#include "bas/module_base.hpp"
#include "bas/gui.hpp"
#include "bas/runtime.hpp"

namespace bas {

/**
 * GUI Module
 * Provides GUI and UI functions
 */
class GUIModule : public Module {
public:
    const char* name() const override { return "gui"; }
    
    void register_functions(FunctionRegistry& registry) override {
        register_gui_functions(registry);
    }
    
    void initialize() override {}
    void shutdown() override {}
};

extern "C" Module* create_gui_module() {
    return new GUIModule();
}

} // namespace bas


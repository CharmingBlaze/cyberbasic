#include "bas/module_base.hpp"
#include "bas/ai.hpp"
#include "bas/navigation.hpp"
#include "bas/runtime.hpp"

namespace bas {

/**
 * AI Module
 * Provides AI and navigation functions
 */
class AIModule : public Module {
public:
    const char* name() const override { return "ai"; }
    
    void register_functions(FunctionRegistry& registry) override {
        register_ai_functions(registry);
        register_navigation_functions(registry);
    }
    
    void initialize() override {}
    void shutdown() override {}
};

extern "C" Module* create_ai_module() {
    return new AIModule();
}

} // namespace bas


#include "bas/module_base.hpp"
#include "bas/camera3d.hpp"
#include "bas/lighting3d.hpp"
#include "bas/models3d.hpp"
#include "bas/runtime.hpp"

namespace bas {

/**
 * 3D Graphics Module
 * Provides 3D graphics, camera, lighting, and model functions
 */
class Graphics3DModule : public Module {
public:
    const char* name() const override { return "3d"; }
    
    void register_functions(FunctionRegistry& registry) override {
        register_camera3d_functions(registry);
        register_lighting3d_functions(registry);
        register_models3d_functions(registry);
    }
    
    void initialize() override {}
    void shutdown() override {}
};

extern "C" Module* create_3d_module() {
    return new Graphics3DModule();
}

} // namespace bas


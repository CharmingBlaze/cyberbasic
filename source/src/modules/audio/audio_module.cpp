#include "bas/module_base.hpp"
#include "bas/audio.hpp"
#include "bas/runtime.hpp"

namespace bas {

/**
 * Audio Module
 * Provides audio playback and management functions
 */
class AudioModule : public Module {
public:
    const char* name() const override { return "audio"; }
    
    void register_functions(FunctionRegistry& registry) override {
        register_audio_functions(registry);
    }
    
    void initialize() override {
        // Audio module initialization if needed
    }
    
    void shutdown() override {
        // Audio module cleanup if needed
    }
};

// Factory function to create module instance
extern "C" Module* create_audio_module() {
    return new AudioModule();
}

} // namespace bas


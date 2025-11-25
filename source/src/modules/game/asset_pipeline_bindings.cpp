#include "bas/asset_pipeline.hpp"
#include "bas/runtime.hpp"

namespace bas {

void register_asset_pipeline_functions(FunctionRegistry& R) {
    
    // === ASSET PIPELINE FUNCTIONS ===
    
    R.add("LOADASSETTEXTURE", NativeFn{"LOADASSETTEXTURE", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADASSETTEXTURE: expected 1 arg");
        std::string filename = args[0].as_string();
        int asset_id = AssetPipeline::load_texture(filename);
        return Value::from_int(asset_id);
    }});
    
    R.add("LOADASSETMODEL", NativeFn{"LOADASSETMODEL", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADASSETMODEL: expected 1 arg");
        std::string filename = args[0].as_string();
        int asset_id = AssetPipeline::load_model(filename);
        return Value::from_int(asset_id);
    }});
    
    R.add("LOADASSETSOUND", NativeFn{"LOADASSETSOUND", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADASSETSOUND: expected 1 arg");
        std::string filename = args[0].as_string();
        int asset_id = AssetPipeline::load_sound(filename);
        return Value::from_int(asset_id);
    }});
    
    R.add("LOADASSETMUSIC", NativeFn{"LOADASSETMUSIC", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADASSETMUSIC: expected 1 arg");
        std::string filename = args[0].as_string();
        int asset_id = AssetPipeline::load_music(filename);
        return Value::from_int(asset_id);
    }});
    
    R.add("LOADASSETFONT", NativeFn{"LOADASSETFONT", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADASSETFONT: expected 1 arg");
        std::string filename = args[0].as_string();
        int asset_id = AssetPipeline::load_font(filename);
        return Value::from_int(asset_id);
    }});
    
    R.add("LOADASSETSHADER", NativeFn{"LOADASSETSHADER", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADASSETSHADER: expected 1 arg");
        std::string filename = args[0].as_string();
        int asset_id = AssetPipeline::load_shader(filename);
        return Value::from_int(asset_id);
    }});
    
    R.add("BATCHTEXTURES", NativeFn{"BATCHTEXTURES", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("BATCHTEXTURES: expected 2 args");
        std::string input_dir = args[0].as_string();
        std::string output_file = args[1].as_string();
        bool success = AssetPipeline::batch_textures(input_dir, output_file);
        return Value::from_bool(success);
    }});
    
    R.add("OPTIMIZEMODEL", NativeFn{"OPTIMIZEMODEL", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("OPTIMIZEMODEL: expected 2 args");
        int model_id = static_cast<int>(args[0].as_int());
        double quality = args[1].as_number();
        bool success = AssetPipeline::optimize_model(model_id, quality);
        return Value::from_bool(success);
    }});
    
    R.add("COMPRESSAUDIO", NativeFn{"COMPRESSAUDIO", 3, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("COMPRESSAUDIO: expected 3 args");
        int sound_id = static_cast<int>(args[0].as_int());
        std::string format = args[1].as_string();
        int bitrate = static_cast<int>(args[2].as_int());
        bool success = AssetPipeline::compress_audio(sound_id, format, bitrate);
        return Value::from_bool(success);
    }});
    
    R.add("ISASSETLOADED", NativeFn{"ISASSETLOADED", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISASSETLOADED: expected 1 arg");
        int asset_id = static_cast<int>(args[0].as_int());
        bool loaded = AssetPipeline::is_asset_loaded(asset_id);
        return Value::from_bool(loaded);
    }});
    
    R.add("GETASSETFILENAME", NativeFn{"GETASSETFILENAME", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETASSETFILENAME: expected 1 arg");
        int asset_id = static_cast<int>(args[0].as_int());
        std::string filename = AssetPipeline::get_asset_filename(asset_id);
        return Value::from_string(filename);
    }});
    
    R.add("UNLOADASSET", NativeFn{"UNLOADASSET", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADASSET: expected 1 arg");
        int asset_id = static_cast<int>(args[0].as_int());
        AssetPipeline::unload_asset(asset_id);
        return Value::nil();
    }});
    
    R.add("UNLOADALLASSETS", NativeFn{"UNLOADALLASSETS", 0, [](const std::vector<Value>& args) -> Value {
        (void)args; // Suppress unused parameter warning
        AssetPipeline::unload_all_assets();
        return Value::nil();
    }});
}

} // namespace bas

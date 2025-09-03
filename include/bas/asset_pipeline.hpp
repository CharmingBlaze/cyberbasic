#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "value.hpp"

namespace bas {

enum class AssetType {
    TEXTURE,
    MODEL,
    SOUND,
    MUSIC,
    FONT,
    SHADER
};

struct Asset {
    int id;
    std::string name;
    std::string filename;
    AssetType type;
    bool loaded;
    void* data; // Platform-specific data (Texture2D, Model, Sound, etc.)
    
    Asset(int asset_id, const std::string& asset_name, const std::string& file, AssetType asset_type)
        : id(asset_id), name(asset_name), filename(file), type(asset_type), loaded(false), data(nullptr) {}
    
    ~Asset() {
        // Cleanup will be handled by the AssetPipeline
    }
};

class AssetPipeline {
private:
    static int next_asset_id;
    static std::unordered_map<int, std::shared_ptr<Asset>> assets;
    static std::unordered_map<std::string, int> asset_lookup;
    
public:
    // Asset loading
    static int load_texture(const std::string& filename);
    static int load_model(const std::string& filename);
    static int load_sound(const std::string& filename);
    static int load_music(const std::string& filename);
    static int load_font(const std::string& filename);
    static int load_shader(const std::string& filename);
    
    // Asset management
    static std::shared_ptr<Asset> get_asset(int asset_id);
    static std::shared_ptr<Asset> get_asset(const std::string& name);
    static void unload_asset(int asset_id);
    static void unload_all_assets();
    
    // Asset optimization
    static bool batch_textures(const std::string& input_dir, const std::string& output_file);
    static bool optimize_model(int model_id, double quality);
    static bool compress_audio(int sound_id, const std::string& format, int bitrate);
    
    // Asset information
    static bool is_asset_loaded(int asset_id);
    static std::string get_asset_filename(int asset_id);
    static AssetType get_asset_type(int asset_id);
    
    // Cleanup
    static void shutdown();
};

} // namespace bas

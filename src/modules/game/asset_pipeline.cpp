#include "bas/asset_pipeline.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>

namespace bas {

// Static member definitions
int AssetPipeline::next_asset_id = 1;
std::unordered_map<int, std::shared_ptr<Asset>> AssetPipeline::assets;
std::unordered_map<std::string, int> AssetPipeline::asset_lookup;

int AssetPipeline::load_texture(const std::string& filename) {
    int id = next_asset_id++;
    auto asset = std::make_shared<Asset>(id, filename, filename, AssetType::TEXTURE);
    
    // For now, just mark as loaded (actual Raylib loading would go here)
    asset->loaded = true;
    
    assets[id] = asset;
    asset_lookup[filename] = id;
    return id;
}

int AssetPipeline::load_model(const std::string& filename) {
    int id = next_asset_id++;
    auto asset = std::make_shared<Asset>(id, filename, filename, AssetType::MODEL);
    
    // For now, just mark as loaded (actual Raylib loading would go here)
    asset->loaded = true;
    
    assets[id] = asset;
    asset_lookup[filename] = id;
    return id;
}

int AssetPipeline::load_sound(const std::string& filename) {
    int id = next_asset_id++;
    auto asset = std::make_shared<Asset>(id, filename, filename, AssetType::SOUND);
    
    // For now, just mark as loaded (actual Raylib loading would go here)
    asset->loaded = true;
    
    assets[id] = asset;
    asset_lookup[filename] = id;
    return id;
}

int AssetPipeline::load_music(const std::string& filename) {
    int id = next_asset_id++;
    auto asset = std::make_shared<Asset>(id, filename, filename, AssetType::MUSIC);
    
    // For now, just mark as loaded (actual Raylib loading would go here)
    asset->loaded = true;
    
    assets[id] = asset;
    asset_lookup[filename] = id;
    return id;
}

int AssetPipeline::load_font(const std::string& filename) {
    int id = next_asset_id++;
    auto asset = std::make_shared<Asset>(id, filename, filename, AssetType::FONT);
    
    // For now, just mark as loaded (actual Raylib loading would go here)
    asset->loaded = true;
    
    assets[id] = asset;
    asset_lookup[filename] = id;
    return id;
}

int AssetPipeline::load_shader(const std::string& filename) {
    int id = next_asset_id++;
    auto asset = std::make_shared<Asset>(id, filename, filename, AssetType::SHADER);
    
    // For now, just mark as loaded (actual Raylib loading would go here)
    asset->loaded = true;
    
    assets[id] = asset;
    asset_lookup[filename] = id;
    return id;
}

std::shared_ptr<Asset> AssetPipeline::get_asset(int asset_id) {
    auto it = assets.find(asset_id);
    return (it != assets.end()) ? it->second : nullptr;
}

std::shared_ptr<Asset> AssetPipeline::get_asset(const std::string& name) {
    auto it = asset_lookup.find(name);
    if (it != asset_lookup.end()) {
        return get_asset(it->second);
    }
    return nullptr;
}

void AssetPipeline::unload_asset(int asset_id) {
    auto it = assets.find(asset_id);
    if (it != assets.end()) {
        // Remove from lookup table
        asset_lookup.erase(it->second->filename);
        assets.erase(it);
    }
}

void AssetPipeline::unload_all_assets() {
    assets.clear();
    asset_lookup.clear();
}

bool AssetPipeline::batch_textures(const std::string& input_dir, const std::string& output_file) {
    // For now, just return true (actual texture batching would go here)
    std::cout << "Batching textures from " << input_dir << " to " << output_file << std::endl;
    return true;
}

bool AssetPipeline::optimize_model(int model_id, double quality) {
    auto asset = get_asset(model_id);
    if (asset && asset->type == AssetType::MODEL) {
        std::cout << "Optimizing model " << asset->name << " with quality " << quality << std::endl;
        return true;
    }
    return false;
}

bool AssetPipeline::compress_audio(int sound_id, const std::string& format, int bitrate) {
    auto asset = get_asset(sound_id);
    if (asset && (asset->type == AssetType::SOUND || asset->type == AssetType::MUSIC)) {
        std::cout << "Compressing audio " << asset->name << " to " << format << " at " << bitrate << "kbps" << std::endl;
        return true;
    }
    return false;
}

bool AssetPipeline::is_asset_loaded(int asset_id) {
    auto asset = get_asset(asset_id);
    return asset ? asset->loaded : false;
}

std::string AssetPipeline::get_asset_filename(int asset_id) {
    auto asset = get_asset(asset_id);
    return asset ? asset->filename : "";
}

AssetType AssetPipeline::get_asset_type(int asset_id) {
    auto asset = get_asset(asset_id);
    return asset ? asset->type : AssetType::TEXTURE;
}

void AssetPipeline::shutdown() {
    unload_all_assets();
}

} // namespace bas

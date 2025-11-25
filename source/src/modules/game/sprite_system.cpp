#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <raylib.h>
#include <unordered_map>
#include <string>

namespace bas {

// Sprite object storage
struct SpriteData {
    Texture2D texture{};
    float x{0.0f};
    float y{0.0f};
    float scale{1.0f};
    float rotation{0.0f};
    Color tint{WHITE};
    bool visible{true};
    int frameWidth{0};
    int frameHeight{0};
    int currentFrame{0};
};

static std::unordered_map<int, SpriteData> g_sprites;
static int g_next_sprite_id = 1;

// Sprite constructor: Sprite(imagePath)
static Value sprite_constructor(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string path = args[0].as_string();
    Texture2D texture = LoadTexture(path.c_str());
    
    if (texture.id == 0) {
        return Value::nil();
    }
    
    int id = g_next_sprite_id++;
    SpriteData& sprite = g_sprites[id];
    sprite.texture = texture;
    sprite.frameWidth = texture.width;
    sprite.frameHeight = texture.height;
    
    Value::Map obj;
    obj["_type"] = Value::from_string("Sprite");
    obj["_id"] = Value::from_int(id);
    obj["x"] = Value::from_number(0.0);
    obj["y"] = Value::from_number(0.0);
    obj["scale"] = Value::from_number(1.0);
    obj["rotation"] = Value::from_number(0.0);
    obj["visible"] = Value::from_bool(true);
    obj["width"] = Value::from_int(texture.width);
    obj["height"] = Value::from_int(texture.height);
    
    return Value::from_map(std::move(obj));
}

// Sprite.draw()
static Value sprite_draw(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    int id = static_cast<int>(idIt->second.as_int());
    auto spriteIt = g_sprites.find(id);
    if (spriteIt == g_sprites.end()) {
        return Value::nil();
    }
    
    SpriteData& sprite = spriteIt->second;
    if (!sprite.visible) {
        return Value::nil();
    }
    
    // Update from object properties
    auto xIt = map.find("x");
    auto yIt = map.find("y");
    auto scaleIt = map.find("scale");
    auto rotIt = map.find("rotation");
    
    if (xIt != map.end()) sprite.x = static_cast<float>(xIt->second.as_number());
    if (yIt != map.end()) sprite.y = static_cast<float>(yIt->second.as_number());
    if (scaleIt != map.end()) sprite.scale = static_cast<float>(scaleIt->second.as_number());
    if (rotIt != map.end()) sprite.rotation = static_cast<float>(rotIt->second.as_number());
    
    // Draw sprite
    Vector2 pos = {sprite.x, sprite.y};
    float rot = sprite.rotation;
    float scale = sprite.scale;
    
    DrawTextureEx(sprite.texture, pos, rot, scale, sprite.tint);
    
    return Value::nil();
}

// Sprite.setPosition(x, y)
static Value sprite_setPosition(const std::vector<Value>& args) {
    if (args.size() < 3 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return args[0]; // Return original object
    }
    
    int id = static_cast<int>(idIt->second.as_int());
    float x = static_cast<float>(args[1].as_number());
    float y = static_cast<float>(args[2].as_number());
    
    auto spriteIt = g_sprites.find(id);
    if (spriteIt != g_sprites.end()) {
        spriteIt->second.x = x;
        spriteIt->second.y = y;
    }
    
    // Return updated object
    Value::Map updated = map;
    updated["x"] = Value::from_number(x);
    updated["y"] = Value::from_number(y);
    return Value::from_map(std::move(updated));
}

// Sprite.setScale(scale)
static Value sprite_setScale(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    float scale = static_cast<float>(args[1].as_number());
    
    auto idIt = map.find("_id");
    if (idIt != map.end() && idIt->second.is_int()) {
        int id = static_cast<int>(idIt->second.as_int());
        auto spriteIt = g_sprites.find(id);
        if (spriteIt != g_sprites.end()) {
            spriteIt->second.scale = scale;
        }
    }
    
    Value::Map updated = map;
    updated["scale"] = Value::from_number(scale);
    return Value::from_map(std::move(updated));
}

// Sprite.setRotation(angle)
static Value sprite_setRotation(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    float rotation = static_cast<float>(args[1].as_number());
    
    auto idIt = map.find("_id");
    if (idIt != map.end() && idIt->second.is_int()) {
        int id = static_cast<int>(idIt->second.as_int());
        auto spriteIt = g_sprites.find(id);
        if (spriteIt != g_sprites.end()) {
            spriteIt->second.rotation = rotation;
        }
    }
    
    Value::Map updated = map;
    updated["rotation"] = Value::from_number(rotation);
    return Value::from_map(std::move(updated));
}

// Register sprite system functions
void register_sprite_system(FunctionRegistry& R) {
    R.add("SPRITE", NativeFn{"SPRITE", 1, sprite_constructor});
    R.add("SPRITE_DRAW", NativeFn{"SPRITE_DRAW", 1, sprite_draw});
    R.add("SPRITE_SETPOSITION", NativeFn{"SPRITE_SETPOSITION", 3, sprite_setPosition});
    R.add("SPRITE_SETSCALE", NativeFn{"SPRITE_SETSCALE", 2, sprite_setScale});
    R.add("SPRITE_SETROTATION", NativeFn{"SPRITE_SETROTATION", 2, sprite_setRotation});
}

} // namespace bas


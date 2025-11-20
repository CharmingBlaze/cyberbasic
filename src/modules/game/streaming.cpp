#include "bas/streaming.hpp"
#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <cmath>

namespace bas {

// Streaming chunk data
struct Chunk {
    int x, y;
    bool isLoaded;
    std::vector<int> entityIds;  // Entities in this chunk
};

struct StreamingSystem {
    float cellSizeX{100.0f};
    float cellSizeY{100.0f};
    int radius{2};  // Load radius in cells
    int playerEntityId{-1};
    std::unordered_map<std::string, Chunk> chunks;  // "x,y" -> Chunk
};

static StreamingSystem g_streaming;

// Generate chunk key from coordinates
static std::string chunk_key(int x, int y) {
    return std::to_string(x) + "," + std::to_string(y);
}

// Get chunk coordinates from world position
static void world_to_chunk(float worldX, float worldY, int& chunkX, int& chunkY) {
    chunkX = static_cast<int>(std::floor(worldX / g_streaming.cellSizeX));
    chunkY = static_cast<int>(std::floor(worldY / g_streaming.cellSizeY));
}

// STREAM.setGrid(cellSizeX, cellSizeY)
static Value stream_setGrid(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::nil();
    }
    
    g_streaming.cellSizeX = static_cast<float>(args[0].as_number());
    g_streaming.cellSizeY = static_cast<float>(args[1].as_number());
    
    return Value::nil();
}

// STREAM.setRadius(cells)
static Value stream_setRadius(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::nil();
    }
    
    g_streaming.radius = args[0].as_int();
    return Value::nil();
}

// STREAM.pinPlayer(entityId)
static Value stream_pinPlayer(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::nil();
    }
    
    g_streaming.playerEntityId = args[0].as_int();
    return Value::nil();
}

// STREAM.update() - Update streaming system
static Value stream_update(const std::vector<Value>& args) {
    (void)args;
    
    if (g_streaming.playerEntityId < 0) {
        return Value::nil();
    }
    
    // Get player position (would need entity system integration)
    float playerX = 0.0f;
    float playerY = 0.0f;
    
    int playerChunkX, playerChunkY;
    world_to_chunk(playerX, playerY, playerChunkX, playerChunkY);
    
    // Determine which chunks should be loaded
    std::vector<std::string> shouldLoad;
    for (int dx = -g_streaming.radius; dx <= g_streaming.radius; ++dx) {
        for (int dy = -g_streaming.radius; dy <= g_streaming.radius; ++dy) {
            int chunkX = playerChunkX + dx;
            int chunkY = playerChunkY + dy;
            shouldLoad.push_back(chunk_key(chunkX, chunkY));
        }
    }
    
    // Unload chunks that are too far away
    for (auto it = g_streaming.chunks.begin(); it != g_streaming.chunks.end();) {
        bool shouldKeep = false;
        for (const auto& key : shouldLoad) {
            if (it->first == key) {
                shouldKeep = true;
                break;
            }
        }
        
        if (!shouldKeep) {
            // Unload chunk (would unload entities here)
            it = g_streaming.chunks.erase(it);
        } else {
            ++it;
        }
    }
    
    // Load new chunks
    for (const auto& key : shouldLoad) {
        if (g_streaming.chunks.find(key) == g_streaming.chunks.end()) {
            Chunk chunk;
            size_t commaPos = key.find(',');
            chunk.x = std::stoi(key.substr(0, commaPos));
            chunk.y = std::stoi(key.substr(commaPos + 1));
            chunk.isLoaded = true;
            g_streaming.chunks[key] = chunk;
            // Would load entities here
        }
    }
    
    return Value::nil();
}

// STREAM.getChunkCount() -> int
static Value stream_getChunkCount(const std::vector<Value>& args) {
    (void)args;
    return Value::from_int(static_cast<int>(g_streaming.chunks.size()));
}

// STREAM.getChunkAt(worldX, worldY) -> string
static Value stream_getChunkAt(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::from_string("");
    }
    
    float worldX = static_cast<float>(args[0].as_number());
    float worldY = static_cast<float>(args[1].as_number());
    
    int chunkX, chunkY;
    world_to_chunk(worldX, worldY, chunkX, chunkY);
    
    return Value::from_string(chunk_key(chunkX, chunkY));
}

// STREAM.isChunkLoaded(chunkX, chunkY) -> bool
static Value stream_isChunkLoaded(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::from_bool(false);
    }
    
    int chunkX = args[0].as_int();
    int chunkY = args[1].as_int();
    
    std::string key = chunk_key(chunkX, chunkY);
    auto it = g_streaming.chunks.find(key);
    return Value::from_bool(it != g_streaming.chunks.end() && it->second.isLoaded);
}

void register_streaming(FunctionRegistry& registry) {
    registry.add("STREAM_SETGRID", NativeFn{"STREAM_SETGRID", 2, stream_setGrid});
    registry.add("STREAM_SETRADIUS", NativeFn{"STREAM_SETRADIUS", 1, stream_setRadius});
    registry.add("STREAM_PINPLAYER", NativeFn{"STREAM_PINPLAYER", 1, stream_pinPlayer});
    registry.add("STREAM_UPDATE", NativeFn{"STREAM_UPDATE", 0, stream_update});
    registry.add("STREAM_GETCHUNKCOUNT", NativeFn{"STREAM_GETCHUNKCOUNT", 0, stream_getChunkCount});
    registry.add("STREAM_GETCHUNKAT", NativeFn{"STREAM_GETCHUNKAT", 2, stream_getChunkAt});
    registry.add("STREAM_ISCHUNKLOADED", NativeFn{"STREAM_ISCHUNKLOADED", 2, stream_isChunkLoaded});
}

}


#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include "bas/modern_features.hpp"
#include "bas/networking.hpp"
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <chrono>

namespace bas {

// Forward declarations
extern std::unique_ptr<NetworkingSystem> g_networking_system;
extern std::unique_ptr<struct AudioSystem> g_audio_system;

// ===== SET DATA STRUCTURE =====

// Hash function for Value (needed for unordered_set)
struct ValueHash {
    std::size_t operator()(const Value& v) const {
        if (v.is_string()) {
            return std::hash<std::string>{}(v.as_string());
        } else if (v.is_int()) {
            return std::hash<long long>{}(v.as_int());
        } else if (v.is_number()) {
            return std::hash<double>{}(v.as_number());
        } else if (v.is_bool()) {
            return std::hash<bool>{}(v.as_bool());
        }
        return 0;
    }
};

// Equality function for Value
struct ValueEqual {
    bool operator()(const Value& a, const Value& b) const {
        if (a.is_string() && b.is_string()) return a.as_string() == b.as_string();
        if (a.is_int() && b.is_int()) return a.as_int() == b.as_int();
        if (a.is_number() && b.is_number()) return a.as_number() == b.as_number();
        if (a.is_bool() && b.is_bool()) return a.as_bool() == b.as_bool();
        return false;
    }
};

// Set storage
struct SetData {
    std::unordered_set<Value, ValueHash, ValueEqual> elements;
    int id;
};

static std::unordered_map<int, SetData> g_sets;
static int g_next_set_id = 1;

// Set() constructor
static Value set_constructor(const std::vector<Value>& args) {
    int id = g_next_set_id++;
    SetData& set = g_sets[id];
    set.id = id;
    
    // Add initial elements
    for (const auto& arg : args) {
        set.elements.insert(arg);
    }
    
    Value::Map obj;
    obj["_type"] = Value::from_string("Set");
    obj["_id"] = Value::from_int(id);
    obj["size"] = Value::from_int(static_cast<long long>(set.elements.size()));
    
    return Value::from_map(std::move(obj));
}

// Set.add(element)
static Value set_add(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return args[0];
    }
    
    int setId = idIt->second.as_int();
    auto setIt = g_sets.find(setId);
    if (setIt == g_sets.end()) {
        return args[0];
    }
    
    if (args.size() > 1) {
        setIt->second.elements.insert(args[1]);
    }
    
    Value::Map updated = map;
    updated["size"] = Value::from_int(static_cast<long long>(setIt->second.elements.size()));
    
    return Value::from_map(std::move(updated));
}

// Set.remove(element)
static Value set_remove(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return args[0];
    }
    
    int setId = idIt->second.as_int();
    auto setIt = g_sets.find(setId);
    if (setIt != g_sets.end()) {
        setIt->second.elements.erase(args[1]);
    }
    
    Value::Map updated = map;
    updated["size"] = Value::from_int(static_cast<long long>(setIt->second.elements.size()));
    
    return Value::from_map(std::move(updated));
}

// Set.contains(element) -> bool
static Value set_contains(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::from_bool(false);
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::from_bool(false);
    }
    
    int setId = idIt->second.as_int();
    auto setIt = g_sets.find(setId);
    if (setIt == g_sets.end()) {
        return Value::from_bool(false);
    }
    
    return Value::from_bool(setIt->second.elements.count(args[1]) > 0);
}

// Set.size() -> int
static Value set_size(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::from_int(0);
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::from_int(0);
    }
    
    int setId = idIt->second.as_int();
    auto setIt = g_sets.find(setId);
    if (setIt == g_sets.end()) {
        return Value::from_int(0);
    }
    
    return Value::from_int(static_cast<long long>(setIt->second.elements.size()));
}

// ===== HIGH-LEVEL NETWORKING COMMANDS =====

// CONNECT SERVER address, port
static Value connect_server(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::nil();
    }
    
    std::string address = args[0].as_string();
    int port = args[1].as_int();
    
    if (!g_networking_system) {
        return Value::nil();
    }
    
    // Create and connect client
    int clientId = g_networking_system->create_client("Client", address, port);
    bool connected = g_networking_system->connect_client(clientId);
    
    if (connected) {
        Value::Map clientObj;
        clientObj["_type"] = Value::from_string("NetworkClient");
        clientObj["_id"] = Value::from_int(clientId);
        clientObj["address"] = Value::from_string(address);
        clientObj["port"] = Value::from_int(port);
        clientObj["connected"] = Value::from_bool(true);
        return Value::from_map(std::move(clientObj));
    }
    
    return Value::nil();
}

// CREATE SERVER port, [maxClients]
static Value create_server(const std::vector<Value>& args) {
    if (args.empty()) {
        return Value::nil();
    }
    
    int port = args[0].as_int();
    int maxClients = args.size() > 1 ? args[1].as_int() : 32;
    
    if (!g_networking_system) {
        return Value::nil();
    }
    
    int serverId = g_networking_system->create_server("Server", port, maxClients);
    bool started = g_networking_system->start_server(serverId);
    
    if (started) {
        Value::Map serverObj;
        serverObj["_type"] = Value::from_string("NetworkServer");
        serverObj["_id"] = Value::from_int(serverId);
        serverObj["port"] = Value::from_int(port);
        serverObj["maxClients"] = Value::from_int(maxClients);
        serverObj["running"] = Value::from_bool(true);
        return Value::from_map(std::move(serverObj));
    }
    
    return Value::nil();
}

// SEND MESSAGE client/server, message
static Value send_message(const std::vector<Value>& args) {
    if (args.size() < 2 || !g_networking_system) {
        return Value::nil();
    }
    
    // Get client/server ID
    int id = 0;
    if (args[0].is_map()) {
        const auto& map = args[0].as_map();
        auto idIt = map.find("_id");
        if (idIt != map.end() && idIt->second.is_int()) {
            id = idIt->second.as_int();
        }
    } else if (args[0].is_int()) {
        id = args[0].as_int();
    }
    
    if (id == 0) {
        return Value::nil();
    }
    
    // Create message
    std::string messageStr = args[1].as_string();
    NetworkMessage msg(MessageType::DATA, messageStr, id);
    msg.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count() / 1000.0;
    
    // Send message
    g_networking_system->send_message(id, msg);
    
    return Value::nil();
}

// RECEIVE MESSAGE [client/server] -> message or nil
static Value receive_message(const std::vector<Value>& args) {
    if (!g_networking_system) {
        return Value::nil();
    }
    
    int id = 0;
    if (!args.empty()) {
        if (args[0].is_map()) {
            const auto& map = args[0].as_map();
            auto idIt = map.find("_id");
            if (idIt != map.end() && idIt->second.is_int()) {
                id = idIt->second.as_int();
            }
        } else if (args[0].is_int()) {
            id = args[0].as_int();
        }
    }
    
    // Get message from queue
    if (g_networking_system->has_messages()) {
        NetworkMessage msg = g_networking_system->get_next_message();
        Value::Map msgObj;
        msgObj["type"] = Value::from_string(std::to_string(static_cast<int>(msg.type)));
        msgObj["data"] = Value::from_string(msg.data);
        msgObj["timestamp"] = Value::from_number(msg.timestamp);
        msgObj["senderId"] = Value::from_int(msg.sender_id);
        return Value::from_map(std::move(msgObj));
    }
    
    return Value::nil();
}

// BROADCAST server, message
static Value broadcast(const std::vector<Value>& args) {
    if (args.size() < 2 || !g_networking_system) {
        return Value::nil();
    }
    
    int serverId = 0;
    if (args[0].is_map()) {
        const auto& map = args[0].as_map();
        auto idIt = map.find("_id");
        if (idIt != map.end() && idIt->second.is_int()) {
            serverId = idIt->second.as_int();
        }
    } else if (args[0].is_int()) {
        serverId = args[0].as_int();
    }
    
    if (serverId == 0) {
        return Value::nil();
    }
    
    std::string messageStr = args[1].as_string();
    NetworkMessage msg(MessageType::DATA, messageStr, -1);
    msg.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count() / 1000.0;
    
    g_networking_system->broadcast_message(serverId, msg, -1);
    
    return Value::nil();
}

// ===== HIGH-LEVEL FILE I/O COMMANDS =====

// READ FILE "path" -> string
static Value read_file(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string path = args[0].as_string();
    
    std::ifstream file(path);
    if (!file.is_open()) {
        return Value::nil();
    }
    
    std::ostringstream oss;
    oss << file.rdbuf();
    return Value::from_string(oss.str());
}

// WRITE FILE "path", content
static Value write_file(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string path = args[0].as_string();
    std::string content = args[1].as_string();
    
    std::ofstream file(path);
    if (!file.is_open()) {
        return Value::from_bool(false);
    }
    
    file << content;
    return Value::from_bool(true);
}

// LOAD FILE "path" -> string (alias for READ FILE)
static Value load_file(const std::vector<Value>& args) {
    return read_file(args);
}

// SAVE FILE "path", content (alias for WRITE FILE)
static Value save_file(const std::vector<Value>& args) {
    return write_file(args);
}

// ===== JSON HIGH-LEVEL COMMANDS =====

// LOAD JSON "path" -> value
static Value load_json(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string path = args[0].as_string();
    
    // Read file
    std::ifstream file(path);
    if (!file.is_open()) {
        return Value::nil();
    }
    
    std::ostringstream oss;
    oss << file.rdbuf();
    std::string json = oss.str();
    
    // Parse JSON
    // Would call JSON.parse() here
    // For now, return the string
    return Value::from_string(json);
}

// SAVE JSON "path", value
static Value save_json(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string path = args[0].as_string();
    
    // Stringify JSON
    // Would call JSON.stringify() here
    // For now, write placeholder
    std::ofstream file(path);
    if (!file.is_open()) {
        return Value::from_bool(false);
    }
    
    // Would write stringified JSON
    file << "{}";
    return Value::from_bool(true);
}

// Register all modern features
void register_modern_features(FunctionRegistry& R) {
    // Set data structure
    R.add("SET", NativeFn{"SET", -1, set_constructor});
    R.add("SET_ADD", NativeFn{"SET_ADD", 2, set_add});
    R.add("SET_REMOVE", NativeFn{"SET_REMOVE", 2, set_remove});
    R.add("SET_CONTAINS", NativeFn{"SET_CONTAINS", 2, set_contains});
    R.add("SET_SIZE", NativeFn{"SET_SIZE", 1, set_size});
    
    // High-level networking
    R.add("CONNECT_SERVER", NativeFn{"CONNECT_SERVER", 2, connect_server});
    R.add("CREATE_SERVER", NativeFn{"CREATE_SERVER", -1, create_server});
    R.add("SEND_MESSAGE", NativeFn{"SEND_MESSAGE", 2, send_message});
    R.add("RECEIVE_MESSAGE", NativeFn{"RECEIVE_MESSAGE", 1, receive_message});
    R.add("BROADCAST", NativeFn{"BROADCAST", 2, broadcast});
    
    // High-level file I/O
    R.add("READ_FILE", NativeFn{"READ_FILE", 1, read_file});
    R.add("WRITE_FILE", NativeFn{"WRITE_FILE", 2, write_file});
    R.add("LOAD_FILE", NativeFn{"LOAD_FILE", 1, load_file});
    R.add("SAVE_FILE", NativeFn{"SAVE_FILE", 2, save_file});
    
    // High-level JSON
    R.add("LOAD_JSON", NativeFn{"LOAD_JSON", 1, load_json});
    R.add("SAVE_JSON", NativeFn{"SAVE_JSON", 2, save_json});
}

} // namespace bas


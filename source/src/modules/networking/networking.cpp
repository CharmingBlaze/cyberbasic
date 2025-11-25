#include "bas/networking.hpp"
#include "bas/runtime.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>

namespace bas {

// Global networking system instance
std::unique_ptr<NetworkingSystem> g_networking_system;

// NetworkingSystem implementation
NetworkingSystem::NetworkingSystem() 
    : running(false), next_server_id(0), next_client_id(0), 
      heartbeat_interval(5.0), connection_timeout(30.0) {
}

NetworkingSystem::~NetworkingSystem() {
    stop();
}

// Server management
int NetworkingSystem::create_server(const std::string& name, int port, int max_clients) {
    auto server = std::make_unique<NetworkServer>(next_server_id++, name, port, max_clients);
    int id = server->id;
    servers.push_back(std::move(server));
    return id;
}

void NetworkingSystem::remove_server(int server_id) {
    servers.erase(std::remove_if(servers.begin(), servers.end(),
        [server_id](const std::unique_ptr<NetworkServer>& server) {
            return server->id == server_id;
        }), servers.end());
}

NetworkServer* NetworkingSystem::get_server(int server_id) {
    auto it = std::find_if(servers.begin(), servers.end(),
        [server_id](const std::unique_ptr<NetworkServer>& server) {
            return server->id == server_id;
        });
    return (it != servers.end()) ? it->get() : nullptr;
}

bool NetworkingSystem::start_server(int server_id) {
    NetworkServer* server = get_server(server_id);
    if (server && !server->running) {
        server->running = true;
        start_server_thread(*server);
        return true;
    }
    return false;
}

bool NetworkingSystem::stop_server(int server_id) {
    NetworkServer* server = get_server(server_id);
    if (server && server->running) {
        server->running = false;
        stop_server_thread(*server);
        return true;
    }
    return false;
}

void NetworkingSystem::set_server_max_clients(int server_id, int max_clients) {
    NetworkServer* server = get_server(server_id);
    if (server) {
        server->max_clients = max_clients;
    }
}

// Client management
int NetworkingSystem::create_client(const std::string& name, const std::string& address, int port) {
    auto client = std::make_unique<NetworkClient>(next_client_id++, name, address, port);
    int id = client->id;
    clients.push_back(std::move(client));
    return id;
}

void NetworkingSystem::remove_client(int client_id) {
    clients.erase(std::remove_if(clients.begin(), clients.end(),
        [client_id](const std::unique_ptr<NetworkClient>& client) {
            return client->id == client_id;
        }), clients.end());
}

NetworkClient* NetworkingSystem::get_client(int client_id) {
    auto it = std::find_if(clients.begin(), clients.end(),
        [client_id](const std::unique_ptr<NetworkClient>& client) {
            return client->id == client_id;
        });
    return (it != clients.end()) ? it->get() : nullptr;
}

bool NetworkingSystem::connect_client(int client_id) {
    NetworkClient* client = get_client(client_id);
    if (client && !client->connected) {
        connect_to_server(*client);
        return client->connected;
    }
    return false;
}

bool NetworkingSystem::disconnect_client(int client_id) {
    NetworkClient* client = get_client(client_id);
    if (client && client->connected) {
        client->connected = false;
        stop_client_thread(*client);
        return true;
    }
    return false;
}

void NetworkingSystem::set_client_name(int client_id, const std::string& name) {
    NetworkClient* client = get_client(client_id);
    if (client) {
        client->name = name;
    }
}

// Message handling
void NetworkingSystem::send_message(int client_id, const NetworkMessage& message) {
    NetworkClient* client = get_client(client_id);
    if (client && client->connected) {
        send_message_to_client(*client, message);
    }
}

void NetworkingSystem::send_message_to_server(int server_id, const NetworkMessage& message) {
    NetworkServer* server = get_server(server_id);
    if (server && server->running) {
        send_message_to_server(*server, message);
    }
}

void NetworkingSystem::broadcast_message(int server_id, const NetworkMessage& message, int exclude_client_id) {
    NetworkServer* server = get_server(server_id);
    if (server && server->running) {
        broadcast_message(*server, message, exclude_client_id);
    }
}

void NetworkingSystem::send_chat_message(int client_id, const std::string& message) {
    NetworkMessage chat_msg = create_chat_message(client_id, message);
    send_message(client_id, chat_msg);
}

void NetworkingSystem::send_game_state(int client_id, const std::string& state_data) {
    NetworkMessage state_msg = create_game_state_message(client_id, state_data);
    send_message(client_id, state_msg);
}

void NetworkingSystem::send_player_action(int client_id, const std::string& action_data) {
    NetworkMessage action_msg = create_player_action_message(client_id, action_data);
    send_message(client_id, action_msg);
}

// Message receiving
bool NetworkingSystem::has_messages() {
    std::lock_guard<std::mutex> lock(global_queue_mutex);
    return !global_message_queue.empty();
}

NetworkMessage NetworkingSystem::get_next_message() {
    std::lock_guard<std::mutex> lock(global_queue_mutex);
    if (!global_message_queue.empty()) {
        NetworkMessage message = global_message_queue.front();
        global_message_queue.pop();
        return message;
    }
    return NetworkMessage();
}

std::vector<NetworkMessage> NetworkingSystem::get_all_messages() {
    std::lock_guard<std::mutex> lock(global_queue_mutex);
    std::vector<NetworkMessage> messages;
    while (!global_message_queue.empty()) {
        messages.push_back(global_message_queue.front());
        global_message_queue.pop();
    }
    return messages;
}

void NetworkingSystem::clear_message_queue() {
    std::lock_guard<std::mutex> lock(global_queue_mutex);
    while (!global_message_queue.empty()) {
        global_message_queue.pop();
    }
}

// System management
void NetworkingSystem::update() {
    process_messages();
    
    // Update servers
    for (auto& server : servers) {
        if (server->running) {
            remove_disconnected_clients(*server);
        }
    }
    
    // Update clients
    for (auto& client : clients) {
        if (client->connected) {
            check_connection_timeout(*client);
        }
    }
}

void NetworkingSystem::start() {
    running = true;
    network_thread = std::thread([this]() {
        while (running) {
            update();
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
        }
    });
}

void NetworkingSystem::stop() {
    running = false;
    if (network_thread.joinable()) {
        network_thread.join();
    }
    
    // Stop all servers
    for (auto& server : servers) {
        if (server->running) {
            stop_server(server->id);
        }
    }
    
    // Disconnect all clients
    for (auto& client : clients) {
        if (client->connected) {
            disconnect_client(client->id);
        }
    }
}

void NetworkingSystem::set_heartbeat_interval(double interval) {
    heartbeat_interval = interval;
}

void NetworkingSystem::set_connection_timeout(double timeout) {
    connection_timeout = timeout;
}

// Statistics
NetworkStats NetworkingSystem::get_server_stats(int server_id) {
    NetworkServer* server = get_server(server_id);
    if (server) {
        NetworkStats stats;
        stats.connection_time = get_current_time();
        return stats;
    }
    return NetworkStats();
}

NetworkStats NetworkingSystem::get_client_stats(int client_id) {
    NetworkClient* client = get_client(client_id);
    if (client) {
        NetworkStats stats;
        stats.connection_time = get_current_time();
        return stats;
    }
    return NetworkStats();
}

int NetworkingSystem::get_server_count() const {
    return static_cast<int>(servers.size());
}

int NetworkingSystem::get_client_count() const {
    return static_cast<int>(clients.size());
}

int NetworkingSystem::get_connected_clients_count(int server_id) {
    NetworkServer* server = get_server(server_id);
    if (server) {
        int count = 0;
        for (const auto& client : server->clients) {
            if (client->connected) {
                count++;
            }
        }
        return count;
    }
    return 0;
}

// Utility functions
bool NetworkingSystem::is_server_running(int server_id) {
    NetworkServer* server = get_server(server_id);
    return server ? server->running : false;
}

bool NetworkingSystem::is_client_connected(int client_id) {
    NetworkClient* client = get_client(client_id);
    return client ? client->connected : false;
}

std::string NetworkingSystem::get_client_address(int client_id) {
    NetworkClient* client = get_client(client_id);
    return client ? client->address : "";
}

int NetworkingSystem::get_client_port(int client_id) {
    NetworkClient* client = get_client(client_id);
    return client ? client->port : 0;
}

void NetworkingSystem::set_message_reliable(int client_id, bool reliable) {
    // This would set reliability for future messages
    (void)client_id; (void)reliable; // Suppress unused parameter warnings
}

// Pre-built message types
NetworkMessage NetworkingSystem::create_connect_message(int client_id) {
    return NetworkMessage(MessageType::CONNECT, "connect", client_id);
}

NetworkMessage NetworkingSystem::create_disconnect_message(int client_id) {
    return NetworkMessage(MessageType::DISCONNECT, "disconnect", client_id);
}

NetworkMessage NetworkingSystem::create_heartbeat_message(int client_id) {
    return NetworkMessage(MessageType::HEARTBEAT, "heartbeat", client_id);
}

NetworkMessage NetworkingSystem::create_chat_message(int client_id, const std::string& text) {
    return NetworkMessage(MessageType::CHAT, text, client_id);
}

NetworkMessage NetworkingSystem::create_game_state_message(int client_id, const std::string& state) {
    return NetworkMessage(MessageType::GAME_STATE, state, client_id);
}

NetworkMessage NetworkingSystem::create_player_action_message(int client_id, const std::string& action) {
    return NetworkMessage(MessageType::PLAYER_ACTION, action, client_id);
}

// Private helper methods
void NetworkingSystem::process_messages() {
    // Process messages from all servers and clients
    for (auto& server : servers) {
        std::lock_guard<std::mutex> lock(server->queue_mutex);
        while (!server->message_queue.empty()) {
            NetworkMessage message = server->message_queue.front();
            server->message_queue.pop();
            
            // Add to global queue
            std::lock_guard<std::mutex> global_lock(global_queue_mutex);
            global_message_queue.push(message);
        }
    }
    
    for (auto& client : clients) {
        std::lock_guard<std::mutex> lock(client->queue_mutex);
        while (!client->message_queue.empty()) {
            NetworkMessage message = client->message_queue.front();
            client->message_queue.pop();
            
            // Add to global queue
            std::lock_guard<std::mutex> global_lock(global_queue_mutex);
            global_message_queue.push(message);
        }
    }
}

void NetworkingSystem::handle_connect_message(const NetworkMessage& message) {
    (void)message; // Suppress unused parameter warning
    // Handle connection message
}

void NetworkingSystem::handle_disconnect_message(const NetworkMessage& message) {
    (void)message; // Suppress unused parameter warning
    // Handle disconnection message
}

void NetworkingSystem::handle_data_message(const NetworkMessage& message) {
    (void)message; // Suppress unused parameter warning
    // Handle data message
}

void NetworkingSystem::handle_heartbeat_message(const NetworkMessage& message) {
    (void)message; // Suppress unused parameter warning
    // Handle heartbeat message
}

void NetworkingSystem::handle_chat_message(const NetworkMessage& message) {
    (void)message; // Suppress unused parameter warning
    // Handle chat message
}

void NetworkingSystem::handle_game_state_message(const NetworkMessage& message) {
    (void)message; // Suppress unused parameter warning
    // Handle game state message
}

void NetworkingSystem::handle_player_action_message(const NetworkMessage& message) {
    (void)message; // Suppress unused parameter warning
    // Handle player action message
}

void NetworkingSystem::start_server_thread(NetworkServer& server) {
    // This would start the server thread
    (void)server; // Suppress unused parameter warning
}

void NetworkingSystem::stop_server_thread(NetworkServer& server) {
    // This would stop the server thread
    (void)server; // Suppress unused parameter warning
}

void NetworkingSystem::accept_connections(NetworkServer& server) {
    // This would accept incoming connections
    (void)server; // Suppress unused parameter warning
}

void NetworkingSystem::handle_client_connection(NetworkServer& server, int client_socket) {
    // This would handle a client connection
    (void)server; (void)client_socket; // Suppress unused parameter warnings
}

void NetworkingSystem::remove_disconnected_clients(NetworkServer& server) {
    // Remove disconnected clients
    server.clients.erase(
        std::remove_if(server.clients.begin(), server.clients.end(),
            [](const std::unique_ptr<NetworkClient>& client) {
                return !client->connected;
            }),
        server.clients.end());
}

void NetworkingSystem::start_client_thread(NetworkClient& client) {
    // This would start the client thread
    (void)client; // Suppress unused parameter warning
}

void NetworkingSystem::stop_client_thread(NetworkClient& client) {
    // This would stop the client thread
    (void)client; // Suppress unused parameter warning
}

void NetworkingSystem::connect_to_server(NetworkClient& client) {
    // This would connect to the server
    client.connected = true;
    client.last_heartbeat = get_current_time();
}

void NetworkingSystem::send_heartbeat(NetworkClient& client) {
    // This would send a heartbeat
    client.last_heartbeat = get_current_time();
}

void NetworkingSystem::check_connection_timeout(NetworkClient& client) {
    double current_time = get_current_time();
    if (current_time - client.last_heartbeat > connection_timeout) {
        client.connected = false;
    }
}

double NetworkingSystem::get_current_time() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration<double>(duration).count();
}

std::string NetworkingSystem::serialize_message(const NetworkMessage& message) {
    // Simple serialization - in a real implementation, this would be more robust
    std::ostringstream oss;
    oss << static_cast<int>(message.type) << "|" << message.data << "|" 
        << message.sender_id << "|" << message.receiver_id << "|" 
        << message.timestamp << "|" << (message.reliable ? "1" : "0");
    return oss.str();
}

NetworkMessage NetworkingSystem::deserialize_message(const std::string& data) {
    // Simple deserialization - in a real implementation, this would be more robust
    NetworkMessage message;
    std::istringstream iss(data);
    std::string token;
    
    if (std::getline(iss, token, '|')) {
        message.type = static_cast<MessageType>(std::stoi(token));
    }
    if (std::getline(iss, token, '|')) {
        message.data = token;
    }
    if (std::getline(iss, token, '|')) {
        message.sender_id = std::stoi(token);
    }
    if (std::getline(iss, token, '|')) {
        message.receiver_id = std::stoi(token);
    }
    if (std::getline(iss, token, '|')) {
        message.timestamp = std::stod(token);
    }
    if (std::getline(iss, token, '|')) {
        message.reliable = (token == "1");
    }
    
    return message;
}

bool NetworkingSystem::send_message_to_client(NetworkClient& client, const NetworkMessage& message) {
    // This would send a message to the client
    (void)client; (void)message; // Suppress unused parameter warnings
    return true;
}

bool NetworkingSystem::send_message_to_server(NetworkServer& server, const NetworkMessage& message) {
    // This would send a message to the server
    (void)server; (void)message; // Suppress unused parameter warnings
    return true;
}

void NetworkingSystem::broadcast_message(NetworkServer& server, const NetworkMessage& message, int exclude_client_id) {
    // This would broadcast a message to all clients
    (void)server; (void)message; (void)exclude_client_id; // Suppress unused parameter warnings
}

// Native function implementations
Value networking_init_system(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_networking_system) {
        g_networking_system = std::make_unique<NetworkingSystem>();
    }
    return Value::nil();
}

Value networking_create_server(const std::vector<Value>& args) {
    if (args.size() != 3 || !g_networking_system) return Value::from_int(-1);
    
    std::string name = args[0].as_string();
    int port = static_cast<int>(args[1].as_int());
    int max_clients = static_cast<int>(args[2].as_int());
    
    int server_id = g_networking_system->create_server(name, port, max_clients);
    return Value::from_int(server_id);
}

Value networking_create_client(const std::vector<Value>& args) {
    if (args.size() != 3 || !g_networking_system) return Value::from_int(-1);
    
    std::string name = args[0].as_string();
    std::string address = args[1].as_string();
    int port = static_cast<int>(args[2].as_int());
    
    int client_id = g_networking_system->create_client(name, address, port);
    return Value::from_int(client_id);
}

Value networking_start_server(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_networking_system) return Value::from_bool(false);
    
    int server_id = static_cast<int>(args[0].as_int());
    bool success = g_networking_system->start_server(server_id);
    return Value::from_bool(success);
}

Value networking_connect_client(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_networking_system) return Value::from_bool(false);
    
    int client_id = static_cast<int>(args[0].as_int());
    bool success = g_networking_system->connect_client(client_id);
    return Value::from_bool(success);
}

Value networking_send_chat_message(const std::vector<Value>& args) {
    if (args.size() != 2 || !g_networking_system) return Value::nil();
    
    int client_id = static_cast<int>(args[0].as_int());
    std::string message = args[1].as_string();
    
    g_networking_system->send_chat_message(client_id, message);
    return Value::nil();
}

Value networking_has_messages(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_networking_system) return Value::from_bool(false);
    
    bool has_msg = g_networking_system->has_messages();
    return Value::from_bool(has_msg);
}

Value networking_get_next_message(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_networking_system) return Value::from_string("");
    
    NetworkMessage message = g_networking_system->get_next_message();
    return Value::from_string(message.data);
}

Value networking_update_networking_system(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_networking_system) return Value::nil();
    
    g_networking_system->update();
    return Value::nil();
}

Value networking_get_server_count(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_networking_system) return Value::from_int(0);
    
    int count = g_networking_system->get_server_count();
    return Value::from_int(count);
}

Value networking_get_client_count(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_networking_system) return Value::from_int(0);
    
    int count = g_networking_system->get_client_count();
    return Value::from_int(count);
}

void register_networking_functions(FunctionRegistry& registry) {
    registry.add("INITNETWORKING", NativeFn{"INITNETWORKING", 0, networking_init_system});
    registry.add("CREATENETWORKSERVER", NativeFn{"CREATENETWORKSERVER", 3, networking_create_server});
    registry.add("CREATENETWORKCLIENT", NativeFn{"CREATENETWORKCLIENT", 3, networking_create_client});
    registry.add("STARTNETWORKSERVER", NativeFn{"STARTNETWORKSERVER", 1, networking_start_server});
    registry.add("CONNECTNETWORKCLIENT", NativeFn{"CONNECTNETWORKCLIENT", 1, networking_connect_client});
    registry.add("SENDCHATMESSAGE", NativeFn{"SENDCHATMESSAGE", 2, networking_send_chat_message});
    registry.add("NETWORKHASMESSAGES", NativeFn{"NETWORKHASMESSAGES", 0, networking_has_messages});
    registry.add("NETWORKGETNEXTMESSAGE", NativeFn{"NETWORKGETNEXTMESSAGE", 0, networking_get_next_message});
    registry.add("UPDATENETWORKINGSYSTEM", NativeFn{"UPDATENETWORKINGSYSTEM", 0, networking_update_networking_system});
    registry.add("GETNETWORKSERVERCOUNT", NativeFn{"GETNETWORKSERVERCOUNT", 0, networking_get_server_count});
    registry.add("GETNETWORKCLIENTCOUNT", NativeFn{"GETNETWORKCLIENTCOUNT", 0, networking_get_client_count});
}

} // namespace bas

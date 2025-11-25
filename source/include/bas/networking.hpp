#pragma once

#include "value.hpp"
#include "runtime.hpp"
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>

namespace bas {

// Forward declarations
struct NetworkMessage;
struct NetworkClient;
struct NetworkServer;
class NetworkingSystem;

// Network message types
enum class MessageType {
    CONNECT,
    DISCONNECT,
    DATA,
    HEARTBEAT,
    CHAT,
    GAME_STATE,
    PLAYER_ACTION,
    CUSTOM
};

// Network message structure
struct NetworkMessage {
    int id;
    MessageType type;
    std::string data;
    int sender_id;
    int receiver_id;
    double timestamp;
    bool reliable;
    
    NetworkMessage() : id(0), type(MessageType::DATA), sender_id(-1), receiver_id(-1), 
                       timestamp(0.0), reliable(false) {}
    
    NetworkMessage(MessageType type, const std::string& data, int sender_id = -1, int receiver_id = -1)
        : id(0), type(type), data(data), sender_id(sender_id), receiver_id(receiver_id),
          timestamp(0.0), reliable(false) {}
};

// Network client structure
struct NetworkClient {
    int id;
    std::string name;
    std::string address;
    int port;
    bool connected;
    double last_heartbeat;
    std::queue<NetworkMessage> message_queue;
    std::mutex queue_mutex;
    
    NetworkClient(int id, const std::string& name, const std::string& address, int port)
        : id(id), name(name), address(address), port(port), connected(false), last_heartbeat(0.0) {}
};

// Network server structure
struct NetworkServer {
    int id;
    std::string name;
    int port;
    int max_clients;
    bool running;
    std::vector<std::unique_ptr<NetworkClient>> clients;
    std::queue<NetworkMessage> message_queue;
    std::mutex queue_mutex;
    std::atomic<int> next_client_id;
    
    NetworkServer(int id, const std::string& name, int port, int max_clients = 32)
        : id(id), name(name), port(port), max_clients(max_clients), running(false), next_client_id(1) {}
};

// Network connection states
enum class ConnectionState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    RECONNECTING,
    ERROR
};

// Network statistics
struct NetworkStats {
    int messages_sent;
    int messages_received;
    int bytes_sent;
    int bytes_received;
    double connection_time;
    double last_activity;
    
    NetworkStats() : messages_sent(0), messages_received(0), bytes_sent(0), bytes_received(0),
                     connection_time(0.0), last_activity(0.0) {}
};

// Networking system
class NetworkingSystem {
private:
    std::vector<std::unique_ptr<NetworkServer>> servers;
    std::vector<std::unique_ptr<NetworkClient>> clients;
    std::queue<NetworkMessage> global_message_queue;
    std::mutex global_queue_mutex;
    std::thread network_thread;
    std::atomic<bool> running;
    int next_server_id;
    int next_client_id;
    double heartbeat_interval;
    double connection_timeout;
    
    // Message handling
    void process_messages();
    void handle_connect_message(const NetworkMessage& message);
    void handle_disconnect_message(const NetworkMessage& message);
    void handle_data_message(const NetworkMessage& message);
    void handle_heartbeat_message(const NetworkMessage& message);
    void handle_chat_message(const NetworkMessage& message);
    void handle_game_state_message(const NetworkMessage& message);
    void handle_player_action_message(const NetworkMessage& message);
    
    // Server management
    void start_server_thread(NetworkServer& server);
    void stop_server_thread(NetworkServer& server);
    void accept_connections(NetworkServer& server);
    void handle_client_connection(NetworkServer& server, int client_socket);
    void remove_disconnected_clients(NetworkServer& server);
    
    // Client management
    void start_client_thread(NetworkClient& client);
    void stop_client_thread(NetworkClient& client);
    void connect_to_server(NetworkClient& client);
    void send_heartbeat(NetworkClient& client);
    void check_connection_timeout(NetworkClient& client);
    
    // Utility functions
    double get_current_time();
    std::string serialize_message(const NetworkMessage& message);
    NetworkMessage deserialize_message(const std::string& data);
    bool send_message_to_client(NetworkClient& client, const NetworkMessage& message);
    bool send_message_to_server(NetworkServer& server, const NetworkMessage& message);
    void broadcast_message(NetworkServer& server, const NetworkMessage& message, int exclude_client_id = -1);
    
public:
    NetworkingSystem();
    ~NetworkingSystem();
    
    // Server management
    int create_server(const std::string& name, int port, int max_clients = 32);
    void remove_server(int server_id);
    NetworkServer* get_server(int server_id);
    bool start_server(int server_id);
    bool stop_server(int server_id);
    void set_server_max_clients(int server_id, int max_clients);
    
    // Client management
    int create_client(const std::string& name, const std::string& address, int port);
    void remove_client(int client_id);
    NetworkClient* get_client(int client_id);
    bool connect_client(int client_id);
    bool disconnect_client(int client_id);
    void set_client_name(int client_id, const std::string& name);
    
    // Message handling
    void send_message(int client_id, const NetworkMessage& message);
    void send_message_to_server(int server_id, const NetworkMessage& message);
    void broadcast_message(int server_id, const NetworkMessage& message, int exclude_client_id = -1);
    void send_chat_message(int client_id, const std::string& message);
    void send_game_state(int client_id, const std::string& state_data);
    void send_player_action(int client_id, const std::string& action_data);
    
    // Message receiving
    bool has_messages();
    NetworkMessage get_next_message();
    std::vector<NetworkMessage> get_all_messages();
    void clear_message_queue();
    
    // System management
    void update();
    void start();
    void stop();
    void set_heartbeat_interval(double interval);
    void set_connection_timeout(double timeout);
    
    // Statistics
    NetworkStats get_server_stats(int server_id);
    NetworkStats get_client_stats(int client_id);
    int get_server_count() const;
    int get_client_count() const;
    int get_connected_clients_count(int server_id);
    
    // Utility functions
    bool is_server_running(int server_id);
    bool is_client_connected(int client_id);
    std::string get_client_address(int client_id);
    int get_client_port(int client_id);
    void set_message_reliable(int client_id, bool reliable);
    
    // Pre-built message types
    NetworkMessage create_connect_message(int client_id);
    NetworkMessage create_disconnect_message(int client_id);
    NetworkMessage create_heartbeat_message(int client_id);
    NetworkMessage create_chat_message(int client_id, const std::string& text);
    NetworkMessage create_game_state_message(int client_id, const std::string& state);
    NetworkMessage create_player_action_message(int client_id, const std::string& action);
};

// Global networking system instance
extern std::unique_ptr<NetworkingSystem> g_networking_system;

// Native function declarations
void register_networking_functions(FunctionRegistry& registry);

} // namespace bas

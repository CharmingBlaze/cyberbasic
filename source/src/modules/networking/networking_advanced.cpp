#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include "bas/networking_advanced.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
    #define close(s) closesocket(s)
    #define SOCKET_ERROR_CODE WSAGetLastError()
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <fcntl.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define SOCKET_ERROR_CODE errno
#endif

namespace bas {

// Socket connection storage
struct SocketConnection {
    SOCKET socket;
    std::string address;
    int port;
    bool connected;
    std::string buffer;
    std::mutex buffer_mutex;
    
    SocketConnection() : socket(INVALID_SOCKET), port(0), connected(false) {}
};

struct WebSocketConnection {
    SOCKET socket;
    std::string url;
    bool connected;
    std::string buffer;
    std::mutex buffer_mutex;
    
    WebSocketConnection() : socket(INVALID_SOCKET), connected(false) {}
};

struct HttpConnection {
    std::string url;
    std::string response;
    bool completed;
    
    HttpConnection() : completed(false) {}
};

// Global storage
static std::unordered_map<int, SocketConnection> g_tcp_connections;
static std::unordered_map<int, SocketConnection> g_udp_sockets;
static std::unordered_map<int, WebSocketConnection> g_websocket_connections;
static std::unordered_map<int, HttpConnection> g_http_requests;
static int g_next_tcp_id = 1;
static int g_next_udp_id = 1;
static int g_next_websocket_id = 1;
static int g_next_http_id = 1;
static std::mutex g_networking_mutex;
static bool g_winsock_initialized = false;

// Initialize networking (Windows only)
static void init_networking() {
#ifdef _WIN32
    if (!g_winsock_initialized) {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
        g_winsock_initialized = true;
    }
#endif
}

// Cleanup networking (Windows only)
static void cleanup_networking() {
#ifdef _WIN32
    if (g_winsock_initialized) {
        WSACleanup();
        g_winsock_initialized = false;
    }
#endif
}

// Parse URL
static bool parse_url(const std::string& url, std::string& host, std::string& path, int& port, bool& is_https) {
    is_https = (url.find("https://") == 0);
    std::string protocol = is_https ? "https://" : "http://";
    
    size_t start = url.find(protocol);
    if (start == std::string::npos) {
        start = 0;
    } else {
        start += protocol.length();
    }
    
    size_t path_start = url.find('/', start);
    size_t port_start = url.find(':', start);
    
    if (port_start != std::string::npos && (path_start == std::string::npos || port_start < path_start)) {
        host = url.substr(start, port_start - start);
        size_t port_end = (path_start != std::string::npos) ? path_start : url.length();
        port = std::stoi(url.substr(port_start + 1, port_end - port_start - 1));
    } else {
        host = (path_start != std::string::npos) ? url.substr(start, path_start - start) : url.substr(start);
        port = is_https ? 443 : 80;
    }
    
    path = (path_start != std::string::npos) ? url.substr(path_start) : "/";
    return true;
}

// Resolve hostname to IP
static std::string resolve_hostname(const std::string& hostname) {
    struct addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    struct addrinfo* result = nullptr;
    if (getaddrinfo(hostname.c_str(), nullptr, &hints, &result) != 0) {
        return "";
    }
    
    char ip[INET_ADDRSTRLEN] = {};
    for (struct addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        if (ptr->ai_family == AF_INET) {
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)ptr->ai_addr;
            inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);
            break;
        }
    }
    
    freeaddrinfo(result);
    return std::string(ip);
}

// HTTP GET implementation
Value http_get_impl(const std::vector<Value>& args) {
    if (args.empty()) return Value::from_string("HTTP GET: URL required");
    
    init_networking();
    std::string url = args[0].as_string();
    
    std::string host, path;
    int port;
    bool is_https;
    
    if (!parse_url(url, host, path, port, is_https)) {
        return Value::from_string("Invalid URL");
    }
    
    // For HTTPS, we'd need SSL/TLS support - simplified for now
    if (is_https) {
        return Value::from_string("HTTPS not yet supported (requires SSL/TLS)");
    }
    
    std::string ip = resolve_hostname(host);
    if (ip.empty()) {
        return Value::from_string("Failed to resolve hostname: " + host);
    }
    
    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        return Value::from_string("Failed to create socket");
    }
    
    // Connect
    struct sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);
    
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        close(sock);
        return Value::from_string("Failed to connect to " + host);
    }
    
    // Send HTTP GET request
    std::ostringstream request;
    request << "GET " << path << " HTTP/1.1\r\n";
    request << "Host: " << host << "\r\n";
    request << "Connection: close\r\n";
    request << "\r\n";
    
    std::string request_str = request.str();
    send(sock, request_str.c_str(), static_cast<int>(request_str.length()), 0);
    
    // Receive response
    std::string response;
    char buffer[4096];
    int bytes_received;
    
    while ((bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        response += buffer;
    }
    
    close(sock);
    
    // Extract body (skip headers)
    size_t body_start = response.find("\r\n\r\n");
    if (body_start != std::string::npos) {
        response = response.substr(body_start + 4);
    }
    
    return Value::from_string(response);
}

// HTTP POST implementation
Value http_post_impl(const std::vector<Value>& args) {
    if (args.size() < 2) return Value::from_string("HTTP POST: URL and data required");
    
    init_networking();
    std::string url = args[0].as_string();
    std::string data = args[1].as_string();
    
    std::string host, path;
    int port;
    bool is_https;
    
    if (!parse_url(url, host, path, port, is_https)) {
        return Value::from_string("Invalid URL");
    }
    
    if (is_https) {
        return Value::from_string("HTTPS not yet supported (requires SSL/TLS)");
    }
    
    std::string ip = resolve_hostname(host);
    if (ip.empty()) {
        return Value::from_string("Failed to resolve hostname: " + host);
    }
    
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        return Value::from_string("Failed to create socket");
    }
    
    struct sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);
    
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        close(sock);
        return Value::from_string("Failed to connect to " + host);
    }
    
    // Send HTTP POST request
    std::ostringstream request;
    request << "POST " << path << " HTTP/1.1\r\n";
    request << "Host: " << host << "\r\n";
    request << "Content-Type: application/x-www-form-urlencoded\r\n";
    request << "Content-Length: " << data.length() << "\r\n";
    request << "Connection: close\r\n";
    request << "\r\n";
    request << data;
    
    std::string request_str = request.str();
    send(sock, request_str.c_str(), static_cast<int>(request_str.length()), 0);
    
    // Receive response
    std::string response;
    char buffer[4096];
    int bytes_received;
    
    while ((bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        response += buffer;
    }
    
    close(sock);
    
    // Extract body
    size_t body_start = response.find("\r\n\r\n");
    if (body_start != std::string::npos) {
        response = response.substr(body_start + 4);
    }
    
    return Value::from_string(response);
}

// Download file implementation
Value download_file_impl(const std::vector<Value>& args) {
    if (args.size() < 2) return Value::from_bool(false);
    
    init_networking();
    std::string url = args[0].as_string();
    std::string filename = args[1].as_string();
    
    // Get HTTP response
    Value response = http_get_impl({Value::from_string(url)});
    std::string content = response.as_string();
    
    // Write to file
    FILE* file = fopen(filename.c_str(), "wb");
    if (!file) {
        return Value::from_bool(false);
    }
    
    fwrite(content.c_str(), 1, content.length(), file);
    fclose(file);
    
    return Value::from_bool(true);
}

// Upload file implementation
Value upload_file_impl(const std::vector<Value>& args) {
    if (args.size() < 2) return Value::from_bool(false);
    
    init_networking();
    std::string url = args[0].as_string();
    std::string filename = args[1].as_string();
    
    // Read file
    FILE* file = fopen(filename.c_str(), "rb");
    if (!file) {
        return Value::from_bool(false);
    }
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    std::vector<char> buffer(file_size);
    fread(buffer.data(), 1, file_size, file);
    fclose(file);
    
    std::string data(buffer.data(), file_size);
    
    // POST to URL
    Value response = http_post_impl({Value::from_string(url), Value::from_string(data)});
    
    return Value::from_bool(!response.as_string().empty());
}

// TCP Connect implementation
Value tcp_connect_impl(const std::vector<Value>& args) {
    if (args.size() < 2) return Value::from_int(-1);
    
    init_networking();
    std::string address = args[0].as_string();
    int port = static_cast<int>(args[1].as_int());
    
    std::lock_guard<std::mutex> lock(g_networking_mutex);
    
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        return Value::from_int(-1);
    }
    
    struct sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    // Try to parse as IP address first
    if (inet_pton(AF_INET, address.c_str(), &server_addr.sin_addr) != 1) {
        // Resolve hostname
        std::string ip = resolve_hostname(address);
        if (ip.empty() || inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) != 1) {
            close(sock);
            return Value::from_int(-1);
        }
    }
    
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        close(sock);
        return Value::from_int(-1);
    }
    
    int id = g_next_tcp_id++;
    SocketConnection& conn = g_tcp_connections[id];
    conn.socket = sock;
    conn.address = address;
    conn.port = port;
    conn.connected = true;
    
    return Value::from_int(id);
}

// TCP Send implementation
Value tcp_send_impl(const std::vector<Value>& args) {
    if (args.size() < 2) return Value::from_bool(false);
    
    std::lock_guard<std::mutex> lock(g_networking_mutex);
    
    int id = static_cast<int>(args[0].as_int());
    auto it = g_tcp_connections.find(id);
    if (it == g_tcp_connections.end() || !it->second.connected) {
        return Value::from_bool(false);
    }
    
    std::string data = args[1].as_string();
    int sent = send(it->second.socket, data.c_str(), static_cast<int>(data.length()), 0);
    
    return Value::from_bool(sent > 0);
}

// TCP Receive implementation
Value tcp_receive_impl(const std::vector<Value>& args) {
    if (args.empty()) return Value::from_string("");
    
    std::lock_guard<std::mutex> lock(g_networking_mutex);
    
    int id = static_cast<int>(args[0].as_int());
    auto it = g_tcp_connections.find(id);
    if (it == g_tcp_connections.end() || !it->second.connected) {
        return Value::from_string("");
    }
    
    char buffer[4096];
    int received = recv(it->second.socket, buffer, sizeof(buffer) - 1, 0);
    
    if (received > 0) {
        buffer[received] = '\0';
        return Value::from_string(std::string(buffer, received));
    }
    
    return Value::from_string("");
}

// TCP Close implementation
Value tcp_close_impl(const std::vector<Value>& args) {
    if (args.empty()) return Value::nil();
    
    std::lock_guard<std::mutex> lock(g_networking_mutex);
    
    int id = static_cast<int>(args[0].as_int());
    auto it = g_tcp_connections.find(id);
    if (it != g_tcp_connections.end()) {
        if (it->second.socket != INVALID_SOCKET) {
            close(it->second.socket);
        }
        g_tcp_connections.erase(it);
    }
    
    return Value::nil();
}

// UDP Create implementation
Value udp_create_impl(const std::vector<Value>& args) {
    init_networking();
    
    int port = args.empty() ? 0 : static_cast<int>(args[0].as_int());
    
    std::lock_guard<std::mutex> lock(g_networking_mutex);
    
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        return Value::from_int(-1);
    }
    
    // Bind to port if specified
    if (port > 0) {
        struct sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);
        
        if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
            close(sock);
            return Value::from_int(-1);
        }
    }
    
    int id = g_next_udp_id++;
    SocketConnection& conn = g_udp_sockets[id];
    conn.socket = sock;
    conn.port = port;
    conn.connected = true;
    
    return Value::from_int(id);
}

// UDP Send implementation
Value udp_send_impl(const std::vector<Value>& args) {
    if (args.size() < 4) return Value::from_bool(false);
    
    std::lock_guard<std::mutex> lock(g_networking_mutex);
    
    int id = static_cast<int>(args[0].as_int());
    std::string data = args[1].as_string();
    std::string address = args[2].as_string();
    int port = static_cast<int>(args[3].as_int());
    
    auto it = g_udp_sockets.find(id);
    if (it == g_udp_sockets.end() || !it->second.connected) {
        return Value::from_bool(false);
    }
    
    struct sockaddr_in dest_addr = {};
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, address.c_str(), &dest_addr.sin_addr) != 1) {
        std::string ip = resolve_hostname(address);
        if (ip.empty() || inet_pton(AF_INET, ip.c_str(), &dest_addr.sin_addr) != 1) {
            return Value::from_bool(false);
        }
    }
    
    int sent = sendto(it->second.socket, data.c_str(), static_cast<int>(data.length()), 0,
                      (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    
    return Value::from_bool(sent > 0);
}

// UDP Receive implementation
Value udp_receive_impl(const std::vector<Value>& args) {
    if (args.empty()) return Value::from_string("");
    
    std::lock_guard<std::mutex> lock(g_networking_mutex);
    
    int id = static_cast<int>(args[0].as_int());
    auto it = g_udp_sockets.find(id);
    if (it == g_udp_sockets.end() || !it->second.connected) {
        return Value::from_string("");
    }
    
    char buffer[4096];
    struct sockaddr_in from_addr = {};
    socklen_t from_len = sizeof(from_addr);
    
    // Set non-blocking for receive
#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(it->second.socket, FIONBIO, &mode);
#else
    int flags = fcntl(it->second.socket, F_GETFL, 0);
    fcntl(it->second.socket, F_SETFL, flags | O_NONBLOCK);
#endif
    
    int received = recvfrom(it->second.socket, buffer, sizeof(buffer) - 1, 0,
                           (struct sockaddr*)&from_addr, &from_len);
    
    if (received > 0) {
        buffer[received] = '\0';
        return Value::from_string(std::string(buffer, received));
    }
    
    return Value::from_string("");
}

// UDP Close implementation
Value udp_close_impl(const std::vector<Value>& args) {
    if (args.empty()) return Value::nil();
    
    std::lock_guard<std::mutex> lock(g_networking_mutex);
    
    int id = static_cast<int>(args[0].as_int());
    auto it = g_udp_sockets.find(id);
    if (it != g_udp_sockets.end()) {
        if (it->second.socket != INVALID_SOCKET) {
            close(it->second.socket);
        }
        g_udp_sockets.erase(it);
    }
    
    return Value::nil();
}

// WebSocket Connect (simplified - basic HTTP upgrade)
Value websocket_connect_impl(const std::vector<Value>& args) {
    if (args.empty()) return Value::from_int(-1);
    
    init_networking();
    std::string url = args[0].as_string();
    
    std::string host, path;
    int port;
    bool is_https;
    
    if (!parse_url(url, host, path, port, is_https)) {
        return Value::from_int(-1);
    }
    
    if (is_https) {
        return Value::from_int(-1); // HTTPS WebSocket requires SSL
    }
    
    std::string ip = resolve_hostname(host);
    if (ip.empty()) {
        return Value::from_int(-1);
    }
    
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        return Value::from_int(-1);
    }
    
    struct sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);
    
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        close(sock);
        return Value::from_int(-1);
    }
    
    // Send WebSocket upgrade request
    std::string key = "dGhlIHNhbXBsZSBub25jZQ=="; // Base64 encoded nonce
    std::ostringstream request;
    request << "GET " << path << " HTTP/1.1\r\n";
    request << "Host: " << host << "\r\n";
    request << "Upgrade: websocket\r\n";
    request << "Connection: Upgrade\r\n";
    request << "Sec-WebSocket-Key: " << key << "\r\n";
    request << "Sec-WebSocket-Version: 13\r\n";
    request << "\r\n";
    
    std::string request_str = request.str();
    send(sock, request_str.c_str(), static_cast<int>(request_str.length()), 0);
    
    // Read response (simplified - just check for 101)
    char buffer[4096];
    recv(sock, buffer, sizeof(buffer) - 1, 0);
    buffer[sizeof(buffer) - 1] = '\0';
    std::string response(buffer);
    
    if (response.find("101") == std::string::npos) {
        close(sock);
        return Value::from_int(-1);
    }
    
    std::lock_guard<std::mutex> lock(g_networking_mutex);
    int id = g_next_websocket_id++;
    WebSocketConnection& conn = g_websocket_connections[id];
    conn.socket = sock;
    conn.url = url;
    conn.connected = true;
    
    return Value::from_int(id);
}

// WebSocket Send (simplified frame format)
Value websocket_send_impl(const std::vector<Value>& args) {
    if (args.size() < 2) return Value::from_bool(false);
    
    std::lock_guard<std::mutex> lock(g_networking_mutex);
    
    int id = static_cast<int>(args[0].as_int());
    auto it = g_websocket_connections.find(id);
    if (it == g_websocket_connections.end() || !it->second.connected) {
        return Value::from_bool(false);
    }
    
    std::string data = args[1].as_string();
    
    // Simple WebSocket frame (text frame, no masking for client)
    std::vector<unsigned char> frame;
    frame.push_back(0x81); // FIN + text frame
    
    if (data.length() < 126) {
        frame.push_back(static_cast<unsigned char>(data.length()));
    } else if (data.length() < 65536) {
        frame.push_back(126);
        frame.push_back((data.length() >> 8) & 0xFF);
        frame.push_back(data.length() & 0xFF);
    } else {
        frame.push_back(127);
        for (int i = 7; i >= 0; --i) {
            frame.push_back((data.length() >> (i * 8)) & 0xFF);
        }
    }
    
    frame.insert(frame.end(), data.begin(), data.end());
    
    int sent = send(it->second.socket, (char*)frame.data(), static_cast<int>(frame.size()), 0);
    return Value::from_bool(sent > 0);
}

// WebSocket Receive (simplified frame parsing)
Value websocket_receive_impl(const std::vector<Value>& args) {
    if (args.empty()) return Value::from_string("");
    
    std::lock_guard<std::mutex> lock(g_networking_mutex);
    
    int id = static_cast<int>(args[0].as_int());
    auto it = g_websocket_connections.find(id);
    if (it == g_websocket_connections.end() || !it->second.connected) {
        return Value::from_string("");
    }
    
    unsigned char buffer[4096];
    int received = recv(it->second.socket, (char*)buffer, sizeof(buffer), 0);
    
    if (received < 2) {
        return Value::from_string("");
    }
    
    // Parse WebSocket frame (simplified)
    bool masked = (buffer[1] & 0x80) != 0;
    int payload_len = buffer[1] & 0x7F;
    int offset = 2;
    
    if (payload_len == 126) {
        payload_len = (buffer[2] << 8) | buffer[3];
        offset = 4;
    } else if (payload_len == 127) {
        payload_len = 0;
        for (int i = 0; i < 8; ++i) {
            payload_len = (payload_len << 8) | buffer[2 + i];
        }
        offset = 10;
    }
    
    if (masked) {
        offset += 4; // Skip mask
    }
    
    if (offset + payload_len > received) {
        return Value::from_string("");
    }
    
    std::string message((char*)buffer + offset, payload_len);
    return Value::from_string(message);
}

// WebSocket Close
Value websocket_close_impl(const std::vector<Value>& args) {
    if (args.empty()) return Value::nil();
    
    std::lock_guard<std::mutex> lock(g_networking_mutex);
    
    int id = static_cast<int>(args[0].as_int());
    auto it = g_websocket_connections.find(id);
    if (it != g_websocket_connections.end()) {
        if (it->second.socket != INVALID_SOCKET) {
            close(it->second.socket);
        }
        g_websocket_connections.erase(it);
    }
    
    return Value::nil();
}

// Register advanced networking functions
void register_advanced_networking_functions(FunctionRegistry& R) {
    R.add("HTTPGET", NativeFn{"HTTPGET", 1, http_get_impl});
    R.add("HTTPPOST", NativeFn{"HTTPPOST", 2, http_post_impl});
    R.add("DOWNLOADFILE", NativeFn{"DOWNLOADFILE", 2, download_file_impl});
    R.add("UPLOADFILE", NativeFn{"UPLOADFILE", 2, upload_file_impl});
    R.add("WEBSOCKETCONNECT", NativeFn{"WEBSOCKETCONNECT", 1, websocket_connect_impl});
    R.add("WEBSOCKETSEND", NativeFn{"WEBSOCKETSEND", 2, websocket_send_impl});
    R.add("WEBSOCKETRECEIVE", NativeFn{"WEBSOCKETRECEIVE", 1, websocket_receive_impl});
    R.add("WEBSOCKETCLOSE", NativeFn{"WEBSOCKETCLOSE", 1, websocket_close_impl});
    R.add("TCPCONNECT", NativeFn{"TCPCONNECT", 2, tcp_connect_impl});
    R.add("TCPSEND", NativeFn{"TCPSEND", 2, tcp_send_impl});
    R.add("TCPRECEIVE", NativeFn{"TCPRECEIVE", 1, tcp_receive_impl});
    R.add("TCPCLOSE", NativeFn{"TCPCLOSE", 1, tcp_close_impl});
    R.add("UDPCREATE", NativeFn{"UDPCREATE", 1, udp_create_impl});
    R.add("UDPSEND", NativeFn{"UDPSEND", 4, udp_send_impl});
    R.add("UDPRECEIVE", NativeFn{"UDPRECEIVE", 1, udp_receive_impl});
    R.add("UDPCLOSE", NativeFn{"UDPCLOSE", 1, udp_close_impl});
}

} // namespace bas


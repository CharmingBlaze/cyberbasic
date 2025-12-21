#pragma once

#include "value.hpp"
#include "runtime.hpp"
#include <vector>

namespace bas {

// Advanced networking function implementations
Value http_get_impl(const std::vector<Value>& args);
Value http_post_impl(const std::vector<Value>& args);
Value download_file_impl(const std::vector<Value>& args);
Value upload_file_impl(const std::vector<Value>& args);
Value websocket_connect_impl(const std::vector<Value>& args);
Value websocket_send_impl(const std::vector<Value>& args);
Value websocket_receive_impl(const std::vector<Value>& args);
Value websocket_close_impl(const std::vector<Value>& args);
Value tcp_connect_impl(const std::vector<Value>& args);
Value tcp_send_impl(const std::vector<Value>& args);
Value tcp_receive_impl(const std::vector<Value>& args);
Value tcp_close_impl(const std::vector<Value>& args);
Value udp_create_impl(const std::vector<Value>& args);
Value udp_send_impl(const std::vector<Value>& args);
Value udp_receive_impl(const std::vector<Value>& args);
Value udp_close_impl(const std::vector<Value>& args);

} // namespace bas


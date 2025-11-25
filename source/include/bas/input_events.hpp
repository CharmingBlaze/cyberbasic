#pragma once
#include "runtime.hpp"
#include "ast.hpp"
#include <memory>
#include <vector>
#include <string>

namespace bas {
    void register_input_events(FunctionRegistry& R);
    void set_event_registry(FunctionRegistry* registry);
    void process_input_events();
    void register_key_event_handler(const std::string& eventName,
                                    const std::string& keyName,
                                    std::vector<std::unique_ptr<Stmt>> body);
    void register_mouse_event_handler(const std::string& eventName,
                                     std::vector<std::unique_ptr<Stmt>> body);
}


#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include "bas/ast.hpp"
#include <raylib.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>

namespace bas {

// Event handler storage
struct EventHandlerData {
    std::string eventType;  // "KEY", "MOUSE", etc.
    std::string eventName;  // "PRESSED", "CLICKED", etc.
    int keyCode{0};         // For key events
    std::vector<std::unique_ptr<Stmt>> body; // Handler body
    FunctionRegistry* registry{nullptr};
};

static std::vector<EventHandlerData> g_event_handlers;
static FunctionRegistry* g_event_registry = nullptr;

// Set the function registry for event handlers
void set_event_registry(FunctionRegistry* registry) {
    g_event_registry = registry;
}

// Register an event handler
void register_event_handler(const std::string& eventType, 
                            const std::string& eventName,
                            int keyCode,
                            std::vector<std::unique_ptr<Stmt>> body) {
    EventHandlerData handler;
    handler.eventType = eventType;
    handler.eventName = eventName;
    handler.keyCode = keyCode;
    handler.body = std::move(body);
    handler.registry = g_event_registry;
    g_event_handlers.push_back(std::move(handler));
}

// Process input events (should be called each frame)
void process_input_events() {
    if (!g_event_registry) return;
    
    // Process key events
    for (auto& handler : g_event_handlers) {
        if (handler.eventType == "KEY") {
            bool shouldTrigger = false;
            
            if (handler.eventName == "PRESSED") {
                shouldTrigger = IsKeyPressed(handler.keyCode);
            } else if (handler.eventName == "DOWN") {
                shouldTrigger = IsKeyDown(handler.keyCode);
            } else if (handler.eventName == "RELEASED") {
                shouldTrigger = IsKeyUp(handler.keyCode);
            }
            
            if (shouldTrigger && handler.registry) {
                // Execute handler body
                // Note: This requires access to Env, which would need to be passed
                // For now, this is a placeholder structure
            }
        } else if (handler.eventType == "MOUSE") {
            bool shouldTrigger = false;
            
            if (handler.eventName == "CLICKED") {
                shouldTrigger = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            } else if (handler.eventName == "DOWN") {
                shouldTrigger = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
            } else if (handler.eventName == "RELEASED") {
                shouldTrigger = IsMouseButtonUp(MOUSE_BUTTON_LEFT);
            }
            
            if (shouldTrigger && handler.registry) {
                // Execute handler body
            }
        }
    }
}

// Helper to convert key name to key code
int get_key_code(const std::string& keyName) {
    // Map common key names to Raylib key codes
    if (keyName == "SPACE") return KEY_SPACE;
    if (keyName == "ENTER") return KEY_ENTER;
    if (keyName == "ESCAPE") return KEY_ESCAPE;
    if (keyName == "UP") return KEY_UP;
    if (keyName == "DOWN") return KEY_DOWN;
    if (keyName == "LEFT") return KEY_LEFT;
    if (keyName == "RIGHT") return KEY_RIGHT;
    if (keyName == "W") return KEY_W;
    if (keyName == "A") return KEY_A;
    if (keyName == "S") return KEY_S;
    if (keyName == "D") return KEY_D;
    // Add more as needed
    return 0;
}

// ON KEY PRESSED keyName ... END ON
// This would be called from the interpreter when processing EventHandler AST nodes
void register_key_event_handler(const std::string& eventName,
                                const std::string& keyName,
                                std::vector<std::unique_ptr<Stmt>> body) {
    int keyCode = get_key_code(keyName);
    register_event_handler("KEY", eventName, keyCode, std::move(body));
}

// ON MOUSE eventName ... END ON
void register_mouse_event_handler(const std::string& eventName,
                                  std::vector<std::unique_ptr<Stmt>> body) {
    register_event_handler("MOUSE", eventName, 0, std::move(body));
}

// Input.getKey(keyName) - returns true if key is currently down
static Value input_getKey(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::from_bool(false);
    }
    
    std::string keyName = args[0].as_string();
    int keyCode = get_key_code(keyName);
    return Value::from_bool(IsKeyDown(keyCode));
}

// Input.getKeyPressed(keyName) - returns true if key was just pressed
static Value input_getKeyPressed(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::from_bool(false);
    }
    
    std::string keyName = args[0].as_string();
    int keyCode = get_key_code(keyName);
    return Value::from_bool(IsKeyPressed(keyCode));
}

// Input.getMouseX() - returns mouse X position
static Value input_getMouseX(const std::vector<Value>& args) {
    (void)args;
    return Value::from_int(GetMouseX());
}

// Input.getMouseY() - returns mouse Y position
static Value input_getMouseY(const std::vector<Value>& args) {
    (void)args;
    return Value::from_int(GetMouseY());
}

// Input.getMouseButton(button) - returns true if mouse button is down
static Value input_getMouseButton(const std::vector<Value>& args) {
    if (args.empty()) {
        return Value::from_bool(false);
    }
    
    int button = static_cast<int>(args[0].as_int());
    MouseButton mb = (button == 0) ? MOUSE_BUTTON_LEFT : 
                     (button == 1) ? MOUSE_BUTTON_RIGHT : MOUSE_BUTTON_MIDDLE;
    return Value::from_bool(IsMouseButtonDown(mb));
}

// Register input system functions
void register_input_events(FunctionRegistry& R) {
    R.add("INPUT_GETKEY", NativeFn{"INPUT_GETKEY", 1, input_getKey});
    R.add("INPUT_GETKEYPRESSED", NativeFn{"INPUT_GETKEYPRESSED", 1, input_getKeyPressed});
    R.add("INPUT_GETMOUSEX", NativeFn{"INPUT_GETMOUSEX", 0, input_getMouseX});
    R.add("INPUT_GETMOUSEY", NativeFn{"INPUT_GETMOUSEY", 0, input_getMouseY});
    R.add("INPUT_GETMOUSEBUTTON", NativeFn{"INPUT_GETMOUSEBUTTON", 1, input_getMouseButton});
}

} // namespace bas


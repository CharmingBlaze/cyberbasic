#include "bas/enhanced_events.hpp"
#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <functional>

namespace bas {

// Event handler data
struct EventListener {
    std::string eventName;
    std::string handlerId;
    std::string handlerFunction;  // Function name to call
    bool isActive;
};

static std::unordered_map<std::string, std::vector<EventListener>> g_event_handlers;
static std::vector<std::pair<std::string, std::string>> g_event_queue;  // eventName, payload

// EVENT.subscribe(eventName, handlerFunction) -> handlerId
static Value event_subscribe(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::from_string("");
    }
    
    std::string eventName = args[0].as_string();
    std::string handlerFunction = args[1].as_string();
    
    EventListener handler;
    handler.eventName = eventName;
    handler.handlerFunction = handlerFunction;
    handler.handlerId = eventName + "_" + std::to_string(g_event_handlers[eventName].size());
    handler.isActive = true;
    
    g_event_handlers[eventName].push_back(handler);
    
    return Value::from_string(handler.handlerId);
}

// EVENT.unsubscribe(eventName, handlerId)
static Value event_unsubscribe(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::nil();
    }
    
    std::string eventName = args[0].as_string();
    std::string handlerId = args[1].as_string();
    
    auto& handlers = g_event_handlers[eventName];
    for (auto it = handlers.begin(); it != handlers.end(); ++it) {
        if (it->handlerId == handlerId) {
            handlers.erase(it);
            break;
        }
    }
    
    return Value::nil();
}

// EVENT.fire(eventName, [payload])
static Value event_fire(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::nil();
    }
    
    std::string eventName = args[0].as_string();
    std::string payload = args.size() > 1 ? args[1].as_string() : "";
    
    // Add to queue for processing
    g_event_queue.push_back({eventName, payload});
    
    return Value::nil();
}

// EVENT.fireImmediate(eventName, [payload]) - Fire immediately
static Value event_fireImmediate(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::nil();
    }
    
    std::string eventName = args[0].as_string();
    std::string payload = args.size() > 1 ? args[1].as_string() : "";
    
    // Fire immediately (would call handler functions here)
    auto it = g_event_handlers.find(eventName);
    if (it != g_event_handlers.end()) {
        for (const auto& handler : it->second) {
            if (handler.isActive) {
                // Would call handler.handlerFunction(payload) here
                // This requires integration with the interpreter
            }
        }
    }
    
    return Value::nil();
}

// EVENT.processQueue() - Process queued events
static Value event_processQueue(const std::vector<Value>& args) {
    (void)args;
    
    // Process all queued events
    for (const auto& event : g_event_queue) {
        std::string eventName = event.first;
        std::string payload = event.second;
        
        auto it = g_event_handlers.find(eventName);
        if (it != g_event_handlers.end()) {
            for (const auto& handler : it->second) {
                if (handler.isActive) {
                    // Would call handler.handlerFunction(payload) here
                }
            }
        }
    }
    
    g_event_queue.clear();
    return Value::nil();
}

// EVENT.clearQueue()
static Value event_clearQueue(const std::vector<Value>& args) {
    (void)args;
    g_event_queue.clear();
    return Value::nil();
}

// EVENT.getQueueSize() -> int
static Value event_getQueueSize(const std::vector<Value>& args) {
    (void)args;
    return Value::from_int(static_cast<int>(g_event_queue.size()));
}

// EVENT.hasHandlers(eventName) -> bool
static Value event_hasHandlers(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::from_bool(false);
    }
    
    std::string eventName = args[0].as_string();
    auto it = g_event_handlers.find(eventName);
    return Value::from_bool(it != g_event_handlers.end() && !it->second.empty());
}

void register_enhanced_events(FunctionRegistry& registry) {
    registry.add("EVENT_SUBSCRIBE", NativeFn{"EVENT_SUBSCRIBE", 2, event_subscribe});
    registry.add("EVENT_UNSUBSCRIBE", NativeFn{"EVENT_UNSUBSCRIBE", 2, event_unsubscribe});
    registry.add("EVENT_FIRE", NativeFn{"EVENT_FIRE", 2, event_fire});
    registry.add("EVENT_FIREIMMEDIATE", NativeFn{"EVENT_FIREIMMEDIATE", 2, event_fireImmediate});
    registry.add("EVENT_PROCESSQUEUE", NativeFn{"EVENT_PROCESSQUEUE", 0, event_processQueue});
    registry.add("EVENT_CLEARQUEUE", NativeFn{"EVENT_CLEARQUEUE", 0, event_clearQueue});
    registry.add("EVENT_GETQUEUESIZE", NativeFn{"EVENT_GETQUEUESIZE", 0, event_getQueueSize});
    registry.add("EVENT_HASHANDLERS", NativeFn{"EVENT_HASHANDLERS", 1, event_hasHandlers});
}

}


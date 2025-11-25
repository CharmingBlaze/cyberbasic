#pragma once
#include "runtime.hpp"

namespace bas {
    void register_timer_system(FunctionRegistry& R);
    void update_delta_time();
    double get_delta_time();
}


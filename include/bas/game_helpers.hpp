#pragma once
#include "runtime.hpp"

namespace bas {
    void register_game_helpers(FunctionRegistry& R);
    void register_asset_manager(FunctionRegistry& R);
    void register_save_load(FunctionRegistry& R);
    void register_profiling(FunctionRegistry& R);
    void register_debug_viz(FunctionRegistry& R);
}


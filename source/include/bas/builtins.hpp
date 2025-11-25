#pragma once
#include "runtime.hpp"

namespace bas {
// Register standard library builtins (PRINT, STR, VAL, LEN, SUBSTR, RANDOM, SEED, TIME, ASSERT).
void register_builtins(FunctionRegistry&);
} // namespace bas

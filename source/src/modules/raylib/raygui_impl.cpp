// raygui implementation - must be in exactly one translation unit
// Must include raylib.h BEFORE defining RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raymath.h"

// Define RAYGUI_IMPLEMENTATION before including raygui.h
#define RAYGUI_IMPLEMENTATION

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wsign-conversion"
// TextToFloat is already declared in raylib.h, no need to redefine
#include "raygui.h"
#pragma GCC diagnostic pop


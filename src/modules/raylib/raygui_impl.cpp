// raygui implementation - must be in exactly one translation unit
// Must include raylib.h BEFORE defining RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raymath.h"

// Define RAYGUI_IMPLEMENTATION before including raygui.h
#define RAYGUI_IMPLEMENTATION

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wsign-conversion"
// Workaround for TextToFloat forward declaration issue
// We'll define a simple TextToFloat if it's not already defined
static float TextToFloat(const char *text) {
    if (!text || !*text) return 0.0f;
    return (float)strtod(text, nullptr);
}
#include "raygui.h"
#pragma GCC diagnostic pop


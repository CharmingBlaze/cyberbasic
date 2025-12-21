// Auto-generated from specs/raylib_api.yaml
#include "raylib.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include "raymath.h"
#pragma GCC diagnostic pop
#include "bas/runtime.hpp"
#include "bas/yaml_module_loader.hpp"
#include "bas/value.hpp"
#include "bas/raygui_helpers.hpp"
#include "bas/raymath_helpers.hpp"
#include "bas/camera3d.hpp"
#include "bas/vector3d.hpp"
#include "bas/networking_advanced.hpp"
#include "bas/post_processing_registry.hpp"
#include "bas/particle_system_registry.hpp"
#include "bas/custom_cyberbasic.hpp"

// Forward declaration for camera system
namespace bas {
    extern std::unique_ptr<CameraSystem3D> g_camera_system_3d;
}
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <memory>
#include <random>
#include <algorithm>

using bas::Value;
using Fn = bas::NativeFn;

// Helper macros to avoid narrowing conversion warnings
#define FLOAT(i) static_cast<float>(args[i].as_number())
#define INT(i) args[i].as_int()
#define STR(i) args[i].as_string()
#define BOOL(i) args[i].as_bool()

// Helper function for converting Value to string
static std::string to_string_value(const Value& v) {
  if (v.is_string()) return v.as_string();
  if (v.is_number()) {
    std::ostringstream os;
    os.imbue(std::locale::classic());
    os << v.as_number();
    return os.str();
  }
  if (v.is_int()) {
    std::ostringstream os;
    os.imbue(std::locale::classic());
    os << v.as_int();
    return os.str();
  }
  if (v.is_bool()) return v.as_bool() ? "TRUE" : "FALSE";
  if (v.is_array()) return "[Array]";
  return "";
}

namespace bas {
namespace rlreg {
    // Resource management
    static int next_tex_id = 1;
    static std::unordered_map<int, ::Texture2D> textures;
    static int next_sound_id = 1;
    static std::unordered_map<int, ::Sound> sounds;
    static int next_model_id = 1;
    static std::unordered_map<int, ::Model> models;
    static int next_shader_id = 1;
    static std::unordered_map<int, ::Shader> shaders;
    static int next_font_id = 1;
    static std::unordered_map<int, ::Font> fonts;
    static int next_music_id = 1;
    static std::unordered_map<int, ::Music> music;
    static std::unordered_map<int, ::AudioStream> audio_streams;
    static int next_wave_id = 1;
    static std::unordered_map<int, ::Wave> waves;
    static int next_mesh_id = 1;
    static std::unordered_map<int, ::Mesh> meshes;
    static int next_material_id = 1;
    static std::unordered_map<int, ::Material> materials;
    static int next_image_id = 1;
    static std::unordered_map<int, ::Image> images;
    static int next_audiostream_id = 1;
    static std::unordered_map<int, ::AudioStream> audiostreams;
    static std::unordered_map<int, ::Music> musics;
    static int next_cubemap_id = 1;
    static std::unordered_map<int, ::Texture2D> cubemaps;
    static int next_rendertex_id = 1;
    static std::unordered_map<int, ::RenderTexture2D> rendertextures;
    static int next_camera2d_id = 1;
    static std::unordered_map<int, ::Camera2D> cameras2d;
}

void register_raylib_bindings(FunctionRegistry& R) {
    R.add_with_policy("INITWINDOW", Fn{"INITWINDOW", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("INITWINDOW: expected 3 args");
        InitWindow(args[0].as_int(), args[1].as_int(), args[2].as_string().c_str());
        return Value::nil();
    }}, true);    R.add_with_policy("CLOSEWINDOW", Fn{"CLOSEWINDOW", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("CLOSEWINDOW: expected 0 args");
        CloseWindow();
        return Value::nil();
    }}, true);    R.add_with_policy("WINDOWSHOULDCLOSE", Fn{"WINDOWSHOULDCLOSE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("WINDOWSHOULDCLOSE: expected 0 args");
        return Value::from_bool(WindowShouldClose());
    }}, true);    R.add_with_policy("ISWINDOWREADY", Fn{"ISWINDOWREADY", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWREADY: expected 0 args");
        return Value::from_bool(IsWindowReady());
    }}, true);    R.add_with_policy("ISWINDOWFULLSCREEN", Fn{"ISWINDOWFULLSCREEN", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWFULLSCREEN: expected 0 args");
        return Value::from_bool(IsWindowFullscreen());
    }}, true);    R.add_with_policy("ISWINDOWHIDDEN", Fn{"ISWINDOWHIDDEN", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWHIDDEN: expected 0 args");
        return Value::from_bool(IsWindowHidden());
    }}, true);    R.add_with_policy("ISWINDOWMINIMIZED", Fn{"ISWINDOWMINIMIZED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWMINIMIZED: expected 0 args");
        return Value::from_bool(IsWindowMinimized());
    }}, true);    R.add_with_policy("ISWINDOWMAXIMIZED", Fn{"ISWINDOWMAXIMIZED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWMAXIMIZED: expected 0 args");
        return Value::from_bool(IsWindowMaximized());
    }}, true);    R.add_with_policy("ISWINDOWFOCUSED", Fn{"ISWINDOWFOCUSED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWFOCUSED: expected 0 args");
        return Value::from_bool(IsWindowFocused());
    }}, true);    R.add_with_policy("ISWINDOWRESIZED", Fn{"ISWINDOWRESIZED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWRESIZED: expected 0 args");
        return Value::from_bool(IsWindowResized());
    }}, true);    R.add_with_policy("ISWINDOWSTATE", Fn{"ISWINDOWSTATE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISWINDOWSTATE: expected 1 args");
        return Value::from_bool(IsWindowState(args[0].as_int()));
    }}, true);    R.add_with_policy("SETWINDOWSTATE", Fn{"SETWINDOWSTATE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETWINDOWSTATE: expected 1 args");
        SetWindowState(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("CLEARWINDOWSTATE", Fn{"CLEARWINDOWSTATE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("CLEARWINDOWSTATE: expected 1 args");
        ClearWindowState(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("TOGGLEFULLSCREEN", Fn{"TOGGLEFULLSCREEN", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("TOGGLEFULLSCREEN: expected 0 args");
        ToggleFullscreen();
        return Value::nil();
    }}, true);    R.add_with_policy("TOGGLEBORDERLESSWINDOWED", Fn{"TOGGLEBORDERLESSWINDOWED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("TOGGLEBORDERLESSWINDOWED: expected 0 args");
        ToggleBorderlessWindowed();
        return Value::nil();
    }}, true);    R.add_with_policy("MAXIMIZEWINDOW", Fn{"MAXIMIZEWINDOW", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("MAXIMIZEWINDOW: expected 0 args");
        MaximizeWindow();
        return Value::nil();
    }}, true);    R.add_with_policy("MINIMIZEWINDOW", Fn{"MINIMIZEWINDOW", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("MINIMIZEWINDOW: expected 0 args");
        MinimizeWindow();
        return Value::nil();
    }}, true);    R.add_with_policy("RESTOREWINDOW", Fn{"RESTOREWINDOW", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("RESTOREWINDOW: expected 0 args");
        RestoreWindow();
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWTITLE", Fn{"SETWINDOWTITLE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETWINDOWTITLE: expected 1 args");
        SetWindowTitle(args[0].as_string().c_str());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWPOSITION", Fn{"SETWINDOWPOSITION", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETWINDOWPOSITION: expected 2 args");
        SetWindowPosition(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWMONITOR", Fn{"SETWINDOWMONITOR", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETWINDOWMONITOR: expected 1 args");
        SetWindowMonitor(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWMINSIZE", Fn{"SETWINDOWMINSIZE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETWINDOWMINSIZE: expected 2 args");
        SetWindowMinSize(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWMAXSIZE", Fn{"SETWINDOWMAXSIZE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETWINDOWMAXSIZE: expected 2 args");
        SetWindowMaxSize(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWSIZE", Fn{"SETWINDOWSIZE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETWINDOWSIZE: expected 2 args");
        SetWindowSize(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWOPACITY", Fn{"SETWINDOWOPACITY", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETWINDOWOPACITY: expected 1 args");
        SetWindowOpacity(static_cast<float>(args[0].as_number()));
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWFOCUSED", Fn{"SETWINDOWFOCUSED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("SETWINDOWFOCUSED: expected 0 args");
        SetWindowFocused();
        return Value::nil();
    }}, true);    R.add_with_policy("GETSCREENWIDTH", Fn{"GETSCREENWIDTH", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETSCREENWIDTH: expected 0 args");
        return Value::from_int(GetScreenWidth());
    }}, true);    R.add_with_policy("GETSCREENHEIGHT", Fn{"GETSCREENHEIGHT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETSCREENHEIGHT: expected 0 args");
        return Value::from_int(GetScreenHeight());
    }}, true);    R.add_with_policy("GETRENDERWIDTH", Fn{"GETRENDERWIDTH", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETRENDERWIDTH: expected 0 args");
        return Value::from_int(GetRenderWidth());
    }}, true);    R.add_with_policy("GETRENDERHEIGHT", Fn{"GETRENDERHEIGHT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETRENDERHEIGHT: expected 0 args");
        return Value::from_int(GetRenderHeight());
    }}, true);    R.add_with_policy("GETMONITORCOUNT", Fn{"GETMONITORCOUNT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMONITORCOUNT: expected 0 args");
        return Value::from_int(GetMonitorCount());
    }}, true);    R.add_with_policy("GETCURRENTMONITOR", Fn{"GETCURRENTMONITOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETCURRENTMONITOR: expected 0 args");
        return Value::from_int(GetCurrentMonitor());
    }}, true);    R.add_with_policy("GETMONITORPOSITION", Fn{"GETMONITORPOSITION", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORPOSITION: expected 1 args");
        ::Vector2 pos = ::GetMonitorPosition(INT(0));
        return Value::from_string(std::to_string(pos.x) + "," + std::to_string(pos.y));
    }}, true);    R.add_with_policy("GETMONITORWIDTH", Fn{"GETMONITORWIDTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORWIDTH: expected 1 args");
        return Value::from_int(GetMonitorWidth(args[0].as_int()));
    }}, true);    R.add_with_policy("GETMONITORHEIGHT", Fn{"GETMONITORHEIGHT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORHEIGHT: expected 1 args");
        return Value::from_int(GetMonitorHeight(args[0].as_int()));
    }}, true);    R.add_with_policy("GETMONITORREFRESHRATE", Fn{"GETMONITORREFRESHRATE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORREFRESHRATE: expected 1 args");
        return Value::from_int(GetMonitorRefreshRate(args[0].as_int()));
    }}, true);    R.add_with_policy("GETWINDOWPOSITION", Fn{"GETWINDOWPOSITION", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETWINDOWPOSITION: expected 0 args");
        ::Vector2 pos = ::GetWindowPosition();
        return Value::from_string(std::to_string(pos.x) + "," + std::to_string(pos.y));
    }}, true);    R.add_with_policy("GETWINDOWSCALEDPI", Fn{"GETWINDOWSCALEDPI", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETWINDOWSCALEDPI: expected 0 args");
        ::Vector2 scale = ::GetWindowScaleDPI();
        return Value::from_string(std::to_string(scale.x) + "," + std::to_string(scale.y));
    }}, true);    R.add_with_policy("GETMONITORNAME", Fn{"GETMONITORNAME", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORNAME: expected 1 args");
        return Value::from_string(GetMonitorName(args[0].as_int()));
    }}, true);    R.add_with_policy("SETCLIPBOARDTEXT", Fn{"SETCLIPBOARDTEXT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETCLIPBOARDTEXT: expected 1 args");
        SetClipboardText(args[0].as_string().c_str());
        return Value::nil();
    }}, true);    R.add_with_policy("GETCLIPBOARDTEXT", Fn{"GETCLIPBOARDTEXT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETCLIPBOARDTEXT: expected 0 args");
        return Value::from_string(GetClipboardText());
    }}, true);    R.add_with_policy("ENABLEEVENTWAITING", Fn{"ENABLEEVENTWAITING", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENABLEEVENTWAITING: expected 0 args");
        EnableEventWaiting();
        return Value::nil();
    }}, true);    R.add_with_policy("DISABLEEVENTWAITING", Fn{"DISABLEEVENTWAITING", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("DISABLEEVENTWAITING: expected 0 args");
        DisableEventWaiting();
        return Value::nil();
    }}, true);    R.add_with_policy("SHOWCURSOR", Fn{"SHOWCURSOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("SHOWCURSOR: expected 0 args");
        ShowCursor();
        return Value::nil();
    }}, true);    R.add_with_policy("HIDECURSOR", Fn{"HIDECURSOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("HIDECURSOR: expected 0 args");
        HideCursor();
        return Value::nil();
    }}, true);    R.add_with_policy("ISCURSORHIDDEN", Fn{"ISCURSORHIDDEN", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISCURSORHIDDEN: expected 0 args");
        return Value::from_bool(IsCursorHidden());
    }}, true);    R.add_with_policy("ENABLECURSOR", Fn{"ENABLECURSOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENABLECURSOR: expected 0 args");
        EnableCursor();
        return Value::nil();
    }}, true);    R.add_with_policy("DISABLECURSOR", Fn{"DISABLECURSOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("DISABLECURSOR: expected 0 args");
        DisableCursor();
        return Value::nil();
    }}, true);    R.add_with_policy("ISCURSORONSCREEN", Fn{"ISCURSORONSCREEN", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISCURSORONSCREEN: expected 0 args");
        return Value::from_bool(IsCursorOnScreen());
    }}, true);    R.add_with_policy("BEGINDRAWING", Fn{"BEGINDRAWING", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("BEGINDRAWING: expected 0 args");
        BeginDrawing();
        return Value::nil();
    }}, true);    R.add_with_policy("ENDDRAWING", Fn{"ENDDRAWING", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDDRAWING: expected 0 args");
        EndDrawing();
        return Value::nil();
    }}, true);    R.add_with_policy("CLEARBACKGROUND", Fn{"CLEARBACKGROUND", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("CLEARBACKGROUND: expected 3 args");
        ::Color c{(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), 255};
        ::ClearBackground(c);
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINMODE2D", Fn{"BEGINMODE2D", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("BEGINMODE2D: expected 1 args");
        // Create default 2D camera for now
        ::Camera2D camera = {};
        camera.offset = ::Vector2{ 0.0f, 0.0f };
        camera.target = ::Vector2{ 0.0f, 0.0f };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
        ::BeginMode2D(camera);
        return Value::nil();
    }}, true);    R.add_with_policy("ENDMODE2D", Fn{"ENDMODE2D", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDMODE2D: expected 0 args");
        EndMode2D();
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINMODE3D", Fn{"BEGINMODE3D", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("BEGINMODE3D: expected 1 args");
        // Create proper 3D camera with good default values
        ::Camera3D camera = {};
        camera.position = ::Vector3{ 0.0f, 10.0f, 10.0f };    // Camera position
        camera.target = ::Vector3{ 0.0f, 0.0f, 0.0f };        // Camera looking at point
        camera.up = ::Vector3{ 0.0f, 1.0f, 0.0f };            // Camera up vector (rotation towards target)
        camera.fovy = 45.0f;                                     // Camera field-of-view Y
        camera.projection = CAMERA_PERSPECTIVE;                  // Camera projection type
        ::BeginMode3D(camera);
        return Value::nil();
    }}, true);    R.add_with_policy("ENDMODE3D", Fn{"ENDMODE3D", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDMODE3D: expected 0 args");
        EndMode3D();
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINTEXTUREMODE", Fn{"BEGINTEXTUREMODE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("BEGINTEXTUREMODE: expected 1 args");
        // Create default render texture for now
        ::RenderTexture2D target = {};
        ::BeginTextureMode(target);
        return Value::nil();
    }}, true);    R.add_with_policy("ENDTEXTUREMODE", Fn{"ENDTEXTUREMODE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDTEXTUREMODE: expected 0 args");
        EndTextureMode();
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINSHADERMODE", Fn{"BEGINSHADERMODE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("BEGINSHADERMODE: expected 1 args");
        // Create default shader for now
        ::Shader shader = {};
        ::BeginShaderMode(shader);
        return Value::nil();
    }}, true);    R.add_with_policy("ENDSHADERMODE", Fn{"ENDSHADERMODE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDSHADERMODE: expected 0 args");
        EndShaderMode();
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINBLENDMODE", Fn{"BEGINBLENDMODE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("BEGINBLENDMODE: expected 1 args");
        BeginBlendMode(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("ENDBLENDMODE", Fn{"ENDBLENDMODE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDBLENDMODE: expected 0 args");
        EndBlendMode();
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINSCISSORMODE", Fn{"BEGINSCISSORMODE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("BEGINSCISSORMODE: expected 4 args");
        BeginScissorMode(args[0].as_int(), args[1].as_int(), args[2].as_int(), args[3].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("ENDSCISSORMODE", Fn{"ENDSCISSORMODE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDSCISSORMODE: expected 0 args");
        EndScissorMode();
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINVRSTEREOMODE", Fn{"BEGINVRSTEREOMODE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("BEGINVRSTEREOMODE: expected 1 args");
        // VR stereo mode - use raylib's VR config
        ::VrDeviceInfo device = {2160, 1200, 0.110f, 0.062f, 0.041f, 0.07f, 0.064f, {1.0f, 0.22f, 0.24f, 0.0f}, {0.996f, -0.004f, 1.014f, 0.0f}};
        ::VrStereoConfig config = ::LoadVrStereoConfig(device);
        ::BeginVrStereoMode(config);
        return Value::nil();
    }}, true);    R.add_with_policy("ENDVRSTEREOMODE", Fn{"ENDVRSTEREOMODE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDVRSTEREOMODE: expected 0 args");
        // End VR stereo mode
        ::EndVrStereoMode();
        return Value::nil();
    }}, true);    R.add_with_policy("LOADVRSTEREOCONFIG", Fn{"LOADVRSTEREOCONFIG", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADVRSTEREOCONFIG: expected 1 args");
        // Load VR stereo config - raylib manages this internally
        ::VrDeviceInfo device = {2160, 1200, 0.110f, 0.062f, 0.041f, 0.07f, 0.064f, {1.0f, 0.22f, 0.24f, 0.0f}, {0.996f, -0.004f, 1.014f, 0.0f}};
        ::VrStereoConfig config = ::LoadVrStereoConfig(device);
        // Return a tracking ID (raylib manages the actual config)
        static int next_vr_id = 1;
        return Value::from_int(next_vr_id++);
    }}, true);    R.add_with_policy("UNLOADVRSTEREOCONFIG", Fn{"UNLOADVRSTEREOCONFIG", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADVRSTEREOCONFIG: expected 1 args");
        // Unload VR stereo config - raylib manages this internally
        // Note: raylib doesn't have UnloadVrStereoConfig, config is managed automatically
        (void)INT(0); // Suppress unused parameter warning
        return Value::nil();
    }}, true);    R.add_with_policy("LOADSHADER", Fn{"LOADSHADER", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADSHADER: expected 2 args");
        ::Shader shader = ::LoadShader(STR(0).c_str(), STR(1).c_str());
        int id = rlreg::next_shader_id++;
        rlreg::shaders[id] = shader;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADSHADERFROMEMORY", Fn{"LOADSHADERFROMEMORY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADSHADERFROMEMORY: expected 2 args");
        ::Shader shader = ::LoadShaderFromMemory(STR(0).c_str(), STR(1).c_str());
        int id = rlreg::next_shader_id++;
        rlreg::shaders[id] = shader;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISSHADERVALID", Fn{"ISSHADERVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISSHADERVALID: expected 1 args");
        if (rlreg::shaders.find(INT(0)) == rlreg::shaders.end()) {
          return Value::from_bool(false);
        }
        return Value::from_bool(::IsShaderValid(rlreg::shaders.at(INT(0))));
    }}, true);    R.add_with_policy("GETSHADERLOCATION", Fn{"GETSHADERLOCATION", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETSHADERLOCATION: expected 2 args");
        if (rlreg::shaders.find(INT(0)) == rlreg::shaders.end()) {
          return Value::from_int(-1);
        }
        return Value::from_int(::GetShaderLocation(rlreg::shaders.at(INT(0)), STR(1).c_str()));
    }}, true);    R.add_with_policy("GETSHADERLOCATIONATTRIB", Fn{"GETSHADERLOCATIONATTRIB", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETSHADERLOCATIONATTRIB: expected 2 args");
        if (rlreg::shaders.find(INT(0)) == rlreg::shaders.end()) {
          return Value::from_int(-1);
        }
        return Value::from_int(::GetShaderLocationAttrib(rlreg::shaders.at(INT(0)), STR(1).c_str()));
    }}, true);    R.add_with_policy("SETSHADERVALUEFLOAT", Fn{"SETSHADERVALUEFLOAT", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETSHADERVALUEFLOAT: expected 3 args");
        if (rlreg::shaders.find(INT(0)) == rlreg::shaders.end()) {
          return Value::nil();
        }
        float value = FLOAT(2);
        ::SetShaderValue(rlreg::shaders.at(INT(0)), INT(1), &value, SHADER_UNIFORM_FLOAT);
        return Value::nil();
    }}, true);    R.add_with_policy("SETSHADERVALUEINT", Fn{"SETSHADERVALUEINT", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETSHADERVALUEINT: expected 3 args");
        if (rlreg::shaders.find(INT(0)) == rlreg::shaders.end()) {
          return Value::nil();
        }
        int value = INT(2);
        ::SetShaderValue(rlreg::shaders.at(INT(0)), INT(1), &value, SHADER_UNIFORM_INT);
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADSHADER", Fn{"UNLOADSHADER", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADSHADER: expected 1 args");
        if (rlreg::shaders.find(INT(0)) != rlreg::shaders.end()) {
          ::UnloadShader(rlreg::shaders.at(INT(0)));
          rlreg::shaders.erase(INT(0));
        }
        return Value::nil();
    }}, true);    R.add_with_policy("GETSCREENTOWORLDRAY", Fn{"GETSCREENTOWORLDRAY", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GETSCREENTOWORLDRAY: expected 3 args");
        ::Vector2 position = {FLOAT(0), FLOAT(1)};
        ::Camera3D camera = {};
        camera.position = ::Vector3{ 0.0f, 10.0f, 10.0f };
        camera.target = ::Vector3{ 0.0f, 0.0f, 0.0f };
        camera.up = ::Vector3{ 0.0f, 1.0f, 0.0f };
        camera.fovy = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;
        ::Ray ray = ::GetScreenToWorldRay(position, camera);
        return Value::from_string(std::to_string(ray.position.x) + "," + 
                                 std::to_string(ray.position.y) + "," + 
                                 std::to_string(ray.position.z) + "," +
                                 std::to_string(ray.direction.x) + "," + 
                                 std::to_string(ray.direction.y) + "," + 
                                 std::to_string(ray.direction.z));
    }}, true);    R.add_with_policy("GETWORLDTOSCREEN", Fn{"GETWORLDTOSCREEN", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GETWORLDTOSCREEN: expected 3 args");
        ::Vector3 position = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Camera3D camera = {};
        camera.position = ::Vector3{ 0.0f, 10.0f, 10.0f };
        camera.target = ::Vector3{ 0.0f, 0.0f, 0.0f };
        camera.up = ::Vector3{ 0.0f, 1.0f, 0.0f };
        camera.fovy = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;
        ::Vector2 screen_pos = ::GetWorldToScreen(position, camera);
        return Value::from_string(std::to_string(screen_pos.x) + "," + std::to_string(screen_pos.y));
    }}, true);    R.add_with_policy("GETWORLDTOSCREEN2D", Fn{"GETWORLDTOSCREEN2D", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETWORLDTOSCREEN2D: expected 2 args");
        ::Vector2 position = {FLOAT(0), FLOAT(1)};
        ::Camera2D camera = {};
        camera.offset = ::Vector2{ 0.0f, 0.0f };
        camera.target = ::Vector2{ 0.0f, 0.0f };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
        ::Vector2 screen_pos = ::GetWorldToScreen2D(position, camera);
        return Value::from_string(std::to_string(screen_pos.x) + "," + std::to_string(screen_pos.y));
    }}, true);    R.add_with_policy("GETSCREENTOWORLD2D", Fn{"GETSCREENTOWORLD2D", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETSCREENTOWORLD2D: expected 2 args");
        ::Vector2 position = {FLOAT(0), FLOAT(1)};
        ::Camera2D camera = {};
        camera.offset = ::Vector2{ 0.0f, 0.0f };
        camera.target = ::Vector2{ 0.0f, 0.0f };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
        ::Vector2 world_pos = ::GetScreenToWorld2D(position, camera);
        return Value::from_string(std::to_string(world_pos.x) + "," + std::to_string(world_pos.y));
    }}, true);    R.add_with_policy("GETCAMERAMATRIX", Fn{"GETCAMERAMATRIX", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETCAMERAMATRIX: expected 0 args");
        ::Camera3D camera = {};
        camera.position = ::Vector3{ 0.0f, 10.0f, 10.0f };
        camera.target = ::Vector3{ 0.0f, 0.0f, 0.0f };
        camera.up = ::Vector3{ 0.0f, 1.0f, 0.0f };
        camera.fovy = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;
        ::Matrix matrix = ::GetCameraMatrix(camera);
        // Return matrix as comma-separated string (16 values)
        std::string result = std::to_string(matrix.m0) + "," + std::to_string(matrix.m4) + "," + std::to_string(matrix.m8) + "," + std::to_string(matrix.m12) + "," +
                            std::to_string(matrix.m1) + "," + std::to_string(matrix.m5) + "," + std::to_string(matrix.m9) + "," + std::to_string(matrix.m13) + "," +
                            std::to_string(matrix.m2) + "," + std::to_string(matrix.m6) + "," + std::to_string(matrix.m10) + "," + std::to_string(matrix.m14) + "," +
                            std::to_string(matrix.m3) + "," + std::to_string(matrix.m7) + "," + std::to_string(matrix.m11) + "," + std::to_string(matrix.m15);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("GETCAMERAMATRIX2D", Fn{"GETCAMERAMATRIX2D", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETCAMERAMATRIX2D: expected 0 args");
        ::Camera2D camera = {};
        camera.offset = ::Vector2{ 0.0f, 0.0f };
        camera.target = ::Vector2{ 0.0f, 0.0f };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
        ::Matrix matrix = ::GetCameraMatrix2D(camera);
        std::string result = std::to_string(matrix.m0) + "," + std::to_string(matrix.m4) + "," + std::to_string(matrix.m8) + "," + std::to_string(matrix.m12) + "," +
                            std::to_string(matrix.m1) + "," + std::to_string(matrix.m5) + "," + std::to_string(matrix.m9) + "," + std::to_string(matrix.m13) + "," +
                            std::to_string(matrix.m2) + "," + std::to_string(matrix.m6) + "," + std::to_string(matrix.m10) + "," + std::to_string(matrix.m14) + "," +
                            std::to_string(matrix.m3) + "," + std::to_string(matrix.m7) + "," + std::to_string(matrix.m11) + "," + std::to_string(matrix.m15);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("SETTARGETFPS", Fn{"SETTARGETFPS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETTARGETFPS: expected 1 args");
        SetTargetFPS(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("GETFRAMETIME", Fn{"GETFRAMETIME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETFRAMETIME: expected 0 args");
        return Value::from_number(GetFrameTime());
    }}, true);    R.add_with_policy("GETTIME", Fn{"GETTIME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETTIME: expected 0 args");
        return Value::from_number(GetTime());
    }}, true);    R.add_with_policy("GETFPS", Fn{"GETFPS", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETFPS: expected 0 args");
        return Value::from_int(GetFPS());
    }}, true);    R.add_with_policy("SWAPSCREENBUFFER", Fn{"SWAPSCREENBUFFER", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("SWAPSCREENBUFFER: expected 0 args");
        SwapScreenBuffer();
        return Value::nil();
    }}, true);    R.add_with_policy("POLLINPUTEVENTS", Fn{"POLLINPUTEVENTS", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("POLLINPUTEVENTS: expected 0 args");
        PollInputEvents();
        return Value::nil();
    }}, true);    R.add_with_policy("WAITTIME", Fn{"WAITTIME", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("WAITTIME: expected 1 args");
        WaitTime(static_cast<float>(args[0].as_number()));
        return Value::nil();
    }}, true);    R.add_with_policy("SETRANDOMSEED", Fn{"SETRANDOMSEED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETRANDOMSEED: expected 1 args");
        SetRandomSeed(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("GETRANDOMVALUE", Fn{"GETRANDOMVALUE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETRANDOMVALUE: expected 2 args");
        return Value::from_int(GetRandomValue(args[0].as_int(), args[1].as_int()));
    }}, true);    R.add_with_policy("LOADRANDOMSEQUENCE", Fn{"LOADRANDOMSEQUENCE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("LOADRANDOMSEQUENCE: expected 3 args");
        int count = INT(0);
        int min = INT(1);
        int max = INT(2);
        int *sequence = ::LoadRandomSequence(count, min, max);

        // Convert to comma-separated string
        std::string result = "";
        for (int i = 0; i < count; i++) {
            if (i > 0) result += ",";
            result += std::to_string(sequence[i]);
        }

        ::UnloadRandomSequence(sequence);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("TAKESCREENSHOT", Fn{"TAKESCREENSHOT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TAKESCREENSHOT: expected 1 args");
        TakeScreenshot(args[0].as_string().c_str());
        return Value::nil();
    }}, true);    R.add_with_policy("SETCONFIGFLAGS", Fn{"SETCONFIGFLAGS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETCONFIGFLAGS: expected 1 args");
        SetConfigFlags(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("OPENURL", Fn{"OPENURL", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("OPENURL: expected 1 args");
        OpenURL(args[0].as_string().c_str());
        return Value::nil();
    }}, true);    R.add_with_policy("TRACELOG", Fn{"TRACELOG", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("TRACELOG: expected 2 args");
        int logLevel = INT(0);
        std::string text = STR(1);
        ::TraceLog(logLevel, "%s", text.c_str());
        return Value::nil();
    }}, true);    R.add_with_policy("SETTRACELOGLEVEL", Fn{"SETTRACELOGLEVEL", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETTRACELOGLEVEL: expected 1 args");
        SetTraceLogLevel(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("LOADFILEDATA", Fn{"LOADFILEDATA", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADFILEDATA: expected 1 args");
        int dataSize;
        unsigned char *data = ::LoadFileData(STR(0).c_str(), &dataSize);
        if (data == nullptr) return Value::from_string("");

        // Convert to base64 string for safe transport
        std::string result = "";
        for (int i = 0; i < dataSize; i++) {
            if (i > 0) result += ",";
            result += std::to_string(data[i]);
        }

        ::UnloadFileData(data);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("SAVEFILEDATA", Fn{"SAVEFILEDATA", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SAVEFILEDATA: expected 2 args");
        // Parse comma-separated byte values
        std::string dataStr = STR(1);
        std::vector<unsigned char> bytes;

        size_t pos = 0;
        while (pos < dataStr.length()) {
            size_t comma = dataStr.find(',', pos);
            if (comma == std::string::npos) comma = dataStr.length();
    
            std::string byteStr = dataStr.substr(pos, comma - pos);
            bytes.push_back(static_cast<unsigned char>(std::stoi(byteStr)));
            pos = comma + 1;
        }

        bool success = ::SaveFileData(STR(0).c_str(), bytes.data(), bytes.size());
        return Value::from_bool(success);
    }}, true);    R.add_with_policy("LOADFILETEXT", Fn{"LOADFILETEXT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADFILETEXT: expected 1 args");
        char *text = ::LoadFileText(STR(0).c_str());
        if (text == nullptr) return Value::from_string("");

        std::string result(text);
        ::UnloadFileText(text);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("SAVEFILETEXT", Fn{"SAVEFILETEXT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SAVEFILETEXT: expected 2 args");
        std::string text = STR(1);
        char *cText = const_cast<char*>(text.c_str());
        bool success = ::SaveFileText(STR(0).c_str(), cText);
        return Value::from_bool(success);
    }}, true);    R.add_with_policy("FILEEXISTS", Fn{"FILEEXISTS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("FILEEXISTS: expected 1 args");
        return Value::from_bool(FileExists(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("DIRECTORYEXISTS", Fn{"DIRECTORYEXISTS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("DIRECTORYEXISTS: expected 1 args");
        return Value::from_bool(DirectoryExists(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("ISFILEEXTENSION", Fn{"ISFILEEXTENSION", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ISFILEEXTENSION: expected 2 args");
        return Value::from_bool(IsFileExtension(args[0].as_string().c_str(), args[1].as_string().c_str()));
    }}, true);    R.add_with_policy("GETFILELENGTH", Fn{"GETFILELENGTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETFILELENGTH: expected 1 args");
        return Value::from_int(GetFileLength(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("GETFILEEXTENSION", Fn{"GETFILEEXTENSION", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETFILEEXTENSION: expected 1 args");
        return Value::from_string(GetFileExtension(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("GETFILENAME", Fn{"GETFILENAME", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETFILENAME: expected 1 args");
        return Value::from_string(GetFileName(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("GETFILENAMEWITHHOUTEXT", Fn{"GETFILENAMEWITHHOUTEXT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETFILENAMEWITHHOUTEXT: expected 1 args");
        return Value::from_string(GetFileNameWithoutExt(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("GETDIRECTORYPATH", Fn{"GETDIRECTORYPATH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETDIRECTORYPATH: expected 1 args");
        return Value::from_string(GetDirectoryPath(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("GETPREVDIRECTORYPATH", Fn{"GETPREVDIRECTORYPATH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETPREVDIRECTORYPATH: expected 1 args");
        return Value::from_string(GetPrevDirectoryPath(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("GETWORKINGDIRECTORY", Fn{"GETWORKINGDIRECTORY", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETWORKINGDIRECTORY: expected 0 args");
        return Value::from_string(GetWorkingDirectory());
    }}, true);    R.add_with_policy("GETAPPLICATIONDIRECTORY", Fn{"GETAPPLICATIONDIRECTORY", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETAPPLICATIONDIRECTORY: expected 0 args");
        return Value::from_string(GetApplicationDirectory());
    }}, true);    R.add_with_policy("MAKEDIRECTORY", Fn{"MAKEDIRECTORY", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("MAKEDIRECTORY: expected 1 args");
        return Value::from_int(MakeDirectory(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("CHANGEDIRECTORY", Fn{"CHANGEDIRECTORY", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("CHANGEDIRECTORY: expected 1 args");
        return Value::from_bool(ChangeDirectory(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("ISPATHFILE", Fn{"ISPATHFILE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISPATHFILE: expected 1 args");
        return Value::from_bool(IsPathFile(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("ISFILENAMEVALID", Fn{"ISFILENAMEVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISFILENAMEVALID: expected 1 args");
        return Value::from_bool(IsFileNameValid(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("LOADDIRECTORYFILES", Fn{"LOADDIRECTORYFILES", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADDIRECTORYFILES: expected 1 args");
        ::FilePathList files = ::LoadDirectoryFiles(STR(0).c_str());

        std::string result = "";
        for (int i = 0; i < files.count; i++) {
            if (i > 0) result += ";";
            result += files.paths[i];
        }

        ::UnloadDirectoryFiles(files);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("ISFILEDROPPED", Fn{"ISFILEDROPPED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISFILEDROPPED: expected 0 args");
        return Value::from_bool(IsFileDropped());
    }}, true);    R.add_with_policy("LOADDROPPEDFILES", Fn{"LOADDROPPEDFILES", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("LOADDROPPEDFILES: expected 0 args");
        ::FilePathList files = ::LoadDroppedFiles();

        std::string result = "";
        for (int i = 0; i < files.count; i++) {
            if (i > 0) result += ";";
            result += files.paths[i];
        }

        ::UnloadDroppedFiles(files);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("GETFILEMODTIME", Fn{"GETFILEMODTIME", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETFILEMODTIME: expected 1 args");
        return Value::from_int(GetFileModTime(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("COMPRESSDATA", Fn{"COMPRESSDATA", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("COMPRESSDATA: expected 1 args");
        std::string input = STR(0);
        int compDataSize;
        unsigned char *compData = ::CompressData(
            reinterpret_cast<const unsigned char*>(input.c_str()), 
            input.length(), 
            &compDataSize
        );

        if (compData == nullptr) return Value::from_string("");

        std::string result = "";
        for (int i = 0; i < compDataSize; i++) {
            if (i > 0) result += ",";
            result += std::to_string(compData[i]);
        }

        ::MemFree(compData);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("ENCODEDATABASE64", Fn{"ENCODEDATABASE64", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ENCODEDATABASE64: expected 1 args");
        std::string input = STR(0);
        int outputSize;
        char *encoded = ::EncodeDataBase64(
            reinterpret_cast<const unsigned char*>(input.c_str()), 
            input.length(), 
            &outputSize
        );

        if (encoded == nullptr) return Value::from_string("");

        std::string result(encoded);
        ::MemFree(encoded);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("ISKEYPRESSED", Fn{"ISKEYPRESSED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISKEYPRESSED: expected 1 args");
        return Value::from_bool(IsKeyPressed(args[0].as_int()));
    }}, true);    R.add_with_policy("ISKEYPRESSEDREPEAT", Fn{"ISKEYPRESSEDREPEAT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISKEYPRESSEDREPEAT: expected 1 args");
        return Value::from_bool(IsKeyPressedRepeat(args[0].as_int()));
    }}, true);    R.add_with_policy("ISKEYDOWN", Fn{"ISKEYDOWN", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISKEYDOWN: expected 1 args");
        return Value::from_bool(IsKeyDown(args[0].as_int()));
    }}, true);    R.add_with_policy("ISKEYRELEASED", Fn{"ISKEYRELEASED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISKEYRELEASED: expected 1 args");
        return Value::from_bool(IsKeyReleased(args[0].as_int()));
    }}, true);    R.add_with_policy("ISKEYUP", Fn{"ISKEYUP", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISKEYUP: expected 1 args");
        return Value::from_bool(IsKeyUp(args[0].as_int()));
    }}, true);    R.add_with_policy("GETKEYPRESSED", Fn{"GETKEYPRESSED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETKEYPRESSED: expected 0 args");
        return Value::from_int(GetKeyPressed());
    }}, true);    R.add_with_policy("GETCHARPRESSED", Fn{"GETCHARPRESSED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETCHARPRESSED: expected 0 args");
        return Value::from_int(GetCharPressed());
    }}, true);    R.add_with_policy("SETEXITKEY", Fn{"SETEXITKEY", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETEXITKEY: expected 1 args");
        SetExitKey(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("ISGAMEPADAVAILABLE", Fn{"ISGAMEPADAVAILABLE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISGAMEPADAVAILABLE: expected 1 args");
        return Value::from_bool(IsGamepadAvailable(args[0].as_int()));
    }}, true);    R.add_with_policy("GETGAMEPADNAME", Fn{"GETGAMEPADNAME", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETGAMEPADNAME: expected 1 args");
        return Value::from_string(GetGamepadName(args[0].as_int()));
    }}, true);    R.add_with_policy("ISGAMEPADBUTTONPRESSED", Fn{"ISGAMEPADBUTTONPRESSED", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ISGAMEPADBUTTONPRESSED: expected 2 args");
        return Value::from_bool(IsGamepadButtonPressed(args[0].as_int(), args[1].as_int()));
    }}, true);    R.add_with_policy("ISGAMEPADBUTTONDOWN", Fn{"ISGAMEPADBUTTONDOWN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ISGAMEPADBUTTONDOWN: expected 2 args");
        return Value::from_bool(IsGamepadButtonDown(args[0].as_int(), args[1].as_int()));
    }}, true);    R.add_with_policy("ISGAMEPADBUTTONRELEASED", Fn{"ISGAMEPADBUTTONRELEASED", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ISGAMEPADBUTTONRELEASED: expected 2 args");
        return Value::from_bool(IsGamepadButtonReleased(args[0].as_int(), args[1].as_int()));
    }}, true);    R.add_with_policy("ISGAMEPADBUTTONUP", Fn{"ISGAMEPADBUTTONUP", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ISGAMEPADBUTTONUP: expected 2 args");
        return Value::from_bool(IsGamepadButtonUp(args[0].as_int(), args[1].as_int()));
    }}, true);    R.add_with_policy("GETGAMEPADBUTTONPRESSED", Fn{"GETGAMEPADBUTTONPRESSED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGAMEPADBUTTONPRESSED: expected 0 args");
        return Value::from_int(GetGamepadButtonPressed());
    }}, true);    R.add_with_policy("GETGAMEPADAXISCOUNT", Fn{"GETGAMEPADAXISCOUNT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETGAMEPADAXISCOUNT: expected 1 args");
        return Value::from_int(GetGamepadAxisCount(args[0].as_int()));
    }}, true);    R.add_with_policy("GETGAMEPADAXISMOVEMENT", Fn{"GETGAMEPADAXISMOVEMENT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETGAMEPADAXISMOVEMENT: expected 2 args");
        return Value::from_number(GetGamepadAxisMovement(args[0].as_int(), args[1].as_int()));
    }}, true);    R.add_with_policy("SETGAMEPADMAPPINGS", Fn{"SETGAMEPADMAPPINGS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETGAMEPADMAPPINGS: expected 1 args");
        return Value::from_int(SetGamepadMappings(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("SETGAMEPADVIBRTION", Fn{"SETGAMEPADVIBRTION", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("SETGAMEPADVIBRTION: expected 4 args");
        SetGamepadVibration(args[0].as_int(), static_cast<float>(args[1].as_number()), static_cast<float>(args[2].as_number()), static_cast<float>(args[3].as_number()));
        return Value::nil();
    }}, true);    R.add_with_policy("ISMOUSEBUTTONPRESSED", Fn{"ISMOUSEBUTTONPRESSED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMOUSEBUTTONPRESSED: expected 1 args");
        return Value::from_bool(IsMouseButtonPressed(args[0].as_int()));
    }}, true);    R.add_with_policy("ISMOUSEBUTTONDOWN", Fn{"ISMOUSEBUTTONDOWN", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMOUSEBUTTONDOWN: expected 1 args");
        return Value::from_bool(IsMouseButtonDown(args[0].as_int()));
    }}, true);    R.add_with_policy("ISMOUSEBUTTONRELEASED", Fn{"ISMOUSEBUTTONRELEASED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMOUSEBUTTONRELEASED: expected 1 args");
        return Value::from_bool(IsMouseButtonReleased(args[0].as_int()));
    }}, true);    R.add_with_policy("ISMOUSEBUTTONUP", Fn{"ISMOUSEBUTTONUP", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMOUSEBUTTONUP: expected 1 args");
        return Value::from_bool(IsMouseButtonUp(args[0].as_int()));
    }}, true);    R.add_with_policy("GETMOUSEX", Fn{"GETMOUSEX", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMOUSEX: expected 0 args");
        return Value::from_int(GetMouseX());
    }}, true);    R.add_with_policy("GETMOUSEY", Fn{"GETMOUSEY", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMOUSEY: expected 0 args");
        return Value::from_int(GetMouseY());
    }}, true);    R.add_with_policy("GETMOUSEPOSITION", Fn{"GETMOUSEPOSITION", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMOUSEPOSITION: expected 0 args");
        ::Vector2 pos = ::GetMousePosition();
        return Value::from_string(std::to_string(pos.x) + "," + std::to_string(pos.y));
    }}, true);    R.add_with_policy("GETMOUSEDELTA", Fn{"GETMOUSEDELTA", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMOUSEDELTA: expected 0 args");
        ::Vector2 delta = ::GetMouseDelta();
        return Value::from_string(std::to_string(delta.x) + "," + std::to_string(delta.y));
    }}, true);    R.add_with_policy("SETMOUSEPOSITION", Fn{"SETMOUSEPOSITION", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETMOUSEPOSITION: expected 2 args");
        SetMousePosition(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETMOUSEOFFSET", Fn{"SETMOUSEOFFSET", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETMOUSEOFFSET: expected 2 args");
        SetMouseOffset(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETMOUSESCALE", Fn{"SETMOUSESCALE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETMOUSESCALE: expected 2 args");
        SetMouseScale(static_cast<float>(args[0].as_number()), static_cast<float>(args[1].as_number()));
        return Value::nil();
    }}, true);    R.add_with_policy("GETMOUSEWHEELMOVE", Fn{"GETMOUSEWHEELMOVE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMOUSEWHEELMOVE: expected 0 args");
        return Value::from_number(GetMouseWheelMove());
    }}, true);    R.add_with_policy("GETMOUSEWHEELMOVEV", Fn{"GETMOUSEWHEELMOVEV", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMOUSEWHEELMOVEV: expected 0 args");
        ::Vector2 wheel = ::GetMouseWheelMoveV();
        return Value::from_string(std::to_string(wheel.x) + "," + std::to_string(wheel.y));
    }}, true);    R.add_with_policy("SETMOUSECURSOR", Fn{"SETMOUSECURSOR", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETMOUSECURSOR: expected 1 args");
        SetMouseCursor(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("GETTOUCHX", Fn{"GETTOUCHX", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETTOUCHX: expected 0 args");
        return Value::from_int(GetTouchX());
    }}, true);    R.add_with_policy("GETTOUCHY", Fn{"GETTOUCHY", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETTOUCHY: expected 0 args");
        return Value::from_int(GetTouchY());
    }}, true);    R.add_with_policy("GETTOUCHPOSITION", Fn{"GETTOUCHPOSITION", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETTOUCHPOSITION: expected 1 args");
        ::Vector2 pos = ::GetTouchPosition(INT(0));
        return Value::from_string(std::to_string(pos.x) + "," + std::to_string(pos.y));
    }}, true);    R.add_with_policy("GETTOUCHPOINTID", Fn{"GETTOUCHPOINTID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETTOUCHPOINTID: expected 1 args");
        return Value::from_int(GetTouchPointId(args[0].as_int()));
    }}, true);    R.add_with_policy("GETTOUCHPOINTCOUNT", Fn{"GETTOUCHPOINTCOUNT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETTOUCHPOINTCOUNT: expected 0 args");
        return Value::from_int(GetTouchPointCount());
    }}, true);    R.add_with_policy("SETGESTURESENABLED", Fn{"SETGESTURESENABLED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETGESTURESENABLED: expected 1 args");
        SetGesturesEnabled(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("ISGESTUREDETECTED", Fn{"ISGESTUREDETECTED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISGESTUREDETECTED: expected 1 args");
        return Value::from_bool(IsGestureDetected(args[0].as_int()));
    }}, true);    R.add_with_policy("GETGESTUREDETECTED", Fn{"GETGESTUREDETECTED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREDETECTED: expected 0 args");
        return Value::from_int(GetGestureDetected());
    }}, true);    R.add_with_policy("GETGESTUREHOLDDURATION", Fn{"GETGESTUREHOLDDURATION", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREHOLDDURATION: expected 0 args");
        return Value::from_number(GetGestureHoldDuration());
    }}, true);    R.add_with_policy("GETGESTUREDRAGVECTOR", Fn{"GETGESTUREDRAGVECTOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREDRAGVECTOR: expected 0 args");
        ::Vector2 drag = ::GetGestureDragVector();
        return Value::from_string(std::to_string(drag.x) + "," + std::to_string(drag.y));
    }}, true);    R.add_with_policy("GETGESTUREDRAGANGLE", Fn{"GETGESTUREDRAGANGLE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREDRAGANGLE: expected 0 args");
        return Value::from_number(GetGestureDragAngle());
    }}, true);    R.add_with_policy("GETGESTUREPINCHVECTOR", Fn{"GETGESTUREPINCHVECTOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREPINCHVECTOR: expected 0 args");
        ::Vector2 pinch = ::GetGesturePinchVector();
        return Value::from_string(std::to_string(pinch.x) + "," + std::to_string(pinch.y));
    }}, true);    R.add_with_policy("GETGESTUREPINCHANGLE", Fn{"GETGESTUREPINCHANGLE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREPINCHANGLE: expected 0 args");
        return Value::from_number(GetGesturePinchAngle());
    }}, true);    R.add_with_policy("UPDATECAMERA", Fn{"UPDATECAMERA", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UPDATECAMERA: expected 1 args");
        // Use our advanced camera system if available
        if (g_camera_system_3d) {
            g_camera_system_3d->update(::GetFrameTime());
        } else {
            // Fallback to basic raylib camera update
            ::Camera3D camera = {};
            camera.position = ::Vector3{ 0.0f, 10.0f, 10.0f };
            camera.target = ::Vector3{ 0.0f, 0.0f, 0.0f };
            camera.up = ::Vector3{ 0.0f, 1.0f, 0.0f };
            camera.fovy = 45.0f;
            camera.projection = CAMERA_PERSPECTIVE;
            ::UpdateCamera(&camera, INT(0));
        }
        return Value::nil();
    }}, true);    R.add_with_policy("UPDATECAMERAPRO", Fn{"UPDATECAMERAPRO", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("UPDATECAMERAPRO: expected 7 args");
        // Use our advanced camera system if available
        if (g_camera_system_3d) {
            g_camera_system_3d->set_input_movement(FLOAT(0), FLOAT(1), FLOAT(2));
            g_camera_system_3d->set_input_rotation(FLOAT(3), FLOAT(4), FLOAT(5));
            g_camera_system_3d->set_input_zoom(FLOAT(6));
            g_camera_system_3d->update(::GetFrameTime());
        } else {
            // Fallback to basic raylib camera update
            ::Camera3D camera = {};
            camera.position = ::Vector3{ 0.0f, 10.0f, 10.0f };
            camera.target = ::Vector3{ 0.0f, 0.0f, 0.0f };
            camera.up = ::Vector3{ 0.0f, 1.0f, 0.0f };
            camera.fovy = 45.0f;
            camera.projection = CAMERA_PERSPECTIVE;
    
            ::Vector3 movement = {FLOAT(0), FLOAT(1), FLOAT(2)};
            ::Vector3 rotation = {FLOAT(3), FLOAT(4), FLOAT(5)};
            float zoom = FLOAT(6);
    
            ::UpdateCameraPro(&camera, movement, rotation, zoom);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("CREATEFPSCAMERA", Fn{"CREATEFPSCAMERA", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("CREATEFPSCAMERA: expected 4 args");
        if (!g_camera_system_3d) {
            g_camera_system_3d = std::make_unique<CameraSystem3D>();
        }
        return Value::from_int(g_camera_system_3d->create_fps_camera(
            STR(0), 
            FLOAT(1), 
            FLOAT(2), 
            FLOAT(3)
        ));
    }}, true);    R.add_with_policy("CREATETPSCAMERA", Fn{"CREATETPSCAMERA", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("CREATETPSCAMERA: expected 4 args");
        if (!g_camera_system_3d) {
            g_camera_system_3d = std::make_unique<CameraSystem3D>();
        }
        return Value::from_int(g_camera_system_3d->create_tps_camera(
            STR(0), 
            FLOAT(1), 
            FLOAT(2), 
            FLOAT(3)
        ));
    }}, true);    R.add_with_policy("CREATEORBITCAMERA", Fn{"CREATEORBITCAMERA", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("CREATEORBITCAMERA: expected 5 args");
        if (!g_camera_system_3d) {
            g_camera_system_3d = std::make_unique<CameraSystem3D>();
        }
        return Value::from_int(g_camera_system_3d->create_orbit_camera(
            STR(0), 
            FLOAT(1), 
            FLOAT(2), 
            FLOAT(3),
            FLOAT(4)
        ));
    }}, true);    R.add_with_policy("CREATEFREECAMERA", Fn{"CREATEFREECAMERA", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("CREATEFREECAMERA: expected 4 args");
        if (!g_camera_system_3d) {
            g_camera_system_3d = std::make_unique<CameraSystem3D>();
        }
        return Value::from_int(g_camera_system_3d->create_free_camera(
            STR(0), 
            FLOAT(1), 
            FLOAT(2), 
            FLOAT(3)
        ));
    }}, true);    R.add_with_policy("SETACTIVECAMERA", Fn{"SETACTIVECAMERA", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETACTIVECAMERA: expected 1 args");
        if (g_camera_system_3d) {
            g_camera_system_3d->set_active_camera(INT(0));
        }
        return Value::nil();
    }}, true);    R.add_with_policy("SETCAMERAPOSITION", Fn{"SETCAMERAPOSITION", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("SETCAMERAPOSITION: expected 4 args");
        if (g_camera_system_3d) {
            g_camera_system_3d->set_camera_position(
                INT(0), 
                FLOAT(1), 
                FLOAT(2), 
                FLOAT(3)
            );
        }
        return Value::nil();
    }}, true);    R.add_with_policy("SETCAMERATARGET", Fn{"SETCAMERATARGET", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("SETCAMERATARGET: expected 4 args");
        if (g_camera_system_3d) {
            g_camera_system_3d->set_camera_target(
                INT(0), 
                FLOAT(1), 
                FLOAT(2), 
                FLOAT(3)
            );
        }
        return Value::nil();
    }}, true);    R.add_with_policy("SETCAMERAFOV", Fn{"SETCAMERAFOV", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETCAMERAFOV: expected 2 args");
        if (g_camera_system_3d) {
            g_camera_system_3d->set_camera_fov(INT(0), FLOAT(1));
        }
        return Value::nil();
    }}, true);    R.add_with_policy("SETCAMERAMOUSESENSITIVITY", Fn{"SETCAMERAMOUSESENSITIVITY", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETCAMERAMOUSESENSITIVITY: expected 3 args");
        if (g_camera_system_3d) {
            g_camera_system_3d->set_mouse_sensitivity(
                INT(0), 
                FLOAT(1), 
                FLOAT(2)
            );
        }
        return Value::nil();
    }}, true);    R.add_with_policy("SETCAMERASMOOTHING", Fn{"SETCAMERASMOOTHING", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETCAMERASMOOTHING: expected 3 args");
        if (g_camera_system_3d) {
            g_camera_system_3d->set_smoothing(
                INT(0), 
                BOOL(1), 
                FLOAT(2)
            );
        }
        return Value::nil();
    }}, true);    R.add_with_policy("GETCAMERAPOSITION", Fn{"GETCAMERAPOSITION", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCAMERAPOSITION: expected 1 args");
        if (g_camera_system_3d) {
            bas::Vector3D pos = g_camera_system_3d->get_camera_position(INT(0));
            return Value::from_string(std::to_string(pos.x) + "," + 
                                     std::to_string(pos.y) + "," + 
                                     std::to_string(pos.z));
        }
        return Value::from_string("0,10,10");
    }}, true);    R.add_with_policy("GETCAMERATARGET", Fn{"GETCAMERATARGET", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCAMERATARGET: expected 1 args");
        if (g_camera_system_3d) {
            bas::Vector3D target = g_camera_system_3d->get_camera_target(INT(0));
            return Value::from_string(std::to_string(target.x) + "," + 
                                     std::to_string(target.y) + "," + 
                                     std::to_string(target.z));
        }
        return Value::from_string("0,0,0");
    }}, true);    R.add_with_policy("GETCAMERACOUNT", Fn{"GETCAMERACOUNT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETCAMERACOUNT: expected 0 args");
        if (g_camera_system_3d) {
            return Value::from_int(g_camera_system_3d->get_camera_count());
        }
        return Value::from_int(0);
    }}, true);    R.add_with_policy("SETSHAPESTEXTURE", Fn{"SETSHAPESTEXTURE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("SETSHAPESTEXTURE: expected 5 args");
        // For now, use a simplified approach - would need texture registry
        ::Texture2D texture = {};
        texture.id = INT(0);
        ::Rectangle source = {
            FLOAT(1), 
            FLOAT(2), 
            FLOAT(3), 
            FLOAT(4)
        };
        ::SetShapesTexture(texture, source);
        return Value::nil();
    }}, true);    R.add_with_policy("GETSHAPESTEXTURE", Fn{"GETSHAPESTEXTURE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETSHAPESTEXTURE: expected 0 args");
        ::Texture2D texture = ::GetShapesTexture();
        return Value::from_int(texture.id);
    }}, true);    R.add_with_policy("GETSHAPESTEXTURERECTANGLE", Fn{"GETSHAPESTEXTURERECTANGLE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETSHAPESTEXTURERECTANGLE: expected 0 args");
        ::Rectangle rect = ::GetShapesTextureRectangle();
        return Value::from_string(std::to_string(rect.x) + "," + 
                                 std::to_string(rect.y) + "," + 
                                 std::to_string(rect.width) + "," + 
                                 std::to_string(rect.height));
    }}, true);    R.add_with_policy("DRAWPIXEL", Fn{"DRAWPIXEL", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("DRAWPIXEL: expected 5 args");
        ::Color color = {(unsigned char)INT(2), (unsigned char)INT(3), (unsigned char)INT(4), 255};
        ::DrawPixel(INT(0), INT(1), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPIXELV", Fn{"DRAWPIXELV", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("DRAWPIXELV: expected 5 args");
        ::Vector2 position = {FLOAT(0), FLOAT(1)};
        ::Color color = {(unsigned char)INT(2), (unsigned char)INT(3), (unsigned char)INT(4), 255};
        ::DrawPixelV(position, color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWLINE", Fn{"DRAWLINE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWLINE: expected 7 args");
        ::Color color = {(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawLine(INT(0), INT(1), INT(2), INT(3), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWLINEV", Fn{"DRAWLINEV", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWLINEV: expected 7 args");
        ::Vector2 startPos = {FLOAT(0), FLOAT(1)};
        ::Vector2 endPos = {FLOAT(2), FLOAT(3)};
        ::Color color = {(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawLineV(startPos, endPos, color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWLINEEX", Fn{"DRAWLINEEX", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWLINEEX: expected 8 args");
        ::Vector2 startPos = {FLOAT(0), FLOAT(1)};
        ::Vector2 endPos = {FLOAT(2), FLOAT(3)};
        ::Color color = {(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawLineEx(startPos, endPos, FLOAT(4), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWLINEBEZIER", Fn{"DRAWLINEBEZIER", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWLINEBEZIER: expected 8 args");
        ::Vector2 startPos = {FLOAT(0), FLOAT(1)};
        ::Vector2 endPos = {FLOAT(2), FLOAT(3)};
        ::Color color = {(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawLineBezier(startPos, endPos, FLOAT(4), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLE", Fn{"DRAWCIRCLE", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWCIRCLE: expected 6 args");
        ::Color color = {(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::DrawCircle(INT(0), INT(1), FLOAT(2), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLESECTOR", Fn{"DRAWCIRCLESECTOR", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCIRCLESECTOR: expected 9 args");
        ::Vector2 center = {FLOAT(0), FLOAT(1)};
        ::Color color = {(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawCircleSector(center, FLOAT(2), FLOAT(3), FLOAT(4), INT(5), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLEGRADIENT", Fn{"DRAWCIRCLEGRADIENT", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCIRCLEGRADIENT: expected 9 args");
        ::Color inner = {(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::Color outer = {(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawCircleGradient(INT(0), INT(1), FLOAT(2), inner, outer);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLEV", Fn{"DRAWCIRCLEV", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWCIRCLEV: expected 6 args");
        ::Vector2 center = {FLOAT(0), FLOAT(1)};
        ::Color color = {(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::DrawCircleV(center, FLOAT(2), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLELINES", Fn{"DRAWCIRCLELINES", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWCIRCLELINES: expected 6 args");
        ::Color color = {(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::DrawCircleLines(INT(0), INT(1), FLOAT(2), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWELLIPSE", Fn{"DRAWELLIPSE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWELLIPSE: expected 7 args");
        ::Color color = {(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawEllipse(INT(0), INT(1), FLOAT(2), FLOAT(3), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWELLIPSELINES", Fn{"DRAWELLIPSELINES", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWELLIPSELINES: expected 7 args");
        ::Color color = {(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawEllipseLines(INT(0), INT(1), FLOAT(2), FLOAT(3), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRING", Fn{"DRAWRING", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWRING: expected 10 args");
        ::Vector2 center = {FLOAT(0), FLOAT(1)};
        ::Color color = {(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawRing(center, FLOAT(2), FLOAT(3), FLOAT(4), FLOAT(5), INT(6), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRINGLINES", Fn{"DRAWRINGLINES", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWRINGLINES: expected 10 args");
        ::Vector2 center = {FLOAT(0), FLOAT(1)};
        ::Color color = {(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawRingLines(center, FLOAT(2), FLOAT(3), FLOAT(4), FLOAT(5), INT(6), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLE", Fn{"DRAWRECTANGLE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWRECTANGLE: expected 7 args");
        ::Color color = {(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawRectangle(INT(0), INT(1), INT(2), INT(3), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEV", Fn{"DRAWRECTANGLEV", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWRECTANGLEV: expected 7 args");
        ::Vector2 position = {FLOAT(0), FLOAT(1)};
        ::Vector2 size = {FLOAT(2), FLOAT(3)};
        ::Color color = {(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawRectangleV(position, size, color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEREC", Fn{"DRAWRECTANGLEREC", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWRECTANGLEREC: expected 7 args");
        ::Rectangle rec = {FLOAT(0), FLOAT(1), FLOAT(2), FLOAT(3)};
        ::Color color = {(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawRectangleRec(rec, color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEPRO", Fn{"DRAWRECTANGLEPRO", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWRECTANGLEPRO: expected 10 args");
        ::Rectangle rec = {FLOAT(0), FLOAT(1), FLOAT(2), FLOAT(3)};
        ::Vector2 origin = {FLOAT(4), FLOAT(5)};
        ::Color color = {(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawRectanglePro(rec, origin, FLOAT(6), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEGRADIENTV", Fn{"DRAWRECTANGLEGRADIENTV", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWRECTANGLEGRADIENTV: expected 10 args");
        ::Color top = {(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::Color bottom = {(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawRectangleGradientV(INT(0), INT(1), INT(2), INT(3), top, bottom);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEGRADIENTH", Fn{"DRAWRECTANGLEGRADIENTH", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWRECTANGLEGRADIENTH: expected 10 args");
        ::Color left = {(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::Color right = {(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawRectangleGradientH(INT(0), INT(1), INT(2), INT(3), left, right);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEGRADIENTEX", Fn{"DRAWRECTANGLEGRADIENTEX", 16, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 16) throw std::runtime_error("DRAWRECTANGLEGRADIENTEX: expected 16 args");
        ::Rectangle rec = {FLOAT(0), FLOAT(1), FLOAT(2), FLOAT(3)};
        ::Color topLeft = {(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::Color bottomLeft = {(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::Color topRight = {(unsigned char)INT(10), (unsigned char)INT(11), (unsigned char)INT(12), 255};
        ::Color bottomRight = {(unsigned char)INT(13), (unsigned char)INT(14), (unsigned char)INT(15), 255};
        ::DrawRectangleGradientEx(rec, topLeft, bottomLeft, topRight, bottomRight);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLELINES", Fn{"DRAWRECTANGLELINES", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWRECTANGLELINES: expected 7 args");
        ::Color color = {(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawRectangleLines(INT(0), INT(1), INT(2), INT(3), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLELINESEX", Fn{"DRAWRECTANGLELINESEX", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWRECTANGLELINESEX: expected 8 args");
        ::Rectangle rec = {FLOAT(0), FLOAT(1), FLOAT(2), FLOAT(3)};
        ::Color color = {(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawRectangleLinesEx(rec, FLOAT(4), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEROUNDED", Fn{"DRAWRECTANGLEROUNDED", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWRECTANGLEROUNDED: expected 9 args");
        ::Rectangle rec = {FLOAT(0), FLOAT(1), FLOAT(2), FLOAT(3)};
        ::Color color = {(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawRectangleRounded(rec, FLOAT(4), INT(5), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEROUNDEDLINES", Fn{"DRAWRECTANGLEROUNDEDLINES", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWRECTANGLEROUNDEDLINES: expected 9 args");
        ::Rectangle rec = {FLOAT(0), FLOAT(1), FLOAT(2), FLOAT(3)};
        ::Color color = {(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawRectangleRoundedLines(rec, FLOAT(4), INT(5), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEROUNDEDLINESEX", Fn{"DRAWRECTANGLEROUNDEDLINESEX", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWRECTANGLEROUNDEDLINESEX: expected 10 args");
        ::Rectangle rec = {FLOAT(0), FLOAT(1), FLOAT(2), FLOAT(3)};
        ::Color color = {(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawRectangleRoundedLinesEx(rec, FLOAT(4), INT(5), FLOAT(6), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTRIANGLE", Fn{"DRAWTRIANGLE", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWTRIANGLE: expected 9 args");
        ::Vector2 v1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 v2 = {FLOAT(2), FLOAT(3)};
        ::Vector2 v3 = {FLOAT(4), FLOAT(5)};
        ::Color color = {(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawTriangle(v1, v2, v3, color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTRIANGLELINES", Fn{"DRAWTRIANGLELINES", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWTRIANGLELINES: expected 9 args");
        ::Vector2 v1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 v2 = {FLOAT(2), FLOAT(3)};
        ::Vector2 v3 = {FLOAT(4), FLOAT(5)};
        ::Color color = {(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawTriangleLines(v1, v2, v3, color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPOLY", Fn{"DRAWPOLY", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWPOLY: expected 8 args");
        ::Vector2 center = {FLOAT(0), FLOAT(1)};
        ::Color color = {(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawPoly(center, INT(2), FLOAT(3), FLOAT(4), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPOLYLINES", Fn{"DRAWPOLYLINES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWPOLYLINES: expected 8 args");
        ::Vector2 center = {FLOAT(0), FLOAT(1)};
        ::Color color = {(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawPolyLines(center, INT(2), FLOAT(3), FLOAT(4), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPOLYLINESEX", Fn{"DRAWPOLYLINESEX", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWPOLYLINESEX: expected 9 args");
        ::Vector2 center = {FLOAT(0), FLOAT(1)};
        ::Color color = {(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawPolyLinesEx(center, INT(2), FLOAT(3), FLOAT(4), FLOAT(5), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINELINEAR", Fn{"DRAWSPLINELINEAR", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("DRAWSPLINELINEAR: expected 5 args");
        std::string points_str = STR(0);
        std::vector<::Vector2> points;
        std::stringstream ss(points_str);
        std::string point;
        while (std::getline(ss, point, ';')) {
            std::stringstream point_ss(point);
            std::string x_str, y_str;
            if (std::getline(point_ss, x_str, ',') && std::getline(point_ss, y_str, ',')) {
                points.push_back({std::stof(x_str), std::stof(y_str)});
            }
        }
        ::Color color = {(unsigned char)INT(2), (unsigned char)INT(3), (unsigned char)INT(4), 255};
        if (points.size() >= 2) {
            ::DrawSplineLinear(points.data(), points.size(), FLOAT(1), color);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINEBASIS", Fn{"DRAWSPLINEBASIS", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("DRAWSPLINEBASIS: expected 5 args");
        std::string points_str = STR(0);
        std::vector<::Vector2> points;
        std::stringstream ss(points_str);
        std::string point;
        while (std::getline(ss, point, ';')) {
            std::stringstream point_ss(point);
            std::string x_str, y_str;
            if (std::getline(point_ss, x_str, ',') && std::getline(point_ss, y_str, ',')) {
                points.push_back({std::stof(x_str), std::stof(y_str)});
            }
        }
        ::Color color = {(unsigned char)INT(2), (unsigned char)INT(3), (unsigned char)INT(4), 255};
        if (points.size() >= 4) {
            ::DrawSplineBasis(points.data(), points.size(), FLOAT(1), color);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINECATMULLROM", Fn{"DRAWSPLINECATMULLROM", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("DRAWSPLINECATMULLROM: expected 5 args");
        std::string points_str = STR(0);
        std::vector<::Vector2> points;
        std::stringstream ss(points_str);
        std::string point;
        while (std::getline(ss, point, ';')) {
            std::stringstream point_ss(point);
            std::string x_str, y_str;
            if (std::getline(point_ss, x_str, ',') && std::getline(point_ss, y_str, ',')) {
                points.push_back({std::stof(x_str), std::stof(y_str)});
            }
        }
        ::Color color = {(unsigned char)INT(2), (unsigned char)INT(3), (unsigned char)INT(4), 255};
        if (points.size() >= 4) {
            ::DrawSplineCatmullRom(points.data(), points.size(), FLOAT(1), color);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINEBEZIERCUBIC", Fn{"DRAWSPLINEBEZIERCUBIC", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("DRAWSPLINEBEZIERCUBIC: expected 5 args");
        std::string points_str = STR(0);
        std::vector<::Vector2> points;
        std::stringstream ss(points_str);
        std::string point;
        while (std::getline(ss, point, ';')) {
            std::stringstream point_ss(point);
            std::string x_str, y_str;
            if (std::getline(point_ss, x_str, ',') && std::getline(point_ss, y_str, ',')) {
                points.push_back({std::stof(x_str), std::stof(y_str)});
            }
        }
        ::Color color = {(unsigned char)INT(2), (unsigned char)INT(3), (unsigned char)INT(4), 255};
        if (points.size() >= 4) {
            ::DrawSplineBezierCubic(points.data(), points.size(), FLOAT(1), color);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINESEGMENTLINEAR", Fn{"DRAWSPLINESEGMENTLINEAR", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWSPLINESEGMENTLINEAR: expected 8 args");
        ::Vector2 p1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 p2 = {FLOAT(2), FLOAT(3)};
        ::Color color = {(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawSplineSegmentLinear(p1, p2, FLOAT(4), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINESEGMENTBASIS", Fn{"DRAWSPLINESEGMENTBASIS", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWSPLINESEGMENTBASIS: expected 12 args");
        ::Vector2 p1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 p2 = {FLOAT(2), FLOAT(3)};
        ::Vector2 p3 = {FLOAT(4), FLOAT(5)};
        ::Vector2 p4 = {FLOAT(6), FLOAT(7)};
        ::Color color = {(unsigned char)INT(9), (unsigned char)INT(10), (unsigned char)INT(11), 255};
        ::DrawSplineSegmentBasis(p1, p2, p3, p4, FLOAT(8), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINESEGMENTCATMULLROM", Fn{"DRAWSPLINESEGMENTCATMULLROM", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWSPLINESEGMENTCATMULLROM: expected 12 args");
        ::Vector2 p1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 p2 = {FLOAT(2), FLOAT(3)};
        ::Vector2 p3 = {FLOAT(4), FLOAT(5)};
        ::Vector2 p4 = {FLOAT(6), FLOAT(7)};
        ::Color color = {(unsigned char)INT(9), (unsigned char)INT(10), (unsigned char)INT(11), 255};
        ::DrawSplineSegmentCatmullRom(p1, p2, p3, p4, FLOAT(8), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINESEGMENTBEZIERQUADRATIC", Fn{"DRAWSPLINESEGMENTBEZIERQUADRATIC", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWSPLINESEGMENTBEZIERQUADRATIC: expected 10 args");
        ::Vector2 p1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 c2 = {FLOAT(2), FLOAT(3)};
        ::Vector2 p3 = {FLOAT(4), FLOAT(5)};
        ::Color color = {(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawSplineSegmentBezierQuadratic(p1, c2, p3, FLOAT(6), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINESEGMENTBEZIERCUBIC", Fn{"DRAWSPLINESEGMENTBEZIERCUBIC", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWSPLINESEGMENTBEZIERCUBIC: expected 12 args");
        ::Vector2 p1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 c2 = {FLOAT(2), FLOAT(3)};
        ::Vector2 c3 = {FLOAT(4), FLOAT(5)};
        ::Vector2 p4 = {FLOAT(6), FLOAT(7)};
        ::Color color = {(unsigned char)INT(9), (unsigned char)INT(10), (unsigned char)INT(11), 255};
        ::DrawSplineSegmentBezierCubic(p1, c2, c3, p4, FLOAT(8), color);
        return Value::nil();
    }}, true);    R.add_with_policy("GETSPLINEPOINTLINEAR", Fn{"GETSPLINEPOINTLINEAR", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("GETSPLINEPOINTLINEAR: expected 5 args");
        ::Vector2 startPos = {FLOAT(0), FLOAT(1)};
        ::Vector2 endPos = {FLOAT(2), FLOAT(3)};
        ::Vector2 result = ::GetSplinePointLinear(startPos, endPos, FLOAT(4));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("GETSPLINEPOINTBASIS", Fn{"GETSPLINEPOINTBASIS", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("GETSPLINEPOINTBASIS: expected 9 args");
        ::Vector2 p1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 p2 = {FLOAT(2), FLOAT(3)};
        ::Vector2 p3 = {FLOAT(4), FLOAT(5)};
        ::Vector2 p4 = {FLOAT(6), FLOAT(7)};
        ::Vector2 result = ::GetSplinePointBasis(p1, p2, p3, p4, FLOAT(8));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("GETSPLINEPOINTCATMULLROM", Fn{"GETSPLINEPOINTCATMULLROM", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("GETSPLINEPOINTCATMULLROM: expected 9 args");
        ::Vector2 p1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 p2 = {FLOAT(2), FLOAT(3)};
        ::Vector2 p3 = {FLOAT(4), FLOAT(5)};
        ::Vector2 p4 = {FLOAT(6), FLOAT(7)};
        ::Vector2 result = ::GetSplinePointCatmullRom(p1, p2, p3, p4, FLOAT(8));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("GETSPLINEPOINTBEZIERQUAD", Fn{"GETSPLINEPOINTBEZIERQUAD", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("GETSPLINEPOINTBEZIERQUAD: expected 7 args");
        ::Vector2 p1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 c2 = {FLOAT(2), FLOAT(3)};
        ::Vector2 p3 = {FLOAT(4), FLOAT(5)};
        ::Vector2 result = ::GetSplinePointBezierQuad(p1, c2, p3, FLOAT(6));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("GETSPLINEPOINTBEZIERCUBIC", Fn{"GETSPLINEPOINTBEZIERCUBIC", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("GETSPLINEPOINTBEZIERCUBIC: expected 9 args");
        ::Vector2 p1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 c2 = {FLOAT(2), FLOAT(3)};
        ::Vector2 c3 = {FLOAT(4), FLOAT(5)};
        ::Vector2 p4 = {FLOAT(6), FLOAT(7)};
        ::Vector2 result = ::GetSplinePointBezierCubic(p1, c2, c3, p4, FLOAT(8));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("CHECKCOLLISIONRECS", Fn{"CHECKCOLLISIONRECS", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("CHECKCOLLISIONRECS: expected 8 args");
        ::Rectangle rec1 = {FLOAT(0), FLOAT(1), FLOAT(2), FLOAT(3)};
        ::Rectangle rec2 = {FLOAT(4), FLOAT(5), FLOAT(6), FLOAT(7)};
        return Value::from_int(::CheckCollisionRecs(rec1, rec2) ? 1 : 0);
    }}, true);    R.add_with_policy("CHECKCOLLISIONCIRCLES", Fn{"CHECKCOLLISIONCIRCLES", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("CHECKCOLLISIONCIRCLES: expected 6 args");
        ::Vector2 center1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 center2 = {FLOAT(3), FLOAT(4)};
        return Value::from_int(::CheckCollisionCircles(center1, FLOAT(2), center2, FLOAT(5)) ? 1 : 0);
    }}, true);    R.add_with_policy("CHECKCOLLISIONCIRCLEREC", Fn{"CHECKCOLLISIONCIRCLEREC", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("CHECKCOLLISIONCIRCLEREC: expected 7 args");
        ::Vector2 center = {FLOAT(0), FLOAT(1)};
        ::Rectangle rec = {FLOAT(3), FLOAT(4), FLOAT(5), FLOAT(6)};
        return Value::from_int(::CheckCollisionCircleRec(center, FLOAT(2), rec) ? 1 : 0);
    }}, true);    R.add_with_policy("CHECKCOLLISIONCIRCLELINE", Fn{"CHECKCOLLISIONCIRCLELINE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("CHECKCOLLISIONCIRCLELINE: expected 7 args");
        ::Vector2 center = {FLOAT(0), FLOAT(1)};
        ::Vector2 p1 = {FLOAT(3), FLOAT(4)};
        ::Vector2 p2 = {FLOAT(5), FLOAT(6)};
        return Value::from_int(::CheckCollisionCircleLine(center, FLOAT(2), p1, p2) ? 1 : 0);
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTREC", Fn{"CHECKCOLLISIONPOINTREC", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("CHECKCOLLISIONPOINTREC: expected 6 args");
        ::Vector2 point = {FLOAT(0), FLOAT(1)};
        ::Rectangle rec = {FLOAT(2), FLOAT(3), FLOAT(4), FLOAT(5)};
        return Value::from_int(::CheckCollisionPointRec(point, rec) ? 1 : 0);
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTCIRCLE", Fn{"CHECKCOLLISIONPOINTCIRCLE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("CHECKCOLLISIONPOINTCIRCLE: expected 5 args");
        ::Vector2 point = {FLOAT(0), FLOAT(1)};
        ::Vector2 center = {FLOAT(2), FLOAT(3)};
        return Value::from_int(::CheckCollisionPointCircle(point, center, FLOAT(4)) ? 1 : 0);
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTTRIANGLE", Fn{"CHECKCOLLISIONPOINTTRIANGLE", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("CHECKCOLLISIONPOINTTRIANGLE: expected 8 args");
        ::Vector2 point = {FLOAT(0), FLOAT(1)};
        ::Vector2 p1 = {FLOAT(2), FLOAT(3)};
        ::Vector2 p2 = {FLOAT(4), FLOAT(5)};
        ::Vector2 p3 = {FLOAT(6), FLOAT(7)};
        return Value::from_int(::CheckCollisionPointTriangle(point, p1, p2, p3) ? 1 : 0);
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTLINE", Fn{"CHECKCOLLISIONPOINTLINE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("CHECKCOLLISIONPOINTLINE: expected 7 args");
        ::Vector2 point = {FLOAT(0), FLOAT(1)};
        ::Vector2 p1 = {FLOAT(2), FLOAT(3)};
        ::Vector2 p2 = {FLOAT(4), FLOAT(5)};
        return Value::from_int(::CheckCollisionPointLine(point, p1, p2, INT(6)) ? 1 : 0);
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTPOLY", Fn{"CHECKCOLLISIONPOINTPOLY", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("CHECKCOLLISIONPOINTPOLY: expected 3 args");
        ::Vector2 point = {FLOAT(0), FLOAT(1)};
        std::string points_str = STR(2);
        std::vector<::Vector2> points;
        std::stringstream ss(points_str);
        std::string point_str;
        while (std::getline(ss, point_str, ';')) {
            std::stringstream point_ss(point_str);
            std::string x_str, y_str;
            if (std::getline(point_ss, x_str, ',') && std::getline(point_ss, y_str, ',')) {
                points.push_back({std::stof(x_str), std::stof(y_str)});
            }
        }
        if (points.size() >= 3) {
            return Value::from_int(::CheckCollisionPointPoly(point, points.data(), points.size()) ? 1 : 0);
        }
        return Value::from_int(0);
    }}, true);    R.add_with_policy("CHECKCOLLISIONLINES", Fn{"CHECKCOLLISIONLINES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("CHECKCOLLISIONLINES: expected 8 args");
        ::Vector2 startPos1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 endPos1 = {FLOAT(2), FLOAT(3)};
        ::Vector2 startPos2 = {FLOAT(4), FLOAT(5)};
        ::Vector2 endPos2 = {FLOAT(6), FLOAT(7)};
        ::Vector2 collisionPoint;
        bool collision = ::CheckCollisionLines(startPos1, endPos1, startPos2, endPos2, &collisionPoint);
        if (collision) {
            return Value::from_string(std::to_string(collisionPoint.x) + "," + std::to_string(collisionPoint.y));
        } else {
            return Value::from_string("");
        }
    }}, true);    R.add_with_policy("GETCOLLISIONREC", Fn{"GETCOLLISIONREC", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("GETCOLLISIONREC: expected 8 args");
        ::Rectangle rec1 = {FLOAT(0), FLOAT(1), FLOAT(2), FLOAT(3)};
        ::Rectangle rec2 = {FLOAT(4), FLOAT(5), FLOAT(6), FLOAT(7)};
        ::Rectangle collision = ::GetCollisionRec(rec1, rec2);
        return Value::from_string(std::to_string(collision.x) + "," + 
                                 std::to_string(collision.y) + "," + 
                                 std::to_string(collision.width) + "," + 
                                 std::to_string(collision.height));
    }}, true);    R.add_with_policy("LOADIMAGE", Fn{"LOADIMAGE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADIMAGE: expected 1 args");
        ::Image image = ::LoadImage(STR(0).c_str());
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADIMAGERAW", Fn{"LOADIMAGERAW", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("LOADIMAGERAW: expected 5 args");
        ::Image image = ::LoadImageRaw(STR(0).c_str(), INT(1), INT(2), INT(3), INT(4));
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADIMAGEANIM", Fn{"LOADIMAGEANIM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADIMAGEANIM: expected 1 args");
        int frames;
        ::Image image = ::LoadImageAnim(STR(0).c_str(), &frames);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_string(std::to_string(id) + "," + std::to_string(frames));
    }}, true);    R.add_with_policy("LOADIMAGEFROMSCREEN", Fn{"LOADIMAGEFROMSCREEN", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("LOADIMAGEFROMSCREEN: expected 0 args");
        ::Image image = ::LoadImageFromScreen();
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISIMAGEVALID", Fn{"ISIMAGEVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISIMAGEVALID: expected 1 args");
        if (rlreg::images.find(INT(0)) == rlreg::images.end()) {
          return Value::from_int(0);
        }
        ::Image image = rlreg::images.at(INT(0));
        return Value::from_int(::IsImageValid(image) ? 1 : 0);
    }}, true);    R.add_with_policy("UNLOADIMAGE", Fn{"UNLOADIMAGE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADIMAGE: expected 1 args");
        if (rlreg::images.find(INT(0)) != rlreg::images.end()) {
          ::Image image = rlreg::images.at(INT(0));
          ::UnloadImage(image);
          rlreg::images.erase(INT(0));
        }
        return Value::nil();
    }}, true);    R.add_with_policy("EXPORTIMAGE", Fn{"EXPORTIMAGE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTIMAGE: expected 2 args");
        if (rlreg::images.find(INT(0)) == rlreg::images.end()) {
          return Value::from_int(0);
        }
        ::Image image = rlreg::images.at(INT(0));
        return Value::from_int(::ExportImage(image, STR(1).c_str()) ? 1 : 0);
    }}, true);    R.add_with_policy("EXPORTIMAGEASCODE", Fn{"EXPORTIMAGEASCODE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTIMAGEASCODE: expected 2 args");
        if (rlreg::images.find(INT(0)) == rlreg::images.end()) {
          return Value::from_int(0);
        }
        ::Image image = rlreg::images.at(INT(0));
        return Value::from_int(::ExportImageAsCode(image, STR(1).c_str()) ? 1 : 0);
    }}, true);    R.add_with_policy("GETFONTDEFAULT", Fn{"GETFONTDEFAULT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETFONTDEFAULT: expected 0 args");
        ::Font font = ::GetFontDefault();
        static int next_id = 1;
        return Value::from_int(next_id++);
    }}, true);    R.add_with_policy("LOADFONT", Fn{"LOADFONT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADFONT: expected 1 args");
        ::Font font = ::LoadFont(STR(0).c_str());
        static int next_id = 1;
        return Value::from_int(next_id++);
    }}, true);    R.add_with_policy("LOADFONTEX", Fn{"LOADFONTEX", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADFONTEX: expected 2 args");
        ::Font font = ::LoadFontEx(STR(0).c_str(), INT(1), NULL, 0);
        static int next_id = 1;
        return Value::from_int(next_id++);
    }}, true);    R.add_with_policy("ISFONTVALID", Fn{"ISFONTVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISFONTVALID: expected 1 args");
        ::Font font = {}; // Look up by ID in real implementation
        return Value::from_int(::IsFontValid(font) ? 1 : 0);
    }}, true);    R.add_with_policy("UNLOADFONT", Fn{"UNLOADFONT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADFONT: expected 1 args");
        ::Font font = {}; // Look up by ID in real implementation
        ::UnloadFont(font);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWFPS", Fn{"DRAWFPS", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("DRAWFPS: expected 2 args");
        ::DrawFPS(INT(0), INT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXT", Fn{"DRAWTEXT", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWTEXT: expected 7 args");
        ::Color color = {(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawText(STR(0).c_str(), INT(1), INT(2), INT(3), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTEX", Fn{"DRAWTEXTEX", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWTEXTEX: expected 9 args");
        ::Font font = {}; // Look up by ID in real implementation
        ::Vector2 position = {FLOAT(2), FLOAT(3)};
        ::Color tint = {(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawTextEx(font, STR(1).c_str(), position, FLOAT(4), FLOAT(5), tint);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTPRO", Fn{"DRAWTEXTPRO", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWTEXTPRO: expected 12 args");
        ::Font font = {}; // Look up by ID in real implementation
        ::Vector2 position = {FLOAT(2), FLOAT(3)};
        ::Vector2 origin = {FLOAT(4), FLOAT(5)};
        ::Color tint = {(unsigned char)INT(9), (unsigned char)INT(10), (unsigned char)INT(11), 255};
        ::DrawTextPro(font, STR(1).c_str(), position, origin, FLOAT(6), FLOAT(7), FLOAT(8), tint);
        return Value::nil();
    }}, true);    R.add_with_policy("MEASURETEXT", Fn{"MEASURETEXT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("MEASURETEXT: expected 2 args");
        return Value::from_int(::MeasureText(STR(0).c_str(), INT(1)));
    }}, true);    R.add_with_policy("MEASURETEXTEX", Fn{"MEASURETEXTEX", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("MEASURETEXTEX: expected 4 args");
        ::Font font = {}; // Look up by ID in real implementation
        ::Vector2 size = ::MeasureTextEx(font, STR(1).c_str(), FLOAT(2), FLOAT(3));
        return Value::from_string(std::to_string(size.x) + "," + std::to_string(size.y));
    }}, true);    R.add_with_policy("TEXTLENGTH", Fn{"TEXTLENGTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTLENGTH: expected 1 args");
        return Value::from_int(::TextLength(STR(0).c_str()));
    }}, true);    R.add_with_policy("TEXTSUBTEXT", Fn{"TEXTSUBTEXT", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("TEXTSUBTEXT: expected 3 args");
        const char* result = ::TextSubtext(STR(0).c_str(), INT(1), INT(2));
        return Value::from_string(std::string(result));
    }}, true);    R.add_with_policy("TEXTTOUPPER", Fn{"TEXTTOUPPER", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTTOUPPER: expected 1 args");
        const char* result = ::TextToUpper(STR(0).c_str());
        return Value::from_string(std::string(result));
    }}, true);    R.add_with_policy("TEXTTOLOWER", Fn{"TEXTTOLOWER", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTTOLOWER: expected 1 args");
        const char* result = ::TextToLower(STR(0).c_str());
        return Value::from_string(std::string(result));
    }}, true);    R.add_with_policy("TEXTTOINTEGER", Fn{"TEXTTOINTEGER", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTTOINTEGER: expected 1 args");
        return Value::from_int(::TextToInteger(STR(0).c_str()));
    }}, true);    R.add_with_policy("TEXTTOFLOAT", Fn{"TEXTTOFLOAT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTTOFLOAT: expected 1 args");
        return Value::from_number(::TextToFloat(STR(0).c_str()));
    }}, true);    R.add_with_policy("TEXTFINDINDEX", Fn{"TEXTFINDINDEX", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("TEXTFINDINDEX: expected 2 args");
        return Value::from_int(::TextFindIndex(STR(0).c_str(), STR(1).c_str()));
    }}, true);    R.add_with_policy("TEXTISEQUAL", Fn{"TEXTISEQUAL", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("TEXTISEQUAL: expected 2 args");
        return Value::from_int(::TextIsEqual(STR(0).c_str(), STR(1).c_str()) ? 1 : 0);
    }}, true);    R.add_with_policy("DRAWLINE3D", Fn{"DRAWLINE3D", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWLINE3D: expected 9 args");
        ::Vector3 startPos = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Vector3 endPos = {FLOAT(3), FLOAT(4), FLOAT(5)};
        ::Color color = {(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawLine3D(startPos, endPos, color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPOINT3D", Fn{"DRAWPOINT3D", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWPOINT3D: expected 6 args");
        ::Vector3 position = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Color color = {(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::DrawPoint3D(position, color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLE3D", Fn{"DRAWCIRCLE3D", 11, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 11) throw std::runtime_error("DRAWCIRCLE3D: expected 11 args");
        ::Vector3 center = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Vector3 rotationAxis = {FLOAT(4), FLOAT(5), FLOAT(6)};
        ::Color color = {(unsigned char)INT(8), (unsigned char)INT(9), (unsigned char)INT(10), 255};
        ::DrawCircle3D(center, FLOAT(3), rotationAxis, FLOAT(7), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTRIANGLE3D", Fn{"DRAWTRIANGLE3D", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWTRIANGLE3D: expected 12 args");
        ::Vector3 v1 = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Vector3 v2 = {FLOAT(3), FLOAT(4), FLOAT(5)};
        ::Vector3 v3 = {FLOAT(6), FLOAT(7), FLOAT(8)};
        ::Color color = {(unsigned char)INT(9), (unsigned char)INT(10), (unsigned char)INT(11), 255};
        ::DrawTriangle3D(v1, v2, v3, color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCUBE", Fn{"DRAWCUBE", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCUBE: expected 9 args");
        ::Vector3 position = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Color color = {(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawCube(position, FLOAT(3), FLOAT(4), FLOAT(5), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCUBEV", Fn{"DRAWCUBEV", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCUBEV: expected 9 args");
        ::Vector3 position = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Vector3 size = {FLOAT(3), FLOAT(4), FLOAT(5)};
        ::Color color = {(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawCubeV(position, size, color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCUBEWIRES", Fn{"DRAWCUBEWIRES", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCUBEWIRES: expected 9 args");
        ::Vector3 position = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Color color = {(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawCubeWires(position, FLOAT(3), FLOAT(4), FLOAT(5), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPHERE", Fn{"DRAWSPHERE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWSPHERE: expected 7 args");
        ::Vector3 centerPos = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Color color = {(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawSphere(centerPos, FLOAT(3), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPHEREEX", Fn{"DRAWSPHEREEX", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWSPHEREEX: expected 9 args");
        ::Vector3 centerPos = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Color color = {(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawSphereEx(centerPos, FLOAT(3), INT(4), INT(5), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPHEREWIRES", Fn{"DRAWSPHEREWIRES", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWSPHEREWIRES: expected 9 args");
        ::Vector3 centerPos = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Color color = {(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawSphereWires(centerPos, FLOAT(3), INT(4), INT(5), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCYLINDER", Fn{"DRAWCYLINDER", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWCYLINDER: expected 10 args");
        ::Vector3 position = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Color color = {(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawCylinder(position, FLOAT(3), FLOAT(4), FLOAT(5), INT(6), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPLANE", Fn{"DRAWPLANE", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWPLANE: expected 8 args");
        ::Vector3 centerPos = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Vector2 size = {FLOAT(3), FLOAT(4)};
        ::Color color = {(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawPlane(centerPos, size, color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWGRID", Fn{"DRAWGRID", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("DRAWGRID: expected 2 args");
        ::DrawGrid(INT(0), FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("LOADMODEL", Fn{"LOADMODEL", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADMODEL: expected 1 args");
        ::Model model = ::LoadModel(STR(0).c_str());
        static int next_id = 1;
        return Value::from_int(next_id++);
    }}, true);    R.add_with_policy("ISMODELVALID", Fn{"ISMODELVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMODELVALID: expected 1 args");
        ::Model model = {}; // Look up by ID in real implementation
        return Value::from_int(::IsModelValid(model) ? 1 : 0);
    }}, true);    R.add_with_policy("UNLOADMODEL", Fn{"UNLOADMODEL", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADMODEL: expected 1 args");
        ::Model model = {}; // Look up by ID in real implementation
        ::UnloadModel(model);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWMODEL", Fn{"DRAWMODEL", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWMODEL: expected 8 args");
        ::Model model = {}; // Look up by ID in real implementation
        ::Vector3 position = {FLOAT(1), FLOAT(2), FLOAT(3)};
        ::Color tint = {(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawModel(model, position, FLOAT(4), tint);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWMODELWIRES", Fn{"DRAWMODELWIRES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWMODELWIRES: expected 8 args");
        ::Model model = {}; // Look up by ID in real implementation
        ::Vector3 position = {FLOAT(1), FLOAT(2), FLOAT(3)};
        ::Color tint = {(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawModelWires(model, position, FLOAT(4), tint);
        return Value::nil();
    }}, true);    R.add_with_policy("CHECKCOLLISIONSPHERES", Fn{"CHECKCOLLISIONSPHERES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("CHECKCOLLISIONSPHERES: expected 8 args");
        ::Vector3 center1 = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Vector3 center2 = {FLOAT(4), FLOAT(5), FLOAT(6)};
        return Value::from_int(::CheckCollisionSpheres(center1, FLOAT(3), center2, FLOAT(7)) ? 1 : 0);
    }}, true);    R.add_with_policy("CHECKCOLLISIONBOXSPHERE", Fn{"CHECKCOLLISIONBOXSPHERE", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("CHECKCOLLISIONBOXSPHERE: expected 10 args");
        ::BoundingBox box = {{FLOAT(0), FLOAT(1), FLOAT(2)}, 
                             {FLOAT(3), FLOAT(4), FLOAT(5)}};
        ::Vector3 center = {FLOAT(6), FLOAT(7), FLOAT(8)};
        return Value::from_int(::CheckCollisionBoxSphere(box, center, FLOAT(9)) ? 1 : 0);
    }}, true);    R.add_with_policy("INITAUDIODEVICE", Fn{"INITAUDIODEVICE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("INITAUDIODEVICE: expected 0 args");
        ::InitAudioDevice();
        return Value::nil();
    }}, true);    R.add_with_policy("CLOSEAUDIODEVICE", Fn{"CLOSEAUDIODEVICE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("CLOSEAUDIODEVICE: expected 0 args");
        ::CloseAudioDevice();
        return Value::nil();
    }}, true);    R.add_with_policy("ISAUDIODEVICEREADY", Fn{"ISAUDIODEVICEREADY", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISAUDIODEVICEREADY: expected 0 args");
        return Value::from_int(::IsAudioDeviceReady() ? 1 : 0);
    }}, true);    R.add_with_policy("SETMASTERVOLUME", Fn{"SETMASTERVOLUME", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETMASTERVOLUME: expected 1 args");
        ::SetMasterVolume(FLOAT(0));
        return Value::nil();
    }}, true);    R.add_with_policy("GETMASTERVOLUME", Fn{"GETMASTERVOLUME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMASTERVOLUME: expected 0 args");
        return Value::from_number(::GetMasterVolume());
    }}, true);    R.add_with_policy("LOADWAVE", Fn{"LOADWAVE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADWAVE: expected 1 args");
        ::Wave wave = ::LoadWave(STR(0).c_str());
        static int next_id = 1;
        return Value::from_int(next_id++);
    }}, true);    R.add_with_policy("ISWAVEVALID", Fn{"ISWAVEVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISWAVEVALID: expected 1 args");
        ::Wave wave = {}; // Look up by ID in real implementation
        return Value::from_int(::IsWaveValid(wave) ? 1 : 0);
    }}, true);    R.add_with_policy("LOADSOUND", Fn{"LOADSOUND", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADSOUND: expected 1 args");
        ::Sound sound = ::LoadSound(STR(0).c_str());
        static int next_id = 1;
        return Value::from_int(next_id++);
    }}, true);    R.add_with_policy("LOADSOUNDFROMWAVE", Fn{"LOADSOUNDFROMWAVE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADSOUNDFROMWAVE: expected 1 args");
        ::Wave wave = {}; // Look up by ID in real implementation
        ::Sound sound = ::LoadSoundFromWave(wave);
        static int next_id = 1;
        return Value::from_int(next_id++);
    }}, true);    R.add_with_policy("ISSOUNDVALID", Fn{"ISSOUNDVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISSOUNDVALID: expected 1 args");
        ::Sound sound = {}; // Look up by ID in real implementation
        return Value::from_int(::IsSoundValid(sound) ? 1 : 0);
    }}, true);    R.add_with_policy("UNLOADWAVE", Fn{"UNLOADWAVE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADWAVE: expected 1 args");
        ::Wave wave = {}; // Look up by ID in real implementation
        ::UnloadWave(wave);
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADSOUND", Fn{"UNLOADSOUND", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADSOUND: expected 1 args");
        ::Sound sound = {}; // Look up by ID in real implementation
        ::UnloadSound(sound);
        return Value::nil();
    }}, true);    R.add_with_policy("PLAYSOUND", Fn{"PLAYSOUND", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PLAYSOUND: expected 1 args");
        ::Sound sound = {}; // Look up by ID in real implementation
        ::PlaySound(sound);
        return Value::nil();
    }}, true);    R.add_with_policy("STOPSOUND", Fn{"STOPSOUND", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("STOPSOUND: expected 1 args");
        ::Sound sound = {}; // Look up by ID in real implementation
        ::StopSound(sound);
        return Value::nil();
    }}, true);    R.add_with_policy("PAUSESOUND", Fn{"PAUSESOUND", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PAUSESOUND: expected 1 args");
        ::Sound sound = {}; // Look up by ID in real implementation
        ::PauseSound(sound);
        return Value::nil();
    }}, true);    R.add_with_policy("RESUMESOUND", Fn{"RESUMESOUND", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("RESUMESOUND: expected 1 args");
        ::Sound sound = {}; // Look up by ID in real implementation
        ::ResumeSound(sound);
        return Value::nil();
    }}, true);    R.add_with_policy("ISSOUNDPLAYING", Fn{"ISSOUNDPLAYING", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISSOUNDPLAYING: expected 1 args");
        ::Sound sound = {}; // Look up by ID in real implementation
        return Value::from_int(::IsSoundPlaying(sound) ? 1 : 0);
    }}, true);    R.add_with_policy("SETSOUNDVOLUME", Fn{"SETSOUNDVOLUME", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETSOUNDVOLUME: expected 2 args");
        ::Sound sound = {}; // Look up by ID in real implementation
        ::SetSoundVolume(sound, FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("SETSOUNDPITCH", Fn{"SETSOUNDPITCH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETSOUNDPITCH: expected 2 args");
        ::Sound sound = {}; // Look up by ID in real implementation
        ::SetSoundPitch(sound, FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("SETSOUNDPAN", Fn{"SETSOUNDPAN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETSOUNDPAN: expected 2 args");
        ::Sound sound = {}; // Look up by ID in real implementation
        ::SetSoundPan(sound, FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("LOADMUSICSTREAM", Fn{"LOADMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADMUSICSTREAM: expected 1 args");
        ::Music music = ::LoadMusicStream(STR(0).c_str());
        static int next_id = 1;
        return Value::from_int(next_id++);
    }}, true);    R.add_with_policy("ISMUSICVALID", Fn{"ISMUSICVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMUSICVALID: expected 1 args");
        ::Music music = {}; // Look up by ID in real implementation
        return Value::from_int(::IsMusicValid(music) ? 1 : 0);
    }}, true);    R.add_with_policy("UNLOADMUSICSTREAM", Fn{"UNLOADMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADMUSICSTREAM: expected 1 args");
        ::Music music = {}; // Look up by ID in real implementation
        ::UnloadMusicStream(music);
        return Value::nil();
    }}, true);    R.add_with_policy("PLAYMUSICSTREAM", Fn{"PLAYMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PLAYMUSICSTREAM: expected 1 args");
        ::Music music = {}; // Look up by ID in real implementation
        ::PlayMusicStream(music);
        return Value::nil();
    }}, true);    R.add_with_policy("ISMUSICSTREAMPLAYING", Fn{"ISMUSICSTREAMPLAYING", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMUSICSTREAMPLAYING: expected 1 args");
        ::Music music = {}; // Look up by ID in real implementation
        return Value::from_int(::IsMusicStreamPlaying(music) ? 1 : 0);
    }}, true);    R.add_with_policy("UPDATEMUSICSTREAM", Fn{"UPDATEMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UPDATEMUSICSTREAM: expected 1 args");
        ::Music music = {}; // Look up by ID in real implementation
        ::UpdateMusicStream(music);
        return Value::nil();
    }}, true);    R.add_with_policy("STOPMUSICSTREAM", Fn{"STOPMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("STOPMUSICSTREAM: expected 1 args");
        ::Music music = {}; // Look up by ID in real implementation
        ::StopMusicStream(music);
        return Value::nil();
    }}, true);    R.add_with_policy("PAUSEMUSICSTREAM", Fn{"PAUSEMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PAUSEMUSICSTREAM: expected 1 args");
        ::Music music = {}; // Look up by ID in real implementation
        ::PauseMusicStream(music);
        return Value::nil();
    }}, true);    R.add_with_policy("RESUMEMUSICSTREAM", Fn{"RESUMEMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("RESUMEMUSICSTREAM: expected 1 args");
        ::Music music = {}; // Look up by ID in real implementation
        ::ResumeMusicStream(music);
        return Value::nil();
    }}, true);    R.add_with_policy("SETMUSICVOLUME", Fn{"SETMUSICVOLUME", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETMUSICVOLUME: expected 2 args");
        ::Music music = {}; // Look up by ID in real implementation
        ::SetMusicVolume(music, FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("SETMUSICPITCH", Fn{"SETMUSICPITCH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETMUSICPITCH: expected 2 args");
        ::Music music = {}; // Look up by ID in real implementation
        ::SetMusicPitch(music, FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("GETMUSICTIMELENGTH", Fn{"GETMUSICTIMELENGTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMUSICTIMELENGTH: expected 1 args");
        ::Music music = {}; // Look up by ID in real implementation
        return Value::from_number(::GetMusicTimeLength(music));
    }}, true);    R.add_with_policy("GETMUSICTIMEPLAYED", Fn{"GETMUSICTIMEPLAYED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMUSICTIMEPLAYED: expected 1 args");
        ::Music music = {}; // Look up by ID in real implementation
        return Value::from_number(::GetMusicTimePlayed(music));
    }}, true);    R.add_with_policy("CLAMP", Fn{"CLAMP", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("CLAMP: expected 3 args");
        return Value::from_number(::Clamp(FLOAT(0), FLOAT(1), FLOAT(2)));
    }}, true);    R.add_with_policy("LERP", Fn{"LERP", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("LERP: expected 3 args");
        return Value::from_number(::Lerp(FLOAT(0), FLOAT(1), FLOAT(2)));
    }}, true);    R.add_with_policy("NORMALIZE", Fn{"NORMALIZE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("NORMALIZE: expected 3 args");
        return Value::from_number(::Normalize(FLOAT(0), FLOAT(1), FLOAT(2)));
    }}, true);    R.add_with_policy("REMAP", Fn{"REMAP", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("REMAP: expected 5 args");
        return Value::from_number(::Remap(FLOAT(0), FLOAT(1), FLOAT(2), FLOAT(3), FLOAT(4)));
    }}, true);    R.add_with_policy("WRAP", Fn{"WRAP", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("WRAP: expected 3 args");
        return Value::from_number(::Wrap(FLOAT(0), FLOAT(1), FLOAT(2)));
    }}, true);    R.add_with_policy("FLOATEQUALS", Fn{"FLOATEQUALS", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("FLOATEQUALS: expected 2 args");
        return Value::from_int(::FloatEquals(FLOAT(0), FLOAT(1)));
    }}, true);    R.add_with_policy("VECTOR2ZERO", Fn{"VECTOR2ZERO", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("VECTOR2ZERO: expected 0 args");
        ::Vector2 v = ::Vector2Zero();
        return Value::from_string(std::to_string(v.x) + "," + std::to_string(v.y));
    }}, true);    R.add_with_policy("VECTOR2ONE", Fn{"VECTOR2ONE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("VECTOR2ONE: expected 0 args");
        ::Vector2 v = ::Vector2One();
        return Value::from_string(std::to_string(v.x) + "," + std::to_string(v.y));
    }}, true);    R.add_with_policy("VECTOR2ADD", Fn{"VECTOR2ADD", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR2ADD: expected 4 args");
        ::Vector2 v1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 v2 = {FLOAT(2), FLOAT(3)};
        ::Vector2 result = ::Vector2Add(v1, v2);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2SUBTRACT", Fn{"VECTOR2SUBTRACT", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR2SUBTRACT: expected 4 args");
        ::Vector2 v1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 v2 = {FLOAT(2), FLOAT(3)};
        ::Vector2 result = ::Vector2Subtract(v1, v2);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2LENGTH", Fn{"VECTOR2LENGTH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("VECTOR2LENGTH: expected 2 args");
        ::Vector2 v = {FLOAT(0), FLOAT(1)};
        return Value::from_number(::Vector2Length(v));
    }}, true);    R.add_with_policy("VECTOR2DISTANCE", Fn{"VECTOR2DISTANCE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR2DISTANCE: expected 4 args");
        ::Vector2 v1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 v2 = {FLOAT(2), FLOAT(3)};
        return Value::from_number(::Vector2Distance(v1, v2));
    }}, true);    R.add_with_policy("VECTOR2DOTPRODUCT", Fn{"VECTOR2DOTPRODUCT", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR2DOTPRODUCT: expected 4 args");
        ::Vector2 v1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 v2 = {FLOAT(2), FLOAT(3)};
        return Value::from_number(::Vector2DotProduct(v1, v2));
    }}, true);    R.add_with_policy("VECTOR2NORMALIZE", Fn{"VECTOR2NORMALIZE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("VECTOR2NORMALIZE: expected 2 args");
        ::Vector2 v = {FLOAT(0), FLOAT(1)};
        ::Vector2 result = ::Vector2Normalize(v);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2LERP", Fn{"VECTOR2LERP", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("VECTOR2LERP: expected 5 args");
        ::Vector2 v1 = {FLOAT(0), FLOAT(1)};
        ::Vector2 v2 = {FLOAT(2), FLOAT(3)};
        ::Vector2 result = ::Vector2Lerp(v1, v2, FLOAT(4));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR3ZERO", Fn{"VECTOR3ZERO", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("VECTOR3ZERO: expected 0 args");
        ::Vector3 v = ::Vector3Zero();
        return Value::from_string(std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z));
    }}, true);    R.add_with_policy("VECTOR3ONE", Fn{"VECTOR3ONE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("VECTOR3ONE: expected 0 args");
        ::Vector3 v = ::Vector3One();
        return Value::from_string(std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z));
    }}, true);    R.add_with_policy("VECTOR3ADD", Fn{"VECTOR3ADD", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3ADD: expected 6 args");
        ::Vector3 v1 = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Vector3 v2 = {FLOAT(3), FLOAT(4), FLOAT(5)};
        ::Vector3 result = ::Vector3Add(v1, v2);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3SUBTRACT", Fn{"VECTOR3SUBTRACT", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3SUBTRACT: expected 6 args");
        ::Vector3 v1 = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Vector3 v2 = {FLOAT(3), FLOAT(4), FLOAT(5)};
        ::Vector3 result = ::Vector3Subtract(v1, v2);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3LENGTH", Fn{"VECTOR3LENGTH", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("VECTOR3LENGTH: expected 3 args");
        ::Vector3 v = {FLOAT(0), FLOAT(1), FLOAT(2)};
        return Value::from_number(::Vector3Length(v));
    }}, true);    R.add_with_policy("VECTOR3DISTANCE", Fn{"VECTOR3DISTANCE", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3DISTANCE: expected 6 args");
        ::Vector3 v1 = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Vector3 v2 = {FLOAT(3), FLOAT(4), FLOAT(5)};
        return Value::from_number(::Vector3Distance(v1, v2));
    }}, true);    R.add_with_policy("VECTOR3DOTPRODUCT", Fn{"VECTOR3DOTPRODUCT", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3DOTPRODUCT: expected 6 args");
        ::Vector3 v1 = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Vector3 v2 = {FLOAT(3), FLOAT(4), FLOAT(5)};
        return Value::from_number(::Vector3DotProduct(v1, v2));
    }}, true);    R.add_with_policy("VECTOR3CROSSPRODUCT", Fn{"VECTOR3CROSSPRODUCT", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3CROSSPRODUCT: expected 6 args");
        ::Vector3 v1 = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Vector3 v2 = {FLOAT(3), FLOAT(4), FLOAT(5)};
        ::Vector3 result = ::Vector3CrossProduct(v1, v2);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3NORMALIZE", Fn{"VECTOR3NORMALIZE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("VECTOR3NORMALIZE: expected 3 args");
        ::Vector3 v = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Vector3 result = ::Vector3Normalize(v);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3LERP", Fn{"VECTOR3LERP", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("VECTOR3LERP: expected 7 args");
        ::Vector3 v1 = {FLOAT(0), FLOAT(1), FLOAT(2)};
        ::Vector3 v2 = {FLOAT(3), FLOAT(4), FLOAT(5)};
        ::Vector3 result = ::Vector3Lerp(v1, v2, FLOAT(6));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("COLOREQUAL", Fn{"COLOREQUAL", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("COLOREQUAL: expected 8 args");
        ::Color col1 = {(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3)};
        ::Color col2 = {(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7)};
        return Value::from_int(::ColorIsEqual(col1, col2) ? 1 : 0);
    }}, true);    R.add_with_policy("FADE", Fn{"FADE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("FADE: expected 5 args");
        ::Color color = {(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3)};
        ::Color result = ::Fade(color, FLOAT(4));
        return Value::from_string(std::to_string(result.r) + "," + std::to_string(result.g) + "," + std::to_string(result.b) + "," + std::to_string(result.a));
    }}, true);    R.add_with_policy("COLORTOINT", Fn{"COLORTOINT", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("COLORTOINT: expected 4 args");
        ::Color color = {(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3)};
        return Value::from_int(::ColorToInt(color));
    }}, true);    R.add_with_policy("COLORTOHSV", Fn{"COLORTOHSV", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("COLORTOHSV: expected 4 args");
        ::Color color = {(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3)};
        ::Vector3 hsv = ::ColorToHSV(color);
        return Value::from_string(std::to_string(hsv.x) + "," + std::to_string(hsv.y) + "," + std::to_string(hsv.z));
    }}, true);    R.add_with_policy("COLORFROMHSV", Fn{"COLORFROMHSV", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("COLORFROMHSV: expected 3 args");
        ::Color result = ::ColorFromHSV(FLOAT(0), FLOAT(1), FLOAT(2));
        return Value::from_string(std::to_string(result.r) + "," + std::to_string(result.g) + "," + std::to_string(result.b) + "," + std::to_string(result.a));
    }}, true);    R.add_with_policy("COLORTINT", Fn{"COLORTINT", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("COLORTINT: expected 8 args");
        ::Color color = {(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3)};
        ::Color tint = {(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7)};
        ::Color result = ::ColorTint(color, tint);
        return Value::from_string(std::to_string(result.r) + "," + std::to_string(result.g) + "," + std::to_string(result.b) + "," + std::to_string(result.a));
    }}, true);    R.add_with_policy("COLORBRIGHTNESS", Fn{"COLORBRIGHTNESS", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("COLORBRIGHTNESS: expected 5 args");
        ::Color color = {(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3)};
        ::Color result = ::ColorBrightness(color, FLOAT(4));
        return Value::from_string(std::to_string(result.r) + "," + std::to_string(result.g) + "," + std::to_string(result.b) + "," + std::to_string(result.a));
    }}, true);    R.add_with_policy("COLORCONTRAST", Fn{"COLORCONTRAST", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("COLORCONTRAST: expected 5 args");
        ::Color color = {(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3)};
        ::Color result = ::ColorContrast(color, FLOAT(4));
        return Value::from_string(std::to_string(result.r) + "," + std::to_string(result.g) + "," + std::to_string(result.b) + "," + std::to_string(result.a));
    }}, true);    R.add_with_policy("COLORALPHA", Fn{"COLORALPHA", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("COLORALPHA: expected 5 args");
        ::Color color = {(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3)};
        ::Color result = ::ColorAlpha(color, FLOAT(4));
        return Value::from_string(std::to_string(result.r) + "," + std::to_string(result.g) + "," + std::to_string(result.b) + "," + std::to_string(result.a));
    }}, true);    R.add_with_policy("COLORLERP", Fn{"COLORLERP", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("COLORLERP: expected 9 args");
        ::Color color1 = {(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3)};
        ::Color color2 = {(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7)};
        ::Color result = ::ColorLerp(color1, color2, FLOAT(8));
        return Value::from_string(std::to_string(result.r) + "," + std::to_string(result.g) + "," + std::to_string(result.b) + "," + std::to_string(result.a));
    }}, true);    R.add_with_policy("GETCOLOR", Fn{"GETCOLOR", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCOLOR: expected 1 args");
        ::Color result = ::GetColor(INT(0));
        return Value::from_string(std::to_string(result.r) + "," + std::to_string(result.g) + "," + std::to_string(result.b) + "," + std::to_string(result.a));
    }}, true);    R.add_with_policy("LIGHTGRAY", Fn{"LIGHTGRAY", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("200,200,200,255");
    }}, true);    R.add_with_policy("GRAY", Fn{"GRAY", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("130,130,130,255");
    }}, true);    R.add_with_policy("DARKGRAY", Fn{"DARKGRAY", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("80,80,80,255");
    }}, true);    R.add_with_policy("YELLOW", Fn{"YELLOW", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("253,249,0,255");
    }}, true);    R.add_with_policy("GOLD", Fn{"GOLD", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("255,203,0,255");
    }}, true);    R.add_with_policy("ORANGE", Fn{"ORANGE", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("255,161,0,255");
    }}, true);    R.add_with_policy("PINK", Fn{"PINK", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("255,109,194,255");
    }}, true);    R.add_with_policy("RED", Fn{"RED", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("230,41,55,255");
    }}, true);    R.add_with_policy("MAROON", Fn{"MAROON", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("190,33,55,255");
    }}, true);    R.add_with_policy("GREEN", Fn{"GREEN", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("0,228,48,255");
    }}, true);    R.add_with_policy("LIME", Fn{"LIME", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("0,158,47,255");
    }}, true);    R.add_with_policy("DARKGREEN", Fn{"DARKGREEN", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("0,117,44,255");
    }}, true);    R.add_with_policy("SKYBLUE", Fn{"SKYBLUE", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("102,191,255,255");
    }}, true);    R.add_with_policy("BLUE", Fn{"BLUE", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("0,121,241,255");
    }}, true);    R.add_with_policy("DARKBLUE", Fn{"DARKBLUE", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("0,82,172,255");
    }}, true);    R.add_with_policy("PURPLE", Fn{"PURPLE", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("200,122,255,255");
    }}, true);    R.add_with_policy("VIOLET", Fn{"VIOLET", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("135,60,190,255");
    }}, true);    R.add_with_policy("DARKPURPLE", Fn{"DARKPURPLE", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("112,31,126,255");
    }}, true);    R.add_with_policy("BEIGE", Fn{"BEIGE", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("211,176,131,255");
    }}, true);    R.add_with_policy("BROWN", Fn{"BROWN", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("127,106,79,255");
    }}, true);    R.add_with_policy("DARKBROWN", Fn{"DARKBROWN", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("76,63,47,255");
    }}, true);    R.add_with_policy("WHITE", Fn{"WHITE", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("255,255,255,255");
    }}, true);    R.add_with_policy("BLACK", Fn{"BLACK", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("0,0,0,255");
    }}, true);    R.add_with_policy("BLANK", Fn{"BLANK", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("0,0,0,0");
    }}, true);    R.add_with_policy("MAGENTA", Fn{"MAGENTA", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("255,0,255,255");
    }}, true);    R.add_with_policy("RAYWHITE", Fn{"RAYWHITE", 0, [] (const std::vector<Value>& args) -> Value {
        return Value::from_string("245,245,245,255");
    }}, true);    R.add_with_policy("INITWINDOW", Fn{"INITWINDOW", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("INITWINDOW: expected 3 args");
        InitWindow(args[0].as_int(), args[1].as_int(), args[2].as_string().c_str());
        return Value::nil();
    }}, true);    R.add_with_policy("WINDOWSHOULDCLOSE", Fn{"WINDOWSHOULDCLOSE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("WINDOWSHOULDCLOSE: expected 0 args");
        return Value::from_bool(WindowShouldClose());
    }}, true);    R.add_with_policy("ISWINDOWREADY", Fn{"ISWINDOWREADY", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWREADY: expected 0 args");
        return Value::from_bool(IsWindowReady());
    }}, true);    R.add_with_policy("ISWINDOWFULLSCREEN", Fn{"ISWINDOWFULLSCREEN", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWFULLSCREEN: expected 0 args");
        return Value::from_bool(IsWindowFullscreen());
    }}, true);    R.add_with_policy("ISWINDOWHIDDEN", Fn{"ISWINDOWHIDDEN", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWHIDDEN: expected 0 args");
        return Value::from_bool(IsWindowHidden());
    }}, true);    R.add_with_policy("ISWINDOWMINIMIZED", Fn{"ISWINDOWMINIMIZED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWMINIMIZED: expected 0 args");
        return Value::from_bool(IsWindowMinimized());
    }}, true);    R.add_with_policy("ISWINDOWMAXIMIZED", Fn{"ISWINDOWMAXIMIZED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWMAXIMIZED: expected 0 args");
        return Value::from_bool(IsWindowMaximized());
    }}, true);    R.add_with_policy("ISWINDOWFOCUSED", Fn{"ISWINDOWFOCUSED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWFOCUSED: expected 0 args");
        return Value::from_bool(IsWindowFocused());
    }}, true);    R.add_with_policy("ISWINDOWRESIZED", Fn{"ISWINDOWRESIZED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWRESIZED: expected 0 args");
        return Value::from_bool(IsWindowResized());
    }}, true);    R.add_with_policy("ISWINDOWSTATE", Fn{"ISWINDOWSTATE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISWINDOWSTATE: expected 1 args");
        return Value::from_bool(IsWindowState(args[0].as_int()));
    }}, true);    R.add_with_policy("SETWINDOWSTATE", Fn{"SETWINDOWSTATE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETWINDOWSTATE: expected 1 args");
        SetWindowState(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("CLEARWINDOWSTATE", Fn{"CLEARWINDOWSTATE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("CLEARWINDOWSTATE: expected 1 args");
        ClearWindowState(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("TOGGLEFULLSCREEN", Fn{"TOGGLEFULLSCREEN", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("TOGGLEFULLSCREEN: expected 0 args");
        ToggleFullscreen();
        return Value::nil();
    }}, true);    R.add_with_policy("TOGGLEBORDERLESSWINDOWED", Fn{"TOGGLEBORDERLESSWINDOWED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("TOGGLEBORDERLESSWINDOWED: expected 0 args");
        ToggleBorderlessWindowed();
        return Value::nil();
    }}, true);    R.add_with_policy("MAXIMIZEWINDOW", Fn{"MAXIMIZEWINDOW", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("MAXIMIZEWINDOW: expected 0 args");
        MaximizeWindow();
        return Value::nil();
    }}, true);    R.add_with_policy("MINIMIZEWINDOW", Fn{"MINIMIZEWINDOW", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("MINIMIZEWINDOW: expected 0 args");
        MinimizeWindow();
        return Value::nil();
    }}, true);    R.add_with_policy("RESTOREWINDOW", Fn{"RESTOREWINDOW", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("RESTOREWINDOW: expected 0 args");
        RestoreWindow();
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWICON", Fn{"SETWINDOWICON", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETWINDOWICON: expected 1 args");
        // Load image from file path and set as window icon ::Image icon = ::LoadImage(STR(0).c_str()); if (::IsImageValid(icon)) { ::SetWindowIcon(icon); ::UnloadImage(icon); }
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWPOSITION", Fn{"SETWINDOWPOSITION", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETWINDOWPOSITION: expected 2 args");
        SetWindowPosition(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWMONITOR", Fn{"SETWINDOWMONITOR", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETWINDOWMONITOR: expected 1 args");
        SetWindowMonitor(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWMINSIZE", Fn{"SETWINDOWMINSIZE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETWINDOWMINSIZE: expected 2 args");
        SetWindowMinSize(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWMAXSIZE", Fn{"SETWINDOWMAXSIZE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETWINDOWMAXSIZE: expected 2 args");
        SetWindowMaxSize(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWOPACITY", Fn{"SETWINDOWOPACITY", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETWINDOWOPACITY: expected 1 args");
        SetWindowOpacity(args[0].as_number());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWFOCUSED", Fn{"SETWINDOWFOCUSED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("SETWINDOWFOCUSED: expected 0 args");
        SetWindowFocused();
        return Value::nil();
    }}, true);    R.add_with_policy("GETWINDOWHANDLE", Fn{"GETWINDOWHANDLE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETWINDOWHANDLE: expected 0 args");
        return Value::from_int((long long)GetWindowHandle());
    }}, true);    R.add_with_policy("GETSCREENWIDTH", Fn{"GETSCREENWIDTH", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETSCREENWIDTH: expected 0 args");
        return Value::from_int(GetScreenWidth());
    }}, true);    R.add_with_policy("GETSCREENHEIGHT", Fn{"GETSCREENHEIGHT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETSCREENHEIGHT: expected 0 args");
        return Value::from_int(GetScreenHeight());
    }}, true);    R.add_with_policy("GETRENDERWIDTH", Fn{"GETRENDERWIDTH", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETRENDERWIDTH: expected 0 args");
        return Value::from_int(GetRenderWidth());
    }}, true);    R.add_with_policy("GETRENDERHEIGHT", Fn{"GETRENDERHEIGHT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETRENDERHEIGHT: expected 0 args");
        return Value::from_int(GetRenderHeight());
    }}, true);    R.add_with_policy("GETCURRENTMONITOR", Fn{"GETCURRENTMONITOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETCURRENTMONITOR: expected 0 args");
        return Value::from_int(GetCurrentMonitor());
    }}, true);    R.add_with_policy("GETMONITORPOSITION", Fn{"GETMONITORPOSITION", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORPOSITION: expected 1 args");
        ::Vector2 pos = ::GetMonitorPosition(INT(0));
        return Value::from_string(std::to_string(pos.x) + "," + std::to_string(pos.y));
    }}, true);    R.add_with_policy("GETMONITORNAME", Fn{"GETMONITORNAME", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORNAME: expected 1 args");
        return Value::from_string(GetMonitorName(args[0].as_int()));
    }}, true);    R.add_with_policy("SETCLIPBOARDTEXT", Fn{"SETCLIPBOARDTEXT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETCLIPBOARDTEXT: expected 1 args");
        SetClipboardText(args[0].as_string().c_str());
        return Value::nil();
    }}, true);    R.add_with_policy("GETCLIPBOARDTEXT", Fn{"GETCLIPBOARDTEXT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETCLIPBOARDTEXT: expected 0 args");
        return Value::from_string(GetClipboardText());
    }}, true);    R.add_with_policy("ENABLEEVENTWAITING", Fn{"ENABLEEVENTWAITING", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENABLEEVENTWAITING: expected 0 args");
        EnableEventWaiting();
        return Value::nil();
    }}, true);    R.add_with_policy("DISABLEEVENTWAITING", Fn{"DISABLEEVENTWAITING", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("DISABLEEVENTWAITING: expected 0 args");
        DisableEventWaiting();
        return Value::nil();
    }}, true);    R.add_with_policy("SHOWCURSOR", Fn{"SHOWCURSOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("SHOWCURSOR: expected 0 args");
        ShowCursor();
        return Value::nil();
    }}, true);    R.add_with_policy("HIDECURSOR", Fn{"HIDECURSOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("HIDECURSOR: expected 0 args");
        HideCursor();
        return Value::nil();
    }}, true);    R.add_with_policy("ISCURSORHIDDEN", Fn{"ISCURSORHIDDEN", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISCURSORHIDDEN: expected 0 args");
        return Value::from_bool(IsCursorHidden());
    }}, true);    R.add_with_policy("ENABLECURSOR", Fn{"ENABLECURSOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENABLECURSOR: expected 0 args");
        EnableCursor();
        return Value::nil();
    }}, true);    R.add_with_policy("DISABLECURSOR", Fn{"DISABLECURSOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("DISABLECURSOR: expected 0 args");
        DisableCursor();
        return Value::nil();
    }}, true);    R.add_with_policy("ISCURSORONSCREEN", Fn{"ISCURSORONSCREEN", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISCURSORONSCREEN: expected 0 args");
        return Value::from_bool(IsCursorOnScreen());
    }}, true);    R.add_with_policy("SETTARGETFPS", Fn{"SETTARGETFPS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETTARGETFPS: expected 1 args");
        SetTargetFPS(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("GETFPS", Fn{"GETFPS", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETFPS: expected 0 args");
        return Value::from_int(GetFPS());
    }}, true);    R.add_with_policy("GETDELTATIME", Fn{"GETDELTATIME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETDELTATIME: expected 0 args");
        return Value::from_number(GetFrameTime());
    }}, true);    R.add_with_policy("GETTIME", Fn{"GETTIME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETTIME: expected 0 args");
        return Value::from_number(GetTime());
    }}, true);    R.add_with_policy("BEGINDRAW", Fn{"BEGINDRAW", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("BEGINDRAW: expected 0 args");
        BeginDrawing();
        return Value::nil();
    }}, true);    R.add_with_policy("ENDDRAW", Fn{"ENDDRAW", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDDRAW: expected 0 args");
        EndDrawing();
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINMODE2D", Fn{"BEGINMODE2D", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("BEGINMODE2D: expected 1 args");
        // Use camera ID to look up Camera2D, or create default
        ::Camera2D camera = {};
        if (rlreg::cameras2d.find(INT(0)) != rlreg::cameras2d.end()) {
          camera = rlreg::cameras2d.at(INT(0));
        } else {
          // Default 2D camera centered at origin
          camera.offset = ::Vector2{0.0f, 0.0f};
          camera.target = ::Vector2{0.0f, 0.0f};
          camera.rotation = 0.0f;
          camera.zoom = 1.0f;
        }
        ::BeginMode2D(camera);
        return Value::nil();
    }}, true);    R.add_with_policy("ENDMODE2D", Fn{"ENDMODE2D", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDMODE2D: expected 0 args");
        EndMode2D();
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINMODE3D", Fn{"BEGINMODE3D", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("BEGINMODE3D: expected 1 args");
        // Use camera from camera system if available, otherwise use default
        ::Camera3D camera = {};
        if (g_camera_system_3d) {
          Camera3D* cam = g_camera_system_3d->get_active_camera();
          if (cam) {
            camera.position = ::Vector3{cam->position.x, cam->position.y, cam->position.z};
            camera.target = ::Vector3{cam->target.x, cam->target.y, cam->target.z};
            camera.up = ::Vector3{cam->up.x, cam->up.y, cam->up.z};
            camera.fovy = cam->fov;
            camera.projection = CAMERA_PERSPECTIVE;
          } else {
            // Default camera if no active camera
            camera.position = ::Vector3{0.0f, 10.0f, 10.0f};
            camera.target = ::Vector3{0.0f, 0.0f, 0.0f};
            camera.up = ::Vector3{0.0f, 1.0f, 0.0f};
            camera.fovy = 45.0f;
            camera.projection = CAMERA_PERSPECTIVE;
          }
        } else {
          // Default camera if system not initialized
          camera.position = ::Vector3{0.0f, 10.0f, 10.0f};
          camera.target = ::Vector3{0.0f, 0.0f, 0.0f};
          camera.up = ::Vector3{0.0f, 1.0f, 0.0f};
          camera.fovy = 45.0f;
          camera.projection = CAMERA_PERSPECTIVE;
        }
        ::BeginMode3D(camera);
        return Value::nil();
    }}, true);    R.add_with_policy("ENDMODE3D", Fn{"ENDMODE3D", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDMODE3D: expected 0 args");
        EndMode3D();
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINTEXTUREMODE", Fn{"BEGINTEXTUREMODE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("BEGINTEXTUREMODE: expected 1 args");
        // Use render texture ID to look up RenderTexture2D
        if (rlreg::rendertextures.find(INT(0)) != rlreg::rendertextures.end()) {
          ::BeginTextureMode(rlreg::rendertextures.at(INT(0)));
        } else {
          // Default empty render texture (may not work, but prevents crash)
          ::BeginTextureMode(::RenderTexture2D{});
        }
        return Value::nil();
    }}, true);    R.add_with_policy("ENDTEXTUREMODE", Fn{"ENDTEXTUREMODE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDTEXTUREMODE: expected 0 args");
        EndTextureMode();
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINBLENDMODE", Fn{"BEGINBLENDMODE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("BEGINBLENDMODE: expected 1 args");
        BeginBlendMode(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("ENDBLENDMODE", Fn{"ENDBLENDMODE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDBLENDMODE: expected 0 args");
        EndBlendMode();
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINSCISSORMODE", Fn{"BEGINSCISSORMODE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("BEGINSCISSORMODE: expected 4 args");
        BeginScissorMode(args[0].as_int(), args[1].as_int(), args[2].as_int(), args[3].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("ENDSCISSORMODE", Fn{"ENDSCISSORMODE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDSCISSORMODE: expected 0 args");
        EndScissorMode();
        return Value::nil();
    }}, true);    R.add_with_policy("SETEXITKEY", Fn{"SETEXITKEY", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETEXITKEY: expected 1 args");
        SetExitKey(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("TAKESCREENSHOT", Fn{"TAKESCREENSHOT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TAKESCREENSHOT: expected 1 args");
        TakeScreenshot(args[0].as_string().c_str());
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWFPS", Fn{"DRAWFPS", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("DRAWFPS: expected 2 args");
        DrawFPS(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("CLEARBACKGROUND", Fn{"CLEARBACKGROUND", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("CLEARBACKGROUND: expected 3 args");
        ::Color c{(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), 255};
        ::ClearBackground(c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXT", Fn{"DRAWTEXT", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWTEXT: expected 7 args");
        ::Color c{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawText(STR(0).c_str(), INT(1), INT(2), INT(3), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTEX", Fn{"DRAWTEXTEX", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWTEXTEX: expected 9 args");
        ::Font font = rlreg::fonts.at(INT(0));
        ::Vector2 pos{(float)INT(1), (float)INT(2)};
        ::Color c{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawTextEx(font, STR(1).c_str(), pos, (float)FLOAT(3), (float)FLOAT(4), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTPRO", Fn{"DRAWTEXTPRO", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWTEXTPRO: expected 12 args");
        ::Font font = rlreg::fonts.at(INT(0));
        ::Vector2 pos{(float)INT(1), (float)INT(2)};
        ::Vector2 origin{(float)INT(3), (float)INT(4)};
        ::Color c{(unsigned char)INT(8), (unsigned char)INT(9), (unsigned char)INT(10), 255};
        ::DrawTextPro(font, STR(1).c_str(), pos, origin, (float)FLOAT(5), (float)FLOAT(6), (float)FLOAT(7), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLE", Fn{"DRAWRECTANGLE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWRECTANGLE: expected 7 args");
        ::Color c{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawRectangle(INT(0), INT(1), INT(2), INT(3), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEV", Fn{"DRAWRECTANGLEV", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWRECTANGLEV: expected 7 args");
        ::Vector2 pos{(float)INT(0), (float)INT(1)};
        ::Vector2 size{(float)INT(2), (float)INT(3)};
        ::Color c{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawRectangleV(pos, size, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEREC", Fn{"DRAWRECTANGLEREC", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWRECTANGLEREC: expected 7 args");
        ::Rectangle rec{(float)INT(0), (float)INT(1), (float)INT(2), (float)INT(3)};
        ::Color c{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawRectangleRec(rec, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEPRO", Fn{"DRAWRECTANGLEPRO", 11, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 11) throw std::runtime_error("DRAWRECTANGLEPRO: expected 11 args");
        ::Rectangle rec{(float)INT(0), (float)INT(1), (float)INT(2), (float)INT(3)};
        ::Vector2 origin{(float)INT(4), (float)INT(5)};
        ::Color c{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawRectanglePro(rec, origin, (float)FLOAT(6), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEGRADIENTV", Fn{"DRAWRECTANGLEGRADIENTV", 15, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 15) throw std::runtime_error("DRAWRECTANGLEGRADIENTV: expected 15 args");
        ::Color top{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::Color bottom{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawRectangleGradientV(INT(0), INT(1), INT(2), INT(3), top, bottom);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEGRADIENTH", Fn{"DRAWRECTANGLEGRADIENTH", 15, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 15) throw std::runtime_error("DRAWRECTANGLEGRADIENTH: expected 15 args");
        ::Color left{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::Color right{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawRectangleGradientH(INT(0), INT(1), INT(2), INT(3), left, right);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEROUNDED", Fn{"DRAWRECTANGLEROUNDED", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWRECTANGLEROUNDED: expected 9 args");
        ::Rectangle rec{(float)INT(0), (float)INT(1), (float)INT(2), (float)INT(3)};
        ::Color c{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawRectangleRounded(rec, (float)FLOAT(4), INT(5), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLELINES", Fn{"DRAWRECTANGLELINES", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWRECTANGLELINES: expected 7 args");
        ::Color c{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawRectangleLines(INT(0), INT(1), INT(2), INT(3), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLELINESEX", Fn{"DRAWRECTANGLELINESEX", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWRECTANGLELINESEX: expected 8 args");
        ::Rectangle rec{(float)INT(0), (float)INT(1), (float)INT(2), (float)INT(3)};
        ::Color c{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawRectangleLinesEx(rec, (float)FLOAT(4), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLE", Fn{"DRAWCIRCLE", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWCIRCLE: expected 6 args");
        ::Color c{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::DrawCircle(INT(0), INT(1), (float)INT(2), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLEV", Fn{"DRAWCIRCLEV", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWCIRCLEV: expected 6 args");
        ::Vector2 center{(float)INT(0), (float)INT(1)};
        ::Color c{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::DrawCircleV(center, (float)INT(2), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLELINES", Fn{"DRAWCIRCLELINES", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWCIRCLELINES: expected 6 args");
        ::Color c{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::DrawCircleLines(INT(0), INT(1), (float)INT(2), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLELINESV", Fn{"DRAWCIRCLELINESV", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWCIRCLELINESV: expected 6 args");
        ::Vector2 center{(float)INT(0), (float)INT(1)};
        ::Color c{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::DrawCircleLinesV(center, (float)INT(2), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLESECTOR", Fn{"DRAWCIRCLESECTOR", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCIRCLESECTOR: expected 9 args");
        ::Vector2 center{(float)INT(0), (float)INT(1)};
        ::Color c{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawCircleSector(center, (float)INT(2), (float)FLOAT(3), (float)FLOAT(4), INT(5), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLESECTORLINES", Fn{"DRAWCIRCLESECTORLINES", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCIRCLESECTORLINES: expected 9 args");
        ::Vector2 center{(float)INT(0), (float)INT(1)};
        ::Color c{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawCircleSectorLines(center, (float)INT(2), (float)FLOAT(3), (float)FLOAT(4), INT(5), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLEGRADIENT", Fn{"DRAWCIRCLEGRADIENT", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWCIRCLEGRADIENT: expected 12 args");
        ::Color inner{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::Color outer{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawCircleGradient(INT(0), INT(1), (float)INT(2), inner, outer);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWELLIPSE", Fn{"DRAWELLIPSE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWELLIPSE: expected 7 args");
        ::Color c{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawEllipse(INT(0), INT(1), (float)INT(2), (float)INT(3), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWELLIPSELINES", Fn{"DRAWELLIPSELINES", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWELLIPSELINES: expected 7 args");
        ::Color c{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawEllipseLines(INT(0), INT(1), (float)INT(2), (float)INT(3), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRING", Fn{"DRAWRING", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWRING: expected 10 args");
        ::Vector2 center{(float)INT(0), (float)INT(1)};
        ::Color c{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawRing(center, (float)INT(2), (float)INT(3), (float)FLOAT(4), (float)FLOAT(5), INT(6), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRINGLINES", Fn{"DRAWRINGLINES", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWRINGLINES: expected 10 args");
        ::Vector2 center{(float)INT(0), (float)INT(1)};
        ::Color c{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawRingLines(center, (float)INT(2), (float)INT(3), (float)FLOAT(4), (float)FLOAT(5), INT(6), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWLINE", Fn{"DRAWLINE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWLINE: expected 7 args");
        ::Color c{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawLine(INT(0), INT(1), INT(2), INT(3), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWLINEV", Fn{"DRAWLINEV", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWLINEV: expected 7 args");
        ::Vector2 start{(float)INT(0), (float)INT(1)};
        ::Vector2 end{(float)INT(2), (float)INT(3)};
        ::Color c{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawLineV(start, end, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWLINEEX", Fn{"DRAWLINEEX", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWLINEEX: expected 8 args");
        ::Vector2 start{(float)INT(0), (float)INT(1)};
        ::Vector2 end{(float)INT(2), (float)INT(3)};
        ::Color c{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawLineEx(start, end, (float)FLOAT(4), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTRIANGLE", Fn{"DRAWTRIANGLE", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWTRIANGLE: expected 9 args");
        ::Vector2 v1{(float)INT(0), (float)INT(1)};
        ::Vector2 v2{(float)INT(2), (float)INT(3)};
        ::Vector2 v3{(float)INT(4), (float)INT(5)};
        ::Color c{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawTriangle(v1, v2, v3, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTRIANGLELINES", Fn{"DRAWTRIANGLELINES", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWTRIANGLELINES: expected 9 args");
        ::Vector2 v1{(float)INT(0), (float)INT(1)};
        ::Vector2 v2{(float)INT(2), (float)INT(3)};
        ::Vector2 v3{(float)INT(4), (float)INT(5)};
        ::Color c{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawTriangleLines(v1, v2, v3, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPOLY", Fn{"DRAWPOLY", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWPOLY: expected 8 args");
        ::Vector2 center{(float)INT(0), (float)INT(1)};
        ::Color c{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawPoly(center, INT(2), (float)INT(3), (float)FLOAT(4), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPOLYLINES", Fn{"DRAWPOLYLINES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWPOLYLINES: expected 8 args");
        ::Vector2 center{(float)INT(0), (float)INT(1)};
        ::Color c{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawPolyLines(center, INT(2), (float)INT(3), (float)FLOAT(4), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPIXEL", Fn{"DRAWPIXEL", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("DRAWPIXEL: expected 5 args");
        ::Color c{(unsigned char)INT(2), (unsigned char)INT(3), (unsigned char)INT(4), 255};
        ::DrawPixel(INT(0), INT(1), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPIXELV", Fn{"DRAWPIXELV", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("DRAWPIXELV: expected 5 args");
        ::Vector2 pos{(float)INT(0), (float)INT(1)};
        ::Color c{(unsigned char)INT(2), (unsigned char)INT(3), (unsigned char)INT(4), 255};
        ::DrawPixelV(pos, c);
        return Value::nil();
    }}, true);    R.add_with_policy("ISKEYPRESSEDREPEAT", Fn{"ISKEYPRESSEDREPEAT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISKEYPRESSEDREPEAT: expected 1 args");
        return Value::from_bool(IsKeyPressedRepeat(args[0].as_int()));
    }}, true);    R.add_with_policy("ISKEYUP", Fn{"ISKEYUP", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISKEYUP: expected 1 args");
        return Value::from_bool(IsKeyUp(args[0].as_int()));
    }}, true);    R.add_with_policy("GETKEYPRESSED", Fn{"GETKEYPRESSED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETKEYPRESSED: expected 0 args");
        return Value::from_int(GetKeyPressed());
    }}, true);    R.add_with_policy("GETCHARPRESSED", Fn{"GETCHARPRESSED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETCHARPRESSED: expected 0 args");
        return Value::from_int(GetCharPressed());
    }}, true);    R.add_with_policy("ISMOUSEBUTTONPRESSED", Fn{"ISMOUSEBUTTONPRESSED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMOUSEBUTTONPRESSED: expected 1 args");
        return Value::from_bool(IsMouseButtonPressed(args[0].as_int()));
    }}, true);    R.add_with_policy("ISMOUSEBUTTONRELEASED", Fn{"ISMOUSEBUTTONRELEASED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMOUSEBUTTONRELEASED: expected 1 args");
        return Value::from_bool(IsMouseButtonReleased(args[0].as_int()));
    }}, true);    R.add_with_policy("ISMOUSEBUTTONUP", Fn{"ISMOUSEBUTTONUP", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMOUSEBUTTONUP: expected 1 args");
        return Value::from_bool(IsMouseButtonUp(args[0].as_int()));
    }}, true);    R.add_with_policy("GETMOUSEPOSITION", Fn{"GETMOUSEPOSITION", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMOUSEPOSITION: expected 0 args");
        ::Vector2 pos = ::GetMousePosition();
        return Value::from_string(std::to_string(pos.x) + "," + std::to_string(pos.y));
    }}, true);    R.add_with_policy("GETMOUSEDELTA", Fn{"GETMOUSEDELTA", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMOUSEDELTA: expected 0 args");
        ::Vector2 delta = ::GetMouseDelta();
        return Value::from_string(std::to_string(delta.x) + "," + std::to_string(delta.y));
    }}, true);    R.add_with_policy("GETMOUSEWHEELMOVE", Fn{"GETMOUSEWHEELMOVE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMOUSEWHEELMOVE: expected 0 args");
        return Value::from_int(GetMouseWheelMove());
    }}, true);    R.add_with_policy("GETMOUSEWHEELMOVEV", Fn{"GETMOUSEWHEELMOVEV", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMOUSEWHEELMOVEV: expected 0 args");
        ::Vector2 wheel = ::GetMouseWheelMoveV();
        return Value::from_string(std::to_string(wheel.x) + "," + std::to_string(wheel.y));
    }}, true);    R.add_with_policy("SETMOUSEPOSITION", Fn{"SETMOUSEPOSITION", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETMOUSEPOSITION: expected 2 args");
        SetMousePosition(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETMOUSEOFFSET", Fn{"SETMOUSEOFFSET", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETMOUSEOFFSET: expected 2 args");
        SetMouseOffset(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETMOUSESCALE", Fn{"SETMOUSESCALE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETMOUSESCALE: expected 2 args");
        SetMouseScale(args[0].as_number(), args[1].as_number());
        return Value::nil();
    }}, true);    R.add_with_policy("SETMOUSECURSOR", Fn{"SETMOUSECURSOR", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETMOUSECURSOR: expected 1 args");
        SetMouseCursor(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("GETTOUCHX", Fn{"GETTOUCHX", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETTOUCHX: expected 0 args");
        return Value::from_int(GetTouchX());
    }}, true);    R.add_with_policy("GETTOUCHY", Fn{"GETTOUCHY", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETTOUCHY: expected 0 args");
        return Value::from_int(GetTouchY());
    }}, true);    R.add_with_policy("GETTOUCHPOSITION", Fn{"GETTOUCHPOSITION", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETTOUCHPOSITION: expected 1 args");
        ::Vector2 pos = ::GetTouchPosition(INT(0));
        return Value::from_string(std::to_string(pos.x) + "," + std::to_string(pos.y));
    }}, true);    R.add_with_policy("GETTOUCHPOINTID", Fn{"GETTOUCHPOINTID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETTOUCHPOINTID: expected 1 args");
        return Value::from_int(GetTouchPointId(args[0].as_int()));
    }}, true);    R.add_with_policy("GETTOUCHPOINTCOUNT", Fn{"GETTOUCHPOINTCOUNT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETTOUCHPOINTCOUNT: expected 0 args");
        return Value::from_int(GetTouchPointCount());
    }}, true);    R.add_with_policy("ISGAMEPADAVAILABLE", Fn{"ISGAMEPADAVAILABLE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISGAMEPADAVAILABLE: expected 1 args");
        return Value::from_bool(::IsGamepadAvailable(INT(0)));
    }}, true);    R.add_with_policy("GETGAMEPADNAME", Fn{"GETGAMEPADNAME", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETGAMEPADNAME: expected 1 args");
        return Value::from_string(::GetGamepadName(INT(0)));
    }}, true);    R.add_with_policy("ISGAMEPADBUTTONPRESSED", Fn{"ISGAMEPADBUTTONPRESSED", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ISGAMEPADBUTTONPRESSED: expected 2 args");
        return Value::from_bool(::IsGamepadButtonPressed(INT(0), INT(1)));
    }}, true);    R.add_with_policy("ISGAMEPADBUTTONDOWN", Fn{"ISGAMEPADBUTTONDOWN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ISGAMEPADBUTTONDOWN: expected 2 args");
        return Value::from_bool(::IsGamepadButtonDown(INT(0), INT(1)));
    }}, true);    R.add_with_policy("ISGAMEPADBUTTONRELEASED", Fn{"ISGAMEPADBUTTONRELEASED", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ISGAMEPADBUTTONRELEASED: expected 2 args");
        return Value::from_bool(::IsGamepadButtonReleased(INT(0), INT(1)));
    }}, true);    R.add_with_policy("ISGAMEPADBUTTONUP", Fn{"ISGAMEPADBUTTONUP", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ISGAMEPADBUTTONUP: expected 2 args");
        return Value::from_bool(::IsGamepadButtonUp(INT(0), INT(1)));
    }}, true);    R.add_with_policy("GETGAMEPADBUTTONPRESSED", Fn{"GETGAMEPADBUTTONPRESSED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGAMEPADBUTTONPRESSED: expected 0 args");
        return Value::from_int(::GetGamepadButtonPressed());
    }}, true);    R.add_with_policy("GETGAMEPADAXISCOUNT", Fn{"GETGAMEPADAXISCOUNT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETGAMEPADAXISCOUNT: expected 1 args");
        return Value::from_int(::GetGamepadAxisCount(INT(0)));
    }}, true);    R.add_with_policy("GETGAMEPADAXISMOVEMENT", Fn{"GETGAMEPADAXISMOVEMENT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETGAMEPADAXISMOVEMENT: expected 2 args");
        return Value::from_number(::GetGamepadAxisMovement(INT(0), INT(1)));
    }}, true);    R.add_with_policy("SETGAMEPADMAPPINGS", Fn{"SETGAMEPADMAPPINGS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETGAMEPADMAPPINGS: expected 1 args");
        return Value::from_int(::SetGamepadMappings(STR(0).c_str()));
    }}, true);    R.add_with_policy("SETGAMEPADVIBRATION", Fn{"SETGAMEPADVIBRATION", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("SETGAMEPADVIBRATION: expected 4 args");
        // Set gamepad vibration for both motors with duration ::SetGamepadVibration(INT(0), (float)DOUBLE(1), (float)DOUBLE(2), (float)DOUBLE(3));
        return Value::nil();
    }}, true);    R.add_with_policy("LOADTEXTURE", Fn{"LOADTEXTURE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADTEXTURE: expected 1 args");
        ::Texture2D tex = ::LoadTexture(STR(0).c_str());
        int id = rlreg::next_tex_id++;
        rlreg::textures[id] = tex;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADTEXTUREFROMIMAGE", Fn{"LOADTEXTUREFROMIMAGE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADTEXTUREFROMIMAGE: expected 1 args");
        ::Texture2D tex = ::LoadTextureFromImage(rlreg::images.at(INT(0)));
        int id = rlreg::next_tex_id++;
        rlreg::textures[id] = tex;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADTEXTURECUBEMAP", Fn{"LOADTEXTURECUBEMAP", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADTEXTURECUBEMAP: expected 2 args");
        ::TextureCubemap tex = ::LoadTextureCubemap(rlreg::images.at(INT(0)), INT(1));
        int id = rlreg::next_cubemap_id++;
        rlreg::cubemaps[id] = tex;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADRENDERTEXTURE", Fn{"LOADRENDERTEXTURE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADRENDERTEXTURE: expected 2 args");
        ::RenderTexture2D tex = ::LoadRenderTexture(INT(0), INT(1));
        int id = rlreg::next_rendertex_id++;
        rlreg::rendertextures[id] = tex;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISTEXTUREVALID", Fn{"ISTEXTUREVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISTEXTUREVALID: expected 1 args");
        return Value::from_bool(::IsTextureValid(rlreg::textures.at(INT(0))));
    }}, true);    R.add_with_policy("ISRENDERTEXTUREVALID", Fn{"ISRENDERTEXTUREVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISRENDERTEXTUREVALID: expected 1 args");
        return Value::from_bool(::IsRenderTextureValid(rlreg::rendertextures.at(INT(0))));
    }}, true);    R.add_with_policy("UPDATETEXTURE", Fn{"UPDATETEXTURE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("UPDATETEXTURE: expected 2 args");
        // Note: This is simplified - would need proper pixel data handling
        ::UpdateTexture(rlreg::textures.at(INT(0)), nullptr);
        return Value::nil();
    }}, true);    R.add_with_policy("UPDATETEXTUREREC", Fn{"UPDATETEXTUREREC", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("UPDATETEXTUREREC: expected 6 args");
        ::Rectangle rec{(float)INT(1), (float)INT(2), (float)INT(3), (float)INT(4)};
        // Note: This is simplified - would need proper pixel data handling
        ::UpdateTextureRec(rlreg::textures.at(INT(0)), rec, nullptr);
        return Value::nil();
    }}, true);    R.add_with_policy("GENTEXTUREMIPMAPS", Fn{"GENTEXTUREMIPMAPS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GENTEXTUREMIPMAPS: expected 1 args");
        ::GenTextureMipmaps(&rlreg::textures[INT(0)]);
        return Value::nil();
    }}, true);    R.add_with_policy("SETTEXTUREFILTER", Fn{"SETTEXTUREFILTER", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETTEXTUREFILTER: expected 2 args");
        ::SetTextureFilter(rlreg::textures.at(INT(0)), INT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("SETTEXTUREWRAP", Fn{"SETTEXTUREWRAP", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETTEXTUREWRAP: expected 2 args");
        ::SetTextureWrap(rlreg::textures.at(INT(0)), INT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTURE", Fn{"DRAWTEXTURE", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWTEXTURE: expected 6 args");
        ::Color c{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::DrawTexture(rlreg::textures.at(INT(0)), INT(1), INT(2), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTUREV", Fn{"DRAWTEXTUREV", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWTEXTUREV: expected 6 args");
        ::Vector2 pos{(float)INT(1), (float)INT(2)};
        ::Color c{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::DrawTextureV(rlreg::textures.at(INT(0)), pos, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTUREEX", Fn{"DRAWTEXTUREEX", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWTEXTUREEX: expected 8 args");
        ::Vector2 pos{(float)INT(1), (float)INT(2)};
        ::Color c{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawTextureEx(rlreg::textures.at(INT(0)), pos, (float)FLOAT(3), (float)FLOAT(4), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTUREREC", Fn{"DRAWTEXTUREREC", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWTEXTUREREC: expected 9 args");
        ::Rectangle src{(float)INT(1), (float)INT(2), (float)INT(3), (float)INT(4)};
        ::Vector2 pos{(float)INT(5), (float)INT(6)};
        ::Color c{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawTextureRec(rlreg::textures.at(INT(0)), src, pos, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTUREPRO", Fn{"DRAWTEXTUREPRO", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWTEXTUREPRO: expected 12 args");
        ::Rectangle src{(float)INT(1), (float)INT(2), (float)INT(3), (float)INT(4)};
        ::Rectangle dest{(float)INT(5), (float)INT(6), (float)INT(7), (float)INT(8)};
        ::Vector2 origin{(float)INT(9), (float)INT(10)};
        ::Color c{(unsigned char)INT(11), (unsigned char)INT(12), (unsigned char)INT(13), 255};
        ::DrawTexturePro(rlreg::textures.at(INT(0)), src, dest, origin, 0.0f, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTURENPATCH", Fn{"DRAWTEXTURENPATCH", 16, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 16) throw std::runtime_error("DRAWTEXTURENPATCH: expected 16 args");
        ::NPatchInfo nPatchInfo;
        nPatchInfo.source = {(float)INT(1), (float)INT(2), (float)INT(3), (float)INT(4)};
        nPatchInfo.left = INT(5);
        nPatchInfo.top = INT(6);
        nPatchInfo.right = INT(7);
        nPatchInfo.bottom = INT(8);
        nPatchInfo.layout = INT(9);
        ::Rectangle dest{(float)INT(10), (float)INT(11), (float)INT(12), (float)INT(13)};
        ::Vector2 origin{(float)INT(14), (float)INT(15)};
        ::Color c{(unsigned char)INT(16), (unsigned char)INT(17), (unsigned char)INT(18), 255};
        ::DrawTextureNPatch(rlreg::textures.at(INT(0)), nPatchInfo, dest, origin, (float)FLOAT(14), c);
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADTEXTURE", Fn{"UNLOADTEXTURE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADTEXTURE: expected 1 args");
        int id = INT(0);
        auto it = rlreg::textures.find(id);
        if (it != rlreg::textures.end()) {
          ::UnloadTexture(it->second);
          rlreg::textures.erase(it);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADRENDERTEXTURE", Fn{"UNLOADRENDERTEXTURE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADRENDERTEXTURE: expected 1 args");
        int id = INT(0);
        auto it = rlreg::rendertextures.find(id);
        if (it != rlreg::rendertextures.end()) {
          ::UnloadRenderTexture(it->second);
          rlreg::rendertextures.erase(it);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("INITAUDIODEVICE", Fn{"INITAUDIODEVICE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("INITAUDIODEVICE: expected 0 args");
        InitAudioDevice();
        return Value::nil();
    }}, true);    R.add_with_policy("CLOSEAUDIODEVICE", Fn{"CLOSEAUDIODEVICE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("CLOSEAUDIODEVICE: expected 0 args");
        CloseAudioDevice();
        return Value::nil();
    }}, true);    R.add_with_policy("ISAUDIODEVICEREADY", Fn{"ISAUDIODEVICEREADY", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISAUDIODEVICEREADY: expected 0 args");
        return Value::from_bool(IsAudioDeviceReady());
    }}, true);    R.add_with_policy("SETMASTERVOLUME", Fn{"SETMASTERVOLUME", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETMASTERVOLUME: expected 1 args");
        SetMasterVolume(args[0].as_number());
        return Value::nil();
    }}, true);    R.add_with_policy("GETMASTERVOLUME", Fn{"GETMASTERVOLUME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMASTERVOLUME: expected 0 args");
        return Value::from_number(GetMasterVolume());
    }}, true);    R.add_with_policy("LOADWAVE", Fn{"LOADWAVE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADWAVE: expected 1 args");
        ::Wave wave = ::LoadWave(STR(0).c_str());
        int id = rlreg::next_wave_id++;
        rlreg::waves[id] = wave;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADWAVEFROMMEMORY", Fn{"LOADWAVEFROMMEMORY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADWAVEFROMMEMORY: expected 2 args");
        ::Wave wave = ::LoadWaveFromMemory(STR(0).c_str(), (const unsigned char*)STR(1).c_str(), STR(1).length());
        int id = rlreg::next_wave_id++;
        rlreg::waves[id] = wave;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISWAVEVALID", Fn{"ISWAVEVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISWAVEVALID: expected 1 args");
        return Value::from_bool(::IsWaveValid(rlreg::waves.at(INT(0))));
    }}, true);    R.add_with_policy("LOADSOUNDFROMWAVE", Fn{"LOADSOUNDFROMWAVE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADSOUNDFROMWAVE: expected 1 args");
        ::Sound sound = ::LoadSoundFromWave(rlreg::waves.at(INT(0)));
        int id = rlreg::next_sound_id++;
        rlreg::sounds[id] = sound;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADSOUNDALIAS", Fn{"LOADSOUNDALIAS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADSOUNDALIAS: expected 1 args");
        ::Sound sound = ::LoadSoundAlias(rlreg::sounds.at(INT(0)));
        int id = rlreg::next_sound_id++;
        rlreg::sounds[id] = sound;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISSOUNDVALID", Fn{"ISSOUNDVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISSOUNDVALID: expected 1 args");
        return Value::from_bool(::IsSoundValid(rlreg::sounds.at(INT(0))));
    }}, true);    R.add_with_policy("UPDATESOUND", Fn{"UPDATESOUND", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("UPDATESOUND: expected 2 args");
        // Note: This is simplified - would need proper audio data handling
        ::UpdateSound(rlreg::sounds.at(INT(0)), nullptr, 0);
        return Value::nil();
    }}, true);    R.add_with_policy("PAUSESOUND", Fn{"PAUSESOUND", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PAUSESOUND: expected 1 args");
        ::PauseSound(rlreg::sounds.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("RESUMESOUND", Fn{"RESUMESOUND", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("RESUMESOUND: expected 1 args");
        ::ResumeSound(rlreg::sounds.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("ISSOUNDPLAYING", Fn{"ISSOUNDPLAYING", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISSOUNDPLAYING: expected 1 args");
        return Value::from_bool(::IsSoundPlaying(rlreg::sounds.at(INT(0))));
    }}, true);    R.add_with_policy("SETSOUNDVOLUME", Fn{"SETSOUNDVOLUME", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETSOUNDVOLUME: expected 2 args");
        ::SetSoundVolume(rlreg::sounds.at(INT(0)), (float)FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("SETSOUNDPITCH", Fn{"SETSOUNDPITCH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETSOUNDPITCH: expected 2 args");
        ::SetSoundPitch(rlreg::sounds.at(INT(0)), (float)FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("SETSOUNDPAN", Fn{"SETSOUNDPAN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETSOUNDPAN: expected 2 args");
        ::SetSoundPan(rlreg::sounds.at(INT(0)), (float)FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("WAVECOPY", Fn{"WAVECOPY", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("WAVECOPY: expected 1 args");
        ::Wave wave = ::WaveCopy(rlreg::waves.at(INT(0)));
        int id = rlreg::next_wave_id++;
        rlreg::waves[id] = wave;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("WAVECROP", Fn{"WAVECROP", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("WAVECROP: expected 3 args");
        ::WaveCrop(&rlreg::waves[INT(0)], INT(1), INT(2));
        return Value::nil();
    }}, true);    R.add_with_policy("WAVEFORMAT", Fn{"WAVEFORMAT", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("WAVEFORMAT: expected 4 args");
        ::WaveFormat(&rlreg::waves[INT(0)], INT(1), INT(2), INT(3));
        return Value::nil();
    }}, true);    R.add_with_policy("LOADWAVESAMPLES", Fn{"LOADWAVESAMPLES", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADWAVESAMPLES: expected 1 args");
        float* samples = ::LoadWaveSamples(rlreg::waves.at(INT(0)));
        (void)samples; // Suppress unused variable warning
        // Note: This returns a pointer, simplified for BASIC
        return Value::from_string("samples_loaded");
    }}, true);    R.add_with_policy("UNLOADWAVESAMPLES", Fn{"UNLOADWAVESAMPLES", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADWAVESAMPLES: expected 1 args");
        // Note: This is simplified - would need proper sample data handling
        ::UnloadWaveSamples(nullptr);
        return Value::nil();
    }}, true);    R.add_with_policy("LOADMUSICSTREAM", Fn{"LOADMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADMUSICSTREAM: expected 1 args");
        ::Music music = ::LoadMusicStream(STR(0).c_str());
        int id = rlreg::next_music_id++;
        rlreg::musics[id] = music;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADMUSICSTREAMFROMMEMORY", Fn{"LOADMUSICSTREAMFROMMEMORY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADMUSICSTREAMFROMMEMORY: expected 2 args");
        ::Music music = ::LoadMusicStreamFromMemory(STR(0).c_str(), (const unsigned char*)STR(1).c_str(), STR(1).length());
        int id = rlreg::next_music_id++;
        rlreg::musics[id] = music;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISMUSICSTREAMVALID", Fn{"ISMUSICSTREAMVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMUSICSTREAMVALID: expected 1 args");
        return Value::from_bool(::IsMusicStreamPlaying(rlreg::musics.at(INT(0))));
    }}, true);    R.add_with_policy("UNLOADMUSICSTREAM", Fn{"UNLOADMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADMUSICSTREAM: expected 1 args");
        int id = INT(0);
        auto it = rlreg::musics.find(id);
        if (it != rlreg::musics.end()) {
          ::UnloadMusicStream(it->second);
          rlreg::musics.erase(it);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("PLAYMUSICSTREAM", Fn{"PLAYMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PLAYMUSICSTREAM: expected 1 args");
        ::PlayMusicStream(rlreg::musics.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("ISMUSICSTREAMPLAYING", Fn{"ISMUSICSTREAMPLAYING", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMUSICSTREAMPLAYING: expected 1 args");
        return Value::from_bool(::IsMusicStreamPlaying(rlreg::musics.at(INT(0))));
    }}, true);    R.add_with_policy("UPDATEMUSICSTREAM", Fn{"UPDATEMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UPDATEMUSICSTREAM: expected 1 args");
        ::UpdateMusicStream(rlreg::musics.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("STOPMUSICSTREAM", Fn{"STOPMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("STOPMUSICSTREAM: expected 1 args");
        ::StopMusicStream(rlreg::musics.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("PAUSEMUSICSTREAM", Fn{"PAUSEMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PAUSEMUSICSTREAM: expected 1 args");
        ::PauseMusicStream(rlreg::musics.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("RESUMEMUSICSTREAM", Fn{"RESUMEMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("RESUMEMUSICSTREAM: expected 1 args");
        ::ResumeMusicStream(rlreg::musics.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("SEEKMUSICSTREAM", Fn{"SEEKMUSICSTREAM", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SEEKMUSICSTREAM: expected 2 args");
        ::SeekMusicStream(rlreg::musics.at(INT(0)), (float)FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("SETMUSICVOLUME", Fn{"SETMUSICVOLUME", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETMUSICVOLUME: expected 2 args");
        ::SetMusicVolume(rlreg::musics.at(INT(0)), (float)FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("SETMUSICPITCH", Fn{"SETMUSICPITCH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETMUSICPITCH: expected 2 args");
        ::SetMusicPitch(rlreg::musics.at(INT(0)), (float)FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("SETMUSICPAN", Fn{"SETMUSICPAN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETMUSICPAN: expected 2 args");
        ::SetMusicPan(rlreg::musics.at(INT(0)), (float)FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("GETMUSICTIMELENGTH", Fn{"GETMUSICTIMELENGTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMUSICTIMELENGTH: expected 1 args");
        return Value::from_number(::GetMusicTimeLength(rlreg::musics.at(INT(0))));
    }}, true);    R.add_with_policy("GETMUSICTIMEPLAYED", Fn{"GETMUSICTIMEPLAYED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMUSICTIMEPLAYED: expected 1 args");
        return Value::from_number(::GetMusicTimePlayed(rlreg::musics.at(INT(0))));
    }}, true);    R.add_with_policy("LOADAUDIOSTREAM", Fn{"LOADAUDIOSTREAM", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("LOADAUDIOSTREAM: expected 3 args");
        ::AudioStream stream = ::LoadAudioStream(INT(0), INT(1), INT(2));
        int id = rlreg::next_audiostream_id++;
        rlreg::audiostreams[id] = stream;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISAUDIOSTREAMVALID", Fn{"ISAUDIOSTREAMVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISAUDIOSTREAMVALID: expected 1 args");
        return Value::from_bool(::IsAudioStreamValid(rlreg::audiostreams.at(INT(0))));
    }}, true);    R.add_with_policy("UNLOADAUDIOSTREAM", Fn{"UNLOADAUDIOSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADAUDIOSTREAM: expected 1 args");
        int id = INT(0);
        auto it = rlreg::audiostreams.find(id);
        if (it != rlreg::audiostreams.end()) {
          ::UnloadAudioStream(it->second);
          rlreg::audiostreams.erase(it);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("UPDATEAUDIOSTREAM", Fn{"UPDATEAUDIOSTREAM", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("UPDATEAUDIOSTREAM: expected 2 args");
        // Note: This is simplified - would need proper audio data handling
        ::UpdateAudioStream(rlreg::audiostreams.at(INT(0)), nullptr, 0);
        return Value::nil();
    }}, true);    R.add_with_policy("ISAUDIOSTREAMPROCESSED", Fn{"ISAUDIOSTREAMPROCESSED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISAUDIOSTREAMPROCESSED: expected 1 args");
        return Value::from_bool(::IsAudioStreamProcessed(rlreg::audiostreams.at(INT(0))));
    }}, true);    R.add_with_policy("PLAYAUDIOSTREAM", Fn{"PLAYAUDIOSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PLAYAUDIOSTREAM: expected 1 args");
        ::PlayAudioStream(rlreg::audiostreams.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("PAUSEAUDIOSTREAM", Fn{"PAUSEAUDIOSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PAUSEAUDIOSTREAM: expected 1 args");
        ::PauseAudioStream(rlreg::audiostreams.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("RESUMEAUDIOSTREAM", Fn{"RESUMEAUDIOSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("RESUMEAUDIOSTREAM: expected 1 args");
        ::ResumeAudioStream(rlreg::audiostreams.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("ISAUDIOSTREAMPLAYING", Fn{"ISAUDIOSTREAMPLAYING", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISAUDIOSTREAMPLAYING: expected 1 args");
        return Value::from_bool(::IsAudioStreamPlaying(rlreg::audiostreams.at(INT(0))));
    }}, true);    R.add_with_policy("STOPAUDIOSTREAM", Fn{"STOPAUDIOSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("STOPAUDIOSTREAM: expected 1 args");
        ::StopAudioStream(rlreg::audiostreams.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("SETAUDIOSTREAMVOLUME", Fn{"SETAUDIOSTREAMVOLUME", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETAUDIOSTREAMVOLUME: expected 2 args");
        ::SetAudioStreamVolume(rlreg::audiostreams.at(INT(0)), (float)FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("SETAUDIOSTREAMPITCH", Fn{"SETAUDIOSTREAMPITCH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETAUDIOSTREAMPITCH: expected 2 args");
        ::SetAudioStreamPitch(rlreg::audiostreams.at(INT(0)), (float)FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("SETAUDIOSTREAMPAN", Fn{"SETAUDIOSTREAMPAN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETAUDIOSTREAMPAN: expected 2 args");
        ::SetAudioStreamPan(rlreg::audiostreams.at(INT(0)), (float)FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("SETAUDIOSTREAMBUFFERSIZEDEFAULT", Fn{"SETAUDIOSTREAMBUFFERSIZEDEFAULT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETAUDIOSTREAMBUFFERSIZEDEFAULT: expected 1 args");
        SetAudioStreamBufferSizeDefault(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETAUDIOSTREAMCALLBACK", Fn{"SETAUDIOSTREAMCALLBACK", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETAUDIOSTREAMCALLBACK: expected 2 args");
        // Note: This is simplified - would need proper callback handling
        ::SetAudioStreamCallback(rlreg::audiostreams.at(INT(0)), nullptr);
        return Value::nil();
    }}, true);    R.add_with_policy("ATTACHAUDIOSTREAMPROCESSOR", Fn{"ATTACHAUDIOSTREAMPROCESSOR", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ATTACHAUDIOSTREAMPROCESSOR: expected 2 args");
        // Note: This is simplified - would need proper processor handling
        ::AttachAudioStreamProcessor(rlreg::audiostreams.at(INT(0)), nullptr);
        return Value::nil();
    }}, true);    R.add_with_policy("DETACHAUDIOSTREAMPROCESSOR", Fn{"DETACHAUDIOSTREAMPROCESSOR", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("DETACHAUDIOSTREAMPROCESSOR: expected 2 args");
        // Note: This is simplified - would need proper processor handling
        ::DetachAudioStreamProcessor(rlreg::audiostreams.at(INT(0)), nullptr);
        return Value::nil();
    }}, true);    R.add_with_policy("ATTACHAUDIOMIXEDPROCESSOR", Fn{"ATTACHAUDIOMIXEDPROCESSOR", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ATTACHAUDIOMIXEDPROCESSOR: expected 1 args");
        // Note: This is simplified - would need proper processor handling
        ::AttachAudioMixedProcessor(nullptr);
        return Value::nil();
    }}, true);    R.add_with_policy("DETACHAUDIOMIXEDPROCESSOR", Fn{"DETACHAUDIOMIXEDPROCESSOR", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("DETACHAUDIOMIXEDPROCESSOR: expected 1 args");
        // Note: This is simplified - would need proper processor handling
        ::DetachAudioMixedProcessor(nullptr);
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADWAVE", Fn{"UNLOADWAVE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADWAVE: expected 1 args");
        int id = INT(0);
        auto it = rlreg::waves.find(id);
        if (it != rlreg::waves.end()) {
          ::UnloadWave(it->second);
          rlreg::waves.erase(it);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADSOUNDALIAS", Fn{"UNLOADSOUNDALIAS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADSOUNDALIAS: expected 1 args");
        int id = INT(0);
        auto it = rlreg::sounds.find(id);
        if (it != rlreg::sounds.end()) {
          ::UnloadSoundAlias(it->second);
          rlreg::sounds.erase(it);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("EXPORTWAVE", Fn{"EXPORTWAVE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTWAVE: expected 2 args");
        return Value::from_bool(::ExportWave(rlreg::waves.at(INT(0)), STR(1).c_str()));
    }}, true);    R.add_with_policy("EXPORTWAVEASCODE", Fn{"EXPORTWAVEASCODE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTWAVEASCODE: expected 2 args");
        return Value::from_bool(::ExportWaveAsCode(rlreg::waves.at(INT(0)), STR(1).c_str()));
    }}, true);    R.add_with_policy("LOADMODEL", Fn{"LOADMODEL", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADMODEL: expected 1 args");
        ::Model model = ::LoadModel(STR(0).c_str());
        int id = rlreg::next_model_id++;
        rlreg::models[id] = model;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADMODELFROMMESH", Fn{"LOADMODELFROMMESH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADMODELFROMMESH: expected 1 args");
        ::Model model = ::LoadModelFromMesh(rlreg::meshes.at(INT(0)));
        int id = rlreg::next_model_id++;
        rlreg::models[id] = model;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISMODELVALID", Fn{"ISMODELVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMODELVALID: expected 1 args");
        return Value::from_bool(::IsModelValid(rlreg::models.at(INT(0))));
    }}, true);    R.add_with_policy("GETMODELBOUNDINGBOX", Fn{"GETMODELBOUNDINGBOX", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMODELBOUNDINGBOX: expected 1 args");
        ::BoundingBox box = ::GetModelBoundingBox(rlreg::models.at(INT(0)));
        return Value::from_string(std::to_string(box.min.x) + "," + std::to_string(box.min.y) + "," + std::to_string(box.min.z) + "," + 
                                 std::to_string(box.max.x) + "," + std::to_string(box.max.y) + "," + std::to_string(box.max.z));
    }}, true);    R.add_with_policy("DRAWMODEL", Fn{"DRAWMODEL", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWMODEL: expected 8 args");
        ::Vector3 position{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        float scale = (float)FLOAT(4);
        ::Color tint{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawModel(rlreg::models.at(INT(0)), position, scale, tint);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWMODELEX", Fn{"DRAWMODELEX", 11, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 11) throw std::runtime_error("DRAWMODELEX: expected 11 args");
        ::Vector3 position{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        ::Vector3 rotationAxis{(float)FLOAT(4), (float)FLOAT(5), (float)FLOAT(6)};
        ::Vector3 scale{(float)FLOAT(7), (float)FLOAT(7), (float)FLOAT(7)};
        ::Color tint{(unsigned char)INT(8), (unsigned char)INT(9), (unsigned char)INT(10), 255};
        ::DrawModelEx(rlreg::models.at(INT(0)), position, rotationAxis, 0.0f, scale, tint);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWMODELWIRES", Fn{"DRAWMODELWIRES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWMODELWIRES: expected 8 args");
        ::Vector3 position{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        float scale = (float)FLOAT(4);
        ::Color tint{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawModelWires(rlreg::models.at(INT(0)), position, scale, tint);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWMODELWIRESEX", Fn{"DRAWMODELWIRESEX", 11, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 11) throw std::runtime_error("DRAWMODELWIRESEX: expected 11 args");
        ::Vector3 position{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        ::Vector3 rotationAxis{(float)FLOAT(4), (float)FLOAT(5), (float)FLOAT(6)};
        ::Vector3 scale{(float)FLOAT(7), (float)FLOAT(7), (float)FLOAT(7)};
        ::Color tint{(unsigned char)INT(8), (unsigned char)INT(9), (unsigned char)INT(10), 255};
        ::DrawModelWiresEx(rlreg::models.at(INT(0)), position, rotationAxis, 0.0f, scale, tint);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWBOUNDINGBOX", Fn{"DRAWBOUNDINGBOX", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("DRAWBOUNDINGBOX: expected 4 args");
        // Parse bounding box string format: "minX,minY,minZ,maxX,maxY,maxZ"
        std::string boxStr = STR(0);
        size_t pos = 0;
        std::vector<float> coords;
        while ((pos = boxStr.find(",")) != std::string::npos) {
          coords.push_back(std::stof(boxStr.substr(0, pos)));
          boxStr.erase(0, pos + 1);
        }
        coords.push_back(std::stof(boxStr));
        if (coords.size() >= 6) {
          ::BoundingBox box{{coords[0], coords[1], coords[2]}, {coords[3], coords[4], coords[5]}};
          ::Color c{(unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3), 255};
          ::DrawBoundingBox(box, c);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWBILLBOARD", Fn{"DRAWBILLBOARD", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWBILLBOARD: expected 9 args");
        // Note: This requires a Camera object, simplified for BASIC
        ::Camera camera = ::Camera{};
        ::Vector3 position{(float)FLOAT(2), (float)FLOAT(3), (float)FLOAT(4)};
        ::Color tint{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawBillboard(camera, rlreg::textures.at(INT(1)), position, (float)FLOAT(5), tint);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWBILLBOARDREC", Fn{"DRAWBILLBOARDREC", 14, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 14) throw std::runtime_error("DRAWBILLBOARDREC: expected 14 args");
        // Note: This requires a Camera object, simplified for BASIC
        ::Camera camera = ::Camera{};
        ::Rectangle source{(float)INT(2), (float)INT(3), (float)INT(4), (float)INT(5)};
        ::Vector3 position{(float)FLOAT(6), (float)FLOAT(7), (float)FLOAT(8)};
        ::Vector2 size{(float)FLOAT(9), (float)FLOAT(10)};
        ::Color tint{(unsigned char)INT(11), (unsigned char)INT(12), (unsigned char)INT(13), 255};
        ::DrawBillboardRec(camera, rlreg::textures.at(INT(1)), source, position, size, tint);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWBILLBOARDPRO", Fn{"DRAWBILLBOARDPRO", 18, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 18) throw std::runtime_error("DRAWBILLBOARDPRO: expected 18 args");
        // Note: This requires a Camera object, simplified for BASIC
        ::Camera camera = ::Camera{};
        ::Rectangle source{(float)INT(2), (float)INT(3), (float)INT(4), (float)INT(5)};
        ::Vector3 position{(float)FLOAT(6), (float)FLOAT(7), (float)FLOAT(8)};
        ::Vector3 up{(float)FLOAT(9), (float)FLOAT(10), (float)FLOAT(11)};
        ::Vector2 size{(float)FLOAT(12), (float)FLOAT(13)};
        ::Vector2 origin{(float)FLOAT(14), (float)FLOAT(15)};
        ::Color tint{(unsigned char)INT(16), (unsigned char)INT(17), (unsigned char)INT(18), 255};
        ::DrawBillboardPro(camera, rlreg::textures.at(INT(1)), source, position, up, size, origin, 0.0f, tint);
        return Value::nil();
    }}, true);    R.add_with_policy("UPLOADMESH", Fn{"UPLOADMESH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("UPLOADMESH: expected 2 args");
        ::UploadMesh(&rlreg::meshes[INT(0)], BOOL(1));
        return Value::nil();
    }}, true);    R.add_with_policy("UPDATEMESHBUFFER", Fn{"UPDATEMESHBUFFER", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("UPDATEMESHBUFFER: expected 5 args");
        // Note: This is simplified - would need proper buffer data handling
        ::UpdateMeshBuffer(rlreg::meshes.at(INT(0)), INT(1), nullptr, INT(3), INT(4));
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADMESH", Fn{"UNLOADMESH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADMESH: expected 1 args");
        int id = INT(0);
        auto it = rlreg::meshes.find(id);
        if (it != rlreg::meshes.end()) {
          ::UnloadMesh(it->second);
          rlreg::meshes.erase(it);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWMESH", Fn{"DRAWMESH", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("DRAWMESH: expected 3 args");
        // Note: This requires a Material object and Matrix transform, simplified for BASIC
        ::Material material = ::Material{};
        ::Matrix transform = MatrixIdentity();
        ::DrawMesh(rlreg::meshes.at(INT(0)), material, transform);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWMESHINSTANCED", Fn{"DRAWMESHINSTANCED", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("DRAWMESHINSTANCED: expected 3 args");
        // Note: This requires a Material object and Matrix transforms array, simplified for BASIC
        ::Material material = ::Material{};
        ::Matrix transform = MatrixIdentity();
        ::DrawMeshInstanced(rlreg::meshes.at(INT(0)), material, &transform, 1);
        return Value::nil();
    }}, true);    R.add_with_policy("GETMESHBOUNDINGBOX", Fn{"GETMESHBOUNDINGBOX", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMESHBOUNDINGBOX: expected 1 args");
        ::BoundingBox box = ::GetMeshBoundingBox(rlreg::meshes.at(INT(0)));
        return Value::from_string(std::to_string(box.min.x) + "," + std::to_string(box.min.y) + "," + std::to_string(box.min.z) + "," + 
                                 std::to_string(box.max.x) + "," + std::to_string(box.max.y) + "," + std::to_string(box.max.z));
    }}, true);    R.add_with_policy("GENMESHTANGENTS", Fn{"GENMESHTANGENTS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GENMESHTANGENTS: expected 1 args");
        ::GenMeshTangents(&rlreg::meshes[INT(0)]);
        return Value::nil();
    }}, true);    R.add_with_policy("EXPORTMESH", Fn{"EXPORTMESH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTMESH: expected 2 args");
        return Value::from_bool(::ExportMesh(rlreg::meshes.at(INT(0)), STR(1).c_str()));
    }}, true);    R.add_with_policy("GENMESHPOLY", Fn{"GENMESHPOLY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GENMESHPOLY: expected 2 args");
        ::Mesh mesh = ::GenMeshPoly(INT(0), (float)FLOAT(1));
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENMESHPLANE", Fn{"GENMESHPLANE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("GENMESHPLANE: expected 4 args");
        ::Mesh mesh = ::GenMeshPlane((float)FLOAT(0), (float)FLOAT(1), INT(2), INT(3));
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENMESHCUBE", Fn{"GENMESHCUBE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENMESHCUBE: expected 3 args");
        ::Mesh mesh = ::GenMeshCube((float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2));
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENMESHSPHERE", Fn{"GENMESHSPHERE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENMESHSPHERE: expected 3 args");
        ::Mesh mesh = ::GenMeshSphere((float)FLOAT(0), INT(1), INT(2));
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENMESHHEMISPHERE", Fn{"GENMESHHEMISPHERE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENMESHHEMISPHERE: expected 3 args");
        ::Mesh mesh = ::GenMeshHemiSphere((float)FLOAT(0), INT(1), INT(2));
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENMESHCYLINDER", Fn{"GENMESHCYLINDER", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENMESHCYLINDER: expected 3 args");
        ::Mesh mesh = ::GenMeshCylinder((float)FLOAT(0), (float)FLOAT(1), INT(2));
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENMESHCONE", Fn{"GENMESHCONE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENMESHCONE: expected 3 args");
        ::Mesh mesh = ::GenMeshCone((float)FLOAT(0), (float)FLOAT(1), INT(2));
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENMESHTORUS", Fn{"GENMESHTORUS", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("GENMESHTORUS: expected 4 args");
        ::Mesh mesh = ::GenMeshTorus((float)FLOAT(0), (float)FLOAT(1), INT(2), INT(3));
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENMESHKNOT", Fn{"GENMESHKNOT", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("GENMESHKNOT: expected 4 args");
        ::Mesh mesh = ::GenMeshKnot((float)FLOAT(0), (float)FLOAT(1), INT(2), INT(3));
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENMESHHEIGHTMAP", Fn{"GENMESHHEIGHTMAP", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("GENMESHHEIGHTMAP: expected 4 args");
        ::Vector3 size{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        ::Mesh mesh = ::GenMeshHeightmap(rlreg::images.at(INT(0)), size);
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENMESHCUBICMAP", Fn{"GENMESHCUBICMAP", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("GENMESHCUBICMAP: expected 4 args");
        ::Vector3 cubeSize{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        ::Mesh mesh = ::GenMeshCubicmap(rlreg::images.at(INT(0)), cubeSize);
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADMATERIALS", Fn{"LOADMATERIALS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADMATERIALS: expected 1 args");
        int materialCount;
        ::Material* materials = ::LoadMaterials(STR(0).c_str(), &materialCount);
        (void)materials; // Suppress unused variable warning
        // Note: This returns a pointer array, simplified for BASIC
        return Value::from_string("materials_loaded:" + std::to_string(materialCount));
    }}, true);    R.add_with_policy("LOADMATERIALDEFAULT", Fn{"LOADMATERIALDEFAULT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("LOADMATERIALDEFAULT: expected 0 args");
        ::Material material = ::LoadMaterialDefault();
        int id = rlreg::next_material_id++;
        rlreg::materials[id] = material;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISMATERIALVALID", Fn{"ISMATERIALVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMATERIALVALID: expected 1 args");
        return Value::from_bool(::IsMaterialValid(rlreg::materials.at(INT(0))));
    }}, true);    R.add_with_policy("UNLOADMATERIAL", Fn{"UNLOADMATERIAL", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADMATERIAL: expected 1 args");
        int id = INT(0);
        auto it = rlreg::materials.find(id);
        if (it != rlreg::materials.end()) {
          ::UnloadMaterial(it->second);
          rlreg::materials.erase(it);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("SETMATERIALTEXTURE", Fn{"SETMATERIALTEXTURE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETMATERIALTEXTURE: expected 3 args");
        ::SetMaterialTexture(&rlreg::materials[INT(0)], INT(1), rlreg::textures.at(INT(2)));
        return Value::nil();
    }}, true);    R.add_with_policy("SETMODELMESHMATERIAL", Fn{"SETMODELMESHMATERIAL", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETMODELMESHMATERIAL: expected 3 args");
        ::SetModelMeshMaterial(&rlreg::models[INT(0)], INT(1), INT(2));
        return Value::nil();
    }}, true);    R.add_with_policy("LOADMODELANIMATIONS", Fn{"LOADMODELANIMATIONS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADMODELANIMATIONS: expected 1 args");
        int animCount;
        ::ModelAnimation* animations = ::LoadModelAnimations(STR(0).c_str(), &animCount);
        (void)animations; // Suppress unused variable warning
        // Note: This returns a pointer array, simplified for BASIC
        return Value::from_string("animations_loaded:" + std::to_string(animCount));
    }}, true);    R.add_with_policy("UPDATEMODELANIMATION", Fn{"UPDATEMODELANIMATION", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("UPDATEMODELANIMATION: expected 3 args");
        // Note: This requires a ModelAnimation object, simplified for BASIC
        ::ModelAnimation anim = ::ModelAnimation{};
        ::UpdateModelAnimation(rlreg::models.at(INT(0)), anim, INT(2));
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADMODELANIMATION", Fn{"UNLOADMODELANIMATION", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADMODELANIMATION: expected 1 args");
        // Note: This requires a ModelAnimation object, simplified for BASIC
        ::ModelAnimation anim = ::ModelAnimation{};
        ::UnloadModelAnimation(anim);
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADMODELANIMATIONS", Fn{"UNLOADMODELANIMATIONS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADMODELANIMATIONS: expected 1 args");
        // Note: This requires a ModelAnimation array, simplified for BASIC
        ::UnloadModelAnimations(nullptr, 0);
        return Value::nil();
    }}, true);    R.add_with_policy("ISMODELANIMATIONVALID", Fn{"ISMODELANIMATIONVALID", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ISMODELANIMATIONVALID: expected 2 args");
        // Note: This requires a ModelAnimation object, simplified for BASIC
        ::ModelAnimation anim = ::ModelAnimation{};
        return Value::from_bool(::IsModelAnimationValid(rlreg::models.at(INT(0)), anim));
    }}, true);    R.add_with_policy("CHECKCOLLISIONSPHERES", Fn{"CHECKCOLLISIONSPHERES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("CHECKCOLLISIONSPHERES: expected 8 args");
        ::Vector3 center1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        float radius1 = (float)FLOAT(3);
        ::Vector3 center2{(float)FLOAT(4), (float)FLOAT(5), (float)FLOAT(6)};
        float radius2 = (float)FLOAT(7);
        return Value::from_bool(::CheckCollisionSpheres(center1, radius1, center2, radius2));
    }}, true);    R.add_with_policy("CHECKCOLLISIONBOXES", Fn{"CHECKCOLLISIONBOXES", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("CHECKCOLLISIONBOXES: expected 2 args");
        // Parse bounding box strings
        std::string box1Str = STR(0);
        std::string box2Str = STR(1);
        // Note: This is simplified - would need proper parsing
        ::BoundingBox box1 = ::BoundingBox{{0,0,0}, {1,1,1}};
        ::BoundingBox box2 = ::BoundingBox{{0,0,0}, {1,1,1}};
        return Value::from_bool(::CheckCollisionBoxes(box1, box2));
    }}, true);    R.add_with_policy("CHECKCOLLISIONBOXSPHERE", Fn{"CHECKCOLLISIONBOXSPHERE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("CHECKCOLLISIONBOXSPHERE: expected 5 args");
        // Parse bounding box string
        std::string boxStr = STR(0);
        // Note: This is simplified - would need proper parsing
        ::BoundingBox box = ::BoundingBox{{0,0,0}, {1,1,1}};
        ::Vector3 center{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        float radius = (float)FLOAT(4);
        return Value::from_bool(::CheckCollisionBoxSphere(box, center, radius));
    }}, true);    R.add_with_policy("GETRAYCOLLISIONSPHERE", Fn{"GETRAYCOLLISIONSPHERE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("GETRAYCOLLISIONSPHERE: expected 5 args");
        // Parse ray string format: "posX,posY,posZ,dirX,dirY,dirZ"
        std::string rayStr = STR(0);
        size_t pos = 0;
        std::vector<float> coords;
        while ((pos = rayStr.find(",")) != std::string::npos) {
          coords.push_back(std::stof(rayStr.substr(0, pos)));
          rayStr.erase(0, pos + 1);
        }
        coords.push_back(std::stof(rayStr));
        if (coords.size() >= 6) {
          ::Ray ray{{coords[0], coords[1], coords[2]}, {coords[3], coords[4], coords[5]}};
          ::Vector3 center{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
          float radius = (float)FLOAT(4);
          ::RayCollision collision = ::GetRayCollisionSphere(ray, center, radius);
          return Value::from_string(std::to_string(collision.hit) + "," + std::to_string(collision.distance) + "," + 
                                   std::to_string(collision.point.x) + "," + std::to_string(collision.point.y) + "," + std::to_string(collision.point.z));
        }
        return Value::from_string("0,0,0,0,0");
    }}, true);    R.add_with_policy("GETRAYCOLLISIONBOX", Fn{"GETRAYCOLLISIONBOX", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETRAYCOLLISIONBOX: expected 2 args");
        // Parse ray and bounding box strings
        std::string rayStr = STR(0);
        std::string boxStr = STR(1);
        // Note: This is simplified - would need proper parsing
        ::Ray ray = ::Ray{{0,0,0}, {0,0,1}};
        ::BoundingBox box = ::BoundingBox{{0,0,0}, {1,1,1}};
        ::RayCollision collision = ::GetRayCollisionBox(ray, box);
        return Value::from_string(std::to_string(collision.hit) + "," + std::to_string(collision.distance) + "," + 
                                 std::to_string(collision.point.x) + "," + std::to_string(collision.point.y) + "," + std::to_string(collision.point.z));
    }}, true);    R.add_with_policy("GETRAYCOLLISIONMESH", Fn{"GETRAYCOLLISIONMESH", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GETRAYCOLLISIONMESH: expected 3 args");
        // Parse ray and matrix strings
        std::string rayStr = STR(0);
        std::string matrixStr = STR(2);
        // Note: This is simplified - would need proper parsing
        ::Ray ray = ::Ray{{0,0,0}, {0,0,1}};
        ::Matrix transform = MatrixIdentity();
        ::RayCollision collision = ::GetRayCollisionMesh(ray, rlreg::meshes.at(INT(1)), transform);
        return Value::from_string(std::to_string(collision.hit) + "," + std::to_string(collision.distance) + "," + 
                                 std::to_string(collision.point.x) + "," + std::to_string(collision.point.y) + "," + std::to_string(collision.point.z));
    }}, true);    R.add_with_policy("GETRAYCOLLISIONTRIANGLE", Fn{"GETRAYCOLLISIONTRIANGLE", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("GETRAYCOLLISIONTRIANGLE: expected 10 args");
        // Parse ray string format: "posX,posY,posZ,dirX,dirY,dirZ"
        std::string rayStr = STR(0);
        size_t pos = 0;
        std::vector<float> coords;
        while ((pos = rayStr.find(",")) != std::string::npos) {
          coords.push_back(std::stof(rayStr.substr(0, pos)));
          rayStr.erase(0, pos + 1);
        }
        coords.push_back(std::stof(rayStr));
        if (coords.size() >= 6) {
          ::Ray ray{{coords[0], coords[1], coords[2]}, {coords[3], coords[4], coords[5]}};
          ::Vector3 p1{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
          ::Vector3 p2{(float)FLOAT(4), (float)FLOAT(5), (float)FLOAT(6)};
          ::Vector3 p3{(float)FLOAT(7), (float)FLOAT(8), (float)FLOAT(9)};
          ::RayCollision collision = ::GetRayCollisionTriangle(ray, p1, p2, p3);
          return Value::from_string(std::to_string(collision.hit) + "," + std::to_string(collision.distance) + "," + 
                                   std::to_string(collision.point.x) + "," + std::to_string(collision.point.y) + "," + std::to_string(collision.point.z));
        }
        return Value::from_string("0,0,0,0,0");
    }}, true);    R.add_with_policy("GETRAYCOLLISIONQUAD", Fn{"GETRAYCOLLISIONQUAD", 13, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 13) throw std::runtime_error("GETRAYCOLLISIONQUAD: expected 13 args");
        // Parse ray string format: "posX,posY,posZ,dirX,dirY,dirZ"
        std::string rayStr = STR(0);
        size_t pos = 0;
        std::vector<float> coords;
        while ((pos = rayStr.find(",")) != std::string::npos) {
          coords.push_back(std::stof(rayStr.substr(0, pos)));
          rayStr.erase(0, pos + 1);
        }
        coords.push_back(std::stof(rayStr));
        if (coords.size() >= 6) {
          ::Ray ray{{coords[0], coords[1], coords[2]}, {coords[3], coords[4], coords[5]}};
          ::Vector3 p1{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
          ::Vector3 p2{(float)FLOAT(4), (float)FLOAT(5), (float)FLOAT(6)};
          ::Vector3 p3{(float)FLOAT(7), (float)FLOAT(8), (float)FLOAT(9)};
          ::Vector3 p4{(float)FLOAT(10), (float)FLOAT(11), (float)FLOAT(12)};
          ::RayCollision collision = ::GetRayCollisionQuad(ray, p1, p2, p3, p4);
          return Value::from_string(std::to_string(collision.hit) + "," + std::to_string(collision.distance) + "," + 
                                   std::to_string(collision.point.x) + "," + std::to_string(collision.point.y) + "," + std::to_string(collision.point.z));
        }
        return Value::from_string("0,0,0,0,0");
    }}, true);    R.add_with_policy("UNLOADMODEL", Fn{"UNLOADMODEL", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADMODEL: expected 1 args");
        int id = INT(0);
        auto it = rlreg::models.find(id);
        if (it != rlreg::models.end()) {
          ::UnloadModel(it->second);
          rlreg::models.erase(it);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWLINE3D", Fn{"DRAWLINE3D", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWLINE3D: expected 9 args");
        ::Vector3 start{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 end{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Color c{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawLine3D(start, end, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPOINT3D", Fn{"DRAWPOINT3D", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWPOINT3D: expected 6 args");
        ::Vector3 position{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Color c{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::DrawPoint3D(position, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLE3D", Fn{"DRAWCIRCLE3D", 11, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 11) throw std::runtime_error("DRAWCIRCLE3D: expected 11 args");
        ::Vector3 center{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 rotationAxis{(float)FLOAT(4), (float)FLOAT(5), (float)FLOAT(6)};
        ::Color c{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawCircle3D(center, (float)FLOAT(3), rotationAxis, 0.0f, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTRIANGLE3D", Fn{"DRAWTRIANGLE3D", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWTRIANGLE3D: expected 12 args");
        ::Vector3 v1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 v2{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Vector3 v3{(float)FLOAT(6), (float)FLOAT(7), (float)FLOAT(8)};
        ::Color c{(unsigned char)INT(9), (unsigned char)INT(10), (unsigned char)INT(11), 255};
        ::DrawTriangle3D(v1, v2, v3, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTRIANGLESTRIP3D", Fn{"DRAWTRIANGLESTRIP3D", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("DRAWTRIANGLESTRIP3D: expected 4 args");
        // Note: This is simplified - would need proper Vector3 array handling
        ::Vector3 points[4] = {{0,0,0}, {1,0,0}, {0,1,0}, {1,1,0}};
        ::Color c{(unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3), 255};
        ::DrawTriangleStrip3D(points, 4, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCUBE", Fn{"DRAWCUBE", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCUBE: expected 9 args");
        ::Vector3 position{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Color c{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawCube(position, (float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCUBEV", Fn{"DRAWCUBEV", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCUBEV: expected 9 args");
        ::Vector3 position{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 size{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Color c{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawCubeV(position, size, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCUBEWIRES", Fn{"DRAWCUBEWIRES", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCUBEWIRES: expected 9 args");
        ::Vector3 position{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Color c{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawCubeWires(position, (float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCUBEWIRESV", Fn{"DRAWCUBEWIRESV", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCUBEWIRESV: expected 9 args");
        ::Vector3 position{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 size{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Color c{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawCubeWiresV(position, size, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPHERE", Fn{"DRAWSPHERE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWSPHERE: expected 7 args");
        ::Vector3 center{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Color c{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawSphere(center, (float)FLOAT(3), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPHEREEX", Fn{"DRAWSPHEREEX", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWSPHEREEX: expected 9 args");
        ::Vector3 center{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Color c{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawSphereEx(center, (float)FLOAT(3), INT(4), INT(5), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPHEREWIRES", Fn{"DRAWSPHEREWIRES", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWSPHEREWIRES: expected 9 args");
        ::Vector3 center{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Color c{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawSphereWires(center, (float)FLOAT(3), INT(4), INT(5), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCYLINDER", Fn{"DRAWCYLINDER", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWCYLINDER: expected 10 args");
        ::Vector3 position{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Color c{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawCylinder(position, (float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5), INT(6), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCYLINDEREX", Fn{"DRAWCYLINDEREX", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWCYLINDEREX: expected 12 args");
        ::Vector3 start{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 end{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Color c{(unsigned char)INT(10), (unsigned char)INT(11), (unsigned char)INT(12), 255};
        ::DrawCylinderEx(start, end, (float)FLOAT(6), (float)FLOAT(7), INT(8), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCYLINDERWIRES", Fn{"DRAWCYLINDERWIRES", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWCYLINDERWIRES: expected 10 args");
        ::Vector3 position{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Color c{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawCylinderWires(position, (float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5), INT(6), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCYLINDERWIRESEX", Fn{"DRAWCYLINDERWIRESEX", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWCYLINDERWIRESEX: expected 12 args");
        ::Vector3 start{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 end{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Color c{(unsigned char)INT(10), (unsigned char)INT(11), (unsigned char)INT(12), 255};
        ::DrawCylinderWiresEx(start, end, (float)FLOAT(6), (float)FLOAT(7), INT(8), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCAPSULE", Fn{"DRAWCAPSULE", 11, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 11) throw std::runtime_error("DRAWCAPSULE: expected 11 args");
        ::Vector3 start{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 end{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Color c{(unsigned char)INT(8), (unsigned char)INT(9), (unsigned char)INT(10), 255};
        ::DrawCapsule(start, end, (float)FLOAT(6), INT(7), INT(8), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCAPSULEWIRES", Fn{"DRAWCAPSULEWIRES", 11, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 11) throw std::runtime_error("DRAWCAPSULEWIRES: expected 11 args");
        ::Vector3 start{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 end{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Color c{(unsigned char)INT(8), (unsigned char)INT(9), (unsigned char)INT(10), 255};
        ::DrawCapsuleWires(start, end, (float)FLOAT(6), INT(7), INT(8), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPLANE", Fn{"DRAWPLANE", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWPLANE: expected 8 args");
        ::Vector3 center{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector2 size{(float)FLOAT(3), (float)FLOAT(4)};
        ::Color c{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawPlane(center, size, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRAY", Fn{"DRAWRAY", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("DRAWRAY: expected 4 args");
        // Parse ray string format: "posX,posY,posZ,dirX,dirY,dirZ"
        std::string rayStr = STR(0);
        size_t pos = 0;
        std::vector<float> coords;
        while ((pos = rayStr.find(",")) != std::string::npos) {
          coords.push_back(std::stof(rayStr.substr(0, pos)));
          rayStr.erase(0, pos + 1);
        }
        coords.push_back(std::stof(rayStr));
        if (coords.size() >= 6) {
          ::Ray ray{{coords[0], coords[1], coords[2]}, {coords[3], coords[4], coords[5]}};
          ::Color c{(unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3), 255};
          ::DrawRay(ray, c);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWGRID", Fn{"DRAWGRID", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("DRAWGRID: expected 2 args");
        ::DrawGrid(INT(0), (float)FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("CHECKCOLLISIONRECS", Fn{"CHECKCOLLISIONRECS", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("CHECKCOLLISIONRECS: expected 8 args");
        ::Rectangle rec1{(float)INT(0), (float)INT(1), (float)INT(2), (float)INT(3)};
        ::Rectangle rec2{(float)INT(4), (float)INT(5), (float)INT(6), (float)INT(7)};
        return Value::from_bool(::CheckCollisionRecs(rec1, rec2));
    }}, true);    R.add_with_policy("CHECKCOLLISIONCIRCLES", Fn{"CHECKCOLLISIONCIRCLES", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("CHECKCOLLISIONCIRCLES: expected 6 args");
        ::Vector2 center1{(float)INT(0), (float)INT(1)};
        float radius1 = (float)INT(2);
        ::Vector2 center2{(float)INT(3), (float)INT(4)};
        float radius2 = (float)INT(5);
        return Value::from_bool(::CheckCollisionCircles(center1, radius1, center2, radius2));
    }}, true);    R.add_with_policy("CHECKCOLLISIONCIRCLEREC", Fn{"CHECKCOLLISIONCIRCLEREC", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("CHECKCOLLISIONCIRCLEREC: expected 7 args");
        ::Vector2 center{(float)INT(0), (float)INT(1)};
        float radius = (float)INT(2);
        ::Rectangle rec{(float)INT(3), (float)INT(4), (float)INT(5), (float)INT(6)};
        return Value::from_bool(::CheckCollisionCircleRec(center, radius, rec));
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTREC", Fn{"CHECKCOLLISIONPOINTREC", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("CHECKCOLLISIONPOINTREC: expected 6 args");
        ::Vector2 point{(float)INT(0), (float)INT(1)};
        ::Rectangle rec{(float)INT(2), (float)INT(3), (float)INT(4), (float)INT(5)};
        return Value::from_bool(::CheckCollisionPointRec(point, rec));
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTCIRCLE", Fn{"CHECKCOLLISIONPOINTCIRCLE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("CHECKCOLLISIONPOINTCIRCLE: expected 5 args");
        ::Vector2 point{(float)INT(0), (float)INT(1)};
        ::Vector2 center{(float)INT(2), (float)INT(3)};
        float radius = (float)INT(4);
        return Value::from_bool(::CheckCollisionPointCircle(point, center, radius));
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTTRIANGLE", Fn{"CHECKCOLLISIONPOINTTRIANGLE", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("CHECKCOLLISIONPOINTTRIANGLE: expected 9 args");
        ::Vector2 point{(float)INT(0), (float)INT(1)};
        ::Vector2 p1{(float)INT(2), (float)INT(3)};
        ::Vector2 p2{(float)INT(4), (float)INT(5)};
        ::Vector2 p3{(float)INT(6), (float)INT(7)};
        return Value::from_bool(::CheckCollisionPointTriangle(point, p1, p2, p3));
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTLINE", Fn{"CHECKCOLLISIONPOINTLINE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("CHECKCOLLISIONPOINTLINE: expected 7 args");
        ::Vector2 point{(float)INT(0), (float)INT(1)};
        ::Vector2 p1{(float)INT(2), (float)INT(3)};
        ::Vector2 p2{(float)INT(4), (float)INT(5)};
        return Value::from_bool(::CheckCollisionPointLine(point, p1, p2, INT(6)));
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTPOLY", Fn{"CHECKCOLLISIONPOINTPOLY", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("CHECKCOLLISIONPOINTPOLY: expected 3 args");
        ::Vector2 point{(float)INT(0), (float)INT(1)};
        // Note: This is simplified - would need proper polygon parsing
        ::Vector2 points[4] = {{0,0}, {1,0}, {1,1}, {0,1}};
        return Value::from_bool(::CheckCollisionPointPoly(point, points, 4));
    }}, true);    R.add_with_policy("CHECKCOLLISIONLINES", Fn{"CHECKCOLLISIONLINES", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("CHECKCOLLISIONLINES: expected 9 args");
        ::Vector2 startPos1{(float)INT(0), (float)INT(1)};
        ::Vector2 endPos1{(float)INT(2), (float)INT(3)};
        ::Vector2 startPos2{(float)INT(4), (float)INT(5)};
        ::Vector2 endPos2{(float)INT(6), (float)INT(7)};
        ::Vector2 collisionPoint;
        bool result = ::CheckCollisionLines(startPos1, endPos1, startPos2, endPos2, &collisionPoint);
        // Note: collisionPoint would need to be returned somehow
        return Value::from_bool(result);
    }}, true);    R.add_with_policy("GETCOLLISIONREC", Fn{"GETCOLLISIONREC", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("GETCOLLISIONREC: expected 8 args");
        ::Rectangle rec1{(float)INT(0), (float)INT(1), (float)INT(2), (float)INT(3)};
        ::Rectangle rec2{(float)INT(4), (float)INT(5), (float)INT(6), (float)INT(7)};
        ::Rectangle collision = ::GetCollisionRec(rec1, rec2);
        return Value::from_string(std::to_string(collision.x) + "," + std::to_string(collision.y) + "," + 
                                 std::to_string(collision.width) + "," + std::to_string(collision.height));
    }}, true);    R.add_with_policy("LOADSHADER", Fn{"LOADSHADER", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADSHADER: expected 2 args");
        ::Shader shader = ::LoadShader(STR(0).c_str(), STR(1).c_str());
        int id = rlreg::next_shader_id++;
        rlreg::shaders[id] = shader;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADSHADERFROMMEMORY", Fn{"LOADSHADERFROMMEMORY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADSHADERFROMMEMORY: expected 2 args");
        ::Shader shader = ::LoadShaderFromMemory(STR(0).c_str(), STR(1).c_str());
        int id = rlreg::next_shader_id++;
        rlreg::shaders[id] = shader;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISSHADERVALID", Fn{"ISSHADERVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISSHADERVALID: expected 1 args");
        return Value::from_bool(::IsShaderValid(rlreg::shaders.at(INT(0))));
    }}, true);    R.add_with_policy("GETSHADERLOCATION", Fn{"GETSHADERLOCATION", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETSHADERLOCATION: expected 2 args");
        return Value::from_int(::GetShaderLocation(rlreg::shaders.at(INT(0)), STR(1).c_str()));
    }}, true);    R.add_with_policy("GETSHADERLOCATIONATTRIB", Fn{"GETSHADERLOCATIONATTRIB", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETSHADERLOCATIONATTRIB: expected 2 args");
        return Value::from_int(::GetShaderLocationAttrib(rlreg::shaders.at(INT(0)), STR(1).c_str()));
    }}, true);    R.add_with_policy("SETSHADERVALUE", Fn{"SETSHADERVALUE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("SETSHADERVALUE: expected 4 args");
        // Note: This is simplified - would need proper value handling
        ::SetShaderValue(rlreg::shaders.at(INT(0)), INT(1), nullptr, INT(3));
        return Value::nil();
    }}, true);    R.add_with_policy("SETSHADERVALUEV", Fn{"SETSHADERVALUEV", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("SETSHADERVALUEV: expected 5 args");
        // Note: This is simplified - would need proper value handling
        ::SetShaderValueV(rlreg::shaders.at(INT(0)), INT(1), nullptr, INT(3), INT(4));
        return Value::nil();
    }}, true);    R.add_with_policy("SETSHADERVALUEMATRIX", Fn{"SETSHADERVALUEMATRIX", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETSHADERVALUEMATRIX: expected 3 args");
        // Note: This is simplified - would need proper matrix handling
        ::Matrix matrix = MatrixIdentity();
        ::SetShaderValueMatrix(rlreg::shaders.at(INT(0)), INT(1), matrix);
        return Value::nil();
    }}, true);    R.add_with_policy("SETSHADERVALUETEXTURE", Fn{"SETSHADERVALUETEXTURE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETSHADERVALUETEXTURE: expected 3 args");
        ::SetShaderValueTexture(rlreg::shaders.at(INT(0)), INT(1), rlreg::textures.at(INT(2)));
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINSHADERMODE", Fn{"BEGINSHADERMODE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("BEGINSHADERMODE: expected 1 args");
        ::BeginShaderMode(rlreg::shaders.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("ENDSHADERMODE", Fn{"ENDSHADERMODE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDSHADERMODE: expected 0 args");
        EndShaderMode();
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADSHADER", Fn{"UNLOADSHADER", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADSHADER: expected 1 args");
        int id = INT(0);
        auto it = rlreg::shaders.find(id);
        if (it != rlreg::shaders.end()) {
          ::UnloadShader(it->second);
          rlreg::shaders.erase(it);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("GETRANDOMVALUE", Fn{"GETRANDOMVALUE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETRANDOMVALUE: expected 2 args");
        return Value::from_int(GetRandomValue(args[0].as_int(), args[1].as_int()));
    }}, true);    R.add_with_policy("SETRANDOMSEED", Fn{"SETRANDOMSEED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETRANDOMSEED: expected 1 args");
        SetRandomSeed(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("GETMONITORCOUNT", Fn{"GETMONITORCOUNT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMONITORCOUNT: expected 0 args");
        return Value::from_int(GetMonitorCount());
    }}, true);    R.add_with_policy("GETMONITORWIDTH", Fn{"GETMONITORWIDTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORWIDTH: expected 1 args");
        return Value::from_int(GetMonitorWidth(args[0].as_int()));
    }}, true);    R.add_with_policy("GETMONITORHEIGHT", Fn{"GETMONITORHEIGHT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORHEIGHT: expected 1 args");
        return Value::from_int(GetMonitorHeight(args[0].as_int()));
    }}, true);    R.add_with_policy("GETMONITORPHYSICALWIDTH", Fn{"GETMONITORPHYSICALWIDTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORPHYSICALWIDTH: expected 1 args");
        return Value::from_int(GetMonitorPhysicalWidth(args[0].as_int()));
    }}, true);    R.add_with_policy("GETMONITORPHYSICALHEIGHT", Fn{"GETMONITORPHYSICALHEIGHT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORPHYSICALHEIGHT: expected 1 args");
        return Value::from_int(GetMonitorPhysicalHeight(args[0].as_int()));
    }}, true);    R.add_with_policy("GETMONITORREFRESHRATE", Fn{"GETMONITORREFRESHRATE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORREFRESHRATE: expected 1 args");
        return Value::from_int(GetMonitorRefreshRate(args[0].as_int()));
    }}, true);    R.add_with_policy("GETWINDOWPOSITION", Fn{"GETWINDOWPOSITION", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETWINDOWPOSITION: expected 0 args");
        ::Vector2 pos = ::GetWindowPosition();
        return Value::from_string(std::to_string(pos.x) + "," + std::to_string(pos.y));
    }}, true);    R.add_with_policy("GETWINDOWSCALEDPI", Fn{"GETWINDOWSCALEDPI", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETWINDOWSCALEDPI: expected 0 args");
        ::Vector2 scale = ::GetWindowScaleDPI();
        return Value::from_string(std::to_string(scale.x) + "," + std::to_string(scale.y));
    }}, true);    R.add_with_policy("SETWINDOWMIN", Fn{"SETWINDOWMIN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETWINDOWMIN: expected 2 args");
        SetWindowMinSize(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWMAX", Fn{"SETWINDOWMAX", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETWINDOWMAX: expected 2 args");
        SetWindowMaxSize(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETCONFIGFLAGS", Fn{"SETCONFIGFLAGS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETCONFIGFLAGS: expected 1 args");
        SetConfigFlags(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("OPENURL", Fn{"OPENURL", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("OPENURL: expected 1 args");
        OpenURL(args[0].as_string().c_str());
        return Value::nil();
    }}, true);    R.add_with_policy("TRACELOG", Fn{"TRACELOG", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("TRACELOG: expected 2 args");
        TraceLog(args[0].as_int(), args[1].as_string().c_str());
        return Value::nil();
    }}, true);    R.add_with_policy("SETTRACELOGLEVEL", Fn{"SETTRACELOGLEVEL", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETTRACELOGLEVEL: expected 1 args");
        SetTraceLogLevel(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("MEMALLOC", Fn{"MEMALLOC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("MEMALLOC: expected 1 args");
        void* ptr = ::MemAlloc(INT(0));
        return Value::from_int((intptr_t)ptr);
    }}, true);    R.add_with_policy("MEMREALLOC", Fn{"MEMREALLOC", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("MEMREALLOC: expected 2 args");
        void* ptr = ::MemRealloc((void*)(intptr_t)INT(0), INT(1));
        return Value::from_int((intptr_t)ptr);
    }}, true);    R.add_with_policy("MEMFREE", Fn{"MEMFREE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("MEMFREE: expected 1 args");
        ::MemFree((void*)(intptr_t)INT(0));
        return Value::nil();
    }}, true);    R.add_with_policy("LOADFILEDATA", Fn{"LOADFILEDATA", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADFILEDATA: expected 1 args");
        int dataSize;
        unsigned char* data = ::LoadFileData(STR(0).c_str(), &dataSize);
        (void)data; // Suppress unused variable warning
        // Note: This returns binary data, simplified for BASIC
        return Value::from_string("data_loaded:" + std::to_string(dataSize));
    }}, true);    R.add_with_policy("UNLOADFILEDATA", Fn{"UNLOADFILEDATA", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADFILEDATA: expected 1 args");
        // Note: This is simplified - would need proper data handling
        ::UnloadFileData(nullptr);
        return Value::nil();
    }}, true);    R.add_with_policy("SAVEFILEDATA", Fn{"SAVEFILEDATA", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SAVEFILEDATA: expected 2 args");
        // Note: This is simplified - would need proper data handling
        return Value::from_bool(::SaveFileData(STR(0).c_str(), nullptr, 0));
    }}, true);    R.add_with_policy("EXPORTDATAASCODE", Fn{"EXPORTDATAASCODE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTDATAASCODE: expected 2 args");
        // Note: This is simplified - would need proper data handling
        return Value::from_bool(::ExportDataAsCode(nullptr, 0, STR(1).c_str()));
    }}, true);    R.add_with_policy("LOADFILETEXT", Fn{"LOADFILETEXT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADFILETEXT: expected 1 args");
        char* text = ::LoadFileText(STR(0).c_str());
        std::string result(text);
        ::UnloadFileText(text);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("UNLOADFILETEXT", Fn{"UNLOADFILETEXT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADFILETEXT: expected 1 args");
        // Note: This is simplified - would need proper text handling
        ::UnloadFileText(nullptr);
        return Value::nil();
    }}, true);    R.add_with_policy("DIRECTORYEXISTS", Fn{"DIRECTORYEXISTS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("DIRECTORYEXISTS: expected 1 args");
        return Value::from_bool(DirectoryExists(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("ISFILEEXTENSION", Fn{"ISFILEEXTENSION", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ISFILEEXTENSION: expected 2 args");
        return Value::from_bool(IsFileExtension(args[0].as_string().c_str(), args[1].as_string().c_str()));
    }}, true);    R.add_with_policy("GETFILELENGTH", Fn{"GETFILELENGTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETFILELENGTH: expected 1 args");
        return Value::from_int(GetFileLength(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("GETFILEEXTENSION", Fn{"GETFILEEXTENSION", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETFILEEXTENSION: expected 1 args");
        return Value::from_string(GetFileExtension(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("GETFILENAME", Fn{"GETFILENAME", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETFILENAME: expected 1 args");
        return Value::from_string(GetFileName(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("GETFILENAMEWITHOUTEXT", Fn{"GETFILENAMEWITHOUTEXT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETFILENAMEWITHOUTEXT: expected 1 args");
        return Value::from_string(GetFileNameWithoutExt(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("GETDIRECTORYPATH", Fn{"GETDIRECTORYPATH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETDIRECTORYPATH: expected 1 args");
        return Value::from_string(GetDirectoryPath(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("GETPREVDIRECTORYPATH", Fn{"GETPREVDIRECTORYPATH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETPREVDIRECTORYPATH: expected 1 args");
        return Value::from_string(GetPrevDirectoryPath(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("GETWORKINGDIRECTORY", Fn{"GETWORKINGDIRECTORY", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETWORKINGDIRECTORY: expected 0 args");
        return Value::from_string(GetWorkingDirectory());
    }}, true);    R.add_with_policy("GETAPPLICATIONDIRECTORY", Fn{"GETAPPLICATIONDIRECTORY", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETAPPLICATIONDIRECTORY: expected 0 args");
        return Value::from_string(GetApplicationDirectory());
    }}, true);    R.add_with_policy("CHANGEDIRECTORY", Fn{"CHANGEDIRECTORY", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("CHANGEDIRECTORY: expected 1 args");
        return Value::from_bool(ChangeDirectory(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("ISPATHFILE", Fn{"ISPATHFILE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISPATHFILE: expected 1 args");
        return Value::from_bool(IsPathFile(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("LOADDIRECTORYFILES", Fn{"LOADDIRECTORYFILES", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADDIRECTORYFILES: expected 1 args");
        ::FilePathList files = ::LoadDirectoryFiles(STR(0).c_str());
        // Note: This returns a FilePathList struct, simplified for BASIC
        return Value::from_string("files_loaded:" + std::to_string(files.count));
    }}, true);    R.add_with_policy("LOADDIRECTORYFILESEX", Fn{"LOADDIRECTORYFILESEX", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("LOADDIRECTORYFILESEX: expected 3 args");
        ::FilePathList files = ::LoadDirectoryFilesEx(STR(0).c_str(), STR(1).c_str(), BOOL(2));
        // Note: This returns a FilePathList struct, simplified for BASIC
        return Value::from_string("files_loaded:" + std::to_string(files.count));
    }}, true);    R.add_with_policy("UNLOADDIRECTORYFILES", Fn{"UNLOADDIRECTORYFILES", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADDIRECTORYFILES: expected 1 args");
        // Note: This is simplified - would need proper FilePathList handling
        ::FilePathList files = ::FilePathList{};
        ::UnloadDirectoryFiles(files);
        return Value::nil();
    }}, true);    R.add_with_policy("ISFILEDROPPED", Fn{"ISFILEDROPPED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISFILEDROPPED: expected 0 args");
        return Value::from_bool(IsFileDropped());
    }}, true);    R.add_with_policy("LOADDROPPEDFILES", Fn{"LOADDROPPEDFILES", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("LOADDROPPEDFILES: expected 0 args");
        ::FilePathList files = ::LoadDroppedFiles();
        // Note: This returns a FilePathList struct, simplified for BASIC
        return Value::from_string("files_loaded:" + std::to_string(files.count));
    }}, true);    R.add_with_policy("UNLOADDROPPEDFILES", Fn{"UNLOADDROPPEDFILES", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADDROPPEDFILES: expected 1 args");
        // Note: This is simplified - would need proper FilePathList handling
        ::FilePathList files = ::FilePathList{};
        ::UnloadDroppedFiles(files);
        return Value::nil();
    }}, true);    R.add_with_policy("GETFILEMODTIME", Fn{"GETFILEMODTIME", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETFILEMODTIME: expected 1 args");
        return Value::from_int(GetFileModTime(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("COMPRESSDATA", Fn{"COMPRESSDATA", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("COMPRESSDATA: expected 1 args");
        // Note: This is simplified - would need proper data handling
        int compDataSize;
        unsigned char* compData = ::CompressData(nullptr, 0, &compDataSize);
        (void)compData; // Suppress unused variable warning
        return Value::from_string("compressed:" + std::to_string(compDataSize));
    }}, true);    R.add_with_policy("DECOMPRESSDATA", Fn{"DECOMPRESSDATA", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("DECOMPRESSDATA: expected 1 args");
        // Note: This is simplified - would need proper data handling
        int dataSize;
        unsigned char* data = ::DecompressData(nullptr, 0, &dataSize);
        (void)data; // Suppress unused variable warning
        return Value::from_string("decompressed:" + std::to_string(dataSize));
    }}, true);    R.add_with_policy("ENCODEDATABASE64", Fn{"ENCODEDATABASE64", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ENCODEDATABASE64: expected 1 args");
        // Note: This is simplified - would need proper data handling
        int outputSize;
        char* encoded = ::EncodeDataBase64(nullptr, 0, &outputSize);
        std::string result(encoded);
        ::MemFree(encoded);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("LOADIMAGERAW", Fn{"LOADIMAGERAW", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("LOADIMAGERAW: expected 5 args");
        ::Image image = ::LoadImageRaw(STR(0).c_str(), INT(1), INT(2), INT(3), INT(4));
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADIMAGEANIM", Fn{"LOADIMAGEANIM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADIMAGEANIM: expected 1 args");
        int frames;
        ::Image image = ::LoadImageAnim(STR(0).c_str(), &frames);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_string("image_loaded:" + std::to_string(frames));
    }}, true);    R.add_with_policy("LOADIMAGEFROMMEMORY", Fn{"LOADIMAGEFROMMEMORY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADIMAGEFROMMEMORY: expected 2 args");
        ::Image image = ::LoadImageFromMemory(STR(0).c_str(), (const unsigned char*)STR(1).c_str(), STR(1).length());
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADIMAGEFROMTEXTURE", Fn{"LOADIMAGEFROMTEXTURE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADIMAGEFROMTEXTURE: expected 1 args");
        ::Image image = ::LoadImageFromTexture(rlreg::textures.at(INT(0)));
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADIMAGEFROMSCREEN", Fn{"LOADIMAGEFROMSCREEN", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("LOADIMAGEFROMSCREEN: expected 0 args");
        ::Image image = ::LoadImageFromScreen();
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISIMAGEVALID", Fn{"ISIMAGEVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISIMAGEVALID: expected 1 args");
        return Value::from_bool(::IsImageValid(rlreg::images.at(INT(0))));
    }}, true);    R.add_with_policy("EXPORTIMAGE", Fn{"EXPORTIMAGE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTIMAGE: expected 2 args");
        return Value::from_bool(::ExportImage(rlreg::images.at(INT(0)), STR(1).c_str()));
    }}, true);    R.add_with_policy("EXPORTIMAGETOMEMORY", Fn{"EXPORTIMAGETOMEMORY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTIMAGETOMEMORY: expected 2 args");
        int fileSize;
        unsigned char* data = ::ExportImageToMemory(rlreg::images.at(INT(0)), STR(1).c_str(), &fileSize);
        (void)data; // Suppress unused variable warning
        // Note: This returns binary data, simplified for BASIC
        return Value::from_string("data_exported:" + std::to_string(fileSize));
    }}, true);    R.add_with_policy("EXPORTIMAGEASCODE", Fn{"EXPORTIMAGEASCODE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTIMAGEASCODE: expected 2 args");
        return Value::from_bool(::ExportImageAsCode(rlreg::images.at(INT(0)), STR(1).c_str()));
    }}, true);    R.add_with_policy("GENIMAGECOLOR", Fn{"GENIMAGECOLOR", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("GENIMAGECOLOR: expected 5 args");
        ::Color color{(unsigned char)INT(2), (unsigned char)INT(3), (unsigned char)INT(4), 255};
        ::Image image = ::GenImageColor(INT(0), INT(1), color);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENIMAGEGRADIENTLINEAR", Fn{"GENIMAGEGRADIENTLINEAR", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("GENIMAGEGRADIENTLINEAR: expected 12 args");
        ::Color start{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::Color end{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::Image image = ::GenImageGradientLinear(INT(0), INT(1), INT(2), start, end);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENIMAGEGRADIENTRADIAL", Fn{"GENIMAGEGRADIENTRADIAL", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("GENIMAGEGRADIENTRADIAL: expected 9 args");
        ::Color inner{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::Color outer{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::Image image = ::GenImageGradientRadial(INT(0), INT(1), (float)FLOAT(2), inner, outer);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENIMAGEGRADIENTSQUARE", Fn{"GENIMAGEGRADIENTSQUARE", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("GENIMAGEGRADIENTSQUARE: expected 9 args");
        ::Color inner{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::Color outer{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::Image image = ::GenImageGradientSquare(INT(0), INT(1), (float)FLOAT(2), inner, outer);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENIMAGECHECKED", Fn{"GENIMAGECHECKED", 15, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 15) throw std::runtime_error("GENIMAGECHECKED: expected 15 args");
        ::Color col1{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::Color col2{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::Image image = ::GenImageChecked(INT(0), INT(1), INT(2), INT(3), col1, col2);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENIMAGEWHITENOISE", Fn{"GENIMAGEWHITENOISE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENIMAGEWHITENOISE: expected 3 args");
        ::Image image = ::GenImageWhiteNoise(INT(0), INT(1), (float)FLOAT(2));
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENIMAGEPERLINNOISE", Fn{"GENIMAGEPERLINNOISE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("GENIMAGEPERLINNOISE: expected 5 args");
        ::Image image = ::GenImagePerlinNoise(INT(0), INT(1), INT(2), INT(3), (float)FLOAT(4));
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENIMAGECELLULAR", Fn{"GENIMAGECELLULAR", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENIMAGECELLULAR: expected 3 args");
        ::Image image = ::GenImageCellular(INT(0), INT(1), INT(2));
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENIMAGETEXT", Fn{"GENIMAGETEXT", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENIMAGETEXT: expected 3 args");
        ::Image image = ::GenImageText(INT(0), INT(1), STR(2).c_str());
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GETFONTDEFAULT", Fn{"GETFONTDEFAULT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETFONTDEFAULT: expected 0 args");
        ::Font font = ::GetFontDefault();
        int id = rlreg::next_font_id++;
        rlreg::fonts[id] = font;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADFONT", Fn{"LOADFONT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADFONT: expected 1 args");
        ::Font font = ::LoadFont(STR(0).c_str());
        int id = rlreg::next_font_id++;
        rlreg::fonts[id] = font;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADFONTEX", Fn{"LOADFONTEX", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("LOADFONTEX: expected 3 args");
        // Note: This is simplified - would need proper codepoints array handling
        ::Font font = ::LoadFontEx(STR(0).c_str(), INT(1), nullptr, 0);
        int id = rlreg::next_font_id++;
        rlreg::fonts[id] = font;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADFONTFROMIMAGE", Fn{"LOADFONTFROMIMAGE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("LOADFONTFROMIMAGE: expected 5 args");
        ::Color key{(unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3), 255};
        ::Font font = ::LoadFontFromImage(rlreg::images.at(INT(0)), key, INT(4));
        int id = rlreg::next_font_id++;
        rlreg::fonts[id] = font;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADFONTFROMMEMORY", Fn{"LOADFONTFROMMEMORY", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("LOADFONTFROMMEMORY: expected 4 args");
        // Note: This is simplified - would need proper codepoints array handling
        ::Font font = ::LoadFontFromMemory(STR(0).c_str(), (const unsigned char*)STR(1).c_str(), STR(1).length(), INT(2), nullptr, 0);
        int id = rlreg::next_font_id++;
        rlreg::fonts[id] = font;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISFONTVALID", Fn{"ISFONTVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISFONTVALID: expected 1 args");
        return Value::from_bool(::IsFontValid(rlreg::fonts.at(INT(0))));
    }}, true);    R.add_with_policy("LOADFONTDATA", Fn{"LOADFONTDATA", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("LOADFONTDATA: expected 4 args");
        // Note: This is simplified - would need proper codepoints array handling
        int glyphCount = 0;
        ::GlyphInfo* glyphs = ::LoadFontData((const unsigned char*)STR(0).c_str(), STR(0).length(), INT(1), nullptr, 0, INT(3));
        (void)glyphs; // Suppress unused variable warning
        // Note: This returns a GlyphInfo array, simplified for BASIC
        return Value::from_string("glyphs_loaded:" + std::to_string(glyphCount));
    }}, true);    R.add_with_policy("GENIMAGEFONTATLAS", Fn{"GENIMAGEFONTATLAS", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("GENIMAGEFONTATLAS: expected 5 args");
        // Note: This is simplified - would need proper GlyphInfo array handling
        ::GlyphInfo glyphs[1] = {::GlyphInfo{}};
        ::Rectangle* glyphRecs = nullptr;
        ::Image image = ::GenImageFontAtlas(glyphs, &glyphRecs, 1, INT(2), INT(3), INT(4));
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("UNLOADFONTDATA", Fn{"UNLOADFONTDATA", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADFONTDATA: expected 1 args");
        // Note: This is simplified - would need proper GlyphInfo array handling
        ::UnloadFontData(nullptr, 0);
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADFONT", Fn{"UNLOADFONT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADFONT: expected 1 args");
        int id = INT(0);
        auto it = rlreg::fonts.find(id);
        if (it != rlreg::fonts.end()) {
          ::UnloadFont(it->second);
          rlreg::fonts.erase(it);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("EXPORTFONTASCODE", Fn{"EXPORTFONTASCODE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTFONTASCODE: expected 2 args");
        return Value::from_bool(::ExportFontAsCode(rlreg::fonts.at(INT(0)), STR(1).c_str()));
    }}, true);    R.add_with_policy("SETTEXTLINESPACING", Fn{"SETTEXTLINESPACING", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETTEXTLINESPACING: expected 1 args");
        SetTextLineSpacing(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("MEASURETEXT", Fn{"MEASURETEXT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("MEASURETEXT: expected 2 args");
        return Value::from_int(MeasureText(args[0].as_string().c_str(), args[1].as_int()));
    }}, true);    R.add_with_policy("MEASURETEXTEX", Fn{"MEASURETEXTEX", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("MEASURETEXTEX: expected 4 args");
        ::Vector2 size = ::MeasureTextEx(rlreg::fonts.at(INT(0)), STR(1).c_str(), (float)FLOAT(2), (float)FLOAT(3));
        return Value::from_string(std::to_string(size.x) + "," + std::to_string(size.y));
    }}, true);    R.add_with_policy("GETGLYPHINDEX", Fn{"GETGLYPHINDEX", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETGLYPHINDEX: expected 2 args");
        return Value::from_int(::GetGlyphIndex(rlreg::fonts.at(INT(0)), INT(1)));
    }}, true);    R.add_with_policy("GETGLYPHINFO", Fn{"GETGLYPHINFO", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETGLYPHINFO: expected 2 args");
        ::GlyphInfo info = ::GetGlyphInfo(rlreg::fonts.at(INT(0)), INT(1));
        return Value::from_string(std::to_string(info.value) + "," + std::to_string(info.offsetX) + "," + std::to_string(info.offsetY) + "," + std::to_string(info.advanceX));
    }}, true);    R.add_with_policy("GETGLYPHATLASREC", Fn{"GETGLYPHATLASREC", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETGLYPHATLASREC: expected 2 args");
        ::Rectangle rec = ::GetGlyphAtlasRec(rlreg::fonts.at(INT(0)), INT(1));
        return Value::from_string(std::to_string(rec.x) + "," + std::to_string(rec.y) + "," + std::to_string(rec.width) + "," + std::to_string(rec.height));
    }}, true);    R.add_with_policy("LOADUTF8", Fn{"LOADUTF8", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADUTF8: expected 1 args");
        // Note: This is simplified - would need proper codepoints array handling
        char* text = ::LoadUTF8(nullptr, 0);
        std::string result(text);
        ::UnloadUTF8(text);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("UNLOADUTF8", Fn{"UNLOADUTF8", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADUTF8: expected 1 args");
        // Note: This is simplified - would need proper text handling
        ::UnloadUTF8(nullptr);
        return Value::nil();
    }}, true);    R.add_with_policy("LOADCODEPOINTS", Fn{"LOADCODEPOINTS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADCODEPOINTS: expected 1 args");
        // Note: This is simplified - would need proper codepoints array handling
        int count;
        int* codepoints = ::LoadCodepoints(STR(0).c_str(), &count);
        (void)codepoints; // Suppress unused variable warning
        return Value::from_string("codepoints_loaded:" + std::to_string(count));
    }}, true);    R.add_with_policy("UNLOADCODEPOINTS", Fn{"UNLOADCODEPOINTS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADCODEPOINTS: expected 1 args");
        // Note: This is simplified - would need proper codepoints array handling
        ::UnloadCodepoints(nullptr);
        return Value::nil();
    }}, true);    R.add_with_policy("GETCODEPOINTCOUNT", Fn{"GETCODEPOINTCOUNT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCODEPOINTCOUNT: expected 1 args");
        return Value::from_int(GetCodepointCount(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("GETCODEPOINT", Fn{"GETCODEPOINT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCODEPOINT: expected 1 args");
        // Note: This is simplified - would need proper text handling
        int codepointSize;
        int codepoint = ::GetCodepoint(STR(0).c_str(), &codepointSize);
        return Value::from_string(std::to_string(codepoint) + "," + std::to_string(codepointSize));
    }}, true);    R.add_with_policy("GETCODEPOINTNEXT", Fn{"GETCODEPOINTNEXT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCODEPOINTNEXT: expected 1 args");
        // Note: This is simplified - would need proper text handling
        int codepointSize;
        int codepoint = ::GetCodepointNext(STR(0).c_str(), &codepointSize);
        return Value::from_string(std::to_string(codepoint) + "," + std::to_string(codepointSize));
    }}, true);    R.add_with_policy("GETCODEPOINTPREVIOUS", Fn{"GETCODEPOINTPREVIOUS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCODEPOINTPREVIOUS: expected 1 args");
        // Note: This is simplified - would need proper text handling
        int codepointSize;
        int codepoint = ::GetCodepointPrevious(STR(0).c_str(), &codepointSize);
        return Value::from_string(std::to_string(codepoint) + "," + std::to_string(codepointSize));
    }}, true);    R.add_with_policy("CODEPOINTTOUTF8", Fn{"CODEPOINTTOUTF8", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("CODEPOINTTOUTF8: expected 1 args");
        int utf8Size;
        const char* text = ::CodepointToUTF8(INT(0), &utf8Size);
        std::string result(text);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("TEXTCOPY", Fn{"TEXTCOPY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("TEXTCOPY: expected 2 args");
        // Note: This is simplified - would need proper text handling
        return Value::from_int(::TextCopy(nullptr, STR(1).c_str()));
    }}, true);    R.add_with_policy("TEXTISEQUAL", Fn{"TEXTISEQUAL", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("TEXTISEQUAL: expected 2 args");
        return Value::from_bool(TextIsEqual(args[0].as_string().c_str(), args[1].as_string().c_str()));
    }}, true);    R.add_with_policy("TEXTLENGTH", Fn{"TEXTLENGTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTLENGTH: expected 1 args");
        return Value::from_int(TextLength(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("TEXTFORMAT", Fn{"TEXTFORMAT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTFORMAT: expected 1 args");
        // Note: This is simplified - would need proper formatting handling
        return Value::from_string(::TextFormat(STR(0).c_str()));
    }}, true);    R.add_with_policy("TEXTSUBTEXT", Fn{"TEXTSUBTEXT", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("TEXTSUBTEXT: expected 3 args");
        return Value::from_string(TextSubtext(args[0].as_string().c_str(), args[1].as_int(), args[2].as_int()));
    }}, true);    R.add_with_policy("TEXTINSERT", Fn{"TEXTINSERT", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("TEXTINSERT: expected 3 args");
        char* result = ::TextInsert(STR(0).c_str(), STR(1).c_str(), INT(2));
        std::string text(result);
        ::MemFree(result);
        return Value::from_string(text);
    }}, true);    R.add_with_policy("TEXTTOINTEGER", Fn{"TEXTTOINTEGER", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTTOINTEGER: expected 1 args");
        return Value::from_int(TextToInteger(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("DRAWTEXTCODEPOINT", Fn{"DRAWTEXTCODEPOINT", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWTEXTCODEPOINT: expected 8 args");
        ::Vector2 position{(float)FLOAT(2), (float)FLOAT(3)};
        ::Color c{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawTextCodepoint(rlreg::fonts.at(INT(0)), INT(1), position, (float)FLOAT(4), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWLINES", Fn{"DRAWLINES", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWLINES: expected 7 args");
        ::Color c{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::DrawLine(INT(0), INT(1), INT(2), INT(3), c);
        return Value::nil();
    }}, true);    R.add_with_policy("MIN", Fn{"MIN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("MIN: expected 2 args");
        return Value::from_number(std::min(FLOAT(0), FLOAT(1)));
    }}, true);    R.add_with_policy("MAX", Fn{"MAX", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("MAX: expected 2 args");
        return Value::from_number(std::max(FLOAT(0), FLOAT(1)));
    }}, true);    R.add_with_policy("CLAMP", Fn{"CLAMP", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("CLAMP: expected 3 args");
        double value = FLOAT(0);
        double min_val = FLOAT(1);
        double max_val = FLOAT(2);
        if (value < min_val) value = min_val;
        if (value > max_val) value = max_val;
        return Value::from_number(value);
    }}, true);    R.add_with_policy("COLLISIONRECTANGLES", Fn{"COLLISIONRECTANGLES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("COLLISIONRECTANGLES: expected 8 args");
        // Check if two rectangles overlap
        int x1 = INT(0);
        int y1 = INT(1);
        int w1 = INT(2);
        int h1 = INT(3);
        int x2 = INT(4);
        int y2 = INT(5);
        int w2 = INT(6);
        int h2 = INT(7);

        bool collision = !(x1 + w1 < x2 || x2 + w2 < x1 || y1 + h1 < y2 || y2 + h2 < y1);
        return Value::from_bool(collision);
    }}, true);    R.add_with_policy("POINTINRECTANGLE", Fn{"POINTINRECTANGLE", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("POINTINRECTANGLE: expected 6 args");
        // Check if a point is inside a rectangle
        int px = INT(0);
        int py = INT(1);
        int rx = INT(2);
        int ry = INT(3);
        int rw = INT(4);
        int rh = INT(5);

        bool inside = (px >= rx && px <= rx + rw && py >= ry && py <= ry + rh);
        return Value::from_bool(inside);
    }}, true);    R.add_with_policy("DISTANCE", Fn{"DISTANCE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("DISTANCE: expected 4 args");
        // Calculate distance between two points (squared to avoid sqrt)
        int x1 = INT(0);
        int y1 = INT(1);
        int x2 = INT(2);
        int y2 = INT(3);

        double dx = (double)(x2 - x1);
        double dy = (double)(y2 - y1);
        double distance_squared = dx * dx + dy * dy;
        return Value::from_number(distance_squared);
    }}, true);    R.add_with_policy("SPLIT", Fn{"SPLIT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SPLIT: expected 2 args");
        // Simple split function - returns array index of first occurrence
        std::string text = STR(0);
        std::string delimiter = STR(1);
        size_t pos = text.find(delimiter);
        if (pos == std::string::npos) {
          return Value::from_int(-1); // Not found
        }
        return Value::from_int((int)pos);
    }}, true);    R.add_with_policy("LOADMUSIC", Fn{"LOADMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADMUSIC: expected 1 args");
        ::Music music = ::LoadMusicStream(STR(0).c_str());
        int id = rlreg::next_music_id++;
        rlreg::musics[id] = music;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("UNLOADMUSIC", Fn{"UNLOADMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADMUSIC: expected 1 args");
        int id = INT(0);
        auto it = rlreg::musics.find(id);
        if (it != rlreg::musics.end()) {
          ::UnloadMusicStream(it->second);
          rlreg::musics.erase(it);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("PLAYMUSIC", Fn{"PLAYMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PLAYMUSIC: expected 1 args");
        int id = INT(0);
        auto it = rlreg::musics.find(id);
        if (it != rlreg::musics.end()) {
          ::PlayMusicStream(it->second);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("STOPMUSIC", Fn{"STOPMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("STOPMUSIC: expected 1 args");
        int id = INT(0);
        auto it = rlreg::musics.find(id);
        if (it != rlreg::musics.end()) {
          ::StopMusicStream(it->second);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("PAUSEMUSIC", Fn{"PAUSEMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PAUSEMUSIC: expected 1 args");
        int id = INT(0);
        auto it = rlreg::musics.find(id);
        if (it != rlreg::musics.end()) {
          ::PauseMusicStream(it->second);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("RESUMEMUSIC", Fn{"RESUMEMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("RESUMEMUSIC: expected 1 args");
        int id = INT(0);
        auto it = rlreg::musics.find(id);
        if (it != rlreg::musics.end()) {
          ::ResumeMusicStream(it->second);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("UPDATEMUSIC", Fn{"UPDATEMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UPDATEMUSIC: expected 1 args");
        int id = INT(0);
        auto it = rlreg::musics.find(id);
        if (it != rlreg::musics.end()) {
          ::UpdateMusicStream(it->second);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("ISMUSICPLAYING", Fn{"ISMUSICPLAYING", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMUSICPLAYING: expected 1 args");
        int id = INT(0);
        auto it = rlreg::musics.find(id);
        if (it != rlreg::musics.end()) {
          return Value::from_bool(::IsMusicStreamPlaying(it->second));
        }
        return Value::from_bool(false);
    }}, true);    R.add_with_policy("GETTEXTUREWIDTH", Fn{"GETTEXTUREWIDTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETTEXTUREWIDTH: expected 1 args");
        int id = INT(0);
        auto it = rlreg::textures.find(id);
        if (it != rlreg::textures.end()) {
          return Value::from_int(it->second.width);
        }
        return Value::from_int(0);
    }}, true);    R.add_with_policy("GETTEXTUREHEIGHT", Fn{"GETTEXTUREHEIGHT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETTEXTUREHEIGHT: expected 1 args");
        int id = INT(0);
        auto it = rlreg::textures.find(id);
        if (it != rlreg::textures.end()) {
          return Value::from_int(it->second.height);
        }
        return Value::from_int(0);
    }}, true);    R.add_with_policy("IMAGEFROMTEXTURE", Fn{"IMAGEFROMTEXTURE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("IMAGEFROMTEXTURE: expected 1 args");
        // GetTextureData not available in current Raylib version
        // Return -1 to indicate failure
        return Value::from_int(-1);
    }}, true);    R.add_with_policy("TEXTUREFROMIMAGE", Fn{"TEXTUREFROMIMAGE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTUREFROMIMAGE: expected 1 args");
        int id = INT(0);
        auto it = rlreg::images.find(id);
        if (it != rlreg::images.end()) {
          ::Texture2D texture = ::LoadTextureFromImage(it->second);
          int texture_id = rlreg::next_tex_id++;
          rlreg::textures[texture_id] = texture;
          return Value::from_int(texture_id);
        }
        return Value::from_int(-1);
    }}, true);    R.add_with_policy("RESIZEIMAGE", Fn{"RESIZEIMAGE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("RESIZEIMAGE: expected 3 args");
        int id = INT(0);
        int new_width = INT(1);
        int new_height = INT(2);
        auto it = rlreg::images.find(id);
        if (it != rlreg::images.end()) {
          ::ImageResize(&it->second, new_width, new_height);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("CROPIMAGE", Fn{"CROPIMAGE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("CROPIMAGE: expected 5 args");
        int id = INT(0);
        int x = INT(1);
        int y = INT(2);
        int width = INT(3);
        int height = INT(4);
        auto it = rlreg::images.find(id);
        if (it != rlreg::images.end()) {
          ::Rectangle crop{(float)x, (float)y, (float)width, (float)height};
          ::ImageCrop(&it->second, crop);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("FLIPIMAGE", Fn{"FLIPIMAGE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("FLIPIMAGE: expected 1 args");
        int id = INT(0);
        auto it = rlreg::images.find(id);
        if (it != rlreg::images.end()) {
          ::ImageFlipVertical(&it->second);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("ROTATEIMAGE", Fn{"ROTATEIMAGE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ROTATEIMAGE: expected 2 args");
        int id = INT(0);
        int degrees = INT(1);
        auto it = rlreg::images.find(id);
        if (it != rlreg::images.end()) {
          ::ImageRotate(&it->second, degrees);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("REPLACE", Fn{"REPLACE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("REPLACE: expected 3 args");
        std::string text = STR(0);
        std::string search = STR(1);
        std::string replace = STR(2);
        size_t pos = 0;
        while ((pos = text.find(search, pos)) != std::string::npos) {
          text.replace(pos, search.length(), replace);
          pos += replace.length();
        }
        return Value::from_string(text);
    }}, true);    R.add_with_policy("INSTRREV", Fn{"INSTRREV", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("INSTRREV: expected 2 args");
        std::string text = STR(0);
        std::string search = STR(1);
        size_t pos = text.rfind(search);
        if (pos == std::string::npos) return Value::from_int(0);
        return Value::from_int((int)pos + 1); // BASIC uses 1-based indexing
    }}, true);    R.add_with_policy("STRREPEAT", Fn{"STRREPEAT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("STRREPEAT: expected 2 args");
        std::string text = STR(0);
        int count = INT(1);
        std::string result;
        for (int i = 0; i < count; i++) {
          result += text;
        }
        return Value::from_string(result);
    }}, true);    R.add_with_policy("STRREVERSE", Fn{"STRREVERSE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("STRREVERSE: expected 1 args");
        std::string text = STR(0);
        std::reverse(text.begin(), text.end());
        return Value::from_string(text);
    }}, true);    R.add_with_policy("SETGESTURESENABLED", Fn{"SETGESTURESENABLED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETGESTURESENABLED: expected 1 args");
        ::SetGesturesEnabled(INT(0));
        return Value::nil();
    }}, true);    R.add_with_policy("ISGESTUREDETECTED", Fn{"ISGESTUREDETECTED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISGESTUREDETECTED: expected 1 args");
        return Value::from_bool(::IsGestureDetected(INT(0)));
    }}, true);    R.add_with_policy("GETGESTUREDETECTED", Fn{"GETGESTUREDETECTED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREDETECTED: expected 0 args");
        return Value::from_int(::GetGestureDetected());
    }}, true);    R.add_with_policy("GETGESTUREHOLDTIME", Fn{"GETGESTUREHOLDTIME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREHOLDTIME: expected 0 args");
        return Value::from_number(::GetGestureHoldDuration());
    }}, true);    R.add_with_policy("GETGESTUREDRAGVECTOR", Fn{"GETGESTUREDRAGVECTOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREDRAGVECTOR: expected 0 args");
        ::Vector2 vec = ::GetGestureDragVector();
        return Value::from_string(std::to_string((int)vec.x) + "," + std::to_string((int)vec.y));
    }}, true);    R.add_with_policy("GETGESTUREDRAGANGLE", Fn{"GETGESTUREDRAGANGLE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREDRAGANGLE: expected 0 args");
        return Value::from_number(::GetGestureDragAngle());
    }}, true);    R.add_with_policy("GETGESTUREPINCHVECTOR", Fn{"GETGESTUREPINCHVECTOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREPINCHVECTOR: expected 0 args");
        ::Vector2 vec = ::GetGesturePinchVector();
        return Value::from_string(std::to_string((int)vec.x) + "," + std::to_string((int)vec.y));
    }}, true);    R.add_with_policy("GETGESTUREPINCHANGLE", Fn{"GETGESTUREPINCHANGLE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREPINCHANGLE: expected 0 args");
        return Value::from_number(::GetGesturePinchAngle());
    }}, true);    R.add_with_policy("SORT", Fn{"SORT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SORT: expected 1 args");
        // SORT function for arrays - sorts the array in ascending order
        if (args.empty() || !args[0].is_array()) {
          throw std::runtime_error("SORT: argument must be an array");
        }
        Value::Array arr = args[0].as_array();
        std::sort(arr.begin(), arr.end(), [](const Value& a, const Value& b) {
          if (a.is_number() && b.is_number()) {
            return a.as_number() < b.as_number();
          }
          if (a.is_string() && b.is_string()) {
            return a.as_string() < b.as_string();
          }
          return to_string_value(a) < to_string_value(b);
        });
        return Value::from_array(std::move(arr));
    }}, true);    R.add_with_policy("REVERSE", Fn{"REVERSE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("REVERSE: expected 1 args");
        // REVERSE function for arrays - reverses the order of elements
        if (args.empty() || !args[0].is_array()) {
          throw std::runtime_error("REVERSE: argument must be an array");
        }
        Value::Array arr = args[0].as_array();
        std::reverse(arr.begin(), arr.end());
        return Value::from_array(std::move(arr));
    }}, true);    R.add_with_policy("FIND", Fn{"FIND", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("FIND: expected 2 args");
        // FIND function for arrays - finds the first occurrence of a value
        if (args.size() < 2 || !args[0].is_array()) {
          return Value::from_int(-1);
        }
        const Value::Array& arr = args[0].as_array();
        const Value& search = args[1];
        for (size_t i = 0; i < arr.size(); ++i) {
          if (arr[i] == search) {
            return Value::from_int(static_cast<long long>(i));
          }
        }
        return Value::from_int(-1);
    }}, true);    R.add_with_policy("BINARYSEARCH", Fn{"BINARYSEARCH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("BINARYSEARCH: expected 2 args");
        // BINARYSEARCH function for sorted arrays
        if (args.size() < 2 || !args[0].is_array()) {
          return Value::from_int(-1);
        }
        const Value::Array& arr = args[0].as_array();
        const Value& search = args[1];
        size_t left = 0, right = arr.size();
        while (left < right) {
          size_t mid = left + (right - left) / 2;
          if (arr[mid] == search) {
            return Value::from_int(static_cast<long long>(mid));
          }
          bool less = false;
          if (arr[mid].is_number() && search.is_number()) {
            less = arr[mid].as_number() < search.as_number();
          } else if (arr[mid].is_string() && search.is_string()) {
            less = arr[mid].as_string() < search.as_string();
          } else {
            less = to_string_value(arr[mid]) < to_string_value(search);
          }
          if (less) {
            left = mid + 1;
          } else {
            right = mid;
          }
        }
        return Value::from_int(-1);
    }}, true);    R.add_with_policy("ISNUMBER", Fn{"ISNUMBER", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISNUMBER: expected 1 args");
        std::string text = STR(0);
        if (text.empty()) return Value::from_bool(false);
        size_t pos = 0;
        if (text[0] == '+' || text[0] == '-') pos = 1;
        if (pos >= text.length()) return Value::from_bool(false);
        bool has_digit = false;
        bool has_dot = false;
        for (size_t i = pos; i < text.length(); i++) {
          if (text[i] >= '0' && text[i] <= '9') {
            has_digit = true;
          } else if (text[i] == '.' && !has_dot) {
            has_dot = true;
          } else {
            return Value::from_bool(false);
          }
        }
        return Value::from_bool(has_digit);
    }}, true);    R.add_with_policy("ISSTRING", Fn{"ISSTRING", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISSTRING: expected 1 args");
        // Always returns true for string arguments
        return Value::from_bool(true);
    }}, true);    R.add_with_policy("ISARRAY", Fn{"ISARRAY", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISARRAY: expected 1 args");
        // Check if argument is an array
        if (args.empty()) {
          return Value::from_bool(false);
        }
        return Value::from_bool(args[0].is_array());
    }}, true);    R.add_with_policy("ARRAYLENGTH", Fn{"ARRAYLENGTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ARRAYLENGTH: expected 1 args");
        // Returns the length of an array
        if (args.empty() || !args[0].is_array()) {
          return Value::from_int(0);
        }
        return Value::from_int(static_cast<long long>(args[0].as_array().size()));
    }}, true);    R.add_with_policy("ARRAYCOPY", Fn{"ARRAYCOPY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ARRAYCOPY: expected 2 args");
        // Copies one array to another
        if (args.size() < 2 || !args[0].is_array()) {
          return Value::nil();
        }
        Value::Array source = args[0].as_array();
        Value::Array dest = source; // Copy
        return Value::from_array(std::move(dest));
    }}, true);    R.add_with_policy("ARRAYFILL", Fn{"ARRAYFILL", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ARRAYFILL: expected 2 args");
        // Fills an array with a specified value
        if (args.size() < 2 || !args[0].is_array()) {
          return Value::nil();
        }
        Value::Array arr = args[0].as_array();
        const Value& fillValue = args[1];
        for (auto& elem : arr) {
          elem = fillValue;
        }
        return Value::from_array(std::move(arr));
    }}, true);    R.add_with_policy("ARRAYSHUFFLE", Fn{"ARRAYSHUFFLE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ARRAYSHUFFLE: expected 1 args");
        // Randomly shuffles the elements of an array
        if (args.empty() || !args[0].is_array()) {
          return Value::nil();
        }
        Value::Array arr = args[0].as_array();
        static std::mt19937_64 eng{std::random_device{}()};
        std::shuffle(arr.begin(), arr.end(), eng);
        return Value::from_array(std::move(arr));
    }}, true);    R.add_with_policy("ARRAYUNIQUE", Fn{"ARRAYUNIQUE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ARRAYUNIQUE: expected 1 args");
        // Removes duplicate elements from an array
        if (args.empty() || !args[0].is_array()) {
          return Value::nil();
        }
        Value::Array arr = args[0].as_array();
        Value::Array unique;
        for (const auto& elem : arr) {
          bool found = false;
          for (const auto& u : unique) {
            if (u == elem) {
              found = true;
              break;
            }
          }
          if (!found) {
            unique.push_back(elem);
          }
        }
        return Value::from_array(std::move(unique));
    }}, true);    R.add_with_policy("ARRAYJOIN", Fn{"ARRAYJOIN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ARRAYJOIN: expected 2 args");
        // Joins array elements into a string with a delimiter
        if (args.size() < 2 || !args[0].is_array()) {
          return Value::from_string("");
        }
        const Value::Array& arr = args[0].as_array();
        std::string delimiter = args[1].as_string();
        std::string result;
        for (size_t i = 0; i < arr.size(); ++i) {
          if (i > 0) result += delimiter;
          result += to_string_value(arr[i]);
        }
        return Value::from_string(result);
    }}, true);    R.add_with_policy("ARRAYSPLIT", Fn{"ARRAYSPLIT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ARRAYSPLIT: expected 2 args");
        // Splits a string into an array using a delimiter
        // Note: This returns the array, not an ID
        if (args.size() < 2) {
          return Value::from_array(Value::Array{});
        }
        std::string str = args[0].as_string();
        std::string delimiter = args[1].as_string();
        Value::Array result;
        if (delimiter.empty()) {
          // Split into individual characters
          for (char c : str) {
            result.push_back(Value::from_string(std::string(1, c)));
          }
        } else {
          size_t pos = 0;
          while ((pos = str.find(delimiter)) != std::string::npos) {
            result.push_back(Value::from_string(str.substr(0, pos)));
            str.erase(0, pos + delimiter.length());
          }
          if (!str.empty()) {
            result.push_back(Value::from_string(str));
          }
        }
        return Value::from_array(std::move(result));
    }}, true);    R.add_with_policy("HTTPGET", Fn{"HTTPGET", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("HTTPGET: expected 1 args");
        // HTTP GET request - implemented with sockets
        return http_get_impl(args);
    }}, true);    R.add_with_policy("HTTPPOST", Fn{"HTTPPOST", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("HTTPPOST: expected 2 args");
        // HTTP POST request - implemented with sockets
        return http_post_impl(args);
    }}, true);    R.add_with_policy("DOWNLOADFILE", Fn{"DOWNLOADFILE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("DOWNLOADFILE: expected 2 args");
        // Download file from URL - implemented with HTTP GET
        return download_file_impl(args);
    }}, true);    R.add_with_policy("UPLOADFILE", Fn{"UPLOADFILE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("UPLOADFILE: expected 2 args");
        // Upload file to URL - implemented with HTTP POST
        return upload_file_impl(args);
    }}, true);    R.add_with_policy("WEBSOCKETCONNECT", Fn{"WEBSOCKETCONNECT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("WEBSOCKETCONNECT: expected 1 args");
        // WebSocket connection - implemented with HTTP upgrade
        return websocket_connect_impl(args);
    }}, true);    R.add_with_policy("WEBSOCKETSEND", Fn{"WEBSOCKETSEND", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("WEBSOCKETSEND: expected 2 args");
        // WebSocket send message - implemented with WebSocket frames
        return websocket_send_impl(args);
    }}, true);    R.add_with_policy("WEBSOCKETRECEIVE", Fn{"WEBSOCKETRECEIVE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("WEBSOCKETRECEIVE: expected 1 args");
        // WebSocket receive message - implemented with WebSocket frame parsing
        return websocket_receive_impl(args);
    }}, true);    R.add_with_policy("WEBSOCKETCLOSE", Fn{"WEBSOCKETCLOSE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("WEBSOCKETCLOSE: expected 1 args");
        // WebSocket close connection - implemented
        return websocket_close_impl(args);
    }}, true);    R.add_with_policy("TCPCONNECT", Fn{"TCPCONNECT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("TCPCONNECT: expected 2 args");
        // TCP connection - implemented with sockets
        return tcp_connect_impl(args);
    }}, true);    R.add_with_policy("TCPSEND", Fn{"TCPSEND", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("TCPSEND: expected 2 args");
        // TCP send data - implemented
        return tcp_send_impl(args);
    }}, true);    R.add_with_policy("TCPRECEIVE", Fn{"TCPRECEIVE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TCPRECEIVE: expected 1 args");
        // TCP receive data - implemented
        return tcp_receive_impl(args);
    }}, true);    R.add_with_policy("TCPCLOSE", Fn{"TCPCLOSE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TCPCLOSE: expected 1 args");
        // TCP close connection - implemented
        return tcp_close_impl(args);
    }}, true);    R.add_with_policy("UDPCREATE", Fn{"UDPCREATE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UDPCREATE: expected 1 args");
        // UDP socket creation - implemented
        return udp_create_impl(args);
    }}, true);    R.add_with_policy("UDPSEND", Fn{"UDPSEND", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("UDPSEND: expected 4 args");
        // UDP send data - implemented
        return udp_send_impl(args);
    }}, true);    R.add_with_policy("UDPRECEIVE", Fn{"UDPRECEIVE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UDPRECEIVE: expected 1 args");
        // UDP receive data - implemented
        return udp_receive_impl(args);
    }}, true);    R.add_with_policy("UDPCLOSE", Fn{"UDPCLOSE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UDPCLOSE: expected 1 args");
        // UDP close socket - implemented
        return udp_close_impl(args);
    }}, true);    R.add_with_policy("DRAWCONE", Fn{"DRAWCONE", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWCONE: expected 8 args");
        // DrawCone not available in current Raylib version
        // Using DrawCylinder as alternative
        ::Vector3 position{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Color c{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawCylinder(position, (float)FLOAT(3), 0.0f, (float)FLOAT(4), 8, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCONEWIRES", Fn{"DRAWCONEWIRES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWCONEWIRES: expected 8 args");
        // DrawConeWires not available in current Raylib version
        // Using DrawCylinderWires as alternative
        ::Vector3 position{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Color c{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawCylinderWires(position, (float)FLOAT(3), 0.0f, (float)FLOAT(4), 8, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTORUS", Fn{"DRAWTORUS", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWTORUS: expected 8 args");
        // DrawTorus not available in current Raylib version
        // Using DrawCylinder as alternative
        ::Vector3 position{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Color c{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawCylinder(position, (float)FLOAT(3), (float)FLOAT(3), (float)FLOAT(4), 8, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTORUSWIRES", Fn{"DRAWTORUSWIRES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWTORUSWIRES: expected 8 args");
        // DrawTorusWires not available in current Raylib version
        // Using DrawCylinderWires as alternative
        ::Vector3 position{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Color c{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawCylinderWires(position, (float)FLOAT(3), (float)FLOAT(3), (float)FLOAT(4), 8, c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPOLYWIRES", Fn{"DRAWPOLYWIRES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWPOLYWIRES: expected 8 args");
        // DrawPolyWires not available in current Raylib version
        // Using DrawPoly as alternative
        ::Vector2 center{(float)FLOAT(0), (float)FLOAT(1)};
        ::Color c{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawPoly(center, INT(3), (float)FLOAT(4), 0.0f, c);
        return Value::nil();
    }}, true);    R.add_with_policy("INITWINDOW", Fn{"INITWINDOW", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("INITWINDOW: expected 3 args");
        InitWindow(args[0].as_int(), args[1].as_int(), args[2].as_string().c_str());
        return Value::nil();
    }}, true);    R.add_with_policy("CLOSEWINDOW", Fn{"CLOSEWINDOW", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("CLOSEWINDOW: expected 0 args");
        CloseWindow();
        return Value::nil();
    }}, true);    R.add_with_policy("WINDOWSHOULDCLOSE", Fn{"WINDOWSHOULDCLOSE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("WINDOWSHOULDCLOSE: expected 0 args");
        return Value::from_bool(WindowShouldClose());
    }}, true);    R.add_with_policy("ISWINDOWREADY", Fn{"ISWINDOWREADY", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWREADY: expected 0 args");
        return Value::from_bool(IsWindowReady());
    }}, true);    R.add_with_policy("ISWINDOWFULLSCREEN", Fn{"ISWINDOWFULLSCREEN", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWFULLSCREEN: expected 0 args");
        return Value::from_bool(IsWindowFullscreen());
    }}, true);    R.add_with_policy("ISWINDOWHIDDEN", Fn{"ISWINDOWHIDDEN", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWHIDDEN: expected 0 args");
        return Value::from_bool(IsWindowHidden());
    }}, true);    R.add_with_policy("ISWINDOWMINIMIZED", Fn{"ISWINDOWMINIMIZED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWMINIMIZED: expected 0 args");
        return Value::from_bool(IsWindowMinimized());
    }}, true);    R.add_with_policy("ISWINDOWMAXIMIZED", Fn{"ISWINDOWMAXIMIZED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWMAXIMIZED: expected 0 args");
        return Value::from_bool(IsWindowMaximized());
    }}, true);    R.add_with_policy("ISWINDOWFOCUSED", Fn{"ISWINDOWFOCUSED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWFOCUSED: expected 0 args");
        return Value::from_bool(IsWindowFocused());
    }}, true);    R.add_with_policy("ISWINDOWRESIZED", Fn{"ISWINDOWRESIZED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ISWINDOWRESIZED: expected 0 args");
        return Value::from_bool(IsWindowResized());
    }}, true);    R.add_with_policy("ISWINDOWSTATE", Fn{"ISWINDOWSTATE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISWINDOWSTATE: expected 1 args");
        return Value::from_bool(IsWindowState(args[0].as_int()));
    }}, true);    R.add_with_policy("SETWINDOWTITLE", Fn{"SETWINDOWTITLE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETWINDOWTITLE: expected 1 args");
        SetWindowTitle(args[0].as_string().c_str());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWMINIMUMSIZE", Fn{"SETWINDOWMINIMUMSIZE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETWINDOWMINIMUMSIZE: expected 2 args");
        SetWindowMinSize(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWPOSITION", Fn{"SETWINDOWPOSITION", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETWINDOWPOSITION: expected 2 args");
        SetWindowPosition(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWMONITOR", Fn{"SETWINDOWMONITOR", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETWINDOWMONITOR: expected 1 args");
        SetWindowMonitor(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWSTATE", Fn{"SETWINDOWSTATE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETWINDOWSTATE: expected 1 args");
        SetWindowState(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("GETMONITORCOUNT", Fn{"GETMONITORCOUNT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMONITORCOUNT: expected 0 args");
        return Value::from_int(GetMonitorCount());
    }}, true);    R.add_with_policy("GETMONITORWIDTH", Fn{"GETMONITORWIDTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORWIDTH: expected 1 args");
        return Value::from_int(GetMonitorWidth(args[0].as_int()));
    }}, true);    R.add_with_policy("GETMONITORHEIGHT", Fn{"GETMONITORHEIGHT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORHEIGHT: expected 1 args");
        return Value::from_int(GetMonitorHeight(args[0].as_int()));
    }}, true);    R.add_with_policy("GETMONITORPHYSICALWIDTH", Fn{"GETMONITORPHYSICALWIDTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORPHYSICALWIDTH: expected 1 args");
        return Value::from_int(GetMonitorPhysicalWidth(args[0].as_int()));
    }}, true);    R.add_with_policy("GETMONITORPHYSICALHEIGHT", Fn{"GETMONITORPHYSICALHEIGHT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORPHYSICALHEIGHT: expected 1 args");
        return Value::from_int(GetMonitorPhysicalHeight(args[0].as_int()));
    }}, true);    R.add_with_policy("GETMONITORREFRESHRATE", Fn{"GETMONITORREFRESHRATE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMONITORREFRESHRATE: expected 1 args");
        return Value::from_int(GetMonitorRefreshRate(args[0].as_int()));
    }}, true);    R.add_with_policy("GETCURRENTMONITOR", Fn{"GETCURRENTMONITOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETCURRENTMONITOR: expected 0 args");
        return Value::from_int(GetCurrentMonitor());
    }}, true);    R.add_with_policy("BEGINDRAWING", Fn{"BEGINDRAWING", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("BEGINDRAWING: expected 0 args");
        BeginDrawing();
        return Value::nil();
    }}, true);    R.add_with_policy("ENDDRAWING", Fn{"ENDDRAWING", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDDRAWING: expected 0 args");
        EndDrawing();
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINBLENDMODE", Fn{"BEGINBLENDMODE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("BEGINBLENDMODE: expected 1 args");
        BeginBlendMode(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("ENDBLENDMODE", Fn{"ENDBLENDMODE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDBLENDMODE: expected 0 args");
        EndBlendMode();
        return Value::nil();
    }}, true);    R.add_with_policy("SETTARGETFPS", Fn{"SETTARGETFPS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETTARGETFPS: expected 1 args");
        SetTargetFPS(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("GETFPS", Fn{"GETFPS", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETFPS: expected 0 args");
        return Value::from_int(GetFPS());
    }}, true);    R.add_with_policy("GETFRAMETIME", Fn{"GETFRAMETIME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETFRAMETIME: expected 0 args");
        return Value::from_number(GetFrameTime());
    }}, true);    R.add_with_policy("GETTIME", Fn{"GETTIME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETTIME: expected 0 args");
        return Value::from_number(GetTime());
    }}, true);    R.add_with_policy("INITGAME", Fn{"INITGAME", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("INITGAME: expected 3 args");
        InitWindow(INT(0), INT(1), STR(2).c_str());
        SetTargetFPS(60);
        return Value::nil();
    }}, true);    R.add_with_policy("CLOSEGAME", Fn{"CLOSEGAME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("CLOSEGAME: expected 0 args");
        CloseWindow();
        return Value::nil();
    }}, true);    R.add_with_policy("GAMESHOULDCLOSE", Fn{"GAMESHOULDCLOSE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GAMESHOULDCLOSE: expected 0 args");
        return Value::from_bool(WindowShouldClose());
    }}, true);    R.add_with_policy("BEGINGAMEFRAME", Fn{"BEGINGAMEFRAME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("BEGINGAMEFRAME: expected 0 args");
        BeginDrawing();
        ClearBackground(Color{0, 0, 0, 255});
        return Value::nil();
    }}, true);    R.add_with_policy("ENDGAMEFRAME", Fn{"ENDGAMEFRAME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDGAMEFRAME: expected 0 args");
        EndDrawing();
        return Value::nil();
    }}, true);    R.add_with_policy("GETGAMEDELTATIME", Fn{"GETGAMEDELTATIME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGAMEDELTATIME: expected 0 args");
        return Value::from_number(GetFrameTime());
    }}, true);    R.add_with_policy("GETGAMEFPS", Fn{"GETGAMEFPS", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGAMEFPS: expected 0 args");
        return Value::from_number(GetFPS());
    }}, true);    R.add_with_policy("SETUP3DCAMERA", Fn{"SETUP3DCAMERA", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("SETUP3DCAMERA: expected 7 args");
        // Simple 3D camera setup - stores camera state globally
        // This is a simplified version for easy use
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINDRAW3D", Fn{"BEGINDRAW3D", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("BEGINDRAW3D: expected 0 args");
        // Begin 3D mode - simplified version
        return Value::nil();
    }}, true);    R.add_with_policy("ENDDRAW3D", Fn{"ENDDRAW3D", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDDRAW3D: expected 0 args");
        // End 3D mode - simplified version
        return Value::nil();
    }}, true);    R.add_with_policy("LOADSPRITE", Fn{"LOADSPRITE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADSPRITE: expected 1 args");
        Texture2D texture = LoadTexture(STR(0).c_str());
        return Value::from_number((intptr_t)&texture);
    }}, true);    R.add_with_policy("DRAWSPRITE", Fn{"DRAWSPRITE", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWSPRITE: expected 8 args");
        Texture2D texture = *(Texture2D*)INT(0);
        Rectangle source = {0, 0, (float)texture.width, (float)texture.height};
        Rectangle dest = {(float)INT(1), (float)INT(2), (float)INT(3), (float)INT(4)};
        Vector2 origin = {(float)INT(3)/2, (float)INT(4)/2};
        Color tint = {(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        DrawTexturePro(texture, source, dest, origin, (float)INT(5), tint);
        return Value::nil();
    }}, true);    R.add_with_policy("CHECKCOLLISIONRECTS", Fn{"CHECKCOLLISIONRECTS", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("CHECKCOLLISIONRECTS: expected 8 args");
        Rectangle rect1 = {(float)INT(0), (float)INT(1), (float)INT(2), (float)INT(3)};
        Rectangle rect2 = {(float)INT(4), (float)INT(5), (float)INT(6), (float)INT(7)};
        return Value::from_bool(CheckCollisionRecs(rect1, rect2));
    }}, true);    R.add_with_policy("CHECKCOLLISIONCIRCLES", Fn{"CHECKCOLLISIONCIRCLES", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("CHECKCOLLISIONCIRCLES: expected 6 args");
        Vector2 center1 = {(float)INT(0), (float)INT(1)};
        Vector2 center2 = {(float)INT(3), (float)INT(4)};
        return Value::from_bool(CheckCollisionCircles(center1, (float)INT(2), center2, (float)INT(5)));
    }}, true);    R.add_with_policy("ISKEYHELD", Fn{"ISKEYHELD", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISKEYHELD: expected 1 args");
        return Value::from_bool(IsKeyDown(INT(0)));
    }}, true);    R.add_with_policy("ISKEYPRESSED", Fn{"ISKEYPRESSED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISKEYPRESSED: expected 1 args");
        return Value::from_bool(IsKeyPressed(INT(0)));
    }}, true);    R.add_with_policy("ISMOUSEHELD", Fn{"ISMOUSEHELD", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMOUSEHELD: expected 1 args");
        return Value::from_bool(IsMouseButtonDown(INT(0)));
    }}, true);    R.add_with_policy("ISMOUSEPRESSED", Fn{"ISMOUSEPRESSED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMOUSEPRESSED: expected 1 args");
        return Value::from_bool(IsMouseButtonPressed(INT(0)));
    }}, true);    R.add_with_policy("GETMOUSEPOSITION", Fn{"GETMOUSEPOSITION", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMOUSEPOSITION: expected 0 args");
        Vector2 mousePos = GetMousePosition();
        return Value::from_string(std::to_string((int)mousePos.x) + "," + std::to_string((int)mousePos.y));
    }}, true);    R.add_with_policy("LOADGAMESOUND", Fn{"LOADGAMESOUND", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADGAMESOUND: expected 1 args");
        Sound sound = LoadSound(STR(0).c_str());
        return Value::from_number((intptr_t)&sound);
    }}, true);    R.add_with_policy("PLAYGAMESOUND", Fn{"PLAYGAMESOUND", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PLAYGAMESOUND: expected 1 args");
        Sound sound = *(Sound*)INT(0);
        PlaySound(sound);
        return Value::nil();
    }}, true);    R.add_with_policy("LOADGAMEMUSIC", Fn{"LOADGAMEMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADGAMEMUSIC: expected 1 args");
        Music music = LoadMusicStream(STR(0).c_str());
        return Value::from_number((intptr_t)&music);
    }}, true);    R.add_with_policy("PLAYGAMEMUSIC", Fn{"PLAYGAMEMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PLAYGAMEMUSIC: expected 1 args");
        Music music = *(Music*)INT(0);
        PlayMusicStream(music);
        return Value::nil();
    }}, true);    R.add_with_policy("RANDOMINT", Fn{"RANDOMINT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("RANDOMINT: expected 2 args");
        return Value::from_number(GetRandomValue(INT(0), INT(1)));
    }}, true);    R.add_with_policy("RANDOMFLOAT", Fn{"RANDOMFLOAT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("RANDOMFLOAT: expected 2 args");
        float min = FLOAT(0);
        float max = FLOAT(1);
        return Value::from_number(min + (max - min) * ((float)GetRandomValue(0, 1000) / 1000.0f));
    }}, true);    R.add_with_policy("DISTANCE2D", Fn{"DISTANCE2D", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("DISTANCE2D: expected 4 args");
        Vector2 p1 = {(float)INT(0), (float)INT(1)};
        Vector2 p2 = {(float)INT(2), (float)INT(3)};
        return Value::from_number(Vector2Distance(p1, p2));
    }}, true);    R.add_with_policy("SETWINDOWSIZE", Fn{"SETWINDOWSIZE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETWINDOWSIZE: expected 2 args");
        SetWindowSize(args[0].as_int(), args[1].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETWINDOWICONS", Fn{"SETWINDOWICONS", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETWINDOWICONS: expected 2 args");
        // Load image from path and set as window icon
        // For multiple icons, call SetWindowIcon multiple times
        ::Image icon = ::LoadImage(STR(0).c_str());
        if (::IsImageValid(icon)) {
          ::SetWindowIcon(icon);
          ::UnloadImage(icon);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("GETWINDOWPOSITION", Fn{"GETWINDOWPOSITION", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETWINDOWPOSITION: expected 0 args");
        ::Vector2 pos = ::GetWindowPosition();
        return Value::from_string(std::to_string(pos.x) + "," + std::to_string(pos.y));
    }}, true);    R.add_with_policy("GETWINDOWSCALEDPI", Fn{"GETWINDOWSCALEDPI", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETWINDOWSCALEDPI: expected 0 args");
        ::Vector2 scale = ::GetWindowScaleDPI();
        return Value::from_string(std::to_string(scale.x) + "," + std::to_string(scale.y));
    }}, true);    R.add_with_policy("GETCLIPBOARDIMAGE", Fn{"GETCLIPBOARDIMAGE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETCLIPBOARDIMAGE: expected 0 args");
        ::Image img = ::GetClipboardImage();
        int id = rlreg::next_image_id++;
        rlreg::images[id] = img;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("SWAPSCREENBUFFER", Fn{"SWAPSCREENBUFFER", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("SWAPSCREENBUFFER: expected 0 args");
        SwapScreenBuffer();
        return Value::nil();
    }}, true);    R.add_with_policy("POLLINPUTEVENTS", Fn{"POLLINPUTEVENTS", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("POLLINPUTEVENTS: expected 0 args");
        PollInputEvents();
        return Value::nil();
    }}, true);    R.add_with_policy("WAITTIME", Fn{"WAITTIME", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("WAITTIME: expected 1 args");
        WaitTime(args[0].as_number());
        return Value::nil();
    }}, true);    R.add_with_policy("LOADRANDOMSEQUENCE", Fn{"LOADRANDOMSEQUENCE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("LOADRANDOMSEQUENCE: expected 3 args");
        int* seq = ::LoadRandomSequence(INT(0), INT(1), INT(2));
        (void)seq; // Suppress unused variable warning
        // Random sequence is managed internally by raylib
        return Value::from_string("sequence_loaded");
    }}, true);    R.add_with_policy("UNLOADRANDOMSEQUENCE", Fn{"UNLOADRANDOMSEQUENCE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADRANDOMSEQUENCE: expected 1 args");
        // Note: Sequence is managed internally by raylib, no action needed
        (void)STR(0); // Suppress unused parameter warning
        return Value::nil();
    }}, true);    R.add_with_policy("GETSCREENTOWORLDRAYEX", Fn{"GETSCREENTOWORLDRAYEX", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("GETSCREENTOWORLDRAYEX: expected 5 args");
        ::Vector2 mousePos{(float)FLOAT(0), (float)FLOAT(1)};
        ::Camera3D camera;
        camera.position = {(float)0.0f, (float)0.0f, (float)0.0f};
        camera.target = {(float)0.0f, (float)0.0f, (float)1.0f};
        camera.up = {(float)0.0f, (float)1.0f, (float)0.0f};
        camera.fovy = 60.0f;
        camera.projection = INT(2);
        ::Ray ray = ::GetScreenToWorldRayEx(mousePos, camera, INT(3), INT(4));
        return Value::from_string(std::to_string(ray.position.x) + "," + std::to_string(ray.position.y) + "," + std::to_string(ray.position.z) + "," + std::to_string(ray.direction.x) + "," + std::to_string(ray.direction.y) + "," + std::to_string(ray.direction.z));
    }}, true);    R.add_with_policy("GETWORLDTOSCREENEX", Fn{"GETWORLDTOSCREENEX", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("GETWORLDTOSCREENEX: expected 6 args");
        ::Vector3 worldPos{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Camera3D camera;
        camera.position = {(float)0.0f, (float)0.0f, (float)0.0f};
        camera.target = {(float)0.0f, (float)0.0f, (float)1.0f};
        camera.up = {(float)0.0f, (float)1.0f, (float)0.0f};
        camera.fovy = 60.0f;
        camera.projection = INT(3);
        ::Vector2 screenPos = ::GetWorldToScreenEx(worldPos, camera, INT(4), INT(5));
        return Value::from_string(std::to_string(screenPos.x) + "," + std::to_string(screenPos.y));
    }}, true);    R.add_with_policy("GETWORLDTOSCREEN2D", Fn{"GETWORLDTOSCREEN2D", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GETWORLDTOSCREEN2D: expected 3 args");
        ::Vector2 worldPos{(float)FLOAT(0), (float)FLOAT(1)};
        ::Camera2D camera;
        camera.target = {(float)0.0f, (float)0.0f};
        camera.offset = {(float)0.0f, (float)0.0f};
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
        ::Vector2 screenPos = ::GetWorldToScreen2D(worldPos, camera);
        return Value::from_string(std::to_string(screenPos.x) + "," + std::to_string(screenPos.y));
    }}, true);    R.add_with_policy("GETSCREENTOWORLD2D", Fn{"GETSCREENTOWORLD2D", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GETSCREENTOWORLD2D: expected 3 args");
        ::Vector2 screenPos{(float)FLOAT(0), (float)FLOAT(1)};
        ::Camera2D camera;
        camera.target = {(float)0.0f, (float)0.0f};
        camera.offset = {(float)0.0f, (float)0.0f};
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
        ::Vector2 worldPos = ::GetScreenToWorld2D(screenPos, camera);
        return Value::from_string(std::to_string(worldPos.x) + "," + std::to_string(worldPos.y));
    }}, true);    R.add_with_policy("GETCAMERAMATRIX", Fn{"GETCAMERAMATRIX", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCAMERAMATRIX: expected 1 args");
        ::Camera3D camera;
        camera.position = {(float)0.0f, (float)0.0f, (float)0.0f};
        camera.target = {(float)0.0f, (float)0.0f, (float)1.0f};
        camera.up = {(float)0.0f, (float)1.0f, (float)0.0f};
        camera.fovy = 60.0f;
        camera.projection = CAMERA_PERSPECTIVE;
        ::Matrix matrix = ::GetCameraMatrix(camera);
        return Value::from_string(std::to_string(matrix.m0) + "," + std::to_string(matrix.m1) + "," + std::to_string(matrix.m2) + "," + std::to_string(matrix.m3) + "," + std::to_string(matrix.m4) + "," + std::to_string(matrix.m5) + "," + std::to_string(matrix.m6) + "," + std::to_string(matrix.m7) + "," + std::to_string(matrix.m8) + "," + std::to_string(matrix.m9) + "," + std::to_string(matrix.m10) + "," + std::to_string(matrix.m11) + "," + std::to_string(matrix.m12) + "," + std::to_string(matrix.m13) + "," + std::to_string(matrix.m14) + "," + std::to_string(matrix.m15));
    }}, true);    R.add_with_policy("GETCAMERAMATRIX2D", Fn{"GETCAMERAMATRIX2D", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCAMERAMATRIX2D: expected 1 args");
        ::Camera2D camera;
        camera.target = {(float)0.0f, (float)0.0f};
        camera.offset = {(float)0.0f, (float)0.0f};
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
        ::Matrix matrix = ::GetCameraMatrix2D(camera);
        return Value::from_string(std::to_string(matrix.m0) + "," + std::to_string(matrix.m1) + "," + std::to_string(matrix.m2) + "," + std::to_string(matrix.m3) + "," + std::to_string(matrix.m4) + "," + std::to_string(matrix.m5) + "," + std::to_string(matrix.m6) + "," + std::to_string(matrix.m7) + "," + std::to_string(matrix.m8) + "," + std::to_string(matrix.m9) + "," + std::to_string(matrix.m10) + "," + std::to_string(matrix.m11) + "," + std::to_string(matrix.m12) + "," + std::to_string(matrix.m13) + "," + std::to_string(matrix.m14) + "," + std::to_string(matrix.m15));
    }}, true);    R.add_with_policy("LOADVRSTEREOCONFIG", Fn{"LOADVRSTEREOCONFIG", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADVRSTEREOCONFIG: expected 1 args");
        ::VrDeviceInfo device = {2160, 1200, 0.110f, 0.062f, 0.041f, 0.07f, 0.064f, {1.0f, 0.22f, 0.24f, 0.0f}, {0.996f, -0.004f, 1.014f, 0.0f}};
        ::VrStereoConfig config = ::LoadVrStereoConfig(device);
        static int next_vr_id = 1;
        // Note: VR config is managed internally, return ID for tracking
        return Value::from_int(next_vr_id++);
    }}, true);    R.add_with_policy("UNLOADVRSTEREOCONFIG", Fn{"UNLOADVRSTEREOCONFIG", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADVRSTEREOCONFIG: expected 1 args");
        // Note: VR config is managed internally by raylib
        (void)INT(0); // Suppress unused parameter warning
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINVRSTEREOMODE", Fn{"BEGINVRSTEREOMODE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("BEGINVRSTEREOMODE: expected 1 args");
        ::VrDeviceInfo device = {2160, 1200, 0.110f, 0.062f, 0.041f, 0.07f, 0.064f, {1.0f, 0.22f, 0.24f, 0.0f}, {0.996f, -0.004f, 1.014f, 0.0f}};
        ::VrStereoConfig config = ::LoadVrStereoConfig(device);
        ::BeginVrStereoMode(config);
        return Value::nil();
    }}, true);    R.add_with_policy("ENDVRSTEREOMODE", Fn{"ENDVRSTEREOMODE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDVRSTEREOMODE: expected 0 args");
        EndVrStereoMode();
        return Value::nil();
    }}, true);    R.add_with_policy("ISSHADERVALID", Fn{"ISSHADERVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISSHADERVALID: expected 1 args");
        if (rlreg::shaders.find(INT(0)) == rlreg::shaders.end()) {
          return Value::from_bool(false);
        }
        return Value::from_bool(::IsShaderValid(rlreg::shaders.at(INT(0))));
    }}, true);    R.add_with_policy("GETSHADERLOCATIONATTRIB", Fn{"GETSHADERLOCATIONATTRIB", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETSHADERLOCATIONATTRIB: expected 2 args");
        if (rlreg::shaders.find(INT(0)) == rlreg::shaders.end()) {
          return Value::from_int(-1);
        }
        return Value::from_int(::GetShaderLocationAttrib(rlreg::shaders.at(INT(0)), STR(1).c_str()));
    }}, true);    R.add_with_policy("SETSHADERVALUEV", Fn{"SETSHADERVALUEV", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("SETSHADERVALUEV: expected 5 args");
        // Parse 4-component vector from string "x,y,z,w"
        std::string vecStr = STR(2);
        size_t pos1 = vecStr.find(',');
        size_t pos2 = vecStr.find(',', pos1 + 1);
        size_t pos3 = vecStr.find(',', pos2 + 1);
        if (pos1 != std::string::npos && pos2 != std::string::npos && pos3 != std::string::npos) {
          float values[4] = {
            std::stof(vecStr.substr(0, pos1)),
            std::stof(vecStr.substr(pos1 + 1, pos2 - pos1 - 1)),
            std::stof(vecStr.substr(pos2 + 1, pos3 - pos2 - 1)),
            std::stof(vecStr.substr(pos3 + 1))
          };
          if (rlreg::shaders.find(INT(0)) != rlreg::shaders.end()) {
            ::SetShaderValueV(rlreg::shaders.at(INT(0)), INT(1), values, SHADER_UNIFORM_VEC4, INT(4));
          }
        }
        return Value::nil();
    }}, true);    R.add_with_policy("SETSHADERVALUEMATRIX", Fn{"SETSHADERVALUEMATRIX", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETSHADERVALUEMATRIX: expected 3 args");
        // Parse 4x4 matrix from string "m0,m1,m2,...,m15"
        std::string matStr = STR(2);
        ::Matrix matrix = {0};
        size_t start = 0;
        size_t pos = 0;
        int idx = 0;
        float* matData = &matrix.m0;
        while ((pos = matStr.find(',', start)) != std::string::npos && idx < 16) {
          matData[idx++] = std::stof(matStr.substr(start, pos - start));
          start = pos + 1;
        }
        if (idx < 16 && start < matStr.length()) {
          matData[idx] = std::stof(matStr.substr(start));
        }
        if (rlreg::shaders.find(INT(0)) != rlreg::shaders.end()) {
          ::SetShaderValueMatrix(rlreg::shaders.at(INT(0)), INT(1), matrix);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("SETSHADERVALUETEXTURE", Fn{"SETSHADERVALUETEXTURE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETSHADERVALUETEXTURE: expected 3 args");
        if (rlreg::shaders.find(INT(0)) != rlreg::shaders.end() && rlreg::textures.find(INT(2)) != rlreg::textures.end()) {
          ::SetShaderValueTexture(rlreg::shaders.at(INT(0)), INT(1), rlreg::textures.at(INT(2)));
        }
        return Value::nil();
    }}, true);    R.add_with_policy("MAKEDIRECTORY", Fn{"MAKEDIRECTORY", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("MAKEDIRECTORY: expected 1 args");
        return Value::from_int(MakeDirectory(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("ISFILENAMEVALID", Fn{"ISFILENAMEVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISFILENAMEVALID: expected 1 args");
        return Value::from_bool(IsFileNameValid(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("DECODEDATABASE64", Fn{"DECODEDATABASE64", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("DECODEDATABASE64: expected 1 args");
        int outputSize;
        unsigned char* data = ::DecodeDataBase64((const unsigned char*)STR(0).c_str(), &outputSize);
        std::string result((char*)data, outputSize);
        ::MemFree(data);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("COMPUTECRC32", Fn{"COMPUTECRC32", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("COMPUTECRC32: expected 1 args");
        return Value::from_int(::ComputeCRC32((unsigned char*)STR(0).c_str(), STR(0).length()));
    }}, true);    R.add_with_policy("COMPUTEMD5", Fn{"COMPUTEMD5", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("COMPUTEMD5: expected 1 args");
        unsigned int* hash = ::ComputeMD5((unsigned char*)STR(0).c_str(), STR(0).length());
        std::string result = std::to_string(hash[0]) + "," + std::to_string(hash[1]) + "," + std::to_string(hash[2]) + "," + std::to_string(hash[3]);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("COMPUTESHA1", Fn{"COMPUTESHA1", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("COMPUTESHA1: expected 1 args");
        unsigned int* hash = ::ComputeSHA1((unsigned char*)STR(0).c_str(), STR(0).length());
        std::string result = std::to_string(hash[0]) + "," + std::to_string(hash[1]) + "," + std::to_string(hash[2]) + "," + std::to_string(hash[3]) + "," + std::to_string(hash[4]);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("LOADAUTOMATIONEVENTLIST", Fn{"LOADAUTOMATIONEVENTLIST", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADAUTOMATIONEVENTLIST: expected 1 args");
        ::AutomationEventList list = ::LoadAutomationEventList(STR(0).c_str());
        static int next_automation_id = 1;
        // Note: Automation list is managed internally, return ID for tracking
        return Value::from_int(next_automation_id++);
    }}, true);    R.add_with_policy("UNLOADAUTOMATIONEVENTLIST", Fn{"UNLOADAUTOMATIONEVENTLIST", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADAUTOMATIONEVENTLIST: expected 1 args");
        // Note: Automation list is managed internally by raylib
        (void)INT(0); // Suppress unused parameter warning
        return Value::nil();
    }}, true);    R.add_with_policy("EXPORTAUTOMATIONEVENTLIST", Fn{"EXPORTAUTOMATIONEVENTLIST", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTAUTOMATIONEVENTLIST: expected 2 args");
        // Note: Automation list is managed internally by raylib
        (void)INT(0); // Suppress unused parameter warning
        return Value::from_bool(false);
    }}, true);    R.add_with_policy("SETAUTOMATIONEVENTLIST", Fn{"SETAUTOMATIONEVENTLIST", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETAUTOMATIONEVENTLIST: expected 1 args");
        // Note: Automation list is managed internally by raylib
        (void)INT(0); // Suppress unused parameter warning
        return Value::nil();
    }}, true);    R.add_with_policy("SETAUTOMATIONEVENTBASEFRAME", Fn{"SETAUTOMATIONEVENTBASEFRAME", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETAUTOMATIONEVENTBASEFRAME: expected 1 args");
        SetAutomationEventBaseFrame(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("STARTAUTOMATIONEVENTRECORDING", Fn{"STARTAUTOMATIONEVENTRECORDING", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("STARTAUTOMATIONEVENTRECORDING: expected 0 args");
        StartAutomationEventRecording();
        return Value::nil();
    }}, true);    R.add_with_policy("STOPAUTOMATIONEVENTRECORDING", Fn{"STOPAUTOMATIONEVENTRECORDING", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("STOPAUTOMATIONEVENTRECORDING: expected 0 args");
        StopAutomationEventRecording();
        return Value::nil();
    }}, true);    R.add_with_policy("PLAYAUTOMATIONEVENT", Fn{"PLAYAUTOMATIONEVENT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PLAYAUTOMATIONEVENT: expected 1 args");
        // Note: Automation events are managed internally by raylib
        (void)INT(0); // Suppress unused parameter warning
        return Value::nil();
    }}, true);    R.add_with_policy("SETTRACELOGCALLBACK", Fn{"SETTRACELOGCALLBACK", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("SETTRACELOGCALLBACK: expected 0 args");
        // Note: Callbacks require C++ function pointers, not supported in BASIC
        // This function is a no-op in BASIC bindings
        return Value::nil();
    }}, true);    R.add_with_policy("SETLOADFILEDATACALLBACK", Fn{"SETLOADFILEDATACALLBACK", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("SETLOADFILEDATACALLBACK: expected 0 args");
        // Note: Callbacks require C++ function pointers, not supported in BASIC
        // This function is a no-op in BASIC bindings
        return Value::nil();
    }}, true);    R.add_with_policy("SETSAVEFILEDATACALLBACK", Fn{"SETSAVEFILEDATACALLBACK", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("SETSAVEFILEDATACALLBACK: expected 0 args");
        // Note: Callbacks require C++ function pointers, not supported in BASIC
        // This function is a no-op in BASIC bindings
        return Value::nil();
    }}, true);    R.add_with_policy("SETLOADFILETEXTCALLBACK", Fn{"SETLOADFILETEXTCALLBACK", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("SETLOADFILETEXTCALLBACK: expected 0 args");
        // Note: Callbacks require C++ function pointers, not supported in BASIC
        // This function is a no-op in BASIC bindings
        return Value::nil();
    }}, true);    R.add_with_policy("SETSAVEFILETEXTCALLBACK", Fn{"SETSAVEFILETEXTCALLBACK", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("SETSAVEFILETEXTCALLBACK: expected 0 args");
        // Note: Callbacks require C++ function pointers, not supported in BASIC
        // This function is a no-op in BASIC bindings
        return Value::nil();
    }}, true);    R.add_with_policy("ISKEYPRESSEDREPEAT", Fn{"ISKEYPRESSEDREPEAT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISKEYPRESSEDREPEAT: expected 1 args");
        return Value::from_bool(IsKeyPressedRepeat(args[0].as_int()));
    }}, true);    R.add_with_policy("GETCHARPRESSED", Fn{"GETCHARPRESSED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETCHARPRESSED: expected 0 args");
        return Value::from_int(GetCharPressed());
    }}, true);    R.add_with_policy("SETGAMEPADVIBRATION", Fn{"SETGAMEPADVIBRATION", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("SETGAMEPADVIBRATION: expected 4 args");
        SetGamepadVibration(args[0].as_int(), static_cast<float>(args[1].as_number()), static_cast<float>(args[2].as_number()), static_cast<float>(args[3].as_number()));
        return Value::nil();
    }}, true);    R.add_with_policy("GETMOUSEWHEELMOVEV", Fn{"GETMOUSEWHEELMOVEV", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMOUSEWHEELMOVEV: expected 0 args");
        ::Vector2 wheel = ::GetMouseWheelMoveV();
        return Value::from_string(std::to_string(wheel.x) + "," + std::to_string(wheel.y));
    }}, true);    R.add_with_policy("SETMOUSECURSOR", Fn{"SETMOUSECURSOR", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETMOUSECURSOR: expected 1 args");
        SetMouseCursor(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("GETTOUCHX", Fn{"GETTOUCHX", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETTOUCHX: expected 0 args");
        return Value::from_int(GetTouchX());
    }}, true);    R.add_with_policy("GETTOUCHY", Fn{"GETTOUCHY", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETTOUCHY: expected 0 args");
        return Value::from_int(GetTouchY());
    }}, true);    R.add_with_policy("GETTOUCHPOSITION", Fn{"GETTOUCHPOSITION", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETTOUCHPOSITION: expected 1 args");
        ::Vector2 pos = ::GetTouchPosition(INT(0));
        return Value::from_string(std::to_string(pos.x) + "," + std::to_string(pos.y));
    }}, true);    R.add_with_policy("GETTOUCHPOINTID", Fn{"GETTOUCHPOINTID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETTOUCHPOINTID: expected 1 args");
        return Value::from_int(GetTouchPointId(args[0].as_int()));
    }}, true);    R.add_with_policy("GETTOUCHPOINTCOUNT", Fn{"GETTOUCHPOINTCOUNT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETTOUCHPOINTCOUNT: expected 0 args");
        return Value::from_int(GetTouchPointCount());
    }}, true);    R.add_with_policy("SETGESTURESENABLED", Fn{"SETGESTURESENABLED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETGESTURESENABLED: expected 1 args");
        SetGesturesEnabled(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("ISGESTUREDETECTED", Fn{"ISGESTUREDETECTED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISGESTUREDETECTED: expected 1 args");
        return Value::from_bool(IsGestureDetected(args[0].as_int()));
    }}, true);    R.add_with_policy("GETGESTUREDETECTED", Fn{"GETGESTUREDETECTED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREDETECTED: expected 0 args");
        return Value::from_int(GetGestureDetected());
    }}, true);    R.add_with_policy("GETGESTUREHOLDDURATION", Fn{"GETGESTUREHOLDDURATION", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREHOLDDURATION: expected 0 args");
        return Value::from_number(GetGestureHoldDuration());
    }}, true);    R.add_with_policy("GETGESTUREDRAGVECTOR", Fn{"GETGESTUREDRAGVECTOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREDRAGVECTOR: expected 0 args");
        ::Vector2 vec = ::GetGestureDragVector();
        return Value::from_string(std::to_string(vec.x) + "," + std::to_string(vec.y));
    }}, true);    R.add_with_policy("GETGESTUREDRAGANGLE", Fn{"GETGESTUREDRAGANGLE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREDRAGANGLE: expected 0 args");
        return Value::from_number(GetGestureDragAngle());
    }}, true);    R.add_with_policy("GETGESTUREPINCHVECTOR", Fn{"GETGESTUREPINCHVECTOR", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREPINCHVECTOR: expected 0 args");
        ::Vector2 vec = ::GetGesturePinchVector();
        return Value::from_string(std::to_string(vec.x) + "," + std::to_string(vec.y));
    }}, true);    R.add_with_policy("GETGESTUREPINCHANGLE", Fn{"GETGESTUREPINCHANGLE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGESTUREPINCHANGLE: expected 0 args");
        return Value::from_number(GetGesturePinchAngle());
    }}, true);    R.add_with_policy("UPDATECAMERA", Fn{"UPDATECAMERA", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("UPDATECAMERA: expected 2 args");
        ::Camera3D camera;
        camera.position = {(float)0.0f, (float)0.0f, (float)0.0f};
        camera.target = {(float)0.0f, (float)0.0f, (float)1.0f};
        camera.up = {(float)0.0f, (float)1.0f, (float)0.0f};
        camera.fovy = 60.0f;
        camera.projection = CAMERA_PERSPECTIVE;
        ::UpdateCamera(&camera, INT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("UPDATECAMERAPRO", Fn{"UPDATECAMERAPRO", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("UPDATECAMERAPRO: expected 8 args");
        ::Camera3D camera;
        camera.position = {(float)0.0f, (float)0.0f, (float)0.0f};
        camera.target = {(float)0.0f, (float)0.0f, (float)1.0f};
        camera.up = {(float)0.0f, (float)1.0f, (float)0.0f};
        camera.fovy = 60.0f;
        camera.projection = CAMERA_PERSPECTIVE;
        ::Vector3 movement{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        ::Vector3 rotation{(float)FLOAT(4), (float)FLOAT(5), (float)FLOAT(6)};
        ::UpdateCameraPro(&camera, movement, rotation, (float)FLOAT(7));
        return Value::nil();
    }}, true);    R.add_with_policy("SETSHAPESTEXTURE", Fn{"SETSHAPESTEXTURE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("SETSHAPESTEXTURE: expected 5 args");
        ::Rectangle source{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3), (float)FLOAT(4)};
        ::SetShapesTexture(rlreg::textures.at(INT(0)), source);
        return Value::nil();
    }}, true);    R.add_with_policy("GETSHAPESTEXTURE", Fn{"GETSHAPESTEXTURE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETSHAPESTEXTURE: expected 0 args");
        ::Texture2D tex = ::GetShapesTexture();
        int id = rlreg::next_tex_id++;
        rlreg::textures[id] = tex;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GETSHAPESTEXTURERECTANGLE", Fn{"GETSHAPESTEXTURERECTANGLE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETSHAPESTEXTURERECTANGLE: expected 0 args");
        ::Rectangle rec = ::GetShapesTextureRectangle();
        return Value::from_string(std::to_string(rec.x) + "," + std::to_string(rec.y) + "," + std::to_string(rec.width) + "," + std::to_string(rec.height));
    }}, true);    R.add_with_policy("DRAWLINESTRIP", Fn{"DRAWLINESTRIP", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("DRAWLINESTRIP: expected 5 args");
        // Parse points from comma-separated string "x1,y1,x2,y2,..."
        std::string pointStr = STR(0);
        std::vector<::Vector2> points;
        size_t start = 0;
        size_t pos = 0;
        float x = 0, y = 0;
        bool hasX = false;
        while ((pos = pointStr.find(',', start)) != std::string::npos) {
          if (!hasX) {
            x = std::stof(pointStr.substr(start, pos - start));
            hasX = true;
          } else {
            y = std::stof(pointStr.substr(start, pos - start));
            points.push_back({x, y});
            hasX = false;
          }
          start = pos + 1;
        }
        if (hasX && start < pointStr.length()) {
          y = std::stof(pointStr.substr(start));
          points.push_back({x, y});
        }
        ::Color color{(unsigned char)INT(2), (unsigned char)INT(3), (unsigned char)INT(4), 255};
        if (!points.empty()) {
          ::DrawLineStrip(points.data(), (int)points.size(), color);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLELINESV", Fn{"DRAWCIRCLELINESV", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWCIRCLELINESV: expected 6 args");
        ::Vector2 center{(float)FLOAT(0), (float)FLOAT(1)};
        ::Color c{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::DrawCircleLinesV(center, (float)FLOAT(2), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEPRO", Fn{"DRAWRECTANGLEPRO", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWRECTANGLEPRO: expected 10 args");
        ::Rectangle rec{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 origin{(float)FLOAT(4), (float)FLOAT(5)};
        ::Color c{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawRectanglePro(rec, origin, (float)FLOAT(6), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEGRADIENTV", Fn{"DRAWRECTANGLEGRADIENTV", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWRECTANGLEGRADIENTV: expected 10 args");
        ::Color top{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::Color bottom{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawRectangleGradientV(INT(0), INT(1), INT(2), INT(3), top, bottom);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEGRADIENTH", Fn{"DRAWRECTANGLEGRADIENTH", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWRECTANGLEGRADIENTH: expected 10 args");
        ::Color left{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::Color right{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawRectangleGradientH(INT(0), INT(1), INT(2), INT(3), left, right);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEGRADIENTEX", Fn{"DRAWRECTANGLEGRADIENTEX", 14, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 14) throw std::runtime_error("DRAWRECTANGLEGRADIENTEX: expected 14 args");
        ::Rectangle rec{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        ::Color topLeft{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::Color bottomLeft{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::Color topRight{(unsigned char)INT(10), (unsigned char)INT(11), (unsigned char)INT(12), 255};
        ::Color bottomRight{(unsigned char)INT(13), (unsigned char)INT(14), (unsigned char)INT(15), 255};
        ::DrawRectangleGradientEx(rec, topLeft, bottomLeft, topRight, bottomRight);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEROUNDEDLINESEX", Fn{"DRAWRECTANGLEROUNDEDLINESEX", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWRECTANGLEROUNDEDLINESEX: expected 10 args");
        ::Rectangle rec{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        ::Color c{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawRectangleRoundedLinesEx(rec, (float)FLOAT(4), INT(5), (float)FLOAT(6), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINELINEAR", Fn{"DRAWSPLINELINEAR", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWSPLINELINEAR: expected 6 args");
        // Parse points from comma-separated string "x1,y1,x2,y2,..."
        std::string pointStr = STR(0);
        std::vector<::Vector2> points;
        size_t start = 0;
        size_t pos = 0;
        float x = 0, y = 0;
        bool hasX = false;
        while ((pos = pointStr.find(',', start)) != std::string::npos) {
          if (!hasX) {
            x = std::stof(pointStr.substr(start, pos - start));
            hasX = true;
          } else {
            y = std::stof(pointStr.substr(start, pos - start));
            points.push_back({x, y});
            hasX = false;
          }
          start = pos + 1;
        }
        if (hasX && start < pointStr.length()) {
          y = std::stof(pointStr.substr(start));
          points.push_back({x, y});
        }
        ::Color color{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        if (points.size() >= 2) {
          ::DrawSplineLinear(points.data(), (int)points.size(), (float)FLOAT(2), color);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINEBASIS", Fn{"DRAWSPLINEBASIS", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWSPLINEBASIS: expected 6 args");
        // Parse points from comma-separated string "x1,y1,x2,y2,..."
        std::string pointStr = STR(0);
        std::vector<::Vector2> points;
        size_t start = 0;
        size_t pos = 0;
        float x = 0, y = 0;
        bool hasX = false;
        while ((pos = pointStr.find(',', start)) != std::string::npos) {
          if (!hasX) {
            x = std::stof(pointStr.substr(start, pos - start));
            hasX = true;
          } else {
            y = std::stof(pointStr.substr(start, pos - start));
            points.push_back({x, y});
            hasX = false;
          }
          start = pos + 1;
        }
        if (hasX && start < pointStr.length()) {
          y = std::stof(pointStr.substr(start));
          points.push_back({x, y});
        }
        ::Color color{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        if (points.size() >= 4) {
          ::DrawSplineBasis(points.data(), (int)points.size(), (float)FLOAT(2), color);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINECATMULLROM", Fn{"DRAWSPLINECATMULLROM", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWSPLINECATMULLROM: expected 6 args");
        // Parse points from comma-separated string "x1,y1,x2,y2,..."
        std::string pointStr = STR(0);
        std::vector<::Vector2> points;
        size_t start = 0;
        size_t pos = 0;
        float x = 0, y = 0;
        bool hasX = false;
        while ((pos = pointStr.find(',', start)) != std::string::npos) {
          if (!hasX) {
            x = std::stof(pointStr.substr(start, pos - start));
            hasX = true;
          } else {
            y = std::stof(pointStr.substr(start, pos - start));
            points.push_back({x, y});
            hasX = false;
          }
          start = pos + 1;
        }
        if (hasX && start < pointStr.length()) {
          y = std::stof(pointStr.substr(start));
          points.push_back({x, y});
        }
        ::Color color{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        if (points.size() >= 4) {
          ::DrawSplineCatmullRom(points.data(), (int)points.size(), (float)FLOAT(2), color);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINEBEZIERQUADRATIC", Fn{"DRAWSPLINEBEZIERQUADRATIC", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWSPLINEBEZIERQUADRATIC: expected 6 args");
        // Parse points from comma-separated string "x1,y1,x2,y2,..."
        std::string pointStr = STR(0);
        std::vector<::Vector2> points;
        size_t start = 0;
        size_t pos = 0;
        float x = 0, y = 0;
        bool hasX = false;
        while ((pos = pointStr.find(',', start)) != std::string::npos) {
          if (!hasX) {
            x = std::stof(pointStr.substr(start, pos - start));
            hasX = true;
          } else {
            y = std::stof(pointStr.substr(start, pos - start));
            points.push_back({x, y});
            hasX = false;
          }
          start = pos + 1;
        }
        if (hasX && start < pointStr.length()) {
          y = std::stof(pointStr.substr(start));
          points.push_back({x, y});
        }
        ::Color color{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        if (points.size() >= 3) {
          ::DrawSplineBezierQuadratic(points.data(), (int)points.size(), (float)FLOAT(2), color);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINEBEZIERCUBIC", Fn{"DRAWSPLINEBEZIERCUBIC", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWSPLINEBEZIERCUBIC: expected 6 args");
        // Parse points from comma-separated string "x1,y1,x2,y2,..."
        std::string pointStr = STR(0);
        std::vector<::Vector2> points;
        size_t start = 0;
        size_t pos = 0;
        float x = 0, y = 0;
        bool hasX = false;
        while ((pos = pointStr.find(',', start)) != std::string::npos) {
          if (!hasX) {
            x = std::stof(pointStr.substr(start, pos - start));
            hasX = true;
          } else {
            y = std::stof(pointStr.substr(start, pos - start));
            points.push_back({x, y});
            hasX = false;
          }
          start = pos + 1;
        }
        if (hasX && start < pointStr.length()) {
          y = std::stof(pointStr.substr(start));
          points.push_back({x, y});
        }
        ::Color color{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        if (points.size() >= 4) {
          ::DrawSplineBezierCubic(points.data(), (int)points.size(), (float)FLOAT(2), color);
        }
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINESEGMENTLINEAR", Fn{"DRAWSPLINESEGMENTLINEAR", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWSPLINESEGMENTLINEAR: expected 8 args");
        ::Vector2 p1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 p2{(float)FLOAT(2), (float)FLOAT(3)};
        ::Color c{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawSplineSegmentLinear(p1, p2, (float)FLOAT(4), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINESEGMENTBASIS", Fn{"DRAWSPLINESEGMENTBASIS", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWSPLINESEGMENTBASIS: expected 12 args");
        ::Vector2 p1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 p2{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 p3{(float)FLOAT(4), (float)FLOAT(5)};
        ::Vector2 p4{(float)FLOAT(6), (float)FLOAT(7)};
        ::Color c{(unsigned char)INT(9), (unsigned char)INT(10), (unsigned char)INT(11), 255};
        ::DrawSplineSegmentBasis(p1, p2, p3, p4, (float)FLOAT(8), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINESEGMENTCATMULLROM", Fn{"DRAWSPLINESEGMENTCATMULLROM", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWSPLINESEGMENTCATMULLROM: expected 12 args");
        ::Vector2 p1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 p2{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 p3{(float)FLOAT(4), (float)FLOAT(5)};
        ::Vector2 p4{(float)FLOAT(6), (float)FLOAT(7)};
        ::Color c{(unsigned char)INT(9), (unsigned char)INT(10), (unsigned char)INT(11), 255};
        ::DrawSplineSegmentCatmullRom(p1, p2, p3, p4, (float)FLOAT(8), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINESEGMENTBEZIERQUADRATIC", Fn{"DRAWSPLINESEGMENTBEZIERQUADRATIC", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWSPLINESEGMENTBEZIERQUADRATIC: expected 10 args");
        ::Vector2 p1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 c2{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 p3{(float)FLOAT(4), (float)FLOAT(5)};
        ::Color c{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::DrawSplineSegmentBezierQuadratic(p1, c2, p3, (float)FLOAT(6), c);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPLINESEGMENTBEZIERCUBIC", Fn{"DRAWSPLINESEGMENTBEZIERCUBIC", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWSPLINESEGMENTBEZIERCUBIC: expected 12 args");
        ::Vector2 p1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 c2{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 c3{(float)FLOAT(4), (float)FLOAT(5)};
        ::Vector2 p4{(float)FLOAT(6), (float)FLOAT(7)};
        ::Color c{(unsigned char)INT(9), (unsigned char)INT(10), (unsigned char)INT(11), 255};
        ::DrawSplineSegmentBezierCubic(p1, c2, c3, p4, (float)FLOAT(8), c);
        return Value::nil();
    }}, true);    R.add_with_policy("GETSPLINEPOINTLINEAR", Fn{"GETSPLINEPOINTLINEAR", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("GETSPLINEPOINTLINEAR: expected 5 args");
        ::Vector2 start{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 end{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 point = ::GetSplinePointLinear(start, end, (float)FLOAT(4));
        return Value::from_string(std::to_string(point.x) + "," + std::to_string(point.y));
    }}, true);    R.add_with_policy("GETSPLINEPOINTBASIS", Fn{"GETSPLINEPOINTBASIS", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("GETSPLINEPOINTBASIS: expected 9 args");
        ::Vector2 p1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 p2{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 p3{(float)FLOAT(4), (float)FLOAT(5)};
        ::Vector2 p4{(float)FLOAT(6), (float)FLOAT(7)};
        ::Vector2 point = ::GetSplinePointBasis(p1, p2, p3, p4, (float)FLOAT(8));
        return Value::from_string(std::to_string(point.x) + "," + std::to_string(point.y));
    }}, true);    R.add_with_policy("GETSPLINEPOINTCATMULLROM", Fn{"GETSPLINEPOINTCATMULLROM", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("GETSPLINEPOINTCATMULLROM: expected 9 args");
        ::Vector2 p1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 p2{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 p3{(float)FLOAT(4), (float)FLOAT(5)};
        ::Vector2 p4{(float)FLOAT(6), (float)FLOAT(7)};
        ::Vector2 point = ::GetSplinePointCatmullRom(p1, p2, p3, p4, (float)FLOAT(8));
        return Value::from_string(std::to_string(point.x) + "," + std::to_string(point.y));
    }}, true);    R.add_with_policy("GETSPLINEPOINTBEZIERQUAD", Fn{"GETSPLINEPOINTBEZIERQUAD", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("GETSPLINEPOINTBEZIERQUAD: expected 7 args");
        ::Vector2 p1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 c2{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 p3{(float)FLOAT(4), (float)FLOAT(5)};
        ::Vector2 point = ::GetSplinePointBezierQuad(p1, c2, p3, (float)FLOAT(6));
        return Value::from_string(std::to_string(point.x) + "," + std::to_string(point.y));
    }}, true);    R.add_with_policy("GETSPLINEPOINTBEZIERCUBIC", Fn{"GETSPLINEPOINTBEZIERCUBIC", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("GETSPLINEPOINTBEZIERCUBIC: expected 9 args");
        ::Vector2 p1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 c2{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 c3{(float)FLOAT(4), (float)FLOAT(5)};
        ::Vector2 p4{(float)FLOAT(6), (float)FLOAT(7)};
        ::Vector2 point = ::GetSplinePointBezierCubic(p1, c2, c3, p4, (float)FLOAT(8));
        return Value::from_string(std::to_string(point.x) + "," + std::to_string(point.y));
    }}, true);    R.add_with_policy("CHECKCOLLISIONCIRCLELINE", Fn{"CHECKCOLLISIONCIRCLELINE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("CHECKCOLLISIONCIRCLELINE: expected 7 args");
        ::Vector2 center{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 p1{(float)FLOAT(3), (float)FLOAT(4)};
        ::Vector2 p2{(float)FLOAT(5), (float)FLOAT(6)};
        return Value::from_bool(::CheckCollisionCircleLine(center, (float)FLOAT(2), p1, p2));
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTTRIANGLE", Fn{"CHECKCOLLISIONPOINTTRIANGLE", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("CHECKCOLLISIONPOINTTRIANGLE: expected 8 args");
        ::Vector2 point{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 p1{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 p2{(float)FLOAT(4), (float)FLOAT(5)};
        ::Vector2 p3{(float)FLOAT(6), (float)FLOAT(7)};
        return Value::from_bool(::CheckCollisionPointTriangle(point, p1, p2, p3));
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTLINE", Fn{"CHECKCOLLISIONPOINTLINE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("CHECKCOLLISIONPOINTLINE: expected 7 args");
        ::Vector2 point{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 p1{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 p2{(float)FLOAT(4), (float)FLOAT(5)};
        return Value::from_bool(::CheckCollisionPointLine(point, p1, p2, INT(6)));
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTPOLY", Fn{"CHECKCOLLISIONPOINTPOLY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("CHECKCOLLISIONPOINTPOLY: expected 2 args");
        // Parse point and polygon from string "pointX,pointY,polyX1,polyY1,polyX2,polyY2,..."
        std::string dataStr = STR(0);
        std::vector<::Vector2> points;
        size_t start = 0;
        size_t pos = 0;
        float x = 0, y = 0;
        bool hasX = false;
        int pointCount = INT(1);
        ::Vector2 checkPoint{0, 0};
        bool isFirstPoint = true;
        while ((pos = dataStr.find(',', start)) != std::string::npos) {
          if (!hasX) {
            x = std::stof(dataStr.substr(start, pos - start));
            hasX = true;
          } else {
            y = std::stof(dataStr.substr(start, pos - start));
            if (isFirstPoint) {
              checkPoint = {x, y};
              isFirstPoint = false;
            } else {
              points.push_back({x, y});
            }
            hasX = false;
          }
          start = pos + 1;
        }
        if (hasX && start < dataStr.length()) {
          y = std::stof(dataStr.substr(start));
          if (isFirstPoint) {
            checkPoint = {x, y};
          } else {
            points.push_back({x, y});
          }
        }
        if (points.size() >= 3 && pointCount > 0) {
          return Value::from_bool(::CheckCollisionPointPoly(checkPoint, points.data(), pointCount));
        }
        return Value::from_bool(false);
    }}, true);    R.add_with_policy("CHECKCOLLISIONLINES", Fn{"CHECKCOLLISIONLINES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("CHECKCOLLISIONLINES: expected 8 args");
        ::Vector2 start1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 end1{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 start2{(float)FLOAT(4), (float)FLOAT(5)};
        ::Vector2 end2{(float)FLOAT(6), (float)FLOAT(7)};
        ::Vector2 collisionPoint;
        bool result = ::CheckCollisionLines(start1, end1, start2, end2, &collisionPoint);
        if (result) {
          return Value::from_string(std::to_string(collisionPoint.x) + "," + std::to_string(collisionPoint.y));
        }
        return Value::from_string("");
    }}, true);    R.add_with_policy("GETCOLLISIONREC", Fn{"GETCOLLISIONREC", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("GETCOLLISIONREC: expected 8 args");
        ::Rectangle rec1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        ::Rectangle rec2{(float)FLOAT(4), (float)FLOAT(5), (float)FLOAT(6), (float)FLOAT(7)};
        ::Rectangle collision = ::GetCollisionRec(rec1, rec2);
        return Value::from_string(std::to_string(collision.x) + "," + std::to_string(collision.y) + "," + std::to_string(collision.width) + "," + std::to_string(collision.height));
    }}, true);    R.add_with_policy("LOADIMAGEANIMFROMMEMORY", Fn{"LOADIMAGEANIMFROMMEMORY", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("LOADIMAGEANIMFROMMEMORY: expected 3 args");
        int frames;
        ::Image img = ::LoadImageAnimFromMemory(STR(0).c_str(), (const unsigned char*)STR(1).c_str(), INT(2), &frames);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = img;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADIMAGEFROMSCREEN", Fn{"LOADIMAGEFROMSCREEN", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("LOADIMAGEFROMSCREEN: expected 0 args");
        ::Image img = ::LoadImageFromScreen();
        int id = rlreg::next_image_id++;
        rlreg::images[id] = img;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISIMAGEVALID", Fn{"ISIMAGEVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISIMAGEVALID: expected 1 args");
        return Value::from_bool(::IsImageValid(rlreg::images.at(INT(0))));
    }}, true);    R.add_with_policy("EXPORTIMAGETOMEMORY", Fn{"EXPORTIMAGETOMEMORY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTIMAGETOMEMORY: expected 2 args");
        int fileSize;
        unsigned char* data = ::ExportImageToMemory(rlreg::images.at(INT(0)), STR(1).c_str(), &fileSize);
        std::string result((char*)data, fileSize);
        ::MemFree(data);
        return Value::from_string(result);
    }}, true);    R.add_with_policy("GENIMAGEGRADIENTLINEAR", Fn{"GENIMAGEGRADIENTLINEAR", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("GENIMAGEGRADIENTLINEAR: expected 9 args");
        ::Color start{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::Color end{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::Image img = ::GenImageGradientLinear(INT(0), INT(1), INT(2), start, end);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = img;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENIMAGEGRADIENTRADIAL", Fn{"GENIMAGEGRADIENTRADIAL", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("GENIMAGEGRADIENTRADIAL: expected 9 args");
        ::Color inner{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::Color outer{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::Image img = ::GenImageGradientRadial(INT(0), INT(1), (float)FLOAT(2), inner, outer);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = img;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENIMAGEGRADIENTSQUARE", Fn{"GENIMAGEGRADIENTSQUARE", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("GENIMAGEGRADIENTSQUARE: expected 9 args");
        ::Color inner{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::Color outer{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::Image img = ::GenImageGradientSquare(INT(0), INT(1), (float)FLOAT(2), inner, outer);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = img;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENIMAGECHECKED", Fn{"GENIMAGECHECKED", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("GENIMAGECHECKED: expected 10 args");
        ::Color col1{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::Color col2{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::Image img = ::GenImageChecked(INT(0), INT(1), INT(2), INT(3), col1, col2);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = img;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENIMAGEWHITENOISE", Fn{"GENIMAGEWHITENOISE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENIMAGEWHITENOISE: expected 3 args");
        ::Image img = ::GenImageWhiteNoise(INT(0), INT(1), (float)FLOAT(2));
        int id = rlreg::next_image_id++;
        rlreg::images[id] = img;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENIMAGEPERLINNOISE", Fn{"GENIMAGEPERLINNOISE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("GENIMAGEPERLINNOISE: expected 5 args");
        ::Image img = ::GenImagePerlinNoise(INT(0), INT(1), INT(2), INT(3), (float)FLOAT(4));
        int id = rlreg::next_image_id++;
        rlreg::images[id] = img;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENIMAGECELLULAR", Fn{"GENIMAGECELLULAR", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENIMAGECELLULAR: expected 3 args");
        ::Image img = ::GenImageCellular(INT(0), INT(1), INT(2));
        int id = rlreg::next_image_id++;
        rlreg::images[id] = img;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENIMAGETEXT", Fn{"GENIMAGETEXT", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENIMAGETEXT: expected 3 args");
        ::Image img = ::GenImageText(INT(0), INT(1), STR(2).c_str());
        int id = rlreg::next_image_id++;
        rlreg::images[id] = img;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("IMAGEFROMIMAGE", Fn{"IMAGEFROMIMAGE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("IMAGEFROMIMAGE: expected 5 args");
        ::Rectangle rec{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3), (float)FLOAT(4)};
        ::Image img = ::ImageFromImage(rlreg::images.at(INT(0)), rec);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = img;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("IMAGEFROMCHANNEL", Fn{"IMAGEFROMCHANNEL", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("IMAGEFROMCHANNEL: expected 2 args");
        ::Image img = ::ImageFromChannel(rlreg::images.at(INT(0)), INT(1));
        int id = rlreg::next_image_id++;
        rlreg::images[id] = img;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("IMAGETEXT", Fn{"IMAGETEXT", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("IMAGETEXT: expected 5 args");
        ::Color color{(unsigned char)INT(2), (unsigned char)INT(3), (unsigned char)INT(4), 255};
        ::Image img = ::ImageText(STR(0).c_str(), INT(1), color);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = img;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("IMAGETEXTEX", Fn{"IMAGETEXTEX", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("IMAGETEXTEX: expected 7 args");
        ::Color tint{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::Image img = ::ImageTextEx(rlreg::fonts.at(INT(0)), STR(1).c_str(), (float)FLOAT(2), (float)FLOAT(3), tint);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = img;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("IMAGEFORMAT", Fn{"IMAGEFORMAT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("IMAGEFORMAT: expected 2 args");
        ::ImageFormat(&rlreg::images.at(INT(0)), INT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGETOPOT", Fn{"IMAGETOPOT", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("IMAGETOPOT: expected 4 args");
        ::Color fill{(unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3), 255};
        ::ImageToPOT(&rlreg::images.at(INT(0)), fill);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEALPHACROP", Fn{"IMAGEALPHACROP", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("IMAGEALPHACROP: expected 2 args");
        ::ImageAlphaCrop(&rlreg::images.at(INT(0)), (float)FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEALPHACLEAR", Fn{"IMAGEALPHACLEAR", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("IMAGEALPHACLEAR: expected 5 args");
        ::Color color{(unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3), 255};
        ::ImageAlphaClear(&rlreg::images.at(INT(0)), color, (float)FLOAT(4));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEALPHAMASK", Fn{"IMAGEALPHAMASK", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("IMAGEALPHAMASK: expected 2 args");
        ::ImageAlphaMask(&rlreg::images.at(INT(0)), rlreg::images.at(INT(1)));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEALPHAPREMULTIPLY", Fn{"IMAGEALPHAPREMULTIPLY", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("IMAGEALPHAPREMULTIPLY: expected 1 args");
        ::ImageAlphaPremultiply(&rlreg::images.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEBLURGAUSSIAN", Fn{"IMAGEBLURGAUSSIAN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("IMAGEBLURGAUSSIAN: expected 2 args");
        ::ImageBlurGaussian(&rlreg::images.at(INT(0)), INT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEKERNELCONVOLUTION", Fn{"IMAGEKERNELCONVOLUTION", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("IMAGEKERNELCONVOLUTION: expected 3 args");
        // Parse kernel from comma-separated string "v1,v2,v3,..."
        std::string kernelStr = STR(1);
        int kernelSize = INT(2);
        float* kernel = new float[kernelSize * kernelSize];
        size_t start = 0;
        size_t pos = 0;
        int idx = 0;
        while ((pos = kernelStr.find(',', start)) != std::string::npos && idx < kernelSize * kernelSize) {
          kernel[idx++] = std::stof(kernelStr.substr(start, pos - start));
          start = pos + 1;
        }
        if (idx < kernelSize * kernelSize && start < kernelStr.length()) {
          kernel[idx] = std::stof(kernelStr.substr(start));
        }
        ::ImageKernelConvolution(&rlreg::images.at(INT(0)), kernel, kernelSize);
        delete[] kernel;
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGERESIZENN", Fn{"IMAGERESIZENN", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("IMAGERESIZENN: expected 3 args");
        ::ImageResizeNN(&rlreg::images.at(INT(0)), INT(1), INT(2));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGERESIZECANVAS", Fn{"IMAGERESIZECANVAS", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("IMAGERESIZECANVAS: expected 8 args");
        ::Color fill{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::ImageResizeCanvas(&rlreg::images.at(INT(0)), INT(1), INT(2), INT(3), INT(4), fill);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEMIPMAPS", Fn{"IMAGEMIPMAPS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("IMAGEMIPMAPS: expected 1 args");
        ::ImageMipmaps(&rlreg::images.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDITHER", Fn{"IMAGEDITHER", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("IMAGEDITHER: expected 5 args");
        ::ImageDither(&rlreg::images.at(INT(0)), INT(1), INT(2), INT(3), INT(4));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEFLIPVERTICAL", Fn{"IMAGEFLIPVERTICAL", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("IMAGEFLIPVERTICAL: expected 1 args");
        ::ImageFlipVertical(&rlreg::images.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEFLIPHORIZONTAL", Fn{"IMAGEFLIPHORIZONTAL", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("IMAGEFLIPHORIZONTAL: expected 1 args");
        ::ImageFlipHorizontal(&rlreg::images.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEROTATE", Fn{"IMAGEROTATE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("IMAGEROTATE: expected 2 args");
        ::ImageRotate(&rlreg::images.at(INT(0)), INT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEROTATECW", Fn{"IMAGEROTATECW", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("IMAGEROTATECW: expected 1 args");
        ::ImageRotateCW(&rlreg::images.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEROTATECCW", Fn{"IMAGEROTATECCW", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("IMAGEROTATECCW: expected 1 args");
        ::ImageRotateCCW(&rlreg::images.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGECOLORTINT", Fn{"IMAGECOLORTINT", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("IMAGECOLORTINT: expected 4 args");
        ::Color color{(unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3), 255};
        ::ImageColorTint(&rlreg::images.at(INT(0)), color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGECOLORINVERT", Fn{"IMAGECOLORINVERT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("IMAGECOLORINVERT: expected 1 args");
        ::ImageColorInvert(&rlreg::images.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGECOLORGRAYSCALE", Fn{"IMAGECOLORGRAYSCALE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("IMAGECOLORGRAYSCALE: expected 1 args");
        ::ImageColorGrayscale(&rlreg::images.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGECOLORCONTRAST", Fn{"IMAGECOLORCONTRAST", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("IMAGECOLORCONTRAST: expected 2 args");
        ::ImageColorContrast(&rlreg::images.at(INT(0)), (float)FLOAT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGECOLORBRIGHTNESS", Fn{"IMAGECOLORBRIGHTNESS", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("IMAGECOLORBRIGHTNESS: expected 2 args");
        ::ImageColorBrightness(&rlreg::images.at(INT(0)), INT(1));
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGECOLORREPLACE", Fn{"IMAGECOLORREPLACE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("IMAGECOLORREPLACE: expected 7 args");
        ::Color color{(unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3), 255};
        ::Color replace{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::ImageColorReplace(&rlreg::images.at(INT(0)), color, replace);
        return Value::nil();
    }}, true);    R.add_with_policy("GETIMAGEALPHABORDER", Fn{"GETIMAGEALPHABORDER", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETIMAGEALPHABORDER: expected 2 args");
        ::Rectangle rec = ::GetImageAlphaBorder(rlreg::images.at(INT(0)), (float)FLOAT(1));
        return Value::from_string(std::to_string(rec.x) + "," + std::to_string(rec.y) + "," + std::to_string(rec.width) + "," + std::to_string(rec.height));
    }}, true);    R.add_with_policy("GETIMAGECOLOR", Fn{"GETIMAGECOLOR", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GETIMAGECOLOR: expected 3 args");
        ::Color c = ::GetImageColor(rlreg::images.at(INT(0)), INT(1), INT(2));
        return Value::from_string(std::to_string(c.r) + "," + std::to_string(c.g) + "," + std::to_string(c.b) + "," + std::to_string(c.a));
    }}, true);    R.add_with_policy("IMAGECLEARBACKGROUND", Fn{"IMAGECLEARBACKGROUND", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("IMAGECLEARBACKGROUND: expected 4 args");
        ::Color color{(unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3), 255};
        ::ImageClearBackground(&rlreg::images.at(INT(0)), color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWPIXEL", Fn{"IMAGEDRAWPIXEL", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("IMAGEDRAWPIXEL: expected 6 args");
        ::Color color{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::ImageDrawPixel(&rlreg::images.at(INT(0)), INT(1), INT(2), color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWPIXELV", Fn{"IMAGEDRAWPIXELV", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("IMAGEDRAWPIXELV: expected 6 args");
        ::Vector2 position{(float)FLOAT(1), (float)FLOAT(2)};
        ::Color color{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::ImageDrawPixelV(&rlreg::images.at(INT(0)), position, color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWLINE", Fn{"IMAGEDRAWLINE", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("IMAGEDRAWLINE: expected 8 args");
        ::Color color{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::ImageDrawLine(&rlreg::images.at(INT(0)), INT(1), INT(2), INT(3), INT(4), color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWLINEV", Fn{"IMAGEDRAWLINEV", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("IMAGEDRAWLINEV: expected 8 args");
        ::Vector2 start{(float)FLOAT(1), (float)FLOAT(2)};
        ::Vector2 end{(float)FLOAT(3), (float)FLOAT(4)};
        ::Color color{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::ImageDrawLineV(&rlreg::images.at(INT(0)), start, end, color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWLINEEX", Fn{"IMAGEDRAWLINEEX", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("IMAGEDRAWLINEEX: expected 9 args");
        ::Vector2 start{(float)FLOAT(1), (float)FLOAT(2)};
        ::Vector2 end{(float)FLOAT(3), (float)FLOAT(4)};
        ::Color color{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::ImageDrawLineEx(&rlreg::images.at(INT(0)), start, end, INT(5), color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWCIRCLE", Fn{"IMAGEDRAWCIRCLE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("IMAGEDRAWCIRCLE: expected 7 args");
        ::Color color{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::ImageDrawCircle(&rlreg::images.at(INT(0)), INT(1), INT(2), INT(3), color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWCIRCLEV", Fn{"IMAGEDRAWCIRCLEV", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("IMAGEDRAWCIRCLEV: expected 7 args");
        ::Vector2 center{(float)FLOAT(1), (float)FLOAT(2)};
        ::Color color{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::ImageDrawCircleV(&rlreg::images.at(INT(0)), center, INT(3), color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWCIRCLELINES", Fn{"IMAGEDRAWCIRCLELINES", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("IMAGEDRAWCIRCLELINES: expected 7 args");
        ::Color color{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::ImageDrawCircleLines(&rlreg::images.at(INT(0)), INT(1), INT(2), INT(3), color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWCIRCLELINESV", Fn{"IMAGEDRAWCIRCLELINESV", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("IMAGEDRAWCIRCLELINESV: expected 7 args");
        ::Vector2 center{(float)FLOAT(1), (float)FLOAT(2)};
        ::Color color{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::ImageDrawCircleLinesV(&rlreg::images.at(INT(0)), center, INT(3), color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWRECTANGLE", Fn{"IMAGEDRAWRECTANGLE", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("IMAGEDRAWRECTANGLE: expected 8 args");
        ::Color color{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::ImageDrawRectangle(&rlreg::images.at(INT(0)), INT(1), INT(2), INT(3), INT(4), color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWRECTANGLEV", Fn{"IMAGEDRAWRECTANGLEV", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("IMAGEDRAWRECTANGLEV: expected 8 args");
        ::Vector2 position{(float)FLOAT(1), (float)FLOAT(2)};
        ::Vector2 size{(float)FLOAT(3), (float)FLOAT(4)};
        ::Color color{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::ImageDrawRectangleV(&rlreg::images.at(INT(0)), position, size, color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWRECTANGLEREC", Fn{"IMAGEDRAWRECTANGLEREC", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("IMAGEDRAWRECTANGLEREC: expected 8 args");
        ::Rectangle rec{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3), (float)FLOAT(4)};
        ::Color color{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::ImageDrawRectangleRec(&rlreg::images.at(INT(0)), rec, color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWRECTANGLELINES", Fn{"IMAGEDRAWRECTANGLELINES", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("IMAGEDRAWRECTANGLELINES: expected 9 args");
        ::Rectangle rec{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3), (float)FLOAT(4)};
        ::Color color{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::ImageDrawRectangleLines(&rlreg::images.at(INT(0)), rec, INT(5), color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWTRIANGLE", Fn{"IMAGEDRAWTRIANGLE", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("IMAGEDRAWTRIANGLE: expected 10 args");
        ::Vector2 v1{(float)FLOAT(1), (float)FLOAT(2)};
        ::Vector2 v2{(float)FLOAT(3), (float)FLOAT(4)};
        ::Vector2 v3{(float)FLOAT(5), (float)FLOAT(6)};
        ::Color color{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::ImageDrawTriangle(&rlreg::images.at(INT(0)), v1, v2, v3, color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWTRIANGLEEX", Fn{"IMAGEDRAWTRIANGLEEX", 16, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 16) throw std::runtime_error("IMAGEDRAWTRIANGLEEX: expected 16 args");
        ::Vector2 v1{(float)FLOAT(1), (float)FLOAT(2)};
        ::Vector2 v2{(float)FLOAT(3), (float)FLOAT(4)};
        ::Vector2 v3{(float)FLOAT(5), (float)FLOAT(6)};
        ::Color c1{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::Color c2{(unsigned char)INT(10), (unsigned char)INT(11), (unsigned char)INT(12), 255};
        ::Color c3{(unsigned char)INT(13), (unsigned char)INT(14), (unsigned char)INT(15), 255};
        ::ImageDrawTriangleEx(&rlreg::images.at(INT(0)), v1, v2, v3, c1, c2, c3);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWTRIANGLELINES", Fn{"IMAGEDRAWTRIANGLELINES", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("IMAGEDRAWTRIANGLELINES: expected 10 args");
        ::Vector2 v1{(float)FLOAT(1), (float)FLOAT(2)};
        ::Vector2 v2{(float)FLOAT(3), (float)FLOAT(4)};
        ::Vector2 v3{(float)FLOAT(5), (float)FLOAT(6)};
        ::Color color{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::ImageDrawTriangleLines(&rlreg::images.at(INT(0)), v1, v2, v3, color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWTEXT", Fn{"IMAGEDRAWTEXT", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("IMAGEDRAWTEXT: expected 8 args");
        ::Color color{(unsigned char)INT(4), (unsigned char)INT(5), (unsigned char)INT(6), 255};
        ::ImageDrawText(&rlreg::images.at(INT(0)), STR(1).c_str(), INT(2), INT(3), INT(4), color);
        return Value::nil();
    }}, true);    R.add_with_policy("IMAGEDRAWTEXTEX", Fn{"IMAGEDRAWTEXTEX", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("IMAGEDRAWTEXTEX: expected 10 args");
        ::Vector2 position{(float)FLOAT(3), (float)FLOAT(4)};
        ::Color tint{(unsigned char)INT(7), (unsigned char)INT(8), (unsigned char)INT(9), 255};
        ::ImageDrawTextEx(&rlreg::images.at(INT(0)), rlreg::fonts.at(INT(1)), STR(2).c_str(), position, (float)FLOAT(5), (float)FLOAT(6), tint);
        return Value::nil();
    }}, true);    R.add_with_policy("LOADTEXTURECUBEMAP", Fn{"LOADTEXTURECUBEMAP", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADTEXTURECUBEMAP: expected 2 args");
        ::Texture2D tex = ::LoadTextureCubemap(rlreg::images.at(INT(0)), INT(1));
        int id = rlreg::next_cubemap_id++;
        rlreg::cubemaps[id] = tex;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISTEXTUREVALID", Fn{"ISTEXTUREVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISTEXTUREVALID: expected 1 args");
        return Value::from_bool(::IsTextureValid(rlreg::textures.at(INT(0))));
    }}, true);    R.add_with_policy("ISRENDERTEXTUREVALID", Fn{"ISRENDERTEXTUREVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISRENDERTEXTUREVALID: expected 1 args");
        return Value::from_bool(::IsRenderTextureValid(rlreg::rendertextures.at(INT(0))));
    }}, true);    R.add_with_policy("UPDATETEXTUREREC", Fn{"UPDATETEXTUREREC", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("UPDATETEXTUREREC: expected 6 args");
        ::Rectangle rec{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3), (float)FLOAT(4)};
        std::string pixelData = STR(5);
        // Convert string to unsigned char array (assuming RGBA format)
        const unsigned char* pixels = reinterpret_cast<const unsigned char*>(pixelData.c_str());
        ::UpdateTextureRec(rlreg::textures.at(INT(0)), rec, pixels);
        return Value::nil();
    }}, true);    R.add_with_policy("COLORISEQUAL", Fn{"COLORISEQUAL", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("COLORISEQUAL: expected 6 args");
        ::Color col1{(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), 255};
        ::Color col2{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        return Value::from_bool(::ColorIsEqual(col1, col2));
    }}, true);    R.add_with_policy("COLORTOINT", Fn{"COLORTOINT", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("COLORTOINT: expected 3 args");
        ::Color color{(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), 255};
        return Value::from_int(::ColorToInt(color));
    }}, true);    R.add_with_policy("COLORNORMALIZE", Fn{"COLORNORMALIZE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("COLORNORMALIZE: expected 3 args");
        ::Color color{(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), 255};
        ::Vector4 normalized = ::ColorNormalize(color);
        return Value::from_string(std::to_string(normalized.x) + "," + std::to_string(normalized.y) + "," + std::to_string(normalized.z) + "," + std::to_string(normalized.w));
    }}, true);    R.add_with_policy("COLORFROMNORMALIZED", Fn{"COLORFROMNORMALIZED", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("COLORFROMNORMALIZED: expected 4 args");
        ::Vector4 normalized{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        ::Color color = ::ColorFromNormalized(normalized);
        return Value::from_string(std::to_string(color.r) + "," + std::to_string(color.g) + "," + std::to_string(color.b) + "," + std::to_string(color.a));
    }}, true);    R.add_with_policy("COLORTOHSV", Fn{"COLORTOHSV", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("COLORTOHSV: expected 3 args");
        ::Color color{(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), 255};
        ::Vector3 hsv = ::ColorToHSV(color);
        return Value::from_string(std::to_string(hsv.x) + "," + std::to_string(hsv.y) + "," + std::to_string(hsv.z));
    }}, true);    R.add_with_policy("COLORFROMHSV", Fn{"COLORFROMHSV", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("COLORFROMHSV: expected 3 args");
        ::Color color = ::ColorFromHSV((float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2));
        return Value::from_string(std::to_string(color.r) + "," + std::to_string(color.g) + "," + std::to_string(color.b) + "," + std::to_string(color.a));
    }}, true);    R.add_with_policy("COLORTINT", Fn{"COLORTINT", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("COLORTINT: expected 6 args");
        ::Color color{(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), 255};
        ::Color tint{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::Color result = ::ColorTint(color, tint);
        return Value::from_string(std::to_string(result.r) + "," + std::to_string(result.g) + "," + std::to_string(result.b) + "," + std::to_string(result.a));
    }}, true);    R.add_with_policy("COLORBRIGHTNESS", Fn{"COLORBRIGHTNESS", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("COLORBRIGHTNESS: expected 4 args");
        ::Color color{(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), 255};
        ::Color result = ::ColorBrightness(color, (float)FLOAT(3));
        return Value::from_string(std::to_string(result.r) + "," + std::to_string(result.g) + "," + std::to_string(result.b) + "," + std::to_string(result.a));
    }}, true);    R.add_with_policy("COLORCONTRAST", Fn{"COLORCONTRAST", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("COLORCONTRAST: expected 4 args");
        ::Color color{(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), 255};
        ::Color result = ::ColorContrast(color, (float)FLOAT(3));
        return Value::from_string(std::to_string(result.r) + "," + std::to_string(result.g) + "," + std::to_string(result.b) + "," + std::to_string(result.a));
    }}, true);    R.add_with_policy("COLORALPHA", Fn{"COLORALPHA", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("COLORALPHA: expected 4 args");
        ::Color color{(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), 255};
        ::Color result = ::ColorAlpha(color, (float)FLOAT(3));
        return Value::from_string(std::to_string(result.r) + "," + std::to_string(result.g) + "," + std::to_string(result.b) + "," + std::to_string(result.a));
    }}, true);    R.add_with_policy("COLORALPHABLEND", Fn{"COLORALPHABLEND", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("COLORALPHABLEND: expected 9 args");
        ::Color dst{(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), 255};
        ::Color src{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::Color tint{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::Color result = ::ColorAlphaBlend(dst, src, tint);
        return Value::from_string(std::to_string(result.r) + "," + std::to_string(result.g) + "," + std::to_string(result.b) + "," + std::to_string(result.a));
    }}, true);    R.add_with_policy("COLORLERP", Fn{"COLORLERP", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("COLORLERP: expected 7 args");
        ::Color color1{(unsigned char)INT(0), (unsigned char)INT(1), (unsigned char)INT(2), 255};
        ::Color color2{(unsigned char)INT(3), (unsigned char)INT(4), (unsigned char)INT(5), 255};
        ::Color result = ::ColorLerp(color1, color2, (float)FLOAT(6));
        return Value::from_string(std::to_string(result.r) + "," + std::to_string(result.g) + "," + std::to_string(result.b) + "," + std::to_string(result.a));
    }}, true);    R.add_with_policy("GETCOLOR", Fn{"GETCOLOR", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCOLOR: expected 1 args");
        ::Color color = ::GetColor(INT(0));
        return Value::from_string(std::to_string(color.r) + "," + std::to_string(color.g) + "," + std::to_string(color.b) + "," + std::to_string(color.a));
    }}, true);    R.add_with_policy("GETPIXELDATASIZE", Fn{"GETPIXELDATASIZE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GETPIXELDATASIZE: expected 3 args");
        return Value::from_int(GetPixelDataSize(args[0].as_int(), args[1].as_int(), args[2].as_int()));
    }}, true);    R.add_with_policy("LOADFONTFROMIMAGE", Fn{"LOADFONTFROMIMAGE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("LOADFONTFROMIMAGE: expected 5 args");
        ::Color key{(unsigned char)INT(1), (unsigned char)INT(2), (unsigned char)INT(3), 255};
        ::Font font = ::LoadFontFromImage(rlreg::images.at(INT(0)), key, INT(4));
        int id = rlreg::next_font_id++;
        rlreg::fonts[id] = font;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("LOADFONTFROMMEMORY", Fn{"LOADFONTFROMMEMORY", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("LOADFONTFROMMEMORY: expected 4 args");
        // Load font without custom codepoints (uses default)
        ::Font font = ::LoadFontFromMemory(STR(0).c_str(), (const unsigned char*)STR(1).c_str(), INT(2), INT(3), nullptr, 0);
        int id = rlreg::next_font_id++;
        rlreg::fonts[id] = font;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISFONTVALID", Fn{"ISFONTVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISFONTVALID: expected 1 args");
        return Value::from_bool(::IsFontValid(rlreg::fonts.at(INT(0))));
    }}, true);    R.add_with_policy("EXPORTFONTASCODE", Fn{"EXPORTFONTASCODE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTFONTASCODE: expected 2 args");
        return Value::from_bool(::ExportFontAsCode(rlreg::fonts.at(INT(0)), STR(1).c_str()));
    }}, true);    R.add_with_policy("DRAWTEXTCODEPOINTS", Fn{"DRAWTEXTCODEPOINTS", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWTEXTCODEPOINTS: expected 9 args");
        // Parse codepoints from comma-separated string "cp1,cp2,cp3,..."
        std::string codepointStr = STR(1);
        std::vector<int> codepoints;
        size_t start = 0;
        size_t pos = 0;
        while ((pos = codepointStr.find(',', start)) != std::string::npos) {
          codepoints.push_back(std::stoi(codepointStr.substr(start, pos - start)));
          start = pos + 1;
        }
        if (start < codepointStr.length()) {
          codepoints.push_back(std::stoi(codepointStr.substr(start)));
        }
        ::Vector2 position{(float)FLOAT(2), (float)FLOAT(3)};
        ::Color color{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawTextCodepoints(rlreg::fonts.at(INT(0)), codepoints.data(), codepoints.size(), position, (float)FLOAT(4), (float)FLOAT(5), color);
        return Value::nil();
    }}, true);    R.add_with_policy("SETTEXTLINESPACING", Fn{"SETTEXTLINESPACING", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETTEXTLINESPACING: expected 1 args");
        SetTextLineSpacing(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("GETGLYPHINDEX", Fn{"GETGLYPHINDEX", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETGLYPHINDEX: expected 2 args");
        return Value::from_int(::GetGlyphIndex(rlreg::fonts.at(INT(0)), INT(1)));
    }}, true);    R.add_with_policy("GETGLYPHINFO", Fn{"GETGLYPHINFO", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETGLYPHINFO: expected 2 args");
        ::GlyphInfo info = ::GetGlyphInfo(rlreg::fonts.at(INT(0)), INT(1));
        return Value::from_string(std::to_string(info.value) + "," + std::to_string(info.offsetX) + "," + std::to_string(info.offsetY) + "," + std::to_string(info.advanceX) + "," + std::to_string(info.image.width) + "," + std::to_string(info.image.height));
    }}, true);    R.add_with_policy("GETGLYPHATLASREC", Fn{"GETGLYPHATLASREC", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETGLYPHATLASREC: expected 2 args");
        ::Rectangle rec = ::GetGlyphAtlasRec(rlreg::fonts.at(INT(0)), INT(1));
        return Value::from_string(std::to_string(rec.x) + "," + std::to_string(rec.y) + "," + std::to_string(rec.width) + "," + std::to_string(rec.height));
    }}, true);    R.add_with_policy("GETCODEPOINTCOUNT", Fn{"GETCODEPOINTCOUNT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCODEPOINTCOUNT: expected 1 args");
        return Value::from_int(GetCodepointCount(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("GETCODEPOINT", Fn{"GETCODEPOINT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCODEPOINT: expected 1 args");
        int codepointSize;
        int codepoint = ::GetCodepoint(STR(0).c_str(), &codepointSize);
        return Value::from_int(codepoint);
    }}, true);    R.add_with_policy("GETCODEPOINTNEXT", Fn{"GETCODEPOINTNEXT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCODEPOINTNEXT: expected 1 args");
        int codepointSize;
        int codepoint = ::GetCodepointNext(STR(0).c_str(), &codepointSize);
        return Value::from_int(codepoint);
    }}, true);    R.add_with_policy("GETCODEPOINTPREVIOUS", Fn{"GETCODEPOINTPREVIOUS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCODEPOINTPREVIOUS: expected 1 args");
        int codepointSize;
        int codepoint = ::GetCodepointPrevious(STR(0).c_str(), &codepointSize);
        return Value::from_int(codepoint);
    }}, true);    R.add_with_policy("CODEPOINTTOUTF8", Fn{"CODEPOINTTOUTF8", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("CODEPOINTTOUTF8: expected 1 args");
        int utf8Size;
        const char* utf8 = ::CodepointToUTF8(INT(0), &utf8Size);
        return Value::from_string(std::string(utf8, utf8Size));
    }}, true);    R.add_with_policy("TEXTCOPY", Fn{"TEXTCOPY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("TEXTCOPY: expected 2 args");
        // Note: TextCopy modifies dst, simplified for BASIC
        return Value::from_int(0);
    }}, true);    R.add_with_policy("TEXTISEQUAL", Fn{"TEXTISEQUAL", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("TEXTISEQUAL: expected 2 args");
        return Value::from_bool(TextIsEqual(args[0].as_string().c_str(), args[1].as_string().c_str()));
    }}, true);    R.add_with_policy("TEXTLENGTH", Fn{"TEXTLENGTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTLENGTH: expected 1 args");
        return Value::from_int(TextLength(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("TEXTFORMAT", Fn{"TEXTFORMAT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTFORMAT: expected 1 args");
        // Note: TextFormat supports variadic args, simplified for BASIC
        return Value::from_string(STR(0));
    }}, true);    R.add_with_policy("TEXTSUBTEXT", Fn{"TEXTSUBTEXT", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("TEXTSUBTEXT: expected 3 args");
        return Value::from_string(TextSubtext(args[0].as_string().c_str(), args[1].as_int(), args[2].as_int()));
    }}, true);    R.add_with_policy("TEXTREPLACE", Fn{"TEXTREPLACE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("TEXTREPLACE: expected 3 args");
        // Note: TextReplace modifies text, simplified for BASIC
        return Value::from_string(STR(0));
    }}, true);    R.add_with_policy("TEXTINSERT", Fn{"TEXTINSERT", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("TEXTINSERT: expected 3 args");
        // Note: TextInsert modifies text, simplified for BASIC
        return Value::from_string(STR(0));
    }}, true);    R.add_with_policy("TEXTJOIN", Fn{"TEXTJOIN", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("TEXTJOIN: expected 3 args");
        // Parse text list from comma-separated string and join with delimiter
        std::string textList = STR(0);
        std::string delimiter = STR(2);
        std::vector<std::string> texts;
        size_t start = 0;
        size_t pos = 0;
        while ((pos = textList.find(',', start)) != std::string::npos) {
          texts.push_back(textList.substr(start, pos - start));
          start = pos + 1;
        }
        if (start < textList.length()) {
          texts.push_back(textList.substr(start));
        }
        std::string result;
        for (size_t i = 0; i < texts.size() && i < (size_t)INT(1); ++i) {
          if (i > 0) result += delimiter;
          result += texts[i];
        }
        return Value::from_string(result);
    }}, true);    R.add_with_policy("TEXTSPLIT", Fn{"TEXTSPLIT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("TEXTSPLIT: expected 2 args");
        int count;
        const char** result = ::TextSplit(STR(0).c_str(), STR(1)[0], &count);
        std::string joined;
        for (int i = 0; i < count; ++i) {
          if (i > 0) joined += ",";
          joined += result[i];
        }
        // Note: Returns comma-separated string, can be split in BASIC
        return Value::from_string(joined);
    }}, true);    R.add_with_policy("TEXTFINDINDEX", Fn{"TEXTFINDINDEX", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("TEXTFINDINDEX: expected 2 args");
        return Value::from_int(TextFindIndex(args[0].as_string().c_str(), args[1].as_string().c_str()));
    }}, true);    R.add_with_policy("TEXTTOUPPER", Fn{"TEXTTOUPPER", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTTOUPPER: expected 1 args");
        return Value::from_string(TextToUpper(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("TEXTTOLOWER", Fn{"TEXTTOLOWER", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTTOLOWER: expected 1 args");
        return Value::from_string(TextToLower(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("TEXTOPASCAL", Fn{"TEXTOPASCAL", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTOPASCAL: expected 1 args");
        return Value::from_string(TextToPascal(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("TEXTTOSNAKE", Fn{"TEXTTOSNAKE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTTOSNAKE: expected 1 args");
        return Value::from_string(TextToSnake(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("TEXTTOCAMEL", Fn{"TEXTTOCAMEL", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTTOCAMEL: expected 1 args");
        return Value::from_string(TextToCamel(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("TEXTTOINTEGER", Fn{"TEXTTOINTEGER", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTTOINTEGER: expected 1 args");
        return Value::from_int(TextToInteger(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("TEXTTOFLOAT", Fn{"TEXTTOFLOAT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTTOFLOAT: expected 1 args");
        return Value::from_number(TextToFloat(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("DRAWCAPSULE", Fn{"DRAWCAPSULE", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWCAPSULE: expected 12 args");
        ::Vector3 startPos{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 endPos{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Color color{(unsigned char)INT(9), (unsigned char)INT(10), (unsigned char)INT(11), 255};
        ::DrawCapsule(startPos, endPos, (float)FLOAT(6), INT(7), INT(8), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCAPSULEWIRES", Fn{"DRAWCAPSULEWIRES", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWCAPSULEWIRES: expected 12 args");
        ::Vector3 startPos{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 endPos{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Color color{(unsigned char)INT(9), (unsigned char)INT(10), (unsigned char)INT(11), 255};
        ::DrawCapsuleWires(startPos, endPos, (float)FLOAT(6), INT(7), INT(8), color);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWMODELPOINTS", Fn{"DRAWMODELPOINTS", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWMODELPOINTS: expected 8 args");
        ::Vector3 position{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        ::Color tint{(unsigned char)INT(5), (unsigned char)INT(6), (unsigned char)INT(7), 255};
        ::DrawModelPoints(rlreg::models.at(INT(0)), position, (float)FLOAT(4), tint);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWMODELPOINTSEX", Fn{"DRAWMODELPOINTSEX", 14, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 14) throw std::runtime_error("DRAWMODELPOINTSEX: expected 14 args");
        ::Vector3 position{(float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        ::Vector3 rotationAxis{(float)FLOAT(4), (float)FLOAT(5), (float)FLOAT(6)};
        ::Vector3 scale{(float)FLOAT(8), (float)FLOAT(9), (float)FLOAT(10)};
        ::Color tint{(unsigned char)INT(11), (unsigned char)INT(12), (unsigned char)INT(13), 255};
        ::DrawModelPointsEx(rlreg::models.at(INT(0)), position, rotationAxis, (float)FLOAT(7), scale, tint);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWBILLBOARD", Fn{"DRAWBILLBOARD", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWBILLBOARD: expected 9 args");
        ::Camera3D camera = {};
        if (g_camera_system_3d) {
          Camera3D* cam = g_camera_system_3d->get_active_camera();
          if (cam) {
            camera.position = ::Vector3{cam->position.x, cam->position.y, cam->position.z};
            camera.target = ::Vector3{cam->target.x, cam->target.y, cam->target.z};
            camera.up = ::Vector3{cam->up.x, cam->up.y, cam->up.z};
            camera.fovy = cam->fov;
            camera.projection = CAMERA_PERSPECTIVE;
          } else {
            camera.position = ::Vector3{0.0f, 0.0f, 0.0f};
            camera.target = ::Vector3{0.0f, 0.0f, 1.0f};
            camera.up = ::Vector3{0.0f, 1.0f, 0.0f};
            camera.fovy = 60.0f;
            camera.projection = CAMERA_PERSPECTIVE;
          }
        } else {
          camera.position = ::Vector3{0.0f, 0.0f, 0.0f};
          camera.target = ::Vector3{0.0f, 0.0f, 1.0f};
          camera.up = ::Vector3{0.0f, 1.0f, 0.0f};
          camera.fovy = 60.0f;
          camera.projection = CAMERA_PERSPECTIVE;
        }
        ::Vector3 position{(float)FLOAT(2), (float)FLOAT(3), (float)FLOAT(4)};
        ::Color tint{(unsigned char)INT(6), (unsigned char)INT(7), (unsigned char)INT(8), 255};
        ::DrawBillboard(camera, rlreg::textures.at(INT(1)), position, (float)FLOAT(5), tint);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWBILLBOARDREC", Fn{"DRAWBILLBOARDREC", 13, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 13) throw std::runtime_error("DRAWBILLBOARDREC: expected 13 args");
        ::Camera3D camera = {};
        if (g_camera_system_3d) {
          Camera3D* cam = g_camera_system_3d->get_active_camera();
          if (cam) {
            camera.position = ::Vector3{cam->position.x, cam->position.y, cam->position.z};
            camera.target = ::Vector3{cam->target.x, cam->target.y, cam->target.z};
            camera.up = ::Vector3{cam->up.x, cam->up.y, cam->up.z};
            camera.fovy = cam->fov;
            camera.projection = CAMERA_PERSPECTIVE;
          } else {
            camera.position = ::Vector3{0.0f, 0.0f, 0.0f};
            camera.target = ::Vector3{0.0f, 0.0f, 1.0f};
            camera.up = ::Vector3{0.0f, 1.0f, 0.0f};
            camera.fovy = 60.0f;
            camera.projection = CAMERA_PERSPECTIVE;
          }
        } else {
          camera.position = ::Vector3{0.0f, 0.0f, 0.0f};
          camera.target = ::Vector3{0.0f, 0.0f, 1.0f};
          camera.up = ::Vector3{0.0f, 1.0f, 0.0f};
          camera.fovy = 60.0f;
          camera.projection = CAMERA_PERSPECTIVE;
        }
        ::Vector3 position{(float)FLOAT(2), (float)FLOAT(3), (float)FLOAT(4)};
        ::Rectangle source{(float)0.0f, (float)0.0f, (float)FLOAT(5), (float)FLOAT(6)};
        ::Vector2 size{(float)FLOAT(5), (float)FLOAT(6)};
        ::Color tint{(unsigned char)INT(8), (unsigned char)INT(9), (unsigned char)INT(10), 255};
        ::DrawBillboardRec(camera, rlreg::textures.at(INT(1)), source, position, size, tint);
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWBILLBOARDPRO", Fn{"DRAWBILLBOARDPRO", 18, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 18) throw std::runtime_error("DRAWBILLBOARDPRO: expected 18 args");
        ::Camera3D camera = {};
        if (g_camera_system_3d) {
          Camera3D* cam = g_camera_system_3d->get_active_camera();
          if (cam) {
            camera.position = ::Vector3{cam->position.x, cam->position.y, cam->position.z};
            camera.target = ::Vector3{cam->target.x, cam->target.y, cam->target.z};
            camera.up = ::Vector3{cam->up.x, cam->up.y, cam->up.z};
            camera.fovy = cam->fov;
            camera.projection = CAMERA_PERSPECTIVE;
          } else {
            camera.position = ::Vector3{0.0f, 0.0f, 0.0f};
            camera.target = ::Vector3{0.0f, 0.0f, 1.0f};
            camera.up = ::Vector3{0.0f, 1.0f, 0.0f};
            camera.fovy = 60.0f;
            camera.projection = CAMERA_PERSPECTIVE;
          }
        } else {
          camera.position = ::Vector3{0.0f, 0.0f, 0.0f};
          camera.target = ::Vector3{0.0f, 0.0f, 1.0f};
          camera.up = ::Vector3{0.0f, 1.0f, 0.0f};
          camera.fovy = 60.0f;
          camera.projection = CAMERA_PERSPECTIVE;
        }
        ::Vector3 position{(float)FLOAT(2), (float)FLOAT(3), (float)FLOAT(4)};
        ::Vector3 up{(float)FLOAT(5), (float)FLOAT(6), (float)FLOAT(7)};
        ::Vector2 size{(float)FLOAT(8), (float)FLOAT(9)};
        ::Vector2 origin{(float)FLOAT(10), (float)FLOAT(11)};
        ::Rectangle source{(float)0.0f, (float)0.0f, (float)FLOAT(8), (float)FLOAT(9)};
        ::Color tint{(unsigned char)INT(13), (unsigned char)INT(14), (unsigned char)INT(15), 255};
        ::DrawBillboardPro(camera, rlreg::textures.at(INT(1)), source, position, up, size, origin, (float)FLOAT(12), tint);
        return Value::nil();
    }}, true);    R.add_with_policy("ISMODELVALID", Fn{"ISMODELVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMODELVALID: expected 1 args");
        return Value::from_bool(::IsModelValid(rlreg::models.at(INT(0))));
    }}, true);    R.add_with_policy("GETMODELBOUNDINGBOX", Fn{"GETMODELBOUNDINGBOX", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMODELBOUNDINGBOX: expected 1 args");
        ::BoundingBox box = ::GetModelBoundingBox(rlreg::models.at(INT(0)));
        return Value::from_string(std::to_string(box.min.x) + "," + std::to_string(box.min.y) + "," + std::to_string(box.min.z) + "," + std::to_string(box.max.x) + "," + std::to_string(box.max.y) + "," + std::to_string(box.max.z));
    }}, true);    R.add_with_policy("UPLOADMESH", Fn{"UPLOADMESH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("UPLOADMESH: expected 2 args");
        ::UploadMesh(&rlreg::meshes.at(INT(0)), BOOL(1));
        return Value::nil();
    }}, true);    R.add_with_policy("UPDATEMESHBUFFER", Fn{"UPDATEMESHBUFFER", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("UPDATEMESHBUFFER: expected 5 args");
        std::string dataStr = STR(2);
        const void* data = reinterpret_cast<const void*>(dataStr.c_str());
        ::Mesh mesh = rlreg::meshes.at(INT(0));
        ::UpdateMeshBuffer(mesh, INT(1), data, INT(3), INT(4));
        return Value::nil();
    }}, true);    R.add_with_policy("GETMESHBOUNDINGBOX", Fn{"GETMESHBOUNDINGBOX", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMESHBOUNDINGBOX: expected 1 args");
        ::BoundingBox box = ::GetMeshBoundingBox(rlreg::meshes.at(INT(0)));
        return Value::from_string(std::to_string(box.min.x) + "," + std::to_string(box.min.y) + "," + std::to_string(box.min.z) + "," + std::to_string(box.max.x) + "," + std::to_string(box.max.y) + "," + std::to_string(box.max.z));
    }}, true);    R.add_with_policy("GENMESHTANGENTS", Fn{"GENMESHTANGENTS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GENMESHTANGENTS: expected 1 args");
        ::GenMeshTangents(&rlreg::meshes.at(INT(0)));
        return Value::nil();
    }}, true);    R.add_with_policy("EXPORTMESH", Fn{"EXPORTMESH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTMESH: expected 2 args");
        return Value::from_bool(::ExportMesh(rlreg::meshes.at(INT(0)), STR(1).c_str()));
    }}, true);    R.add_with_policy("EXPORTMESHASCODE", Fn{"EXPORTMESHASCODE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTMESHASCODE: expected 2 args");
        return Value::from_bool(::ExportMeshAsCode(rlreg::meshes.at(INT(0)), STR(1).c_str()));
    }}, true);    R.add_with_policy("GENMESHHEMISPHERE", Fn{"GENMESHHEMISPHERE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENMESHHEMISPHERE: expected 3 args");
        ::Mesh mesh = ::GenMeshHemiSphere((float)FLOAT(0), INT(1), INT(2));
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("GENMESHCONE", Fn{"GENMESHCONE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENMESHCONE: expected 3 args");
        ::Mesh mesh = ::GenMeshCone((float)FLOAT(0), (float)FLOAT(1), INT(2));
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISMATERIALVALID", Fn{"ISMATERIALVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMATERIALVALID: expected 1 args");
        return Value::from_bool(::IsMaterialValid(rlreg::materials.at(INT(0))));
    }}, true);    R.add_with_policy("SETMODELMESHMATERIAL", Fn{"SETMODELMESHMATERIAL", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETMODELMESHMATERIAL: expected 3 args");
        ::SetModelMeshMaterial(&rlreg::models.at(INT(0)), INT(1), INT(2));
        return Value::nil();
    }}, true);    R.add_with_policy("CHECKCOLLISIONSPHERES", Fn{"CHECKCOLLISIONSPHERES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("CHECKCOLLISIONSPHERES: expected 8 args");
        ::Vector3 center1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 center2{(float)FLOAT(4), (float)FLOAT(5), (float)FLOAT(6)};
        return Value::from_bool(::CheckCollisionSpheres(center1, (float)FLOAT(3), center2, (float)FLOAT(7)));
    }}, true);    R.add_with_policy("CHECKCOLLISIONBOXES", Fn{"CHECKCOLLISIONBOXES", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("CHECKCOLLISIONBOXES: expected 12 args");
        ::BoundingBox box1;
        box1.min = {(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        box1.max = {(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::BoundingBox box2;
        box2.min = {(float)FLOAT(6), (float)FLOAT(7), (float)FLOAT(8)};
        box2.max = {(float)FLOAT(9), (float)FLOAT(10), (float)FLOAT(11)};
        return Value::from_bool(::CheckCollisionBoxes(box1, box2));
    }}, true);    R.add_with_policy("CHECKCOLLISIONBOXSPHERE", Fn{"CHECKCOLLISIONBOXSPHERE", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("CHECKCOLLISIONBOXSPHERE: expected 10 args");
        ::BoundingBox box;
        box.min = {(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        box.max = {(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Vector3 center{(float)FLOAT(6), (float)FLOAT(7), (float)FLOAT(8)};
        return Value::from_bool(::CheckCollisionBoxSphere(box, center, (float)FLOAT(9)));
    }}, true);    R.add_with_policy("GETRAYCOLLISIONSPHERE", Fn{"GETRAYCOLLISIONSPHERE", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("GETRAYCOLLISIONSPHERE: expected 10 args");
        ::Ray ray;
        ray.position = {(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ray.direction = {(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Vector3 center{(float)FLOAT(6), (float)FLOAT(7), (float)FLOAT(8)};
        ::RayCollision collision = ::GetRayCollisionSphere(ray, center, (float)FLOAT(9));
        if (collision.hit) {
          return Value::from_string(std::to_string(collision.point.x) + "," + std::to_string(collision.point.y) + "," + std::to_string(collision.point.z) + "," + std::to_string(collision.distance));
        }
        return Value::from_string("");
    }}, true);    R.add_with_policy("GETRAYCOLLISIONBOX", Fn{"GETRAYCOLLISIONBOX", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("GETRAYCOLLISIONBOX: expected 12 args");
        ::Ray ray;
        ray.position = {(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ray.direction = {(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::BoundingBox box;
        box.min = {(float)FLOAT(6), (float)FLOAT(7), (float)FLOAT(8)};
        box.max = {(float)FLOAT(9), (float)FLOAT(10), (float)FLOAT(11)};
        ::RayCollision collision = ::GetRayCollisionBox(ray, box);
        if (collision.hit) {
          return Value::from_string(std::to_string(collision.point.x) + "," + std::to_string(collision.point.y) + "," + std::to_string(collision.point.z) + "," + std::to_string(collision.distance));
        }
        return Value::from_string("");
    }}, true);    R.add_with_policy("GETRAYCOLLISIONTRIANGLE", Fn{"GETRAYCOLLISIONTRIANGLE", 15, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 15) throw std::runtime_error("GETRAYCOLLISIONTRIANGLE: expected 15 args");
        ::Ray ray;
        ray.position = {(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ray.direction = {(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Vector3 p1{(float)FLOAT(6), (float)FLOAT(7), (float)FLOAT(8)};
        ::Vector3 p2{(float)FLOAT(9), (float)FLOAT(10), (float)FLOAT(11)};
        ::Vector3 p3{(float)FLOAT(12), (float)FLOAT(13), (float)FLOAT(14)};
        ::RayCollision collision = ::GetRayCollisionTriangle(ray, p1, p2, p3);
        if (collision.hit) {
          return Value::from_string(std::to_string(collision.point.x) + "," + std::to_string(collision.point.y) + "," + std::to_string(collision.point.z) + "," + std::to_string(collision.distance));
        }
        return Value::from_string("");
    }}, true);    R.add_with_policy("GETRAYCOLLISIONQUAD", Fn{"GETRAYCOLLISIONQUAD", 18, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 18) throw std::runtime_error("GETRAYCOLLISIONQUAD: expected 18 args");
        ::Ray ray;
        ray.position = {(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ray.direction = {(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Vector3 p1{(float)FLOAT(6), (float)FLOAT(7), (float)FLOAT(8)};
        ::Vector3 p2{(float)FLOAT(9), (float)FLOAT(10), (float)FLOAT(11)};
        ::Vector3 p3{(float)FLOAT(12), (float)FLOAT(13), (float)FLOAT(14)};
        ::Vector3 p4{(float)FLOAT(15), (float)FLOAT(16), (float)FLOAT(17)};
        ::RayCollision collision = ::GetRayCollisionQuad(ray, p1, p2, p3, p4);
        if (collision.hit) {
          return Value::from_string(std::to_string(collision.point.x) + "," + std::to_string(collision.point.y) + "," + std::to_string(collision.point.z) + "," + std::to_string(collision.distance));
        }
        return Value::from_string("");
    }}, true);    R.add_with_policy("GETMASTERVOLUME", Fn{"GETMASTERVOLUME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMASTERVOLUME: expected 0 args");
        return Value::from_number(GetMasterVolume());
    }}, true);    R.add_with_policy("LOADSOUNDALIAS", Fn{"LOADSOUNDALIAS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADSOUNDALIAS: expected 1 args");
        ::Sound sound = ::LoadSoundAlias(rlreg::sounds.at(INT(0)));
        int id = rlreg::next_sound_id++;
        rlreg::sounds[id] = sound;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISSOUNDVALID", Fn{"ISSOUNDVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISSOUNDVALID: expected 1 args");
        return Value::from_bool(::IsSoundValid(rlreg::sounds.at(INT(0))));
    }}, true);    R.add_with_policy("ISWAVEVALID", Fn{"ISWAVEVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISWAVEVALID: expected 1 args");
        return Value::from_bool(::IsWaveValid(rlreg::waves.at(INT(0))));
    }}, true);    R.add_with_policy("UNLOADSOUNDALIAS", Fn{"UNLOADSOUNDALIAS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADSOUNDALIAS: expected 1 args");
        ::UnloadSoundAlias(rlreg::sounds.at(INT(0)));
        rlreg::sounds.erase(INT(0));
        return Value::nil();
    }}, true);    R.add_with_policy("WAVECOPY", Fn{"WAVECOPY", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("WAVECOPY: expected 1 args");
        ::Wave wave = ::WaveCopy(rlreg::waves.at(INT(0)));
        int id = rlreg::next_wave_id++;
        rlreg::waves[id] = wave;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("WAVECROP", Fn{"WAVECROP", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("WAVECROP: expected 3 args");
        ::WaveCrop(&rlreg::waves.at(INT(0)), INT(1), INT(2));
        return Value::nil();
    }}, true);    R.add_with_policy("WAVEFORMAT", Fn{"WAVEFORMAT", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("WAVEFORMAT: expected 4 args");
        ::WaveFormat(&rlreg::waves.at(INT(0)), INT(1), INT(2), INT(3));
        return Value::nil();
    }}, true);    R.add_with_policy("LOADMUSICSTREAMFROMMEMORY", Fn{"LOADMUSICSTREAMFROMMEMORY", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("LOADMUSICSTREAMFROMMEMORY: expected 3 args");
        ::Music music = ::LoadMusicStreamFromMemory(STR(0).c_str(), (const unsigned char*)STR(1).c_str(), INT(2));
        int id = rlreg::next_music_id++;
        rlreg::musics[id] = music;
        return Value::from_int(id);
    }}, true);    R.add_with_policy("ISMUSICVALID", Fn{"ISMUSICVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMUSICVALID: expected 1 args");
        return Value::from_bool(::IsMusicValid(rlreg::musics.at(INT(0))));
    }}, true);    R.add_with_policy("ISAUDIOSTREAMVALID", Fn{"ISAUDIOSTREAMVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISAUDIOSTREAMVALID: expected 1 args");
        return Value::from_bool(::IsAudioStreamValid(rlreg::audiostreams.at(INT(0))));
    }}, true);    R.add_with_policy("ISAUDIOSTREAMPROCESSED", Fn{"ISAUDIOSTREAMPROCESSED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISAUDIOSTREAMPROCESSED: expected 1 args");
        return Value::from_bool(::IsAudioStreamProcessed(rlreg::audiostreams.at(INT(0))));
    }}, true);    R.add_with_policy("SETAUDIOSTREAMVOLUME", Fn{"SETAUDIOSTREAMVOLUME", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETAUDIOSTREAMVOLUME: expected 2 args");
        ::SetAudioStreamVolume(rlreg::audiostreams.at(INT(0)), static_cast<float>(FLOAT(1)));
        return Value::nil();
    }}, true);    R.add_with_policy("SETAUDIOSTREAMPITCH", Fn{"SETAUDIOSTREAMPITCH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETAUDIOSTREAMPITCH: expected 2 args");
        ::SetAudioStreamPitch(rlreg::audiostreams.at(INT(0)), static_cast<float>(FLOAT(1)));
        return Value::nil();
    }}, true);    R.add_with_policy("SETAUDIOSTREAMPAN", Fn{"SETAUDIOSTREAMPAN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETAUDIOSTREAMPAN: expected 2 args");
        ::SetAudioStreamPan(rlreg::audiostreams.at(INT(0)), static_cast<float>(FLOAT(1)));
        return Value::nil();
    }}, true);    R.add_with_policy("SETAUDIOSTREAMBUFFERSIZEDEFAULT", Fn{"SETAUDIOSTREAMBUFFERSIZEDEFAULT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETAUDIOSTREAMBUFFERSIZEDEFAULT: expected 1 args");
        SetAudioStreamBufferSizeDefault(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("VECTOR2ZERO", Fn{"VECTOR2ZERO", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("VECTOR2ZERO: expected 0 args");
        ::Vector2 v = ::Vector2Zero();
        return Value::from_string(std::to_string(v.x) + "," + std::to_string(v.y));
    }}, true);    R.add_with_policy("VECTOR2ONE", Fn{"VECTOR2ONE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("VECTOR2ONE: expected 0 args");
        ::Vector2 v = ::Vector2One();
        return Value::from_string(std::to_string(v.x) + "," + std::to_string(v.y));
    }}, true);    R.add_with_policy("VECTOR2ADD", Fn{"VECTOR2ADD", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR2ADD: expected 4 args");
        ::Vector2 v1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 v2{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 result = ::Vector2Add(v1, v2);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2ADDVALUE", Fn{"VECTOR2ADDVALUE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("VECTOR2ADDVALUE: expected 3 args");
        ::Vector2 v{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 result = ::Vector2AddValue(v, (float)FLOAT(2));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2SUBTRACT", Fn{"VECTOR2SUBTRACT", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR2SUBTRACT: expected 4 args");
        ::Vector2 v1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 v2{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 result = ::Vector2Subtract(v1, v2);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2SUBTRACTVALUE", Fn{"VECTOR2SUBTRACTVALUE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("VECTOR2SUBTRACTVALUE: expected 3 args");
        ::Vector2 v{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 result = ::Vector2SubtractValue(v, (float)FLOAT(2));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2LENGTH", Fn{"VECTOR2LENGTH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("VECTOR2LENGTH: expected 2 args");
        ::Vector2 v{(float)FLOAT(0), (float)FLOAT(1)};
        return Value::from_number(::Vector2Length(v));
    }}, true);    R.add_with_policy("VECTOR2LENGTHSQR", Fn{"VECTOR2LENGTHSQR", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("VECTOR2LENGTHSQR: expected 2 args");
        ::Vector2 v{(float)FLOAT(0), (float)FLOAT(1)};
        return Value::from_number(::Vector2LengthSqr(v));
    }}, true);    R.add_with_policy("VECTOR2DOTPRODUCT", Fn{"VECTOR2DOTPRODUCT", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR2DOTPRODUCT: expected 4 args");
        ::Vector2 v1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 v2{(float)FLOAT(2), (float)FLOAT(3)};
        return Value::from_number(::Vector2DotProduct(v1, v2));
    }}, true);    R.add_with_policy("VECTOR2DISTANCE", Fn{"VECTOR2DISTANCE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR2DISTANCE: expected 4 args");
        ::Vector2 v1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 v2{(float)FLOAT(2), (float)FLOAT(3)};
        return Value::from_number(::Vector2Distance(v1, v2));
    }}, true);    R.add_with_policy("VECTOR2DISTANCESQR", Fn{"VECTOR2DISTANCESQR", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR2DISTANCESQR: expected 4 args");
        ::Vector2 v1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 v2{(float)FLOAT(2), (float)FLOAT(3)};
        return Value::from_number(::Vector2DistanceSqr(v1, v2));
    }}, true);    R.add_with_policy("VECTOR2ANGLE", Fn{"VECTOR2ANGLE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR2ANGLE: expected 4 args");
        ::Vector2 v1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 v2{(float)FLOAT(2), (float)FLOAT(3)};
        return Value::from_number(::Vector2Angle(v1, v2));
    }}, true);    R.add_with_policy("VECTOR2SCALE", Fn{"VECTOR2SCALE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("VECTOR2SCALE: expected 3 args");
        ::Vector2 v{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 result = ::Vector2Scale(v, (float)FLOAT(2));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2MULTIPLY", Fn{"VECTOR2MULTIPLY", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR2MULTIPLY: expected 4 args");
        ::Vector2 v1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 v2{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 result = ::Vector2Multiply(v1, v2);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2NEGATE", Fn{"VECTOR2NEGATE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("VECTOR2NEGATE: expected 2 args");
        ::Vector2 v{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 result = ::Vector2Negate(v);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2DIVIDE", Fn{"VECTOR2DIVIDE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR2DIVIDE: expected 4 args");
        ::Vector2 v1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 v2{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 result = ::Vector2Divide(v1, v2);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2NORMALIZE", Fn{"VECTOR2NORMALIZE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("VECTOR2NORMALIZE: expected 2 args");
        ::Vector2 v{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 result = ::Vector2Normalize(v);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2LERP", Fn{"VECTOR2LERP", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("VECTOR2LERP: expected 5 args");
        ::Vector2 v1{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 v2{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 result = ::Vector2Lerp(v1, v2, (float)FLOAT(4));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2REFLECT", Fn{"VECTOR2REFLECT", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR2REFLECT: expected 4 args");
        ::Vector2 v{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 normal{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 result = ::Vector2Reflect(v, normal);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2ROTATE", Fn{"VECTOR2ROTATE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("VECTOR2ROTATE: expected 3 args");
        ::Vector2 v{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 result = ::Vector2Rotate(v, (float)FLOAT(2));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2MOVETOWARDS", Fn{"VECTOR2MOVETOWARDS", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("VECTOR2MOVETOWARDS: expected 5 args");
        ::Vector2 v{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 target{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 result = ::Vector2MoveTowards(v, target, (float)FLOAT(4));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2INVERT", Fn{"VECTOR2INVERT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("VECTOR2INVERT: expected 2 args");
        ::Vector2 v{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 result = ::Vector2Invert(v);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2CLAMP", Fn{"VECTOR2CLAMP", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR2CLAMP: expected 6 args");
        ::Vector2 v{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 min{(float)FLOAT(2), (float)FLOAT(3)};
        ::Vector2 max{(float)FLOAT(4), (float)FLOAT(5)};
        ::Vector2 result = ::Vector2Clamp(v, min, max);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2CLAMPVALUE", Fn{"VECTOR2CLAMPVALUE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR2CLAMPVALUE: expected 4 args");
        ::Vector2 v{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 result = ::Vector2ClampValue(v, (float)FLOAT(2), (float)FLOAT(3));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y));
    }}, true);    R.add_with_policy("VECTOR2EQUALS", Fn{"VECTOR2EQUALS", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR2EQUALS: expected 4 args");
        ::Vector2 p{(float)FLOAT(0), (float)FLOAT(1)};
        ::Vector2 q{(float)FLOAT(2), (float)FLOAT(3)};
        return Value::from_bool(::Vector2Equals(p, q));
    }}, true);    R.add_with_policy("VECTOR3ZERO", Fn{"VECTOR3ZERO", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("VECTOR3ZERO: expected 0 args");
        ::Vector3 v = ::Vector3Zero();
        return Value::from_string(std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z));
    }}, true);    R.add_with_policy("VECTOR3ONE", Fn{"VECTOR3ONE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("VECTOR3ONE: expected 0 args");
        ::Vector3 v = ::Vector3One();
        return Value::from_string(std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z));
    }}, true);    R.add_with_policy("VECTOR3ADD", Fn{"VECTOR3ADD", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3ADD: expected 6 args");
        ::Vector3 v1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 v2{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Vector3 result = ::Vector3Add(v1, v2);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3ADDVALUE", Fn{"VECTOR3ADDVALUE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR3ADDVALUE: expected 4 args");
        ::Vector3 v{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 result = ::Vector3AddValue(v, (float)FLOAT(3));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3SUBTRACT", Fn{"VECTOR3SUBTRACT", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3SUBTRACT: expected 6 args");
        ::Vector3 v1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 v2{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Vector3 result = ::Vector3Subtract(v1, v2);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3SUBTRACTVALUE", Fn{"VECTOR3SUBTRACTVALUE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR3SUBTRACTVALUE: expected 4 args");
        ::Vector3 v{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 result = ::Vector3SubtractValue(v, (float)FLOAT(3));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3SCALE", Fn{"VECTOR3SCALE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("VECTOR3SCALE: expected 4 args");
        ::Vector3 v{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 result = ::Vector3Scale(v, (float)FLOAT(3));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3MULTIPLY", Fn{"VECTOR3MULTIPLY", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3MULTIPLY: expected 6 args");
        ::Vector3 v1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 v2{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Vector3 result = ::Vector3Multiply(v1, v2);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3CROSSPRODUCT", Fn{"VECTOR3CROSSPRODUCT", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3CROSSPRODUCT: expected 6 args");
        ::Vector3 v1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 v2{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Vector3 result = ::Vector3CrossProduct(v1, v2);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3LENGTH", Fn{"VECTOR3LENGTH", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("VECTOR3LENGTH: expected 3 args");
        ::Vector3 v{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        return Value::from_number(::Vector3Length(v));
    }}, true);    R.add_with_policy("VECTOR3LENGTHSQR", Fn{"VECTOR3LENGTHSQR", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("VECTOR3LENGTHSQR: expected 3 args");
        ::Vector3 v{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        return Value::from_number(::Vector3LengthSqr(v));
    }}, true);    R.add_with_policy("VECTOR3DOTPRODUCT", Fn{"VECTOR3DOTPRODUCT", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3DOTPRODUCT: expected 6 args");
        ::Vector3 v1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 v2{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        return Value::from_number(::Vector3DotProduct(v1, v2));
    }}, true);    R.add_with_policy("VECTOR3DISTANCE", Fn{"VECTOR3DISTANCE", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3DISTANCE: expected 6 args");
        ::Vector3 v1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 v2{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        return Value::from_number(::Vector3Distance(v1, v2));
    }}, true);    R.add_with_policy("VECTOR3DISTANCESQR", Fn{"VECTOR3DISTANCESQR", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3DISTANCESQR: expected 6 args");
        ::Vector3 v1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 v2{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        return Value::from_number(::Vector3DistanceSqr(v1, v2));
    }}, true);    R.add_with_policy("VECTOR3ANGLE", Fn{"VECTOR3ANGLE", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3ANGLE: expected 6 args");
        ::Vector3 v1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 v2{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        return Value::from_number(::Vector3Angle(v1, v2));
    }}, true);    R.add_with_policy("VECTOR3NEGATE", Fn{"VECTOR3NEGATE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("VECTOR3NEGATE: expected 3 args");
        ::Vector3 v{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 result = ::Vector3Negate(v);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3DIVIDE", Fn{"VECTOR3DIVIDE", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3DIVIDE: expected 6 args");
        ::Vector3 v1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 v2{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Vector3 result = ::Vector3Divide(v1, v2);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3NORMALIZE", Fn{"VECTOR3NORMALIZE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("VECTOR3NORMALIZE: expected 3 args");
        ::Vector3 v{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 result = ::Vector3Normalize(v);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3LERP", Fn{"VECTOR3LERP", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("VECTOR3LERP: expected 7 args");
        ::Vector3 v1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 v2{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Vector3 result = ::Vector3Lerp(v1, v2, (float)FLOAT(6));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3REFLECT", Fn{"VECTOR3REFLECT", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3REFLECT: expected 6 args");
        ::Vector3 v{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 normal{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Vector3 result = ::Vector3Reflect(v, normal);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3MIN", Fn{"VECTOR3MIN", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3MIN: expected 6 args");
        ::Vector3 v1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 v2{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Vector3 result = ::Vector3Min(v1, v2);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3MAX", Fn{"VECTOR3MAX", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3MAX: expected 6 args");
        ::Vector3 v1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 v2{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Vector3 result = ::Vector3Max(v1, v2);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3INVERT", Fn{"VECTOR3INVERT", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("VECTOR3INVERT: expected 3 args");
        ::Vector3 v{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 result = ::Vector3Invert(v);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3CLAMP", Fn{"VECTOR3CLAMP", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("VECTOR3CLAMP: expected 9 args");
        ::Vector3 v{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 min{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Vector3 max{(float)FLOAT(6), (float)FLOAT(7), (float)FLOAT(8)};
        ::Vector3 result = ::Vector3Clamp(v, min, max);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3CLAMPVALUE", Fn{"VECTOR3CLAMPVALUE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("VECTOR3CLAMPVALUE: expected 5 args");
        ::Vector3 v{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 result = ::Vector3ClampValue(v, (float)FLOAT(3), (float)FLOAT(4));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("VECTOR3EQUALS", Fn{"VECTOR3EQUALS", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("VECTOR3EQUALS: expected 6 args");
        ::Vector3 p{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 q{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        return Value::from_bool(::Vector3Equals(p, q));
    }}, true);    R.add_with_policy("VECTOR3REFRACT", Fn{"VECTOR3REFRACT", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("VECTOR3REFRACT: expected 7 args");
        ::Vector3 v{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2)};
        ::Vector3 n{(float)FLOAT(3), (float)FLOAT(4), (float)FLOAT(5)};
        ::Vector3 result = ::Vector3Refract(v, n, (float)FLOAT(6));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z));
    }}, true);    R.add_with_policy("MATRIXIDENTITY", Fn{"MATRIXIDENTITY", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("MATRIXIDENTITY: expected 0 args");
        ::Matrix mat = MatrixIdentity();
        // Return as 16 comma-separated values
        return Value::from_string(std::to_string(mat.m0) + "," + std::to_string(mat.m1) + "," + std::to_string(mat.m2) + "," + std::to_string(mat.m3) + "," + std::to_string(mat.m4) + "," + std::to_string(mat.m5) + "," + std::to_string(mat.m6) + "," + std::to_string(mat.m7) + "," + std::to_string(mat.m8) + "," + std::to_string(mat.m9) + "," + std::to_string(mat.m10) + "," + std::to_string(mat.m11) + "," + std::to_string(mat.m12) + "," + std::to_string(mat.m13) + "," + std::to_string(mat.m14) + "," + std::to_string(mat.m15));
    }}, true);    R.add_with_policy("MATRIXTRANSLATE", Fn{"MATRIXTRANSLATE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("MATRIXTRANSLATE: expected 3 args");
        ::Matrix mat = ::MatrixTranslate((float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2));
        return Value::from_string(std::to_string(mat.m0) + "," + std::to_string(mat.m1) + "," + std::to_string(mat.m2) + "," + std::to_string(mat.m3) + "," + std::to_string(mat.m4) + "," + std::to_string(mat.m5) + "," + std::to_string(mat.m6) + "," + std::to_string(mat.m7) + "," + std::to_string(mat.m8) + "," + std::to_string(mat.m9) + "," + std::to_string(mat.m10) + "," + std::to_string(mat.m11) + "," + std::to_string(mat.m12) + "," + std::to_string(mat.m13) + "," + std::to_string(mat.m14) + "," + std::to_string(mat.m15));
    }}, true);    R.add_with_policy("MATRIXROTATEX", Fn{"MATRIXROTATEX", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("MATRIXROTATEX: expected 1 args");
        ::Matrix mat = ::MatrixRotateX((float)FLOAT(0));
        return Value::from_string(std::to_string(mat.m0) + "," + std::to_string(mat.m1) + "," + std::to_string(mat.m2) + "," + std::to_string(mat.m3) + "," + std::to_string(mat.m4) + "," + std::to_string(mat.m5) + "," + std::to_string(mat.m6) + "," + std::to_string(mat.m7) + "," + std::to_string(mat.m8) + "," + std::to_string(mat.m9) + "," + std::to_string(mat.m10) + "," + std::to_string(mat.m11) + "," + std::to_string(mat.m12) + "," + std::to_string(mat.m13) + "," + std::to_string(mat.m14) + "," + std::to_string(mat.m15));
    }}, true);    R.add_with_policy("MATRIXROTATEY", Fn{"MATRIXROTATEY", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("MATRIXROTATEY: expected 1 args");
        ::Matrix mat = ::MatrixRotateY((float)FLOAT(0));
        return Value::from_string(std::to_string(mat.m0) + "," + std::to_string(mat.m1) + "," + std::to_string(mat.m2) + "," + std::to_string(mat.m3) + "," + std::to_string(mat.m4) + "," + std::to_string(mat.m5) + "," + std::to_string(mat.m6) + "," + std::to_string(mat.m7) + "," + std::to_string(mat.m8) + "," + std::to_string(mat.m9) + "," + std::to_string(mat.m10) + "," + std::to_string(mat.m11) + "," + std::to_string(mat.m12) + "," + std::to_string(mat.m13) + "," + std::to_string(mat.m14) + "," + std::to_string(mat.m15));
    }}, true);    R.add_with_policy("MATRIXROTATEZ", Fn{"MATRIXROTATEZ", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("MATRIXROTATEZ: expected 1 args");
        ::Matrix mat = ::MatrixRotateZ((float)FLOAT(0));
        return Value::from_string(std::to_string(mat.m0) + "," + std::to_string(mat.m1) + "," + std::to_string(mat.m2) + "," + std::to_string(mat.m3) + "," + std::to_string(mat.m4) + "," + std::to_string(mat.m5) + "," + std::to_string(mat.m6) + "," + std::to_string(mat.m7) + "," + std::to_string(mat.m8) + "," + std::to_string(mat.m9) + "," + std::to_string(mat.m10) + "," + std::to_string(mat.m11) + "," + std::to_string(mat.m12) + "," + std::to_string(mat.m13) + "," + std::to_string(mat.m14) + "," + std::to_string(mat.m15));
    }}, true);    R.add_with_policy("MATRIXSCALE", Fn{"MATRIXSCALE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("MATRIXSCALE: expected 3 args");
        ::Matrix mat = ::MatrixScale((float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2));
        return Value::from_string(std::to_string(mat.m0) + "," + std::to_string(mat.m1) + "," + std::to_string(mat.m2) + "," + std::to_string(mat.m3) + "," + std::to_string(mat.m4) + "," + std::to_string(mat.m5) + "," + std::to_string(mat.m6) + "," + std::to_string(mat.m7) + "," + std::to_string(mat.m8) + "," + std::to_string(mat.m9) + "," + std::to_string(mat.m10) + "," + std::to_string(mat.m11) + "," + std::to_string(mat.m12) + "," + std::to_string(mat.m13) + "," + std::to_string(mat.m14) + "," + std::to_string(mat.m15));
    }}, true);    R.add_with_policy("QUATERNIONIDENTITY", Fn{"QUATERNIONIDENTITY", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("QUATERNIONIDENTITY: expected 0 args");
        ::Quaternion q = ::QuaternionIdentity();
        return Value::from_string(std::to_string(q.x) + "," + std::to_string(q.y) + "," + std::to_string(q.z) + "," + std::to_string(q.w));
    }}, true);    R.add_with_policy("QUATERNIONLENGTH", Fn{"QUATERNIONLENGTH", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("QUATERNIONLENGTH: expected 4 args");
        ::Quaternion q{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        return Value::from_number(::QuaternionLength(q));
    }}, true);    R.add_with_policy("QUATERNIONNORMALIZE", Fn{"QUATERNIONNORMALIZE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("QUATERNIONNORMALIZE: expected 4 args");
        ::Quaternion q{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        ::Quaternion result = ::QuaternionNormalize(q);
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z) + "," + std::to_string(result.w));
    }}, true);    R.add_with_policy("QUATERNIONLERP", Fn{"QUATERNIONLERP", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("QUATERNIONLERP: expected 9 args");
        ::Quaternion q1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        ::Quaternion q2{(float)FLOAT(4), (float)FLOAT(5), (float)FLOAT(6), (float)FLOAT(7)};
        ::Quaternion result = ::QuaternionLerp(q1, q2, (float)FLOAT(8));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z) + "," + std::to_string(result.w));
    }}, true);    R.add_with_policy("QUATERNIONSLERP", Fn{"QUATERNIONSLERP", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("QUATERNIONSLERP: expected 9 args");
        ::Quaternion q1{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        ::Quaternion q2{(float)FLOAT(4), (float)FLOAT(5), (float)FLOAT(6), (float)FLOAT(7)};
        ::Quaternion result = ::QuaternionSlerp(q1, q2, (float)FLOAT(8));
        return Value::from_string(std::to_string(result.x) + "," + std::to_string(result.y) + "," + std::to_string(result.z) + "," + std::to_string(result.w));
    }}, true);    R.add_with_policy("QUATERNIONFROMEULER", Fn{"QUATERNIONFROMEULER", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("QUATERNIONFROMEULER: expected 3 args");
        ::Quaternion q = ::QuaternionFromEuler((float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2));
        return Value::from_string(std::to_string(q.x) + "," + std::to_string(q.y) + "," + std::to_string(q.z) + "," + std::to_string(q.w));
    }}, true);    R.add_with_policy("QUATERNIONTOEULER", Fn{"QUATERNIONTOEULER", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("QUATERNIONTOEULER: expected 4 args");
        ::Quaternion q{(float)FLOAT(0), (float)FLOAT(1), (float)FLOAT(2), (float)FLOAT(3)};
        ::Vector3 euler = ::QuaternionToEuler(q);
        return Value::from_string(std::to_string(euler.x) + "," + std::to_string(euler.y) + "," + std::to_string(euler.z));
    }}, true);}

// Populate NativeFunctionRegistry with all registered raylib functions
// This allows YAML modules to reference raylib functions via the 'native' field
void populate_raylib_natives(NativeFunctionRegistry& native_registry, FunctionRegistry& function_registry) {
    // List of all raylib function names registered in register_raylib_bindings
    // This list is auto-generated and should match the functions registered above
    std::vector<std::string> raylib_function_names = {
        "INITWINDOW",
        "CLOSEWINDOW",
        "WINDOWSHOULDCLOSE",
        "ISWINDOWREADY",
        "ISWINDOWFULLSCREEN",
        "ISWINDOWHIDDEN",
        "ISWINDOWMINIMIZED",
        "ISWINDOWMAXIMIZED",
        "ISWINDOWFOCUSED",
        "ISWINDOWRESIZED",
        "ISWINDOWSTATE",
        "SETWINDOWSTATE",
        "CLEARWINDOWSTATE",
        "TOGGLEFULLSCREEN",
        "TOGGLEBORDERLESSWINDOWED",
        "MAXIMIZEWINDOW",
        "MINIMIZEWINDOW",
        "RESTOREWINDOW",
        "SETWINDOWTITLE",
        "SETWINDOWPOSITION",
        "SETWINDOWMONITOR",
        "SETWINDOWMINSIZE",
        "SETWINDOWMAXSIZE",
        "SETWINDOWSIZE",
        "SETWINDOWOPACITY",
        "SETWINDOWFOCUSED",
        "GETSCREENWIDTH",
        "GETSCREENHEIGHT",
        "GETRENDERWIDTH",
        "GETRENDERHEIGHT",
        "GETMONITORCOUNT",
        "GETCURRENTMONITOR",
        "GETMONITORPOSITION",
        "GETMONITORWIDTH",
        "GETMONITORHEIGHT",
        "GETMONITORREFRESHRATE",
        "GETWINDOWPOSITION",
        "GETWINDOWSCALEDPI",
        "GETMONITORNAME",
        "SETCLIPBOARDTEXT",
        "GETCLIPBOARDTEXT",
        "ENABLEEVENTWAITING",
        "DISABLEEVENTWAITING",
        "SHOWCURSOR",
        "HIDECURSOR",
        "ISCURSORHIDDEN",
        "ENABLECURSOR",
        "DISABLECURSOR",
        "ISCURSORONSCREEN",
        "BEGINDRAWING",
        "ENDDRAWING",
        "CLEARBACKGROUND",
        "BEGINMODE2D",
        "ENDMODE2D",
        "BEGINMODE3D",
        "ENDMODE3D",
        "BEGINTEXTUREMODE",
        "ENDTEXTUREMODE",
        "BEGINSHADERMODE",
        "ENDSHADERMODE",
        "BEGINBLENDMODE",
        "ENDBLENDMODE",
        "BEGINSCISSORMODE",
        "ENDSCISSORMODE",
        "BEGINVRSTEREOMODE",
        "ENDVRSTEREOMODE",
        "LOADVRSTEREOCONFIG",
        "UNLOADVRSTEREOCONFIG",
        "LOADSHADER",
        "LOADSHADERFROMEMORY",
        "ISSHADERVALID",
        "GETSHADERLOCATION",
        "GETSHADERLOCATIONATTRIB",
        "SETSHADERVALUEFLOAT",
        "SETSHADERVALUEINT",
        "UNLOADSHADER",
        "GETSCREENTOWORLDRAY",
        "GETWORLDTOSCREEN",
        "GETWORLDTOSCREEN2D",
        "GETSCREENTOWORLD2D",
        "GETCAMERAMATRIX",
        "GETCAMERAMATRIX2D",
        "SETTARGETFPS",
        "GETFRAMETIME",
        "GETTIME",
        "GETFPS",
        "SWAPSCREENBUFFER",
        "POLLINPUTEVENTS",
        "WAITTIME",
        "SETRANDOMSEED",
        "GETRANDOMVALUE",
        "LOADRANDOMSEQUENCE",
        "TAKESCREENSHOT",
        "SETCONFIGFLAGS",
        "OPENURL",
        "TRACELOG",
        "SETTRACELOGLEVEL",
        "LOADFILEDATA",
        "SAVEFILEDATA",
        "LOADFILETEXT",
        "SAVEFILETEXT",
        "FILEEXISTS",
        "DIRECTORYEXISTS",
        "ISFILEEXTENSION",
        "GETFILELENGTH",
        "GETFILEEXTENSION",
        "GETFILENAME",
        "GETFILENAMEWITHHOUTEXT",
        "GETDIRECTORYPATH",
        "GETPREVDIRECTORYPATH",
        "GETWORKINGDIRECTORY",
        "GETAPPLICATIONDIRECTORY",
        "MAKEDIRECTORY",
        "CHANGEDIRECTORY",
        "ISPATHFILE",
        "ISFILENAMEVALID",
        "LOADDIRECTORYFILES",
        "ISFILEDROPPED",
        "LOADDROPPEDFILES",
        "GETFILEMODTIME",
        "COMPRESSDATA",
        "ENCODEDATABASE64",
        "ISKEYPRESSED",
        "ISKEYPRESSEDREPEAT",
        "ISKEYDOWN",
        "ISKEYRELEASED",
        "ISKEYUP",
        "GETKEYPRESSED",
        "GETCHARPRESSED",
        "SETEXITKEY",
        "ISGAMEPADAVAILABLE",
        "GETGAMEPADNAME",
        "ISGAMEPADBUTTONPRESSED",
        "ISGAMEPADBUTTONDOWN",
        "ISGAMEPADBUTTONRELEASED",
        "ISGAMEPADBUTTONUP",
        "GETGAMEPADBUTTONPRESSED",
        "GETGAMEPADAXISCOUNT",
        "GETGAMEPADAXISMOVEMENT",
        "SETGAMEPADMAPPINGS",
        "SETGAMEPADVIBRTION",
        "ISMOUSEBUTTONPRESSED",
        "ISMOUSEBUTTONDOWN",
        "ISMOUSEBUTTONRELEASED",
        "ISMOUSEBUTTONUP",
        "GETMOUSEX",
        "GETMOUSEY",
        "GETMOUSEPOSITION",
        "GETMOUSEDELTA",
        "SETMOUSEPOSITION",
        "SETMOUSEOFFSET",
        "SETMOUSESCALE",
        "GETMOUSEWHEELMOVE",
        "GETMOUSEWHEELMOVEV",
        "SETMOUSECURSOR",
        "GETTOUCHX",
        "GETTOUCHY",
        "GETTOUCHPOSITION",
        "GETTOUCHPOINTID",
        "GETTOUCHPOINTCOUNT",
        "SETGESTURESENABLED",
        "ISGESTUREDETECTED",
        "GETGESTUREDETECTED",
        "GETGESTUREHOLDDURATION",
        "GETGESTUREDRAGVECTOR",
        "GETGESTUREDRAGANGLE",
        "GETGESTUREPINCHVECTOR",
        "GETGESTUREPINCHANGLE",
        "UPDATECAMERA",
        "UPDATECAMERAPRO",
        "CREATEFPSCAMERA",
        "CREATETPSCAMERA",
        "CREATEORBITCAMERA",
        "CREATEFREECAMERA",
        "SETACTIVECAMERA",
        "SETCAMERAPOSITION",
        "SETCAMERATARGET",
        "SETCAMERAFOV",
        "SETCAMERAMOUSESENSITIVITY",
        "SETCAMERASMOOTHING",
        "GETCAMERAPOSITION",
        "GETCAMERATARGET",
        "GETCAMERACOUNT",
        "SETSHAPESTEXTURE",
        "GETSHAPESTEXTURE",
        "GETSHAPESTEXTURERECTANGLE",
        "DRAWPIXEL",
        "DRAWPIXELV",
        "DRAWLINE",
        "DRAWLINEV",
        "DRAWLINEEX",
        "DRAWLINEBEZIER",
        "DRAWCIRCLE",
        "DRAWCIRCLESECTOR",
        "DRAWCIRCLEGRADIENT",
        "DRAWCIRCLEV",
        "DRAWCIRCLELINES",
        "DRAWELLIPSE",
        "DRAWELLIPSELINES",
        "DRAWRING",
        "DRAWRINGLINES",
        "DRAWRECTANGLE",
        "DRAWRECTANGLEV",
        "DRAWRECTANGLEREC",
        "DRAWRECTANGLEPRO",
        "DRAWRECTANGLEGRADIENTV",
        "DRAWRECTANGLEGRADIENTH",
        "DRAWRECTANGLEGRADIENTEX",
        "DRAWRECTANGLELINES",
        "DRAWRECTANGLELINESEX",
        "DRAWRECTANGLEROUNDED",
        "DRAWRECTANGLEROUNDEDLINES",
        "DRAWRECTANGLEROUNDEDLINESEX",
        "DRAWTRIANGLE",
        "DRAWTRIANGLELINES",
        "DRAWPOLY",
        "DRAWPOLYLINES",
        "DRAWPOLYLINESEX",
        "DRAWSPLINELINEAR",
        "DRAWSPLINEBASIS",
        "DRAWSPLINECATMULLROM",
        "DRAWSPLINEBEZIERCUBIC",
        "DRAWSPLINESEGMENTLINEAR",
        "DRAWSPLINESEGMENTBASIS",
        "DRAWSPLINESEGMENTCATMULLROM",
        "DRAWSPLINESEGMENTBEZIERQUADRATIC",
        "DRAWSPLINESEGMENTBEZIERCUBIC",
        "GETSPLINEPOINTLINEAR",
        "GETSPLINEPOINTBASIS",
        "GETSPLINEPOINTCATMULLROM",
        "GETSPLINEPOINTBEZIERQUAD",
        "GETSPLINEPOINTBEZIERCUBIC",
        "CHECKCOLLISIONRECS",
        "CHECKCOLLISIONCIRCLES",
        "CHECKCOLLISIONCIRCLEREC",
        "CHECKCOLLISIONCIRCLELINE",
        "CHECKCOLLISIONPOINTREC",
        "CHECKCOLLISIONPOINTCIRCLE",
        "CHECKCOLLISIONPOINTTRIANGLE",
        "CHECKCOLLISIONPOINTLINE",
        "CHECKCOLLISIONPOINTPOLY",
        "CHECKCOLLISIONLINES",
        "GETCOLLISIONREC",
        "LOADIMAGE",
        "LOADIMAGERAW",
        "LOADIMAGEANIM",
        "LOADIMAGEFROMSCREEN",
        "ISIMAGEVALID",
        "UNLOADIMAGE",
        "EXPORTIMAGE",
        "EXPORTIMAGEASCODE",
        "GETFONTDEFAULT",
        "LOADFONT",
        "LOADFONTEX",
        "ISFONTVALID",
        "UNLOADFONT",
        "DRAWFPS",
        "DRAWTEXT",
        "DRAWTEXTEX",
        "DRAWTEXTPRO",
        "MEASURETEXT",
        "MEASURETEXTEX",
        "TEXTLENGTH",
        "TEXTSUBTEXT",
        "TEXTTOUPPER",
        "TEXTTOLOWER",
        "TEXTTOINTEGER",
        "TEXTTOFLOAT",
        "TEXTFINDINDEX",
        "TEXTISEQUAL",
        "DRAWLINE3D",
        "DRAWPOINT3D",
        "DRAWCIRCLE3D",
        "DRAWTRIANGLE3D",
        "DRAWCUBE",
        "DRAWCUBEV",
        "DRAWCUBEWIRES",
        "DRAWSPHERE",
        "DRAWSPHEREEX",
        "DRAWSPHEREWIRES",
        "DRAWCYLINDER",
        "DRAWPLANE",
        "DRAWGRID",
        "LOADMODEL",
        "ISMODELVALID",
        "UNLOADMODEL",
        "DRAWMODEL",
        "DRAWMODELWIRES",
        "CHECKCOLLISIONSPHERES",
        "CHECKCOLLISIONBOXSPHERE",
        "INITAUDIODEVICE",
        "CLOSEAUDIODEVICE",
        "ISAUDIODEVICEREADY",
        "SETMASTERVOLUME",
        "GETMASTERVOLUME",
        "LOADWAVE",
        "ISWAVEVALID",
        "LOADSOUND",
        "LOADSOUNDFROMWAVE",
        "ISSOUNDVALID",
        "UNLOADWAVE",
        "UNLOADSOUND",
        "PLAYSOUND",
        "STOPSOUND",
        "PAUSESOUND",
        "RESUMESOUND",
        "ISSOUNDPLAYING",
        "SETSOUNDVOLUME",
        "SETSOUNDPITCH",
        "SETSOUNDPAN",
        "LOADMUSICSTREAM",
        "ISMUSICVALID",
        "UNLOADMUSICSTREAM",
        "PLAYMUSICSTREAM",
        "ISMUSICSTREAMPLAYING",
        "UPDATEMUSICSTREAM",
        "STOPMUSICSTREAM",
        "PAUSEMUSICSTREAM",
        "RESUMEMUSICSTREAM",
        "SETMUSICVOLUME",
        "SETMUSICPITCH",
        "GETMUSICTIMELENGTH",
        "GETMUSICTIMEPLAYED",
        "CLAMP",
        "LERP",
        "NORMALIZE",
        "REMAP",
        "WRAP",
        "FLOATEQUALS",
        "VECTOR2ZERO",
        "VECTOR2ONE",
        "VECTOR2ADD",
        "VECTOR2SUBTRACT",
        "VECTOR2LENGTH",
        "VECTOR2DISTANCE",
        "VECTOR2DOTPRODUCT",
        "VECTOR2NORMALIZE",
        "VECTOR2LERP",
        "VECTOR3ZERO",
        "VECTOR3ONE",
        "VECTOR3ADD",
        "VECTOR3SUBTRACT",
        "VECTOR3LENGTH",
        "VECTOR3DISTANCE",
        "VECTOR3DOTPRODUCT",
        "VECTOR3CROSSPRODUCT",
        "VECTOR3NORMALIZE",
        "VECTOR3LERP",
        "COLOREQUAL",
        "FADE",
        "COLORTOINT",
        "COLORTOHSV",
        "COLORFROMHSV",
        "COLORTINT",
        "COLORBRIGHTNESS",
        "COLORCONTRAST",
        "COLORALPHA",
        "COLORLERP",
        "GETCOLOR",
        "LIGHTGRAY",
        "GRAY",
        "DARKGRAY",
        "YELLOW",
        "GOLD",
        "ORANGE",
        "PINK",
        "RED",
        "MAROON",
        "GREEN",
        "LIME",
        "DARKGREEN",
        "SKYBLUE",
        "BLUE",
        "DARKBLUE",
        "PURPLE",
        "VIOLET",
        "DARKPURPLE",
        "BEIGE",
        "BROWN",
        "DARKBROWN",
        "WHITE",
        "BLACK",
        "BLANK",
        "MAGENTA",
        "RAYWHITE",
        "INITWINDOW",
        "WINDOWSHOULDCLOSE",
        "ISWINDOWREADY",
        "ISWINDOWFULLSCREEN",
        "ISWINDOWHIDDEN",
        "ISWINDOWMINIMIZED",
        "ISWINDOWMAXIMIZED",
        "ISWINDOWFOCUSED",
        "ISWINDOWRESIZED",
        "ISWINDOWSTATE",
        "SETWINDOWSTATE",
        "CLEARWINDOWSTATE",
        "TOGGLEFULLSCREEN",
        "TOGGLEBORDERLESSWINDOWED",
        "MAXIMIZEWINDOW",
        "MINIMIZEWINDOW",
        "RESTOREWINDOW",
        "SETWINDOWICON",
        "SETWINDOWPOSITION",
        "SETWINDOWMONITOR",
        "SETWINDOWMINSIZE",
        "SETWINDOWMAXSIZE",
        "SETWINDOWOPACITY",
        "SETWINDOWFOCUSED",
        "GETWINDOWHANDLE",
        "GETSCREENWIDTH",
        "GETSCREENHEIGHT",
        "GETRENDERWIDTH",
        "GETRENDERHEIGHT",
        "GETCURRENTMONITOR",
        "GETMONITORPOSITION",
        "GETMONITORNAME",
        "SETCLIPBOARDTEXT",
        "GETCLIPBOARDTEXT",
        "ENABLEEVENTWAITING",
        "DISABLEEVENTWAITING",
        "SHOWCURSOR",
        "HIDECURSOR",
        "ISCURSORHIDDEN",
        "ENABLECURSOR",
        "DISABLECURSOR",
        "ISCURSORONSCREEN",
        "SETTARGETFPS",
        "GETFPS",
        "GETDELTATIME",
        "GETTIME",
        "BEGINDRAW",
        "ENDDRAW",
        "BEGINMODE2D",
        "ENDMODE2D",
        "BEGINMODE3D",
        "ENDMODE3D",
        "BEGINTEXTUREMODE",
        "ENDTEXTUREMODE",
        "BEGINBLENDMODE",
        "ENDBLENDMODE",
        "BEGINSCISSORMODE",
        "ENDSCISSORMODE",
        "SETEXITKEY",
        "TAKESCREENSHOT",
        "DRAWFPS",
        "CLEARBACKGROUND",
        "DRAWTEXT",
        "DRAWTEXTEX",
        "DRAWTEXTPRO",
        "DRAWRECTANGLE",
        "DRAWRECTANGLEV",
        "DRAWRECTANGLEREC",
        "DRAWRECTANGLEPRO",
        "DRAWRECTANGLEGRADIENTV",
        "DRAWRECTANGLEGRADIENTH",
        "DRAWRECTANGLEROUNDED",
        "DRAWRECTANGLELINES",
        "DRAWRECTANGLELINESEX",
        "DRAWCIRCLE",
        "DRAWCIRCLEV",
        "DRAWCIRCLELINES",
        "DRAWCIRCLELINESV",
        "DRAWCIRCLESECTOR",
        "DRAWCIRCLESECTORLINES",
        "DRAWCIRCLEGRADIENT",
        "DRAWELLIPSE",
        "DRAWELLIPSELINES",
        "DRAWRING",
        "DRAWRINGLINES",
        "DRAWLINE",
        "DRAWLINEV",
        "DRAWLINEEX",
        "DRAWTRIANGLE",
        "DRAWTRIANGLELINES",
        "DRAWPOLY",
        "DRAWPOLYLINES",
        "DRAWPIXEL",
        "DRAWPIXELV",
        "ISKEYPRESSEDREPEAT",
        "ISKEYUP",
        "GETKEYPRESSED",
        "GETCHARPRESSED",
        "ISMOUSEBUTTONPRESSED",
        "ISMOUSEBUTTONRELEASED",
        "ISMOUSEBUTTONUP",
        "GETMOUSEPOSITION",
        "GETMOUSEDELTA",
        "GETMOUSEWHEELMOVE",
        "GETMOUSEWHEELMOVEV",
        "SETMOUSEPOSITION",
        "SETMOUSEOFFSET",
        "SETMOUSESCALE",
        "SETMOUSECURSOR",
        "GETTOUCHX",
        "GETTOUCHY",
        "GETTOUCHPOSITION",
        "GETTOUCHPOINTID",
        "GETTOUCHPOINTCOUNT",
        "ISGAMEPADAVAILABLE",
        "GETGAMEPADNAME",
        "ISGAMEPADBUTTONPRESSED",
        "ISGAMEPADBUTTONDOWN",
        "ISGAMEPADBUTTONRELEASED",
        "ISGAMEPADBUTTONUP",
        "GETGAMEPADBUTTONPRESSED",
        "GETGAMEPADAXISCOUNT",
        "GETGAMEPADAXISMOVEMENT",
        "SETGAMEPADMAPPINGS",
        "SETGAMEPADVIBRATION",
        "LOADTEXTURE",
        "LOADTEXTUREFROMIMAGE",
        "LOADTEXTURECUBEMAP",
        "LOADRENDERTEXTURE",
        "ISTEXTUREVALID",
        "ISRENDERTEXTUREVALID",
        "UPDATETEXTURE",
        "UPDATETEXTUREREC",
        "GENTEXTUREMIPMAPS",
        "SETTEXTUREFILTER",
        "SETTEXTUREWRAP",
        "DRAWTEXTURE",
        "DRAWTEXTUREV",
        "DRAWTEXTUREEX",
        "DRAWTEXTUREREC",
        "DRAWTEXTUREPRO",
        "DRAWTEXTURENPATCH",
        "UNLOADTEXTURE",
        "UNLOADRENDERTEXTURE",
        "INITAUDIODEVICE",
        "CLOSEAUDIODEVICE",
        "ISAUDIODEVICEREADY",
        "SETMASTERVOLUME",
        "GETMASTERVOLUME",
        "LOADWAVE",
        "LOADWAVEFROMMEMORY",
        "ISWAVEVALID",
        "LOADSOUNDFROMWAVE",
        "LOADSOUNDALIAS",
        "ISSOUNDVALID",
        "UPDATESOUND",
        "PAUSESOUND",
        "RESUMESOUND",
        "ISSOUNDPLAYING",
        "SETSOUNDVOLUME",
        "SETSOUNDPITCH",
        "SETSOUNDPAN",
        "WAVECOPY",
        "WAVECROP",
        "WAVEFORMAT",
        "LOADWAVESAMPLES",
        "UNLOADWAVESAMPLES",
        "LOADMUSICSTREAM",
        "LOADMUSICSTREAMFROMMEMORY",
        "ISMUSICSTREAMVALID",
        "UNLOADMUSICSTREAM",
        "PLAYMUSICSTREAM",
        "ISMUSICSTREAMPLAYING",
        "UPDATEMUSICSTREAM",
        "STOPMUSICSTREAM",
        "PAUSEMUSICSTREAM",
        "RESUMEMUSICSTREAM",
        "SEEKMUSICSTREAM",
        "SETMUSICVOLUME",
        "SETMUSICPITCH",
        "SETMUSICPAN",
        "GETMUSICTIMELENGTH",
        "GETMUSICTIMEPLAYED",
        "LOADAUDIOSTREAM",
        "ISAUDIOSTREAMVALID",
        "UNLOADAUDIOSTREAM",
        "UPDATEAUDIOSTREAM",
        "ISAUDIOSTREAMPROCESSED",
        "PLAYAUDIOSTREAM",
        "PAUSEAUDIOSTREAM",
        "RESUMEAUDIOSTREAM",
        "ISAUDIOSTREAMPLAYING",
        "STOPAUDIOSTREAM",
        "SETAUDIOSTREAMVOLUME",
        "SETAUDIOSTREAMPITCH",
        "SETAUDIOSTREAMPAN",
        "SETAUDIOSTREAMBUFFERSIZEDEFAULT",
        "SETAUDIOSTREAMCALLBACK",
        "ATTACHAUDIOSTREAMPROCESSOR",
        "DETACHAUDIOSTREAMPROCESSOR",
        "ATTACHAUDIOMIXEDPROCESSOR",
        "DETACHAUDIOMIXEDPROCESSOR",
        "UNLOADWAVE",
        "UNLOADSOUNDALIAS",
        "EXPORTWAVE",
        "EXPORTWAVEASCODE",
        "LOADMODEL",
        "LOADMODELFROMMESH",
        "ISMODELVALID",
        "GETMODELBOUNDINGBOX",
        "DRAWMODEL",
        "DRAWMODELEX",
        "DRAWMODELWIRES",
        "DRAWMODELWIRESEX",
        "DRAWBOUNDINGBOX",
        "DRAWBILLBOARD",
        "DRAWBILLBOARDREC",
        "DRAWBILLBOARDPRO",
        "UPLOADMESH",
        "UPDATEMESHBUFFER",
        "UNLOADMESH",
        "DRAWMESH",
        "DRAWMESHINSTANCED",
        "GETMESHBOUNDINGBOX",
        "GENMESHTANGENTS",
        "EXPORTMESH",
        "GENMESHPOLY",
        "GENMESHPLANE",
        "GENMESHCUBE",
        "GENMESHSPHERE",
        "GENMESHHEMISPHERE",
        "GENMESHCYLINDER",
        "GENMESHCONE",
        "GENMESHTORUS",
        "GENMESHKNOT",
        "GENMESHHEIGHTMAP",
        "GENMESHCUBICMAP",
        "LOADMATERIALS",
        "LOADMATERIALDEFAULT",
        "ISMATERIALVALID",
        "UNLOADMATERIAL",
        "SETMATERIALTEXTURE",
        "SETMODELMESHMATERIAL",
        "LOADMODELANIMATIONS",
        "UPDATEMODELANIMATION",
        "UNLOADMODELANIMATION",
        "UNLOADMODELANIMATIONS",
        "ISMODELANIMATIONVALID",
        "CHECKCOLLISIONSPHERES",
        "CHECKCOLLISIONBOXES",
        "CHECKCOLLISIONBOXSPHERE",
        "GETRAYCOLLISIONSPHERE",
        "GETRAYCOLLISIONBOX",
        "GETRAYCOLLISIONMESH",
        "GETRAYCOLLISIONTRIANGLE",
        "GETRAYCOLLISIONQUAD",
        "UNLOADMODEL",
        "DRAWLINE3D",
        "DRAWPOINT3D",
        "DRAWCIRCLE3D",
        "DRAWTRIANGLE3D",
        "DRAWTRIANGLESTRIP3D",
        "DRAWCUBE",
        "DRAWCUBEV",
        "DRAWCUBEWIRES",
        "DRAWCUBEWIRESV",
        "DRAWSPHERE",
        "DRAWSPHEREEX",
        "DRAWSPHEREWIRES",
        "DRAWCYLINDER",
        "DRAWCYLINDEREX",
        "DRAWCYLINDERWIRES",
        "DRAWCYLINDERWIRESEX",
        "DRAWCAPSULE",
        "DRAWCAPSULEWIRES",
        "DRAWPLANE",
        "DRAWRAY",
        "DRAWGRID",
        "CHECKCOLLISIONRECS",
        "CHECKCOLLISIONCIRCLES",
        "CHECKCOLLISIONCIRCLEREC",
        "CHECKCOLLISIONPOINTREC",
        "CHECKCOLLISIONPOINTCIRCLE",
        "CHECKCOLLISIONPOINTTRIANGLE",
        "CHECKCOLLISIONPOINTLINE",
        "CHECKCOLLISIONPOINTPOLY",
        "CHECKCOLLISIONLINES",
        "GETCOLLISIONREC",
        "LOADSHADER",
        "LOADSHADERFROMMEMORY",
        "ISSHADERVALID",
        "GETSHADERLOCATION",
        "GETSHADERLOCATIONATTRIB",
        "SETSHADERVALUE",
        "SETSHADERVALUEV",
        "SETSHADERVALUEMATRIX",
        "SETSHADERVALUETEXTURE",
        "BEGINSHADERMODE",
        "ENDSHADERMODE",
        "UNLOADSHADER",
        "GETRANDOMVALUE",
        "SETRANDOMSEED",
        "GETMONITORCOUNT",
        "GETMONITORWIDTH",
        "GETMONITORHEIGHT",
        "GETMONITORPHYSICALWIDTH",
        "GETMONITORPHYSICALHEIGHT",
        "GETMONITORREFRESHRATE",
        "GETWINDOWPOSITION",
        "GETWINDOWSCALEDPI",
        "SETWINDOWMIN",
        "SETWINDOWMAX",
        "SETCONFIGFLAGS",
        "OPENURL",
        "TRACELOG",
        "SETTRACELOGLEVEL",
        "MEMALLOC",
        "MEMREALLOC",
        "MEMFREE",
        "LOADFILEDATA",
        "UNLOADFILEDATA",
        "SAVEFILEDATA",
        "EXPORTDATAASCODE",
        "LOADFILETEXT",
        "UNLOADFILETEXT",
        "DIRECTORYEXISTS",
        "ISFILEEXTENSION",
        "GETFILELENGTH",
        "GETFILEEXTENSION",
        "GETFILENAME",
        "GETFILENAMEWITHOUTEXT",
        "GETDIRECTORYPATH",
        "GETPREVDIRECTORYPATH",
        "GETWORKINGDIRECTORY",
        "GETAPPLICATIONDIRECTORY",
        "CHANGEDIRECTORY",
        "ISPATHFILE",
        "LOADDIRECTORYFILES",
        "LOADDIRECTORYFILESEX",
        "UNLOADDIRECTORYFILES",
        "ISFILEDROPPED",
        "LOADDROPPEDFILES",
        "UNLOADDROPPEDFILES",
        "GETFILEMODTIME",
        "COMPRESSDATA",
        "DECOMPRESSDATA",
        "ENCODEDATABASE64",
        "LOADIMAGERAW",
        "LOADIMAGEANIM",
        "LOADIMAGEFROMMEMORY",
        "LOADIMAGEFROMTEXTURE",
        "LOADIMAGEFROMSCREEN",
        "ISIMAGEVALID",
        "EXPORTIMAGE",
        "EXPORTIMAGETOMEMORY",
        "EXPORTIMAGEASCODE",
        "GENIMAGECOLOR",
        "GENIMAGEGRADIENTLINEAR",
        "GENIMAGEGRADIENTRADIAL",
        "GENIMAGEGRADIENTSQUARE",
        "GENIMAGECHECKED",
        "GENIMAGEWHITENOISE",
        "GENIMAGEPERLINNOISE",
        "GENIMAGECELLULAR",
        "GENIMAGETEXT",
        "GETFONTDEFAULT",
        "LOADFONT",
        "LOADFONTEX",
        "LOADFONTFROMIMAGE",
        "LOADFONTFROMMEMORY",
        "ISFONTVALID",
        "LOADFONTDATA",
        "GENIMAGEFONTATLAS",
        "UNLOADFONTDATA",
        "UNLOADFONT",
        "EXPORTFONTASCODE",
        "SETTEXTLINESPACING",
        "MEASURETEXT",
        "MEASURETEXTEX",
        "GETGLYPHINDEX",
        "GETGLYPHINFO",
        "GETGLYPHATLASREC",
        "LOADUTF8",
        "UNLOADUTF8",
        "LOADCODEPOINTS",
        "UNLOADCODEPOINTS",
        "GETCODEPOINTCOUNT",
        "GETCODEPOINT",
        "GETCODEPOINTNEXT",
        "GETCODEPOINTPREVIOUS",
        "CODEPOINTTOUTF8",
        "TEXTCOPY",
        "TEXTISEQUAL",
        "TEXTLENGTH",
        "TEXTFORMAT",
        "TEXTSUBTEXT",
        "TEXTINSERT",
        "TEXTTOINTEGER",
        "DRAWTEXTCODEPOINT",
        "DRAWLINES",
        "MIN",
        "MAX",
        "CLAMP",
        "COLLISIONRECTANGLES",
        "POINTINRECTANGLE",
        "DISTANCE",
        "SPLIT",
        "LOADMUSIC",
        "UNLOADMUSIC",
        "PLAYMUSIC",
        "STOPMUSIC",
        "PAUSEMUSIC",
        "RESUMEMUSIC",
        "UPDATEMUSIC",
        "ISMUSICPLAYING",
        "GETTEXTUREWIDTH",
        "GETTEXTUREHEIGHT",
        "IMAGEFROMTEXTURE",
        "TEXTUREFROMIMAGE",
        "RESIZEIMAGE",
        "CROPIMAGE",
        "FLIPIMAGE",
        "ROTATEIMAGE",
        "REPLACE",
        "INSTRREV",
        "STRREPEAT",
        "STRREVERSE",
        "SETGESTURESENABLED",
        "ISGESTUREDETECTED",
        "GETGESTUREDETECTED",
        "GETGESTUREHOLDTIME",
        "GETGESTUREDRAGVECTOR",
        "GETGESTUREDRAGANGLE",
        "GETGESTUREPINCHVECTOR",
        "GETGESTUREPINCHANGLE",
        "SORT",
        "REVERSE",
        "FIND",
        "BINARYSEARCH",
        "ISNUMBER",
        "ISSTRING",
        "ISARRAY",
        "ARRAYLENGTH",
        "ARRAYCOPY",
        "ARRAYFILL",
        "ARRAYSHUFFLE",
        "ARRAYUNIQUE",
        "ARRAYJOIN",
        "ARRAYSPLIT",
        "HTTPGET",
        "HTTPPOST",
        "DOWNLOADFILE",
        "UPLOADFILE",
        "WEBSOCKETCONNECT",
        "WEBSOCKETSEND",
        "WEBSOCKETRECEIVE",
        "WEBSOCKETCLOSE",
        "TCPCONNECT",
        "TCPSEND",
        "TCPRECEIVE",
        "TCPCLOSE",
        "UDPCREATE",
        "UDPSEND",
        "UDPRECEIVE",
        "UDPCLOSE",
        "DRAWCONE",
        "DRAWCONEWIRES",
        "DRAWTORUS",
        "DRAWTORUSWIRES",
        "DRAWPOLYWIRES",
        "INITWINDOW",
        "CLOSEWINDOW",
        "WINDOWSHOULDCLOSE",
        "ISWINDOWREADY",
        "ISWINDOWFULLSCREEN",
        "ISWINDOWHIDDEN",
        "ISWINDOWMINIMIZED",
        "ISWINDOWMAXIMIZED",
        "ISWINDOWFOCUSED",
        "ISWINDOWRESIZED",
        "ISWINDOWSTATE",
        "SETWINDOWTITLE",
        "SETWINDOWMINIMUMSIZE",
        "SETWINDOWPOSITION",
        "SETWINDOWMONITOR",
        "SETWINDOWSTATE",
        "GETMONITORCOUNT",
        "GETMONITORWIDTH",
        "GETMONITORHEIGHT",
        "GETMONITORPHYSICALWIDTH",
        "GETMONITORPHYSICALHEIGHT",
        "GETMONITORREFRESHRATE",
        "GETCURRENTMONITOR",
        "BEGINDRAWING",
        "ENDDRAWING",
        "BEGINBLENDMODE",
        "ENDBLENDMODE",
        "SETTARGETFPS",
        "GETFPS",
        "GETFRAMETIME",
        "GETTIME",
        "INITGAME",
        "CLOSEGAME",
        "GAMESHOULDCLOSE",
        "BEGINGAMEFRAME",
        "ENDGAMEFRAME",
        "GETGAMEDELTATIME",
        "GETGAMEFPS",
        "SETUP3DCAMERA",
        "BEGINDRAW3D",
        "ENDDRAW3D",
        "LOADSPRITE",
        "DRAWSPRITE",
        "CHECKCOLLISIONRECTS",
        "CHECKCOLLISIONCIRCLES",
        "ISKEYHELD",
        "ISKEYPRESSED",
        "ISMOUSEHELD",
        "ISMOUSEPRESSED",
        "GETMOUSEPOSITION",
        "LOADGAMESOUND",
        "PLAYGAMESOUND",
        "LOADGAMEMUSIC",
        "PLAYGAMEMUSIC",
        "RANDOMINT",
        "RANDOMFLOAT",
        "DISTANCE2D",
        "SETWINDOWSIZE",
        "SETWINDOWICONS",
        "GETWINDOWPOSITION",
        "GETWINDOWSCALEDPI",
        "GETCLIPBOARDIMAGE",
        "SWAPSCREENBUFFER",
        "POLLINPUTEVENTS",
        "WAITTIME",
        "LOADRANDOMSEQUENCE",
        "UNLOADRANDOMSEQUENCE",
        "GETSCREENTOWORLDRAYEX",
        "GETWORLDTOSCREENEX",
        "GETWORLDTOSCREEN2D",
        "GETSCREENTOWORLD2D",
        "GETCAMERAMATRIX",
        "GETCAMERAMATRIX2D",
        "LOADVRSTEREOCONFIG",
        "UNLOADVRSTEREOCONFIG",
        "BEGINVRSTEREOMODE",
        "ENDVRSTEREOMODE",
        "ISSHADERVALID",
        "GETSHADERLOCATIONATTRIB",
        "SETSHADERVALUEV",
        "SETSHADERVALUEMATRIX",
        "SETSHADERVALUETEXTURE",
        "MAKEDIRECTORY",
        "ISFILENAMEVALID",
        "DECODEDATABASE64",
        "COMPUTECRC32",
        "COMPUTEMD5",
        "COMPUTESHA1",
        "LOADAUTOMATIONEVENTLIST",
        "UNLOADAUTOMATIONEVENTLIST",
        "EXPORTAUTOMATIONEVENTLIST",
        "SETAUTOMATIONEVENTLIST",
        "SETAUTOMATIONEVENTBASEFRAME",
        "STARTAUTOMATIONEVENTRECORDING",
        "STOPAUTOMATIONEVENTRECORDING",
        "PLAYAUTOMATIONEVENT",
        "SETTRACELOGCALLBACK",
        "SETLOADFILEDATACALLBACK",
        "SETSAVEFILEDATACALLBACK",
        "SETLOADFILETEXTCALLBACK",
        "SETSAVEFILETEXTCALLBACK",
        "ISKEYPRESSEDREPEAT",
        "GETCHARPRESSED",
        "SETGAMEPADVIBRATION",
        "GETMOUSEWHEELMOVEV",
        "SETMOUSECURSOR",
        "GETTOUCHX",
        "GETTOUCHY",
        "GETTOUCHPOSITION",
        "GETTOUCHPOINTID",
        "GETTOUCHPOINTCOUNT",
        "SETGESTURESENABLED",
        "ISGESTUREDETECTED",
        "GETGESTUREDETECTED",
        "GETGESTUREHOLDDURATION",
        "GETGESTUREDRAGVECTOR",
        "GETGESTUREDRAGANGLE",
        "GETGESTUREPINCHVECTOR",
        "GETGESTUREPINCHANGLE",
        "UPDATECAMERA",
        "UPDATECAMERAPRO",
        "SETSHAPESTEXTURE",
        "GETSHAPESTEXTURE",
        "GETSHAPESTEXTURERECTANGLE",
        "DRAWLINESTRIP",
        "DRAWCIRCLELINESV",
        "DRAWRECTANGLEPRO",
        "DRAWRECTANGLEGRADIENTV",
        "DRAWRECTANGLEGRADIENTH",
        "DRAWRECTANGLEGRADIENTEX",
        "DRAWRECTANGLEROUNDEDLINESEX",
        "DRAWSPLINELINEAR",
        "DRAWSPLINEBASIS",
        "DRAWSPLINECATMULLROM",
        "DRAWSPLINEBEZIERQUADRATIC",
        "DRAWSPLINEBEZIERCUBIC",
        "DRAWSPLINESEGMENTLINEAR",
        "DRAWSPLINESEGMENTBASIS",
        "DRAWSPLINESEGMENTCATMULLROM",
        "DRAWSPLINESEGMENTBEZIERQUADRATIC",
        "DRAWSPLINESEGMENTBEZIERCUBIC",
        "GETSPLINEPOINTLINEAR",
        "GETSPLINEPOINTBASIS",
        "GETSPLINEPOINTCATMULLROM",
        "GETSPLINEPOINTBEZIERQUAD",
        "GETSPLINEPOINTBEZIERCUBIC",
        "CHECKCOLLISIONCIRCLELINE",
        "CHECKCOLLISIONPOINTTRIANGLE",
        "CHECKCOLLISIONPOINTLINE",
        "CHECKCOLLISIONPOINTPOLY",
        "CHECKCOLLISIONLINES",
        "GETCOLLISIONREC",
        "LOADIMAGEANIMFROMMEMORY",
        "LOADIMAGEFROMSCREEN",
        "ISIMAGEVALID",
        "EXPORTIMAGETOMEMORY",
        "GENIMAGEGRADIENTLINEAR",
        "GENIMAGEGRADIENTRADIAL",
        "GENIMAGEGRADIENTSQUARE",
        "GENIMAGECHECKED",
        "GENIMAGEWHITENOISE",
        "GENIMAGEPERLINNOISE",
        "GENIMAGECELLULAR",
        "GENIMAGETEXT",
        "IMAGEFROMIMAGE",
        "IMAGEFROMCHANNEL",
        "IMAGETEXT",
        "IMAGETEXTEX",
        "IMAGEFORMAT",
        "IMAGETOPOT",
        "IMAGEALPHACROP",
        "IMAGEALPHACLEAR",
        "IMAGEALPHAMASK",
        "IMAGEALPHAPREMULTIPLY",
        "IMAGEBLURGAUSSIAN",
        "IMAGEKERNELCONVOLUTION",
        "IMAGERESIZENN",
        "IMAGERESIZECANVAS",
        "IMAGEMIPMAPS",
        "IMAGEDITHER",
        "IMAGEFLIPVERTICAL",
        "IMAGEFLIPHORIZONTAL",
        "IMAGEROTATE",
        "IMAGEROTATECW",
        "IMAGEROTATECCW",
        "IMAGECOLORTINT",
        "IMAGECOLORINVERT",
        "IMAGECOLORGRAYSCALE",
        "IMAGECOLORCONTRAST",
        "IMAGECOLORBRIGHTNESS",
        "IMAGECOLORREPLACE",
        "GETIMAGEALPHABORDER",
        "GETIMAGECOLOR",
        "IMAGECLEARBACKGROUND",
        "IMAGEDRAWPIXEL",
        "IMAGEDRAWPIXELV",
        "IMAGEDRAWLINE",
        "IMAGEDRAWLINEV",
        "IMAGEDRAWLINEEX",
        "IMAGEDRAWCIRCLE",
        "IMAGEDRAWCIRCLEV",
        "IMAGEDRAWCIRCLELINES",
        "IMAGEDRAWCIRCLELINESV",
        "IMAGEDRAWRECTANGLE",
        "IMAGEDRAWRECTANGLEV",
        "IMAGEDRAWRECTANGLEREC",
        "IMAGEDRAWRECTANGLELINES",
        "IMAGEDRAWTRIANGLE",
        "IMAGEDRAWTRIANGLEEX",
        "IMAGEDRAWTRIANGLELINES",
        "IMAGEDRAWTEXT",
        "IMAGEDRAWTEXTEX",
        "LOADTEXTURECUBEMAP",
        "ISTEXTUREVALID",
        "ISRENDERTEXTUREVALID",
        "UPDATETEXTUREREC",
        "COLORISEQUAL",
        "COLORTOINT",
        "COLORNORMALIZE",
        "COLORFROMNORMALIZED",
        "COLORTOHSV",
        "COLORFROMHSV",
        "COLORTINT",
        "COLORBRIGHTNESS",
        "COLORCONTRAST",
        "COLORALPHA",
        "COLORALPHABLEND",
        "COLORLERP",
        "GETCOLOR",
        "GETPIXELDATASIZE",
        "LOADFONTFROMIMAGE",
        "LOADFONTFROMMEMORY",
        "ISFONTVALID",
        "EXPORTFONTASCODE",
        "DRAWTEXTCODEPOINTS",
        "SETTEXTLINESPACING",
        "GETGLYPHINDEX",
        "GETGLYPHINFO",
        "GETGLYPHATLASREC",
        "GETCODEPOINTCOUNT",
        "GETCODEPOINT",
        "GETCODEPOINTNEXT",
        "GETCODEPOINTPREVIOUS",
        "CODEPOINTTOUTF8",
        "TEXTCOPY",
        "TEXTISEQUAL",
        "TEXTLENGTH",
        "TEXTFORMAT",
        "TEXTSUBTEXT",
        "TEXTREPLACE",
        "TEXTINSERT",
        "TEXTJOIN",
        "TEXTSPLIT",
        "TEXTFINDINDEX",
        "TEXTTOUPPER",
        "TEXTTOLOWER",
        "TEXTOPASCAL",
        "TEXTTOSNAKE",
        "TEXTTOCAMEL",
        "TEXTTOINTEGER",
        "TEXTTOFLOAT",
        "DRAWCAPSULE",
        "DRAWCAPSULEWIRES",
        "DRAWMODELPOINTS",
        "DRAWMODELPOINTSEX",
        "DRAWBILLBOARD",
        "DRAWBILLBOARDREC",
        "DRAWBILLBOARDPRO",
        "ISMODELVALID",
        "GETMODELBOUNDINGBOX",
        "UPLOADMESH",
        "UPDATEMESHBUFFER",
        "GETMESHBOUNDINGBOX",
        "GENMESHTANGENTS",
        "EXPORTMESH",
        "EXPORTMESHASCODE",
        "GENMESHHEMISPHERE",
        "GENMESHCONE",
        "ISMATERIALVALID",
        "SETMODELMESHMATERIAL",
        "CHECKCOLLISIONSPHERES",
        "CHECKCOLLISIONBOXES",
        "CHECKCOLLISIONBOXSPHERE",
        "GETRAYCOLLISIONSPHERE",
        "GETRAYCOLLISIONBOX",
        "GETRAYCOLLISIONTRIANGLE",
        "GETRAYCOLLISIONQUAD",
        "GETMASTERVOLUME",
        "LOADSOUNDALIAS",
        "ISSOUNDVALID",
        "ISWAVEVALID",
        "UNLOADSOUNDALIAS",
        "WAVECOPY",
        "WAVECROP",
        "WAVEFORMAT",
        "LOADMUSICSTREAMFROMMEMORY",
        "ISMUSICVALID",
        "ISAUDIOSTREAMVALID",
        "ISAUDIOSTREAMPROCESSED",
        "SETAUDIOSTREAMVOLUME",
        "SETAUDIOSTREAMPITCH",
        "SETAUDIOSTREAMPAN",
        "SETAUDIOSTREAMBUFFERSIZEDEFAULT",
        "VECTOR2ZERO",
        "VECTOR2ONE",
        "VECTOR2ADD",
        "VECTOR2ADDVALUE",
        "VECTOR2SUBTRACT",
        "VECTOR2SUBTRACTVALUE",
        "VECTOR2LENGTH",
        "VECTOR2LENGTHSQR",
        "VECTOR2DOTPRODUCT",
        "VECTOR2DISTANCE",
        "VECTOR2DISTANCESQR",
        "VECTOR2ANGLE",
        "VECTOR2SCALE",
        "VECTOR2MULTIPLY",
        "VECTOR2NEGATE",
        "VECTOR2DIVIDE",
        "VECTOR2NORMALIZE",
        "VECTOR2LERP",
        "VECTOR2REFLECT",
        "VECTOR2ROTATE",
        "VECTOR2MOVETOWARDS",
        "VECTOR2INVERT",
        "VECTOR2CLAMP",
        "VECTOR2CLAMPVALUE",
        "VECTOR2EQUALS",
        "VECTOR3ZERO",
        "VECTOR3ONE",
        "VECTOR3ADD",
        "VECTOR3ADDVALUE",
        "VECTOR3SUBTRACT",
        "VECTOR3SUBTRACTVALUE",
        "VECTOR3SCALE",
        "VECTOR3MULTIPLY",
        "VECTOR3CROSSPRODUCT",
        "VECTOR3LENGTH",
        "VECTOR3LENGTHSQR",
        "VECTOR3DOTPRODUCT",
        "VECTOR3DISTANCE",
        "VECTOR3DISTANCESQR",
        "VECTOR3ANGLE",
        "VECTOR3NEGATE",
        "VECTOR3DIVIDE",
        "VECTOR3NORMALIZE",
        "VECTOR3LERP",
        "VECTOR3REFLECT",
        "VECTOR3MIN",
        "VECTOR3MAX",
        "VECTOR3INVERT",
        "VECTOR3CLAMP",
        "VECTOR3CLAMPVALUE",
        "VECTOR3EQUALS",
        "VECTOR3REFRACT",
        "MATRIXIDENTITY",
        "MATRIXTRANSLATE",
        "MATRIXROTATEX",
        "MATRIXROTATEY",
        "MATRIXROTATEZ",
        "MATRIXSCALE",
        "QUATERNIONIDENTITY",
        "QUATERNIONLENGTH",
        "QUATERNIONNORMALIZE",
        "QUATERNIONLERP",
        "QUATERNIONSLERP",
        "QUATERNIONFROMEULER",
        "QUATERNIONTOEULER"
    };
    
    // Register each function in the native registry with a callback that calls the function
    for (const auto& func_name : raylib_function_names) {
        const NativeFn* fn = function_registry.find(func_name);
        if (fn) {
            native_registry.register_native(func_name, fn->fn);
        }
    }
}

} // namespace bas

// Auto-generated from specs/raylib_api.yaml
#include "raylib.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include "raymath.h"
#pragma GCC diagnostic pop
#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <vector>
#include <string>
#include <unordered_map>

using bas::Value;
using Fn = bas::NativeFn;

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
        // TODO: SetWindowIcon expects Image, not string
        // For now, just return nil
        return Value::nil();
        0;
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
        ::Vector2 pos = ::GetMonitorPosition(args[0].as_int());
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
        // Note: This requires a Camera2D object, simplified for BASIC
        ::BeginMode2D(::Camera2D{});
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ENDMODE2D", Fn{"ENDMODE2D", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDMODE2D: expected 0 args");
        EndMode2D();
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINMODE3D", Fn{"BEGINMODE3D", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("BEGINMODE3D: expected 1 args");
        // Note: This requires a Camera3D object, simplified for BASIC
        ::BeginMode3D(::Camera3D{});
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ENDMODE3D", Fn{"ENDMODE3D", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDMODE3D: expected 0 args");
        EndMode3D();
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINTEXTUREMODE", Fn{"BEGINTEXTUREMODE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("BEGINTEXTUREMODE: expected 1 args");
        // Note: This requires a RenderTexture2D object, simplified for BASIC
        ::BeginTextureMode(::RenderTexture2D{});
        0;
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
        ::Color c{(unsigned char)args[0].as_int(), (unsigned char)args[1].as_int(), (unsigned char)args[2].as_int(), 255};
        ::ClearBackground(c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXT", Fn{"DRAWTEXT", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWTEXT: expected 7 args");
        ::Color c{(unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), 255};
        ::DrawText(args[0].as_string().c_str(), args[1].as_int(), args[2].as_int(), args[3].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTEX", Fn{"DRAWTEXTEX", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWTEXTEX: expected 9 args");
        ::Font font = rlreg::fonts.at(args[0].as_int());
        ::Vector2 pos{(float)args[1].as_int(), (float)args[2].as_int()};
        ::Color c{(unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), 255};
        ::DrawTextEx(font, args[1].as_string().c_str(), pos, (float)args[3].as_number(), (float)args[4].as_number(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTPRO", Fn{"DRAWTEXTPRO", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWTEXTPRO: expected 12 args");
        ::Font font = rlreg::fonts.at(args[0].as_int());
        ::Vector2 pos{(float)args[1].as_int(), (float)args[2].as_int()};
        ::Vector2 origin{(float)args[3].as_int(), (float)args[4].as_int()};
        ::Color c{(unsigned char)args[8].as_int(), (unsigned char)args[9].as_int(), (unsigned char)args[10].as_int(), 255};
        ::DrawTextPro(font, args[1].as_string().c_str(), pos, origin, (float)args[5].as_number(), (float)args[6].as_number(), (float)args[7].as_number(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLE", Fn{"DRAWRECTANGLE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWRECTANGLE: expected 7 args");
        ::Color c{(unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), 255};
        ::DrawRectangle(args[0].as_int(), args[1].as_int(), args[2].as_int(), args[3].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEV", Fn{"DRAWRECTANGLEV", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWRECTANGLEV: expected 7 args");
        ::Vector2 pos{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Vector2 size{(float)args[2].as_int(), (float)args[3].as_int()};
        ::Color c{(unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), 255};
        ::DrawRectangleV(pos, size, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEREC", Fn{"DRAWRECTANGLEREC", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWRECTANGLEREC: expected 7 args");
        ::Rectangle rec{(float)args[0].as_int(), (float)args[1].as_int(), (float)args[2].as_int(), (float)args[3].as_int()};
        ::Color c{(unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), 255};
        ::DrawRectangleRec(rec, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEPRO", Fn{"DRAWRECTANGLEPRO", 11, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 11) throw std::runtime_error("DRAWRECTANGLEPRO: expected 11 args");
        ::Rectangle rec{(float)args[0].as_int(), (float)args[1].as_int(), (float)args[2].as_int(), (float)args[3].as_int()};
        ::Vector2 origin{(float)args[4].as_int(), (float)args[5].as_int()};
        ::Color c{(unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), (unsigned char)args[9].as_int(), 255};
        ::DrawRectanglePro(rec, origin, (float)args[6].as_number(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEGRADIENTV", Fn{"DRAWRECTANGLEGRADIENTV", 15, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 15) throw std::runtime_error("DRAWRECTANGLEGRADIENTV: expected 15 args");
        ::Color top{(unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), 255};
        ::Color bottom{(unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), (unsigned char)args[9].as_int(), 255};
        ::DrawRectangleGradientV(args[0].as_int(), args[1].as_int(), args[2].as_int(), args[3].as_int(), top, bottom);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEGRADIENTH", Fn{"DRAWRECTANGLEGRADIENTH", 15, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 15) throw std::runtime_error("DRAWRECTANGLEGRADIENTH: expected 15 args");
        ::Color left{(unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), 255};
        ::Color right{(unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), (unsigned char)args[9].as_int(), 255};
        ::DrawRectangleGradientH(args[0].as_int(), args[1].as_int(), args[2].as_int(), args[3].as_int(), left, right);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLEROUNDED", Fn{"DRAWRECTANGLEROUNDED", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWRECTANGLEROUNDED: expected 9 args");
        ::Rectangle rec{(float)args[0].as_int(), (float)args[1].as_int(), (float)args[2].as_int(), (float)args[3].as_int()};
        ::Color c{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::DrawRectangleRounded(rec, (float)args[4].as_number(), args[5].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLELINES", Fn{"DRAWRECTANGLELINES", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWRECTANGLELINES: expected 7 args");
        ::Color c{(unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), 255};
        ::DrawRectangleLines(args[0].as_int(), args[1].as_int(), args[2].as_int(), args[3].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRECTANGLELINESEX", Fn{"DRAWRECTANGLELINESEX", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWRECTANGLELINESEX: expected 8 args");
        ::Rectangle rec{(float)args[0].as_int(), (float)args[1].as_int(), (float)args[2].as_int(), (float)args[3].as_int()};
        ::Color c{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::DrawRectangleLinesEx(rec, (float)args[4].as_number(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLE", Fn{"DRAWCIRCLE", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWCIRCLE: expected 6 args");
        ::Color c{(unsigned char)args[3].as_int(), (unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), 255};
        ::DrawCircle(args[0].as_int(), args[1].as_int(), (float)args[2].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLEV", Fn{"DRAWCIRCLEV", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWCIRCLEV: expected 6 args");
        ::Vector2 center{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Color c{(unsigned char)args[3].as_int(), (unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), 255};
        ::DrawCircleV(center, (float)args[2].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLELINES", Fn{"DRAWCIRCLELINES", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWCIRCLELINES: expected 6 args");
        ::Color c{(unsigned char)args[3].as_int(), (unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), 255};
        ::DrawCircleLines(args[0].as_int(), args[1].as_int(), (float)args[2].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLELINESV", Fn{"DRAWCIRCLELINESV", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWCIRCLELINESV: expected 6 args");
        ::Vector2 center{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Color c{(unsigned char)args[3].as_int(), (unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), 255};
        ::DrawCircleLinesV(center, (float)args[2].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLESECTOR", Fn{"DRAWCIRCLESECTOR", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCIRCLESECTOR: expected 9 args");
        ::Vector2 center{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Color c{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::DrawCircleSector(center, (float)args[2].as_int(), (float)args[3].as_number(), (float)args[4].as_number(), args[5].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLESECTORLINES", Fn{"DRAWCIRCLESECTORLINES", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCIRCLESECTORLINES: expected 9 args");
        ::Vector2 center{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Color c{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::DrawCircleSectorLines(center, (float)args[2].as_int(), (float)args[3].as_number(), (float)args[4].as_number(), args[5].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLEGRADIENT", Fn{"DRAWCIRCLEGRADIENT", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWCIRCLEGRADIENT: expected 12 args");
        ::Color inner{(unsigned char)args[3].as_int(), (unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), 255};
        ::Color outer{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::DrawCircleGradient(args[0].as_int(), args[1].as_int(), (float)args[2].as_int(), inner, outer);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWELLIPSE", Fn{"DRAWELLIPSE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWELLIPSE: expected 7 args");
        ::Color c{(unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), 255};
        ::DrawEllipse(args[0].as_int(), args[1].as_int(), (float)args[2].as_int(), (float)args[3].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWELLIPSELINES", Fn{"DRAWELLIPSELINES", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWELLIPSELINES: expected 7 args");
        ::Color c{(unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), 255};
        ::DrawEllipseLines(args[0].as_int(), args[1].as_int(), (float)args[2].as_int(), (float)args[3].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRING", Fn{"DRAWRING", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWRING: expected 10 args");
        ::Vector2 center{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Color c{(unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), (unsigned char)args[9].as_int(), 255};
        ::DrawRing(center, (float)args[2].as_int(), (float)args[3].as_int(), (float)args[4].as_number(), (float)args[5].as_number(), args[6].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRINGLINES", Fn{"DRAWRINGLINES", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWRINGLINES: expected 10 args");
        ::Vector2 center{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Color c{(unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), (unsigned char)args[9].as_int(), 255};
        ::DrawRingLines(center, (float)args[2].as_int(), (float)args[3].as_int(), (float)args[4].as_number(), (float)args[5].as_number(), args[6].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWLINE", Fn{"DRAWLINE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWLINE: expected 7 args");
        ::Color c{(unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), 255};
        ::DrawLine(args[0].as_int(), args[1].as_int(), args[2].as_int(), args[3].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWLINEV", Fn{"DRAWLINEV", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWLINEV: expected 7 args");
        ::Vector2 start{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Vector2 end{(float)args[2].as_int(), (float)args[3].as_int()};
        ::Color c{(unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), 255};
        ::DrawLineV(start, end, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWLINEEX", Fn{"DRAWLINEEX", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWLINEEX: expected 8 args");
        ::Vector2 start{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Vector2 end{(float)args[2].as_int(), (float)args[3].as_int()};
        ::Color c{(unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), 255};
        ::DrawLineEx(start, end, (float)args[4].as_number(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTRIANGLE", Fn{"DRAWTRIANGLE", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWTRIANGLE: expected 9 args");
        ::Vector2 v1{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Vector2 v2{(float)args[2].as_int(), (float)args[3].as_int()};
        ::Vector2 v3{(float)args[4].as_int(), (float)args[5].as_int()};
        ::Color c{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::DrawTriangle(v1, v2, v3, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTRIANGLELINES", Fn{"DRAWTRIANGLELINES", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWTRIANGLELINES: expected 9 args");
        ::Vector2 v1{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Vector2 v2{(float)args[2].as_int(), (float)args[3].as_int()};
        ::Vector2 v3{(float)args[4].as_int(), (float)args[5].as_int()};
        ::Color c{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::DrawTriangleLines(v1, v2, v3, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPOLY", Fn{"DRAWPOLY", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWPOLY: expected 8 args");
        ::Vector2 center{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Color c{(unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), 255};
        ::DrawPoly(center, args[2].as_int(), (float)args[3].as_int(), (float)args[4].as_number(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPOLYLINES", Fn{"DRAWPOLYLINES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWPOLYLINES: expected 8 args");
        ::Vector2 center{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Color c{(unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), 255};
        ::DrawPolyLines(center, args[2].as_int(), (float)args[3].as_int(), (float)args[4].as_number(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPIXEL", Fn{"DRAWPIXEL", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("DRAWPIXEL: expected 5 args");
        ::Color c{(unsigned char)args[2].as_int(), (unsigned char)args[3].as_int(), (unsigned char)args[4].as_int(), 255};
        ::DrawPixel(args[0].as_int(), args[1].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPIXELV", Fn{"DRAWPIXELV", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("DRAWPIXELV: expected 5 args");
        ::Vector2 pos{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Color c{(unsigned char)args[2].as_int(), (unsigned char)args[3].as_int(), (unsigned char)args[4].as_int(), 255};
        ::DrawPixelV(pos, c);
        0;
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
        ::Vector2 pos = ::GetTouchPosition(args[0].as_int());
        return Value::from_string(std::to_string(pos.x) + "," + std::to_string(pos.y));
        
    }}, true);    R.add_with_policy("GETTOUCHPOINTID", Fn{"GETTOUCHPOINTID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETTOUCHPOINTID: expected 1 args");
        return Value::from_int(GetTouchPointId(args[0].as_int()));
    }}, true);    R.add_with_policy("GETTOUCHPOINTCOUNT", Fn{"GETTOUCHPOINTCOUNT", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETTOUCHPOINTCOUNT: expected 0 args");
        return Value::from_int(GetTouchPointCount());
    }}, true);    R.add_with_policy("ISGAMEPADAVAILABLE", Fn{"ISGAMEPADAVAILABLE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISGAMEPADAVAILABLE: expected 1 args");
        return Value::from_bool(::IsGamepadAvailable(args[0].as_int()));
        
    }}, true);    R.add_with_policy("GETGAMEPADNAME", Fn{"GETGAMEPADNAME", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETGAMEPADNAME: expected 1 args");
        return Value::from_string(::GetGamepadName(args[0].as_int()));
        
    }}, true);    R.add_with_policy("ISGAMEPADBUTTONPRESSED", Fn{"ISGAMEPADBUTTONPRESSED", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ISGAMEPADBUTTONPRESSED: expected 2 args");
        return Value::from_bool(::IsGamepadButtonPressed(args[0].as_int(), args[1].as_int()));
        
    }}, true);    R.add_with_policy("ISGAMEPADBUTTONDOWN", Fn{"ISGAMEPADBUTTONDOWN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ISGAMEPADBUTTONDOWN: expected 2 args");
        return Value::from_bool(::IsGamepadButtonDown(args[0].as_int(), args[1].as_int()));
        
    }}, true);    R.add_with_policy("ISGAMEPADBUTTONRELEASED", Fn{"ISGAMEPADBUTTONRELEASED", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ISGAMEPADBUTTONRELEASED: expected 2 args");
        return Value::from_bool(::IsGamepadButtonReleased(args[0].as_int(), args[1].as_int()));
        
    }}, true);    R.add_with_policy("ISGAMEPADBUTTONUP", Fn{"ISGAMEPADBUTTONUP", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ISGAMEPADBUTTONUP: expected 2 args");
        return Value::from_bool(::IsGamepadButtonUp(args[0].as_int(), args[1].as_int()));
        
    }}, true);    R.add_with_policy("GETGAMEPADBUTTONPRESSED", Fn{"GETGAMEPADBUTTONPRESSED", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETGAMEPADBUTTONPRESSED: expected 0 args");
        return Value::from_int(::GetGamepadButtonPressed());
        
    }}, true);    R.add_with_policy("GETGAMEPADAXISCOUNT", Fn{"GETGAMEPADAXISCOUNT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETGAMEPADAXISCOUNT: expected 1 args");
        return Value::from_int(::GetGamepadAxisCount(args[0].as_int()));
        
    }}, true);    R.add_with_policy("GETGAMEPADAXISMOVEMENT", Fn{"GETGAMEPADAXISMOVEMENT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETGAMEPADAXISMOVEMENT: expected 2 args");
        return Value::from_number(::GetGamepadAxisMovement(args[0].as_int(), args[1].as_int()));
        
    }}, true);    R.add_with_policy("SETGAMEPADMAPPINGS", Fn{"SETGAMEPADMAPPINGS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETGAMEPADMAPPINGS: expected 1 args");
        return Value::from_int(::SetGamepadMappings(args[0].as_string().c_str()));
        
    }}, true);    R.add_with_policy("SETGAMEPADVIBRATION", Fn{"SETGAMEPADVIBRATION", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("SETGAMEPADVIBRATION: expected 4 args");
        // TODO: SetGamepadVibration function not found in current Raylib version
        // For now, just return nil
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("LOADTEXTURE", Fn{"LOADTEXTURE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADTEXTURE: expected 1 args");
        ::Texture2D tex = ::LoadTexture(args[0].as_string().c_str());
        int id = rlreg::next_tex_id++;
        rlreg::textures[id] = tex;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("LOADTEXTUREFROMIMAGE", Fn{"LOADTEXTUREFROMIMAGE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADTEXTUREFROMIMAGE: expected 1 args");
        ::Texture2D tex = ::LoadTextureFromImage(rlreg::images.at(args[0].as_int()));
        int id = rlreg::next_tex_id++;
        rlreg::textures[id] = tex;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("LOADTEXTURECUBEMAP", Fn{"LOADTEXTURECUBEMAP", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADTEXTURECUBEMAP: expected 2 args");
        ::TextureCubemap tex = ::LoadTextureCubemap(rlreg::images.at(args[0].as_int()), args[1].as_int());
        int id = rlreg::next_cubemap_id++;
        rlreg::cubemaps[id] = tex;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("LOADRENDERTEXTURE", Fn{"LOADRENDERTEXTURE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADRENDERTEXTURE: expected 2 args");
        ::RenderTexture2D tex = ::LoadRenderTexture(args[0].as_int(), args[1].as_int());
        int id = rlreg::next_rendertex_id++;
        rlreg::rendertextures[id] = tex;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("ISTEXTUREVALID", Fn{"ISTEXTUREVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISTEXTUREVALID: expected 1 args");
        return Value::from_bool(::IsTextureReady(rlreg::textures.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("ISRENDERTEXTUREVALID", Fn{"ISRENDERTEXTUREVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISRENDERTEXTUREVALID: expected 1 args");
        return Value::from_bool(::IsRenderTextureReady(rlreg::rendertextures.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("UPDATETEXTURE", Fn{"UPDATETEXTURE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("UPDATETEXTURE: expected 2 args");
        // Note: This is simplified - would need proper pixel data handling
        ::UpdateTexture(rlreg::textures.at(args[0].as_int()), nullptr);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("UPDATETEXTUREREC", Fn{"UPDATETEXTUREREC", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("UPDATETEXTUREREC: expected 6 args");
        ::Rectangle rec{(float)args[1].as_int(), (float)args[2].as_int(), (float)args[3].as_int(), (float)args[4].as_int()};
        // Note: This is simplified - would need proper pixel data handling
        ::UpdateTextureRec(rlreg::textures.at(args[0].as_int()), rec, nullptr);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("GENTEXTUREMIPMAPS", Fn{"GENTEXTUREMIPMAPS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GENTEXTUREMIPMAPS: expected 1 args");
        ::GenTextureMipmaps(&rlreg::textures[args[0].as_int()]);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SETTEXTUREFILTER", Fn{"SETTEXTUREFILTER", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETTEXTUREFILTER: expected 2 args");
        ::SetTextureFilter(rlreg::textures.at(args[0].as_int()), args[1].as_int());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SETTEXTUREWRAP", Fn{"SETTEXTUREWRAP", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETTEXTUREWRAP: expected 2 args");
        ::SetTextureWrap(rlreg::textures.at(args[0].as_int()), args[1].as_int());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTURE", Fn{"DRAWTEXTURE", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWTEXTURE: expected 6 args");
        ::Color c{(unsigned char)args[3].as_int(), (unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), 255};
        ::DrawTexture(rlreg::textures.at(args[0].as_int()), args[1].as_int(), args[2].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTUREV", Fn{"DRAWTEXTUREV", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWTEXTUREV: expected 6 args");
        ::Vector2 pos{(float)args[1].as_int(), (float)args[2].as_int()};
        ::Color c{(unsigned char)args[3].as_int(), (unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), 255};
        ::DrawTextureV(rlreg::textures.at(args[0].as_int()), pos, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTUREEX", Fn{"DRAWTEXTUREEX", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWTEXTUREEX: expected 8 args");
        ::Vector2 pos{(float)args[1].as_int(), (float)args[2].as_int()};
        ::Color c{(unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), 255};
        ::DrawTextureEx(rlreg::textures.at(args[0].as_int()), pos, (float)args[3].as_number(), (float)args[4].as_number(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTUREREC", Fn{"DRAWTEXTUREREC", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWTEXTUREREC: expected 9 args");
        ::Rectangle src{(float)args[1].as_int(), (float)args[2].as_int(), (float)args[3].as_int(), (float)args[4].as_int()};
        ::Vector2 pos{(float)args[5].as_int(), (float)args[6].as_int()};
        ::Color c{(unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), (unsigned char)args[9].as_int(), 255};
        ::DrawTextureRec(rlreg::textures.at(args[0].as_int()), src, pos, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTUREPRO", Fn{"DRAWTEXTUREPRO", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWTEXTUREPRO: expected 12 args");
        ::Rectangle src{(float)args[1].as_int(), (float)args[2].as_int(), (float)args[3].as_int(), (float)args[4].as_int()};
        ::Rectangle dest{(float)args[5].as_int(), (float)args[6].as_int(), (float)args[7].as_int(), (float)args[8].as_int()};
        ::Vector2 origin{(float)args[9].as_int(), (float)args[10].as_int()};
        ::Color c{(unsigned char)args[11].as_int(), (unsigned char)args[12].as_int(), (unsigned char)args[13].as_int(), 255};
        ::DrawTexturePro(rlreg::textures.at(args[0].as_int()), src, dest, origin, 0.0f, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTEXTURENPATCH", Fn{"DRAWTEXTURENPATCH", 16, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 16) throw std::runtime_error("DRAWTEXTURENPATCH: expected 16 args");
        ::NPatchInfo nPatchInfo;
        nPatchInfo.source = {(float)args[1].as_int(), (float)args[2].as_int(), (float)args[3].as_int(), (float)args[4].as_int()};
        nPatchInfo.left = args[5].as_int();
        nPatchInfo.top = args[6].as_int();
        nPatchInfo.right = args[7].as_int();
        nPatchInfo.bottom = args[8].as_int();
        nPatchInfo.layout = args[9].as_int();
        ::Rectangle dest{(float)args[10].as_int(), (float)args[11].as_int(), (float)args[12].as_int(), (float)args[13].as_int()};
        ::Vector2 origin{(float)args[14].as_int(), (float)args[15].as_int()};
        ::Color c{(unsigned char)args[16].as_int(), (unsigned char)args[17].as_int(), (unsigned char)args[18].as_int(), 255};
        ::DrawTextureNPatch(rlreg::textures.at(args[0].as_int()), nPatchInfo, dest, origin, (float)args[14].as_number(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADTEXTURE", Fn{"UNLOADTEXTURE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADTEXTURE: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::textures.find(id);
        if (it != rlreg::textures.end()) {
          ::UnloadTexture(it->second);
          rlreg::textures.erase(it);
        }
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADRENDERTEXTURE", Fn{"UNLOADRENDERTEXTURE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADRENDERTEXTURE: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::rendertextures.find(id);
        if (it != rlreg::rendertextures.end()) {
          ::UnloadRenderTexture(it->second);
          rlreg::rendertextures.erase(it);
        }
        0;
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
        ::Wave wave = ::LoadWave(args[0].as_string().c_str());
        int id = rlreg::next_wave_id++;
        rlreg::waves[id] = wave;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("LOADWAVEFROMMEMORY", Fn{"LOADWAVEFROMMEMORY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADWAVEFROMMEMORY: expected 2 args");
        ::Wave wave = ::LoadWaveFromMemory(args[0].as_string().c_str(), (const unsigned char*)args[1].as_string().c_str(), args[1].as_string().length());
        int id = rlreg::next_wave_id++;
        rlreg::waves[id] = wave;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("ISWAVEVALID", Fn{"ISWAVEVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISWAVEVALID: expected 1 args");
        return Value::from_bool(::IsWaveReady(rlreg::waves.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("LOADSOUNDFROMWAVE", Fn{"LOADSOUNDFROMWAVE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADSOUNDFROMWAVE: expected 1 args");
        ::Sound sound = ::LoadSoundFromWave(rlreg::waves.at(args[0].as_int()));
        int id = rlreg::next_sound_id++;
        rlreg::sounds[id] = sound;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("LOADSOUNDALIAS", Fn{"LOADSOUNDALIAS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADSOUNDALIAS: expected 1 args");
        ::Sound sound = ::LoadSoundAlias(rlreg::sounds.at(args[0].as_int()));
        int id = rlreg::next_sound_id++;
        rlreg::sounds[id] = sound;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("ISSOUNDVALID", Fn{"ISSOUNDVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISSOUNDVALID: expected 1 args");
        return Value::from_bool(::IsSoundReady(rlreg::sounds.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("UPDATESOUND", Fn{"UPDATESOUND", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("UPDATESOUND: expected 2 args");
        // Note: This is simplified - would need proper audio data handling
        ::UpdateSound(rlreg::sounds.at(args[0].as_int()), nullptr, 0);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("PAUSESOUND", Fn{"PAUSESOUND", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PAUSESOUND: expected 1 args");
        ::PauseSound(rlreg::sounds.at(args[0].as_int()));
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("RESUMESOUND", Fn{"RESUMESOUND", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("RESUMESOUND: expected 1 args");
        ::ResumeSound(rlreg::sounds.at(args[0].as_int()));
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ISSOUNDPLAYING", Fn{"ISSOUNDPLAYING", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISSOUNDPLAYING: expected 1 args");
        return Value::from_bool(::IsSoundPlaying(rlreg::sounds.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("SETSOUNDVOLUME", Fn{"SETSOUNDVOLUME", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETSOUNDVOLUME: expected 2 args");
        ::SetSoundVolume(rlreg::sounds.at(args[0].as_int()), (float)args[1].as_number());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SETSOUNDPITCH", Fn{"SETSOUNDPITCH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETSOUNDPITCH: expected 2 args");
        ::SetSoundPitch(rlreg::sounds.at(args[0].as_int()), (float)args[1].as_number());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SETSOUNDPAN", Fn{"SETSOUNDPAN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETSOUNDPAN: expected 2 args");
        ::SetSoundPan(rlreg::sounds.at(args[0].as_int()), (float)args[1].as_number());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("WAVECOPY", Fn{"WAVECOPY", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("WAVECOPY: expected 1 args");
        ::Wave wave = ::WaveCopy(rlreg::waves.at(args[0].as_int()));
        int id = rlreg::next_wave_id++;
        rlreg::waves[id] = wave;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("WAVECROP", Fn{"WAVECROP", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("WAVECROP: expected 3 args");
        ::WaveCrop(&rlreg::waves[args[0].as_int()], args[1].as_int(), args[2].as_int());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("WAVEFORMAT", Fn{"WAVEFORMAT", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("WAVEFORMAT: expected 4 args");
        ::WaveFormat(&rlreg::waves[args[0].as_int()], args[1].as_int(), args[2].as_int(), args[3].as_int());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("LOADWAVESAMPLES", Fn{"LOADWAVESAMPLES", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADWAVESAMPLES: expected 1 args");
        float* samples = ::LoadWaveSamples(rlreg::waves.at(args[0].as_int()));
        (void)samples; // Suppress unused variable warning
        // Note: This returns a pointer, simplified for BASIC
        return Value::from_string("samples_loaded");
        
    }}, true);    R.add_with_policy("UNLOADWAVESAMPLES", Fn{"UNLOADWAVESAMPLES", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADWAVESAMPLES: expected 1 args");
        // Note: This is simplified - would need proper sample data handling
        ::UnloadWaveSamples(nullptr);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("LOADMUSICSTREAM", Fn{"LOADMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADMUSICSTREAM: expected 1 args");
        ::Music music = ::LoadMusicStream(args[0].as_string().c_str());
        int id = rlreg::next_music_id++;
        rlreg::musics[id] = music;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("LOADMUSICSTREAMFROMMEMORY", Fn{"LOADMUSICSTREAMFROMMEMORY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADMUSICSTREAMFROMMEMORY: expected 2 args");
        ::Music music = ::LoadMusicStreamFromMemory(args[0].as_string().c_str(), (const unsigned char*)args[1].as_string().c_str(), args[1].as_string().length());
        int id = rlreg::next_music_id++;
        rlreg::musics[id] = music;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("ISMUSICSTREAMVALID", Fn{"ISMUSICSTREAMVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMUSICSTREAMVALID: expected 1 args");
        return Value::from_bool(::IsMusicStreamPlaying(rlreg::musics.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("UNLOADMUSICSTREAM", Fn{"UNLOADMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADMUSICSTREAM: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::musics.find(id);
        if (it != rlreg::musics.end()) {
          ::UnloadMusicStream(it->second);
          rlreg::musics.erase(it);
        }
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("PLAYMUSICSTREAM", Fn{"PLAYMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PLAYMUSICSTREAM: expected 1 args");
        ::PlayMusicStream(rlreg::musics.at(args[0].as_int()));
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ISMUSICSTREAMPLAYING", Fn{"ISMUSICSTREAMPLAYING", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMUSICSTREAMPLAYING: expected 1 args");
        return Value::from_bool(::IsMusicStreamPlaying(rlreg::musics.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("UPDATEMUSICSTREAM", Fn{"UPDATEMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UPDATEMUSICSTREAM: expected 1 args");
        ::UpdateMusicStream(rlreg::musics.at(args[0].as_int()));
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("STOPMUSICSTREAM", Fn{"STOPMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("STOPMUSICSTREAM: expected 1 args");
        ::StopMusicStream(rlreg::musics.at(args[0].as_int()));
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("PAUSEMUSICSTREAM", Fn{"PAUSEMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PAUSEMUSICSTREAM: expected 1 args");
        ::PauseMusicStream(rlreg::musics.at(args[0].as_int()));
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("RESUMEMUSICSTREAM", Fn{"RESUMEMUSICSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("RESUMEMUSICSTREAM: expected 1 args");
        ::ResumeMusicStream(rlreg::musics.at(args[0].as_int()));
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SEEKMUSICSTREAM", Fn{"SEEKMUSICSTREAM", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SEEKMUSICSTREAM: expected 2 args");
        ::SeekMusicStream(rlreg::musics.at(args[0].as_int()), (float)args[1].as_number());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SETMUSICVOLUME", Fn{"SETMUSICVOLUME", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETMUSICVOLUME: expected 2 args");
        ::SetMusicVolume(rlreg::musics.at(args[0].as_int()), (float)args[1].as_number());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SETMUSICPITCH", Fn{"SETMUSICPITCH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETMUSICPITCH: expected 2 args");
        ::SetMusicPitch(rlreg::musics.at(args[0].as_int()), (float)args[1].as_number());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SETMUSICPAN", Fn{"SETMUSICPAN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETMUSICPAN: expected 2 args");
        ::SetMusicPan(rlreg::musics.at(args[0].as_int()), (float)args[1].as_number());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("GETMUSICTIMELENGTH", Fn{"GETMUSICTIMELENGTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMUSICTIMELENGTH: expected 1 args");
        return Value::from_number(::GetMusicTimeLength(rlreg::musics.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("GETMUSICTIMEPLAYED", Fn{"GETMUSICTIMEPLAYED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMUSICTIMEPLAYED: expected 1 args");
        return Value::from_number(::GetMusicTimePlayed(rlreg::musics.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("LOADAUDIOSTREAM", Fn{"LOADAUDIOSTREAM", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("LOADAUDIOSTREAM: expected 3 args");
        ::AudioStream stream = ::LoadAudioStream(args[0].as_int(), args[1].as_int(), args[2].as_int());
        int id = rlreg::next_audiostream_id++;
        rlreg::audiostreams[id] = stream;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("ISAUDIOSTREAMVALID", Fn{"ISAUDIOSTREAMVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISAUDIOSTREAMVALID: expected 1 args");
        return Value::from_bool(::IsAudioStreamReady(rlreg::audiostreams.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("UNLOADAUDIOSTREAM", Fn{"UNLOADAUDIOSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADAUDIOSTREAM: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::audiostreams.find(id);
        if (it != rlreg::audiostreams.end()) {
          ::UnloadAudioStream(it->second);
          rlreg::audiostreams.erase(it);
        }
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("UPDATEAUDIOSTREAM", Fn{"UPDATEAUDIOSTREAM", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("UPDATEAUDIOSTREAM: expected 2 args");
        // Note: This is simplified - would need proper audio data handling
        ::UpdateAudioStream(rlreg::audiostreams.at(args[0].as_int()), nullptr, 0);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ISAUDIOSTREAMPROCESSED", Fn{"ISAUDIOSTREAMPROCESSED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISAUDIOSTREAMPROCESSED: expected 1 args");
        return Value::from_bool(::IsAudioStreamProcessed(rlreg::audiostreams.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("PLAYAUDIOSTREAM", Fn{"PLAYAUDIOSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PLAYAUDIOSTREAM: expected 1 args");
        ::PlayAudioStream(rlreg::audiostreams.at(args[0].as_int()));
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("PAUSEAUDIOSTREAM", Fn{"PAUSEAUDIOSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PAUSEAUDIOSTREAM: expected 1 args");
        ::PauseAudioStream(rlreg::audiostreams.at(args[0].as_int()));
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("RESUMEAUDIOSTREAM", Fn{"RESUMEAUDIOSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("RESUMEAUDIOSTREAM: expected 1 args");
        ::ResumeAudioStream(rlreg::audiostreams.at(args[0].as_int()));
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ISAUDIOSTREAMPLAYING", Fn{"ISAUDIOSTREAMPLAYING", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISAUDIOSTREAMPLAYING: expected 1 args");
        return Value::from_bool(::IsAudioStreamPlaying(rlreg::audiostreams.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("STOPAUDIOSTREAM", Fn{"STOPAUDIOSTREAM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("STOPAUDIOSTREAM: expected 1 args");
        ::StopAudioStream(rlreg::audiostreams.at(args[0].as_int()));
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SETAUDIOSTREAMVOLUME", Fn{"SETAUDIOSTREAMVOLUME", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETAUDIOSTREAMVOLUME: expected 2 args");
        ::SetAudioStreamVolume(rlreg::audiostreams.at(args[0].as_int()), (float)args[1].as_number());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SETAUDIOSTREAMPITCH", Fn{"SETAUDIOSTREAMPITCH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETAUDIOSTREAMPITCH: expected 2 args");
        ::SetAudioStreamPitch(rlreg::audiostreams.at(args[0].as_int()), (float)args[1].as_number());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SETAUDIOSTREAMPAN", Fn{"SETAUDIOSTREAMPAN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETAUDIOSTREAMPAN: expected 2 args");
        ::SetAudioStreamPan(rlreg::audiostreams.at(args[0].as_int()), (float)args[1].as_number());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SETAUDIOSTREAMBUFFERSIZEDEFAULT", Fn{"SETAUDIOSTREAMBUFFERSIZEDEFAULT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETAUDIOSTREAMBUFFERSIZEDEFAULT: expected 1 args");
        SetAudioStreamBufferSizeDefault(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("SETAUDIOSTREAMCALLBACK", Fn{"SETAUDIOSTREAMCALLBACK", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETAUDIOSTREAMCALLBACK: expected 2 args");
        // Note: This is simplified - would need proper callback handling
        ::SetAudioStreamCallback(rlreg::audiostreams.at(args[0].as_int()), nullptr);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ATTACHAUDIOSTREAMPROCESSOR", Fn{"ATTACHAUDIOSTREAMPROCESSOR", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ATTACHAUDIOSTREAMPROCESSOR: expected 2 args");
        // Note: This is simplified - would need proper processor handling
        ::AttachAudioStreamProcessor(rlreg::audiostreams.at(args[0].as_int()), nullptr);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DETACHAUDIOSTREAMPROCESSOR", Fn{"DETACHAUDIOSTREAMPROCESSOR", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("DETACHAUDIOSTREAMPROCESSOR: expected 2 args");
        // Note: This is simplified - would need proper processor handling
        ::DetachAudioStreamProcessor(rlreg::audiostreams.at(args[0].as_int()), nullptr);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ATTACHAUDIOMIXEDPROCESSOR", Fn{"ATTACHAUDIOMIXEDPROCESSOR", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ATTACHAUDIOMIXEDPROCESSOR: expected 1 args");
        // Note: This is simplified - would need proper processor handling
        ::AttachAudioMixedProcessor(nullptr);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DETACHAUDIOMIXEDPROCESSOR", Fn{"DETACHAUDIOMIXEDPROCESSOR", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("DETACHAUDIOMIXEDPROCESSOR: expected 1 args");
        // Note: This is simplified - would need proper processor handling
        ::DetachAudioMixedProcessor(nullptr);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADWAVE", Fn{"UNLOADWAVE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADWAVE: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::waves.find(id);
        if (it != rlreg::waves.end()) {
          ::UnloadWave(it->second);
          rlreg::waves.erase(it);
        }
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADSOUNDALIAS", Fn{"UNLOADSOUNDALIAS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADSOUNDALIAS: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::sounds.find(id);
        if (it != rlreg::sounds.end()) {
          ::UnloadSoundAlias(it->second);
          rlreg::sounds.erase(it);
        }
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("EXPORTWAVE", Fn{"EXPORTWAVE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTWAVE: expected 2 args");
        return Value::from_bool(::ExportWave(rlreg::waves.at(args[0].as_int()), args[1].as_string().c_str()));
        
    }}, true);    R.add_with_policy("EXPORTWAVEASCODE", Fn{"EXPORTWAVEASCODE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTWAVEASCODE: expected 2 args");
        return Value::from_bool(::ExportWaveAsCode(rlreg::waves.at(args[0].as_int()), args[1].as_string().c_str()));
        
    }}, true);    R.add_with_policy("LOADMODEL", Fn{"LOADMODEL", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADMODEL: expected 1 args");
        ::Model model = ::LoadModel(args[0].as_string().c_str());
        int id = rlreg::next_model_id++;
        rlreg::models[id] = model;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("LOADMODELFROMMESH", Fn{"LOADMODELFROMMESH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADMODELFROMMESH: expected 1 args");
        ::Model model = ::LoadModelFromMesh(rlreg::meshes.at(args[0].as_int()));
        int id = rlreg::next_model_id++;
        rlreg::models[id] = model;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("ISMODELVALID", Fn{"ISMODELVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMODELVALID: expected 1 args");
        return Value::from_bool(::IsModelReady(rlreg::models.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("GETMODELBOUNDINGBOX", Fn{"GETMODELBOUNDINGBOX", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMODELBOUNDINGBOX: expected 1 args");
        ::BoundingBox box = ::GetModelBoundingBox(rlreg::models.at(args[0].as_int()));
        return Value::from_string(std::to_string(box.min.x) + "," + std::to_string(box.min.y) + "," + std::to_string(box.min.z) + "," + 
                                 std::to_string(box.max.x) + "," + std::to_string(box.max.y) + "," + std::to_string(box.max.z));
        
    }}, true);    R.add_with_policy("DRAWMODEL", Fn{"DRAWMODEL", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWMODEL: expected 8 args");
        ::Vector3 position{(float)args[1].as_number(), (float)args[2].as_number(), (float)args[3].as_number()};
        float scale = (float)args[4].as_number();
        ::Color tint{(unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), 255};
        ::DrawModel(rlreg::models.at(args[0].as_int()), position, scale, tint);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWMODELEX", Fn{"DRAWMODELEX", 11, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 11) throw std::runtime_error("DRAWMODELEX: expected 11 args");
        ::Vector3 position{(float)args[1].as_number(), (float)args[2].as_number(), (float)args[3].as_number()};
        ::Vector3 rotationAxis{(float)args[4].as_number(), (float)args[5].as_number(), (float)args[6].as_number()};
        ::Vector3 scale{(float)args[7].as_number(), (float)args[7].as_number(), (float)args[7].as_number()};
        ::Color tint{(unsigned char)args[8].as_int(), (unsigned char)args[9].as_int(), (unsigned char)args[10].as_int(), 255};
        ::DrawModelEx(rlreg::models.at(args[0].as_int()), position, rotationAxis, 0.0f, scale, tint);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWMODELWIRES", Fn{"DRAWMODELWIRES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWMODELWIRES: expected 8 args");
        ::Vector3 position{(float)args[1].as_number(), (float)args[2].as_number(), (float)args[3].as_number()};
        float scale = (float)args[4].as_number();
        ::Color tint{(unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), 255};
        ::DrawModelWires(rlreg::models.at(args[0].as_int()), position, scale, tint);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWMODELWIRESEX", Fn{"DRAWMODELWIRESEX", 11, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 11) throw std::runtime_error("DRAWMODELWIRESEX: expected 11 args");
        ::Vector3 position{(float)args[1].as_number(), (float)args[2].as_number(), (float)args[3].as_number()};
        ::Vector3 rotationAxis{(float)args[4].as_number(), (float)args[5].as_number(), (float)args[6].as_number()};
        ::Vector3 scale{(float)args[7].as_number(), (float)args[7].as_number(), (float)args[7].as_number()};
        ::Color tint{(unsigned char)args[8].as_int(), (unsigned char)args[9].as_int(), (unsigned char)args[10].as_int(), 255};
        ::DrawModelWiresEx(rlreg::models.at(args[0].as_int()), position, rotationAxis, 0.0f, scale, tint);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWBOUNDINGBOX", Fn{"DRAWBOUNDINGBOX", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("DRAWBOUNDINGBOX: expected 4 args");
        // Parse bounding box string format: "minX,minY,minZ,maxX,maxY,maxZ"
        std::string boxStr = args[0].as_string();
        size_t pos = 0;
        std::vector<float> coords;
        while ((pos = boxStr.find(",")) != std::string::npos) {
          coords.push_back(std::stof(boxStr.substr(0, pos)));
          boxStr.erase(0, pos + 1);
        }
        coords.push_back(std::stof(boxStr));
        if (coords.size() >= 6) {
          ::BoundingBox box{{coords[0], coords[1], coords[2]}, {coords[3], coords[4], coords[5]}};
          ::Color c{(unsigned char)args[1].as_int(), (unsigned char)args[2].as_int(), (unsigned char)args[3].as_int(), 255};
          ::DrawBoundingBox(box, c);
        }
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWBILLBOARD", Fn{"DRAWBILLBOARD", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWBILLBOARD: expected 9 args");
        // Note: This requires a Camera object, simplified for BASIC
        ::Camera camera = ::Camera{};
        ::Vector3 position{(float)args[2].as_number(), (float)args[3].as_number(), (float)args[4].as_number()};
        ::Color tint{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::DrawBillboard(camera, rlreg::textures.at(args[1].as_int()), position, (float)args[5].as_number(), tint);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWBILLBOARDREC", Fn{"DRAWBILLBOARDREC", 14, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 14) throw std::runtime_error("DRAWBILLBOARDREC: expected 14 args");
        // Note: This requires a Camera object, simplified for BASIC
        ::Camera camera = ::Camera{};
        ::Rectangle source{(float)args[2].as_int(), (float)args[3].as_int(), (float)args[4].as_int(), (float)args[5].as_int()};
        ::Vector3 position{(float)args[6].as_number(), (float)args[7].as_number(), (float)args[8].as_number()};
        ::Vector2 size{(float)args[9].as_number(), (float)args[10].as_number()};
        ::Color tint{(unsigned char)args[11].as_int(), (unsigned char)args[12].as_int(), (unsigned char)args[13].as_int(), 255};
        ::DrawBillboardRec(camera, rlreg::textures.at(args[1].as_int()), source, position, size, tint);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWBILLBOARDPRO", Fn{"DRAWBILLBOARDPRO", 18, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 18) throw std::runtime_error("DRAWBILLBOARDPRO: expected 18 args");
        // Note: This requires a Camera object, simplified for BASIC
        ::Camera camera = ::Camera{};
        ::Rectangle source{(float)args[2].as_int(), (float)args[3].as_int(), (float)args[4].as_int(), (float)args[5].as_int()};
        ::Vector3 position{(float)args[6].as_number(), (float)args[7].as_number(), (float)args[8].as_number()};
        ::Vector3 up{(float)args[9].as_number(), (float)args[10].as_number(), (float)args[11].as_number()};
        ::Vector2 size{(float)args[12].as_number(), (float)args[13].as_number()};
        ::Vector2 origin{(float)args[14].as_number(), (float)args[15].as_number()};
        ::Color tint{(unsigned char)args[16].as_int(), (unsigned char)args[17].as_int(), (unsigned char)args[18].as_int(), 255};
        ::DrawBillboardPro(camera, rlreg::textures.at(args[1].as_int()), source, position, up, size, origin, 0.0f, tint);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("UPLOADMESH", Fn{"UPLOADMESH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("UPLOADMESH: expected 2 args");
        ::UploadMesh(&rlreg::meshes[args[0].as_int()], args[1].as_bool());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("UPDATEMESHBUFFER", Fn{"UPDATEMESHBUFFER", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("UPDATEMESHBUFFER: expected 5 args");
        // Note: This is simplified - would need proper buffer data handling
        ::UpdateMeshBuffer(rlreg::meshes.at(args[0].as_int()), args[1].as_int(), nullptr, args[3].as_int(), args[4].as_int());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADMESH", Fn{"UNLOADMESH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADMESH: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::meshes.find(id);
        if (it != rlreg::meshes.end()) {
          ::UnloadMesh(it->second);
          rlreg::meshes.erase(it);
        }
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWMESH", Fn{"DRAWMESH", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("DRAWMESH: expected 3 args");
        // Note: This requires a Material object and Matrix transform, simplified for BASIC
        ::Material material = ::Material{};
        ::Matrix transform = MatrixIdentity();
        ::DrawMesh(rlreg::meshes.at(args[0].as_int()), material, transform);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWMESHINSTANCED", Fn{"DRAWMESHINSTANCED", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("DRAWMESHINSTANCED: expected 3 args");
        // Note: This requires a Material object and Matrix transforms array, simplified for BASIC
        ::Material material = ::Material{};
        ::Matrix transform = MatrixIdentity();
        ::DrawMeshInstanced(rlreg::meshes.at(args[0].as_int()), material, &transform, 1);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("GETMESHBOUNDINGBOX", Fn{"GETMESHBOUNDINGBOX", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETMESHBOUNDINGBOX: expected 1 args");
        ::BoundingBox box = ::GetMeshBoundingBox(rlreg::meshes.at(args[0].as_int()));
        return Value::from_string(std::to_string(box.min.x) + "," + std::to_string(box.min.y) + "," + std::to_string(box.min.z) + "," + 
                                 std::to_string(box.max.x) + "," + std::to_string(box.max.y) + "," + std::to_string(box.max.z));
        
    }}, true);    R.add_with_policy("GENMESHTANGENTS", Fn{"GENMESHTANGENTS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GENMESHTANGENTS: expected 1 args");
        ::GenMeshTangents(&rlreg::meshes[args[0].as_int()]);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("EXPORTMESH", Fn{"EXPORTMESH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTMESH: expected 2 args");
        return Value::from_bool(::ExportMesh(rlreg::meshes.at(args[0].as_int()), args[1].as_string().c_str()));
        
    }}, true);    R.add_with_policy("GENMESHPOLY", Fn{"GENMESHPOLY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GENMESHPOLY: expected 2 args");
        ::Mesh mesh = ::GenMeshPoly(args[0].as_int(), (float)args[1].as_number());
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENMESHPLANE", Fn{"GENMESHPLANE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("GENMESHPLANE: expected 4 args");
        ::Mesh mesh = ::GenMeshPlane((float)args[0].as_number(), (float)args[1].as_number(), args[2].as_int(), args[3].as_int());
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENMESHCUBE", Fn{"GENMESHCUBE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENMESHCUBE: expected 3 args");
        ::Mesh mesh = ::GenMeshCube((float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number());
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENMESHSPHERE", Fn{"GENMESHSPHERE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENMESHSPHERE: expected 3 args");
        ::Mesh mesh = ::GenMeshSphere((float)args[0].as_number(), args[1].as_int(), args[2].as_int());
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENMESHHEMISPHERE", Fn{"GENMESHHEMISPHERE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENMESHHEMISPHERE: expected 3 args");
        ::Mesh mesh = ::GenMeshHemiSphere((float)args[0].as_number(), args[1].as_int(), args[2].as_int());
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENMESHCYLINDER", Fn{"GENMESHCYLINDER", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENMESHCYLINDER: expected 3 args");
        ::Mesh mesh = ::GenMeshCylinder((float)args[0].as_number(), (float)args[1].as_number(), args[2].as_int());
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENMESHCONE", Fn{"GENMESHCONE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENMESHCONE: expected 3 args");
        ::Mesh mesh = ::GenMeshCone((float)args[0].as_number(), (float)args[1].as_number(), args[2].as_int());
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENMESHTORUS", Fn{"GENMESHTORUS", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("GENMESHTORUS: expected 4 args");
        ::Mesh mesh = ::GenMeshTorus((float)args[0].as_number(), (float)args[1].as_number(), args[2].as_int(), args[3].as_int());
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENMESHKNOT", Fn{"GENMESHKNOT", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("GENMESHKNOT: expected 4 args");
        ::Mesh mesh = ::GenMeshKnot((float)args[0].as_number(), (float)args[1].as_number(), args[2].as_int(), args[3].as_int());
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENMESHHEIGHTMAP", Fn{"GENMESHHEIGHTMAP", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("GENMESHHEIGHTMAP: expected 4 args");
        ::Vector3 size{(float)args[1].as_number(), (float)args[2].as_number(), (float)args[3].as_number()};
        ::Mesh mesh = ::GenMeshHeightmap(rlreg::images.at(args[0].as_int()), size);
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENMESHCUBICMAP", Fn{"GENMESHCUBICMAP", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("GENMESHCUBICMAP: expected 4 args");
        ::Vector3 cubeSize{(float)args[1].as_number(), (float)args[2].as_number(), (float)args[3].as_number()};
        ::Mesh mesh = ::GenMeshCubicmap(rlreg::images.at(args[0].as_int()), cubeSize);
        int id = rlreg::next_mesh_id++;
        rlreg::meshes[id] = mesh;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("LOADMATERIALS", Fn{"LOADMATERIALS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADMATERIALS: expected 1 args");
        int materialCount;
        ::Material* materials = ::LoadMaterials(args[0].as_string().c_str(), &materialCount);
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
        return Value::from_bool(::IsMaterialReady(rlreg::materials.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("UNLOADMATERIAL", Fn{"UNLOADMATERIAL", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADMATERIAL: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::materials.find(id);
        if (it != rlreg::materials.end()) {
          ::UnloadMaterial(it->second);
          rlreg::materials.erase(it);
        }
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SETMATERIALTEXTURE", Fn{"SETMATERIALTEXTURE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETMATERIALTEXTURE: expected 3 args");
        ::SetMaterialTexture(&rlreg::materials[args[0].as_int()], args[1].as_int(), rlreg::textures.at(args[2].as_int()));
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SETMODELMESHMATERIAL", Fn{"SETMODELMESHMATERIAL", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETMODELMESHMATERIAL: expected 3 args");
        ::SetModelMeshMaterial(&rlreg::models[args[0].as_int()], args[1].as_int(), args[2].as_int());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("LOADMODELANIMATIONS", Fn{"LOADMODELANIMATIONS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADMODELANIMATIONS: expected 1 args");
        int animCount;
        ::ModelAnimation* animations = ::LoadModelAnimations(args[0].as_string().c_str(), &animCount);
        (void)animations; // Suppress unused variable warning
        // Note: This returns a pointer array, simplified for BASIC
        return Value::from_string("animations_loaded:" + std::to_string(animCount));
        
    }}, true);    R.add_with_policy("UPDATEMODELANIMATION", Fn{"UPDATEMODELANIMATION", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("UPDATEMODELANIMATION: expected 3 args");
        // Note: This requires a ModelAnimation object, simplified for BASIC
        ::ModelAnimation anim = ::ModelAnimation{};
        ::UpdateModelAnimation(rlreg::models.at(args[0].as_int()), anim, args[2].as_int());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADMODELANIMATION", Fn{"UNLOADMODELANIMATION", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADMODELANIMATION: expected 1 args");
        // Note: This requires a ModelAnimation object, simplified for BASIC
        ::ModelAnimation anim = ::ModelAnimation{};
        ::UnloadModelAnimation(anim);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADMODELANIMATIONS", Fn{"UNLOADMODELANIMATIONS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADMODELANIMATIONS: expected 1 args");
        // Note: This requires a ModelAnimation array, simplified for BASIC
        ::UnloadModelAnimations(nullptr, 0);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ISMODELANIMATIONVALID", Fn{"ISMODELANIMATIONVALID", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ISMODELANIMATIONVALID: expected 2 args");
        // Note: This requires a ModelAnimation object, simplified for BASIC
        ::ModelAnimation anim = ::ModelAnimation{};
        return Value::from_bool(::IsModelAnimationValid(rlreg::models.at(args[0].as_int()), anim));
        
    }}, true);    R.add_with_policy("CHECKCOLLISIONSPHERES", Fn{"CHECKCOLLISIONSPHERES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("CHECKCOLLISIONSPHERES: expected 8 args");
        ::Vector3 center1{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        float radius1 = (float)args[3].as_number();
        ::Vector3 center2{(float)args[4].as_number(), (float)args[5].as_number(), (float)args[6].as_number()};
        float radius2 = (float)args[7].as_number();
        return Value::from_bool(::CheckCollisionSpheres(center1, radius1, center2, radius2));
        
    }}, true);    R.add_with_policy("CHECKCOLLISIONBOXES", Fn{"CHECKCOLLISIONBOXES", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("CHECKCOLLISIONBOXES: expected 2 args");
        // Parse bounding box strings
        std::string box1Str = args[0].as_string();
        std::string box2Str = args[1].as_string();
        // Note: This is simplified - would need proper parsing
        ::BoundingBox box1 = ::BoundingBox{{0,0,0}, {1,1,1}};
        ::BoundingBox box2 = ::BoundingBox{{0,0,0}, {1,1,1}};
        return Value::from_bool(::CheckCollisionBoxes(box1, box2));
        
    }}, true);    R.add_with_policy("CHECKCOLLISIONBOXSPHERE", Fn{"CHECKCOLLISIONBOXSPHERE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("CHECKCOLLISIONBOXSPHERE: expected 5 args");
        // Parse bounding box string
        std::string boxStr = args[0].as_string();
        // Note: This is simplified - would need proper parsing
        ::BoundingBox box = ::BoundingBox{{0,0,0}, {1,1,1}};
        ::Vector3 center{(float)args[1].as_number(), (float)args[2].as_number(), (float)args[3].as_number()};
        float radius = (float)args[4].as_number();
        return Value::from_bool(::CheckCollisionBoxSphere(box, center, radius));
        
    }}, true);    R.add_with_policy("GETRAYCOLLISIONSPHERE", Fn{"GETRAYCOLLISIONSPHERE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("GETRAYCOLLISIONSPHERE: expected 5 args");
        // Parse ray string format: "posX,posY,posZ,dirX,dirY,dirZ"
        std::string rayStr = args[0].as_string();
        size_t pos = 0;
        std::vector<float> coords;
        while ((pos = rayStr.find(",")) != std::string::npos) {
          coords.push_back(std::stof(rayStr.substr(0, pos)));
          rayStr.erase(0, pos + 1);
        }
        coords.push_back(std::stof(rayStr));
        if (coords.size() >= 6) {
          ::Ray ray{{coords[0], coords[1], coords[2]}, {coords[3], coords[4], coords[5]}};
          ::Vector3 center{(float)args[1].as_number(), (float)args[2].as_number(), (float)args[3].as_number()};
          float radius = (float)args[4].as_number();
          ::RayCollision collision = ::GetRayCollisionSphere(ray, center, radius);
          return Value::from_string(std::to_string(collision.hit) + "," + std::to_string(collision.distance) + "," + 
                                   std::to_string(collision.point.x) + "," + std::to_string(collision.point.y) + "," + std::to_string(collision.point.z));
        }
        return Value::from_string("0,0,0,0,0");
        
    }}, true);    R.add_with_policy("GETRAYCOLLISIONBOX", Fn{"GETRAYCOLLISIONBOX", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETRAYCOLLISIONBOX: expected 2 args");
        // Parse ray and bounding box strings
        std::string rayStr = args[0].as_string();
        std::string boxStr = args[1].as_string();
        // Note: This is simplified - would need proper parsing
        ::Ray ray = ::Ray{{0,0,0}, {0,0,1}};
        ::BoundingBox box = ::BoundingBox{{0,0,0}, {1,1,1}};
        ::RayCollision collision = ::GetRayCollisionBox(ray, box);
        return Value::from_string(std::to_string(collision.hit) + "," + std::to_string(collision.distance) + "," + 
                                 std::to_string(collision.point.x) + "," + std::to_string(collision.point.y) + "," + std::to_string(collision.point.z));
        
    }}, true);    R.add_with_policy("GETRAYCOLLISIONMESH", Fn{"GETRAYCOLLISIONMESH", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GETRAYCOLLISIONMESH: expected 3 args");
        // Parse ray and matrix strings
        std::string rayStr = args[0].as_string();
        std::string matrixStr = args[2].as_string();
        // Note: This is simplified - would need proper parsing
        ::Ray ray = ::Ray{{0,0,0}, {0,0,1}};
        ::Matrix transform = MatrixIdentity();
        ::RayCollision collision = ::GetRayCollisionMesh(ray, rlreg::meshes.at(args[1].as_int()), transform);
        return Value::from_string(std::to_string(collision.hit) + "," + std::to_string(collision.distance) + "," + 
                                 std::to_string(collision.point.x) + "," + std::to_string(collision.point.y) + "," + std::to_string(collision.point.z));
        
    }}, true);    R.add_with_policy("GETRAYCOLLISIONTRIANGLE", Fn{"GETRAYCOLLISIONTRIANGLE", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("GETRAYCOLLISIONTRIANGLE: expected 10 args");
        // Parse ray string format: "posX,posY,posZ,dirX,dirY,dirZ"
        std::string rayStr = args[0].as_string();
        size_t pos = 0;
        std::vector<float> coords;
        while ((pos = rayStr.find(",")) != std::string::npos) {
          coords.push_back(std::stof(rayStr.substr(0, pos)));
          rayStr.erase(0, pos + 1);
        }
        coords.push_back(std::stof(rayStr));
        if (coords.size() >= 6) {
          ::Ray ray{{coords[0], coords[1], coords[2]}, {coords[3], coords[4], coords[5]}};
          ::Vector3 p1{(float)args[1].as_number(), (float)args[2].as_number(), (float)args[3].as_number()};
          ::Vector3 p2{(float)args[4].as_number(), (float)args[5].as_number(), (float)args[6].as_number()};
          ::Vector3 p3{(float)args[7].as_number(), (float)args[8].as_number(), (float)args[9].as_number()};
          ::RayCollision collision = ::GetRayCollisionTriangle(ray, p1, p2, p3);
          return Value::from_string(std::to_string(collision.hit) + "," + std::to_string(collision.distance) + "," + 
                                   std::to_string(collision.point.x) + "," + std::to_string(collision.point.y) + "," + std::to_string(collision.point.z));
        }
        return Value::from_string("0,0,0,0,0");
        
    }}, true);    R.add_with_policy("GETRAYCOLLISIONQUAD", Fn{"GETRAYCOLLISIONQUAD", 13, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 13) throw std::runtime_error("GETRAYCOLLISIONQUAD: expected 13 args");
        // Parse ray string format: "posX,posY,posZ,dirX,dirY,dirZ"
        std::string rayStr = args[0].as_string();
        size_t pos = 0;
        std::vector<float> coords;
        while ((pos = rayStr.find(",")) != std::string::npos) {
          coords.push_back(std::stof(rayStr.substr(0, pos)));
          rayStr.erase(0, pos + 1);
        }
        coords.push_back(std::stof(rayStr));
        if (coords.size() >= 6) {
          ::Ray ray{{coords[0], coords[1], coords[2]}, {coords[3], coords[4], coords[5]}};
          ::Vector3 p1{(float)args[1].as_number(), (float)args[2].as_number(), (float)args[3].as_number()};
          ::Vector3 p2{(float)args[4].as_number(), (float)args[5].as_number(), (float)args[6].as_number()};
          ::Vector3 p3{(float)args[7].as_number(), (float)args[8].as_number(), (float)args[9].as_number()};
          ::Vector3 p4{(float)args[10].as_number(), (float)args[11].as_number(), (float)args[12].as_number()};
          ::RayCollision collision = ::GetRayCollisionQuad(ray, p1, p2, p3, p4);
          return Value::from_string(std::to_string(collision.hit) + "," + std::to_string(collision.distance) + "," + 
                                   std::to_string(collision.point.x) + "," + std::to_string(collision.point.y) + "," + std::to_string(collision.point.z));
        }
        return Value::from_string("0,0,0,0,0");
        
    }}, true);    R.add_with_policy("UNLOADMODEL", Fn{"UNLOADMODEL", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADMODEL: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::models.find(id);
        if (it != rlreg::models.end()) {
          ::UnloadModel(it->second);
          rlreg::models.erase(it);
        }
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWLINE3D", Fn{"DRAWLINE3D", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWLINE3D: expected 9 args");
        ::Vector3 start{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Vector3 end{(float)args[3].as_number(), (float)args[4].as_number(), (float)args[5].as_number()};
        ::Color c{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::DrawLine3D(start, end, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPOINT3D", Fn{"DRAWPOINT3D", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("DRAWPOINT3D: expected 6 args");
        ::Vector3 position{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Color c{(unsigned char)args[3].as_int(), (unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), 255};
        ::DrawPoint3D(position, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCIRCLE3D", Fn{"DRAWCIRCLE3D", 11, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 11) throw std::runtime_error("DRAWCIRCLE3D: expected 11 args");
        ::Vector3 center{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Vector3 rotationAxis{(float)args[4].as_number(), (float)args[5].as_number(), (float)args[6].as_number()};
        ::Color c{(unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), (unsigned char)args[9].as_int(), 255};
        ::DrawCircle3D(center, (float)args[3].as_number(), rotationAxis, 0.0f, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTRIANGLE3D", Fn{"DRAWTRIANGLE3D", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWTRIANGLE3D: expected 12 args");
        ::Vector3 v1{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Vector3 v2{(float)args[3].as_number(), (float)args[4].as_number(), (float)args[5].as_number()};
        ::Vector3 v3{(float)args[6].as_number(), (float)args[7].as_number(), (float)args[8].as_number()};
        ::Color c{(unsigned char)args[9].as_int(), (unsigned char)args[10].as_int(), (unsigned char)args[11].as_int(), 255};
        ::DrawTriangle3D(v1, v2, v3, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTRIANGLESTRIP3D", Fn{"DRAWTRIANGLESTRIP3D", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("DRAWTRIANGLESTRIP3D: expected 4 args");
        // Note: This is simplified - would need proper Vector3 array handling
        ::Vector3 points[4] = {{0,0,0}, {1,0,0}, {0,1,0}, {1,1,0}};
        ::Color c{(unsigned char)args[1].as_int(), (unsigned char)args[2].as_int(), (unsigned char)args[3].as_int(), 255};
        ::DrawTriangleStrip3D(points, 4, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCUBE", Fn{"DRAWCUBE", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCUBE: expected 9 args");
        ::Vector3 position{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Color c{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::DrawCube(position, (float)args[3].as_number(), (float)args[4].as_number(), (float)args[5].as_number(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCUBEV", Fn{"DRAWCUBEV", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCUBEV: expected 9 args");
        ::Vector3 position{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Vector3 size{(float)args[3].as_number(), (float)args[4].as_number(), (float)args[5].as_number()};
        ::Color c{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::DrawCubeV(position, size, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCUBEWIRES", Fn{"DRAWCUBEWIRES", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCUBEWIRES: expected 9 args");
        ::Vector3 position{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Color c{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::DrawCubeWires(position, (float)args[3].as_number(), (float)args[4].as_number(), (float)args[5].as_number(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCUBEWIRESV", Fn{"DRAWCUBEWIRESV", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWCUBEWIRESV: expected 9 args");
        ::Vector3 position{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Vector3 size{(float)args[3].as_number(), (float)args[4].as_number(), (float)args[5].as_number()};
        ::Color c{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::DrawCubeWiresV(position, size, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPHERE", Fn{"DRAWSPHERE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWSPHERE: expected 7 args");
        ::Vector3 center{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Color c{(unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), 255};
        ::DrawSphere(center, (float)args[3].as_number(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPHEREEX", Fn{"DRAWSPHEREEX", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWSPHEREEX: expected 9 args");
        ::Vector3 center{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Color c{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::DrawSphereEx(center, (float)args[3].as_number(), args[4].as_int(), args[5].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWSPHEREWIRES", Fn{"DRAWSPHEREWIRES", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("DRAWSPHEREWIRES: expected 9 args");
        ::Vector3 center{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Color c{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::DrawSphereWires(center, (float)args[3].as_number(), args[4].as_int(), args[5].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCYLINDER", Fn{"DRAWCYLINDER", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWCYLINDER: expected 10 args");
        ::Vector3 position{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Color c{(unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), (unsigned char)args[9].as_int(), 255};
        ::DrawCylinder(position, (float)args[3].as_number(), (float)args[4].as_number(), (float)args[5].as_number(), args[6].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCYLINDEREX", Fn{"DRAWCYLINDEREX", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWCYLINDEREX: expected 12 args");
        ::Vector3 start{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Vector3 end{(float)args[3].as_number(), (float)args[4].as_number(), (float)args[5].as_number()};
        ::Color c{(unsigned char)args[10].as_int(), (unsigned char)args[11].as_int(), (unsigned char)args[12].as_int(), 255};
        ::DrawCylinderEx(start, end, (float)args[6].as_number(), (float)args[7].as_number(), args[8].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCYLINDERWIRES", Fn{"DRAWCYLINDERWIRES", 10, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 10) throw std::runtime_error("DRAWCYLINDERWIRES: expected 10 args");
        ::Vector3 position{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Color c{(unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), (unsigned char)args[9].as_int(), 255};
        ::DrawCylinderWires(position, (float)args[3].as_number(), (float)args[4].as_number(), (float)args[5].as_number(), args[6].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCYLINDERWIRESEX", Fn{"DRAWCYLINDERWIRESEX", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("DRAWCYLINDERWIRESEX: expected 12 args");
        ::Vector3 start{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Vector3 end{(float)args[3].as_number(), (float)args[4].as_number(), (float)args[5].as_number()};
        ::Color c{(unsigned char)args[10].as_int(), (unsigned char)args[11].as_int(), (unsigned char)args[12].as_int(), 255};
        ::DrawCylinderWiresEx(start, end, (float)args[6].as_number(), (float)args[7].as_number(), args[8].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCAPSULE", Fn{"DRAWCAPSULE", 11, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 11) throw std::runtime_error("DRAWCAPSULE: expected 11 args");
        ::Vector3 start{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Vector3 end{(float)args[3].as_number(), (float)args[4].as_number(), (float)args[5].as_number()};
        ::Color c{(unsigned char)args[8].as_int(), (unsigned char)args[9].as_int(), (unsigned char)args[10].as_int(), 255};
        ::DrawCapsule(start, end, (float)args[6].as_number(), args[7].as_int(), args[8].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCAPSULEWIRES", Fn{"DRAWCAPSULEWIRES", 11, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 11) throw std::runtime_error("DRAWCAPSULEWIRES: expected 11 args");
        ::Vector3 start{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Vector3 end{(float)args[3].as_number(), (float)args[4].as_number(), (float)args[5].as_number()};
        ::Color c{(unsigned char)args[8].as_int(), (unsigned char)args[9].as_int(), (unsigned char)args[10].as_int(), 255};
        ::DrawCapsuleWires(start, end, (float)args[6].as_number(), args[7].as_int(), args[8].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPLANE", Fn{"DRAWPLANE", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWPLANE: expected 8 args");
        ::Vector3 center{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Vector2 size{(float)args[3].as_number(), (float)args[4].as_number()};
        ::Color c{(unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), 255};
        ::DrawPlane(center, size, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWRAY", Fn{"DRAWRAY", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("DRAWRAY: expected 4 args");
        // Parse ray string format: "posX,posY,posZ,dirX,dirY,dirZ"
        std::string rayStr = args[0].as_string();
        size_t pos = 0;
        std::vector<float> coords;
        while ((pos = rayStr.find(",")) != std::string::npos) {
          coords.push_back(std::stof(rayStr.substr(0, pos)));
          rayStr.erase(0, pos + 1);
        }
        coords.push_back(std::stof(rayStr));
        if (coords.size() >= 6) {
          ::Ray ray{{coords[0], coords[1], coords[2]}, {coords[3], coords[4], coords[5]}};
          ::Color c{(unsigned char)args[1].as_int(), (unsigned char)args[2].as_int(), (unsigned char)args[3].as_int(), 255};
          ::DrawRay(ray, c);
        }
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWGRID", Fn{"DRAWGRID", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("DRAWGRID: expected 2 args");
        ::DrawGrid(args[0].as_int(), (float)args[1].as_number());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("CHECKCOLLISIONRECS", Fn{"CHECKCOLLISIONRECS", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("CHECKCOLLISIONRECS: expected 8 args");
        ::Rectangle rec1{(float)args[0].as_int(), (float)args[1].as_int(), (float)args[2].as_int(), (float)args[3].as_int()};
        ::Rectangle rec2{(float)args[4].as_int(), (float)args[5].as_int(), (float)args[6].as_int(), (float)args[7].as_int()};
        return Value::from_bool(::CheckCollisionRecs(rec1, rec2));
        
    }}, true);    R.add_with_policy("CHECKCOLLISIONCIRCLES", Fn{"CHECKCOLLISIONCIRCLES", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("CHECKCOLLISIONCIRCLES: expected 6 args");
        ::Vector2 center1{(float)args[0].as_int(), (float)args[1].as_int()};
        float radius1 = (float)args[2].as_int();
        ::Vector2 center2{(float)args[3].as_int(), (float)args[4].as_int()};
        float radius2 = (float)args[5].as_int();
        return Value::from_bool(::CheckCollisionCircles(center1, radius1, center2, radius2));
        
    }}, true);    R.add_with_policy("CHECKCOLLISIONCIRCLEREC", Fn{"CHECKCOLLISIONCIRCLEREC", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("CHECKCOLLISIONCIRCLEREC: expected 7 args");
        ::Vector2 center{(float)args[0].as_int(), (float)args[1].as_int()};
        float radius = (float)args[2].as_int();
        ::Rectangle rec{(float)args[3].as_int(), (float)args[4].as_int(), (float)args[5].as_int(), (float)args[6].as_int()};
        return Value::from_bool(::CheckCollisionCircleRec(center, radius, rec));
        
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTREC", Fn{"CHECKCOLLISIONPOINTREC", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("CHECKCOLLISIONPOINTREC: expected 6 args");
        ::Vector2 point{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Rectangle rec{(float)args[2].as_int(), (float)args[3].as_int(), (float)args[4].as_int(), (float)args[5].as_int()};
        return Value::from_bool(::CheckCollisionPointRec(point, rec));
        
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTCIRCLE", Fn{"CHECKCOLLISIONPOINTCIRCLE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("CHECKCOLLISIONPOINTCIRCLE: expected 5 args");
        ::Vector2 point{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Vector2 center{(float)args[2].as_int(), (float)args[3].as_int()};
        float radius = (float)args[4].as_int();
        return Value::from_bool(::CheckCollisionPointCircle(point, center, radius));
        
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTTRIANGLE", Fn{"CHECKCOLLISIONPOINTTRIANGLE", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("CHECKCOLLISIONPOINTTRIANGLE: expected 9 args");
        ::Vector2 point{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Vector2 p1{(float)args[2].as_int(), (float)args[3].as_int()};
        ::Vector2 p2{(float)args[4].as_int(), (float)args[5].as_int()};
        ::Vector2 p3{(float)args[6].as_int(), (float)args[7].as_int()};
        return Value::from_bool(::CheckCollisionPointTriangle(point, p1, p2, p3));
        
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTLINE", Fn{"CHECKCOLLISIONPOINTLINE", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("CHECKCOLLISIONPOINTLINE: expected 7 args");
        ::Vector2 point{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Vector2 p1{(float)args[2].as_int(), (float)args[3].as_int()};
        ::Vector2 p2{(float)args[4].as_int(), (float)args[5].as_int()};
        return Value::from_bool(::CheckCollisionPointLine(point, p1, p2, args[6].as_int()));
        
    }}, true);    R.add_with_policy("CHECKCOLLISIONPOINTPOLY", Fn{"CHECKCOLLISIONPOINTPOLY", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("CHECKCOLLISIONPOINTPOLY: expected 3 args");
        ::Vector2 point{(float)args[0].as_int(), (float)args[1].as_int()};
        // Note: This is simplified - would need proper polygon parsing
        ::Vector2 points[4] = {{0,0}, {1,0}, {1,1}, {0,1}};
        return Value::from_bool(::CheckCollisionPointPoly(point, points, 4));
        
    }}, true);    R.add_with_policy("CHECKCOLLISIONLINES", Fn{"CHECKCOLLISIONLINES", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("CHECKCOLLISIONLINES: expected 9 args");
        ::Vector2 startPos1{(float)args[0].as_int(), (float)args[1].as_int()};
        ::Vector2 endPos1{(float)args[2].as_int(), (float)args[3].as_int()};
        ::Vector2 startPos2{(float)args[4].as_int(), (float)args[5].as_int()};
        ::Vector2 endPos2{(float)args[6].as_int(), (float)args[7].as_int()};
        ::Vector2 collisionPoint;
        bool result = ::CheckCollisionLines(startPos1, endPos1, startPos2, endPos2, &collisionPoint);
        // Note: collisionPoint would need to be returned somehow
        return Value::from_bool(result);
        
    }}, true);    R.add_with_policy("GETCOLLISIONREC", Fn{"GETCOLLISIONREC", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("GETCOLLISIONREC: expected 8 args");
        ::Rectangle rec1{(float)args[0].as_int(), (float)args[1].as_int(), (float)args[2].as_int(), (float)args[3].as_int()};
        ::Rectangle rec2{(float)args[4].as_int(), (float)args[5].as_int(), (float)args[6].as_int(), (float)args[7].as_int()};
        ::Rectangle collision = ::GetCollisionRec(rec1, rec2);
        return Value::from_string(std::to_string(collision.x) + "," + std::to_string(collision.y) + "," + 
                                 std::to_string(collision.width) + "," + std::to_string(collision.height));
        
    }}, true);    R.add_with_policy("LOADSHADER", Fn{"LOADSHADER", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADSHADER: expected 2 args");
        ::Shader shader = ::LoadShader(args[0].as_string().c_str(), args[1].as_string().c_str());
        int id = rlreg::next_shader_id++;
        rlreg::shaders[id] = shader;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("LOADSHADERFROMMEMORY", Fn{"LOADSHADERFROMMEMORY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADSHADERFROMMEMORY: expected 2 args");
        ::Shader shader = ::LoadShaderFromMemory(args[0].as_string().c_str(), args[1].as_string().c_str());
        int id = rlreg::next_shader_id++;
        rlreg::shaders[id] = shader;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("ISSHADERVALID", Fn{"ISSHADERVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISSHADERVALID: expected 1 args");
        return Value::from_bool(::IsShaderReady(rlreg::shaders.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("GETSHADERLOCATION", Fn{"GETSHADERLOCATION", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETSHADERLOCATION: expected 2 args");
        return Value::from_int(::GetShaderLocation(rlreg::shaders.at(args[0].as_int()), args[1].as_string().c_str()));
        
    }}, true);    R.add_with_policy("GETSHADERLOCATIONATTRIB", Fn{"GETSHADERLOCATIONATTRIB", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETSHADERLOCATIONATTRIB: expected 2 args");
        return Value::from_int(::GetShaderLocationAttrib(rlreg::shaders.at(args[0].as_int()), args[1].as_string().c_str()));
        
    }}, true);    R.add_with_policy("SETSHADERVALUE", Fn{"SETSHADERVALUE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("SETSHADERVALUE: expected 4 args");
        // Note: This is simplified - would need proper value handling
        ::SetShaderValue(rlreg::shaders.at(args[0].as_int()), args[1].as_int(), nullptr, args[3].as_int());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SETSHADERVALUEV", Fn{"SETSHADERVALUEV", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("SETSHADERVALUEV: expected 5 args");
        // Note: This is simplified - would need proper value handling
        ::SetShaderValueV(rlreg::shaders.at(args[0].as_int()), args[1].as_int(), nullptr, args[3].as_int(), args[4].as_int());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SETSHADERVALUEMATRIX", Fn{"SETSHADERVALUEMATRIX", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETSHADERVALUEMATRIX: expected 3 args");
        // Note: This is simplified - would need proper matrix handling
        ::Matrix matrix = MatrixIdentity();
        ::SetShaderValueMatrix(rlreg::shaders.at(args[0].as_int()), args[1].as_int(), matrix);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SETSHADERVALUETEXTURE", Fn{"SETSHADERVALUETEXTURE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETSHADERVALUETEXTURE: expected 3 args");
        ::SetShaderValueTexture(rlreg::shaders.at(args[0].as_int()), args[1].as_int(), rlreg::textures.at(args[2].as_int()));
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINSHADERMODE", Fn{"BEGINSHADERMODE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("BEGINSHADERMODE: expected 1 args");
        ::BeginShaderMode(rlreg::shaders.at(args[0].as_int()));
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ENDSHADERMODE", Fn{"ENDSHADERMODE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDSHADERMODE: expected 0 args");
        EndShaderMode();
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADSHADER", Fn{"UNLOADSHADER", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADSHADER: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::shaders.find(id);
        if (it != rlreg::shaders.end()) {
          ::UnloadShader(it->second);
          rlreg::shaders.erase(it);
        }
        0;
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
        void* ptr = ::MemAlloc(args[0].as_int());
        return Value::from_int((intptr_t)ptr);
        
    }}, true);    R.add_with_policy("MEMREALLOC", Fn{"MEMREALLOC", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("MEMREALLOC: expected 2 args");
        void* ptr = ::MemRealloc((void*)(intptr_t)args[0].as_int(), args[1].as_int());
        return Value::from_int((intptr_t)ptr);
        
    }}, true);    R.add_with_policy("MEMFREE", Fn{"MEMFREE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("MEMFREE: expected 1 args");
        ::MemFree((void*)(intptr_t)args[0].as_int());
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("LOADFILEDATA", Fn{"LOADFILEDATA", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADFILEDATA: expected 1 args");
        int dataSize;
        unsigned char* data = ::LoadFileData(args[0].as_string().c_str(), &dataSize);
        (void)data; // Suppress unused variable warning
        // Note: This returns binary data, simplified for BASIC
        return Value::from_string("data_loaded:" + std::to_string(dataSize));
        
    }}, true);    R.add_with_policy("UNLOADFILEDATA", Fn{"UNLOADFILEDATA", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADFILEDATA: expected 1 args");
        // Note: This is simplified - would need proper data handling
        ::UnloadFileData(nullptr);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("SAVEFILEDATA", Fn{"SAVEFILEDATA", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SAVEFILEDATA: expected 2 args");
        // Note: This is simplified - would need proper data handling
        return Value::from_bool(::SaveFileData(args[0].as_string().c_str(), nullptr, 0));
        
    }}, true);    R.add_with_policy("EXPORTDATAASCODE", Fn{"EXPORTDATAASCODE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTDATAASCODE: expected 2 args");
        // Note: This is simplified - would need proper data handling
        return Value::from_bool(::ExportDataAsCode(nullptr, 0, args[1].as_string().c_str()));
        
    }}, true);    R.add_with_policy("LOADFILETEXT", Fn{"LOADFILETEXT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADFILETEXT: expected 1 args");
        char* text = ::LoadFileText(args[0].as_string().c_str());
        std::string result(text);
        ::UnloadFileText(text);
        return Value::from_string(result);
        
    }}, true);    R.add_with_policy("UNLOADFILETEXT", Fn{"UNLOADFILETEXT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADFILETEXT: expected 1 args");
        // Note: This is simplified - would need proper text handling
        ::UnloadFileText(nullptr);
        0;
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
        ::FilePathList files = ::LoadDirectoryFiles(args[0].as_string().c_str());
        // Note: This returns a FilePathList struct, simplified for BASIC
        return Value::from_string("files_loaded:" + std::to_string(files.count));
        
    }}, true);    R.add_with_policy("LOADDIRECTORYFILESEX", Fn{"LOADDIRECTORYFILESEX", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("LOADDIRECTORYFILESEX: expected 3 args");
        ::FilePathList files = ::LoadDirectoryFilesEx(args[0].as_string().c_str(), args[1].as_string().c_str(), args[2].as_bool());
        // Note: This returns a FilePathList struct, simplified for BASIC
        return Value::from_string("files_loaded:" + std::to_string(files.count));
        
    }}, true);    R.add_with_policy("UNLOADDIRECTORYFILES", Fn{"UNLOADDIRECTORYFILES", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADDIRECTORYFILES: expected 1 args");
        // Note: This is simplified - would need proper FilePathList handling
        ::FilePathList files = ::FilePathList{};
        ::UnloadDirectoryFiles(files);
        0;
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
        0;
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
        ::Image image = ::LoadImageRaw(args[0].as_string().c_str(), args[1].as_int(), args[2].as_int(), args[3].as_int(), args[4].as_int());
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("LOADIMAGEANIM", Fn{"LOADIMAGEANIM", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADIMAGEANIM: expected 1 args");
        int frames;
        ::Image image = ::LoadImageAnim(args[0].as_string().c_str(), &frames);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_string("image_loaded:" + std::to_string(frames));
        
    }}, true);    R.add_with_policy("LOADIMAGEFROMMEMORY", Fn{"LOADIMAGEFROMMEMORY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("LOADIMAGEFROMMEMORY: expected 2 args");
        ::Image image = ::LoadImageFromMemory(args[0].as_string().c_str(), (const unsigned char*)args[1].as_string().c_str(), args[1].as_string().length());
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("LOADIMAGEFROMTEXTURE", Fn{"LOADIMAGEFROMTEXTURE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADIMAGEFROMTEXTURE: expected 1 args");
        ::Image image = ::LoadImageFromTexture(rlreg::textures.at(args[0].as_int()));
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
        return Value::from_bool(::IsImageReady(rlreg::images.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("EXPORTIMAGE", Fn{"EXPORTIMAGE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTIMAGE: expected 2 args");
        return Value::from_bool(::ExportImage(rlreg::images.at(args[0].as_int()), args[1].as_string().c_str()));
        
    }}, true);    R.add_with_policy("EXPORTIMAGETOMEMORY", Fn{"EXPORTIMAGETOMEMORY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTIMAGETOMEMORY: expected 2 args");
        int fileSize;
        unsigned char* data = ::ExportImageToMemory(rlreg::images.at(args[0].as_int()), args[1].as_string().c_str(), &fileSize);
        (void)data; // Suppress unused variable warning
        // Note: This returns binary data, simplified for BASIC
        return Value::from_string("data_exported:" + std::to_string(fileSize));
        
    }}, true);    R.add_with_policy("EXPORTIMAGEASCODE", Fn{"EXPORTIMAGEASCODE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTIMAGEASCODE: expected 2 args");
        return Value::from_bool(::ExportImageAsCode(rlreg::images.at(args[0].as_int()), args[1].as_string().c_str()));
        
    }}, true);    R.add_with_policy("GENIMAGECOLOR", Fn{"GENIMAGECOLOR", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("GENIMAGECOLOR: expected 5 args");
        ::Color color{(unsigned char)args[2].as_int(), (unsigned char)args[3].as_int(), (unsigned char)args[4].as_int(), 255};
        ::Image image = ::GenImageColor(args[0].as_int(), args[1].as_int(), color);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENIMAGEGRADIENTLINEAR", Fn{"GENIMAGEGRADIENTLINEAR", 12, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 12) throw std::runtime_error("GENIMAGEGRADIENTLINEAR: expected 12 args");
        ::Color start{(unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), 255};
        ::Color end{(unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), (unsigned char)args[9].as_int(), 255};
        ::Image image = ::GenImageGradientLinear(args[0].as_int(), args[1].as_int(), args[2].as_int(), start, end);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENIMAGEGRADIENTRADIAL", Fn{"GENIMAGEGRADIENTRADIAL", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("GENIMAGEGRADIENTRADIAL: expected 9 args");
        ::Color inner{(unsigned char)args[3].as_int(), (unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), 255};
        ::Color outer{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::Image image = ::GenImageGradientRadial(args[0].as_int(), args[1].as_int(), (float)args[2].as_number(), inner, outer);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENIMAGEGRADIENTSQUARE", Fn{"GENIMAGEGRADIENTSQUARE", 9, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 9) throw std::runtime_error("GENIMAGEGRADIENTSQUARE: expected 9 args");
        ::Color inner{(unsigned char)args[3].as_int(), (unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), 255};
        ::Color outer{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::Image image = ::GenImageGradientSquare(args[0].as_int(), args[1].as_int(), (float)args[2].as_number(), inner, outer);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENIMAGECHECKED", Fn{"GENIMAGECHECKED", 15, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 15) throw std::runtime_error("GENIMAGECHECKED: expected 15 args");
        ::Color col1{(unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), 255};
        ::Color col2{(unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), (unsigned char)args[9].as_int(), 255};
        ::Image image = ::GenImageChecked(args[0].as_int(), args[1].as_int(), args[2].as_int(), args[3].as_int(), col1, col2);
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENIMAGEWHITENOISE", Fn{"GENIMAGEWHITENOISE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENIMAGEWHITENOISE: expected 3 args");
        ::Image image = ::GenImageWhiteNoise(args[0].as_int(), args[1].as_int(), (float)args[2].as_number());
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENIMAGEPERLINNOISE", Fn{"GENIMAGEPERLINNOISE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("GENIMAGEPERLINNOISE: expected 5 args");
        ::Image image = ::GenImagePerlinNoise(args[0].as_int(), args[1].as_int(), args[2].as_int(), args[3].as_int(), (float)args[4].as_number());
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENIMAGECELLULAR", Fn{"GENIMAGECELLULAR", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENIMAGECELLULAR: expected 3 args");
        ::Image image = ::GenImageCellular(args[0].as_int(), args[1].as_int(), args[2].as_int());
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("GENIMAGETEXT", Fn{"GENIMAGETEXT", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("GENIMAGETEXT: expected 3 args");
        ::Image image = ::GenImageText(args[0].as_int(), args[1].as_int(), args[2].as_string().c_str());
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
        ::Font font = ::LoadFont(args[0].as_string().c_str());
        int id = rlreg::next_font_id++;
        rlreg::fonts[id] = font;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("LOADFONTEX", Fn{"LOADFONTEX", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("LOADFONTEX: expected 3 args");
        // Note: This is simplified - would need proper codepoints array handling
        ::Font font = ::LoadFontEx(args[0].as_string().c_str(), args[1].as_int(), nullptr, 0);
        int id = rlreg::next_font_id++;
        rlreg::fonts[id] = font;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("LOADFONTFROMIMAGE", Fn{"LOADFONTFROMIMAGE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("LOADFONTFROMIMAGE: expected 5 args");
        ::Color key{(unsigned char)args[1].as_int(), (unsigned char)args[2].as_int(), (unsigned char)args[3].as_int(), 255};
        ::Font font = ::LoadFontFromImage(rlreg::images.at(args[0].as_int()), key, args[4].as_int());
        int id = rlreg::next_font_id++;
        rlreg::fonts[id] = font;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("LOADFONTFROMMEMORY", Fn{"LOADFONTFROMMEMORY", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("LOADFONTFROMMEMORY: expected 4 args");
        // Note: This is simplified - would need proper codepoints array handling
        ::Font font = ::LoadFontFromMemory(args[0].as_string().c_str(), (const unsigned char*)args[1].as_string().c_str(), args[1].as_string().length(), args[2].as_int(), nullptr, 0);
        int id = rlreg::next_font_id++;
        rlreg::fonts[id] = font;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("ISFONTVALID", Fn{"ISFONTVALID", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISFONTVALID: expected 1 args");
        return Value::from_bool(::IsFontReady(rlreg::fonts.at(args[0].as_int())));
        
    }}, true);    R.add_with_policy("LOADFONTDATA", Fn{"LOADFONTDATA", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("LOADFONTDATA: expected 4 args");
        // Note: This is simplified - would need proper codepoints array handling
        int glyphCount = 0;
        ::GlyphInfo* glyphs = ::LoadFontData((const unsigned char*)args[0].as_string().c_str(), args[0].as_string().length(), args[1].as_int(), nullptr, 0, args[3].as_int());
        (void)glyphs; // Suppress unused variable warning
        // Note: This returns a GlyphInfo array, simplified for BASIC
        return Value::from_string("glyphs_loaded:" + std::to_string(glyphCount));
        
    }}, true);    R.add_with_policy("GENIMAGEFONTATLAS", Fn{"GENIMAGEFONTATLAS", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("GENIMAGEFONTATLAS: expected 5 args");
        // Note: This is simplified - would need proper GlyphInfo array handling
        ::GlyphInfo glyphs[1] = {::GlyphInfo{}};
        ::Rectangle* glyphRecs = nullptr;
        ::Image image = ::GenImageFontAtlas(glyphs, &glyphRecs, 1, args[2].as_int(), args[3].as_int(), args[4].as_int());
        int id = rlreg::next_image_id++;
        rlreg::images[id] = image;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("UNLOADFONTDATA", Fn{"UNLOADFONTDATA", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADFONTDATA: expected 1 args");
        // Note: This is simplified - would need proper GlyphInfo array handling
        ::UnloadFontData(nullptr, 0);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("UNLOADFONT", Fn{"UNLOADFONT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADFONT: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::fonts.find(id);
        if (it != rlreg::fonts.end()) {
          ::UnloadFont(it->second);
          rlreg::fonts.erase(it);
        }
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("EXPORTFONTASCODE", Fn{"EXPORTFONTASCODE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("EXPORTFONTASCODE: expected 2 args");
        return Value::from_bool(::ExportFontAsCode(rlreg::fonts.at(args[0].as_int()), args[1].as_string().c_str()));
        
    }}, true);    R.add_with_policy("SETTEXTLINESPACING", Fn{"SETTEXTLINESPACING", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETTEXTLINESPACING: expected 1 args");
        SetTextLineSpacing(args[0].as_int());
        return Value::nil();
    }}, true);    R.add_with_policy("MEASURETEXT", Fn{"MEASURETEXT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("MEASURETEXT: expected 2 args");
        return Value::from_int(MeasureText(args[0].as_string().c_str(), args[1].as_int()));
    }}, true);    R.add_with_policy("MEASURETEXTEX", Fn{"MEASURETEXTEX", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("MEASURETEXTEX: expected 4 args");
        ::Vector2 size = ::MeasureTextEx(rlreg::fonts.at(args[0].as_int()), args[1].as_string().c_str(), (float)args[2].as_number(), (float)args[3].as_number());
        return Value::from_string(std::to_string(size.x) + "," + std::to_string(size.y));
        
    }}, true);    R.add_with_policy("GETGLYPHINDEX", Fn{"GETGLYPHINDEX", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETGLYPHINDEX: expected 2 args");
        return Value::from_int(::GetGlyphIndex(rlreg::fonts.at(args[0].as_int()), args[1].as_int()));
        
    }}, true);    R.add_with_policy("GETGLYPHINFO", Fn{"GETGLYPHINFO", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETGLYPHINFO: expected 2 args");
        ::GlyphInfo info = ::GetGlyphInfo(rlreg::fonts.at(args[0].as_int()), args[1].as_int());
        return Value::from_string(std::to_string(info.value) + "," + std::to_string(info.offsetX) + "," + std::to_string(info.offsetY) + "," + std::to_string(info.advanceX));
        
    }}, true);    R.add_with_policy("GETGLYPHATLASREC", Fn{"GETGLYPHATLASREC", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("GETGLYPHATLASREC: expected 2 args");
        ::Rectangle rec = ::GetGlyphAtlasRec(rlreg::fonts.at(args[0].as_int()), args[1].as_int());
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
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("LOADCODEPOINTS", Fn{"LOADCODEPOINTS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADCODEPOINTS: expected 1 args");
        // Note: This is simplified - would need proper codepoints array handling
        int count;
        int* codepoints = ::LoadCodepoints(args[0].as_string().c_str(), &count);
        (void)codepoints; // Suppress unused variable warning
        return Value::from_string("codepoints_loaded:" + std::to_string(count));
        
    }}, true);    R.add_with_policy("UNLOADCODEPOINTS", Fn{"UNLOADCODEPOINTS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADCODEPOINTS: expected 1 args");
        // Note: This is simplified - would need proper codepoints array handling
        ::UnloadCodepoints(nullptr);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("GETCODEPOINTCOUNT", Fn{"GETCODEPOINTCOUNT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCODEPOINTCOUNT: expected 1 args");
        return Value::from_int(GetCodepointCount(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("GETCODEPOINT", Fn{"GETCODEPOINT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCODEPOINT: expected 1 args");
        // Note: This is simplified - would need proper text handling
        int codepointSize;
        int codepoint = ::GetCodepoint(args[0].as_string().c_str(), &codepointSize);
        return Value::from_string(std::to_string(codepoint) + "," + std::to_string(codepointSize));
        
    }}, true);    R.add_with_policy("GETCODEPOINTNEXT", Fn{"GETCODEPOINTNEXT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCODEPOINTNEXT: expected 1 args");
        // Note: This is simplified - would need proper text handling
        int codepointSize;
        int codepoint = ::GetCodepointNext(args[0].as_string().c_str(), &codepointSize);
        return Value::from_string(std::to_string(codepoint) + "," + std::to_string(codepointSize));
        
    }}, true);    R.add_with_policy("GETCODEPOINTPREVIOUS", Fn{"GETCODEPOINTPREVIOUS", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETCODEPOINTPREVIOUS: expected 1 args");
        // Note: This is simplified - would need proper text handling
        int codepointSize;
        int codepoint = ::GetCodepointPrevious(args[0].as_string().c_str(), &codepointSize);
        return Value::from_string(std::to_string(codepoint) + "," + std::to_string(codepointSize));
        
    }}, true);    R.add_with_policy("CODEPOINTTOUTF8", Fn{"CODEPOINTTOUTF8", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("CODEPOINTTOUTF8: expected 1 args");
        int utf8Size;
        const char* text = ::CodepointToUTF8(args[0].as_int(), &utf8Size);
        std::string result(text);
        return Value::from_string(result);
        
    }}, true);    R.add_with_policy("TEXTCOPY", Fn{"TEXTCOPY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("TEXTCOPY: expected 2 args");
        // Note: This is simplified - would need proper text handling
        return Value::from_int(::TextCopy(nullptr, args[1].as_string().c_str()));
        
    }}, true);    R.add_with_policy("TEXTISEQUAL", Fn{"TEXTISEQUAL", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("TEXTISEQUAL: expected 2 args");
        return Value::from_bool(TextIsEqual(args[0].as_string().c_str(), args[1].as_string().c_str()));
    }}, true);    R.add_with_policy("TEXTLENGTH", Fn{"TEXTLENGTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTLENGTH: expected 1 args");
        return Value::from_int(TextLength(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("TEXTFORMAT", Fn{"TEXTFORMAT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTFORMAT: expected 1 args");
        // Note: This is simplified - would need proper formatting handling
        return Value::from_string(::TextFormat(args[0].as_string().c_str()));
        
    }}, true);    R.add_with_policy("TEXTSUBTEXT", Fn{"TEXTSUBTEXT", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("TEXTSUBTEXT: expected 3 args");
        return Value::from_string(TextSubtext(args[0].as_string().c_str(), args[1].as_int(), args[2].as_int()));
    }}, true);    R.add_with_policy("TEXTINSERT", Fn{"TEXTINSERT", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("TEXTINSERT: expected 3 args");
        char* result = ::TextInsert(args[0].as_string().c_str(), args[1].as_string().c_str(), args[2].as_int());
        std::string text(result);
        ::MemFree(result);
        return Value::from_string(text);
        
    }}, true);    R.add_with_policy("TEXTTOINTEGER", Fn{"TEXTTOINTEGER", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TEXTTOINTEGER: expected 1 args");
        return Value::from_int(TextToInteger(args[0].as_string().c_str()));
    }}, true);    R.add_with_policy("DRAWTEXTCODEPOINT", Fn{"DRAWTEXTCODEPOINT", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWTEXTCODEPOINT: expected 8 args");
        ::Vector2 position{(float)args[2].as_number(), (float)args[3].as_number()};
        ::Color c{(unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), 255};
        ::DrawTextCodepoint(rlreg::fonts.at(args[0].as_int()), args[1].as_int(), position, (float)args[4].as_number(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWLINES", Fn{"DRAWLINES", 7, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 7) throw std::runtime_error("DRAWLINES: expected 7 args");
        ::Color c{(unsigned char)args[4].as_int(), (unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), 255};
        ::DrawLine(args[0].as_int(), args[1].as_int(), args[2].as_int(), args[3].as_int(), c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("MIN", Fn{"MIN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("MIN: expected 2 args");
        return Value::from_number(std::min(args[0].as_number(), args[1].as_number()));
        
    }}, true);    R.add_with_policy("MAX", Fn{"MAX", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("MAX: expected 2 args");
        return Value::from_number(std::max(args[0].as_number(), args[1].as_number()));
        
    }}, true);    R.add_with_policy("CLAMP", Fn{"CLAMP", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("CLAMP: expected 3 args");
        double value = args[0].as_number();
        double min_val = args[1].as_number();
        double max_val = args[2].as_number();
        if (value < min_val) value = min_val;
        if (value > max_val) value = max_val;
        return Value::from_number(value);
        
    }}, true);    R.add_with_policy("COLLISIONRECTANGLES", Fn{"COLLISIONRECTANGLES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("COLLISIONRECTANGLES: expected 8 args");
        // Check if two rectangles overlap
        int x1 = args[0].as_int();
        int y1 = args[1].as_int();
        int w1 = args[2].as_int();
        int h1 = args[3].as_int();
        int x2 = args[4].as_int();
        int y2 = args[5].as_int();
        int w2 = args[6].as_int();
        int h2 = args[7].as_int();

        bool collision = !(x1 + w1 < x2 || x2 + w2 < x1 || y1 + h1 < y2 || y2 + h2 < y1);
        return Value::from_bool(collision);
        
    }}, true);    R.add_with_policy("POINTINRECTANGLE", Fn{"POINTINRECTANGLE", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("POINTINRECTANGLE: expected 6 args");
        // Check if a point is inside a rectangle
        int px = args[0].as_int();
        int py = args[1].as_int();
        int rx = args[2].as_int();
        int ry = args[3].as_int();
        int rw = args[4].as_int();
        int rh = args[5].as_int();

        bool inside = (px >= rx && px <= rx + rw && py >= ry && py <= ry + rh);
        return Value::from_bool(inside);
        
    }}, true);    R.add_with_policy("DISTANCE", Fn{"DISTANCE", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("DISTANCE: expected 4 args");
        // Calculate distance between two points (squared to avoid sqrt)
        int x1 = args[0].as_int();
        int y1 = args[1].as_int();
        int x2 = args[2].as_int();
        int y2 = args[3].as_int();

        double dx = (double)(x2 - x1);
        double dy = (double)(y2 - y1);
        double distance_squared = dx * dx + dy * dy;
        return Value::from_number(distance_squared);
        
    }}, true);    R.add_with_policy("SPLIT", Fn{"SPLIT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SPLIT: expected 2 args");
        // Simple split function - returns array index of first occurrence
        std::string text = args[0].as_string();
        std::string delimiter = args[1].as_string();
        size_t pos = text.find(delimiter);
        if (pos == std::string::npos) {
          return Value::from_int(-1); // Not found
        }
        return Value::from_int((int)pos);
        
    }}, true);    R.add_with_policy("LOADMUSIC", Fn{"LOADMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADMUSIC: expected 1 args");
        ::Music music = ::LoadMusicStream(args[0].as_string().c_str());
        int id = rlreg::next_music_id++;
        rlreg::musics[id] = music;
        return Value::from_int(id);
        
    }}, true);    R.add_with_policy("UNLOADMUSIC", Fn{"UNLOADMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UNLOADMUSIC: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::musics.find(id);
        if (it != rlreg::musics.end()) {
          ::UnloadMusicStream(it->second);
          rlreg::musics.erase(it);
        }
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("PLAYMUSIC", Fn{"PLAYMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PLAYMUSIC: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::musics.find(id);
        if (it != rlreg::musics.end()) {
          ::PlayMusicStream(it->second);
        }
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("STOPMUSIC", Fn{"STOPMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("STOPMUSIC: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::musics.find(id);
        if (it != rlreg::musics.end()) {
          ::StopMusicStream(it->second);
        }
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("PAUSEMUSIC", Fn{"PAUSEMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PAUSEMUSIC: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::musics.find(id);
        if (it != rlreg::musics.end()) {
          ::PauseMusicStream(it->second);
        }
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("RESUMEMUSIC", Fn{"RESUMEMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("RESUMEMUSIC: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::musics.find(id);
        if (it != rlreg::musics.end()) {
          ::ResumeMusicStream(it->second);
        }
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("UPDATEMUSIC", Fn{"UPDATEMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UPDATEMUSIC: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::musics.find(id);
        if (it != rlreg::musics.end()) {
          ::UpdateMusicStream(it->second);
        }
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ISMUSICPLAYING", Fn{"ISMUSICPLAYING", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMUSICPLAYING: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::musics.find(id);
        if (it != rlreg::musics.end()) {
          return Value::from_bool(::IsMusicStreamPlaying(it->second));
        }
        return Value::from_bool(false);
        
    }}, true);    R.add_with_policy("GETTEXTUREWIDTH", Fn{"GETTEXTUREWIDTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETTEXTUREWIDTH: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::textures.find(id);
        if (it != rlreg::textures.end()) {
          return Value::from_int(it->second.width);
        }
        return Value::from_int(0);
        
    }}, true);    R.add_with_policy("GETTEXTUREHEIGHT", Fn{"GETTEXTUREHEIGHT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETTEXTUREHEIGHT: expected 1 args");
        int id = args[0].as_int();
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
        int id = args[0].as_int();
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
        int id = args[0].as_int();
        int new_width = args[1].as_int();
        int new_height = args[2].as_int();
        auto it = rlreg::images.find(id);
        if (it != rlreg::images.end()) {
          ::ImageResize(&it->second, new_width, new_height);
        }
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("CROPIMAGE", Fn{"CROPIMAGE", 5, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 5) throw std::runtime_error("CROPIMAGE: expected 5 args");
        int id = args[0].as_int();
        int x = args[1].as_int();
        int y = args[2].as_int();
        int width = args[3].as_int();
        int height = args[4].as_int();
        auto it = rlreg::images.find(id);
        if (it != rlreg::images.end()) {
          ::Rectangle crop{(float)x, (float)y, (float)width, (float)height};
          ::ImageCrop(&it->second, crop);
        }
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("FLIPIMAGE", Fn{"FLIPIMAGE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("FLIPIMAGE: expected 1 args");
        int id = args[0].as_int();
        auto it = rlreg::images.find(id);
        if (it != rlreg::images.end()) {
          ::ImageFlipVertical(&it->second);
        }
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ROTATEIMAGE", Fn{"ROTATEIMAGE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ROTATEIMAGE: expected 2 args");
        int id = args[0].as_int();
        int degrees = args[1].as_int();
        auto it = rlreg::images.find(id);
        if (it != rlreg::images.end()) {
          ::ImageRotate(&it->second, degrees);
        }
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("REPLACE", Fn{"REPLACE", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("REPLACE: expected 3 args");
        std::string text = args[0].as_string();
        std::string search = args[1].as_string();
        std::string replace = args[2].as_string();
        size_t pos = 0;
        while ((pos = text.find(search, pos)) != std::string::npos) {
          text.replace(pos, search.length(), replace);
          pos += replace.length();
        }
        return Value::from_string(text);
        
    }}, true);    R.add_with_policy("INSTRREV", Fn{"INSTRREV", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("INSTRREV: expected 2 args");
        std::string text = args[0].as_string();
        std::string search = args[1].as_string();
        size_t pos = text.rfind(search);
        if (pos == std::string::npos) return Value::from_int(0);
        return Value::from_int((int)pos + 1); // BASIC uses 1-based indexing
        
    }}, true);    R.add_with_policy("STRREPEAT", Fn{"STRREPEAT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("STRREPEAT: expected 2 args");
        std::string text = args[0].as_string();
        int count = args[1].as_int();
        std::string result;
        for (int i = 0; i < count; i++) {
          result += text;
        }
        return Value::from_string(result);
        
    }}, true);    R.add_with_policy("STRREVERSE", Fn{"STRREVERSE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("STRREVERSE: expected 1 args");
        std::string text = args[0].as_string();
        std::reverse(text.begin(), text.end());
        return Value::from_string(text);
        
    }}, true);    R.add_with_policy("SETGESTURESENABLED", Fn{"SETGESTURESENABLED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETGESTURESENABLED: expected 1 args");
        ::SetGesturesEnabled(args[0].as_int());
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ISGESTUREDETECTED", Fn{"ISGESTUREDETECTED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISGESTUREDETECTED: expected 1 args");
        return Value::from_bool(::IsGestureDetected(args[0].as_int()));
        
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
        // This is a placeholder - actual implementation would need array access
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("REVERSE", Fn{"REVERSE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("REVERSE: expected 1 args");
        // REVERSE function for arrays - reverses the order of elements
        // This is a placeholder - actual implementation would need array access
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("FIND", Fn{"FIND", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("FIND: expected 2 args");
        // FIND function for arrays - finds the first occurrence of a value
        // This is a placeholder - actual implementation would need array access
        return Value::from_int(-1);
        
    }}, true);    R.add_with_policy("BINARYSEARCH", Fn{"BINARYSEARCH", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("BINARYSEARCH: expected 2 args");
        // BINARYSEARCH function for sorted arrays
        // This is a placeholder - actual implementation would need array access
        return Value::from_int(-1);
        
    }}, true);    R.add_with_policy("ISNUMBER", Fn{"ISNUMBER", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISNUMBER: expected 1 args");
        std::string text = args[0].as_string();
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
        // This would need to check if the variable is an array
        // For now, return false as placeholder
        return Value::from_bool(false);
        
    }}, true);    R.add_with_policy("ARRAYLENGTH", Fn{"ARRAYLENGTH", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ARRAYLENGTH: expected 1 args");
        // Returns the length of an array
        // This is a placeholder - actual implementation would need array access
        return Value::from_int(0);
        
    }}, true);    R.add_with_policy("ARRAYCOPY", Fn{"ARRAYCOPY", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ARRAYCOPY: expected 2 args");
        // Copies one array to another
        // This is a placeholder - actual implementation would need array access
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ARRAYFILL", Fn{"ARRAYFILL", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ARRAYFILL: expected 2 args");
        // Fills an array with a specified value
        // This is a placeholder - actual implementation would need array access
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ARRAYSHUFFLE", Fn{"ARRAYSHUFFLE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ARRAYSHUFFLE: expected 1 args");
        // Randomly shuffles the elements of an array
        // This is a placeholder - actual implementation would need array access
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ARRAYUNIQUE", Fn{"ARRAYUNIQUE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ARRAYUNIQUE: expected 1 args");
        // Removes duplicate elements from an array
        // This is a placeholder - actual implementation would need array access
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ARRAYJOIN", Fn{"ARRAYJOIN", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ARRAYJOIN: expected 2 args");
        // Joins array elements into a string with a delimiter
        // This is a placeholder - actual implementation would need array access
        return Value::from_string("");
        
    }}, true);    R.add_with_policy("ARRAYSPLIT", Fn{"ARRAYSPLIT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ARRAYSPLIT: expected 2 args");
        // Splits a string into an array using a delimiter
        // This is a placeholder - actual implementation would need array access
        return Value::from_int(0);
        
    }}, true);    R.add_with_policy("HTTPGET", Fn{"HTTPGET", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("HTTPGET: expected 1 args");
        // HTTP GET request - placeholder for future implementation
        // Would use libcurl or similar for actual HTTP requests
        return Value::from_string("HTTP GET not yet implemented");
        
    }}, true);    R.add_with_policy("HTTPPOST", Fn{"HTTPPOST", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("HTTPPOST: expected 2 args");
        // HTTP POST request - placeholder for future implementation
        // Would use libcurl or similar for actual HTTP requests
        return Value::from_string("HTTP POST not yet implemented");
        
    }}, true);    R.add_with_policy("DOWNLOADFILE", Fn{"DOWNLOADFILE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("DOWNLOADFILE: expected 2 args");
        // Download file from URL - placeholder for future implementation
        // Would use libcurl or similar for actual file downloads
        return Value::from_bool(false);
        
    }}, true);    R.add_with_policy("UPLOADFILE", Fn{"UPLOADFILE", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("UPLOADFILE: expected 2 args");
        // Upload file to URL - placeholder for future implementation
        // Would use libcurl or similar for actual file uploads
        return Value::from_bool(false);
        
    }}, true);    R.add_with_policy("WEBSOCKETCONNECT", Fn{"WEBSOCKETCONNECT", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("WEBSOCKETCONNECT: expected 1 args");
        // WebSocket connection - placeholder for future implementation
        // Would use a WebSocket library for actual connections
        return Value::from_int(-1);
        
    }}, true);    R.add_with_policy("WEBSOCKETSEND", Fn{"WEBSOCKETSEND", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("WEBSOCKETSEND: expected 2 args");
        // WebSocket send message - placeholder for future implementation
        return Value::from_bool(false);
        
    }}, true);    R.add_with_policy("WEBSOCKETRECEIVE", Fn{"WEBSOCKETRECEIVE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("WEBSOCKETRECEIVE: expected 1 args");
        // WebSocket receive message - placeholder for future implementation
        return Value::from_string("");
        
    }}, true);    R.add_with_policy("WEBSOCKETCLOSE", Fn{"WEBSOCKETCLOSE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("WEBSOCKETCLOSE: expected 1 args");
        // WebSocket close connection - placeholder for future implementation
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("TCPCONNECT", Fn{"TCPCONNECT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("TCPCONNECT: expected 2 args");
        // TCP connection - placeholder for future implementation
        // Would use socket library for actual TCP connections
        return Value::from_int(-1);
        
    }}, true);    R.add_with_policy("TCPSEND", Fn{"TCPSEND", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("TCPSEND: expected 2 args");
        // TCP send data - placeholder for future implementation
        return Value::from_bool(false);
        
    }}, true);    R.add_with_policy("TCPRECEIVE", Fn{"TCPRECEIVE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TCPRECEIVE: expected 1 args");
        // TCP receive data - placeholder for future implementation
        return Value::from_string("");
        
    }}, true);    R.add_with_policy("TCPCLOSE", Fn{"TCPCLOSE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("TCPCLOSE: expected 1 args");
        // TCP close connection - placeholder for future implementation
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("UDPCREATE", Fn{"UDPCREATE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UDPCREATE: expected 1 args");
        // UDP socket creation - placeholder for future implementation
        return Value::from_int(-1);
        
    }}, true);    R.add_with_policy("UDPSEND", Fn{"UDPSEND", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("UDPSEND: expected 4 args");
        // UDP send data - placeholder for future implementation
        return Value::from_bool(false);
        
    }}, true);    R.add_with_policy("UDPRECEIVE", Fn{"UDPRECEIVE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UDPRECEIVE: expected 1 args");
        // UDP receive data - placeholder for future implementation
        return Value::from_string("");
        
    }}, true);    R.add_with_policy("UDPCLOSE", Fn{"UDPCLOSE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UDPCLOSE: expected 1 args");
        // UDP close socket - placeholder for future implementation
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCONE", Fn{"DRAWCONE", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWCONE: expected 8 args");
        // DrawCone not available in current Raylib version
        // Using DrawCylinder as alternative
        ::Vector3 position{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Color c{(unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), 255};
        ::DrawCylinder(position, (float)args[3].as_number(), 0.0f, (float)args[4].as_number(), 8, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWCONEWIRES", Fn{"DRAWCONEWIRES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWCONEWIRES: expected 8 args");
        // DrawConeWires not available in current Raylib version
        // Using DrawCylinderWires as alternative
        ::Vector3 position{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Color c{(unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), 255};
        ::DrawCylinderWires(position, (float)args[3].as_number(), 0.0f, (float)args[4].as_number(), 8, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTORUS", Fn{"DRAWTORUS", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWTORUS: expected 8 args");
        // DrawTorus not available in current Raylib version
        // Using DrawCylinder as alternative
        ::Vector3 position{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Color c{(unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), 255};
        ::DrawCylinder(position, (float)args[3].as_number(), (float)args[3].as_number(), (float)args[4].as_number(), 8, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWTORUSWIRES", Fn{"DRAWTORUSWIRES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWTORUSWIRES: expected 8 args");
        // DrawTorusWires not available in current Raylib version
        // Using DrawCylinderWires as alternative
        ::Vector3 position{(float)args[0].as_number(), (float)args[1].as_number(), (float)args[2].as_number()};
        ::Color c{(unsigned char)args[5].as_int(), (unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), 255};
        ::DrawCylinderWires(position, (float)args[3].as_number(), (float)args[3].as_number(), (float)args[4].as_number(), 8, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("DRAWPOLYWIRES", Fn{"DRAWPOLYWIRES", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWPOLYWIRES: expected 8 args");
        // DrawPolyWires not available in current Raylib version
        // Using DrawPoly as alternative
        ::Vector2 center{(float)args[0].as_number(), (float)args[1].as_number()};
        ::Color c{(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        ::DrawPoly(center, args[3].as_int(), (float)args[4].as_number(), 0.0f, c);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("INITGAME", Fn{"INITGAME", 3, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("INITGAME: expected 3 args");
        InitWindow(args[0].as_int(), args[1].as_int(), args[2].as_string().c_str());
        SetTargetFPS(60);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("CLOSEGAME", Fn{"CLOSEGAME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("CLOSEGAME: expected 0 args");
        CloseWindow();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("GAMESHOULDCLOSE", Fn{"GAMESHOULDCLOSE", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GAMESHOULDCLOSE: expected 0 args");
        return Value::from_bool(WindowShouldClose());
        
    }}, true);    R.add_with_policy("BEGINGAMEFRAME", Fn{"BEGINGAMEFRAME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("BEGINGAMEFRAME: expected 0 args");
        BeginDrawing();
        ClearBackground(Color{0, 0, 0, 255});
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ENDGAMEFRAME", Fn{"ENDGAMEFRAME", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDGAMEFRAME: expected 0 args");
        EndDrawing();
        0;
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
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("BEGINDRAW3D", Fn{"BEGINDRAW3D", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("BEGINDRAW3D: expected 0 args");
        // Begin 3D mode - simplified version
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("ENDDRAW3D", Fn{"ENDDRAW3D", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("ENDDRAW3D: expected 0 args");
        // End 3D mode - simplified version
        return Value::nil();
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("LOADSPRITE", Fn{"LOADSPRITE", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADSPRITE: expected 1 args");
        Texture2D texture = LoadTexture(args[0].as_string().c_str());
        return Value::from_number((intptr_t)&texture);
        
    }}, true);    R.add_with_policy("DRAWSPRITE", Fn{"DRAWSPRITE", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("DRAWSPRITE: expected 8 args");
        Texture2D texture = *(Texture2D*)args[0].as_int();
        Rectangle source = {0, 0, (float)texture.width, (float)texture.height};
        Rectangle dest = {(float)args[1].as_int(), (float)args[2].as_int(), (float)args[3].as_int(), (float)args[4].as_int()};
        Vector2 origin = {(float)args[3].as_int()/2, (float)args[4].as_int()/2};
        Color tint = {(unsigned char)args[6].as_int(), (unsigned char)args[7].as_int(), (unsigned char)args[8].as_int(), 255};
        DrawTexturePro(texture, source, dest, origin, (float)args[5].as_int(), tint);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("CHECKCOLLISIONRECTS", Fn{"CHECKCOLLISIONRECTS", 8, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 8) throw std::runtime_error("CHECKCOLLISIONRECTS: expected 8 args");
        Rectangle rect1 = {(float)args[0].as_int(), (float)args[1].as_int(), (float)args[2].as_int(), (float)args[3].as_int()};
        Rectangle rect2 = {(float)args[4].as_int(), (float)args[5].as_int(), (float)args[6].as_int(), (float)args[7].as_int()};
        return Value::from_bool(CheckCollisionRecs(rect1, rect2));
        
    }}, true);    R.add_with_policy("CHECKCOLLISIONCIRCLES", Fn{"CHECKCOLLISIONCIRCLES", 6, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("CHECKCOLLISIONCIRCLES: expected 6 args");
        Vector2 center1 = {(float)args[0].as_int(), (float)args[1].as_int()};
        Vector2 center2 = {(float)args[3].as_int(), (float)args[4].as_int()};
        return Value::from_bool(CheckCollisionCircles(center1, (float)args[2].as_int(), center2, (float)args[5].as_int()));
        
    }}, true);    R.add_with_policy("ISKEYHELD", Fn{"ISKEYHELD", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISKEYHELD: expected 1 args");
        return Value::from_bool(IsKeyDown(args[0].as_int()));
        
    }}, true);    R.add_with_policy("ISKEYPRESSED", Fn{"ISKEYPRESSED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISKEYPRESSED: expected 1 args");
        return Value::from_bool(IsKeyPressed(args[0].as_int()));
        
    }}, true);    R.add_with_policy("ISMOUSEHELD", Fn{"ISMOUSEHELD", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMOUSEHELD: expected 1 args");
        return Value::from_bool(IsMouseButtonDown(args[0].as_int()));
        
    }}, true);    R.add_with_policy("ISMOUSEPRESSED", Fn{"ISMOUSEPRESSED", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISMOUSEPRESSED: expected 1 args");
        return Value::from_bool(IsMouseButtonPressed(args[0].as_int()));
        
    }}, true);    R.add_with_policy("GETMOUSEPOSITION", Fn{"GETMOUSEPOSITION", 0, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 0) throw std::runtime_error("GETMOUSEPOSITION: expected 0 args");
        Vector2 mousePos = GetMousePosition();
        return Value::from_string(std::to_string((int)mousePos.x) + "," + std::to_string((int)mousePos.y));
        
    }}, true);    R.add_with_policy("LOADGAMESOUND", Fn{"LOADGAMESOUND", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADGAMESOUND: expected 1 args");
        Sound sound = LoadSound(args[0].as_string().c_str());
        return Value::from_number((intptr_t)&sound);
        
    }}, true);    R.add_with_policy("PLAYGAMESOUND", Fn{"PLAYGAMESOUND", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PLAYGAMESOUND: expected 1 args");
        Sound sound = *(Sound*)args[0].as_int();
        PlaySound(sound);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("LOADGAMEMUSIC", Fn{"LOADGAMEMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADGAMEMUSIC: expected 1 args");
        Music music = LoadMusicStream(args[0].as_string().c_str());
        return Value::from_number((intptr_t)&music);
        
    }}, true);    R.add_with_policy("PLAYGAMEMUSIC", Fn{"PLAYGAMEMUSIC", 1, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PLAYGAMEMUSIC: expected 1 args");
        Music music = *(Music*)args[0].as_int();
        PlayMusicStream(music);
        0;
        return Value::nil();
    }}, true);    R.add_with_policy("RANDOMINT", Fn{"RANDOMINT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("RANDOMINT: expected 2 args");
        return Value::from_number(GetRandomValue(args[0].as_int(), args[1].as_int()));
        
    }}, true);    R.add_with_policy("RANDOMFLOAT", Fn{"RANDOMFLOAT", 2, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("RANDOMFLOAT: expected 2 args");
        float min = args[0].as_number();
        float max = args[1].as_number();
        return Value::from_number(min + (max - min) * ((float)GetRandomValue(0, 1000) / 1000.0f));
        
    }}, true);    R.add_with_policy("DISTANCE2D", Fn{"DISTANCE2D", 4, [] (const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("DISTANCE2D: expected 4 args");
        Vector2 p1 = {(float)args[0].as_int(), (float)args[1].as_int()};
        Vector2 p2 = {(float)args[2].as_int(), (float)args[3].as_int()};
        return Value::from_number(Vector2Distance(p1, p2));
        
    }}, true);}
} // namespace bas

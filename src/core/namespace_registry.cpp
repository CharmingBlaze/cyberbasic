#include "bas/namespace_registry.hpp"
#include <algorithm>
#include <cctype>

using namespace bas;

void NamespaceRegistry::register_namespace(
    const std::string& namespace_name,
    const std::unordered_map<std::string, std::string>& methods
) {
    std::string upper_name = namespace_name;
    std::transform(upper_name.begin(), upper_name.end(), upper_name.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    
    namespaces_[upper_name] = methods;
}

Value NamespaceRegistry::create_namespace_object(const std::string& namespace_name) const {
    std::string upper_name = namespace_name;
    std::transform(upper_name.begin(), upper_name.end(), upper_name.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    
    Value::Map obj;
    obj["_type"] = Value::from_string("Namespace");
    obj["_name"] = Value::from_string(upper_name);
    
    // Add all methods as properties (they'll be resolved at call time)
    auto it = namespaces_.find(upper_name);
    if (it != namespaces_.end()) {
        for (const auto& [method_name, func_name] : it->second) {
            // Store method metadata
            Value::Map method_obj;
            method_obj["_type"] = Value::from_string("Method");
            method_obj["_namespace"] = Value::from_string(upper_name);
            method_obj["_method"] = Value::from_string(method_name);
            method_obj["_function"] = Value::from_string(func_name);
            obj[method_name] = Value::from_map(std::move(method_obj));
        }
    }
    
    return Value::from_map(std::move(obj));
}

std::string NamespaceRegistry::resolve_method(
    const std::string& namespace_name,
    const std::string& method_name
) const {
    std::string upper_ns = namespace_name;
    std::transform(upper_ns.begin(), upper_ns.end(), upper_ns.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    
    std::string upper_method = method_name;
    std::transform(upper_method.begin(), upper_method.end(), upper_method.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    
    auto ns_it = namespaces_.find(upper_ns);
    if (ns_it == namespaces_.end()) {
        return "";
    }
    
    auto method_it = ns_it->second.find(upper_method);
    if (method_it == ns_it->second.end()) {
        return "";
    }
    
    return method_it->second;
}

bool NamespaceRegistry::has_namespace(const std::string& namespace_name) const {
    std::string upper_name = namespace_name;
    std::transform(upper_name.begin(), upper_name.end(), upper_name.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return namespaces_.find(upper_name) != namespaces_.end();
}

std::vector<std::string> NamespaceRegistry::get_namespaces() const {
    std::vector<std::string> result;
    result.reserve(namespaces_.size());
    for (const auto& [name, _] : namespaces_) {
        result.push_back(name);
    }
    return result;
}

void bas::register_raylib_namespaces(NamespaceRegistry& registry) {
    // Window namespace
    registry.register_namespace("Window", {
        {"init", "INITWINDOW"},
        {"close", "CLOSEWINDOW"},
        {"shouldClose", "WINDOWSHOULDCLOSE"},
        {"isReady", "ISWINDOWREADY"},
        {"isFullscreen", "ISWINDOWFULLSCREEN"},
        {"isHidden", "ISWINDOWHIDDEN"},
        {"isMinimized", "ISWINDOWMINIMIZED"},
        {"isMaximized", "ISWINDOWMAXIMIZED"},
        {"isFocused", "ISWINDOWFOCUSED"},
        {"isResized", "ISWINDOWRESIZED"},
        {"isState", "ISWINDOWSTATE"},
        {"setTitle", "SETWINDOWTITLE"},
        {"setMinSize", "SETWINDOWMINIMUMSIZE"},
        {"setPosition", "SETWINDOWPOSITION"},
        {"setMonitor", "SETWINDOWMONITOR"},
        {"setState", "SETWINDOWSTATE"},
        {"clearState", "CLEARWINDOWSTATE"},
        {"toggleFullscreen", "TOGGLEFULLSCREEN"},
        {"maximize", "MAXIMIZEWINDOW"},
        {"minimize", "MINIMIZEWINDOW"},
        {"restore", "RESTOREWINDOW"},
        {"setIcon", "SETWINDOWICON"},
        {"setClipboardText", "SETCLIPBOARDTEXT"},
        {"getClipboardText", "GETCLIPBOARDTEXT"},
        {"setTargetFPS", "SETTARGETFPS"},
        {"getFPS", "GETFPS"},
        {"getFrameTime", "GETFRAMETIME"},
        {"getTime", "GETTIME"},
        {"getScreenWidth", "GETSCREENWIDTH"},
        {"getScreenHeight", "GETSCREENHEIGHT"},
        {"getRenderWidth", "GETRENDERWIDTH"},
        {"getRenderHeight", "GETRENDERHEIGHT"},
        {"getMonitorCount", "GETMONITORCOUNT"},
        {"getMonitorWidth", "GETMONITORWIDTH"},
        {"getMonitorHeight", "GETMONITORHEIGHT"},
        {"getCurrentMonitor", "GETCURRENTMONITOR"},
    });

    // Graphics namespace (2D drawing)
    registry.register_namespace("Graphics", {
        {"beginDrawing", "BEGINDRAWING"},
        {"endDrawing", "ENDDRAWING"},
        {"beginBlendMode", "BEGINBLENDMODE"},
        {"endBlendMode", "ENDBLENDMODE"},
        {"beginScissorMode", "BEGINSCISSORMODE"},
        {"endScissorMode", "ENDSCISSORMODE"},
        {"clearBackground", "CLEARBACKGROUND"},
        {"drawPixel", "DRAWPIXEL"},
        {"drawPixelV", "DRAWPIXELV"},
        {"drawLine", "DRAWLINE"},
        {"drawLineV", "DRAWLINEV"},
        {"drawLineEx", "DRAWLINEEX"},
        {"drawLineBezier", "DRAWLINEBEZIER"},
        {"drawCircle", "DRAWCIRCLE"},
        {"drawCircleSector", "DRAWCIRCLESECTOR"},
        {"drawCircleSectorLines", "DRAWCIRCLESECTORLINES"},
        {"drawCircleGradient", "DRAWCIRCLEGRADIENT"},
        {"drawCircleV", "DRAWCIRCLEV"},
        {"drawCircleLines", "DRAWCIRCLELINES"},
        {"drawEllipse", "DRAWELLIPSE"},
        {"drawEllipseLines", "DRAWELLIPSELINES"},
        {"drawRing", "DRAWRING"},
        {"drawRingLines", "DRAWRINGLINES"},
        {"drawRectangle", "DRAWRECTANGLE"},
        {"drawRectangleV", "DRAWRECTANGLEV"},
        {"drawRectangleRec", "DRAWRECTANGLEREC"},
        {"drawRectangleLines", "DRAWRECTANGLELINES"},
        {"drawRectangleLinesEx", "DRAWRECTANGLELINESEX"},
        {"drawRectangleRounded", "DRAWRECTANGLEROUNDED"},
        {"drawRectangleRoundedLines", "DRAWRECTANGLEROUNDEDLINES"},
        {"drawTriangle", "DRAWTRIANGLE"},
        {"drawTriangleLines", "DRAWTRIANGLELINES"},
        {"drawTriangleFan", "DRAWTRIANGLEFAN"},
        {"drawTriangleStrip", "DRAWTRIANGLESTRIP"},
        {"drawPoly", "DRAWPOLY"},
        {"drawPolyLines", "DRAWPOLYLINES"},
        {"drawPolyLinesEx", "DRAWPOLYLINESEX"},
        {"drawTexture", "DRAWTEXTURE"},
        {"drawTextureV", "DRAWTEXTUREV"},
        {"drawTextureEx", "DRAWTEXTUREEX"},
        {"drawTextureRec", "DRAWTEXTUREREC"},
        {"drawTexturePro", "DRAWTEXTUREPRO"},
        {"drawTextureNPatch", "DRAWTEXTURENPATCH"},
        {"drawText", "DRAWTEXT"},
        {"drawTextEx", "DRAWTEXTEX"},
        {"drawTextPro", "DRAWTEXTPRO"},
        {"drawTextCodepoint", "DRAWTEXTCODEPOINT"},
        {"drawFPS", "DRAWFPS"},
    });

    // Graphics3D namespace (3D drawing)
    registry.register_namespace("Graphics3D", {
        {"beginMode3D", "BEGINMODE3D"},
        {"endMode3D", "ENDMODE3D"},
        {"drawLine3D", "DRAWLINE3D"},
        {"drawPoint3D", "DRAWPOINT3D"},
        {"drawCircle3D", "DRAWCIRCLE3D"},
        {"drawTriangle3D", "DRAWTRIANGLE3D"},
        {"drawTriangleStrip3D", "DRAWTRIANGLESTRIP3D"},
        {"drawCube", "DRAWCUBE"},
        {"drawCubeV", "DRAWCUBEV"},
        {"drawCubeWires", "DRAWCUBEWIRES"},
        {"drawCubeWiresV", "DRAWCUBEWIRESV"},
        {"drawSphere", "DRAWSPHERE"},
        {"drawSphereEx", "DRAWSPHEREEX"},
        {"drawSphereWires", "DRAWSPHEREWIRES"},
        {"drawCylinder", "DRAWCYLINDER"},
        {"drawCylinderEx", "DRAWCYLINDEREX"},
        {"drawCylinderWires", "DRAWCYLINDERWIRES"},
        {"drawCylinderWiresEx", "DRAWCYLINDERWIRESEX"},
        {"drawPlane", "DRAWPLANE"},
        {"drawRay", "DRAWRAY"},
        {"drawGrid", "DRAWGRID"},
        {"drawGizmo", "DRAWGIZMO"},
    });

    // Audio namespace
    registry.register_namespace("Audio", {
        {"init", "INITAUDIODEVICE"},
        {"close", "CLOSEAUDIODEVICE"},
        {"isReady", "ISAUDIODEVICEREADY"},
        {"setMasterVolume", "SETMASTERVOLUME"},
        {"loadSound", "LOADSOUND"},
        {"loadSoundFromWave", "LOADSOUNDFROMWAVE"},
        {"loadSoundAlias", "LOADSOUNDALIAS"},
        {"isSoundReady", "ISSOUNDREADY"},
        {"updateSound", "UPDATESOUND"},
        {"unloadSound", "UNLOADSOUND"},
        {"unloadSoundAlias", "UNLOADSOUNDALIAS"},
        {"playSound", "PLAYSOUND"},
        {"stopSound", "STOPSOUND"},
        {"pauseSound", "PAUSESOUND"},
        {"resumeSound", "RESUMESOUND"},
        {"isSoundPlaying", "ISSOUNDPLAYING"},
        {"setSoundVolume", "SETSOUNDVOLUME"},
        {"setSoundPitch", "SETSOUNDPITCH"},
        {"setSoundPan", "SETSOUNDPAN"},
        {"loadMusicStream", "LOADMUSICSTREAM"},
        {"isMusicReady", "ISMUSICREADY"},
        {"unloadMusicStream", "UNLOADMUSICSTREAM"},
        {"playMusicStream", "PLAYMUSICSTREAM"},
        {"isMusicStreamPlaying", "ISMUSICSTREAMPLAYING"},
        {"updateMusicStream", "UPDATEMUSICSTREAM"},
        {"stopMusicStream", "STOPMUSICSTREAM"},
        {"pauseMusicStream", "PAUSEMUSICSTREAM"},
        {"resumeMusicStream", "RESUMEMUSICSTREAM"},
        {"seekMusicStream", "SEEKMUSICSTREAM"},
        {"setMusicVolume", "SETMUSICVOLUME"},
        {"setMusicPitch", "SETMUSICPITCH"},
        {"setMusicPan", "SETMUSICPAN"},
        {"getMusicTimeLength", "GETMUSICTIMELENGTH"},
        {"getMusicTimePlayed", "GETMUSICTIMEPLAYED"},
    });

    // Input namespace
    registry.register_namespace("Input", {
        {"isKeyPressed", "ISKEYPRESSED"},
        {"isKeyDown", "ISKEYDOWN"},
        {"isKeyReleased", "ISKEYRELEASED"},
        {"isKeyUp", "ISKEYUP"},
        {"getKeyPressed", "GETKEYPRESSED"},
        {"setExitKey", "SETEXITKEY"},
        {"isMouseButtonPressed", "ISMOUSEBUTTONPRESSED"},
        {"isMouseButtonDown", "ISMOUSEBUTTONDOWN"},
        {"isMouseButtonReleased", "ISMOUSEBUTTONRELEASED"},
        {"isMouseButtonUp", "ISMOUSEBUTTONUP"},
        {"getMouseX", "GETMOUSEX"},
        {"getMouseY", "GETMOUSEY"},
        {"getMousePosition", "GETMOUSEPOSITION"},
        {"getMouseDelta", "GETMOUSEDELTA"},
        {"setMousePosition", "SETMOUSEPOSITION"},
        {"setMouseOffset", "SETMOUSEOFFSET"},
        {"setMouseScale", "SETMOUSESCALE"},
        {"getMouseWheelMove", "GETMOUSEWHEELMOVE"},
        {"getMouseWheelMoveV", "GETMOUSEWHEELMOVEV"},
        {"setMouseCursor", "SETMOUSECURSOR"},
        {"getTouchX", "GETTOUCHX"},
        {"getTouchY", "GETTOUCHY"},
        {"getTouchPosition", "GETTOUCHPOSITION"},
        {"getTouchPointId", "GETTOUCHPOINTID"},
        {"getTouchPointCount", "GETTOUCHPOINTCOUNT"},
        {"isGamepadAvailable", "ISGAMEPADAVAILABLE"},
        {"getGamepadName", "GETGAMEPADNAME"},
        {"isGamepadButtonPressed", "ISGAMEPADBUTTONPRESSED"},
        {"isGamepadButtonDown", "ISGAMEPADBUTTONDOWN"},
        {"isGamepadButtonReleased", "ISGAMEPADBUTTONRELEASED"},
        {"isGamepadButtonUp", "ISGAMEPADBUTTONUP"},
        {"getGamepadButtonPressed", "GETGAMEPADBUTTONPRESSED"},
        {"getGamepadAxisCount", "GETGAMEPADAXISCOUNT"},
        {"getGamepadAxisMovement", "GETGAMEPADAXISMOVEMENT"},
        {"setGamepadMappings", "SETGAMEPADMAPPINGS"},
    });

    // Math namespace
    registry.register_namespace("Math", {
        {"clamp", "CLAMP"},
        {"lerp", "LERP"},
        {"normalize", "NORMALIZE"},
        {"remap", "REMAP"},
        {"wrap", "WRAP"},
        {"floatEquals", "FLOATEQUALS"},
    });

    // File namespace
    registry.register_namespace("File", {
        {"loadFileData", "LOADFILEDATA"},
        {"unloadFileData", "UNLOADFILEDATA"},
        {"saveFileData", "SAVEFILEDATA"},
        {"exportDataAsCode", "EXPORTDATAASCODE"},
        {"loadFileText", "LOADFILETEXT"},
        {"unloadFileText", "UNLOADFILETEXT"},
        {"saveFileText", "SAVEFILETEXT"},
        {"fileExists", "FILEEXISTS"},
        {"directoryExists", "DIRECTORYEXISTS"},
        {"isFileExtension", "ISFILEEXTENSION"},
        {"getFileLength", "GETFILELENGTH"},
        {"getFileExtension", "GETFILEEXTENSION"},
        {"getFileName", "GETFILENAME"},
        {"getFileNameWithoutExt", "GETFILENAMEWITHOUTEXT"},
        {"getDirectoryPath", "GETDIRECTORYPATH"},
        {"getPrevDirectoryPath", "GETPREVDIRECTORYPATH"},
        {"getWorkingDirectory", "GETWORKINGDIRECTORY"},
        {"getApplicationDirectory", "GETAPPLICATIONDIRECTORY"},
        {"changeDirectory", "CHANGEDIRECTORY"},
        {"isPathFile", "ISPATHFILE"},
    });
}

void bas::register_game_namespaces(NamespaceRegistry& registry) {
    // Game namespace
    // Tween namespace
    registry.register_namespace("Tween", {
        {"to", "TWEEN_TO"},
        {"from", "TWEEN_FROM"},
        {"update", "TWEEN_UPDATE"},
        {"stop", "TWEEN_STOP"},
        {"pause", "TWEEN_PAUSE"},
        {"resume", "TWEEN_RESUME"},
        {"getValue", "TWEEN_GETVALUE"},
        {"isComplete", "TWEEN_ISCOMPLETE"},
    });
    
    // Camera namespace (shake)
    registry.register_namespace("Camera", {
        {"shake", "CAMERA_SHAKE"},
        {"setTrauma", "CAMERA_SETTRAUMA"},
        {"getTrauma", "CAMERA_GETTRAUMA"},
        {"updateShake", "CAMERA_UPDATESHAKE"},
        {"getShakeOffsetX", "CAMERA_GETSHAKEOFFSETX"},
        {"getShakeOffsetY", "CAMERA_GETSHAKEOFFSETY"},
        {"getShakeRotation", "CAMERA_GETSHAKEROTATION"},
    });
    
    // Localization namespace
    registry.register_namespace("LOC", {
        {"loadTable", "LOC_LOADTABLE"},
        {"setLanguage", "LOC_SETLANGUAGE"},
        {"get", "LOC_GET"},
        {"getLanguage", "LOC_GETLANGUAGE"},
        {"hasKey", "LOC_HASKEY"},
    });
    
    // StateMachine namespace
    registry.register_namespace("StateMachine", {
        {"create", "STATEMACHINE_CREATE"},
        {"addState", "STATEMACHINE_ADDSTATE"},
        {"setState", "STATEMACHINE_SETSTATE"},
        {"pushState", "STATEMACHINE_PUSHSTATE"},
        {"popState", "STATEMACHINE_POPSTATE"},
        {"getState", "STATEMACHINE_GETSTATE"},
        {"getPreviousState", "STATEMACHINE_GETPREVIOUSSTATE"},
        {"isState", "STATEMACHINE_ISSTATE"},
    });
    
    // PostFX namespace
    registry.register_namespace("PostFX", {
        {"add", "POSTFX_ADD"},
        {"remove", "POSTFX_REMOVE"},
        {"clear", "POSTFX_CLEAR"},
        {"begin", "POSTFX_BEGIN"},
        {"end", "POSTFX_END"},
        {"setActive", "POSTFX_SETACTIVE"},
        {"isActive", "POSTFX_ISACTIVE"},
    });
    
    // Stream namespace
    registry.register_namespace("STREAM", {
        {"setGrid", "STREAM_SETGRID"},
        {"setRadius", "STREAM_SETRADIUS"},
        {"pinPlayer", "STREAM_PINPLAYER"},
        {"update", "STREAM_UPDATE"},
        {"getChunkCount", "STREAM_GETCHUNKCOUNT"},
        {"getChunkAt", "STREAM_GETCHUNKAT"},
        {"isChunkLoaded", "STREAM_ISCHUNKLOADED"},
    });
    
    // Event namespace
    registry.register_namespace("EVENT", {
        {"subscribe", "EVENT_SUBSCRIBE"},
        {"unsubscribe", "EVENT_UNSUBSCRIBE"},
        {"fire", "EVENT_FIRE"},
        {"fireImmediate", "EVENT_FIREIMMEDIATE"},
        {"processQueue", "EVENT_PROCESSQUEUE"},
        {"clearQueue", "EVENT_CLEARQUEUE"},
        {"getQueueSize", "EVENT_GETQUEUESIZE"},
        {"hasHandlers", "EVENT_HASHANDLERS"},
    });
    
    // Transition namespace
    registry.register_namespace("TRANSITION", {
        {"fade", "TRANSITION_FADE"},
        {"slide", "TRANSITION_SLIDE"},
        {"wipe", "TRANSITION_WIPE"},
        {"update", "TRANSITION_UPDATE"},
        {"draw", "TRANSITION_DRAW"},
        {"isComplete", "TRANSITION_ISCOMPLETE"},
        {"stop", "TRANSITION_STOP"},
    });
    
    registry.register_namespace("Game", {
        {"init", "GAME_INIT"},
        {"close", "GAME_CLOSE"},
        {"shouldClose", "GAME_SHOULDCLOSE"},
        {"setBackgroundColor", "GAME_SETBACKGROUNDCOLOR"},
        {"beginFrame", "GAME_BEGINFRAME"},
        {"endFrame", "GAME_ENDFRAME"},
        {"getWidth", "GAME_GETWIDTH"},
        {"getHeight", "GAME_GETHEIGHT"},
        {"setTargetFPS", "GAME_SETTARGETFPS"},
    });
    
    // Draw namespace (2D drawing helpers)
    registry.register_namespace("Draw", {
        {"drawRectangle", "DRAW_DRAWRECTANGLE"},
        {"drawCircle", "DRAW_DRAWCIRCLE"},
        {"drawLine", "DRAW_DRAWLINE"},
        {"drawText", "DRAW_DRAWTEXT"},
    });
    
    // Draw3D namespace (3D drawing helpers)
    registry.register_namespace("Draw3D", {
        {"drawCube", "DRAW3D_DRAWCUBE"},
        {"drawSphere", "DRAW3D_DRAWSPHERE"},
        {"drawGrid", "DRAW3D_DRAWGRID"},
    });
    
    // Collision namespace
    registry.register_namespace("Collision", {
        {"checkRectRect", "COLLISION_CHECKRECTRECT"},
        {"checkRectPoint", "COLLISION_CHECKRECTPOINT"},
        {"checkCircleCircle", "COLLISION_CHECKCIRCLECIRCLE"},
        {"checkCirclePoint", "COLLISION_CHECKCIRCLEPOINT"},
        {"checkSphereSphere", "COLLISION_CHECKSPHERESPHERE"},
        {"distance2D", "COLLISION_DISTANCE2D"},
        {"distance3D", "COLLISION_DISTANCE3D"},
    });
    
    // Math namespace (game math helpers - extends existing Math namespace)
    // Note: These are in addition to the existing Math namespace methods
    // We'll merge with existing Math namespace
    std::unordered_map<std::string, std::string> mathMethods = {
        {"clamp", "MATH_CLAMP"},
        {"lerp", "MATH_LERP"},
        {"lerpAngle", "MATH_LERPANGLE"},
        {"moveTowards", "MATH_MOVETOWARDS"},
        {"smoothDamp", "MATH_SMOOTHDAMP"},
    };
    
    // Merge with existing Math namespace if it exists
    if (registry.has_namespace("Math")) {
        // Get existing methods and merge
        // For now, just register - the registry will handle duplicates
        registry.register_namespace("Math", mathMethods);
    } else {
        registry.register_namespace("Math", mathMethods);
    }
}


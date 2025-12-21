#!/usr/bin/env python3
"""
Raylib bindings generator for BASIC interpreter.
Generates C++ bindings from YAML specification.
"""

import yaml
import pathlib
import textwrap
import sys
import re

HEADER = """// Auto-generated from specs/raylib_api.yaml
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
"""

FOOTER = """}

// Populate NativeFunctionRegistry with all registered raylib functions
// This allows YAML modules to reference raylib functions via the 'native' field
void populate_raylib_natives(NativeFunctionRegistry& native_registry, FunctionRegistry& function_registry) {
    // List of all raylib function names registered in register_raylib_bindings
    // This list is auto-generated and should match the functions registered above
    std::vector<std::string> raylib_function_names = {
{RaylibFunctionNames}
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
"""

def arg_to_cpp(i, t):
    """Convert argument type to C++ expression."""
    a = f"args[{i}]"
    if t.endswith('*'):
        return f"({t})({a}.as_int())"  # Assuming pointers are passed as integer addresses
    
    type_map = {
        "int": f"{a}.as_int()",
        "float": f"static_cast<float>({a}.as_number())",
        "double": f"{a}.as_number()",
        "bool": f"{a}.as_bool()",
        "string": f"{a}.as_string().c_str()",
        "Vector2": f"Value_to_Vector2({a})",
        "Vector3": f"Value_to_Vector3({a})",
        "Color": f"Value_to_Color({a})",
        "color": f"Value_to_Color({a})",
        "Rectangle": f"Value_to_Rectangle({a})",
        "Camera2D": f"Value_to_Camera2D({a})",
        "Camera3D": f"Value_to_Camera3D({a})",
    }
    
    if t in type_map:
        return type_map[t]
    else:
        # Unknown type - try to use Value_to_TypeName pattern
        type_name = t.replace(" ", "").replace("*", "")
        return f"Value_to_{type_name}({a})"  # Will fail at compile time if function doesn't exist, but better than KeyError

def ret_to_value(expr, t):
    """Convert return expression to Value."""
    if t.endswith('*'):
        return f"return Value::from_int((intptr_t)({expr}));" # Return pointers as integers
    return {
        "void": f"{expr};\n        return Value::nil();",
        "int": f"return Value::from_int({expr});",
        "float": f"return Value::from_number({expr});",
        "double": f"return Value::from_number({expr});",
        "bool": f"return Value::from_bool({expr});",
        "string": f"return Value::from_string({expr});",
        "Vector2": f"return Value::from_map(Vector2_to_Map({expr}));",
        "Vector3": f"return Value::from_map(Vector3_to_Map({expr}));",
        "Color": f"return Value::from_map(Color_to_Map({expr}));",
        "color": f"return Value::from_map(Color_to_Map({expr}));",
        "Rectangle": f"return Value::from_map(Rectangle_to_Map({expr}));",
        "Camera2D": f"return Value::from_map(Camera2D_to_Map({expr}));",
        "Camera3D": f"return Value::from_map(Camera3D_to_Map({expr}));",
    }.get(t, f"return Value::from_map({t}_to_Map({expr}));")  # Default: try _to_Map conversion

def load_available_symbols():
    """Parse raylib/raymath headers to collect available function names."""
    candidates = [
        pathlib.Path.cwd() / "_deps" / "raylib-src" / "src" / "raylib.h",
        pathlib.Path.cwd() / "_deps" / "raylib-src" / "src" / "raymath.h",
    ]
    symbols = set()
    for header in candidates:
        if header.exists():
            text = header.read_text(encoding="utf-8", errors="ignore")
            symbols.update(re.findall(r"RLAPI\s+[\w\*\s]+\s+(\w+)\s*\(", text))
            symbols.update(re.findall(r"RMAPI\s+[\w\*\s]+\s+(\w+)\s*\(", text))
    if not symbols:
        print("Warning: Could not locate raylib headers for validation; generating all bindings.")
    return symbols

ALLOWED_PREFIXES = ("std::",)

def is_supported_function(target, available):
    if not available:
        return True
    if not target:
        return True
    normalized = target.lstrip(":")
    if normalized in available:
        return True
    if any(normalized.startswith(prefix) for prefix in ALLOWED_PREFIXES):
        return True
    return False

def emit_fn(f):
    """Generate C++ function binding."""
    name = f["name"]
    map_to = f.get("map_to", f.get("raylib_name", name))
    args = f.get("args", [])
    ret = f.get("ret", "void")
    arity = len(args)
    
    arity_check = f'        if (args.size() != {arity}) throw std::runtime_error("{name}: expected {arity} args");\n'
    
    if "custom_body" in f:
        raw_body = f["custom_body"].rstrip()
        body = textwrap.indent(raw_body, "        ")
        # Fix MatrixIdentity references
        body = body.replace("::MatrixIdentity()", "MatrixIdentity()")
        
        # Handle return statement
        if "return" not in raw_body:
            # No return statement found - need to add one based on return type
            if ret == "void":
                body += "\n        return Value::nil();"
            else:
                # Extract the expression (remove trailing semicolon if present)
                expr = raw_body.rstrip().rstrip(';').strip()
                # Wrap in appropriate return statement based on return type
                if ret == "color":
                    body = f"        return Value::from_map(Color_to_Map({expr}));"
                elif ret == "int":
                    body = f"        return Value::from_int({expr});"
                elif ret == "float" or ret == "double":
                    body = f"        return Value::from_number({expr});"
                elif ret == "bool":
                    body = f"        return Value::from_bool({expr});"
                elif ret == "string":
                    body = f"        return Value::from_string({expr});"
                elif ret == "Vector2":
                    body = f"        return Value::from_map(Vector2_to_Map({expr}));"
                elif ret == "Vector3":
                    body = f"        return Value::from_map(Vector3_to_Map({expr}));"
                else:
                    # Default: assume it's an expression that returns the type
                    body = f"        return {expr};"
        call = body
    else:
        params = ", ".join(arg_to_cpp(i, t) for i, t in enumerate(args))
        call = "        " + ret_to_value(f"{map_to}({params})", ret)
    
    return f'''    R.add_with_policy("{name}", Fn{{"{name}", {arity}, [] (const std::vector<Value>& args) -> Value {{
{arity_check}{call}
    }}}}, true);'''

def emit_constant(c):
    """Generate C++ constant binding."""
    name = c["name"]
    value = c["value"]
    description = c.get("description", "")
    
    # Parse color values (r,g,b,a format)
    if "," in value:
        parts = value.split(",")
        if len(parts) == 4:  # RGBA color
            r, g, b, a = parts
            return f'''    R.add_with_policy("{name}", Fn{{"{name}", 0, [] (const std::vector<Value>& args) -> Value {{
        return Value::from_string("{r},{g},{b},{a}");
    }}}}, true);'''
    
    # Simple string/number constants
    return f'''    R.add_with_policy("{name}", Fn{{"{name}", 0, [] (const std::vector<Value>& args) -> Value {{
        return Value::from_string("{value}");
    }}}}, true);'''

def process_spec_file(spec_path, specs_dir, processed_files, available_symbols, function_names=None):
    """Process a single spec file and return functions/constants."""
    if function_names is None:
        function_names = []
    
    if spec_path in processed_files:
        return [], 0, 0, function_names
    
    processed_files.add(spec_path)
    
    if not spec_path.exists():
        print(f"Warning: {spec_path} not found", file=sys.stderr)
        return [], 0, 0, function_names
    
    print(f"Processing {spec_path.name}...")
    spec = yaml.safe_load(spec_path.read_text())
    
    out = []
    total_functions = 0
    skipped_functions = 0
    
    # Process imports first
    if "imports" in spec:
        for import_file in spec["imports"]:
            import_path = specs_dir / import_file
            import_out, import_total, import_skipped, function_names = process_spec_file(
                import_path, specs_dir, processed_files, available_symbols, function_names
            )
            out.extend(import_out)
            total_functions += import_total
            skipped_functions += import_skipped
    
    # Process functions
    if "functions" in spec:
        for f in spec["functions"]:
            target = f.get("map_to", f.get("raylib_name"))
            if f.get("raylib_name") and not is_supported_function(target, available_symbols):
                skipped_functions += 1
                print(f"Skipping unsupported function {f['name']} -> {target}")
                continue
            out.append(emit_fn(f))
            out.append("")
            function_names.append(f["name"])  # Track function name
            total_functions += 1
    
    # Process constants
    if "constants" in spec:
        for c in spec["constants"]:
            out.append(emit_constant(c))
            out.append("")
            function_names.append(c["name"])  # Track constant name
            total_functions += 1
    
    return out, total_functions, skipped_functions, function_names

def main():
    """Main generation function."""
    try:
        # Look for the YAML files in the source directory
        script_dir = pathlib.Path(__file__).parent
        source_dir = script_dir.parent
        specs_dir = source_dir / "specs"
        
        # Main spec files to process (these will handle imports)
        main_spec_files = [
            "raylib_api.yaml",  # Main API file that imports all modular specs
            "01_core_window.yaml",
            "24_simple_game_apis.yaml",
            "core_extended_functions.yaml",
            "26_input_missing_functions.yaml",
            "27_shapes_missing_functions.yaml",
            "28_textures_images_functions.yaml",
            "29_text_fonts_functions.yaml",
            "30_models_3d_functions.yaml",
            "31_audio_functions.yaml",
            "32_raymath_functions.yaml"
        ]
        
        out = [HEADER]
        total_functions = 0
        skipped_functions = 0
        available_symbols = load_available_symbols()
        processed_files = set()
        function_names = []  # Track all function names for native registry
        
        for spec_file in main_spec_files:
            spec_path = specs_dir / spec_file
            file_out, file_total, file_skipped, function_names = process_spec_file(
                spec_path, specs_dir, processed_files, available_symbols, function_names
            )
            out.extend(file_out)
            total_functions += file_total
            skipped_functions += file_skipped
        
        # Generate function names list for native registry population
        if function_names:
            function_names_list = ",\n".join(f'        "{name}"' for name in function_names)
        else:
            function_names_list = "        // No functions registered"
        
        footer_with_names = FOOTER.replace("{RaylibFunctionNames}", function_names_list)
        out.append(footer_with_names)
        print(f"Generated {total_functions} functions (skipped {skipped_functions})")
        
        # The script's working directory is the build directory.
        # The output path must match the OUTPUT specified in CMakeLists.txt
        build_dir = pathlib.Path.cwd()
        output_path = build_dir / "src" / "bindings" / "rt_raylib.gen.cpp"
        
        # Ensure the target directory exists
        output_path.parent.mkdir(parents=True, exist_ok=True)
        output_path.write_text("".join(out), encoding="utf-8")
        
        print(f"Generated {output_path} with {total_functions} functions")
        return 0
        
    except Exception as e:
        print(f"Error generating bindings: {e}", file=sys.stderr)
        return 1

if __name__ == "__main__":
    sys.exit(main())

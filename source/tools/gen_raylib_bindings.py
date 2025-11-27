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
#include "bas/value.hpp"
#include "bas/camera3d.hpp"
#include "bas/vector3d.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <memory>

using bas::Value;
using Fn = bas::NativeFn;

// Helper macros to avoid narrowing conversion warnings
#define FLOAT(i) static_cast<float>(args[i].as_number())
#define INT(i) args[i].as_int()
#define STR(i) args[i].as_string()
#define BOOL(i) args[i].as_bool()

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
"""

FOOTER = """}
} // namespace bas
"""

def arg_to_cpp(i, t):
    """Convert argument type to C++ expression."""
    a = f"args[{i}]"
    if t.endswith('*'):
        return f"({t})({a}.as_int())"  # Assuming pointers are passed as integer addresses
    return {
        "int": f"{a}.as_int()",
        "float": f"static_cast<float>({a}.as_number())",
        "double": f"{a}.as_number()",
        "bool": f"{a}.as_bool()",
        "string": f"{a}.as_string().c_str()",
        "Vector2": f"Value::to_vector2({a})",
        "Vector3": f"Value::to_vector3({a})",
        "Color": f"Value::to_color({a})",
        "Rectangle": f"Value::to_rectangle({a})",
        "Camera2D": f"Value::to_camera2d({a})",
        "Camera3D": f"Value::to_camera3d({a})",
    }[t]

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
        "Vector2": f"return Value::from_vector2({expr});",
        "Vector3": f"return Value::from_vector3({expr});",
        "Color": f"return Value::from_color({expr});",
        "Rectangle": f"return Value::from_rectangle({expr});",
        "Camera2D": f"return Value::from_camera2d({expr});",
        "Camera3D": f"return Value::from_camera3d({expr});",
    }[t]

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
        if ret == "void" and "return" not in raw_body:
            body += "\n        return Value::nil();"
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

def process_spec_file(spec_path, specs_dir, processed_files, available_symbols):
    """Process a single spec file and return functions/constants."""
    if spec_path in processed_files:
        return [], 0, 0
    
    processed_files.add(spec_path)
    
    if not spec_path.exists():
        print(f"Warning: {spec_path} not found", file=sys.stderr)
        return [], 0, 0
    
    print(f"Processing {spec_path.name}...")
    spec = yaml.safe_load(spec_path.read_text())
    
    out = []
    total_functions = 0
    skipped_functions = 0
    
    # Process imports first
    if "imports" in spec:
        for import_file in spec["imports"]:
            import_path = specs_dir / import_file
            import_out, import_total, import_skipped = process_spec_file(
                import_path, specs_dir, processed_files, available_symbols
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
            total_functions += 1
    
    # Process constants
    if "constants" in spec:
        for c in spec["constants"]:
            out.append(emit_constant(c))
            out.append("")
            total_functions += 1
    
    return out, total_functions, skipped_functions

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
            "25_core_missing_functions.yaml",
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
        
        for spec_file in main_spec_files:
            spec_path = specs_dir / spec_file
            file_out, file_total, file_skipped = process_spec_file(
                spec_path, specs_dir, processed_files, available_symbols
            )
            out.extend(file_out)
            total_functions += file_total
            skipped_functions += file_skipped
        
        out.append(FOOTER)
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

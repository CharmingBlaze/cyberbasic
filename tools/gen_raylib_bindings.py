#!/usr/bin/env python3
"""
Raylib bindings generator for BASIC interpreter.
Generates C++ bindings from YAML specification.
"""

import yaml
import pathlib
import textwrap
import sys

HEADER = """// Auto-generated from specs/raylib_api.yaml
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

def emit_fn(f):
    """Generate C++ function binding."""
    name = f["name"]
    map_to = f.get("map_to", f.get("raylib_name", name))
    args = f.get("args", [])
    ret = f.get("ret", "void")
    arity = len(args)
    
    arity_check = f'        if (args.size() != {arity}) throw std::runtime_error("{name}: expected {arity} args");\n'
    
    if "custom_body" in f:
        body = textwrap.indent(f["custom_body"].rstrip(), "        ")
        # Fix MatrixIdentity references
        body = body.replace("::MatrixIdentity()", "MatrixIdentity()")
        call = f"{body}\n        {ret_to_value('0', 'void') if ret=='void' else ''}"
    else:
        params = ", ".join(arg_to_cpp(i, t) for i, t in enumerate(args))
        call = "        " + ret_to_value(f"{map_to}({params})", ret)
    
    return f'''    R.add_with_policy("{name}", Fn{{"{name}", {arity}, [] (const std::vector<Value>& args) -> Value {{
{arity_check}{call}
    }}}}, true);'''

def main():
    """Main generation function."""
    try:
        # Look for the YAML files in the source directory
        script_dir = pathlib.Path(__file__).parent
        source_dir = script_dir.parent
        specs_dir = source_dir / "specs"
        
        # List of spec files to process
        spec_files = [
            "01_core_window.yaml",
            "raylib_api.yaml",
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
        
        for spec_file in spec_files:
            spec_path = specs_dir / spec_file
            if spec_path.exists():
                print(f"Processing {spec_file}...")
                spec = yaml.safe_load(spec_path.read_text())
                
                if "functions" in spec:
                    for f in spec["functions"]:
                        out.append(emit_fn(f))
                        out.append("")
                        total_functions += 1
            else:
                print(f"Warning: {spec_path} not found", file=sys.stderr)
        
        out.append(FOOTER)
        print(f"Generated {total_functions} functions")
        
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

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
    return {
        "int": f"{a}.as_int()",
        "float": f"static_cast<float>({a}.as_number())",
        "double": f"{a}.as_number()",
        "bool": f"{a}.as_bool()",
        "string": f"{a}.as_string().c_str()",
    }[t]

def ret_to_value(expr, t):
    """Convert return expression to Value."""
    return {
        "void": "return Value::nil();",
        "int": f"return Value::from_int({expr});",
        "float": f"return Value::from_number({expr});",
        "double": f"return Value::from_number({expr});",
        "bool": f"return Value::from_bool({expr});",
        "string": f"return Value::from_string({expr});",
    }[t]

def emit_fn(f):
    """Generate C++ function binding."""
    name = f["name"]
    map_to = f.get("map_to", name)
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
    
    return f'''    R.add("{name}", Fn{{"{name}", {arity}, [] (const std::vector<Value>& args) -> Value {{
{arity_check}{call}
    }}}});'''

def main():
    """Main generation function."""
    try:
        spec_path = pathlib.Path("specs/raylib_api.yaml")
        if not spec_path.exists():
            print(f"Error: {spec_path} not found", file=sys.stderr)
            return 1
            
        spec = yaml.safe_load(spec_path.read_text())
        out = [HEADER]
        
        for f in spec["functions"]:
            out.append(emit_fn(f))
            out.append("")
        
        out.append(FOOTER)
        
        output_path = pathlib.Path("src/rt_raylib.gen.cpp")
        output_path.write_text("".join(out), encoding="utf-8")
        
        print(f"Generated {output_path} with {len(spec['functions'])} functions")
        return 0
        
    except Exception as e:
        print(f"Error generating bindings: {e}", file=sys.stderr)
        return 1

if __name__ == "__main__":
    sys.exit(main())

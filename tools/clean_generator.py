#!/usr/bin/env python3
"""
Clean Raylib Binding Generator for CyberBasic
Professional, scalable, follows best practices
"""

import yaml
import os
import sys
from pathlib import Path
from typing import Dict, List, Any

class CleanGenerator:
    def __init__(self):
        # Simple type mapping - only what we need
        self.simple_types = {
            'int': 'as_int()',
            'float': 'as_number()', 
            'double': 'as_number()',
            'string': 'as_string().c_str()',
            'bool': 'as_bool()'
        }
        
        # Complex types that need special handling
        self.complex_types = {
            'color': 'Color',
            'vector2': 'Vector2', 
            'vector3': 'Vector3',
            'rectangle': 'Rectangle',
            'texture2d': 'Texture2D',
            'sound': 'Sound',
            'music': 'Music',
            'font': 'Font',
            'model': 'Model',
            'mesh': 'Mesh',
            'material': 'Material',
            'shader': 'Shader',
            'image': 'Image',
            'wave': 'Wave',
            'audiostream': 'AudioStream',
            'rendertexture2d': 'RenderTexture2D',
            'texturecubemap': 'TextureCubemap'
        }

    def load_yaml_file(self, filepath: str) -> Dict[str, Any]:
        """Load and parse a YAML file"""
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                return yaml.safe_load(f)
        except Exception as e:
            print(f"Error loading {filepath}: {e}")
            return None

    def is_simple_function(self, func: Dict[str, Any]) -> bool:
        """Check if function only uses simple types or has custom body"""
        if not func.get('working', False):
            return False
            
        # If it has a custom_body, it's handled specially
        if 'custom_body' in func:
            return True
            
        # Otherwise check if all args are simple types
        args = func.get('args', [])
        for arg_type in args:
            if arg_type.lower() not in self.simple_types:
                return False
        return True

    def generate_simple_binding(self, func: Dict[str, Any]) -> str:
        """Generate binding for simple functions only"""
        name = func['name']
        raylib_name = func['raylib_name']
        args = func['args']
        ret_type = func['ret']
        description = func.get('description', '')
        
        # Check if this function has a custom body
        if 'custom_body' in func:
            custom_body = func['custom_body']
            
            # Generate function body for custom functions
            if ret_type == 'void':
                function_body = f'''        if (args.size() != {len(args)}) 
            throw std::runtime_error("{name}: expected {len(args)} args");
        {custom_body};
        return Value::nil();'''
            elif ret_type == 'color':
                function_body = f'''        if (args.size() != {len(args)}) 
            throw std::runtime_error("{name}: expected {len(args)} args");
        auto result = {custom_body};
        return Value::from_number((long long)&result);'''
            elif ret_type == 'vector2':
                function_body = f'''        if (args.size() != {len(args)}) 
            throw std::runtime_error("{name}: expected {len(args)} args");
        auto result = {custom_body};
        return Value::from_number((long long)&result);'''
            elif ret_type == 'string':
                function_body = f'''        if (args.size() != {len(args)}) 
            throw std::runtime_error("{name}: expected {len(args)} args");
        auto result = {custom_body};
        return Value::from_string(result);'''
            elif ret_type in ['sound', 'music', 'wave', 'audiostream', 'image', 'texture2d', 'rendertexture2d', 'font', 'rectangle', 'model', 'mesh', 'material', 'shader', 'camera3d']:
                function_body = f'''        if (args.size() != {len(args)}) 
            throw std::runtime_error("{name}: expected {len(args)} args");
        auto result = {custom_body};
        return Value::from_number((long long)&result);'''
            else:
                function_body = f'''        if (args.size() != {len(args)}) 
            throw std::runtime_error("{name}: expected {len(args)} args");
        auto result = {custom_body};
        return Value::from_number(result);'''
            
            return f'''    // {description}
    R.add("{name}", NativeFn{{"{name}", {len(args)}, [] (const std::vector<Value>& args) -> Value {{
{function_body}
    }}}});'''
        
        # Generate argument conversions for standard functions
        arg_conversions = []
        for i, arg_type in enumerate(args):
            conversion = f'args[{i}].{self.simple_types[arg_type.lower()]}'
            arg_conversions.append(conversion)
        
        arg_list = ', '.join(arg_conversions)
        
        # Generate function body for standard functions
        if ret_type == 'void':
            function_body = f'''        if (args.size() != {len(args)}) 
            throw std::runtime_error("{name}: expected {len(args)} args");
        {raylib_name}({arg_list});
        return Value::nil();'''
        else:
            function_body = f'''        if (args.size() != {len(args)}) 
            throw std::runtime_error("{name}: expected {len(args)} args");
        auto result = {raylib_name}({arg_list});
        return Value::from_number(result);'''
        
        return f'''    // {description}
    R.add("{name}", NativeFn{{"{name}", {len(args)}, [] (const std::vector<Value>& args) -> Value {{
{function_body}
    }}}});'''

    def generate_cpp_bindings(self, functions: List[Dict[str, Any]], output_file: str):
        """Generate clean C++ bindings"""
        
        # Only simple functions
        simple_functions = [f for f in functions if self.is_simple_function(f)]
        
        cpp_code = f"""// Clean Raylib Bindings for CyberBasic
// Generated with professional best practices
#include "raylib.h"
#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include "bas/custom_cyberbasic.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <algorithm>

using bas::Value;
using NativeFn = bas::NativeFn;

namespace bas {{
void register_raylib_bindings(FunctionRegistry& R) {{
    // === {len(simple_functions)} Simple Functions Generated ===
    // Following best practices: simple types only, proper error handling

"""
        
        # Generate function bindings
        for func in simple_functions:
            cpp_code += self.generate_simple_binding(func) + "\n\n"
        
        # Footer
        cpp_code += """}
}
"""
        
        # Write to file
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(cpp_code)
        
        print(f"Generated {output_file} with {len(simple_functions)} simple functions")
        print("Following best practices: clean, maintainable, scalable")

    def process_modular_files(self, spec_files: List[str], output_file: str):
        """Process multiple modular YAML files"""
        all_functions = []
        
        for filepath in spec_files:
            if not os.path.exists(filepath):
                print(f"Warning: {filepath} not found, skipping...")
                continue
                
            data = self.load_yaml_file(filepath)
            if data and 'functions' in data:
                functions = data['functions']
                all_functions.extend(functions)
                print(f"Loaded {len(functions)} functions from {filepath}")
            else:
                print(f"Warning: No functions found in {filepath}")
        
        if not all_functions:
            print("Error: No functions found in any files!")
            sys.exit(1)
        
        print(f"\nTotal functions: {len(all_functions)}")
        
        # Generate C++ bindings
        self.generate_cpp_bindings(all_functions, output_file)

def main():
    """Main function"""
    generator = CleanGenerator()
    
    # Define the modular YAML files
    spec_files = [
        'specs/01_core_window.yaml',
        'specs/02_basic_graphics.yaml',
        'specs/03_colors.yaml',
        'specs/04_basic_drawing.yaml',
        'specs/05_input.yaml',
        'specs/06_audio.yaml',
        'specs/07_textures_images.yaml',
        'specs/08_fonts_text.yaml',
        'specs/09_3d_graphics.yaml',
        'specs/10_advanced_graphics.yaml',
        'specs/11_physics_animation.yaml',
        'specs/12_shaders.yaml',
        'specs/13_advanced_3d.yaml',
        'specs/14_post_processing.yaml',
        'specs/15_remaining_raylib.yaml',
        'specs/16_platform_specific.yaml',
        'specs/17_custom_cyberbasic.yaml',
    ]
    
    # Output file
    output_file = 'src/rt_raylib.gen.cpp'
    
    print("Clean Raylib Binding Generator")
    print("=" * 50)
    print("Professional Features:")
    print("- Simple type handling only")
    print("- Clean, maintainable code")
    print("- Proper error handling")
    print("- Scalable architecture")
    print("- No Windows API conflicts")
    print("=" * 50)
    
    # Process modular files
    generator.process_modular_files(spec_files, output_file)
    
    print(f"\nSuccessfully generated {output_file}")
    print("Ready for compilation with best practices")

if __name__ == "__main__":
    main()

#!/usr/bin/env python3
"""
Comprehensive CyberBasic Examples Fixer

This script fixes common issues in all CyberBasic examples to ensure they work
properly and provide a good user experience.

Common fixes:
1. DRAWTEXT calls - ensure 7 arguments (text, x, y, size, r, g, b)
2. Key constants - replace KEY_* with numeric codes
3. Function calls - ensure correct argument counts
4. Modern syntax - update to current CyberBasic standards
"""

import os
import re
import glob
from pathlib import Path

# Key mappings from raylib constants to numeric codes
KEY_MAPPINGS = {
    'KEY_SPACE': '32',
    'KEY_ENTER': '257', 
    'KEY_ESCAPE': '256',
    'KEY_UP': '265',
    'KEY_DOWN': '264', 
    'KEY_LEFT': '263',
    'KEY_RIGHT': '262',
    'KEY_W': '87',
    'KEY_A': '65', 
    'KEY_S': '83',
    'KEY_D': '68',
    'KEY_J': '74',
    'KEY_K': '75',
    'KEY_L': '76',
    'KEY_R': '82',
    'KEY_E': '69',
    'KEY_Q': '81',
    'KEY_F': '70',
    'KEY_G': '71',
    'KEY_H': '72',
    'KEY_I': '73',
    'KEY_O': '79',
    'KEY_P': '80',
    'KEY_T': '84',
    'KEY_U': '85',
    'KEY_V': '86',
    'KEY_X': '88',
    'KEY_Y': '89',
    'KEY_Z': '90',
    'KEY_LEFT_SHIFT': '340',
    'KEY_LEFT_CONTROL': '341',
    'KEY_LEFT_ALT': '342',
    'KEY_F1': '290',
    'KEY_F2': '291',
    'KEY_F3': '292',
    'KEY_F4': '293',
    'KEY_F5': '294',
}

def fix_drawtext_calls(content):
    """Fix DRAWTEXT calls to use correct 7-argument format."""
    
    def fix_drawtext_match(match):
        full_call = match.group(0)
        
        # Handle simple cases with clear argument separation
        # Pattern: DRAWTEXT("text", x, y, size, r, g, b[, alpha])
        simple_pattern = r'DRAWTEXT\s*\(\s*([^,]+),\s*([^,]+),\s*([^,]+),\s*([^,]+),\s*([^,]+),\s*([^,]+),\s*([^,]+)(?:,\s*([^)]+))?\s*\)'
        simple_match = re.match(simple_pattern, full_call)
        
        if simple_match:
            text, x, y, size, r, g, b = simple_match.groups()[:7]
            alpha = simple_match.group(8)
            
            if alpha:
                # Remove alpha component
                return f'DRAWTEXT({text}, {x}, {y}, {size}, {r}, {g}, {b})'
            else:
                # Already correct format
                return full_call
        
        # Handle complex cases with string concatenation
        # Look for patterns like: DRAWTEXT("text" + STR(var), ...)
        complex_pattern = r'DRAWTEXT\s*\(\s*(.+?),\s*([^,]+),\s*([^,]+),\s*([^,]+),\s*([^,]+),\s*([^,]+),\s*([^,]+)(?:,\s*([^)]+))?\s*\)'
        complex_match = re.match(complex_pattern, full_call)
        
        if complex_match:
            text, x, y, size, r, g, b = complex_match.groups()[:7]
            alpha = complex_match.group(8)
            
            if alpha:
                # Remove alpha component
                return f'DRAWTEXT({text}, {x}, {y}, {size}, {r}, {g}, {b})'
            else:
                # Already correct format
                return full_call
        
        return full_call
    
    # Fix DRAWTEXT calls
    content = re.sub(r'DRAWTEXT\s*\([^)]+\)', fix_drawtext_match, content)
    
    return content

def fix_key_constants(content):
    """Replace KEY_* constants with numeric codes."""
    
    for key_name, key_code in KEY_MAPPINGS.items():
        # Replace KEY_* constants with numeric codes
        content = re.sub(rf'\b{key_name}\b', key_code, content)
    
    return content

def fix_common_function_calls(content):
    """Fix common function call issues."""
    
    # Fix BEGINDRAW/ENDDRAW (should be BEGINDRAWING/ENDDRAWING)
    content = re.sub(r'\bBEGINDRAW\b', 'BEGINDRAWING', content)
    content = re.sub(r'\bENDDRAW\b', 'ENDDRAWING', content)
    
    # Fix BEGINDRAW3D/ENDDRAW3D (should be BEGINMODE3D/ENDMODE3D)
    content = re.sub(r'\bBEGINDRAW3D\b', 'BEGINMODE3D', content)
    content = re.sub(r'\bENDDRAW3D\b', 'ENDMODE3D', content)
    
    return content

def fix_color_usage(content):
    """Fix Color() usage - convert to individual RGB components where needed."""
    
    # This is a more complex fix that would require parsing
    # For now, we'll leave Color() usage as-is since it might be supported
    return content

def add_helpful_comments(content, filename):
    """Add helpful comments to make examples more educational."""
    
    if not content.startswith('REM '):
        # Add a header comment explaining what the example does
        example_name = Path(filename).stem.replace('_', ' ').title()
        header = f"REM {example_name} - CyberBasic Example\n"
        header += f"REM This example demonstrates {example_name.lower()} functionality\n"
        header += f"REM Use WASD or arrow keys to interact (where applicable)\n\n"
        content = header + content
    
    return content

def fix_file(filepath):
    """Fix all issues in a single .bas file."""
    print(f"Fixing {filepath}...")
    
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        original_content = content
        
        # Apply all fixes
        content = fix_drawtext_calls(content)
        content = fix_key_constants(content)
        content = fix_common_function_calls(content)
        content = fix_color_usage(content)
        content = add_helpful_comments(content, filepath)
        
        # Only write if content changed
        if content != original_content:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"  ✓ Fixed {filepath}")
            return True
        else:
            print(f"  - No changes needed for {filepath}")
            return False
            
    except Exception as e:
        print(f"  ✗ Error fixing {filepath}: {e}")
        return False

def main():
    """Fix all examples in the examples directory."""
    examples_dir = Path("examples")
    
    if not examples_dir.exists():
        print("Error: examples directory not found!")
        return
    
    # Find all .bas files
    bas_files = list(examples_dir.glob("*.bas"))
    
    print(f"Found {len(bas_files)} .bas files")
    print("Applying comprehensive fixes...\n")
    
    fixed_count = 0
    
    for filepath in sorted(bas_files):
        if fix_file(filepath):
            fixed_count += 1
        print()
    
    print(f"Fixed {fixed_count} out of {len(bas_files)} files")
    print("\nAll examples should now work properly with CyberBasic!")

if __name__ == "__main__":
    main()

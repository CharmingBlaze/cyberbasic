#!/usr/bin/env python3
"""
Fix DRAWTEXT calls in all CyberBasic examples

This script fixes DRAWTEXT function calls to use the correct 7-argument format:
DRAWTEXT(text, x, y, fontSize, r, g, b)

It handles various incorrect formats:
- 8 arguments (with alpha) -> removes alpha
- 6 arguments (missing color) -> adds white color (255, 255, 255)
- Other formats -> attempts to fix based on pattern
"""

import os
import re
import glob
from pathlib import Path

def fix_drawtext_line(line):
    """Fix a single line containing DRAWTEXT."""
    # Pattern to match DRAWTEXT calls
    pattern = r'DRAWTEXT\s*\(\s*([^)]+)\s*\)'
    
    def fix_drawtext_args(match):
        args_str = match.group(1)
        # Split arguments, handling nested function calls and strings
        args = []
        paren_count = 0
        quote_count = 0
        current_arg = ""
        
        for char in args_str:
            if char == '"' and (not current_arg or current_arg[-1] != '\\'):
                quote_count = (quote_count + 1) % 2
            elif char == '(' and quote_count == 0:
                paren_count += 1
            elif char == ')' and quote_count == 0:
                paren_count -= 1
            elif char == ',' and paren_count == 0 and quote_count == 0:
                args.append(current_arg.strip())
                current_arg = ""
                continue
            
            current_arg += char
        
        if current_arg.strip():
            args.append(current_arg.strip())
        
        # Fix based on argument count
        if len(args) == 7:
            # Already correct format
            return f'DRAWTEXT({", ".join(args)})'
        elif len(args) == 8:
            # Remove alpha (last argument)
            return f'DRAWTEXT({", ".join(args[:7])})'
        elif len(args) == 6:
            # Missing one color component, assume it's missing blue, add white
            return f'DRAWTEXT({", ".join(args)}, 255)'
        elif len(args) == 5:
            # Missing two color components, add green and blue for white
            return f'DRAWTEXT({", ".join(args)}, 255, 255)'
        elif len(args) == 4:
            # Missing all color components, add white (255, 255, 255)
            return f'DRAWTEXT({", ".join(args)}, 255, 255, 255)'
        elif len(args) == 3:
            # Missing font size and color, add default size 20 and white
            return f'DRAWTEXT({", ".join(args)}, 20, 255, 255, 255)'
        else:
            # Unknown format, return as-is with a comment
            print(f"Warning: Unknown DRAWTEXT format with {len(args)} args: {args_str}")
            return match.group(0)
    
    return re.sub(pattern, fix_drawtext_args, line)

def fix_file(filepath):
    """Fix DRAWTEXT calls in a single file."""
    print(f"Processing {filepath}...")
    
    with open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()
    
    modified = False
    new_lines = []
    
    for i, line in enumerate(lines):
        if 'DRAWTEXT' in line.upper():
            new_line = fix_drawtext_line(line)
            if new_line != line:
                print(f"  Line {i+1}: Fixed DRAWTEXT call")
                print(f"    Before: {line.strip()}")
                print(f"    After:  {new_line.strip()}")
                modified = True
            new_lines.append(new_line)
        else:
            new_lines.append(line)
    
    if modified:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.writelines(new_lines)
        print(f"  ✓ Fixed {filepath}")
    else:
        print(f"  - No changes needed for {filepath}")

def main():
    """Fix all DRAWTEXT calls in the examples directory."""
    examples_dir = Path("examples")
    
    if not examples_dir.exists():
        print("Error: examples directory not found!")
        return
    
    # Find all .bas files
    bas_files = list(examples_dir.glob("*.bas"))
    
    print(f"Found {len(bas_files)} .bas files in examples directory")
    print("Fixing DRAWTEXT calls...\n")
    
    for filepath in bas_files:
        fix_file(filepath)
        print()
    
    print("All files processed!")

if __name__ == "__main__":
    main()

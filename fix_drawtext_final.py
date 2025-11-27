#!/usr/bin/env python3
"""
Final DRAWTEXT fixer - handles all edge cases
"""

import os
import re
from pathlib import Path

def fix_drawtext_in_file(filepath):
    """Fix all DRAWTEXT calls in a file to use exactly 7 arguments."""
    
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()
    
    original_content = content
    
    # Find all DRAWTEXT calls and fix them
    lines = content.split('\n')
    fixed_lines = []
    
    for line in lines:
        if 'DRAWTEXT(' in line:
            # Count commas to determine argument count
            # This is a simple approach that works for most cases
            start_idx = line.find('DRAWTEXT(')
            if start_idx != -1:
                # Find the matching closing parenthesis
                paren_count = 0
                end_idx = -1
                for i in range(start_idx + 9, len(line)):  # Start after 'DRAWTEXT('
                    if line[i] == '(':
                        paren_count += 1
                    elif line[i] == ')':
                        if paren_count == 0:
                            end_idx = i
                            break
                        else:
                            paren_count -= 1
                
                if end_idx != -1:
                    # Extract the arguments part
                    args_part = line[start_idx + 9:end_idx]
                    
                    # Count commas (simple approach)
                    comma_count = args_part.count(',')
                    arg_count = comma_count + 1
                    
                    # If we have 8 arguments, remove the last one (alpha)
                    if arg_count == 8:
                        # Find the last comma and remove everything after it
                        last_comma_idx = args_part.rfind(',')
                        if last_comma_idx != -1:
                            new_args = args_part[:last_comma_idx]
                            new_line = line[:start_idx + 9] + new_args + line[end_idx:]
                            line = new_line
                            print(f"  Fixed DRAWTEXT in {filepath}: {arg_count} -> 7 args")
        
        fixed_lines.append(line)
    
    new_content = '\n'.join(fixed_lines)
    
    if new_content != original_content:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(new_content)
        return True
    
    return False

def main():
    """Fix all DRAWTEXT calls in examples."""
    examples_dir = Path("examples")
    
    if not examples_dir.exists():
        print("Error: examples directory not found!")
        return
    
    bas_files = list(examples_dir.glob("*.bas"))
    
    print(f"Fixing DRAWTEXT calls in {len(bas_files)} files...")
    
    fixed_count = 0
    for filepath in bas_files:
        if fix_drawtext_in_file(filepath):
            fixed_count += 1
    
    print(f"Fixed DRAWTEXT calls in {fixed_count} files")

if __name__ == "__main__":
    main()

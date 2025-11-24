# Quick Start: CyberBasic Upgrade Implementation

This is a practical, step-by-step guide to begin the comprehensive upgrade immediately.

## Prerequisites Check

Before starting, ensure you have:
- [ ] Git installed and repository cloned
- [ ] CMake 3.25+ installed
- [ ] C++20 compiler (GCC 13+ or Clang 15+)
- [ ] Python 3.10+ with PyYAML
- [ ] Code editor (VS Code, CLion, etc.)

## Step 1: Setup (30 minutes)

### 1.1 Create Backup Branch
```bash
git checkout -b comprehensive-upgrade
git tag pre-upgrade-backup
```

### 1.2 Verify Current Build
```bash
mkdir build && cd build
cmake ..
cmake --build .
./cyberbasic ../examples/hello_text.bas
```

If this works, you're ready to proceed.

## Step 2: Immediate Cleanup (Day 1)

### 2.1 Remove Backup Files
```bash
# Delete backup files
rm CMakeLists.txt.backup
# Check for other backups
find . -name "*.backup" -o -name "*.old" -o -name "*.bak"
```

### 2.2 Identify Duplicates
Run this script to find potential duplicates:
```bash
# Create audit script
cat > audit_duplicates.sh << 'EOF'
#!/bin/bash
echo "Checking for duplicate function registrations..."
grep -r "register.*function\|FunctionRegistry::add" src/ | sort | uniq -d
echo "Checking for duplicate headers..."
find include/bas -name "*.hpp" | xargs grep -l "class\|struct" | sort | uniq -d
EOF
chmod +x audit_duplicates.sh
./audit_duplicates.sh
```

### 2.3 Remove Unused Includes
Use a tool like `include-what-you-use` or manually check:
```bash
# Example: Check a file for unused includes
# (This requires include-what-you-use tool)
# iwyu src/main.cpp
```

## Step 3: Create Module Structure (Day 2-3)

### 3.1 Create Directory Structure
```bash
mkdir -p src/core
mkdir -p src/modules/{graphics,audio,input,networking,physics,gui,raylib}
mkdir -p src/bindings
```

### 3.2 Move Core Files
```bash
# Move core interpreter files
mv src/lexer.cpp src/core/
mv src/lexer.hpp include/bas/core/ 2>/dev/null || echo "Header may be in different location"
mv src/parser.cpp src/core/
mv src/interpreter.cpp src/core/
mv src/runtime.cpp src/core/
# Keep value.hpp where it is if it's already in include/bas/
```

### 3.3 Update CMakeLists.txt
Add module structure to CMakeLists.txt:
```cmake
# Core interpreter (always built)
add_library(cyberbasic_core STATIC
  src/core/lexer.cpp
  src/core/parser.cpp
  src/core/interpreter.cpp
  src/core/runtime.cpp
)

target_include_directories(cyberbasic_core PUBLIC include)

# Optional modules
option(BUILD_GRAPHICS_MODULE "Build graphics module" ON)
option(BUILD_AUDIO_MODULE "Build audio module" ON)
# ... etc
```

## Step 4: Implement Module Interface (Day 4-5)

### 4.1 Create Module Base Class
Create `include/bas/module_base.hpp`:
```cpp
#pragma once
#include "bas/runtime.hpp"

namespace bas {
    class Module {
    public:
        virtual ~Module() = default;
        virtual void register_functions(FunctionRegistry& registry) = 0;
        virtual void initialize() {}
        virtual void shutdown() {}
        virtual const char* name() const = 0;
        virtual const char* version() const { return "1.0.0"; }
    };
}
```

### 4.2 Convert First Module (Graphics)
Create `src/modules/graphics/graphics_module.cpp`:
```cpp
#include "bas/module_base.hpp"
#include "bas/runtime.hpp"
#include "bas/graphics.hpp"

namespace bas {
    class GraphicsModule : public Module {
    public:
        const char* name() const override { return "graphics"; }
        void register_functions(FunctionRegistry& registry) override {
            register_graphics_functions(registry);
        }
    };
}
```

## Step 5: Modularize Raylib (Week 2)

### 5.1 Organize Raylib Bindings
Create separate files for each category:
- `src/modules/raylib/window.cpp` - Window management
- `src/modules/raylib/graphics2d.cpp` - 2D graphics
- `src/modules/raylib/graphics3d.cpp` - 3D graphics
- `src/modules/raylib/audio.cpp` - Audio functions
- `src/modules/raylib/input.cpp` - Input handling

### 5.2 Update Generator Script
Modify `tools/gen_raylib_bindings.py` to generate modular output:
```python
# Instead of one large file, generate:
# - rt_raylib_window.gen.cpp
# - rt_raylib_graphics2d.gen.cpp
# etc.
```

## Step 6: Update Build System (Week 2-3)

### 6.1 Modular CMakeLists.txt
Create `cmake/Modules.cmake`:
```cmake
function(add_cyberbasic_module name)
    cmake_parse_arguments(MOD "" "DIRECTORY" "SOURCES" ${ARGN})
    add_library(${name} STATIC ${MOD_SOURCES})
    target_link_libraries(${name} PUBLIC cyberbasic_core)
    target_include_directories(${name} PUBLIC include)
endfunction()
```

### 6.2 Update Main CMakeLists.txt
```cmake
include(cmake/Modules.cmake)

# Core (always built)
add_cyberbasic_module(cyberbasic_core
    SOURCES
        src/core/lexer.cpp
        src/core/parser.cpp
        src/core/interpreter.cpp
        src/core/runtime.cpp
)

# Optional modules
if(BUILD_GRAPHICS_MODULE)
    add_cyberbasic_module(cyberbasic_graphics
        SOURCES
            src/modules/graphics/graphics_module.cpp
    )
endif()
```

## Step 7: Code Quality (Week 3-4)

### 7.1 Add .clang-format
Create `.clang-format`:
```yaml
BasedOnStyle: LLVM
IndentWidth: 4
TabWidth: 4
UseTab: Never
ColumnLimit: 100
```

### 7.2 Add clang-tidy
Create `.clang-tidy`:
```yaml
Checks: >
  -*,
  bugprone-*,
  performance-*,
  modernize-*,
  readability-*
WarningsAsErrors: ''
```

### 7.3 Format Code
```bash
find src include -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
```

## Step 8: Testing Setup (Week 4)

### 8.1 Add Testing Framework
Add to CMakeLists.txt:
```cmake
enable_testing()
find_package(Catch2 QUIET)
if(Catch2_FOUND)
    add_subdirectory(tests)
endif()
```

### 8.2 Create First Test
Create `tests/test_lexer.cpp`:
```cpp
#include <catch2/catch_test_macros.hpp>
#include "bas/core/lexer.hpp"

TEST_CASE("Lexer basic tokens", "[lexer]") {
    // Test implementation
}
```

## Daily Checklist

### Week 1: Cleanup
- [ ] Day 1: Remove backups, audit duplicates
- [ ] Day 2: Create module structure
- [ ] Day 3: Move core files
- [ ] Day 4: Implement module interface
- [ ] Day 5: Convert first module

### Week 2: Modularization
- [ ] Day 1-2: Modularize raylib bindings
- [ ] Day 3-4: Update build system
- [ ] Day 5: Test modular build

### Week 3: Quality
- [ ] Day 1-2: Code formatting
- [ ] Day 3-4: Apply C++20 features
- [ ] Day 5: Documentation

### Week 4: Testing
- [ ] Day 1-2: Set up test framework
- [ ] Day 3-4: Write tests
- [ ] Day 5: CI/CD setup

## Common Issues & Solutions

### Issue: Build fails after moving files
**Solution**: Update all include paths and CMakeLists.txt

### Issue: Duplicate symbols
**Solution**: Check FunctionRegistry for duplicate registrations

### Issue: Module not loading
**Solution**: Verify module registration in main.cpp

## Progress Tracking

Use this template to track progress:
```markdown
## Progress Log

### Week 1
- [x] Setup complete
- [x] Cleanup started
- [ ] Module structure created
- [ ] First module converted

### Week 2
- [ ] Raylib modularized
- [ ] Build system updated
- [ ] Tests passing
```

## Getting Help

- Check `COMPREHENSIVE_UPGRADE_PLAN.md` for detailed phases
- Review existing code patterns
- Test frequently
- Commit often with clear messages

## Next Steps After Quick Start

Once you've completed the quick start:
1. Review `COMPREHENSIVE_UPGRADE_PLAN.md` for full roadmap
2. Continue with Phase 2-6 from the comprehensive plan
3. Set up CI/CD for automated testing
4. Begin developer tools implementation

---

**Remember**: This is an iterative process. Don't try to do everything at once. Focus on one module at a time, test frequently, and commit often.


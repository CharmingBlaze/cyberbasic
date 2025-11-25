# CyberBasic Refactor Checklist

Use this checklist to keep the refactor on track. Complete items in order per phase. CI must be green between phases.

## Phase 0 — Safety & Baseline

- [ ] Add duplicate detection to `FunctionRegistry::add()` (warn in Debug, fail in CI unless allowed list)
- [ ] Add a unit test for duplicate registration behavior
- [ ] Revert unsafe returns in `src/rt_raylib.gen.cpp` (no pointers to temporaries); use arrays/handles per PLAN.md
- [ ] Normalize bool/int returns in raylib bindings (use `from_bool`/`from_int`)
- [ ] Quick build and run smoke tests (MSYS2 MinGW-w64 + Ninja)

## Phase 1 — Modularity

- [ ] Split `src/builtins.cpp` into:
      - [ ] `src/builtins_core.cpp`
      - [ ] `src/builtins_console.cpp`
      - [ ] `src/builtins_graphics.cpp`
      - [ ] `src/builtins_audio.cpp`
- [ ] Move includes and registrations accordingly
- [ ] Ensure one registry initialization path in `main`/`interpreter` with clear order

## Phase 1.5 — Raylib Modularization (NEW)

- [ ] **Create modular directory structure**:
      - [ ] Create `src/raylib/` directory
      - [ ] Plan 15 modular .cpp files matching YAML categories
- [ ] **Update generator for modular output**:
      - [ ] Modify `tools/clean_generator.py` to create separate files
      - [ ] Generate `src/raylib/core_window.cpp` (31 functions)
      - [ ] Generate `src/raylib/basic_graphics.cpp` (14 functions)
      - [ ] Generate `src/raylib/colors.cpp` (28 functions)
      - [ ] Generate `src/raylib/basic_drawing.cpp` (10 functions)
      - [ ] Generate `src/raylib/input.cpp` (33 functions)
      - [ ] Generate `src/raylib/audio.cpp` (43 functions)
      - [ ] Generate `src/raylib/textures_images.cpp` (17 functions)
      - [ ] Generate `src/raylib/fonts_text.cpp` (17 functions)
      - [ ] Generate `src/raylib/3d_graphics.cpp` (17 functions)
      - [ ] Generate `src/raylib/advanced_graphics.cpp` (43 functions)
      - [ ] Generate `src/raylib/physics_animation.cpp` (6 functions)
      - [ ] Generate `src/raylib/shaders.cpp` (11 functions)
      - [ ] Generate `src/raylib/advanced_3d.cpp` (12 functions)
      - [ ] Generate `src/raylib/post_processing.cpp` (13 functions)
      - [ ] Generate `src/raylib/remaining_raylib.cpp` (14 functions)
- [ ] **Create header file**:
      - [ ] Create `src/raylib.hpp` including all modules
- [ ] **Update build system**:
      - [ ] Modify CMakeLists.txt for modular compilation
      - [ ] Ensure all 309+ functions compile in modular structure
- [ ] **Test modular system**:
      - [ ] Verify all functions work in new structure
      - [ ] Ensure no compilation errors
      - [ ] Test basic functionality

## Phase 2 — Ownership & Collisions

- [ ] Decide canonical owner for overlapping APIs (window, math, input)
- [ ] Adjust generator to skip/prefix conflicting symbols (e.g., `RL_` prefix)
- [ ] Remove or rename duplicates in modular raylib files
- [ ] Add generator-time collision report and CI check

## Phase 3 — Type Marshaling

- [ ] Enforce struct marshaling conventions:
      - [ ] Color as `[r,g,b,a]` ints
      - [ ] Vector2/3/4 as numeric arrays
      - [ ] Rectangle as `[x,y,w,h]`
- [ ] Add conversion helpers to reduce duplication
- [ ] Tests covering roundtrips and error paths

## Phase 4 — Lifecycle & RAII

- [ ] Implement `RaylibRuntimeGuard` for window/audio cleanup
- [ ] Make `CLOSEWINDOW`/`CLOSEAUDIO` idempotent and integrated with guard
- [ ] Verify Begin/EndDrawing pairing with assertions/logging

## Phase 5 — Legacy Cleanup

- [ ] Inventory legacy/unused code (grep, references, coverage)
- [ ] Mark candidates with `CYBERBASIC_LEGACY` guard and `DEPRECATED` comments
- [ ] Announce deprecation in CHANGELOG
- [ ] After one release, remove guarded code; keep history in Git

## Phase 6 — Tooling, Docs, CI

- [ ] Add clang-format and clang-tidy configs
- [ ] Add CONTRIBUTING.md (branches, commit style, code review, generators)
- [ ] Add docs on type marshaling and binding patterns under `docs/`
- [ ] CI pipeline (Ninja + MinGW) for build, unit, integration, generator-diff check

## Phase 7 — 100% Raylib Coverage

- [ ] Generate full coverage index vs raylib headers
- [ ] Fill missing functions with generator; ensure no collisions
- [ ] Add sample BASIC programs covering graphics, input, audio, files
- [ ] Tag release and publish binaries

## Continuous Policies

- [ ] No raw pointers through `Value`
- [ ] No silent duplicate registrations
- [ ] Small focused files; avoid god files
- [ ] Prefer explicit naming; document deviations

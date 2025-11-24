# CyberBasic Refactor and Architecture Plan

This plan defines how we will refactor CyberBasic into a modular, maintainable, DarkBasicPro-like indie game language with full raylib support while avoiding duplicates, "god files", and fragmentation.

## Vision

- A friendly BASIC-like language for indie game devs.
- 100% raylib coverage with consistent naming and types.
- Predictable, clean runtime lifecycle and resource management.
- Easy to contribute to without creating forks or architectural drift.

## Guiding Principles

- Simplicity first; prefer explicitness over magic.
- Single source of truth per API surface (no duplicates).
- Modular structure over monolith; small focused files.
- Strict type-marshaling conventions, never raw pointers.
- Tooling and CI enforce the above (style, tests, duplicate checks).
- Windows builds prefer MSYS2 MinGW-w64 + Ninja (no MSVC generator).

## Current Issues (to fix)

- Duplicate registrations between `src/builtins.cpp` and `src/rt_raylib.gen.cpp`.
- `FunctionRegistry::add()` silently overwrites duplicates.
- Inconsistent return typing (bools/ints returned via `from_number`).
- Returning addresses of temporaries (e.g., Color/Vector) — unsafe/invalid.
- Mixed concerns in `src/builtins.cpp` (core + console + graphics + audio).
- Lack of lifecycle guards for window/audio (resource risks).
- Legacy/unused code paths increase maintenance load.

## Architecture Overview

- Runtime Core
  - `include/bas/value.hpp`: dynamic types.
  - `include/bas/runtime.hpp`: `NativeFn`, `FunctionRegistry`, registry policies.
  - `src/interpreter.cpp` et al: execution engine.

- Builtins (High-Level Convenience)
  - Core: strings, arrays, math, file IO, random, time.
  - Console: ANSI/cursor helpers.
  - Optional High-Level Graphics/Audio helpers (sprite systems) separated from raw raylib.

- Raylib Bindings (Generated)
  - Thin, faithful wrappers to raylib C API only.
  - No high-level policy or global game state.
  - Naming: UPPERCASE; conflicts with builtins are avoided by generator config.

## Ownership and Namespacing

- Builtins own: general-purpose BASIC functions and CyberBasic helpers.
- Raylib layer owns: raw `raylib` symbols. If a name conflicts, generator either:
  - Skips it, or
  - Prefixes with `RL_` (configurable), or
  - Maps to an alternative agreed name.

## Type Marshaling Conventions

- Bool: `Value::from_bool`/`as_bool`.
- Int: `Value::from_int`/`as_int`.
- Float: `Value::from_number`/`as_number`.
- String: `Value::from_string`/`as_string`.
- Structs
  - Color: `[r:int, g:int, b:int, a:int]`.
  - Vector2: `[x:number, y:number]`; Vector3: `[x,y,z]`; Vector4: `[x,y,z,w]`.
  - Rectangle: `[x:number, y:number, w:number, h:number]`.
- No raw pointers through `Value`. Use arrays or stable integer handles with internal maps.

## Duplicate/Collision Policy

- `FunctionRegistry::add()` detects duplicates and logs clear diagnostics.
- Policy: error or warn depending on build mode. CI fails on unexpected duplicates.
- Generator emits a collision report and obeys skip/prefix rules.

## Legacy Code Policy

- Identify legacy/unused code via grep + coverage + references.
- Mark as deprecated for one release, behind `#ifdef CYBERBASIC_LEGACY`.
- Remove after deprecation window, with changelog notes.
- Never keep dead code; preserve via Git history if needed.

## File Layout Refactor

- Split `src/builtins.cpp` into:
  - `src/builtins_core.cpp` (strings/arrays/math/file/random/time)
  - `src/builtins_console.cpp` (ANSI/cursor)
  - `src/builtins_graphics.cpp` (sprites/textures/window helpers)
  - `src/builtins_audio.cpp` (audio device/sounds)

- **Modularize Raylib Bindings** (NEW):
  - Replace single `src/rt_raylib.gen.cpp` with modular structure:
    - `src/raylib/core_window.cpp` (31 functions)
    - `src/raylib/basic_graphics.cpp` (14 functions)
    - `src/raylib/colors.cpp` (28 functions)
    - `src/raylib/basic_drawing.cpp` (10 functions)
    - `src/raylib/input.cpp` (33 functions)
    - `src/raylib/audio.cpp` (43 functions)
    - `src/raylib/textures_images.cpp` (17 functions)
    - `src/raylib/fonts_text.cpp` (17 functions)
    - `src/raylib/3d_graphics.cpp` (17 functions)
    - `src/raylib/advanced_graphics.cpp` (43 functions)
    - `src/raylib/physics_animation.cpp` (6 functions)
    - `src/raylib/shaders.cpp` (11 functions)
    - `src/raylib/advanced_3d.cpp` (12 functions)
    - `src/raylib/post_processing.cpp` (13 functions)
    - `src/raylib/remaining_raylib.cpp` (14 functions)
  - Create `src/raylib.hpp` header including all modules
  - Update generator to create separate files per category
  - Update CMake to compile all modules
  - Benefits: easier maintenance, better organization, team development

## Lifecycle Management

- RAII guard created at interpreter startup to ensure:
  - Close window if open and frame not ended.
  - Unload textures/sprites; stop audio; close audio device.
- Idempotent close functions; safe on repeated calls.

## Generator Rules

- Emit only raw raylib mappings.
- Use strict type policies (bool→`from_bool`, ints→`from_int`).
- For structs, marshal to arrays per conventions.
- Collision avoidance: consult builtins list, then skip/prefix.

## Testing Strategy

- Unit tests: registry duplicate behavior; type conversions.
- Integration tests: small BASIC scripts exercising window, input, audio.
- Golden tests for generated bindings: presence/arity/type of key APIs.

## Tooling & CI

- clang-format, clang-tidy configs.
- GitHub Actions (or similar) pipeline:
  - Configure+build with Ninja (MSYS2 MinGW-w64 on Windows).
  - Run unit + integration tests.
  - Run generator and ensure no unexpected diffs.

## Roadmap (High-Level)

1) Safety net: duplicate detection, type fixes, revert pointer returns.
2) Split builtins; introduce RAII lifecycle guard.
3) Generator: enforce policies, resolve collisions.
4) **Modularize Raylib bindings** (NEW):
   - Update generator to create separate .cpp files per category
   - Create modular directory structure in src/raylib/
   - Update CMake build system for modular compilation
   - Ensure all 309+ functions work in modular structure
5) Add tests, docs, and style; wire CI.
6) Complete raylib coverage and stabilize APIs.
7) Legacy cleanup pass and 1.0 milestone.

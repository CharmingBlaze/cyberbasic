# CyberBasic C++ Coding Style

This project uses a pragmatic, modern C++ style aimed at clarity and maintainability.

## Language & Tooling

- C++17 or newer where available; avoid non-portable extensions.
- Build: MSYS2 MinGW-w64 + Ninja on Windows. Avoid MSVC/Visual Studio generator.
- Static analysis: clang-tidy; formatting: clang-format.

## Files & Modules

- Keep files small and focused (no god files).
- One primary responsibility per file. Split when a file grows beyond ~500 lines unless justified.
- Headers: minimal includes; prefer forward declarations.
- Do not place imports/includes mid-file; keep at top.

## Naming

- Files: `snake_case` for C++ sources (e.g., `builtins_core.cpp`).
- Types (classes/structs/enums): `PascalCase`.
- Functions/methods: `camelCase` for internal C++; BASIC-exposed names are UPPERCASE.
- Variables: `snake_case`.
- Constants/macros: `UPPER_SNAKE_CASE`.

## Error Handling

- Throw `std::runtime_error` with clear messages for script-visible errors.
- Validate argument counts and types at the binding boundary.
- Avoid exceptions for control flow internally.

## Memory & Ownership

- Prefer value types and RAII to manage resources.
- No raw owning pointers. Use `std::unique_ptr` or `std::shared_ptr` as needed.
- Never pass raw native pointers through `Value`. Use arrays or stable integer handles.

## Concurrency

- The interpreter and registry are single-threaded unless stated otherwise.
- If adding threads, encapsulate state and guard with mutexes; document invariants.

## Raylib Bindings

- Raw bindings mirror raylib C API with minimal glue.
- Use strict marshaling:
  - Bool → `Value::from_bool`
  - Int → `Value::from_int`
  - Float → `Value::from_number`
  - Color → `[r:int, g:int, b:int, a:int]`
  - Vector2/3/4 → numeric arrays
  - Rectangle → `[x:number, y:number, w:number, h:number]`
- No duplicates with builtins. If conflict, generator skips or prefixes with `RL_`.

## Builtins

- High-level helpers live in `builtins_*` modules, not in generated binding files.
- Keep APIs consistent and documented.

## Logging

- Use lightweight logging via `std::cerr` or a small logging utility. Avoid noisy logs in release builds.
- Duplicate registration warnings should include the function name and both sources.

## Tests

- Unit tests for core utilities and registry policies.
- Integration tests for BASIC programs exercising key features.

## Formatting

- Enforce via clang-format (LLVM style with project tweaks if any).
- No trailing whitespace; UNIX line endings preferred.

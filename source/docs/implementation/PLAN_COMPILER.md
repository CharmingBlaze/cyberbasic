# CyberBasic Modular Cross‑Platform Compiler Plan

This plan guides contributors to implement a modular, easy‑to‑distribute compiler/runtime for CyberBasic without duplication. It is pragmatic, incremental, and cross‑platform (Windows/macOS/Linux).


## Goals (non‑negotiable)
- [ ] Robust, modular architecture (no duplicate registrations, clear separation of concerns)
- [ ] Cross‑platform builds and packaging (Windows/macOS/Linux)
- [ ] Simple contributor workflow (clear checklists, CI validation)
- [ ] Backwards compatible with current BASIC programs and builtins
- [ ] Easy distribution as a portable folder/archive per OS


## High‑level Architecture
- [ ] Core executable hosts CLI, parser, AST, diagnostics, and runtime core.
- [ ] Feature modules (graphics, audio, console, generated raylib) are loaded dynamically (DLL/.so/.dylib).
- [ ] One registrar per module; all native functions register via `bas::FunctionRegistry`.
- [ ] Duplicate prevention enforced by `FunctionRegistry::add()`.
- [ ] Execution backend is swappable: Interpreter (existing), Bytecode VM (new), Optional JIT/AOT later.


## Module ABI
- [ ] Add `include/bas/module.hpp` with a stable C ABI:
  - `struct ModuleRegistrar { bas::FunctionRegistry& R; /* future helpers */ };`
  - `extern "C" bool bas_module_register_v1(ModuleRegistrar&)` (entry point)
  - `extern "C" const char* bas_module_info_v1()` (JSON/YAML string with name, semver, capabilities, min runtime ABI)
- [ ] Add `include/bas/platform.hpp` (path utils, dynamic loading abstraction)
- [ ] Implement `src/module_loader.cpp`:
  - Windows: `LoadLibraryW` + `GetProcAddress`
  - macOS/Linux: `dlopen` + `dlsym`
  - Search `modules/` dir and optional `--modules` extra dirs
  - Support `config/modules.json` allow/deny list


## Refactor Builtins into Modules (no duplicates)
- [ ] Convert `src/builtins_console.cpp` => `bas_console` module (DLL/.so/.dylib)
- [ ] Convert `src/builtins_graphics.cpp` => `bas_graphics` module
- [ ] Convert `src/builtins_audio.cpp` => `bas_audio` module
- [ ] Convert `src/rt_raylib.gen.cpp` => `bas_raylib_gen` module
- [ ] Keep only minimal bootstrap in core (if any). Ensure `src/main.cpp` loads modules before program execution.
- [ ] Validate that each module registers unique names; rely on duplicate throw to detect mistakes.


## Execution Backends
- Phase 1 (Default; simple & robust):
  - [ ] Keep current interpreter path as a baseline.
  - [ ] Add CLI `--backend interp|vm|llvm|c` (default `interp` until VM is ready).
- Phase 2 (Incremental performance):
  - [ ] Add Bytecode VM: `include/bas/ir.hpp`, `src/ir_lowering.cpp`, `include/bas/vm.hpp`, `src/vm.cpp`.
  - [ ] VM calls natives via `bas::call(registry, name, args)` to preserve modularity.
  - [ ] CLI default moves to `--backend vm` once tests pass.
- Phase 3 (Optional AOT/JIT):
  - [ ] C Transpiler + external compiler or libtcc for instant AOT (module: `bas_cjit`).
  - [ ] LLVM ORC JIT for best performance (module: `bas_llvmjit`).


## Cross‑Platform Build & Presets
- [ ] Add `CMakePresets.json` with Ninja presets: Debug, RelWithDebInfo, Release
- [ ] Set per‑OS output directories: binaries to `bin/`, modules to `modules/`
- [ ] Options: `-DBAS_BUILD_MODULES=ON`, `-DBAS_MODULES_OUTPUT_DIR=${binaryDir}/modules`
- [ ] Prefer static runtime where feasible; otherwise package redistributables
- [ ] Windows: MSVC or MinGW supported; macOS: Clang/universal optional; Linux: GCC/Clang


## Packaging (Portable)
- [ ] Scripts: `scripts/package.ps1` (Windows), `scripts/package.sh` (POSIX)
- [ ] Deliverables per OS:
  - `bin/cyberbasic[.exe]`
  - `modules/` with module DLLs/.so/.dylib
  - `examples/`, `tests/`, `docs/`, `licenses/`, `config/modules.json`
  - Any third‑party runtime DLLs/.so/.dylib (e.g., raylib)
- [ ] Strip symbols in Release; keep PDB/dSYM/DBG in `bin/symbols/` for RelWithDebInfo


## Runtime Options & Diagnostics
- [ ] CLI flags:
  - `--modules=<path;path>` additional search paths
  - `--no-modules` run core only
  - `--list-mods` discoverable modules and their info
  - `--backend=interp|vm|llvm|c`
  - `--debug-registry` dump registered names at startup (debug only)
- [ ] Log module load failures but continue unless `required` in config


## CI (Matrix)
- [ ] GitHub Actions/Azure Pipelines matrix: `windows-latest`, `macos-latest`, `ubuntu-latest`
- [ ] Configure + build with Ninja (RelWithDebInfo)
- [ ] Run unit/integration tests and sample BASIC programs
- [ ] Artifact packaging by OS (zip/tar.gz)


## Coding Standards & Best Practices
- [ ] Follow `docs/CODING_STYLE.md` and modern C++20 practices
- [ ] No duplicate registrations; one registrar per feature module
- [ ] Public headers under `include/bas/` only expose stable APIs/ABIs
- [ ] Use `std::filesystem` for cross‑platform paths
- [ ] Guard platform code with narrow `#ifdef`s inside `platform.cpp` only
- [ ] Keep functions small, focused; document pre/postconditions


## Migration & Cleanup
- [ ] Ensure `legacy_backup/` remains excluded from builds
- [ ] Remove any old monolithic registrars (already moved) from build scripts
- [ ] Update `README.md` with module loading and distribution instructions


## Milestones & Acceptance
- M1: Module loader + console/graphics/audio as modules
  - [ ] Loader works on all OSes; modules load and register successfully
  - [ ] All examples run; duplicate detection passes
- M2: Packaging per OS
  - [ ] Portable archives produced by scripts on CI for Win/macOS/Linux
- M3: VM backend (optional, recommended)
  - [ ] VM passes the same tests as interpreter; becomes default backend
- M4: Optional AOT/JIT backend(s)
  - [ ] At least one of `libtcc`/LLVM JIT integrated as module; examples run


## Implementation Checklist (Chronological)
1) Platform + Module ABI
- [ ] `include/bas/module.hpp` and `include/bas/platform.hpp`
- [ ] `src/module_loader.cpp` and `src/platform.cpp`
- [ ] `config/modules.json` loader logic

2) Convert Builtins to Modules
- [ ] Build shared libraries for `bas_console`, `bas_graphics`, `bas_audio`, `bas_raylib_gen`
- [ ] Update `src/main.cpp` to load modules at startup
- [ ] Verify registry contains expected names; no duplicates

3) Presets & Packaging
- [ ] Add `CMakePresets.json` (Ninja)
- [ ] Implement `scripts/package.ps1` and `scripts/package.sh`
- [ ] Ship `licenses/` and third‑party DLLs/so/dylib as needed

4) CI
- [ ] Matrix build + test + package artifacts

5) VM Backend (optional next)
- [ ] Add IR/VM files and `--backend` flag integration
- [ ] Achieve parity with interpreter on tests


## Notes & Decisions
- Runtime duplicate prevention stays as final arbiter (no silent overwrite)
- Generated raylib bindings remain namespaced uniquely to avoid overlaps with BASIC‑friendly helpers
- Keep architecture simple; prefer composition over inheritance; limit globals


---

Update this checklist as milestones are completed. Keep PRs small and focused, and include which checklist items they close.

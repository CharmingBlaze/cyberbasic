# CyberBasic: Product & Technical Plan

> Product name: **CyberBasic**.
> Status: draft v2. Last updated: 2026-11-25.

## Progress

| Phase | State | Done | Still open |
|-------|-------|------|------------|
| **0. Walking skeleton** | ✅ Done | Rust compiler → JS generators, Babylon runtime, `cyberbasic build/run` CLI, **in-browser playground** (compiler as WASM, ~160 KB) | Tauri/WebKitGTK rendering spike (§9.2) not run yet |
| **1. Language core** | 🟡 Mostly done | Functions (call before definition, recursion), `Global`/`Const`, locals by default, `For/Next/Step`, `For … = Each`, `Repeat/Until/Forever`, `Select/Case/Default`, `Exit`, `Dim` arrays (multi-dim, bounds-checked), `Type`/`Field`/`New`/`Delete`, `Mod`, `^`, `:` statement chains, single-line `If` owns its line, friendly errors with "did you mean", runtime errors with BASIC line numbers, frozen-loop guard. **Multi-file programs with `Include "file.cb"`** (Blitz-style splice, once per file, cycles ignored; errors, runtime errors and breakpoints name the file). 22 tests, including programs executed under Node. | Rowan CST + error recovery (stops at the first error today), type inference/annotations, namespaced modules (`Import`, beyond `Include`), `On` event handlers, coroutines, Salsa, 2,000-line compile benchmark |
| **2. Runtime & engine API** | 🟡 Mostly done | ~90 commands: 2D drawing (also a HUD over 3D), images, **sprite sheets with animation, rotation and scaling**, sound, keyboard, mouse, **gamepad**, `Delay`, math, strings, 3D entities/lights/camera, **glTF model loading**, **raycasts (`LinePick`)**, **Havok physics** (static/dynamic bodies, impulses, velocity, collision checks). Optional engine parts (Havok, glTF loader) load only when a game first uses them. **Demos: Pong ✅, Breakout ✅, Drift (sprites) ✅, Marble Run (3D physics + glTF, the Phase 2 exit demo) ✅** | Command manifest file (still a Rust table + parity test), tilemaps, physics shapes for loaded models, collision *events* (`On Collision`), 3D sound, screenshot tests |
| **3. IDE v1 + web export** | 🟡 Mostly done | **Desktop-style IDE** (`ide/`, Vite + React + Monaco + dockview): menu bar, run controls, status bar, command palette (Ctrl+Shift+P), toasts, start page with templates. **Workspaces** (Game, Code, Debug, Learn) that remember your layout changes. **Popup panels**: any panel group can float, pop out into its own window, or maximize. **Editor**: highlighting, live error squiggles, autocomplete with snippets and your own functions, hover docs, signature help. **Debugger**: breakpoints (gutter/F9), pause, step (into functions), continue, variables, call location, values on hover; the game freezes while paused. **Multi-file projects**: a tab per file, new/delete file (keeps `Include` lines in step), and go to definition, references and rename across files. **Code navigation**: Outline, go to definition, references, scope-aware rename, Format Document. Console, Problems, Reference (generated from the command table), Project panel with asset preview. Open/save real folders (File System Access API), **single-file web export** with assets embedded. Installable as an app window (PWA with its own title bar). **Desktop app** (`ide/src-tauri`, Tauri 2 on WebView2): frameless window with our own title bar, window buttons and a close guard for unsaved work; Windows installer (~3 MB). The command table now carries docs and feeds the compiler, editor and reference. | Linux/macOS desktop builds (the §9.2 WebKitGTK spike is still pending), pop-out windows in the desktop app, subfolders in projects, Salsa incremental analysis (navigation uses a line-based scan in the IDE today), dockview popout tested only in Chrome |
| **4. Visual scripting** | 🟡 Mostly done | **Node graphs** (`.graph` files), a Blueprint-style visual language: white execution wires set the order, data wires carry values. Nodes: On Start / On Frame, Branch, Sequence, For Loop, Get / Set variable, Math, Compare, And / Or / Not, Value, Code, and **one node per CyberBasic command** (generated from the command table). Editor built into the IDE: pan/zoom canvas, drag nodes, wire pins (drop a wire on empty space to create a connected node), inline values on unconnected inputs, searchable node palette, multi-select, duplicate, delete, fit view, undo/redo, Code view. Graphs compile to CyberBasic, so they run, export and debug like code: breakpoints on nodes, the paused node glows, errors and wiring problems (loops) show on the node. A graph can be the main file or be included from code. **Visual functions**: a Function node (name + inputs, which become pins) with a Return node; Call and Call (value) nodes; functions are shared across the project, so graphs call functions written in code and code calls functions built in graphs, and every function appears in the node palette. **Bouncer** template is made only of nodes and uses both kinds of function. **Behaviour graphs**: a graph switched to *Behaviour* is added to objects with an Add node (or `Behaviour_X_Attach` in code); On Attach / On Update / On Collision (with Other) / On Remove events with Self, freed objects drop their behaviours, per-object variables, all objects updated every frame through the new `EveryFrame` statement. **Behaviours (3D)** template. **Comment boxes** that group nodes (drag to move them together), **copy/cut/paste** of nodes, including between graphs. **Graph lowering lives in the Rust compiler** (`graph.rs`, with a small JSON reader): the IDE calls it through WASM (`cb_lower`) and the CLI builds `.graph` projects (`cyberbasic run main.graph`), so both produce the same code. | Animation state machines, the shared IR (visual files become BASIC today) |
| **5. Scene & creation tools** | 🟡 Mostly done | **Level editor** (`.scene` files): 3D view with orbit camera and grid, move/rotate/scale gizmos with snapping, object list, inspector (name, transform, color, physics, behaviours), cubes/spheres/planes/lights/cameras/glTF models, undo. **Terrain**: a terrain object (size, grid up to 256 squares a side) with Raise / Lower / Smooth / Flatten / Paint brushes (radius, strength, soft edge; vertex-color paint), saved in the scene and built by `CreateTerrain`; static mesh collider; `TerrainHeight`. **Terrain (3D)** template. **Modeller** (`.model` files): parts (box, sphere, cylinder, cone) and joints in a hierarchy, move/rotate/scale gizmos, inspector; **keyframe animation**: named clips (length, loop) on a timeline, posing a part at a time records a key, playback preview. The compiler lowers a model to `CreateModel_<Name>()`; `AnimateEntity` / `StopAnimating` / `EntityAnimation` (also for glTF models' own animations); levels can place models. The level editor and modeller share one 3D view (`viewport.ts`). **Robot (3D)** template. **Exit demo: Platformer (3D)** ✅ (a level from the level editor, a robot from the modeller with walk/idle/jump, a coin behaviour graph, rules in code); for it: `HideEntity` / `ShowEntity`, `KeepUpright`, `VelocityX/Y/Z`, Hidden and Keep upright in the level editor, and levels Include the models and behaviour graphs they use. The compiler lowers a scene to `LoadScene_<Name>` (so scenes work in the IDE, CLI and exports alike); `NameEntity` / `FindEntity` / `EntityName`. **Level (3D)** template. | Terrain texture layers (paint is vertex color today), mesh editing in the modeller (extrude, inset, bevel, loop cut), bones and skinning, glTF export of models, animation blending, prefabs, parenting, asset pipeline, animation state machines, timeline |
| 6. Desktop export & 1.0 | ⬜ Not started | | |

Known deviations from this plan, each marked with a `shortcut:` comment in the code: no shared IR yet (AST → JS directly), command table in Rust instead of `api/*.toml`, runtime in plain JS instead of TypeScript, 2D drawn with Canvas 2D rather than Babylon, Babylon.js loaded from its CDN (exports need internet).

---

## 1. Vision

A modern successor to Blitz3D, BlitzMax and AGK2: **one environment where you write games in a simple BASIC, or build them visually with nodes, or mix both.** It has a modern renderer, 2D and 3D in the same project, an integrated IDE, and one-click export of standalone games that don't need the editor.

**The Blitz promise, kept:** a spinning 3D cube in about 10 lines, running within 5 minutes of installing.

### Principles

1. **Simple first, powerful later.** A beginner never sees types, classes or async until they reach for them.
2. **One engine API, many front-ends.** Every engine command exists exactly once and shows up automatically as a BASIC command, a visual node, an autocomplete entry and a docs page.
3. **Text and visuals are equal.** Both compile to the same IR, use the same debugger and export the same way. Neither is the "real" one.
4. **Exports are real games.** The output is a standalone package (runtime + assets + compiled logic) with no editor dependency.
5. **Projects are plain files.** Text-based, diff-friendly and git-friendly. No binary project blobs.

### Target users

| User | Needs |
|------|-------|
| Beginners / hobbyists | Instant feedback, readable commands, great examples and docs |
| Returning Blitz/AGK users | Familiar procedural style, the `While … Sync … Wend` main loop |
| Visual-first creators | Node graphs with no typing required |
| Experienced devs | Types, modules, debugger, CLI builds, CI-friendly projects |

### Non-goals for 1.0

These are cut so 1.0 can actually ship. They are listed again in [§12](#12-post-10-backlog).

- Mobile export (iOS/Android)
- Networking / multiplayer
- AI decision graphs, behaviour trees and procedural graphs (LiteGraph)
- Plugin/extension API, asset store
- Source compatibility with legacy Blitz3D code (inspired by Blitz, not compatible with it)

---

## 2. What changed from v1 of this plan

These are the big corrections. Each one is explained in the section it links to.

| v1 said | Problem | v2 decision |
|---------|---------|-------------|
| `while … wend` game loop running on Babylon.js | A blocking loop freezes the browser. Babylon drives frames with `requestAnimationFrame`. | Compile the program to a **JS generator**. `Sync` becomes `yield`. See [§4.3](#43-the-main-loop-problem-and-its-solution). |
| Multi-word commands (`load sprite(...)`, `key down(...)`) | Ambiguous to parse, and they collide with user-defined names | **Single-word, case-insensitive commands** (`LoadSprite`, `keydown` both work). See [§4.1](#41-syntax-decisions). |
| "Shared API" as a principle | Nothing said how it is enforced | A **command manifest** generates compiler builtins, nodes, autocomplete and docs. See [§6](#6-the-command-manifest-the-shared-api-made-concrete). |
| Rete.js **and** LiteGraph.js **and** XState in 1.0 | Three graph systems means three UX models and three file formats | **Rete.js only** for 1.0; state machines compile to IR, no XState. LiteGraph only after 1.0 if it's still needed. See [§5](#5-visual-programming). |
| Logos + Chumsky + Rowan | Chumsky and Rowan don't fit together well | Logos + **hand-written recursive-descent parser** producing a Rowan CST (the rust-analyzer approach). See [§8](#8-technology-stack). |
| Tauri for exported games | Linux WebKitGTK has weak WebGL and no WebGPU. Games would render differently per OS. | **Electron runtime for game exports** by default. Tauri stays for the IDE, but gets validated in Phase 0. See [§9](#9-export--distribution). |
| Export is the last phase | "Editor not required" goes unproven until the end | **Web export ships in Phase 3**, desktop in Phase 6. |
| Build the full compiler before anything runs | Months with nothing to show; API mistakes found late | **Phase 0 walking skeleton**: typed code to a cube on screen in weeks. |
| Scene editor added in Phase 6 | Code-first (`CreateCube`) and editor-placed objects have no stated model for working together | **Behaviours + named entities** are designed in from the start. See [§4.4](#44-code-first-vs-scene-first). |

---

## 3. Three ways to program, one platform

| Mode | For | Tech | 1.0? |
|------|-----|------|------|
| **CyberBasic** | The main language and the product's identity | Rust compiler → IR → JS | ✅ |
| **Visual scripting** | Node graphs: gameplay, visual functions, entity behaviours, animation state machines | Custom node editor in the IDE (decided against Rete.js and event sheets) | ✅ |
| **Technical graphs** | AI decision graphs, behaviour trees, procedural generation, dataflow | Custom runtime; graph UI TBD (Rete or LiteGraph) | ❌ post-1.0 |
| **Material/shader graphs** | Materials | Babylon Node Material Editor (embedded, bundled offline) | ✅ |

---

## 4. CyberBasic language

### 4.1 Syntax decisions

| Decision | Choice | Why |
|----------|--------|-----|
| Command names | Single identifiers, **case-insensitive** (`LoadSprite` = `loadsprite`) | Parses unambiguously, matches Blitz, and user functions can't collide with half a command name |
| Call style | Statements need no parens (`Move cube, 0, 0, 5`); functions that return a value use parens (`x = MouseX()`) | Classic Blitz rule; beginners read it naturally |
| Method form | `cube.Move 0, 0, 5` is the same as `Move cube, 0, 0, 5` | Lets beginners write commands and OO users write methods. Both map to the same node |
| Keys & enums | Constants (`KEY_ESCAPE`), not strings (`"ESCAPE"`) | Checked at compile time, and autocomplete can list them |
| Comments | `'` (line) | BASIC standard |
| Blocks | `If/EndIf`, `While/Wend`, `For/Next`, `Function/End Function`, `Repeat/Until`, `Select/Case` | Familiar to Blitz/AGK users |
| Single-line If | `If KeyDown(KEY_LEFT) Then x -= 1` | Short code for short logic |
| Typing | Inferred by default, optional annotations (`speed: Float = 250`); a `Variant` escape hatch | Beginners never write a type; larger projects still get type safety |
| Arrays | 0-based, `Dim a[10]` gives 10 elements | Matches the JS runtime and modern expectations (Blitz's inclusive bound is a known beginner trap) |
| Frame timing | Built-in `Delta` (seconds since last frame) | Frame-rate-independent movement without boilerplate |
| Default scene | Implicit. `Sync` renders and presents | No `CreateScene`/`RenderScene` ceremony for the 90% case |

### 4.2 Examples

**2D**

```basic
Game "My First Game"
Graphics 1280, 720
SetFrameRate 60

player = LoadSprite("player.png")
player.x = 400
player.y = 300
speed = 250

While Not KeyHit(KEY_ESCAPE)
    If KeyDown(KEY_RIGHT) Then player.x += speed * Delta
    If KeyDown(KEY_LEFT)  Then player.x -= speed * Delta
    If KeyDown(KEY_SPACE) Then player.y -= speed * Delta

    Cls
    DrawSprite player
    Sync
Wend
```

**3D**

```basic
Game "My First 3D Game"
Graphics3D 1280, 720

camera = CreateCamera()
PositionEntity camera, 0, 5, -10
PointEntity camera, 0, 0, 0

light  = CreateLight(LIGHT_DIRECTIONAL)
cube   = CreateCube()
ground = CreatePlane(100)
PositionEntity cube, 0, 1, 0
PositionEntity ground, 0, -1, 0

While Not KeyHit(KEY_ESCAPE)
    If KeyDown(KEY_W) Then MoveEntity cube, 0, 0, 5 * Delta
    If KeyDown(KEY_S) Then MoveEntity cube, 0, 0, -5 * Delta
    TurnEntity cube, 0, 45 * Delta, 0
    Sync
Wend
```

**Types, functions, events, coroutines** (the "grow into it" features)

```basic
Type Enemy
    entity
    health: Int = 3
End Type

Function SpawnEnemy(x#, z#)
    e = New Enemy
    e.entity = CreateSphere()
    PositionEntity e.entity, x, 0, z
    Return e
End Function

On Collision player, Enemy             ' event handler, runs during Sync
    other.health -= 1
    If other.health <= 0 Then Delete other
End On

Coroutine FlashScreen()
    For i = 1 To 3
        SetClsColor 255, 255, 255 : Wait 0.1
        SetClsColor 0, 0, 0       : Wait 0.1
    Next
End Coroutine

assets = LoadAsync("level1.glb")      ' returns immediately
While Not assets.ready
    DrawText "Loading " + assets.progress + "%", 10, 10
    Sync
Wend
```

### 4.3 The main loop problem (and its solution)

Blitz programs own the loop: `While … Sync … Wend`. Browsers (and Babylon.js) don't allow that. The page must return control every frame or it freezes.

**Solution:** the compiler emits the program as a **generator function**. Every `Sync` (and `Wait`, and every `Coroutine`) compiles to `yield`. The runtime drives it from Babylon's render loop:

```
Babylon render loop (rAF)
  └─ each frame: update input → run event handlers & behaviours → main.next() → render
```

What this gives us, almost for free:

- Classic Blitz-style code runs unmodified in the browser and in desktop exports.
- **Pause / step / breakpoints** work by simply not resuming the generator (see [§7.3](#73-debugger)).
- Coroutines and `Wait` use the same mechanism.
- A guard can catch infinite loops that never reach `Sync` ("your loop has no Sync; the game froze at line 42") instead of hanging the tab.

### 4.4 Code-first vs scene-first

Both are supported, on top of one runtime model:

- **Code-first (Blitz style):** create everything in code (`CreateCube`). No scene file needed.
- **Scene-first (editor style):** objects are placed in the scene editor and saved to a scene file. Code finds them with `FindEntity("Player")`. **Behaviours** (BASIC files or graphs) attach to entities with `OnStart`, `OnUpdate`, `OnCollision`, etc.
- **Mixed:** a main BASIC program loads a scene (`LoadScene "level1"`) and drives it, while behaviours handle the per-object logic.

This is designed in from Phase 1 (behaviour entry points exist in the language) even though the scene editor arrives in Phase 5.

### 4.5 Feature table

| Category | Features | 1.0? |
|----------|----------|------|
| Fundamentals | Variables, constants, loops, conditions, `Select/Case` | ✅ |
| Data | Arrays, lists, maps, `Type` structures, `For Each` | ✅ |
| Functions | Functions, modules (`Import "utils"`), default params | ✅ |
| Types | Inference, optional annotations, `Variant` | ✅ |
| Objects | Types with methods, components on entities | ✅ (classes with inheritance: post-1.0) |
| Events | Input, collision, timers, custom signals | ✅ |
| Async | Coroutines, `Wait`, async loading | ✅ |
| 2D | Sprites, sprite sheets/animation, tilemaps, text, primitives | ✅ |
| 3D | Meshes, glTF loading, cameras, lights, PBR materials, skeletal animation | ✅ |
| Physics | Rigid bodies, colliders, raycasts (Havok) | ✅ |
| Audio | Sound, music, mixing, 3D positional audio | ✅ |
| Networking | HTTP | ✅ basic `HttpGet/Post` |
| Networking | WebSockets, multiplayer | ❌ post-1.0 |

---

## 5. Visual programming

### 5.1 Systems and ownership

| System | UI | Compiles to | 1.0? |
|--------|----|-------------|------|
| Visual functions | Rete.js | IR functions | ✅ |
| Entity behaviours | Rete.js | IR behaviour (OnStart/OnUpdate/…) | ✅ |
| Animation state machines | Rete.js (state/transition editor) | IR state machine, no XState at runtime | ✅ |
| Materials / shaders | Babylon Node Material Editor | Babylon NodeMaterial JSON | ✅ |
| Behaviour trees | TBD | IR + small BT runtime | ❌ |
| AI decision / utility graphs | TBD (LiteGraph candidate) | IR | ❌ |
| Procedural generation / dataflow | TBD (LiteGraph candidate) | IR | ❌ |

**Why only Rete for 1.0:** one graph library means one node UX, one file format, one graph→IR lowering pass and one debugger overlay. After 1.0 we decide on LiteGraph based on a real need (e.g. dataflow performance with thousands of nodes), not ahead of time.

### 5.2 The same operation, three ways

All three compile to the identical IR call `cmd:MoveEntity(player, 10, 0, 0)`:

```basic
MoveEntity player, 10, 0, 0
```

```
[On Every Frame] ──▶ [Move Entity]
                       entity: Player
                       x: 10  y: 0  z: 0
```

| Condition | Action |
|-----------|--------|
| Every frame | Move **Player** by (10, 0, 0) |

### 5.3 Text ↔ visual conversion

- **Graph → BASIC ("View as code"):** supported. Read-only generated BASIC, great for learning.
- **BASIC → graph:** not supported. Arbitrary code doesn't map cleanly to nodes, and a lossy converter would surprise users.
- **Mixing:** a graph node can call any BASIC function, and BASIC can call any visual function. They share one module namespace.

---

## 6. The command manifest: the shared API made concrete

Every engine command is declared **once** in a manifest (`api/*.toml`):

```toml
[command.MoveEntity]
category = "Transform"
subject  = "Entity"                       # enables `cube.MoveEntity ...` method form
params   = [{ name = "x", type = "Float" },
            { name = "y", type = "Float" },
            { name = "z", type = "Float" }]
returns  = "Void"
doc      = "Moves an entity relative to its own orientation."
example  = "MoveEntity cube, 0, 0, 5 * Delta"
since    = "0.1"
```

The implementation lives in the TypeScript runtime, keyed by name:

```ts
export const MoveEntity: Cmd = (e, x, y, z) => e.node.locallyTranslate(new Vector3(x, y, z));
```

**From that one entry we generate:**

| Consumer | Generated artifact |
|----------|--------------------|
| Compiler | Builtin signatures for name resolution and type checking |
| Visual editor | Node in the palette (category, typed sockets, defaults) |
| Monaco / LSP | Autocomplete, signature help, hover docs |
| Docs site | Command reference page with example |
| Runtime | Typed binding table |

**CI enforces it:** every manifest entry has an implementation, a test and an example, and every runtime export has a manifest entry. A command that exists in BASIC but not as a node (or the other way round) cannot be merged.

---

## 7. Architecture

### 7.1 Pipeline

```
  CyberBasic (.cb)                  Node graphs (.graph)
         │                                       │
   lex + parse (CST)                             │
         │                                       │
   name resolve + type check              graph → IR lowering
         │                                       │
         └───────────────┬───────────────────────┘
                         ▼
                ┌──────────────────┐     ◀── command manifest (api/*.toml)
                │  CYBERBASIC IR   │         drives builtins, nodes, docs
                │ typed, structured│
                │ + source spans   │         spans point to a BASIC line
                └────────┬─────────┘         OR a graph node id
                         ▼
                  JS codegen (ES2022 modules, generators, source maps)
                         ▼
                ┌──────────────────┐
                │CyberBasic Runtime│  TypeScript: command impls, input,
                │   (engine API)   │  audio, scene/behaviour system, debugger hooks
                └────────┬─────────┘
                         ▼
                   Babylon.js  (WebGL2 baseline, WebGPU when available; Havok physics)
                         ▼
                  Export pipeline ──▶ Web  |  Windows  |  Linux  |  macOS
```

### 7.2 Key decisions

- **The whole compiler is one Rust workspace, built twice:** as **WASM** (runs inside the IDE for instant compile-on-type and LSP) and as a **native CLI** (`cyberbasic build`, `cyberbasic run`, headless CI builds). Graph lowering lives in the same compiler, so CLI builds handle graph-based projects too.
- **The IR is structured, not bytecode:** functions, blocks, typed expressions and engine calls by manifest id. That's easy to generate readable JS from, easy to map back to source, and leaves room for a future WASM backend.
- **Every IR node carries a source span**, either a `(file, line, col)` or a `(graph, nodeId)`. Errors, breakpoints and profiling work the same for text and graphs; a runtime error in a graph highlights the node.
- **WebGL2 is the baseline, WebGPU an enhancement.** Babylon falls back automatically. Nothing in the API may require WebGPU.

### 7.3 Debugger

Debug builds insert lightweight statement hooks (`__dbg(spanId)`) into the generated code. Because the program is a generator ([§4.3](#43-the-main-loop-problem-and-its-solution)):

- **Breakpoint / pause:** the hook throws a "suspend" signal; the runtime stops resuming the generator but keeps rendering the frozen frame and accepting debugger commands.
- **Step / continue:** resume with a step mode set.
- **Variables:** codegen emits a scope map per span, so locals and globals show by their BASIC names.
- **Graphs:** the active node lights up; wires animate as values flow (debug mode only).
- Works in the IDE preview **and** in a debug export running on another machine (over WebSocket).

Release builds contain no hooks at all.

---

## 8. Technology stack

| Area | Choice | Notes |
|------|--------|-------|
| Lexer | **Logos** | Fast, derive-based |
| Parser | **Hand-written recursive descent → Rowan CST** | Lossless and error-resilient, ideal for IDE/LSP (the rust-analyzer approach). Replaces Chumsky. |
| Incremental analysis | **Salsa** (from Phase 3) | Fast re-checking while typing |
| Diagnostics | **Ariadne** (CLI), LSP diagnostics (IDE) | Same error data, two renderers |
| Serialization | **Serde** (+ `toml`, `serde_json`) | Manifest, project and graph files |
| LSP | **tower-lsp** logic compiled to WASM, connected to Monaco | No separate language-server process needed in the IDE |
| Runtime | **TypeScript + Babylon.js** | Tree-shaken ES module imports to keep exports small |
| Physics | **Havok** (Babylon plugin, WASM) | Check its license terms for redistribution in exports |
| Audio | **Babylon AudioEngineV2 / Web Audio API** | Spatial audio included |
| IDE UI | **React + Mantine + Monaco** | |
| Docking layout | **dockview** (instead of React Mosaic) | IDE-style tabbed docking, floating panels, saved layouts |
| State | **Zustand** | |
| Graph editor | **Rete.js v2** | |
| Desktop IDE shell | **Tauri 2** | Rust backend handles file watching, builds and packaging. Validate Linux rendering in Phase 0. |
| Game export shell | **Electron** (default), Tauri ("slim", opt-in) | See [§9](#9-export--distribution) |
| Async / parallel | **Tokio** (file watching, build server), **Rayon** (parallel asset processing) | |
| Asset pipeline | **glTF Transform** + **meshoptimizer**, **KTX2/Basis** textures | glTF Transform runs in JS inside the IDE, no Node install needed |
| Asset cache | **SQLite** (`rusqlite`) | Cache of imported/processed assets only. **Never** the source of truth for project data. |
| Repo tooling | Cargo workspace + pnpm workspace, Vitest, Playwright | |

---

## 9. Export & distribution

### 9.1 Targets

| Platform | Output | Phase |
|----------|--------|-------|
| **Web** | Static folder / zip: `index.html`, JS bundle, assets, Havok WASM. Itch.io-ready. | **Phase 3** |
| **Windows** | Portable folder + `.exe` (installer optional) | Phase 6 |
| **Linux** | AppImage + tarball | Phase 6 |
| **macOS** | `.app` bundle (signing/notarization documented; needs the user's Apple ID) | Phase 6 |
| Android / iOS | Capacitor wrapper around the web build | Post-1.0 |

### 9.2 Why Electron for exported games

Tauri uses the OS webview: WebView2 (Chromium) on Windows, WKWebView on macOS, **WebKitGTK on Linux**. WebKitGTK's WebGL performance and compatibility are noticeably worse and it has no WebGPU, so the same game would look and run differently per OS. For a **game** runtime, consistency beats a small download. Electron bundles one known Chromium everywhere. (Construct and GDevelop make the same choice.)

- **Default:** Electron export, same renderer on every OS.
- **Opt-in "slim" export:** Tauri on Windows/macOS for small downloads, with a warning on Linux.
- **The IDE** stays on Tauri if the Phase 0 spike shows the scene view runs acceptably on WebKitGTK. Otherwise the IDE moves to Electron too, and the Rust tooling runs as a sidecar binary. This is decided in Phase 0, not later.

### 9.3 Export contents

```
MyGame/
  MyGame.exe                (or .app / AppImage / index.html)
  resources/
    game.js                 compiled logic (minified, no debug hooks)
    runtime.js              tree-shaken CyberBasic runtime + Babylon
    havok.wasm              only if physics is used
    assets.pak              optimized assets (KTX2 textures, meshopt glTF, OGG audio)
```

---

## 10. IDE

### 10.1 Panels

| Panel | Purpose | Phase |
|-------|---------|-------|
| Code Editor | Monaco + CyberBasic LSP | 3 |
| Console | Logs, errors (clicking jumps to line/node) | 3 |
| Game Preview | Embedded running game, instant restart on save | 3 |
| Debugger | Breakpoints, call stack, variables, watch | 3 |
| Project / Assets | File tree, asset import, thumbnails | 3 / 5 |
| Command Reference | Searchable docs generated from the manifest | 3 |
| Visual Editor | Node graphs | 4 |
| Scene View | 2D/3D editing with gizmos | 5 |
| Hierarchy | Scene entities | 5 |
| Inspector | Entity properties, attached behaviours | 5 |
| Timeline | Animation and sequencing | 5 |
| Material Editor | Embedded Babylon NME | 5 |
| Profiler | Frame time, draw calls, per-function cost | 6 |

### 10.2 Workspace presets

A preset is a saved dockview layout plus a toolbar. It does not hide features.

`Game Dev` (default) · `Code` · `Visual Scripting` · `2D` · `3D` · `Animation` · `Materials`

### 10.3 Beginner experience (counts as a feature, not polish)

- **New Project wizard** with templates: Empty 2D, Empty 3D, Platformer, Top-down shooter, Breakout, First-person walk.
- **Examples browser**: dozens of small, runnable samples, one click to open (Blitz's samples were half the reason it succeeded).
- **F1 on any command** opens its docs and example.
- **Friendly errors**: "`MoveEnity` is not a command. Did you mean `MoveEntity`?" with the line highlighted.

---

## 11. Roadmap

Rough durations assume **1–2 full-time developers**. Each phase ends with something a user can try, and **has exit criteria**. Phases 1 and 2 overlap after the skeleton exists.

### Phase 0: Walking skeleton (3–4 weeks)
Prove the whole pipeline end to end, badly.
- Lexer/parser for a tiny subset: variables, `If`, `While`, calls, `Sync`
- IR v0 → JS generator codegen, compiled to WASM
- Runtime: `Graphics3D`, `CreateCube`, `CreateCamera`, `PositionEntity`, `TurnEntity`, `KeyDown`, `Sync`
- Manifest v0 with codegen for builtins
- A bare HTML page: textarea → compile → running cube
- **Spike:** Tauri on Linux (WebKitGTK) renders a Babylon scene. Measure it and make the IDE shell decision.
- **Exit:** the 3D example in §4.2 runs in a browser from typed source. Tauri/Electron decision recorded.

### Phase 1: Language core (2–3 months)
- Full grammar, error recovery, Rowan CST
- Name resolution, type inference, `Type`, arrays, lists, maps, modules
- Functions, coroutines, `On` event handlers, behaviour entry points
- Diagnostics with Ariadne. Golden-file tests for parser, type checker and codegen.
- CLI: `cyberbasic run file.cb` opens a browser window
- **Exit:** 200+ golden tests pass; a 2,000-line program compiles in under 100 ms.

### Phase 2: Runtime & engine API (2–3 months, overlaps Phase 1)
- ~150 manifest commands: 2D (sprites, animation, tilemaps, text), 3D (meshes, glTF, cameras, lights, materials), input (keyboard, mouse, gamepad), audio, Havok physics, collision events
- Fixed-timestep physics, variable `Delta`
- Screenshot regression tests (Playwright) for rendering commands
- **Exit:** three demo games written purely in CyberBasic: **Pong**, **Breakout**, **3D marble roller**.

### Phase 3: IDE v1 + web export (2–3 months)
- Tauri (or Electron) shell, dockview layout, project manager
- Monaco + WASM LSP: diagnostics, completion, hover, go-to-definition, rename
- Game preview with restart on save, console, **debugger** (breakpoints, step, variables)
- Command reference panel; examples browser; project templates
- **Web export** (static folder, works on itch.io)
- **Exit:** a new user goes from install → template → edited game → web export in under 10 minutes (tested with 5 people who haven't used it).

### Phase 4: Visual scripting (3 months)
- Rete.js editor with nodes generated from the manifest
- Graph → IR lowering in the compiler; graph errors and breakpoints on nodes
- Node graphs, visual functions, behaviours, "View as code"
- **Exit:** Breakout rebuilt with **zero lines of BASIC**, and a hybrid project (BASIC main + graph behaviours) debugs cleanly across both.

### Phase 5: Scene & creation tools (3–4 months)
- Scene view with gizmos, hierarchy, inspector, prefabs, scene file format (the **level editor**)
- **Terrain** in the level editor: heightmap terrain object with sculpt (raise, lower, smooth, flatten) and paint (texture layers) brushes; physics collider from the heightmap
- **3D modeller**: box-modelling (extrude, inset, bevel, loop cut), vertex colors and UVs, saved as glTF so the rest of the pipeline is unchanged; **animation**: keyframes on a timeline, bones and skinning, clips played from code (`AnimateEntity`)
- Asset import pipeline (glTF Transform, meshopt, KTX2), thumbnails, hot reload of assets
- Animation state machines, timeline, embedded Node Material Editor, physics setup in the inspector
- **Exit:** a small 3D platformer level built in the scene editor, with behaviours in both BASIC and graphs.

### Phase 6: Desktop export & 1.0 (2 months)
- Windows / Linux / macOS exports (Electron default, Tauri slim option)
- Profiler; performance pass against the targets in §13
- Docs site, getting-started tutorial, 30+ examples
- **Exit:** 1.0 release. All §13 targets met. A game made in CyberBasic is published on itch.io for all four platforms.

**Total to 1.0: roughly 14–20 months** for a small team. Phase 0 is the cheapest place to discover the plan is wrong, so it comes first and stays short.

---

## 12. Post-1.0 backlog

Roughly ordered by expected value:

1. Mobile export (Capacitor: Android AAB, iOS)
2. Behaviour trees + AI decision graphs
3. WebSockets + simple multiplayer (rooms, state sync)
4. Procedural / dataflow graphs (evaluate LiteGraph vs Rete here)
5. Classes with inheritance, interfaces
6. Plugin API (custom commands = manifest entries + TS impl, so the shared-API design already supports it)
7. Legacy `.b3d` model import and a Blitz3D migration helper
8. WASM codegen backend for hot paths
9. Asset / template store

---

## 13. Success metrics & quality gates

| Metric | Target |
|--------|--------|
| Spinning cube from a blank project | ≤ 10 lines, < 5 min from install |
| Compile time | < 100 ms per 10k lines (incremental, in IDE) |
| Edit → running preview | < 1 s |
| Web export size (no assets, no physics) | < 3 MB gzipped |
| 2D performance | 5,000 sprites at 60 fps on a mid-range laptop (integrated GPU) |
| 3D performance | 500 physics bodies at 60 fps, same machine |
| API coverage | 100% of manifest commands have impl + test + example + node (CI-enforced) |
| Crash-free sessions (IDE) | > 99% |

**Testing strategy**
- Compiler: golden tests (source → CST/diagnostics/IR/JS snapshots), fuzzing the parser
- Runtime: unit tests per command; Playwright screenshot tests for rendering
- Visual: graph → IR snapshot tests; round-trip "View as code" tests
- End-to-end: demo games run headless in CI for N frames with scripted input, asserting state
- Export: CI builds every example for web + all three desktop platforms

---

## 14. Open questions

| Question | Options | Leaning |
|----------|---------|---------|
| License / business model | Fully open source · open-core (paid exports/templates) · paid | Open-source compiler + runtime grows adoption. Decide before Phase 3. |
| Comment syntax | `'` only, or also `;` for Blitz habits | `'` only |
| Array indexing | 0-based vs Blitz-style inclusive `Dim` | 0-based (decided in §4.1, revisit after user tests) |
| Browser-hosted IDE | Ship a web version of the IDE too? | Nearly free, since the compiler already runs in WASM. Consider after Phase 3. |
| Havok redistribution | Confirm license terms for bundling in commercial exports | Check in Phase 2. Fallback: Ammo.js / Rapier (WASM). |

---

## 15. Risks

| Risk | Impact | Mitigation |
|------|--------|------------|
| Scope is too large for a small team | Never ships | Hard 1.0 non-goals (§1); every phase ships something usable |
| Generator-based loop costs performance | Slower games | Benchmark in Phase 0; generators are cheap per frame (one `next()`), and hot paths are plain functions |
| Linux webview rendering | Inconsistent IDE/games | Electron for exports; Phase 0 spike decides the IDE shell |
| Visual and text features drift apart | Visual feels second-class | Manifest + CI parity check (§6) |
| Babylon.js API changes | Runtime breakage | Runtime is the only layer touching Babylon; pin versions; screenshot tests |
| Debugger complexity | Late, flaky debugger | Hook-based design (§7.3) built in Phase 3 on the generator model, not bolted on |
| Beginners bounce off | No adoption | Phase 3 exit criterion is a real user test; examples and templates are roadmap items, not extras |

---

## 16. Repository layout

```
cyberbasic/
├─ crates/
│  ├─ cyberbasic-syntax/      lexer (Logos), parser, Rowan CST
│  ├─ cyberbasic-hir/         name resolution, type checking (Salsa)
│  ├─ cyberbasic-ir/          IR definition + lowering from HIR
│  ├─ cyberbasic-graph/       graph & event-sheet JSON → IR
│  ├─ cyberbasic-codegen-js/  IR → JS generators + source maps
│  ├─ cyberbasic-manifest/    reads api/*.toml, generates builtins
│  ├─ cyberbasic-lsp/         language server logic
│  ├─ cyberbasic-wasm/        WASM bindings for the IDE
│  ├─ cyberbasic-cli/         `cyberbasic build|run|export`
│  └─ cyberbasic-export/      packaging for web/desktop
├─ api/                    command manifest (*.toml), the single source of truth
├─ runtime/                TypeScript runtime on Babylon.js
├─ ide/                    React IDE (Monaco, Rete, dockview)
├─ ide-shell/              Tauri (or Electron) host
├─ examples/               runnable sample projects (also CI tests)
├─ templates/              New Project templates
└─ docs/                   docs site (command reference generated)
```

---

## 17. The one-paragraph summary

CyberBasic is the product's identity. A Rust compiler (built as WASM for the IDE and native for the CLI) turns BASIC **and** visual graphs into one typed IR, then into JavaScript generators that run on a TypeScript runtime over Babylon.js. A single **command manifest** defines every engine command once and generates the BASIC builtins, visual nodes, autocomplete and docs, so text and visual programming can never drift apart. Rete.js powers visual scripting in 1.0. The React IDE runs in Tauri. Games export to the web from Phase 3 and to desktop (on Electron, for consistent rendering) at 1.0. Build a walking skeleton first, prove each layer with real demo games, and keep mobile, networking and AI graphs out of 1.0.

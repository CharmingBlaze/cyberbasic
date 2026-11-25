<p align="center"><img src="cyberbasic.png" alt="CyberBasic" width="420"></p>

# CyberBasic

A modern BASIC for making 2D and 3D games, in the spirit of Blitz3D and AGK2. You write in a simple BASIC, or later build games visually with nodes. Everything runs on a modern renderer (Babylon.js), and finished games export without the editor.

> **Status: early development.** This is a full rewrite; the old C++/raylib interpreter is in this repo's git history. Phase 0 is done and the core language works. Progress and roadmap are in [PLAN.md](PLAN.md).

```basic
Game "Pong-ish"
Graphics 800, 600

Global score

Function Paddle(y)
    If KeyDown(KEY_UP) Then y -= 400 * Delta
    If KeyDown(KEY_DOWN) Then y += 400 * Delta
    Return Max(0, Min(510, y))
End Function

While Not KeyHit(KEY_ESCAPE)
    y = Paddle(y)
    Cls
    Rect 30, y, 14, 90
    Text 10, 10, "Score: " + score
    Sync
Wend
```

## Quick start

Requires [Rust](https://rustup.rs) and a browser.

```sh
cargo run --release -- run examples/pong.cb
```

This compiles `examples/pong.cb` into `examples/build/pong/` and opens it. Use `build` instead of `run` to only write the files. The output folder is a standalone web game (`index.html`, `runtime.js`, `game.js`, plus an `assets/` folder if one sits next to your `.cb` file).

Projects made with nodes build the same way: `cargo run --release -- run examples/behaviours/main.graph`. The main file's folder is the project, so graphs and code in it can call each other's functions.

> Games load Babylon.js from its CDN, so for now they need an internet connection.

### IDE

A desktop-style editor with a game preview, debugger and workspaces. Needs [Node.js](https://nodejs.org) and Rust with the WebAssembly target.

```sh
rustup target add wasm32-unknown-unknown
cd ide
npm install
npm run dev
```

Open <http://localhost:5173> in Chrome or Edge (opening and saving folders needs their File System Access API). In Chrome you can **Install** it from the address bar to get its own window.

#### Desktop app

The same IDE in its own frameless window (Tauri, using Edge WebView2 on Windows):

```sh
cd ide
npm run desktop       # builds ide/dist and the app; the installer lands in target/release/bundle/nsis/
npm run desktop:dev   # runs it against the dev server, with reload
```

The app itself is `target/release/cyberbasic-ide.exe`. Linux and macOS builds use the same commands but haven't been tested yet.

- **Run** with F5. The game runs in the Game panel, scaled to fit.
- **Debug**: click the gutter or press F9 for a breakpoint; F6 pauses, F10 steps (into functions too), F5 continues. The Debugger panel shows every variable in scope.
- **Workspaces** (Game, Code, Debug, Learn) rearrange the panels; your changes to each are remembered. *Workspace → Reset* restores the default.
- **Popup panels**: every panel group has buttons to float it, pop it out into its own window, or maximize it.
- **Node graphs** (visual scripting): *File → New File…* and name it `something.graph`, or start from the **Bouncer** template. Right-click or press Space to add nodes, drag from a pin to wire them (white wires set the order, blue wires carry values), and type values into unconnected inputs. Every CyberBasic command is a node. A **Function** node defines your own function (its inputs become pins, a **Return** node gives back a value); **Call** nodes run it, from any graph, and code can call it too (graphs can likewise call functions written in code). Breakpoints work on nodes, errors show on the node, and *Code* shows the CyberBasic the graph becomes.
- **Behaviour graphs**: switch a graph from *Script* to *Behaviour* and it becomes something you add to objects (start from the **Behaviours (3D)** template). It has **On Attach**, **On Update**, **On Collision** (with the **Other** object; needs physics) and **On Remove** events with a **Self** pin (freeing the object also removes its behaviours), its variables belong to each object separately, and every other graph gets an **Add &lt;Name&gt;** node for it. In code, `Behaviour_Spinner_Attach cube` does the same.
- **Level editor**: *File → New File…* and name it `level.scene`, or start from the **Level (3D)** template. Add cubes, spheres, planes, lights, cameras and models from `assets/`; move, rotate and scale them with gizmos (W / E / R, Snap for grid steps); set name, color, physics (static or dynamic) and behaviours in the inspector. Code builds the level with `LoadScene_Level` (after `Graphics3D`) and finds objects with `FindEntity("Player")`.
- **Terrain**: add a *Terrain* in the level editor, select it and pick a brush: **Raise**, **Lower**, **Smooth**, **Flatten** or **Paint** (left-drag on the ground; right-drag orbits). Set its size and grid in the inspector. Terrains are static physics colliders with their real shape, and `TerrainHeight(terrain, x, z)` tells code how high the ground is. Try the **Terrain (3D)** template.
- **Modeller**: *File → New File…* and name it `hero.model` (it starts as a little robot), or try the **Robot (3D)** template. Build models from boxes, spheres, cylinders and cones hung from **joints** (invisible pivots: shoulders, hips) so parts turn around them. Switch to **Animate** to make clips: pick a time on the timeline and pose parts; each pose becomes a key (◆), and Play previews it. In code, `hero = CreateModel_Hero()` makes one (or place it in a level), `AnimateEntity hero, "walk"` plays a clip, `StopAnimating` and `EntityAnimation(hero)` too. `AnimateEntity` also plays the animations glTF models come with.
- **Canvas tools**: Ctrl+C / Ctrl+X / Ctrl+V copy nodes (with the wires between them) and paste at the mouse, also into another graph; Ctrl+D duplicates. **C** (or the *Comment* button) puts a comment box around the selected nodes: drag its title to move it with everything inside, double-click to rename, drag the corner to resize.
- **Ctrl+Shift+P** opens the command palette: every action, every template, and docs for every command.
- **File → Export Web Game** downloads one `.html` file with your game and its assets inside.

### Playground

Edit and run CyberBasic in the browser; the compiler runs there as WebAssembly.

```sh
rustup target add wasm32-unknown-unknown
cargo build --release -p cyberbasic --lib --target wasm32-unknown-unknown
python -m http.server 8765
```

Then open <http://localhost:8765/playground/>. **Ctrl+Enter** runs the code.

## Examples

| File | Shows |
|------|-------|
| [examples/cube.cb](examples/cube.cb) | 3D camera, light, meshes, HUD text |
| [examples/pong.cb](examples/pong.cb) | Two-player 2D game: functions, globals, constants, collision |
| [examples/breakout.cb](examples/breakout.cb) | `Type`s, `For … Each`, `Delete`, arrays, mouse control |
| [examples/drift.cb](examples/drift.cb) | Animated sprite sheet, rotation, screen wrap |
| [examples/marble.cb](examples/marble.cb) | Havok physics, a glTF model, raycast ground check, follow camera |
| [examples/shooter/](examples/shooter/main.cb) | A game split across files with `Include` (`cargo run --release -- run examples/shooter/main.cb`) |
| [examples/level/](examples/level/main.cb) | A level from the level editor (`level.scene`): walls, physics crates, spinning gems to collect |
| [examples/terrain/](examples/terrain/main.cb) | Roll a marble over sculpted, painted hills (`terrain.scene`); gems placed with `TerrainHeight` |
| [examples/robot/](examples/robot/main.cb) | A robot from the modeller (`robot.model`) walking around a level, switching between walk and idle animations |
| [examples/behaviours/](examples/behaviours/main.graph) | Node graphs only: two behaviour graphs added to seven cubes (open it in the IDE) |

## How it works

```
file.cb ──▶ Rust compiler ──▶ game.js ──▶ CyberBasic runtime ──▶ Babylon.js / Canvas 2D
           (lex, parse, codegen)  (JS generators)   (runtime/runtime.js)
```

The program and every function compile to JavaScript generators. `Sync`, `Delay` and loading commands `yield`, and the runtime resumes the program once per frame, so classic `While … Sync … Wend` game loops run in the browser without freezing it. A loop that never reaches `Sync` is stopped after 5 seconds with an error.

## Language

```basic
' comment
x = 10 : y = 20              ' `:` puts several statements on one line
name$ = "Ada"                ' Blitz suffixes (# % $) are accepted
Const SPEED = 300, SIZE = 32
Global lives = 3, score      ' visible inside functions

If x > 5 Then Print "big" : x = 0 Else Print "small"   ' everything after Then belongs to the If
If x = 1
    ...
ElseIf x = 2
    ...
Else
    ...
EndIf                        ' or End If

For i = 1 To 10 Step 2 : Print i : Next
While x < 100 : x += 1 : Wend
Repeat : x -= 1 : Until x = 0
Repeat ... Forever           ' leave any loop with Exit

Select key
Case 1, 2
    ...
Default
    ...
End Select

Include "enemies.cb"         ' splices in another file (once), path relative to the project folder
EveryFrame MoveEnemies       ' runs a function (with no inputs) at every Sync

Function Add(a, b)           ' can be called before it is defined
    Return a + b             ' variables inside are local unless Global
End Function

Dim grid[10, 10]             ' 0-based, bounds-checked
grid[2, 3] = 1

Type Enemy
    Field x, y, hp
End Type
e = New Enemy : e.hp = 3
For e = Each Enemy
    If e.hp <= 0 Then Delete e
Next
```

- Operators: `+ - * / Mod ^`, `= <> < <= > >=`, `And Or Not`, `+=`, `-=`. `+` joins strings.
- Commands and keywords are case-insensitive. `Delta` is the seconds since the last frame.
- `End` stops the program from anywhere.

## Commands

| Area | Commands |
|------|----------|
| Setup | `Game title`, `Graphics w, h`, `Graphics3D w, h` |
| 2D drawing | `Cls`, `ClsColor r,g,b`, `Color r,g,b`, `Rect x,y,w,h[,fill]`, `Oval x,y,w,h[,fill]`, `Line x1,y1,x2,y2`, `Plot x,y`, `Text x,y,value`, `GraphicsWidth()`, `GraphicsHeight()` |
| Images | `LoadImage(path)`, `LoadAnimImage(path, frameW, frameH, first, count)`, `DrawImage img,x,y[,frame]`, `DrawImageEx img,x,y,angle,scale[,frame]` (centered), `ImageWidth(img)`, `ImageHeight(img)`. Frame numbers wrap around. |
| Sound | `LoadSound(path)`, `PlaySound snd` |
| Input | `KeyDown(key)`, `KeyHit(key)`, `MouseX()`, `MouseY()`, `MouseDown([button])`, `MouseHit([button])` (1 left, 2 right, 3 middle), `JoyX()`, `JoyY()`, `JoyDown(button)` |
| Time / output | `Delay ms`, `Millisecs()`, `Print value` (browser console) |
| Math | `Abs`, `Sqr`, `Sin`, `Cos`, `Tan`, `ATan2` (degrees), `Floor`, `Ceil`, `Int`, `Float`, `Min`, `Max`, `Rnd([min,] max)`, `Rand([min,] max)`, `RectsOverlap(x1,y1,w1,h1, x2,y2,w2,h2)` |
| Strings | `Str`, `Len`, `Left`, `Right`, `Mid(s, start[, n])`, `Upper`, `Lower`, `Chr`, `Asc`, `Instr`, `Replace`, `Trim` (positions start at 1) |
| 3D | `CreateCamera()`, `CreateLight([LIGHT_DIRECTIONAL \| LIGHT_POINT])`, `CreateCube([size])`, `CreateSphere([size])`, `CreatePlane([size])`, `CreateTerrain([size, divisions, heights, colors])`, `TerrainHeight(t, x, z)`, `PositionEntity`, `MoveEntity`, `RotateEntity`, `TurnEntity`, `PointEntity`, `ScaleEntity` (all `e, x, y, z`), `EntityColor e,r,g,b`, `FreeEntity e`, `EntityX/Y/Z(e)`, `EntityDistance(a, b)`, `EntityExists(e)` (false once freed), `CreateModel(data)` (use `CreateModel_<Name>()`), `AnimateEntity e, name[, speed]`, `StopAnimating e`, `EntityAnimation(e)`, `NameEntity e, name`, `FindEntity(name)` (or 0), `EntityName(e)`, `AmbientLight r,g,b`, `LoadMesh(path)` (.glb/.gltf), `LinePick(x,y,z, dx,dy,dz)` (mesh hit or 0), `EntityPickMode e, 0\|1` |
| Physics | `EnablePhysics [gx, gy, gz]`, `EntityPhysics e, mass[, bounce]` (mass 0 = static; spheres get a sphere shape, others a box), `ApplyImpulse e,x,y,z`, `SetVelocity e,x,y,z`, `EntityCollided(a[, b])` (started touching in the last step), `CollidedWith(e)` (what it touched, or 0) |

`LoadImage`, `LoadAnimImage`, `LoadSound`, `LoadMesh`, `EnablePhysics` and `Delay` wait without freezing the game: frames keep rendering until they finish. Havok physics and the glTF loader are downloaded only when a game first uses them.

Keys: `KEY_A`–`KEY_Z`, `KEY_0`–`KEY_9`, `KEY_ESCAPE`, `KEY_SPACE`, `KEY_ENTER`, `KEY_LEFT`, `KEY_RIGHT`, `KEY_UP`, `KEY_DOWN`.

## Project layout

```
crates/cyberbasic/   compiler + CLI (Rust, no dependencies); also builds to WASM
runtime/             runtime.js and the index.html page template
ide/                 desktop-style IDE (React, Monaco, dockview) using the WASM compiler
playground/          minimal in-browser editor
examples/            sample games
PLAN.md              product plan, roadmap and progress
```

## Development

```sh
cargo test
```

Tests cover parsing, code generation and error messages. If Node.js is installed, they also compile small programs, run them against the real runtime and check the output. A parity test makes sure every compiler command exists in the runtime.

## License

See [LICENSE](LICENSE).

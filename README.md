<p align="center"><img src="cyberbasic.png" alt="CyberBasic" width="420"></p>

# CyberBasic

A modern BASIC for making 2D and 3D games, in the spirit of Blitz3D and AGK2. You write in a simple BASIC, or later build games visually with nodes. Everything runs on a modern renderer (Babylon.js), and finished games export without the editor.

> **Status: Phase 0 (walking skeleton).** This is a full rewrite. The old C++/raylib interpreter is in this repo's git history. The roadmap is in [PLAN.md](PLAN.md).

```basic
Game "My First 3D Game"
Graphics3D 1280, 720

camera = CreateCamera()
PositionEntity camera, 0, 5, -10
PointEntity camera, 0, 0, 0

light = CreateLight(LIGHT_DIRECTIONAL)
cube  = CreateCube()
EntityColor cube, 0, 200, 255

While Not KeyHit(KEY_ESCAPE)
    If KeyDown(KEY_W) Then MoveEntity cube, 0, 0, 5 * Delta
    TurnEntity cube, 0, 45 * Delta, 0
    Sync
Wend
```

## Quick start

Requires [Rust](https://rustup.rs) and a browser.

```sh
cargo run --release -- run examples/cube.cb
```

This compiles `examples/cube.cb` to `examples/build/cube/` and opens it. `build` instead of `run` only writes the files. The output folder is a standalone web game: `index.html`, `runtime.js` and `game.js`.

> Phase 0 games load Babylon.js from its CDN, so they need an internet connection.

## How it works

```
file.cb ──▶ Rust compiler ──▶ game.js ──▶ CyberBasic runtime ──▶ Babylon.js
           (lex, parse, codegen)  (JS generator)   (runtime/runtime.js)
```

The `While … Sync … Wend` loop compiles to a JavaScript generator: each `Sync` is a `yield`, and the runtime resumes the program once per frame. That way classic BASIC game loops run in the browser without freezing it.

## Language (Phase 0 subset)

- Variables, `=`, `+=`, `-=`, numbers, strings, `.field` access
- `If … Then … [Else …]` on one line, or block `If / ElseIf / Else / EndIf` (`End If` also works)
- `While … Wend`, `Sync`, `End`
- Operators: `+ - * /`, `= <> < <= > >=`, `And Or Not`
- `'` comments, `:` to put several statements on one line
- Commands are case-insensitive (`MoveEntity` = `moveentity`)
- `Delta`: seconds since the last frame

### Commands

| Command | Description |
|---------|-------------|
| `Game title$` | Sets the window title |
| `Graphics3D width, height` | Opens the 3D view |
| `CreateCamera()` | Creates a camera |
| `CreateLight([type])` | `LIGHT_DIRECTIONAL` (default) or `LIGHT_POINT` |
| `CreateCube([size])`, `CreateSphere([size])`, `CreatePlane([size])` | Primitive meshes |
| `PositionEntity e, x, y, z` | Sets the absolute position |
| `MoveEntity e, x, y, z` | Moves relative to the entity's own facing |
| `RotateEntity e, pitch, yaw, roll` | Sets the absolute rotation (degrees) |
| `TurnEntity e, pitch, yaw, roll` | Rotates relative to the current rotation (degrees) |
| `PointEntity e, x, y, z` | Faces a point |
| `EntityColor e, r, g, b` | Mesh color, 0–255 |
| `KeyDown(key)`, `KeyHit(key)` | Key held / pressed this frame |
| `Print value` | Writes to the browser console |

Keys: `KEY_A`–`KEY_Z`, `KEY_0`–`KEY_9`, `KEY_ESCAPE`, `KEY_SPACE`, `KEY_ENTER`, `KEY_LEFT`, `KEY_RIGHT`, `KEY_UP`, `KEY_DOWN`.

## Project layout

```
crates/cyberbasic/   compiler + CLI (Rust, no dependencies)
runtime/             runtime.js and the index.html page template
examples/            sample programs
PLAN.md              product and technical plan
```

## Development

```sh
cargo test
```

The tests cover parsing, code generation, error messages, and a check that every compiler command exists in the runtime.

## License

See [LICENSE](LICENSE).

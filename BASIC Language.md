# RCBASIC Language & API Reference (Draft v1)

> **Scope**: This document lists the **RCBASIC language keywords** and the **standard library / game API** you ship with the compiler (refactor edition). It’s split into the core language, standard utilities, and the **raylib-powered game API** (2D/3D). All names are shown uppercase for clarity; the compiler treats keywords case-insensitively.

---

## 1) Lexical & Syntax Basics

**Comments**

* Single-line: `'` or `REM`
* Block comments (optional, recommended): `/* ... */`

**Identifiers**

* Letters, digits, `_`; cannot start with digit.
* Case-preserving; comparisons are case-sensitive for identifiers (recommended) but **keywords** are case-insensitive.

**Literals**

* Integers: `0`, `-42`
* Floats: `3.14`, `-0.5`, `1.0e-3`
* Strings: `"Hello"` (UTF‑8)
* Booleans: `TRUE`, `FALSE`
* Null pointer/empty: `NULL`

**Line Continuation**

* Use space + `_` at end of line to continue: `PRINT "Hello" _
  ", world"`

**Statement Separator**

* Newline ends a statement; optional `:` also allowed as a separator.

---

## 2) Types

* `BOOL` (1 byte)
* `INTEGER` (32‑bit signed)
* `LONG` (64‑bit signed)
* `SINGLE` (32‑bit float)
* `DOUBLE` (64‑bit float)
* `STRING` (UTF‑8)
* `POINTER` (native pointer; interop)
* Game value types (exposed by stdlib): `COLOR`, `VECTOR2`, `VECTOR3`, `RECT`, `MATRIX`, `QUATERNION`

**String Suffix**

* Functions returning strings and variables may use `$` suffix (e.g., `NAME$`).

**Arrays & Collections**

* Fixed and dynamic arrays; `REDIM` supports `PRESERVE`.
* Maps/dictionaries exposed via stdlib (`MAP_*`).

---

## 3) Declarations & Scope

**Variables**

* `DIM x AS INTEGER`
* `DIM name$ AS STRING`
* `DIM a(10) AS DOUBLE` (0..10 inclusive by default; or 0..9 if `OPTION BASE 0`)

**Dynamic Arrays**

* `REDIM a(100)` optionally `PRESERVE` to keep contents.

**Assignments (LET and indexed)**

* Simple: `LET x = 10` or `x = 10`.
* Indexed arrays support both forms:
  * `a[i] = value`
  * `LET a[i] = value`
* Multi-dimensional indices are supported: `LET m[i][j] = value`.

**Constants**

* `CONST PI AS DOUBLE = 3.141592653589793`

**Scope Modifiers**

* `GLOBAL`, `LOCAL`, `STATIC` (for persistent locals)

**User Types (UDT / Records)**

```
TYPE PLAYER
  Name$ AS STRING
  HP AS INTEGER
  Pos AS VECTOR3
END TYPE
```

---

## 4) Operators & Precedence (high → low)

1. Member `.` (for UDT fields), array index `()`
2. Unary: `+`, `-`, `NOT`
3. Power: `^`
4. Multiply/Divide: `*`, `/`, integer `\`
5. Add/Sub: `+`, `-`
6. Modulo: `MOD`
7. Comparison: `=`, `<>`, `<`, `<=`, `>`, `>=`
8. Logical AND/OR/XOR: `AND`, `OR`, `XOR`

String concatenation uses `+`.

---

## 5) Control Flow

**If**

```
IF cond THEN
  ...
ELSEIF other THEN
  ...
ELSE
  ...
END IF
```

**Select Case**

```
SELECT CASE x
  CASE 0
  CASE 1 TO 5
  CASE 6, 9, 12
  CASE ELSE
END SELECT
```

**Loops**

```
FOR i = 0 TO 10 STEP 2
  ...
NEXT

WHILE cond
  ...
WEND

DO WHILE cond
  ...
LOOP

DO UNTIL cond
  ...
LOOP
```

**Early exits**: `EXIT FOR`, `EXIT WHILE`, `CONTINUE` (optional)

**Goto / Labels** (discourage in style guide)

```
GOTO MyLabel
MyLabel:
```

---

## 6) Procedures & Functions

**Subroutines**

```
SUB DoThing(a AS INTEGER, BYREF b AS DOUBLE)
  ...
END SUB
```

**Functions**

```
FUNCTION Add(a AS INTEGER, b AS INTEGER) AS INTEGER
  RETURN a + b
END FUNCTION
```

* `BYVAL` (default) / `BYREF`
* Optional/default params supported: `b AS INTEGER = 10`
* `EXIT SUB`, `EXIT FUNCTION`

**Modules (namespacing)**

```
MODULE Utils
  PUBLIC SUB Hello()
  END SUB
END MODULE

IMPORT Utils
Utils.Hello()
```

---

## 7) Error Handling

**Structured**

```
TRY
  ...
CATCH e$
  PRINT "Error: ", e$
FINALLY
  ...
END TRY
```

**Legacy (optional)**

```
ON ERROR GOTO Handler
...
Handler:
RESUME NEXT
```

**Raising**: `THROW "Bad stuff"`

---

## 8) Preprocessor

```
#DEFINE DEBUG
#UNDEF DEBUG
#IFDEF DEBUG
  PRINT "Debug build"
#ELSE
  PRINT "Release"
#ENDIF
#INCLUDE "path/to/file.bas"
#PRAGMA OPTION BASE 0
```

---

## 9) Core Standard Library (non-graphics)

### 9.1 Strings

* `LEN(s$) → INTEGER`
* `LEFT$(s$, n) → STRING`
* `RIGHT$(s$, n) → STRING`
* `MID$(s$, start, length) → STRING`
* `INSTR(hay$, needle$, start=1) → INTEGER`
* `REPLACE$(s$, from$, to$) → STRING`
* `TRIM$(s$)`, `LTRIM$(s$)`, `RTRIM$(s$)`
* `UCASE$(s$)`, `LCASE$(s$)`
* `SPLIT$(s$, delim$) → STRING[]`
* `JOIN$(arr$, delim$) → STRING`
* `FORMAT$(fmt$, args...) → STRING`
* `VAL(s$) → DOUBLE` / `STR$(num) → STRING`
* `CHR$(code) → STRING` / `ASC(ch$) → INTEGER`

### 9.2 Math & Random

* `ABS(x)`, `SGN(x)`, `SQRT(x)`, `EXP(x)`, `LOG(x)`
* `POW(x, y)`, `MIN(a, b)`, `MAX(a, b)`, `CLAMP(x, lo, hi)`
* `SIN(x)`, `COS(x)`, `TAN(x)`, `ASIN(x)`, `ACOS(x)`, `ATAN(x)`, `ATAN2(y, x)`
* `CEIL(x)`, `FLOOR(x)`, `ROUND(x)`
* `LERP(a, b, t)`
* `RANDOMIZE seed` / `RND() → 0..1` / `RNDINT(lo, hi)`

### 9.3 Arrays & Maps

* `ARRAY_LEN(a) → INTEGER`
* `REDIM a(newSize)` / `REDIM PRESERVE a(newSize)`
* `PUSH a, value` / `POP a → value`
* `INSERT a, index, value` / `REMOVE a, index`
* `SORT a, ascending=TRUE`
* `MAP_CREATE() → MAP`
* `MAP_SET(m, key$, value)` / `MAP_GET(m, key$, default)` / `MAP_HAS(m, key$)`
* `MAP_REMOVE(m, key$)` / `MAP_KEYS(m) → STRING[]` / `MAP_VALUES(m) → ANY[]`

### 9.4 Filesystem & I/O

* `OPEN file$ FOR INPUT|OUTPUT|APPEND|BINARY AS #n`
* `CLOSE #n` / `CLOSE ALL`
* `INPUT #n, var` / `LINE INPUT #n, s$`
* `PRINT #n, value1, value2, ...`
* `EOF(#n) → BOOL` / `LOF(#n) → LONG` / `SEEK #n, pos`
* `FILE_EXISTS(file$) → BOOL` / `FILE_DELETE(file$)` / `FILE_COPY(src$, dst$)`
* `DIR_CREATE(path$)` / `DIR_DELETE(path$)` / `DIR_LIST(path$) → STRING[]`
* `CHDIR path$` / `CURDIR$() → STRING`
* `PATH_JOIN(a$, b$) → STRING`

### 9.5 System & Time

* `SHELL cmd$ → INTEGER` (exit code)
* `ENV_GET$(key$) → STRING` / `ENV_SET key$, value$`
* `CLIPBOARD_SET$(s$)` / `CLIPBOARD_GET$() → STRING`
* `SLEEP ms`
* `TIME_NOW() → DOUBLE (seconds)` / `DATE_NOW$() → STRING`
* `UNIX_TIME() → LONG`
* `TIMER_START id$` / `TIMER_ELAPSED(id$) → DOUBLE`

### 9.6 JSON (optional stdlib)

* `JSON_PARSE$(s$) → VALUE` (dynamic)
* `JSON_STRINGIFY$(value, pretty=FALSE) → STRING`

---

## 10) Game API (raylib-powered)

> The following are **helper commands** that wrap raylib and common game patterns. They are grouped by manager. Unless noted, they are available to both 2D & 3D projects.

### 10.1 App / Window

* `APP_INIT width, height, title$, targetFps` – Create window, set target FPS.
* `APP_SET_VSYNC onOff` – Toggle vsync.
* `APP_SET_FULLSCREEN onOff` – Toggle fullscreen.
* `APP_SET_RESOLUTION width, height` – Resize window.
* `APP_SET_ICON path$` – Set window icon from image file.
* `APP_SHUTDOWN` – Close window and free resources.

### 10.2 Input

* `INPUT_BIND action$, keyOrButton` – Map a named action (e.g., "JUMP") to a key/pad.
* `INPUT_UNBIND action$` – Remove a binding.
* `INPUT_AXIS_BIND axis$, negKey, posKey` – Map e.g. MOVE\_X to A/D.
* `INPUT_READ(action$) → DOUBLE` – Get action value (0/1 or -1..1).
* `INPUT_GAMEPAD_DEADZONE padIndex, value` – Set deadzone.
* `INPUT_IS_DOWN(keyOrButton) → BOOL` – Immediate state.
* `INPUT_WAS_PRESSED(keyOrButton) → BOOL` – Edge press.
* `INPUT_TEXT_START` / `INPUT_TEXT_STOP` – Enable/disable text input.

### 10.3 Time / Loop

* `TIME_DELTA() → DOUBLE` – Seconds since last frame.
* `TIME_SCALE setScale` – Multiply delta (slow‑mo).
* `TIMER_AFTER seconds, event$` – Fire event in future.

### 10.4 Assets / Resources

* `ASSET_LOAD_TEXTURE id$, file$` – Cache texture.
* `ASSET_LOAD_SOUND id$, file$` – Cache SFX.
* `ASSET_LOAD_MUSIC id$, file$` – Cache music stream.
* `ASSET_LOAD_FONT id$, file$, size` – Cache font.
* `ASSET_LOAD_MODEL id$, file$` – Cache 3D model.
* `ASSET_GET_TEXTURE(id$) → HANDLE` – Retrieve handle/index.
* `ASSET_UNLOAD id$` / `ASSET_UNLOAD_ALL` – Free.

### 10.5 Scene / State

* `SCENE_PUSH name$` – Push new scene on stack.
* `SCENE_REPLACE name$` – Replace current scene.
* `SCENE_POP` – Pop scene.
* `SCENE_SET_TRANSITION type$, seconds` – Fade/wipe/iris.
* `SCENE_BLACKBOARD_SET key$, value$` / `SCENE_BLACKBOARD_GET(key$)` – Shared data.

### 10.6 Entity (Simple)

* `ENTITY_CREATE prefab$, x, y, z → INTEGER` – Spawn and return id.
* `ENTITY_DESTROY id`
* `ENTITY_SET id, key$, value` / `ENTITY_GET(id, key$)` – Properties.
* `ENTITY_TAG id, tag$` / `ENTITY_FIND_BY_TAG(tag$) → id`

### 10.7 Event Bus

* `EVENT_SUBSCRIBE event$, handlerId$` – Register handler (label/sub).
* `EVENT_UNSUBSCRIBE event$, handlerId$`
* `EVENT_FIRE event$, payload$` – Broadcast simple payload (e.g., JSON text).

### 10.8 Audio

* `AUDIO_PLAY_SFX id$, volume, pitch` – Play one-shot sound.
* `AUDIO_PLAY_MUSIC id$, volume` – Start/loop music.
* `AUDIO_STOP_MUSIC` – Stop music.
* `AUDIO_SET_BUS_VOLUME bus$, volume` – "SFX","MUSIC","UI".
* `AUDIO_SET_SFX_POSITION id$, x, y, z` – 3D panning helper.

### 10.9 UI / HUD (simple)

* `UI_BEGIN` / `UI_END` – Begin/End UI pass.
* `UI_LABEL x, y, text$` – Draw text.
* `UI_BUTTON x, y, w, h, text$ → BOOL` – True when clicked.
* `UI_IMAGE x, y, textureId$` – Draw sprite/texture.

### 10.10 Save / Load

* `SAVE_WRITE slot, key$, value$` – Save string/JSON.
* `SAVE_READ(slot, key$) → STRING` – Load.
* `SAVE_AUTOSAVE` – Commit current profile.
* `SAVE_DELETE slot` – Remove slot.

### 10.11 Localization

* `LOC_LOAD_TABLE file$` – Load CSV/JSON strings.
* `LOC_SET_LANGUAGE code$` – e.g., "en".
* `LOC_GET(key$) → STRING` – Fetch localized text.

### 10.12 Camera (shared)

* `CAMERA_SHAKE amount, decay` – Trauma‑based shake.
* `CAMERA_SET_BOUNDS left, top, right, bottom` – 2D bounds or 3D region.
* `CAMERA_FOLLOW entityId, lerp` – Smooth follow.

### 10.13 Debug & Tools

* `DEBUG_SHOW_OVERLAY onOff` – FPS, timings, draw calls.
* `DEBUG_LOG text$`
* `DEBUG_DRAW_LINE x1, y1, x2, y2, color`
* `DEBUG_DRAW_BOX x, y, w, h, color`

### 10.14 Tween / Curves

* `TWEEN_TO target$, property$, toValue, seconds, easing$`
* `TWEEN_STOP target$, property$`

### 10.15 Particles / VFX

* `FX_EMITTER_CREATE id$, configFile$` – Define emitter.
* `FX_EMIT id$, x, y, z, count` – Spawn particles.
* `FX_STOP id$` – Stop emission.

### 10.16 Post‑Processing / Shaders

* `POSTFX_ADD effect$, configFile$` – Enable effect chain.
* `POSTFX_REMOVE effect$`
* `POSTFX_CLEAR` – Remove all.

### 10.17 Streaming / Chunks

* `STREAM_SET_GRID cellSizeX, cellSizeY`
* `STREAM_SET_RADIUS cells`
* `STREAM_PIN_PLAYER entityId`
* `STREAM_TICK` – Call each frame to load/unload regions.

### 10.18 Replay / Record (optional)

* `REPLAY_START_RECORD file$`
* `REPLAY_STOP_RECORD`
* `REPLAY_PLAY file$`
* `REPLAY_STOP`

### 10.19 Build / Config

* `CFG_SET key$, value$` / `CFG_GET(key$)` – Profiles, flags.

---

## 11) 2D Game API

### 11.1 Sprites / Atlas

* `SPRITE_LOAD_ATLAS atlasId$, jsonMeta$, textureFile$` – Load atlas & metadata.
* `SPRITE_DRAW atlasId$, region$, x, y` – Draw region.
* `SPRITE_COLOR rgba` – Tint next draws.
* `SPRITE_FLIP hFlip, vFlip` – Mirroring.

### 11.2 2D Animation

* `ANIM2D_LOAD animId$, jsonMeta$, atlasId$` – Load clips.
* `ANIM2D_PLAY animId$, clip$` – Start clip.
* `ANIM2D_SET_RATE animId$, rate` – Speed.
* `ANIM2D_DRAW animId$, x, y` – Draw current frame.

### 11.3 Tilemaps

* `TILEMAP_LOAD mapId$, file$` – TMX/LDtk/CSV.
* `TILEMAP_DRAW mapId$, layer$` – Render layer.
* `TILEMAP_GET_COLLISION(mapId$) → colliderId` – Use with physics.
* `TILEMAP_GET_TILE(mapId$, layer$, tx, ty) → INTEGER`

### 11.4 2D Physics (pluggable: Physac/Box2D)

* `PHYS2D_SET_BACKEND name$` – "physac" or "box2d".
* `PHYS2D_BODY_CREATE id$, type$, x, y, w, h` – Types: static/dynamic/kinematic.
* `PHYS2D_BODY_SET id$, key$, value` – Mass, friction, restitution, gravityScale.
* `PHYS2D_RAYCAST x1, y1, x2, y2 → STRING` – Hit payload (JSON) or empty.
* `PHYS2D_STEP fixedDt` – Fixed step update.
* `PHYS2D_ON_CONTACT event$, handlerId$` – "enter", "stay", "exit".

### 11.5 2D Collision‑Only (kinematic)

* `COL2D_SET_TILE_COLLIDER colliderId`
* `COL2D_MOVE x, y, vx, vy, outX, outY, hits` – Swept AABB resolve.
* `COL2D_SET_ONEWAY onOff` / `COL2D_SET_SLOPES onOff`

### 11.6 Projectiles / Bullets (2D)

* `BULLET2D_SPAWN x, y, vx, vy, damage, life`
* `BULLET2D_SET_LAYER layer$`
* `BULLET2D_CLEAR`

### 11.7 Pathfinding 2D (Grid)

* `PATH2D_BUILD_FROM_TILEMAP mapId$, passableLayer$`
* `PATH2D_FIND sx, sy, gx, gy → pathId`
* `PATH2D_NEXT(pathId) → nx, ny, done`

### 11.8 2D Lighting

* `LIGHT2D_ADD id$, x, y, radius, intensity`
* `LIGHT2D_REMOVE id$`
* `LIGHT2D_SET_AMBIENT r, g, b`

### 11.9 Parallax / Background

* `PARALLAX_ADD layerId$, textureId$, speedX, speedY`
* `PARALLAX_DRAW` – Render layers back‑to‑front.

### 11.10 Dialogue / Quest

* `DIALOGUE_LOAD file$` – Nodes & lines.
* `DIALOGUE_START nodeId$`
* `DIALOGUE_CHOOSE optionIndex`
* `QUEST_SET key$, state$` – inactive/active/done.

### 11.11 Combat / Stats

* `STAT_SET entityId, key$, value`
* `STAT_GET(entityId, key$) → value`
* `DAMAGE_APPLY sourceId, targetId, amount, type$`

### 11.12 Inventory / Loot

* `INV_ADD entityId, itemId$, count`
* `INV_REMOVE entityId, itemId$, count`
* `INV_EQUIP entityId, slot$, itemId$`
* `LOOT_DROP x, y, tableId$`

---

## 12) 3D Game API

### 12.1 3D Camera Controllers

* `CAM3D_SET_MODE mode$` – "fps","tps","orbit","free".
* `CAM3D_SET_TARGET x, y, z`
* `CAM3D_SET_POSITION x, y, z`
* `CAM3D_SET_FOV degrees`
* `CAM3D_UPDATE_FROM_INPUT` – WASD/mouse/gamepad.

### 12.2 3D Physics (pluggable: Bullet/Jolt/ODE)

* `PHYS3D_SET_BACKEND name$`
* `PHYS3D_BODY_CREATE id$, type$, shape$, x, y, z, sx, sy, sz` – shape: box, sphere, capsule, mesh.
* `PHYS3D_BODY_SET id$, key$, value` – Mass, friction, restitution.
* `PHYS3D_RAYCAST ox, oy, oz, dx, dy, dz, dist → STRING` – Hit JSON or empty.
* `PHYS3D_CHAR_CREATE id$, radius, height, stepOffset`
* `PHYS3D_CHAR_MOVE id$, vx, vy, vz`
* `PHYS3D_STEP fixedDt`

### 12.3 Models / Materials

* `MODEL_LOAD id$, file$`
* `MODEL_DRAW id$, x, y, z, rx, ry, rz, scale`
* `MAT_SET modelId$, matIndex, key$, value` – color, metal, roughness.

### 12.4 Skeletal Animation

* `SKEL_LOAD modelId$, animFile$`
* `SKEL_PLAY modelId$, clip$, loop`
* `SKEL_SET_RATE modelId$, rate`

### 12.5 NavMesh & Pathfinding (3D)

* `NAVMESH_LOAD id$, file$`
* `NAVMESH_FIND id$, sx, sy, sz, gx, gy, gz → pathId`
* `NAVMESH_NEXT(pathId) → nx, ny, nz, done`

### 12.6 Weapons / Combat (3D)

* `WEAPON_DEF_ADD id$, kind$` – rifle/pistol/rocket.
* `WEAPON_EQUIP entityId, weaponId$`
* `WEAPON_FIRE entityId` – Hitscan/projectile.
* `WEAPON_RELOAD entityId`

### 12.7 Spawner / Waves

* `SPAWN_POINT_ADD id$, x, y, z`
* `SPAWN_ENTITY pointId$, prefab$, count, cooldown`
* `WAVE_START planId$`

### 12.8 Cover / Perception

* `AI_ADD_COVER_POINT x, y, z, nx, ny, nz`
* `AI_SEE(sourceId, targetId) → BOOL`
* `AI_HEAR(sourceId, x, y, z, loudness)`

### 12.9 IK / Procedural

* `IK_LOOK_AT entityId, tx, ty, tz`
* `IK_HAND_ATTACH entityId, bone$, itemId$`
* `IK_FOOT_GROUNDING entityId, onOff`

### 12.10 Atmosphere / Environment

* `ENV_SET_SKYBOX textureId$`
* `ENV_SET_FOG r, g, b, density`
* `ENV_SET_TIME_OF_DAY hours`
* `WEATHER_SET type$` – clear/rain/snow.

### 12.11 Decals / Surface Impacts

* `DECAL_ADD textureId$, x, y, z, nx, ny, nz, size, life`
* `DECAL_CLEAR`

---

## 13) AI (2D & 3D)

### 13.1 Behavior (FSM/BT)

* `AI_SET_BACKEND name$` – fsm/bt/utility.
* `AI_FSM_DEFINE stateId$, onEnter$, onUpdate$, onExit$`
* `AI_FSM_SET entityId, stateId$`
* `AI_BT_LOAD entityId, file$` – Load behavior tree.
* `AI_BLACKBOARD_SET entityId, key$, value` / `AI_BLACKBOARD_GET(entityId, key$)`

### 13.2 Steering / Locomotion

* `STEER_SEEK entityId, tx, ty, tz`
* `STEER_ARRIVE entityId, tx, ty, tz, slowRadius`
* `STEER_AVOID entityId, onOff`
* `STEER_FOLLOW_PATH entityId, pathId`

### 13.3 Tactical

* `TACTIC_SET_TARGETING entityId, rule$` – nearest/lowestHP/threat.
* `TACTIC_REQUEST_COVER(entityId) → coverId$`
* `TACTIC_SET_RETREAT_THRESHOLD entityId, hpPercent`

### 13.4 Senses

* `SENSE_SET_VISION entityId, degrees, range`
* `SENSE_CAN_SEE(entityId, targetId) → BOOL`
* `SENSE_HEAR_EVENT x, y, z, loudness, team$`

---

## 14) Polishing & UX

### 14.1 Camera FX

* `CAMFX_SHAKE trauma, decay`
* `CAMFX_ZOOM targetFov, seconds`
* `CAMFX_TILT degrees, seconds`

### 14.2 Animation Events

* `ANIM_EVENT_BIND entityId, clip$, frame, event$`
* `ANIM_EVENT_CLEAR entityId, clip$`

### 14.3 Feedback

* `FEEDBACK_HITSTOP seconds`
* `FEEDBACK_RUMBLE strength, seconds`
* `FEEDBACK_DAMAGE_NUMBER x, y, amount`

### 14.4 Accessibility

* `ACC_SET_COLORBLIND mode$` – none/protan/deutan/tritan.
* `ACC_SET_AIM_ASSIST strength`
* `ACC_SET_SUBTITLES onOff`

### 14.5 Minimap / Map

* `MAP_SET_BOUNDS left, top, right, bottom`
* `MAP_REVEAL_RECT x, y, w, h`
* `MAP_ADD_MARKER id$, x, y, icon$`
* `MAP_REMOVE_MARKER id$`

---

## 15) Reserved Keywords (lexer)

`AND, AS, ASC, BINARY, BOOL, BYREF, BYVAL, CASE, CATCH, CEIL, CHDIR, CHR, CLOSE, CONST, CONTINUE, COS, DOUBLE, DO, ELSE, ELSEIF, END, ENDIF, ENDSELECT, END SUB, END FUNCTION, ENV, EXIT, EXP, FALSE, FINALLY, FLOOR, FOR, FUNCTION, GOTO, IF, IMPORT, INPUT, INTEGER, LCASE, LEFT, LEN, LOCAL, LOG, LONG, LOOP, LTRIM, MAP, MID, MODULE, NEXT, NOT, ON, OPTION, OR, OUTPUT, PRINT, RANDOMIZE, REDIM, REM, REMOTE, RETURN, RIGHT, ROUND, RTRIM, SELECT, SHELL, SINGLE, SLEEP, SQRT, STEP, STRING, SUB, TAN, THEN, THROW, TIMER, TO, TRUE, TRY, TYPE, UCASE, UNTIL, WEND, WHILE, WRITE, XOR`

> *Note:* Keep this list synced with the parser. Add/remove as the feature set evolves.

---

## 16) Standard Drawing Primitives (Optional Convenience)

These wrap raylib drawing calls for quick tooling and debug. In production, prefer sprite/model systems above.

* `DRAW_PIXEL x, y, color`
* `DRAW_LINE x1, y1, x2, y2, color`
* `DRAW_RECT x, y, w, h, color` / `FILL_RECT x, y, w, h, color`
* `DRAW_CIRCLE x, y, radius, color` / `FILL_CIRCLE x, y, radius, color`
* `DRAW_TRIANGLE x1, y1, x2, y2, x3, y3, color` / `FILL_TRIANGLE ...`
* `DRAW_TEXT text$, x, y, size, color` / `MEASURE_TEXT(text$, size) → VECTOR2`
* 3D helpers: `DRAW_CUBE x, y, z, sx, sy, sz, color` / `DRAW_SPHERE x, y, z, r, color` / `DRAW_CYLINDER ...` / `DRAW_PLANE ...`

---

## 17) Project Templates (Macros)

* `PACK_METROIDVANIA_INIT` – Sets 2D camera/parallax/platformer helpers.
* `PACK_TOPDOWN_INIT` – Twin‑stick controls, grid pathfinding, minimap.
* `PACK_SHOOTER3D_INIT` – FPS camera, character controller, recoil/spread defaults.

---

## 18) Notes for Implementers (Parser/Lexer)

* **Case**: Keywords case‑insensitive; identifiers case‑preserving (treat as case‑sensitive in symbol table).
* **String suffix**: Accept `$` suffix for string vars & fns; also allow `AS STRING` explicit typing.
* **Arrays**: Support `OPTION BASE 0|1`; default to 0 for modern ergonomics.
* **Numbers**: Allow underscores for readability (e.g., `1_000_000`).
* **Escapes**: `\n`, `\t`, `\"`, `\\` in strings.
* **Numeric literal suffixes** (optional): `L` for 64‑bit, `F` for SINGLE, `D` for DOUBLE.
* **Error model**: Exceptions for structured `TRY/CATCH`; provide legacy `ON ERROR` switch only if needed.
* **Foreign function** (future): Design `DECLARE EXTERN` to bind C symbols for power users.

---

**This spec is a living document.** When you add/remove features, update the keyword list and the command tables above so the compiler and docs never drift.

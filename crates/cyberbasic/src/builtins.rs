// The command table: the single place a command is declared. It drives name resolution and
// arity checks in the compiler, and (via `manifest_json`) autocomplete, hover docs and the
// command reference in the IDE. A test checks the runtime implements every entry.
// shortcut: lives in Rust rather than api/*.toml (PLAN.md §6); move when visual nodes need it.

#[derive(Debug)]
pub struct Builtin {
    pub name: &'static str,
    pub returns: bool,
    pub min: usize,
    pub max: usize,
    /// Runtime implements it as a generator that waits across frames (called with `yield*`).
    pub wait: bool,
    pub category: &'static str,
    pub args: &'static str,
    pub doc: &'static str,
}

/// Statement command.
const fn s(category: &'static str, name: &'static str, min: usize, max: usize, args: &'static str, doc: &'static str) -> Builtin {
    Builtin { name, returns: false, min, max, wait: false, category, args, doc }
}
/// Function that returns a value.
const fn f(category: &'static str, name: &'static str, min: usize, max: usize, args: &'static str, doc: &'static str) -> Builtin {
    Builtin { name, returns: true, min, max, wait: false, category, args, doc }
}
/// Waits across frames without freezing the game; `returns` says whether it gives back a value.
const fn w(category: &'static str, name: &'static str, returns: bool, min: usize, max: usize, args: &'static str, doc: &'static str) -> Builtin {
    Builtin { name, returns, min, max, wait: true, category, args, doc }
}

const SETUP: &str = "Setup";
const D2: &str = "2D Drawing";
const IMG: &str = "Images";
const SND: &str = "Sound";
const INPUT: &str = "Input";
const TIME: &str = "Time & Output";
const MATH: &str = "Math";
const STR: &str = "Strings";
const D3: &str = "3D";
const PHYS: &str = "Physics";

#[rustfmt::skip]
pub const BUILTINS: &[Builtin] = &[
    s(SETUP, "Game", 1, 1, "title", "Sets the window title."),
    s(SETUP, "Graphics", 2, 2, "width, height", "Opens a 2D screen of the given size in pixels."),
    s(SETUP, "Graphics3D", 2, 2, "width, height", "Opens a 3D view. 2D commands draw over it as a HUD."),

    s(D2, "Cls", 0, 0, "", "Clears the screen to the ClsColor (in 3D: clears the HUD)."),
    s(D2, "ClsColor", 3, 3, "r, g, b", "Sets the color Cls clears to, 0-255. In 3D it is the sky color."),
    s(D2, "Color", 3, 3, "r, g, b", "Sets the drawing color for shapes and text, 0-255."),
    s(D2, "Rect", 4, 5, "x, y, width, height[, fill]", "Draws a rectangle. fill defaults to 1; 0 draws the outline."),
    s(D2, "Oval", 4, 5, "x, y, width, height[, fill]", "Draws an ellipse inside the given box. fill defaults to 1."),
    s(D2, "Line", 4, 4, "x1, y1, x2, y2", "Draws a line."),
    s(D2, "Plot", 2, 2, "x, y", "Draws a single pixel."),
    s(D2, "Text", 3, 3, "x, y, value", "Draws text with its top-left corner at x, y."),
    f(D2, "GraphicsWidth", 0, 0, "", "Width of the screen in pixels."),
    f(D2, "GraphicsHeight", 0, 0, "", "Height of the screen in pixels."),

    w(IMG, "LoadImage", true, 1, 1, "path", "Loads an image (png, jpg, svg). Waits until it is ready."),
    w(IMG, "LoadAnimImage", true, 5, 5, "path, frameWidth, frameHeight, first, count", "Loads a sprite sheet of equal frames, read left to right, top to bottom."),
    s(IMG, "DrawImage", 3, 4, "image, x, y[, frame]", "Draws an image with its top-left at x, y. Frame numbers wrap around."),
    s(IMG, "DrawImageEx", 5, 6, "image, x, y, angle, scale[, frame]", "Draws an image centered on x, y, rotated (degrees, clockwise) and scaled."),
    f(IMG, "ImageWidth", 1, 1, "image", "Width of an image (or of one frame)."),
    f(IMG, "ImageHeight", 1, 1, "image", "Height of an image (or of one frame)."),

    w(SND, "LoadSound", true, 1, 1, "path", "Loads a sound (ogg, mp3, wav). Waits until it is ready."),
    s(SND, "PlaySound", 1, 1, "sound", "Plays a sound. The same sound can overlap itself."),

    f(INPUT, "KeyDown", 1, 1, "key", "True while the key is held, for example KeyDown(KEY_LEFT)."),
    f(INPUT, "KeyHit", 1, 1, "key", "True only in the frame the key was pressed."),
    f(INPUT, "MouseX", 0, 0, "", "Mouse x position on the screen."),
    f(INPUT, "MouseY", 0, 0, "", "Mouse y position on the screen."),
    f(INPUT, "MouseDown", 0, 1, "[button]", "True while a mouse button is held. 1 left (default), 2 right, 3 middle."),
    f(INPUT, "MouseHit", 0, 1, "[button]", "True only in the frame the mouse button was pressed."),
    f(INPUT, "JoyX", 0, 0, "", "First gamepad's left stick, -1 (left) to 1 (right)."),
    f(INPUT, "JoyY", 0, 0, "", "First gamepad's left stick, -1 (up) to 1 (down)."),
    f(INPUT, "JoyDown", 1, 1, "button", "True while a gamepad button is held (1-based, standard layout)."),

    w(TIME, "Delay", false, 1, 1, "milliseconds", "Waits while the screen keeps updating."),
    f(TIME, "Millisecs", 0, 0, "", "Milliseconds since the game started."),
    s(TIME, "Print", 1, 1, "value", "Writes a line to the console."),

    f(MATH, "Abs", 1, 1, "n", "Absolute value."),
    f(MATH, "Sqr", 1, 1, "n", "Square root."),
    f(MATH, "Sin", 1, 1, "degrees", "Sine of an angle in degrees."),
    f(MATH, "Cos", 1, 1, "degrees", "Cosine of an angle in degrees."),
    f(MATH, "Tan", 1, 1, "degrees", "Tangent of an angle in degrees."),
    f(MATH, "ATan2", 2, 2, "y, x", "Angle in degrees of the point x, y seen from the origin."),
    f(MATH, "Floor", 1, 1, "n", "Rounds down."),
    f(MATH, "Ceil", 1, 1, "n", "Rounds up."),
    f(MATH, "Int", 1, 1, "value", "Whole-number part (towards zero). Also converts strings."),
    f(MATH, "Float", 1, 1, "value", "Converts a value to a number."),
    f(MATH, "Min", 2, 2, "a, b", "The smaller of two numbers."),
    f(MATH, "Max", 2, 2, "a, b", "The larger of two numbers."),
    f(MATH, "Rnd", 1, 2, "[min,] max", "Random decimal number from min (default 0) up to max."),
    f(MATH, "Rand", 1, 2, "[min,] max", "Random whole number from min (default 1) to max, inclusive."),
    f(MATH, "RectsOverlap", 8, 8, "x1, y1, w1, h1, x2, y2, w2, h2", "True if two rectangles overlap."),

    f(STR, "Str", 1, 1, "value", "Converts a value to a string."),
    f(STR, "Len", 1, 1, "s", "Number of characters."),
    f(STR, "Left", 2, 2, "s, n", "The first n characters."),
    f(STR, "Right", 2, 2, "s, n", "The last n characters."),
    f(STR, "Mid", 2, 3, "s, start[, n]", "n characters from position start (1-based), or the rest."),
    f(STR, "Upper", 1, 1, "s", "Upper case."),
    f(STR, "Lower", 1, 1, "s", "Lower case."),
    f(STR, "Chr", 1, 1, "code", "The character with this character code."),
    f(STR, "Asc", 1, 1, "s", "Character code of the first character."),
    f(STR, "Instr", 2, 2, "s, find", "Position of find in s (1-based), or 0."),
    f(STR, "Replace", 3, 3, "s, find, with", "Replaces every occurrence of find."),
    f(STR, "Trim", 1, 1, "s", "Removes spaces from both ends."),

    f(D3, "CreateCamera", 0, 0, "", "Creates a camera. The first one becomes the active view."),
    f(D3, "CreateLight", 0, 1, "[type]", "Creates a light: LIGHT_DIRECTIONAL (default) or LIGHT_POINT."),
    f(D3, "CreateCube", 0, 1, "[size]", "Creates a cube, 1 unit by default."),
    f(D3, "CreateSphere", 0, 1, "[size]", "Creates a sphere with the given diameter."),
    f(D3, "CreatePlane", 0, 1, "[size]", "Creates a flat square floor, 100 units by default."),
    w(D3, "LoadMesh", true, 1, 1, "path", "Loads a .glb or .gltf model. Waits until it is ready."),
    s(D3, "PositionEntity", 4, 4, "entity, x, y, z", "Moves an entity to a position (also teleports physics bodies)."),
    s(D3, "MoveEntity", 4, 4, "entity, x, y, z", "Moves an entity relative to the way it faces."),
    s(D3, "RotateEntity", 4, 4, "entity, pitch, yaw, roll", "Sets an entity's rotation in degrees."),
    s(D3, "TurnEntity", 4, 4, "entity, pitch, yaw, roll", "Rotates an entity by the given degrees."),
    s(D3, "PointEntity", 4, 4, "entity, x, y, z", "Turns an entity to face a point."),
    s(D3, "ScaleEntity", 4, 4, "entity, x, y, z", "Sets an entity's scale."),
    s(D3, "EntityColor", 4, 4, "entity, r, g, b", "Colors a mesh or model, 0-255."),
    s(D3, "FreeEntity", 1, 1, "entity", "Removes an entity from the world."),
    f(D3, "EntityX", 1, 1, "entity", "X position."),
    f(D3, "EntityY", 1, 1, "entity", "Y position."),
    f(D3, "EntityZ", 1, 1, "entity", "Z position."),
    f(D3, "EntityDistance", 2, 2, "a, b", "Distance between two entities."),
    s(D3, "AmbientLight", 3, 3, "r, g, b", "Sets the soft light that reaches every surface."),
    f(D3, "LinePick", 6, 6, "x, y, z, dx, dy, dz", "Casts a ray from x, y, z along dx, dy, dz. Returns the mesh hit, or 0."),
    s(D3, "EntityPickMode", 2, 2, "entity, mode", "0 makes LinePick ignore the entity, 1 makes it pickable again."),

    w(PHYS, "EnablePhysics", false, 0, 3, "[gx, gy, gz]", "Turns on physics (gravity defaults to 0, -9.81, 0). Loads on first use."),
    s(PHYS, "EntityPhysics", 2, 3, "entity, mass[, bounce]", "Gives an entity a physics body. Mass 0 makes it static."),
    s(PHYS, "ApplyImpulse", 4, 4, "entity, x, y, z", "Pushes a physics body."),
    s(PHYS, "SetVelocity", 4, 4, "entity, x, y, z", "Sets a physics body's speed and direction."),
    f(PHYS, "EntityCollided", 1, 2, "a[, b]", "True if a started touching b (or anything) in the last physics step."),
];

// KEY_A..KEY_Z and KEY_0..KEY_9 are accepted by pattern, see `constant`.
pub const CONSTANTS: &[&str] = &[
    "KEY_ESCAPE", "KEY_SPACE", "KEY_ENTER", "KEY_LEFT", "KEY_RIGHT", "KEY_UP", "KEY_DOWN",
    "LIGHT_DIRECTIONAL", "LIGHT_POINT",
];

pub const KEYWORDS: &[&str] = &[
    "if", "then", "else", "elseif", "endif", "end", "while", "wend", "for", "to", "step", "next", "each",
    "repeat", "until", "forever", "exit", "select", "case", "default", "function", "return", "global",
    "const", "dim", "type", "field", "new", "delete", "sync", "include", "everyframe", "and", "or", "not", "mod", "delta",
];

pub fn find(name: &str) -> Option<&'static Builtin> {
    BUILTINS.iter().find(|b| b.name.eq_ignore_ascii_case(name))
}

pub fn constant(name: &str) -> Option<String> {
    let up = name.to_ascii_uppercase();
    let letter_key = up.len() == 5 && up.starts_with("KEY_") && up.as_bytes()[4].is_ascii_alphanumeric();
    (letter_key || CONSTANTS.contains(&up.as_str())).then_some(up)
}

pub fn is_keyword(name: &str) -> bool {
    KEYWORDS.contains(&name.to_ascii_lowercase().as_str())
}

/// Closest builtin within 2 edits, for "did you mean" errors.
pub fn suggest(name: &str) -> Option<&'static str> {
    let n = name.to_ascii_lowercase();
    BUILTINS
        .iter()
        .map(|b| (lev(&n, &b.name.to_ascii_lowercase()), b.name))
        .filter(|(d, _)| *d <= 2)
        .min()
        .map(|(_, s)| s)
}

/// Commands, keywords and constants as JSON for the IDE. Strings are plain ASCII without
/// quotes or backslashes (a test enforces it), so they need no escaping.
pub fn manifest_json() -> String {
    let q = |s: &str| format!("\"{s}\"");
    let cmds: Vec<String> = BUILTINS
        .iter()
        .map(|b| {
            format!(
                "{{\"name\":{},\"category\":{},\"args\":{},\"doc\":{},\"returns\":{},\"wait\":{}}}",
                q(b.name), q(b.category), q(b.args), q(b.doc), b.returns, b.wait
            )
        })
        .collect();
    let mut consts: Vec<String> = CONSTANTS.iter().map(|c| q(c)).collect();
    consts.extend(('A'..='Z').chain('0'..='9').map(|c| q(&format!("KEY_{c}"))));
    let kws: Vec<String> = KEYWORDS.iter().map(|k| q(k)).collect();
    format!("{{\"commands\":[{}],\"constants\":[{}],\"keywords\":[{}]}}", cmds.join(","), consts.join(","), kws.join(","))
}

fn lev(a: &str, b: &str) -> usize {
    let b: Vec<char> = b.chars().collect();
    let mut row: Vec<usize> = (0..=b.len()).collect();
    for (i, ca) in a.chars().enumerate() {
        let mut prev = row[0];
        row[0] = i + 1;
        for j in 0..b.len() {
            let cur = row[j + 1];
            row[j + 1] = (prev + (ca != b[j]) as usize).min(row[j] + 1).min(cur + 1);
            prev = cur;
        }
    }
    row[b.len()]
}

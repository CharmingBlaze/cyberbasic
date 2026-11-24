// shortcut: hard-coded table stands in for the api/*.toml command manifest from PLAN.md §6;
// move to the manifest when visual nodes or generated docs need it (Phase 4).

#[derive(Debug)]
pub struct Builtin {
    pub name: &'static str,
    pub returns: bool,
    pub min: usize,
    pub max: usize,
    /// Runtime implements it as a generator that waits across frames (called with `yield*`).
    pub wait: bool,
}

/// Statement command.
const fn s(name: &'static str, min: usize, max: usize) -> Builtin {
    Builtin { name, returns: false, min, max, wait: false }
}
/// Function that returns a value.
const fn f(name: &'static str, min: usize, max: usize) -> Builtin {
    Builtin { name, returns: true, min, max, wait: false }
}
/// Waits across frames; `returns` says whether it gives back a value.
const fn w(name: &'static str, returns: bool, min: usize, max: usize) -> Builtin {
    Builtin { name, returns, min, max, wait: true }
}

#[rustfmt::skip]
pub const BUILTINS: &[Builtin] = &[
    // Setup
    s("Game", 1, 1), s("Graphics", 2, 2), s("Graphics3D", 2, 2),
    // 2D drawing (also draws over the 3D view as a HUD)
    s("Cls", 0, 0), s("ClsColor", 3, 3), s("Color", 3, 3),
    s("Rect", 4, 5), s("Oval", 4, 5), s("Line", 4, 4), s("Plot", 2, 2), s("Text", 3, 3),
    f("GraphicsWidth", 0, 0), f("GraphicsHeight", 0, 0),
    w("LoadImage", true, 1, 1), w("LoadAnimImage", true, 5, 5), s("DrawImage", 3, 4), s("DrawImageEx", 5, 6),
    f("ImageWidth", 1, 1), f("ImageHeight", 1, 1),
    // Sound
    w("LoadSound", true, 1, 1), s("PlaySound", 1, 1),
    // Input
    f("KeyDown", 1, 1), f("KeyHit", 1, 1),
    f("MouseX", 0, 0), f("MouseY", 0, 0), f("MouseDown", 0, 1), f("MouseHit", 0, 1),
    f("JoyX", 0, 0), f("JoyY", 0, 0), f("JoyDown", 1, 1),
    // Time and output
    w("Delay", false, 1, 1), f("Millisecs", 0, 0), s("Print", 1, 1),
    // Math
    f("Abs", 1, 1), f("Sqr", 1, 1), f("Sin", 1, 1), f("Cos", 1, 1), f("Tan", 1, 1), f("ATan2", 2, 2),
    f("Floor", 1, 1), f("Ceil", 1, 1), f("Int", 1, 1), f("Float", 1, 1), f("Min", 2, 2), f("Max", 2, 2),
    f("Rnd", 1, 2), f("Rand", 1, 2), f("RectsOverlap", 8, 8),
    // Strings
    f("Str", 1, 1), f("Len", 1, 1), f("Left", 2, 2), f("Right", 2, 2), f("Mid", 2, 3),
    f("Upper", 1, 1), f("Lower", 1, 1), f("Chr", 1, 1), f("Asc", 1, 1), f("Instr", 2, 2),
    f("Replace", 3, 3), f("Trim", 1, 1),
    // 3D
    f("CreateCamera", 0, 0), f("CreateLight", 0, 1),
    f("CreateCube", 0, 1), f("CreateSphere", 0, 1), f("CreatePlane", 0, 1),
    s("PositionEntity", 4, 4), s("RotateEntity", 4, 4), s("MoveEntity", 4, 4), s("TurnEntity", 4, 4),
    s("PointEntity", 4, 4), s("ScaleEntity", 4, 4), s("EntityColor", 4, 4), s("FreeEntity", 1, 1),
    f("EntityX", 1, 1), f("EntityY", 1, 1), f("EntityZ", 1, 1), f("EntityDistance", 2, 2),
    s("AmbientLight", 3, 3), w("LoadMesh", true, 1, 1), f("LinePick", 6, 6), s("EntityPickMode", 2, 2),
    // Physics (Havok)
    w("EnablePhysics", false, 0, 3), s("EntityPhysics", 2, 3), s("ApplyImpulse", 4, 4), s("SetVelocity", 4, 4),
    f("EntityCollided", 1, 2),
];

// KEY_A..KEY_Z and KEY_0..KEY_9 are accepted by pattern, see `constant`.
pub const CONSTANTS: &[&str] = &[
    "KEY_ESCAPE", "KEY_SPACE", "KEY_ENTER", "KEY_LEFT", "KEY_RIGHT", "KEY_UP", "KEY_DOWN",
    "LIGHT_DIRECTIONAL", "LIGHT_POINT",
];

pub const KEYWORDS: &[&str] = &[
    "if", "then", "else", "elseif", "endif", "end", "while", "wend", "for", "to", "step", "next", "each",
    "repeat", "until", "forever", "exit", "select", "case", "default", "function", "return", "global",
    "const", "dim", "type", "field", "new", "delete", "sync", "and", "or", "not", "mod", "delta",
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

// shortcut: hard-coded table stands in for the api/*.toml command manifest from PLAN.md §6;
// move to the manifest when visual nodes or generated docs need it (Phase 2/4).

pub struct Builtin {
    pub name: &'static str,
    pub returns: bool,
    pub min: usize,
    pub max: usize,
}

const fn b(name: &'static str, returns: bool, min: usize, max: usize) -> Builtin {
    Builtin { name, returns, min, max }
}

pub const BUILTINS: &[Builtin] = &[
    b("Game", false, 1, 1),
    b("Graphics3D", false, 2, 2),
    b("CreateCamera", true, 0, 0),
    b("CreateLight", true, 0, 1),
    b("CreateCube", true, 0, 1),
    b("CreateSphere", true, 0, 1),
    b("CreatePlane", true, 0, 1),
    b("PositionEntity", false, 4, 4),
    b("RotateEntity", false, 4, 4),
    b("MoveEntity", false, 4, 4),
    b("TurnEntity", false, 4, 4),
    b("PointEntity", false, 4, 4),
    b("EntityColor", false, 4, 4),
    b("KeyDown", true, 1, 1),
    b("KeyHit", true, 1, 1),
    b("Print", false, 1, 1),
];

// KEY_A..KEY_Z and KEY_0..KEY_9 are accepted by pattern, see `constant`.
pub const CONSTANTS: &[&str] = &[
    "KEY_ESCAPE", "KEY_SPACE", "KEY_ENTER", "KEY_LEFT", "KEY_RIGHT", "KEY_UP", "KEY_DOWN",
    "LIGHT_DIRECTIONAL", "LIGHT_POINT",
];

pub const KEYWORDS: &[&str] = &[
    "if", "then", "else", "elseif", "endif", "end", "while", "wend", "sync", "and", "or", "not", "delta",
];

pub fn find(name: &str) -> Option<&'static Builtin> {
    BUILTINS.iter().find(|b| b.name.eq_ignore_ascii_case(name))
}

pub fn constant(name: &str) -> Option<String> {
    let up = name.to_ascii_uppercase();
    let letter_key = up.len() == 5 && up.starts_with("KEY_") && up.as_bytes()[4].is_ascii_alphanumeric();
    (letter_key || CONSTANTS.contains(&up.as_str())).then_some(up)
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

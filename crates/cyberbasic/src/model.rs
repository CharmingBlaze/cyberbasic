//! Models (.model files) from the modeller: parts, joints and animation clips. `hero.model`
//! becomes `Function CreateModel_Hero()`, which builds the model with the runtime's CreateModel
//! (the file travels inside as base64, since BASIC strings can't hold quotes).

use crate::graph::{behaviour_name, Lowered};

pub fn is_model(file: &str) -> bool {
    file.to_ascii_lowercase().ends_with(".model")
}

/// `hero.model` (or `models/hero.model`) -> `CreateModel_Hero`.
pub fn create_function(file: &str) -> String {
    format!("CreateModel_{}", behaviour_name(file.rsplit(['/', '\\']).next().unwrap_or(file)))
}

pub fn lower(file: &str, text: &str) -> Lowered {
    let mut problems = vec![];
    if let Err(e) = crate::json::parse(text) {
        problems.push((String::new(), format!("This model file is damaged: {e}")));
    }
    let code = format!(
        "' Generated from {file} by the modeller. Edit the model, not this code.\n\nFunction {}()\n    Return CreateModel(\"{}\")\nEnd Function\n",
        create_function(file),
        base64(text.as_bytes())
    );
    Lowered { code, map: vec![], problems }
}

pub fn base64(bytes: &[u8]) -> String {
    const A: &[u8; 64] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    let mut out = String::with_capacity(bytes.len().div_ceil(3) * 4);
    for c in bytes.chunks(3) {
        let n = (c[0] as u32) << 16 | (*c.get(1).unwrap_or(&0) as u32) << 8 | *c.get(2).unwrap_or(&0) as u32;
        for i in 0..4 {
            out.push(if i <= c.len() { A[(n >> (18 - 6 * i) & 63) as usize] as char } else { '=' });
        }
    }
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn models_become_a_create_function() {
        assert_eq!(base64(b"Man"), "TWFu");
        assert_eq!(base64(b"Ma"), "TWE=");
        assert_eq!(base64("é!".as_bytes()), "w6kh");
        let l = lower("hero.model", r#"{"parts":[]}"#);
        assert!(l.code.contains("Function CreateModel_Hero()\n    Return CreateModel(\"eyJwYXJ0cyI6W119\")\nEnd Function"), "{}", l.code);
        assert!(l.problems.is_empty());
        assert!(!lower("bad.model", "{").problems.is_empty());
        // A scene places a model by including it and calling its create function.
        let scene = r#"{"objects":[{"id":"o1","name":"Hero","kind":"model","model":"hero.model"}]}"#;
        let s = crate::scene::lower("level.scene", scene, &[]);
        assert!(s.code.starts_with("' Generated from level.scene by the level editor. Edit the scene, not this code.\nInclude \"hero.model\"\n"), "{}", s.code);
        assert!(s.code.contains("e = CreateModel_Hero()"), "{}", s.code);
        let main = "Include \"level.scene\"\nGraphics3D 640, 480\nLoadScene_Level\nAnimateEntity FindEntity(\"Hero\"), \"walk\", 2\nPrint EntityAnimation(FindEntity(\"Hero\"))";
        crate::compile_files(&[("main.cb", main), ("level.scene", scene), ("hero.model", r#"{"parts":[]}"#)], false).unwrap();
    }
}

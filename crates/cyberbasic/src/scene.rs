//! Scenes (.scene files) from the level editor, lowered to CyberBasic: `level.scene` becomes
//! `Function LoadScene_Level()`, which creates, names and places every object. Code then finds
//! them with `FindEntity("Player")`.

use crate::graph::{behaviour_name, Lowered};
use crate::json::{self, Json};

fn num(v: &Json, i: usize, default: f64) -> String {
    let n = match v.arr().get(i) {
        Some(Json::Num(n)) => *n,
        _ => default,
    };
    // Round away float noise from gizmo drags (0.30000000000000004).
    let r = (n * 1e4).round() / 1e4;
    if r == r.trunc() { format!("{}", r as i64) } else { format!("{r}") }
}
fn vec3(v: &Json, default: f64) -> String {
    (0..3).map(|i| num(v, i, default)).collect::<Vec<_>>().join(", ")
}

/// `level.scene` -> `LoadScene_Level`.
pub fn load_function(file: &str) -> String {
    format!("LoadScene_{}", behaviour_name(file))
}

pub fn is_scene(file: &str) -> bool {
    file.to_ascii_lowercase().ends_with(".scene")
}

pub fn lower(file: &str, text: &str) -> Lowered {
    let mut lines: Vec<String> = vec![format!("' Generated from {file} by the level editor. Edit the scene, not this code."), String::new()];
    let mut map = vec![];
    let mut problems = vec![];
    let scene = match json::parse(text) {
        Ok(j) => j,
        Err(e) => {
            problems.push((String::new(), format!("This scene file is damaged: {e}")));
            Json::Null
        }
    };
    let objects = scene.get("objects").arr();
    lines.push(format!("Function {}()", load_function(file)));
    if objects.iter().any(|o| matches!(o.get("physics").str(), "static" | "dynamic")) {
        lines.push("    EnablePhysics".into());
    }
    for o in objects {
        let id = o.get("id").str();
        let mut emit = |s: String| {
            lines.push(format!("    {s}"));
            map.push((lines.len(), id.to_string()));
        };
        let name: String = o.get("name").str().chars().filter(|&c| c != '"' && c >= ' ').collect();
        let kind = o.get("kind").str();
        let create = match kind {
            "cube" => "CreateCube()".to_string(),
            "sphere" => "CreateSphere()".into(),
            "plane" => "CreatePlane(1)".into(),
            "light" => "CreateLight()".into(),
            "pointlight" => "CreateLight(LIGHT_POINT)".into(),
            "camera" => "CreateCamera()".into(),
            "model" => {
                let path: String = o.get("model").str().chars().filter(|&c| c != '"').collect();
                format!("LoadMesh(\"{path}\")")
            }
            other => {
                problems.push((id.to_string(), format!("Unknown kind of object \"{other}\".")));
                continue;
            }
        };
        emit(format!("' {name}"));
        emit(format!("e = {create}"));
        if !name.is_empty() {
            emit(format!("NameEntity e, \"{name}\""));
        }
        emit(format!("PositionEntity e, {}", vec3(o.get("position"), 0.0)));
        emit(format!("RotateEntity e, {}", vec3(o.get("rotation"), 0.0)));
        let mesh = matches!(kind, "cube" | "sphere" | "plane" | "model");
        if mesh {
            let s = vec3(o.get("scale"), 1.0);
            if s != "1, 1, 1" {
                emit(format!("ScaleEntity e, {s}"));
            }
            if o.get("color").arr().len() == 3 {
                emit(format!("EntityColor e, {}", vec3(o.get("color"), 255.0)));
            }
            match o.get("physics").str() {
                "static" => emit("EntityPhysics e, 0".into()),
                "dynamic" => emit(format!("EntityPhysics e, {}", num(&Json::Arr(vec![o.get("mass").clone()]), 0, 1.0))),
                _ => {}
            }
        }
        for b in o.get("behaviours").arr() {
            let b: String = b.str().chars().filter(|c| c.is_alphanumeric() || *c == '_').collect();
            if !b.is_empty() {
                emit(format!("Behaviour_{b}_Attach e"));
            }
        }
    }
    lines.push("End Function".into());
    Lowered { code: lines.join("\n") + "\n", map, problems }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn scenes_become_a_load_function() {
        let s = r#"{"objects":[
            {"id":"o1","name":"Ground","kind":"plane","position":[0,0,0],"rotation":[0,0,0],"scale":[20,1,20],"color":[60,180,90],"physics":"static"},
            {"id":"o2","name":"Ball \"1\"","kind":"sphere","position":[0,3.30000000001,0],"rotation":[0,45,0],"scale":[1,1,1],"physics":"dynamic","mass":2,"behaviours":["Spinner"]},
            {"id":"o3","name":"Sun","kind":"light","position":[0,10,0],"rotation":[50,30,0],"color":[255,255,255]},
            {"id":"o4","kind":"blob"}]}"#;
        let l = lower("level.scene", s);
        assert!(l.code.contains("Function LoadScene_Level()\n    EnablePhysics\n    ' Ground\n    e = CreatePlane(1)\n    NameEntity e, \"Ground\"\n    PositionEntity e, 0, 0, 0\n    RotateEntity e, 0, 0, 0\n    ScaleEntity e, 20, 1, 20\n    EntityColor e, 60, 180, 90\n    EntityPhysics e, 0\n"), "{}", l.code);
        assert!(l.code.contains("NameEntity e, \"Ball 1\"\n    PositionEntity e, 0, 3.3, 0\n    RotateEntity e, 0, 45, 0\n    EntityPhysics e, 2\n    Behaviour_Spinner_Attach e\n"), "{}", l.code);
        assert!(!l.code.contains("EntityColor e, 255"), "lights take no color: {}", l.code);
        assert_eq!(l.problems, vec![("o4".to_string(), "Unknown kind of object \"blob\".".to_string())]);
        assert_eq!(l.map.iter().find(|m| m.1 == "o2").map(|m| l.code.lines().nth(m.0 - 1).unwrap()), Some("    ' Ball 1"));
        let main = "Include \"level.scene\"\nInclude \"spinner.graph\"\nGraphics3D 640, 480\nLoadScene_Level\nPrint EntityName(FindEntity(\"ball 1\"))";
        let spin = r#"{"kind":"behaviour","nodes":[],"links":[]}"#;
        let ok = r#"{"objects":[{"id":"o1","name":"Ball 1","kind":"sphere","behaviours":["Spinner"]}]}"#;
        crate::compile_files(&[("main.cb", main), ("level.scene", ok), ("spinner.graph", spin)], false).unwrap();
    }
}

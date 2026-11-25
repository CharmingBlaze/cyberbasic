use std::path::{Path, PathBuf};
use std::{env, fs, process};

const USAGE: &str = "usage: cyberbasic <build|run> <file.cb|file.graph> [-o <out_dir>]";

fn main() {
    let args: Vec<String> = env::args().skip(1).collect();
    let (cmd, file) = match args.as_slice() {
        [c, f, ..] if c == "build" || c == "run" => (c.as_str(), f.as_str()),
        _ => die(USAGE),
    };
    fs::metadata(file).unwrap_or_else(|e| die(&format!("cannot read {file}: {e}")));
    // Included files are found relative to the main file's folder (the project folder).
    let dir = Path::new(file).parent().unwrap_or(Path::new(".")).to_path_buf();
    let main_name = Path::new(file).file_name().unwrap_or_default().to_string_lossy().into_owned();
    let load = |name: &str| fs::read_to_string(dir.join(name)).ok();
    // The project is the folder (as in the IDE): graphs can call functions from any file in it.
    let mut files = vec![(main_name.clone(), load(&main_name).unwrap_or_else(|| die(&format!("cannot read {file}"))))];
    for e in fs::read_dir(&dir).unwrap_or_else(|e| die(&format!("cannot read {}: {e}", dir.display()))).flatten() {
        let name = e.file_name().to_string_lossy().into_owned();
        let ext = Path::new(&name).extension().map(|x| x.to_ascii_lowercase());
        if name != main_name && (ext.as_deref() == Some("cb".as_ref()) || ext.as_deref() == Some("graph".as_ref()) || ext.as_deref() == Some("scene".as_ref())) {
            files.push((name.clone(), fs::read_to_string(e.path()).unwrap_or_default()));
        }
    }
    let refs: Vec<(&str, &str)> = files.iter().map(|(n, t)| (n.as_str(), t.as_str())).collect();
    let js = cyberbasic_core::compile_files(&refs, false).unwrap_or_else(|e| {
        report(&dir, &e, &load);
        process::exit(1)
    });

    let out = match args.iter().position(|a| a == "-o") {
        Some(i) => PathBuf::from(args.get(i + 1).unwrap_or_else(|| die(USAGE))),
        None => {
            let p = Path::new(file);
            p.parent().unwrap_or(Path::new(".")).join("build").join(p.file_stem().unwrap_or_default())
        }
    };
    let write = |name: &str, body: &str| {
        fs::write(out.join(name), body).unwrap_or_else(|e| die(&format!("cannot write {}: {e}", out.join(name).display())))
    };
    fs::create_dir_all(&out).unwrap_or_else(|e| die(&format!("cannot create {}: {e}", out.display())));
    write("index.html", cyberbasic_core::INDEX_HTML);
    write("runtime.js", cyberbasic_core::RUNTIME_JS);
    write("game.js", &js);
    let assets = Path::new(file).parent().unwrap_or(Path::new(".")).join("assets");
    if assets.is_dir() {
        copy_dir(&assets, &out.join("assets")).unwrap_or_else(|e| die(&format!("cannot copy assets: {e}")));
    }
    let index = out.join("index.html");
    println!("Built {}", index.display());

    if cmd == "run" {
        open(&index);
    }
}

/// Copies the `assets/` folder next to the source file into the build, so LoadImage/LoadSound
/// paths like "assets/player.png" work in the exported game.
fn copy_dir(from: &Path, to: &Path) -> std::io::Result<()> {
    fs::create_dir_all(to)?;
    for entry in fs::read_dir(from)? {
        let path = entry?.path();
        let target = to.join(path.file_name().unwrap_or_default());
        if path.is_dir() {
            copy_dir(&path, &target)?;
        } else {
            fs::copy(&path, &target)?;
        }
    }
    Ok(())
}

fn open(path: &Path) {
    let status = if cfg!(windows) {
        process::Command::new("cmd").args(["/C", "start", ""]).arg(path).status()
    } else if cfg!(target_os = "macos") {
        process::Command::new("open").arg(path).status()
    } else {
        process::Command::new("xdg-open").arg(path).status()
    };
    if !status.is_ok_and(|s| s.success()) {
        eprintln!("Could not open a browser. Open {} manually.", path.display());
    }
}

fn report(dir: &Path, e: &cyberbasic_core::Error, load: &dyn Fn(&str) -> Option<String>) {
    eprintln!("{}:{}:{}: error: {}", dir.join(&e.file).display(), e.line, e.col, e.msg);
    if cyberbasic_core::graph::is_graph(&e.file) {
        return; // the line is in the generated code, not the JSON; open the graph in the IDE to see the node
    }
    let src = load(&e.file).unwrap_or_default();
    if let Some(text) = src.lines().nth(e.line.saturating_sub(1)) {
        eprintln!("  {:>4} | {text}", e.line);
        eprintln!("       | {}^", " ".repeat(e.col.saturating_sub(1)));
    }
}

fn die(msg: &str) -> ! {
    eprintln!("{msg}");
    process::exit(2)
}

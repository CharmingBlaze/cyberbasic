use std::path::{Path, PathBuf};
use std::{env, fs, process};

const USAGE: &str = "usage: cyberbasic <build|run> <file.cb> [-o <out_dir>]";

fn main() {
    let args: Vec<String> = env::args().skip(1).collect();
    let (cmd, file) = match args.as_slice() {
        [c, f, ..] if c == "build" || c == "run" => (c.as_str(), f.as_str()),
        _ => die(USAGE),
    };
    let src = fs::read_to_string(file).unwrap_or_else(|e| die(&format!("cannot read {file}: {e}")));
    let js = cyberbasic::compile(&src).unwrap_or_else(|e| {
        report(file, &src, &e);
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
    write("index.html", cyberbasic::INDEX_HTML);
    write("runtime.js", cyberbasic::RUNTIME_JS);
    write("game.js", &js);
    let index = out.join("index.html");
    println!("Built {}", index.display());

    if cmd == "run" {
        open(&index);
    }
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

fn report(file: &str, src: &str, e: &cyberbasic::Error) {
    eprintln!("{file}:{}:{}: error: {}", e.line, e.col, e.msg);
    if let Some(text) = src.lines().nth(e.line - 1) {
        eprintln!("  {:>4} | {text}", e.line);
        eprintln!("       | {}^", " ".repeat(e.col.saturating_sub(1)));
    }
}

fn die(msg: &str) -> ! {
    eprintln!("{msg}");
    process::exit(2)
}

// CyberBasic runtime. A classic script (not a module) so exported games open straight from disk.
// shortcut: plain JS instead of the planned TypeScript; port once the runtime outgrows one file.
// shortcut: 2D drawing uses Canvas 2D (also as the HUD over 3D) rather than Babylon's 2D layer;
// revisit if sprite counts outgrow it (PLAN.md §13 targets 5,000 sprites).
"use strict";
if (typeof BABYLON === "undefined") {
  document.body.insertAdjacentHTML("beforeend",
    '<pre class="cb-msg err">Babylon.js failed to load. Games load it from cdn.babylonjs.com, so they need internet.</pre>');
  throw new Error("Babylon.js not loaded");
}

const CB = (() => {
  const B = BABYLON, D2R = Math.PI / 180, V = (x, y, z) => new B.Vector3(x, y, z);
  const END = {};
  const keys = new Set(), keyHits = new Set(), buttons = new Set(), buttonHits = new Set();
  const mouse = { x: 0, y: 0 };
  const instances = new Map(); // Type field list -> live objects
  let engine = null, scene = null, ambient = null, ctx = null, mode = 0;
  let clsColor = "rgb(0,0,0)", frameStart = 0, ops = 0;

  addEventListener("keydown", (e) => {
    if (!e.repeat) keyHits.add(e.code);
    keys.add(e.code);
    if (e.code.startsWith("Arrow") || e.code === "Space") e.preventDefault();
  });
  addEventListener("keyup", (e) => keys.delete(e.code));
  addEventListener("mouseup", (e) => buttons.delete(e.button));
  addEventListener("blur", () => { keys.clear(); buttons.clear(); });

  const BUTTON = [0, 0, 2, 1]; // BASIC 1=left 2=right 3=middle -> DOM button numbers
  const rgb = (r, g, b) => `rgb(${r},${g},${b})`;
  const need3d = () => { if (!scene) throw new Error("call Graphics3D before using 3D commands"); };
  const need2d = () => { if (!ctx) throw new Error("call Graphics or Graphics3D first"); };
  const node = (name) => { need3d(); return new B.TransformNode(name, scene); };
  const mesh = (m) => { m.material = new B.StandardMaterial(m.name, scene); return m; };
  const shape = (fill, draw) => { need2d(); ctx.beginPath(); draw(); fill ? ctx.fill() : ctx.stroke(); };
  const say = (text, err) => {
    const p = document.createElement("pre");
    p.className = err ? "cb-msg err" : "cb-msg";
    p.textContent = text;
    document.body.append(p);
  };
  const canvas = (w, h) => {
    const c = document.createElement("canvas");
    c.width = w; c.height = h;
    c.style.cssText = `width:${w}px;height:${h}px`;
    return c;
  };

  function screen(w, h, three) {
    if (mode) throw new Error("Graphics was already called");
    const box = document.createElement("div");
    box.style.cssText = `position:relative;width:${w}px;height:${h}px`;
    const c2 = canvas(w, h);
    if (three) {
      const c3 = canvas(w, h);
      box.append(c3);
      c2.style.cssText += ";position:absolute;inset:0;pointer-events:none";
      engine = new B.Engine(c3, true);
      scene = new B.Scene(engine);
      scene.clearColor = new B.Color4(0.05, 0.05, 0.08, 1);
      ambient = new B.HemisphericLight("ambient", V(0, 1, 0), scene);
      ambient.intensity = 0.3;
    }
    box.append(c2);
    document.body.prepend(box);
    ctx = c2.getContext("2d");
    ctx.textBaseline = "top";
    ctx.font = "16px ui-monospace, Consolas, monospace";
    ctx.fillStyle = ctx.strokeStyle = "#fff";
    box.addEventListener("mousemove", (e) => {
      const r = box.getBoundingClientRect();
      mouse.x = e.clientX - r.left; mouse.y = e.clientY - r.top;
    });
    box.addEventListener("mousedown", (e) => { buttons.add(e.button); buttonHits.add(e.button); });
    box.addEventListener("contextmenu", (e) => e.preventDefault());
    mode = three ? 3 : 2;
    if (!three) cb.Cls();
  }

  // Waits for a media element to load, keeping frames running meanwhile.
  function* load(el, okEvent, path, what) {
    let state = 0;
    el.addEventListener(okEvent, () => { state = 1; }, { once: true });
    el.addEventListener("error", () => { state = 2; }, { once: true });
    el.src = String(path);
    const giveUp = performance.now() + 10000;
    while (state === 0 && performance.now() < giveUp) yield;
    if (state !== 1) throw new Error(`could not load ${what} "${path}"`);
    return el;
  }

  const cb = {
    delta: 0, // seconds since the last frame
    ln: 0,    // current BASIC line, set by generated code
    END,

    KEY_ESCAPE: "Escape", KEY_SPACE: "Space", KEY_ENTER: "Enter",
    KEY_LEFT: "ArrowLeft", KEY_RIGHT: "ArrowRight", KEY_UP: "ArrowUp", KEY_DOWN: "ArrowDown",
    LIGHT_DIRECTIONAL: 1, LIGHT_POINT: 2,

    // --- Setup
    Game(title) { document.title = String(title); },
    Graphics(w, h) { screen(w, h, false); },
    Graphics3D(w, h) { screen(w, h, true); },

    // --- 2D drawing
    Cls() {
      need2d();
      if (mode === 3) return ctx.clearRect(0, 0, ctx.canvas.width, ctx.canvas.height);
      ctx.save(); ctx.fillStyle = clsColor; ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height); ctx.restore();
    },
    ClsColor(r, g, b) {
      clsColor = rgb(r, g, b);
      if (scene) scene.clearColor = new B.Color4(r / 255, g / 255, b / 255, 1);
    },
    Color(r, g, b) { need2d(); ctx.fillStyle = ctx.strokeStyle = rgb(r, g, b); },
    Rect(x, y, w, h, fill = 1) { need2d(); fill ? ctx.fillRect(x, y, w, h) : ctx.strokeRect(x, y, w, h); },
    Oval(x, y, w, h, fill = 1) { shape(fill, () => ctx.ellipse(x + w / 2, y + h / 2, w / 2, h / 2, 0, 0, Math.PI * 2)); },
    Line(x1, y1, x2, y2) { shape(false, () => { ctx.moveTo(x1, y1); ctx.lineTo(x2, y2); }); },
    Plot(x, y) { need2d(); ctx.fillRect(x, y, 1, 1); },
    Text(x, y, s) { need2d(); ctx.fillText(String(s), x, y); },
    GraphicsWidth() { need2d(); return ctx.canvas.width; },
    GraphicsHeight() { need2d(); return ctx.canvas.height; },
    *LoadImage(path) { return yield* load(new Image(), "load", path, "image"); },
    DrawImage(img, x, y) { need2d(); ctx.drawImage(img, x, y); },
    ImageWidth(img) { return img.width; },
    ImageHeight(img) { return img.height; },

    // --- Sound
    *LoadSound(path) {
      const a = new Audio();
      a.preload = "auto";
      return yield* load(a, "canplaythrough", path, "sound");
    },
    PlaySound(snd) { snd.cloneNode().play().catch(() => {}); }, // clone so the same sound can overlap

    // --- Input
    KeyDown(k) { return keys.has(k); },
    KeyHit(k) { return keyHits.has(k); },
    MouseX() { return mouse.x; },
    MouseY() { return mouse.y; },
    MouseDown(b = 1) { return buttons.has(BUTTON[b]); },
    MouseHit(b = 1) { return buttonHits.has(BUTTON[b]); },

    // --- Time and output
    *Delay(ms) { const until = performance.now() + ms; while (performance.now() < until) yield; },
    Millisecs() { return Math.floor(performance.now()); },
    Print(v) { console.log(String(v)); },

    // --- Math (angles in degrees, as in Blitz)
    Abs: Math.abs,
    Sqr: Math.sqrt,
    Sin(d) { return Math.sin(d * D2R); },
    Cos(d) { return Math.cos(d * D2R); },
    Tan(d) { return Math.tan(d * D2R); },
    ATan2(y, x) { return Math.atan2(y, x) / D2R; },
    Floor: Math.floor,
    Ceil: Math.ceil,
    Int(v) { return Math.trunc(Number(v)) || 0; },
    Float(v) { return Number(v) || 0; },
    Min: Math.min,
    Max: Math.max,
    Rnd(a, b) { if (b === undefined) [a, b] = [0, a]; return a + Math.random() * (b - a); },
    Rand(a, b) { if (b === undefined) [a, b] = [1, a]; return a + Math.floor(Math.random() * (b - a + 1)); },
    RectsOverlap(x1, y1, w1, h1, x2, y2, w2, h2) { return x1 < x2 + w2 && x2 < x1 + w1 && y1 < y2 + h2 && y2 < y1 + h1; },

    // --- Strings (positions are 1-based, as in BASIC)
    Str: String,
    Len(s) { return String(s).length; },
    Left(s, n) { return String(s).slice(0, Math.max(0, n)); },
    Right(s, n) { return n > 0 ? String(s).slice(-n) : ""; },
    Mid(s, start, n) { s = String(s); return n === undefined ? s.slice(start - 1) : s.substr(start - 1, n); },
    Upper(s) { return String(s).toUpperCase(); },
    Lower(s) { return String(s).toLowerCase(); },
    Chr(n) { return String.fromCharCode(n); },
    Asc(s) { return String(s).charCodeAt(0) || 0; },
    Instr(s, sub) { return String(s).indexOf(String(sub)) + 1; },
    Replace(s, a, b) { return String(s).replaceAll(String(a), String(b)); },
    Trim(s) { return String(s).trim(); },

    // --- 3D. Cameras and lights sit under a TransformNode so every *Entity command works on them.
    CreateCamera() {
      const n = node("camera");
      const cam = new B.UniversalCamera("camera", V(0, 0, 0), scene);
      cam.parent = n;
      cam.minZ = 0.1;
      return n;
    },
    CreateLight(type = cb.LIGHT_DIRECTIONAL) {
      const n = node("light");
      const l = type === cb.LIGHT_POINT
        ? new B.PointLight("light", V(0, 0, 0), scene)
        : new B.DirectionalLight("light", V(0, 0, 1), scene);
      l.parent = n;
      n.rotation.set(50 * D2R, 30 * D2R, 0); // aim down and across so a default scene is lit
      return n;
    },
    CreateCube(size = 1) { need3d(); return mesh(B.MeshBuilder.CreateBox("cube", { size }, scene)); },
    CreateSphere(size = 1) { need3d(); return mesh(B.MeshBuilder.CreateSphere("sphere", { diameter: size }, scene)); },
    CreatePlane(size = 100) { need3d(); return mesh(B.MeshBuilder.CreateGround("plane", { width: size, height: size }, scene)); },
    PositionEntity(e, x, y, z) { e.position.set(x, y, z); },
    RotateEntity(e, pitch, yaw, roll) { e.rotation.set(pitch * D2R, yaw * D2R, roll * D2R); },
    MoveEntity(e, x, y, z) { e.locallyTranslate(V(x, y, z)); },
    TurnEntity(e, pitch, yaw, roll) { e.addRotation(pitch * D2R, yaw * D2R, roll * D2R); },
    PointEntity(e, x, y, z) { e.lookAt(V(x, y, z)); },
    ScaleEntity(e, x, y, z) { e.scaling.set(x, y, z); },
    EntityColor(e, r, g, b) {
      if (!e.material) throw new Error("EntityColor needs a mesh (cube, sphere, plane)");
      e.material.diffuseColor = new B.Color3(r / 255, g / 255, b / 255);
    },
    FreeEntity(e) { e.dispose(); },
    EntityX(e) { return e.position.x; },
    EntityY(e) { return e.position.y; },
    EntityZ(e) { return e.position.z; },
    EntityDistance(a, b) { return B.Vector3.Distance(a.getAbsolutePosition(), b.getAbsolutePosition()); },
    AmbientLight(r, g, b) {
      need3d();
      ambient.diffuse = ambient.groundColor = new B.Color3(r / 255, g / 255, b / 255);
      ambient.intensity = 1;
    },

    // --- Helpers used by generated code (not BASIC commands)
    dim(n, ...rest) {
      if (!(n >= 0)) throw new Error(`Dim size must be 0 or more, got ${n}`);
      return Array.from({ length: Math.trunc(n) }, () => (rest.length ? cb.dim(...rest) : 0));
    },
    ck(a, i) {
      if (!Array.isArray(a)) throw new Error("that variable is not an array (create it with Dim first)");
      const k = Math.trunc(i);
      if (!(k >= 0 && k < a.length)) throw new Error(`array index ${i} is out of range (0 to ${a.length - 1})`);
      return k;
    },
    ix(a, i) { return a[cb.ck(a, i)]; },
    newT(fields) {
      const o = {};
      for (const f of fields) o[f] = 0;
      Object.defineProperty(o, "$type", { value: fields });
      if (!instances.has(fields)) instances.set(fields, []);
      instances.get(fields).push(o);
      return o;
    },
    each(fields) { return (instances.get(fields) || []).slice(); }, // a copy, so Delete inside For Each is safe
    del(o) {
      if (!o || !o.$type) throw new Error("Delete needs an object created with New");
      const list = instances.get(o.$type), i = list.indexOf(o);
      if (i >= 0) list.splice(i, 1);
      Object.defineProperty(o, "$dead", { value: true });
    },
    // Called in every loop body: stops a loop with no Sync from freezing the page forever.
    guard() {
      if ((++ops & 0xffff) === 0 && performance.now() - frameStart > 5000) {
        throw new Error("this frame has run for over 5 seconds. Is a loop missing `Sync`?");
      }
    },

    // Drives the compiled generator: one resume per frame, `Sync` = yield (PLAN.md §4.3).
    run(main) {
      const prog = main();
      let last = performance.now();
      const frame = (now) => {
        cb.delta = Math.min((now - last) / 1000, 0.1); // clamp so a stalled tab doesn't teleport objects
        last = now;
        frameStart = performance.now();
        if (mode === 3) ctx.clearRect(0, 0, ctx.canvas.width, ctx.canvas.height); // HUD redraws each frame
        let done;
        try {
          done = prog.next().done;
        } catch (err) {
          if (err !== END) {
            console.error(err);
            return say(`Runtime error on line ${cb.ln}: ${err.message}`, true);
          }
          done = true;
        }
        keyHits.clear();
        buttonHits.clear();
        if (scene?.activeCamera) { engine.beginFrame(); scene.render(); engine.endFrame(); }
        if (done) return say("Program ended.");
        requestAnimationFrame(frame);
      };
      requestAnimationFrame(frame);
    },
  };

  for (const c of "ABCDEFGHIJKLMNOPQRSTUVWXYZ") cb["KEY_" + c] = "Key" + c;
  for (const d of "0123456789") cb["KEY_" + d] = "Digit" + d;
  return cb;
})();

// CyberBasic runtime, Phase 0. A classic script (not a module) so exported games open
// straight from disk over file://.
// shortcut: plain JS instead of the planned TypeScript; port once the runtime outgrows one file.
"use strict";
if (typeof BABYLON === "undefined") {
  document.body.insertAdjacentHTML("beforeend",
    '<pre class="cb-msg err">Babylon.js failed to load. Phase 0 games load it from cdn.babylonjs.com, so they need internet.</pre>');
  throw new Error("Babylon.js not loaded");
}

const CB = (() => {
  const B = BABYLON, D2R = Math.PI / 180, V = (x, y, z) => new B.Vector3(x, y, z);
  const down = new Set(), hit = new Set();
  let engine = null, scene = null;

  addEventListener("keydown", (e) => {
    if (!e.repeat) hit.add(e.code);
    down.add(e.code);
    if (e.code.startsWith("Arrow") || e.code === "Space") e.preventDefault();
  });
  addEventListener("keyup", (e) => down.delete(e.code));
  addEventListener("blur", () => down.clear());

  const need = () => { if (!scene) throw new Error("call Graphics3D before using 3D commands"); };
  const node = (name) => { need(); return new B.TransformNode(name, scene); };
  const mesh = (m) => { m.material = new B.StandardMaterial(m.name, scene); return m; };
  const say = (text, err) => {
    const p = document.createElement("pre");
    p.className = err ? "cb-msg err" : "cb-msg";
    p.textContent = text;
    document.body.append(p);
  };

  const cb = {
    delta: 0, // seconds since the last frame
    ln: 0,    // current BASIC line, set by generated code

    KEY_ESCAPE: "Escape", KEY_SPACE: "Space", KEY_ENTER: "Enter",
    KEY_LEFT: "ArrowLeft", KEY_RIGHT: "ArrowRight", KEY_UP: "ArrowUp", KEY_DOWN: "ArrowDown",
    LIGHT_DIRECTIONAL: 1, LIGHT_POINT: 2,

    Game(title) { document.title = String(title); },

    Graphics3D(w, h) {
      const canvas = document.createElement("canvas");
      canvas.width = w; canvas.height = h;
      canvas.style.width = w + "px"; canvas.style.height = h + "px";
      document.body.prepend(canvas);
      engine = new B.Engine(canvas, true);
      scene = new B.Scene(engine);
      scene.clearColor = new B.Color4(0.05, 0.05, 0.08, 1);
      new B.HemisphericLight("ambient", V(0, 1, 0), scene).intensity = 0.3;
    },

    // Cameras and lights sit under a TransformNode so every *Entity command works on them too.
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
    CreateCube(size = 1) { need(); return mesh(B.MeshBuilder.CreateBox("cube", { size }, scene)); },
    CreateSphere(size = 1) { need(); return mesh(B.MeshBuilder.CreateSphere("sphere", { diameter: size }, scene)); },
    CreatePlane(size = 100) { need(); return mesh(B.MeshBuilder.CreateGround("plane", { width: size, height: size }, scene)); },

    PositionEntity(e, x, y, z) { e.position.set(x, y, z); },
    RotateEntity(e, pitch, yaw, roll) { e.rotation.set(pitch * D2R, yaw * D2R, roll * D2R); },
    MoveEntity(e, x, y, z) { e.locallyTranslate(V(x, y, z)); },
    TurnEntity(e, pitch, yaw, roll) { e.addRotation(pitch * D2R, yaw * D2R, roll * D2R); },
    PointEntity(e, x, y, z) { e.lookAt(V(x, y, z)); },
    EntityColor(e, r, g, b) {
      if (!e.material) throw new Error("EntityColor needs a mesh (cube, sphere, plane)");
      e.material.diffuseColor = new B.Color3(r / 255, g / 255, b / 255);
    },

    KeyDown(k) { return down.has(k); },
    KeyHit(k) { return hit.has(k); },
    Print(v) { console.log(String(v)); },

    // Drives the compiled generator: one resume per frame, `Sync` = yield (PLAN.md §4.3).
    run(main) {
      const prog = main();
      let last = performance.now();
      const frame = (now) => {
        cb.delta = Math.min((now - last) / 1000, 0.1); // clamp so a stalled tab doesn't teleport objects
        last = now;
        let done;
        try {
          done = prog.next().done;
        } catch (err) {
          console.error(err);
          return say(`Runtime error on line ${cb.ln}: ${err.message}`, true);
        }
        hit.clear();
        if (scene?.activeCamera) { engine.beginFrame(); scene.render(); engine.endFrame(); }
        if (done) return say("Program ended.");
        requestAnimationFrame(frame);
      };
      requestAnimationFrame(frame);
    },
  };

  // KEY_A..KEY_Z, KEY_0..KEY_9
  for (const c of "ABCDEFGHIJKLMNOPQRSTUVWXYZ") cb["KEY_" + c] = "Key" + c;
  for (const d of "0123456789") cb["KEY_" + d] = "Digit" + d;
  return cb;
})();

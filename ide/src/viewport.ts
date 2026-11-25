// The 3D view shared by the level editor and the modeller: Babylon (the IDE's own copy), an orbit camera,
// a ground grid, click-to-select and move/rotate/scale gizmos.
import { useEffect, useRef, useState, type RefObject } from "react";
import { LIB_URLS } from "./libs";

// shortcut: Babylon is untyped here (loaded as a script like the games, not bundled); type it if the editors grow.
// eslint-disable-next-line @typescript-eslint/no-explicit-any
export type Any = any;
export type GizmoMode = "move" | "rotate" | "scale";
export const D2R = Math.PI / 180;
export const round = (n: number, step = 1e-3) => Math.round(n / step) * step;
export type Vec3 = [number, number, number];

const scripts = new Map<string, Promise<void>>();
function script(url: string) {
  if (!scripts.has(url)) {
    scripts.set(
      url,
      new Promise((ok, fail) => {
        const s = document.createElement("script");
        s.src = url;
        s.onload = () => ok();
        s.onerror = () => fail(new Error(`Couldn't load ${url}`));
        document.head.append(s);
      }),
    );
  }
  return scripts.get(url)!;
}

export interface View { B: Any; engine: Any; scene: Any; camera: Any; gizmos: Any }

/**
 * Sets up the 3D view on `canvas`. Clicking a mesh whose (parent's) metadata has `pickId` calls
 * `onPick(pickId)`; clicking empty space calls `onPick(undefined)`.
 */
export function useViewport(canvas: RefObject<HTMLCanvasElement | null>, onPick: (id?: string) => void, { distance = 24, gridSize = 30, targetY = 0 } = {}) {
  const view = useRef<View>(null);
  const [ready, setReady] = useState(false);
  const [error, setError] = useState<string>();
  const pick = useRef(onPick);
  pick.current = onPick;

  useEffect(() => {
    let disposed = false, observer: ResizeObserver | undefined;
    (async () => {
      try {
        await script(LIB_URLS["babylon.js"]);
        await script(LIB_URLS["babylonjs.loaders.min.js"]);
      } catch (e) {
        return setError((e as Error).message);
      }
      if (disposed || !canvas.current) return;
      const B = (window as Any).BABYLON;
      const engine = new B.Engine(canvas.current, true, { preserveDrawingBuffer: true });
      const s = new B.Scene(engine);
      s.clearColor = new B.Color4(0.07, 0.08, 0.11, 1);
      const camera = new B.ArcRotateCamera("editor", -Math.PI / 2.4, 1.15, distance, new B.Vector3(0, targetY, 0), s);
      camera.attachControl(canvas.current, true);
      camera.wheelPrecision = 720 / distance;
      camera.panningSensibility = 1440 / distance;
      camera.minZ = 0.02;
      new B.HemisphericLight("editor-light", new B.Vector3(0.3, 1, -0.4), s).intensity = 1.1;
      // Ground grid: a line every unit, brighter every 5.
      const lines: Any[] = [], colors: Any[] = [], g = gridSize;
      for (let i = -g; i <= g; i++) {
        const c = i === 0 ? new B.Color4(0.35, 0.55, 0.8, 1) : i % 5 ? new B.Color4(0.18, 0.2, 0.26, 1) : new B.Color4(0.26, 0.29, 0.37, 1);
        lines.push([new B.Vector3(i, 0, -g), new B.Vector3(i, 0, g)], [new B.Vector3(-g, 0, i), new B.Vector3(g, 0, i)]);
        colors.push([c, c], [c, c]);
      }
      const grid = B.MeshBuilder.CreateLineSystem("editor-grid", { lines, colors }, s);
      grid.isPickable = false;
      const gizmos = new B.GizmoManager(s);
      gizmos.usePointerToAttachGizmos = false;
      gizmos.clearGizmoOnEmptyPointerEvent = false;
      view.current = { B, engine, scene: s, camera, gizmos };
      engine.runRenderLoop(() => s.render());
      observer = new ResizeObserver(() => engine.resize());
      observer.observe(canvas.current);
      s.onPointerObservable.add((info: Any) => {
        if (info.type !== B.PointerEventTypes.POINTERTAP || info.event.button !== 0) return;
        let m = info.pickInfo?.pickedMesh;
        while (m && m.metadata?.pickId === undefined) m = m.parent;
        pick.current(m?.metadata?.pickId);
      });
      setReady(true);
    })();
    return () => {
      disposed = true;
      observer?.disconnect();
      view.current?.engine.dispose();
      view.current = null;
    };
  }, []);

  return { view, ready, error };
}

/** Marks a node and its child meshes as picking `id`. */
export function pickable(node: Any, id: string) {
  node.metadata = { ...node.metadata, pickId: id };
  for (const m of node.getChildMeshes?.() ?? []) m.metadata = { ...m.metadata, pickId: id };
}

/** Reads a node's local transform as saved numbers (degrees for rotation). */
export function transformOf(node: Any): { position: Vec3; rotation: Vec3; scale: Vec3 } {
  const r = node.rotationQuaternion ? node.rotationQuaternion.toEulerAngles() : node.rotation;
  return {
    position: [round(node.position.x), round(node.position.y), round(node.position.z)],
    rotation: [round(r.x / D2R, 0.1), round(r.y / D2R, 0.1), round(r.z / D2R, 0.1)],
    scale: [round(node.scaling.x), round(node.scaling.y), round(node.scaling.z)],
  };
}
/** Sets a node's local transform from saved numbers. */
export function setTransform(B: Any, node: Any, position: Vec3, rotation: Vec3, scale?: Vec3) {
  node.position.set(...position);
  const r = rotation.map((d) => d * D2R) as Vec3;
  if (node.rotationQuaternion) node.rotationQuaternion = B.Quaternion.FromEulerAngles(...r);
  else node.rotation.set(...r);
  if (scale) node.scaling.set(...scale);
}

/**
 * Puts the gizmo for `mode` on `node` (or none), with snapping, and calls `onDone(node)` when a
 * drag ends. `deps` re-run it (e.g. when the node is rebuilt).
 */
export function useGizmo(view: RefObject<View | null>, node: Any, opts: { mode: GizmoMode; snap: boolean; canScale: boolean; local?: boolean }, onDone: (node: Any) => void, deps: unknown[]) {
  const done = useRef(onDone);
  done.current = onDone;
  useEffect(() => {
    const v = view.current;
    if (!v) return;
    const { gizmos } = v;
    gizmos.positionGizmoEnabled = opts.mode === "move";
    gizmos.rotationGizmoEnabled = opts.mode === "rotate";
    gizmos.scaleGizmoEnabled = opts.mode === "scale" && opts.canScale;
    gizmos.attachToNode(node ?? null);
    const boxed = node ? [node, ...node.getChildMeshes()].filter((m: Any) => m.getTotalVertices?.() > 0) : [];
    for (const m of boxed) m.showBoundingBox = true;
    const g = gizmos.gizmos;
    if (g.positionGizmo) {
      g.positionGizmo.snapDistance = opts.snap ? 0.5 : 0;
      g.positionGizmo.updateGizmoRotationToMatchAttachedMesh = !!opts.local;
    }
    if (g.rotationGizmo) {
      g.rotationGizmo.snapDistance = opts.snap ? 15 * D2R : 0;
      g.rotationGizmo.updateGizmoRotationToMatchAttachedMesh = !!opts.local;
    }
    if (g.scaleGizmo) g.scaleGizmo.snapDistance = opts.snap ? 0.25 : 0;
    const end = () => node && done.current(node);
    const subs = [g.positionGizmo, g.rotationGizmo, g.scaleGizmo].filter(Boolean).map((z: Any) => [z, z.onDragEndObservable.add(end)]);
    return () => {
      subs.forEach(([z, o]) => z.onDragEndObservable.remove(o));
      for (const m of boxed) if (!m.isDisposed()) m.showBoundingBox = false;
    };
  }, [node, opts.mode, opts.snap, opts.canScale, ...deps]);
}

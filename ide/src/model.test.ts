// Run with `npm test`. Model hierarchy and animation keys from the modeller.
import { test } from "node:test";
import assert from "node:assert/strict";
import { emptyModel, partTree, poseAt, removeKey, sampleKeys, setKey, withChildren, type Clip, type Key } from "./model.ts";

const key = (t: number, x: number): Key => ({ t, position: [x, 0, 0], rotation: [0, x * 10, 0], scale: [1, 1, 1] });

test("keys blend between each other and hold before the first and after the last", () => {
  const keys = [key(0, 0), key(1, 2), key(2, 2)];
  assert.deepEqual(sampleKeys(keys, 0.5).position, [1, 0, 0]);
  assert.deepEqual(sampleKeys(keys, 0.5).rotation, [0, 10, 0]);
  assert.deepEqual(sampleKeys(keys, -1).position, [0, 0, 0]);
  assert.deepEqual(sampleKeys(keys, 9).position, [2, 0, 0]);
});

test("setting a key at a time replaces the key there and keeps keys in time order", () => {
  let c: Clip = { name: "walk", length: 1, loop: true, tracks: {} };
  c = setKey(c, "p1", key(0.5, 1));
  c = setKey(c, "p1", key(0, 0));
  c = setKey(c, "p1", key(0.502, 3)); // within 0.005 of 0.5: the same key
  assert.deepEqual(c.tracks.p1.map((k) => [k.t, k.position[0]]), [[0, 0], [0.502, 3]]);
  c = removeKey(c, "p1", 0);
  c = removeKey(c, "p1", 0.5);
  assert.equal(c.tracks.p1, undefined); // no keys left: no track
});

test("the starter robot: joints carry their limbs, and walking swings the arms", () => {
  const m = emptyModel();
  const tree = partTree(m).map(({ part, depth }) => `${"  ".repeat(depth)}${part.name}`);
  assert.equal(tree[0], "Hips");
  assert.ok(tree.includes("    Left arm"), tree.join("\n"));
  assert.deepEqual([...withChildren(m, "p4")], ["p4", "p5"]); // Left shoulder and the arm on it
  const walk = m.animations.find((c) => c.name === "walk")!;
  assert.equal(poseAt(m, walk, 0).get("p4")!.rotation[0], 35);
  assert.equal(poseAt(m, walk, 0.4).get("p4")!.rotation[0], -35);
  assert.deepEqual(poseAt(m, walk, 0.2).get("p5"), m.parts.find((p) => p.id === "p5")); // the arm itself has no keys
});

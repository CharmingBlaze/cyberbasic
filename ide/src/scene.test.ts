// Run with `npm test`. Terrain data and brushes from the level editor.
import { test } from "node:test";
import assert from "node:assert/strict";
import { dab, decodeColors, decodeHeights, encodeColors, encodeHeights } from "./scene.ts";

/** A flat (n+1) x (n+1) grid of points one unit apart, centred on 0 like Babylon's ground. */
const grid = (n: number) => {
  const pos: number[] = [];
  for (let r = 0; r <= n; r++) for (let c = 0; c <= n; c++) pos.push(c - n / 2, 0, n / 2 - r);
  return pos;
};
const ys = (pos: number[]) => pos.filter((_, i) => i % 3 === 1);
const brush = { tool: "raise" as const, radius: 1.5, strength: 1, color: [255, 0, 0] as [number, number, number], target: 0 };

test("terrain heights and colors survive saving, and flat or unpainted saves as nothing", () => {
  assert.equal(encodeHeights([0, 0, 0]), "");
  assert.equal(encodeHeights([0, 1.234, -2]), "0,1.23,-2");
  assert.deepEqual(Array.from(decodeHeights("0,1.23,-2", 4)), [0, Math.fround(1.23), -2, 0]);
  const c = decodeColors("", 2);
  assert.deepEqual(Array.from(c), [0x6e, 0xa0, 0x5a, 0x6e, 0xa0, 0x5a]);
  assert.equal(encodeColors(c), "");
  c[0] = 255;
  assert.equal(encodeColors(c), "ffa05a6ea05a");
});

test("brushes change only the points under them, with a soft edge", () => {
  const n = 4, pos = grid(n), rgb = decodeColors("", 25);
  assert.ok(dab(pos, rgb, n, 0, 0, brush));
  const h = ys(pos);
  assert.ok(h[12] > h[11] && h[11] > 0, "centre highest, neighbours lower"); // 12 is the middle point
  assert.equal(h[0], 0); // a corner is outside the radius
  assert.equal(dab(pos, rgb, n, 50, 50, brush), false); // off the terrain

  dab(pos, rgb, n, 0, 0, { ...brush, tool: "flatten", strength: 1, target: 0 });
  assert.equal(ys(pos)[12], 0);

  dab(pos, rgb, n, 0, 0, { ...brush, tool: "paint" });
  assert.deepEqual(Array.from(rgb.slice(36, 39)), [255, 0, 0]); // the centre is fully painted
  assert.deepEqual(Array.from(rgb.slice(0, 3)), [0x6e, 0xa0, 0x5a]);

  const bump = grid(n);
  bump[12 * 3 + 1] = 4;
  dab(bump, rgb, n, 0, 0, { ...brush, tool: "smooth", radius: 0.5 });
  assert.equal(ys(bump)[12], 0); // a lone spike smooths to its neighbours' level
});

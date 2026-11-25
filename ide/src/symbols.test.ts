// Run with `npm test` (Node's built-in test runner; Node strips the TypeScript types).
import { test } from "node:test";
import assert from "node:assert/strict";
import { readFileSync, readdirSync } from "node:fs";
import { definition, format, occurrences, scan } from "./symbols.ts";

const examples = new URL("../../examples/", import.meta.url);

test("formatting leaves every hand-formatted example unchanged", () => {
  for (const f of readdirSync(examples).filter((f) => f.endsWith(".cb"))) {
    const src = readFileSync(new URL(f, examples), "utf8").replace(/\r\n/g, "\n");
    assert.equal(format(src), src, f);
  }
});

test("formatting fixes indentation of nested blocks, Select and one-line blocks", () => {
  const messy = "While 1\nIf x\nSelect x\nCase 1\nPrint 1\nDefault\nPrint 2\nEnd Select\nElse\nFor i = 1 To 2 : Print i : Next\nEndIf\nWend";
  const tidy = "While 1\n    If x\n        Select x\n            Case 1\n                Print 1\n            Default\n                Print 2\n        End Select\n    Else\n        For i = 1 To 2 : Print i : Next\n    EndIf\nWend";
  assert.equal(format(messy), tidy);
});

const SRC = `Global score
x = 1
Print x + score
Function F(a)
    x = a
    score += x
End Function
Type Enemy
    Field x, hp
End Type
e = New Enemy : e.x = 5`;

test("scan finds functions, types and declarations", () => {
  const s = scan(SRC);
  assert.deepEqual(s.funcs.map((f) => [f.name, f.line, f.endLine, f.params]), [["F", 4, 7, ["a"]]]);
  assert.deepEqual(s.types[0].fields.map((f) => f.name), ["x", "hp"]);
  assert.deepEqual(s.decls, [{ name: "score", line: 1, kind: "global" }]);
});

test("references respect scope: main's x, F's x and the field x are different", () => {
  const lines = (name: string, at: number) => occurrences(SRC, name, at).map((o) => o.line);
  assert.deepEqual(lines("x", 2), [2, 3]); // main program (field e.x on line 11 excluded)
  assert.deepEqual(lines("x", 5), [5, 6]); // inside F
  assert.deepEqual(lines("score", 3), [1, 3, 6]); // Global: everywhere
});

test("go to definition", () => {
  assert.deepEqual(definition(SRC, "F", 1), { line: 4, col: 1 });
  assert.deepEqual(definition(SRC, "score", 6), { line: 1, col: 1 });
  assert.deepEqual(definition(SRC, "a", 5), { line: 4, col: 1 });
  assert.deepEqual(definition(SRC, "x", 3), { line: 2, col: 1 });
  assert.deepEqual(definition(SRC, "Enemy", 11), { line: 8, col: 1 });
});

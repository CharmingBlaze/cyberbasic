// Event sheets: visual scripts made of events ("when these conditions hold, do these actions").
// A sheet is stored as JSON in a .events file and turned into CyberBasic code, which the normal
// compiler builds. A line map ties errors, breakpoints and the debugger back to events and actions.
// shortcut: sheets become BASIC text rather than the shared IR (PLAN.md §7); fine while the
// compiler is AST -> JS, revisit when the IR exists.

export type Condition =
  | { kind: "call"; name: string; args: string[]; not?: boolean } // any command that returns a value
  | { kind: "compare"; left: string; op: string; right: string; not?: boolean }
  | { kind: "every"; seconds: string; not?: boolean }
  | { kind: "expr"; text: string; not?: boolean };

export type Action =
  | { kind: "call"; name: string; args: string[] } // any statement command
  | { kind: "set"; variable: string; op: "=" | "+=" | "-="; value: string }
  | { kind: "code"; text: string }; // a line of CyberBasic (call your own functions, etc.)

export interface SheetEvent {
  id: number;
  trigger: "start" | "frame";
  comment?: string;
  disabled?: boolean;
  breakpoint?: boolean;
  conditions: Condition[];
  actions: Action[];
}

export interface Sheet {
  version: 1;
  variables: { name: string; value: string }[];
  includes: string[]; // code files this sheet Includes (when it is the main file)
  events: SheetEvent[];
}

/** Where a generated line came from: an event (its `If` line) or one of its actions. */
export interface LineInfo { event: number; action?: number }

export const COMPARE_OPS = ["=", "<>", "<", "<=", ">", ">="];

export function emptySheet(main: boolean): Sheet {
  return {
    version: 1,
    variables: [],
    includes: [],
    events: main
      ? [
          { id: 1, trigger: "start", conditions: [], actions: [{ kind: "call", name: "Graphics", args: ["800", "600"] }] },
          { id: 2, trigger: "frame", conditions: [], actions: [{ kind: "call", name: "Cls", args: [] }] },
        ]
      : [],
  };
}

export function parseSheet(text: string): Sheet {
  try {
    const s = JSON.parse(text) as Partial<Sheet>;
    return { version: 1, variables: s.variables ?? [], includes: s.includes ?? [], events: s.events ?? [] };
  } catch {
    return emptySheet(false);
  }
}

export const sheetText = (s: Sheet) => JSON.stringify(s, null, 2) + "\n";

/** `player.events` -> `Sheet_player`: the prefix of the functions a sheet generates. */
export const sheetPrefix = (file: string) => "Sheet_" + file.replace(/\.events$/i, "").replace(/\W/g, "_");

/** Arguments as code: trailing empty optional ones are dropped; empty required ones stay empty
 * so the compiler reports them on the right action. */
function argList(args: string[], required: number) {
  const out = [...args];
  while (out.length > required && !(out[out.length - 1] ?? "").trim()) out.pop();
  return out.map((a) => a.trim()).join(", ");
}

export function conditionCode(c: Condition, timer: string, returns: (name: string) => number): string {
  let code: string;
  if (c.kind === "call") code = `${c.name}(${argList(c.args, returns(c.name))})`;
  else if (c.kind === "compare") code = `(${c.left.trim()}) ${c.op} (${c.right.trim()})`;
  else if (c.kind === "every") code = `${timer} >= ${c.seconds.trim() || "1"}`;
  else code = `(${c.text.trim()})`;
  return c.not ? `Not ${code}` : code;
}

export function actionCode(a: Action, required: (name: string) => number): string {
  if (a.kind === "call") {
    const args = argList(a.args, required(a.name));
    return args ? `${a.name} ${args}` : a.name;
  }
  if (a.kind === "set") return `${a.variable.trim()} ${a.op} ${a.value.trim()}`;
  return a.text.trim();
}

/**
 * Turns a sheet into CyberBasic. Start events run once, frame events every frame, in order.
 * `required(name)` says how many arguments a command needs. When the sheet is the main file
 * it also gets a game loop (Escape quits).
 */
export function generate(file: string, sheet: Sheet, isMain: boolean, required: (name: string) => number) {
  const lines: string[] = [];
  const map = new Map<number, LineInfo>(); // 1-based generated line -> source
  const emit = (text: string, info?: LineInfo) => {
    lines.push(text);
    if (info) map.set(lines.length, info);
  };
  const prefix = sheetPrefix(file);
  const timer = (e: SheetEvent) => `${prefix.toLowerCase()}_timer${e.id}`;

  emit(`' Generated from ${file} by the event sheet editor. Edit the sheet, not this code.`);
  for (const inc of sheet.includes) emit(`Include "${inc}"`);
  for (const v of sheet.variables) if (v.name.trim()) emit(`Global ${v.name.trim()} = ${v.value.trim() || "0"}`);
  const timers = sheet.events.filter((e) => !e.disabled && e.conditions.some((c) => c.kind === "every"));
  if (timers.length) emit(`Global ${timers.map(timer).join(", ")}`);

  for (const trigger of ["start", "frame"] as const) {
    emit("");
    emit(`Function ${prefix}_${trigger === "start" ? "Start" : "Frame"}()`);
    sheet.events.forEach((e, index) => {
      if (e.trigger !== trigger) return;
      emit(`    ' Event ${index + 1}${e.comment ? `: ${e.comment.replace(/\n/g, " ")}` : ""}${e.disabled ? " (disabled)" : ""}`);
      if (e.disabled) return;
      const every = e.conditions.some((c) => c.kind === "every");
      if (every) emit(`    ${timer(e)} += Delta`, { event: e.id });
      const conds = e.conditions.map((c) => conditionCode(c, timer(e), required));
      const indent = conds.length ? "        " : "    ";
      if (conds.length) emit(`    If ${conds.join(" And ")}`, { event: e.id });
      if (every) emit(`${indent}${timer(e)} = 0`, { event: e.id });
      e.actions.forEach((a, i) => emit(indent + actionCode(a, required), { event: e.id, action: i }));
      if (conds.length) emit("    EndIf", { event: e.id });
    });
    emit("End Function");
  }

  if (isMain) {
    emit("");
    emit(`${prefix}_Start`);
    emit("While Not KeyHit(KEY_ESCAPE)");
    emit(`    ${prefix}_Frame`);
    emit("    Sync");
    emit("Wend");
  }
  return { code: lines.join("\n") + "\n", map };
}

/** The generated line a breakpoint on an event should sit on: its `If`, else its first line. */
export function eventLine(map: Map<number, LineInfo>, eventId: number) {
  for (const [line, info] of map) if (info.event === eventId && info.action === undefined) return line;
  for (const [line, info] of map) if (info.event === eventId) return line;
  return undefined;
}

// Source analysis for editor features (outline, go to definition, references, rename, format).
// shortcut: line-based scan in TypeScript; move to the Rust compiler's CST (Rowan) once it has one.

export interface Block { name: string; line: number; endLine: number }
export interface Func extends Block { params: string[] }
export interface TypeDef extends Block { fields: { name: string; line: number }[] }
export interface Decl { name: string; line: number; kind: "global" | "const" | "array" }
export interface Scan { funcs: Func[]; types: TypeDef[]; decls: Decl[] }

const IDENT = /[A-Za-z_]\w*/;

/** The code part of a line: string contents blanked out and the comment removed. */
export function codeOf(line: string) {
  let out = "", inStr = false;
  for (const ch of line) {
    if (ch === '"') inStr = !inStr;
    else if (ch === "'" && !inStr) break;
    out += inStr && ch !== '"' ? " " : ch;
  }
  return out;
}

export function scan(text: string): Scan {
  const lines = text.split("\n");
  const funcs: Func[] = [], types: TypeDef[] = [], decls: Decl[] = [];
  let fn: Func | null = null, ty: TypeDef | null = null;
  lines.forEach((raw, i) => {
    const line = i + 1, code = codeOf(raw).trim();
    let m: RegExpMatchArray | null;
    if ((m = code.match(/^Function\s+([A-Za-z_]\w*)\s*(?:\(([^)]*)\))?/i))) {
      const params = (m[2] ?? "").split(",").map((p) => p.trim().replace(/[#%$]$/, "")).filter(Boolean);
      fn = { name: m[1], line, endLine: lines.length, params };
      funcs.push(fn);
    } else if (/^End\s+Function\b/i.test(code) && fn) {
      fn.endLine = line;
      fn = null;
    } else if ((m = code.match(/^Type\s+([A-Za-z_]\w*)/i))) {
      ty = { name: m[1], line, endLine: lines.length, fields: [] };
      types.push(ty);
    } else if (/^End\s+Type\b/i.test(code) && ty) {
      ty.endLine = line;
      ty = null;
    } else if (ty && (m = code.match(/^Field\s+(.+)/i))) {
      for (const f of m[1].split(",")) if (IDENT.test(f.trim())) ty.fields.push({ name: f.trim().replace(/[#%$]$/, ""), line });
    } else if (!fn && (m = code.match(/^(Global|Const)\s+(.+)/i))) {
      for (const part of m[2].split(",")) {
        const name = part.trim().match(IDENT)?.[0];
        if (name) decls.push({ name, line, kind: m[1].toLowerCase() === "const" ? "const" : "global" });
      }
    } else if (!fn && (m = code.match(/^Dim\s+([A-Za-z_]\w*)/i))) {
      decls.push({ name: m[1], line, kind: "array" });
    }
  });
  return { funcs, types, decls };
}

const eq = (a: string, b: string) => a.toLowerCase() === b.toLowerCase();

/** The function containing a line, or null for the main program. */
export const scopeAt = (s: Scan, line: number) => s.funcs.find((f) => line >= f.line && line <= f.endLine) ?? null;

/** Whether a name is visible everywhere (function, type, Global, Const, top-level Dim). */
export function isGlobalName(s: Scan, name: string) {
  return s.funcs.some((f) => eq(f.name, name)) || s.types.some((t) => eq(t.name, name)) || s.decls.some((d) => eq(d.name, name));
}

export interface Occurrence { line: number; col: number; length: number }

/** Every use of `name` as an identifier (not inside strings, comments, or after a `.`), limited to
 * its scope. `forceGlobal` is for names declared in another file of the project. */
export function occurrences(text: string, name: string, atLine: number, forceGlobal?: boolean): Occurrence[] {
  const s = scan(text);
  const global = forceGlobal ?? isGlobalName(s, name);
  const scope = global ? null : scopeAt(s, atLine);
  const inScope = (line: number) => {
    if (s.types.some((t) => line > t.line && line < t.endLine)) return false; // Field names are not variables
    if (global) return true;
    const f = scopeAt(s, line);
    return scope ? f === scope : !f; // a local of main is not visible inside functions, and vice versa
  };
  const out: Occurrence[] = [];
  text.split("\n").forEach((raw, i) => {
    const line = i + 1;
    if (!inScope(line)) return;
    const code = codeOf(raw);
    for (const m of code.matchAll(/[A-Za-z_]\w*/g)) {
      if (!eq(m[0], name)) continue;
      if (code[m.index! - 1] === ".") continue; // a field, not this variable
      out.push({ line, col: m.index! + 1, length: m[0].length });
    }
  });
  return out;
}

/** Where `name` (used on `atLine`) is declared, or first assigned if it's an undeclared local. */
export function definition(text: string, name: string, atLine: number): { line: number; col: number } | null {
  const s = scan(text);
  const f = s.funcs.find((x) => eq(x.name, name));
  if (f) return { line: f.line, col: 1 };
  const t = s.types.find((x) => eq(x.name, name));
  if (t) return { line: t.line, col: 1 };
  const d = s.decls.find((x) => eq(x.name, name));
  if (d) return { line: d.line, col: 1 };
  const scope = scopeAt(s, atLine);
  if (scope?.params.some((p) => eq(p, name))) return { line: scope.line, col: 1 };
  // First assignment in the same scope.
  const first = occurrences(text, name, atLine).find((o) => {
    const rest = codeOf(text.split("\n")[o.line - 1]).slice(o.col - 1 + o.length);
    return /^\s*(=|\+=|-=|\[)/.test(rest) || /^\s*For\s*$/i.test(codeOf(text.split("\n")[o.line - 1]).slice(0, o.col - 1));
  });
  return first ? { line: first.line, col: first.col } : null;
}

// ---------------------------------------------------------------- formatting

const OPENS = /^(if\b(?!.*\bthen\s*\S)|while\b|for\b|repeat\b|function\b|type\b)/i;
const CLOSES = /^(endif\b|end\s+(if|function|type)\b|wend\b|next\b|until\b|forever\b)/i;
const MIDDLE = /^(else\b|elseif\b)/i;

/** Re-indents a program by its block structure. Content of lines is left as written. */
export function format(text: string, indent = "    "): string {
  let level = 0;
  const selects: { caseOpen: boolean }[] = [];
  /** Applies one statement's effect on nesting; returns the level that statement sits at. */
  const step = (code: string) => {
    let at = level;
    if (/^select\b/i.test(code)) {
      selects.push({ caseOpen: false });
      level++;
    } else if (/^(case|default)\b/i.test(code) && selects.length) {
      const top = selects[selects.length - 1];
      if (top.caseOpen) level--;
      at = level;
      top.caseOpen = true;
      level++;
    } else if (/^end\s+select\b/i.test(code) && selects.length) {
      if (selects.pop()!.caseOpen) level--;
      level--;
      at = level;
    } else if (CLOSES.test(code)) {
      level--;
      at = level;
    } else if (MIDDLE.test(code)) {
      at = level - 1;
    } else if (OPENS.test(code)) {
      level++;
    }
    level = Math.max(0, level);
    return at;
  };
  return text
    .split("\n")
    .map((raw) => {
      const body = raw.trim();
      if (!body) return "";
      // `For i = 1 To 3 : Print i : Next` opens and closes on one line; the first statement sets the indent.
      const statements = codeOf(body).split(":").map((s) => s.trim()).filter(Boolean);
      const singleLineIf = /^if\b.*\bthen\s*\S/i.test(statements[0] ?? ""); // the rest of the line belongs to it
      const at = statements.length ? step(statements[0]) : level;
      if (!singleLineIf) statements.slice(1).forEach(step);
      return indent.repeat(Math.max(0, at)) + body;
    })
    .join("\n");
}

// CyberBasic support for Monaco: highlighting, completion, hover docs, signature help, theme.
import * as monaco from "./monaco";
import EditorWorker from "monaco-editor/editor/editor.worker?worker";
import { manifest, findCommand, type Command } from "./compiler";
import { get } from "./store";
import { definition, format, isGlobalName, occurrences, scan, type Occurrence } from "./symbols";

/** Every CyberBasic file open in the project (one Monaco model each). */
const projectModels = () => monaco.editor.getModels().filter((m) => m.getLanguageId() === "cyberbasic");
const allText = () => projectModels().map((m) => m.getValue()).join("\n");
/** Functions, types, Globals and Consts are visible from every file (Include splices files together). */
const globalInProject = (name: string) => projectModels().some((m) => isGlobalName(scan(m.getValue()), name));

self.MonacoEnvironment = { getWorker: () => new EditorWorker() };

const LANG = "cyberbasic";
const PROPER: Record<string, string> = { endif: "EndIf", elseif: "ElseIf", wend: "Wend" };
const proper = (k: string) => PROPER[k] ?? k[0].toUpperCase() + k.slice(1);

/** Splits "x, y[, fill]" into argument names, and how many are required. */
function parseArgs(args: string) {
  const required = args.split("[")[0].split(",").map((a) => a.trim()).filter(Boolean);
  const all = args.replace(/[[\]]/g, "").split(",").map((a) => a.trim()).filter(Boolean);
  return { required, all };
}

export const signature = (c: Command) => (c.returns ? `${c.name}(${c.args})` : `${c.name} ${c.args}`.trim());

/** Snippet that inserts a call with placeholders for the required arguments. */
export function callSnippet(c: Command) {
  const ph = parseArgs(c.args).required.map((a, i) => `\${${i + 1}:${a}}`).join(", ");
  return c.returns ? `${c.name}(${ph})` : ph ? `${c.name} ${ph}` : c.name;
}

const BLOCKS: [string, string, string][] = [
  ["If", "If ${1:condition}\n\t$0\nEndIf", "If … EndIf block"],
  ["If Else", "If ${1:condition}\n\t$2\nElse\n\t$0\nEndIf", "If … Else … EndIf"],
  ["For", "For ${1:i} = ${2:1} To ${3:10}\n\t$0\nNext", "Counting loop"],
  ["For Each", "For ${1:item} = Each ${2:TypeName}\n\t$0\nNext", "Loop over every object of a Type"],
  ["While", "While ${1:condition}\n\t$0\nWend", "Loop while a condition holds"],
  ["Repeat", "Repeat\n\t$0\nUntil ${1:condition}", "Loop until a condition holds"],
  ["Function", "Function ${1:Name}(${2})\n\t$0\nEnd Function", "Define a function"],
  ["Select", "Select ${1:value}\nCase ${2:1}\n\t$0\nDefault\n\nEnd Select", "Choose by value"],
  ["Type", "Type ${1:Name}\n\tField ${2:x, y}\nEnd Type", "Define an object type"],
  ["Game loop", "While Not KeyHit(KEY_ESCAPE)\n\t$0\n\tCls\n\tSync\nWend", "Main loop that runs every frame"],
];

/** Functions, types and variables declared in the source, for completion and hover. */
export function userSymbols(text: string) {
  const funcs = new Map<string, string>();
  for (const m of text.matchAll(/^\s*Function\s+([A-Za-z_]\w*)\s*(?:\(([^)]*)\))?/gim)) funcs.set(m[1], m[2] ?? "");
  const types = new Set([...text.matchAll(/^\s*Type\s+([A-Za-z_]\w*)/gim)].map((m) => m[1]));
  const vars = new Set<string>();
  for (const m of text.matchAll(/(?:^|:|\bThen)\s*(?:Global\s+|Const\s+|Dim\s+|For\s+)?([A-Za-z_]\w*)[#%$]?\s*(?:=|\+=|-=|\[)/gim)) vars.add(m[1]);
  const taken = new Set([...manifest.keywords, ...manifest.commands.map((c) => c.name.toLowerCase())]);
  return {
    funcs,
    types,
    vars: [...vars].filter((v) => !taken.has(v.toLowerCase()) && !funcs.has(v)),
  };
}

export function registerLanguage() {
  monaco.languages.register({ id: LANG, extensions: [".cb"], aliases: ["CyberBasic"] });
  monaco.languages.setLanguageConfiguration(LANG, {
    comments: { lineComment: "'" },
    brackets: [["(", ")"], ["[", "]"]],
    autoClosingPairs: [
      { open: "(", close: ")" },
      { open: "[", close: "]" },
      { open: '"', close: '"', notIn: ["string", "comment"] },
    ],
    indentationRules: {
      increaseIndentPattern: /^\s*(if\b(?!.*\bthen\s+\S)|else\b|elseif\b|while\b|for\b|repeat\b|select\b|case\b|default\b|function\b|type\b)/i,
      decreaseIndentPattern: /^\s*(endif\b|end\s+(if|function|select|type)\b|else\b|elseif\b|wend\b|next\b|until\b|forever\b|case\b|default\b)/i,
    },
  });

  monaco.languages.setMonarchTokensProvider(LANG, {
    ignoreCase: true,
    keywords: manifest.keywords,
    commands: manifest.commands.map((c) => c.name.toLowerCase()),
    tokenizer: {
      root: [
        [/'.*$/, "comment"],
        [/"[^"]*"?/, "string"],
        [/\b(KEY_[A-Z0-9]+|LIGHT_[A-Z]+)\b/, "constant"],
        [/\d*\.\d+|\d+/, "number"],
        [/[a-zA-Z_]\w*[#%$]?/, { cases: { "@keywords": "keyword", "@commands": "predefined", "@default": "identifier" } }],
        [/<>|<=|>=|\+=|-=|[=<>+\-*/^]/, "operator"],
        [/[()[\],.:]/, "delimiter"],
      ],
    },
  } as monaco.languages.IMonarchLanguage);

  const K = monaco.languages.CompletionItemKind;
  const SNIPPET = monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet;
  monaco.languages.registerCompletionItemProvider(LANG, {
    provideCompletionItems(model, pos) {
      const word = model.getWordUntilPosition(pos);
      const range = new monaco.Range(pos.lineNumber, word.startColumn, pos.lineNumber, word.endColumn);
      const user = userSymbols(allText()); // names from every file
      const items: monaco.languages.CompletionItem[] = [
        ...manifest.commands.map((c) => ({
          label: { label: c.name, description: c.category },
          kind: c.returns ? K.Function : K.Method,
          detail: signature(c),
          documentation: { value: c.doc },
          insertText: callSnippet(c),
          insertTextRules: SNIPPET,
          range,
          sortText: "2" + c.name,
        })),
        ...BLOCKS.map(([label, body, doc]) => ({
          label: { label, description: "block" },
          kind: K.Snippet,
          documentation: doc,
          insertText: body,
          insertTextRules: SNIPPET,
          range,
          sortText: "3" + label,
        })),
        ...manifest.keywords.map((k) => ({ label: proper(k), kind: K.Keyword, insertText: proper(k), range, sortText: "4" + k })),
        ...manifest.constants.map((c) => ({ label: c, kind: K.Constant, insertText: c, range, sortText: "5" + c })),
        ...[...user.funcs].map(([name, params]) => ({
          label: { label: name, description: "your function" },
          kind: K.Function,
          detail: `Function ${name}(${params})`,
          insertText: `${name}(${params.split(",").map((p, i) => p.trim() && `\${${i + 1}:${p.trim()}}`).filter(Boolean).join(", ")})`,
          insertTextRules: SNIPPET,
          range,
          sortText: "1" + name,
        })),
        ...[...user.types].map((t) => ({ label: t, kind: K.Class, insertText: t, range, sortText: "1" + t })),
        ...user.vars.map((v) => ({ label: v, kind: K.Variable, insertText: v, range, sortText: "0" + v })),
      ];
      return { suggestions: items };
    },
  });

  monaco.languages.registerHoverProvider(LANG, {
    provideHover(model, pos) {
      const word = model.getWordAtPosition(pos);
      if (!word) return null;
      const range = new monaco.Range(pos.lineNumber, word.startColumn, pos.lineNumber, word.endColumn);
      const c = findCommand(word.word);
      if (c) {
        const tags = [c.category, c.returns && "returns a value", c.wait && "waits without freezing"].filter(Boolean).join(" · ");
        return { range, contents: [{ value: "```cyberbasic\n" + signature(c) + "\n```" }, { value: c.doc }, { value: `*${tags}*` }] };
      }
      // While paused, show the variable's current value (the debugger reports names in lower case).
      const run = get().run;
      const value = run.state === "paused" ? run.vars?.[word.word.toLowerCase()] : undefined;
      if (value !== undefined) {
        return { range, contents: [{ value: "```cyberbasic\n" + word.word + " = " + value + "\n```" }, { value: `*value on line ${run.line}*` }] };
      }
      const params = userSymbols(allText()).funcs.get(word.word);
      if (params !== undefined) return { range, contents: [{ value: "```cyberbasic\nFunction " + word.word + "(" + params + ")\n```" }] };
      return null;
    },
  });

  // --- Navigation and refactoring, built on symbols.ts

  const userWord = (model: monaco.editor.ITextModel, pos: monaco.Position) => {
    const w = model.getWordAtPosition(pos);
    if (!w || findCommand(w.word) || manifest.keywords.includes(w.word.toLowerCase())) return null;
    return w;
  };
  const toRange = (o: Occurrence) => new monaco.Range(o.line, o.col, o.line, o.col + o.length);
  /** Uses of a name: in every file if it's global anywhere in the project, else just its scope here. */
  const projectOccurrences = (model: monaco.editor.ITextModel, name: string, line: number): [monaco.editor.ITextModel, Occurrence][] =>
    globalInProject(name)
      ? projectModels().flatMap((m) => occurrences(m.getValue(), name, 1, true).map((o) => [m, o] as [monaco.editor.ITextModel, Occurrence]))
      : occurrences(model.getValue(), name, line).map((o) => [model, o]);

  monaco.languages.registerDocumentSymbolProvider(LANG, {
    provideDocumentSymbols(model) {
      const s = scan(model.getValue());
      const SK = monaco.languages.SymbolKind;
      const sym = (name: string, kind: monaco.languages.SymbolKind, line: number, end = line, children: monaco.languages.DocumentSymbol[] = []) => {
        const range = new monaco.Range(line, 1, end, model.getLineMaxColumn(end));
        return { name, detail: "", kind, range, selectionRange: new monaco.Range(line, 1, line, model.getLineMaxColumn(line)), tags: [], children };
      };
      return [
        ...s.decls.map((d) => sym(d.name, d.kind === "const" ? SK.Constant : d.kind === "array" ? SK.Array : SK.Variable, d.line)),
        ...s.types.map((t) => sym(t.name, SK.Struct, t.line, t.endLine, t.fields.map((f) => sym(f.name, SK.Field, f.line)))),
        ...s.funcs.map((f) => sym(`${f.name}(${f.params.join(", ")})`, SK.Function, f.line, f.endLine)),
      ];
    },
  });

  monaco.languages.registerDefinitionProvider(LANG, {
    provideDefinition(model, pos) {
      const w = userWord(model, pos);
      if (!w) return null;
      // A function, type or Global may be declared in another file.
      if (globalInProject(w.word)) {
        for (const m of [model, ...projectModels().filter((x) => x !== model)]) {
          if (!isGlobalName(scan(m.getValue()), w.word)) continue;
          const d = definition(m.getValue(), w.word, 1);
          if (d) return { uri: m.uri, range: new monaco.Range(d.line, d.col, d.line, d.col) };
        }
      }
      const d = definition(model.getValue(), w.word, pos.lineNumber);
      return d ? { uri: model.uri, range: new monaco.Range(d.line, d.col, d.line, d.col) } : null;
    },
  });

  monaco.languages.registerReferenceProvider(LANG, {
    provideReferences(model, pos) {
      const w = userWord(model, pos);
      return w ? projectOccurrences(model, w.word, pos.lineNumber).map(([m, o]) => ({ uri: m.uri, range: toRange(o) })) : [];
    },
  });

  monaco.languages.registerDocumentHighlightProvider(LANG, {
    provideDocumentHighlights(model, pos) {
      const w = userWord(model, pos);
      return w ? occurrences(model.getValue(), w.word, pos.lineNumber, globalInProject(w.word) || undefined).map((o) => ({ range: toRange(o) })) : [];
    },
  });

  monaco.languages.registerRenameProvider(LANG, {
    resolveRenameLocation(model, pos) {
      const w = userWord(model, pos);
      if (!w) return { range: new monaco.Range(1, 1, 1, 1), text: "", rejectReason: "Only your own names (variables, functions, types) can be renamed." };
      return { range: new monaco.Range(pos.lineNumber, w.startColumn, pos.lineNumber, w.endColumn), text: w.word };
    },
    provideRenameEdits(model, pos, newName) {
      const w = userWord(model, pos)!;
      if (!/^[A-Za-z_]\w*$/.test(newName)) return { edits: [], rejectReason: `“${newName}” isn't a valid name.` };
      if (findCommand(newName) || manifest.keywords.includes(newName.toLowerCase())) return { edits: [], rejectReason: `“${newName}” is a built-in name.` };
      return {
        edits: projectOccurrences(model, w.word, pos.lineNumber).map(([m, o]) => ({
          resource: m.uri,
          versionId: m.getVersionId(),
          textEdit: { range: toRange(o), text: newName },
        })),
      };
    },
  });

  monaco.languages.registerDocumentFormattingEditProvider(LANG, {
    provideDocumentFormattingEdits(model) {
      const text = model.getValue();
      const tidy = format(text, " ".repeat(model.getOptions().indentSize));
      return tidy === text ? [] : [{ range: model.getFullModelRange(), text: tidy }];
    },
  });

  monaco.languages.registerSignatureHelpProvider(LANG, {
    signatureHelpTriggerCharacters: ["(", ",", " "],
    signatureHelpRetriggerCharacters: [","],
    provideSignatureHelp(model, pos) {
      const line = model.getLineContent(pos.lineNumber).slice(0, pos.column - 1);
      const call = currentCall(line);
      const c = call && findCommand(call.name);
      if (!c || !c.args) return null;
      // Parameter ranges as offsets into the label, so short names like "x" don't match inside "x1".
      const label = signature(c);
      let at = label.indexOf(c.returns ? "(" : " ") + 1;
      const parameters = parseArgs(c.args).all.map((a) => {
        const start = label.indexOf(a, at);
        at = start + a.length;
        return { label: [start, start + a.length] as [number, number] };
      });
      return {
        value: { signatures: [{ label, documentation: c.doc, parameters }], activeSignature: 0, activeParameter: call.arg },
        dispose() {},
      };
    },
  });

  monaco.editor.defineTheme("cyberbasic", {
    base: "vs-dark",
    inherit: true,
    rules: [
      { token: "comment", foreground: "5f6787", fontStyle: "italic" },
      { token: "keyword", foreground: "b69cff" },
      { token: "predefined", foreground: "4cc2ff" },
      { token: "constant", foreground: "ffb547" },
      { token: "number", foreground: "f7a072" },
      { token: "string", foreground: "9fe0a3" },
      { token: "operator", foreground: "89ddff" },
      { token: "delimiter", foreground: "8b90a3" },
      { token: "identifier", foreground: "dfe2ee" },
    ],
    colors: {
      "editor.background": "#11131b",
      "editor.foreground": "#dfe2ee",
      "editor.lineHighlightBackground": "#171a25",
      "editor.lineHighlightBorder": "#00000000",
      "editorLineNumber.foreground": "#3a4057",
      "editorLineNumber.activeForeground": "#a9afc8",
      "editorGutter.background": "#11131b",
      "editor.selectionBackground": "#2a3558",
      "editor.inactiveSelectionBackground": "#222a44",
      "editorCursor.foreground": "#4cc2ff",
      "editorIndentGuide.background1": "#1c1f2d",
      "editorIndentGuide.activeBackground1": "#2c3147",
      "editorWidget.background": "#161924",
      "editorWidget.border": "#272b3d",
      "editorHoverWidget.background": "#161924",
      "editorHoverWidget.border": "#272b3d",
      "editorSuggestWidget.background": "#161924",
      "editorSuggestWidget.border": "#272b3d",
      "editorSuggestWidget.selectedBackground": "#222a42",
      "editorError.foreground": "#ff6b6b",
      "scrollbarSlider.background": "#ffffff12",
      "scrollbarSlider.hoverBackground": "#ffffff22",
      "minimap.background": "#11131b",
      "focusBorder": "#4cc2ff55",
    },
  });
}

/** The command call the cursor is inside, and which argument it is on. */
function currentCall(line: string): { name: string; arg: number } | null {
  let depth = 0, commas = 0, inString = false;
  for (let i = line.length - 1; i >= 0; i--) {
    const ch = line[i];
    if (ch === '"') inString = !inString;
    if (inString) continue;
    if (ch === ")") depth++;
    else if (ch === "(") {
      if (depth === 0) {
        const m = /([A-Za-z_]\w*)\s*$/.exec(line.slice(0, i));
        if (m && findCommand(m[1])) return { name: m[1], arg: commas };
        commas = 0;
      } else depth--;
    } else if (ch === "," && depth === 0) commas++;
  }
  // Statement form: `Rect x, y, ...` at the start of a statement.
  const m = /(?:^|:|\bThen\b|\bElse\b)\s*([A-Za-z_]\w*)\s+([^:]*)$/i.exec(line);
  if (!m || !findCommand(m[1])) return null;
  return { name: m[1], arg: (m[2].match(/,/g) || []).length };
}

export { monaco };

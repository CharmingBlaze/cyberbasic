// The event sheet editor: "when the game starts" and "every frame" events, each a list of
// conditions (all must hold) and actions (done in order). Changes save into the .events file.
import { useEffect, useMemo, useRef, useState } from "react";
import * as A from "./actions";
import { manifest, findCommand, type Command } from "./compiler";
import { COMPARE_OPS, parseSheet, sheetPrefix, type Action, type Condition, type Sheet, type SheetEvent } from "./events";
import { Icon } from "./icons";
import { monaco } from "./language";
import { get, set, useStore } from "./store";

const argNames = (c: Command | undefined) => (c?.args ?? "").replace(/[[\]]/g, "").split(",").map((a) => a.trim()).filter(Boolean);

export function EventSheetPanel({ file }: { file: string }) {
  const text = useStore((s) => s.project.files[file] ?? "");
  const isMain = useStore((s) => s.project.fileName === file);
  const diagnostics = useStore((s) => s.diagnostics);
  const run = useStore((s) => s.run);
  const focus = useStore((s) => s.sheetFocus);
  const sheet = useMemo(() => parseSheet(text), [text]);
  const [view, setView] = useState<"sheet" | "code">("sheet");
  const [selected, setSelected] = useState<number>();
  const [flash, setFlash] = useState<number>();
  const cards = useRef(new Map<number, HTMLDivElement>());

  /** Applies a change to the latest saved sheet (not a stale render's copy). */
  const update = (fn: (s: Sheet) => Sheet) => A.updateSheet(file, fn(parseSheet(get().project.files[file] ?? "")));
  const updateEvent = (id: number, fn: (e: SheetEvent) => SheetEvent) => update((s) => ({ ...s, events: s.events.map((e) => (e.id === id ? fn(e) : e)) }));

  // Errors and the paused position arrive as generated-line numbers; the line map says which event.
  const map = A.sheetMaps.get(file);
  const errors = diagnostics.filter((d) => d.file === file).map((d) => ({ ...d, at: map?.get(d.line) }));
  const pausedEvent = run.state === "paused" && run.file === file ? map?.get(run.line ?? 0)?.event : undefined;

  useEffect(() => {
    if (!focus || focus.file !== file) return;
    const id = A.sheetMaps.get(file)?.get(focus.line)?.event;
    if (id === undefined) return;
    setView("sheet");
    setSelected(id);
    A.sheetSelection.set(file, id);
    setFlash(id);
    cards.current.get(id)?.scrollIntoView({ block: "center", behavior: "smooth" });
    const t = setTimeout(() => setFlash(undefined), 1200);
    return () => clearTimeout(t);
  }, [focus, file]);

  const select = (id: number) => {
    setSelected(id);
    A.sheetSelection.set(file, id);
  };

  const addEvent = (trigger: "start" | "frame") => {
    const id = Math.max(0, ...sheet.events.map((e) => e.id)) + 1;
    update((s) => ({ ...s, events: [...s.events, { id, trigger, conditions: [], actions: [] }] }));
    select(id);
    setTimeout(() => cards.current.get(id)?.scrollIntoView({ block: "nearest", behavior: "smooth" }));
  };

  const move = (id: number, dir: -1 | 1) =>
    update((s) => {
      const events = [...s.events];
      const i = events.findIndex((e) => e.id === id);
      // Swap with the nearest event of the same trigger in that direction.
      let j = i + dir;
      while (j >= 0 && j < events.length && events[j].trigger !== events[i].trigger) j += dir;
      if (j < 0 || j >= events.length) return s;
      [events[i], events[j]] = [events[j], events[i]];
      return { ...s, events };
    });

  const sections: { trigger: "start" | "frame"; title: string; hint: string }[] = [
    { trigger: "start", title: "When the game starts", hint: "Runs once: open the screen, load images, set things up." },
    { trigger: "frame", title: "Every frame", hint: "Runs about 60 times a second, top to bottom." },
  ];

  return (
    <div className="panel sheet" onMouseDown={() => set({ activeFile: file })}>
      <div className="panel-bar sheet-bar">
        <Icon name="layout" size={15} className="sheet-icon" />
        <strong>{file}</strong>
        <span className="muted small ellipsis">
          {isMain ? "Event sheet · runs the game (Esc quits)" : `Event sheet · call ${sheetPrefix(file)}_Frame each frame from your code`}
        </span>
        <span className="grow" />
        <div className="seg small">
          <button className={view === "sheet" ? "on" : ""} onClick={() => setView("sheet")}>Sheet</button>
          <button className={view === "code" ? "on" : ""} onClick={() => setView("code")} title="The CyberBasic this sheet becomes">Code</button>
        </div>
      </div>

      {view === "code" ? (
        <CodeView file={file} />
      ) : (
        <div className="sheet-body">
          <Variables sheet={sheet} update={update} />
          {sections.map(({ trigger, title, hint }) => {
            const events = sheet.events.filter((e) => e.trigger === trigger);
            return (
              <section key={trigger} className="sheet-section">
                <header>
                  <h3>{title}</h3>
                  <span className="muted small">{hint}</span>
                  <span className="grow" />
                  <button className="tool" onClick={() => addEvent(trigger)}><Icon name="plus" size={13} /> Event</button>
                </header>
                {events.length === 0 && <div className="empty small">No events yet.</div>}
                {events.map((e) => {
                  const number = sheet.events.indexOf(e) + 1;
                  const eventErrors = errors.filter((d) => d.at?.event === e.id);
                  return (
                    <div
                      key={e.id}
                      ref={(el) => void (el ? cards.current.set(e.id, el) : cards.current.delete(e.id))}
                      className={
                        "event" +
                        (selected === e.id ? " selected" : "") +
                        (e.disabled ? " disabled" : "") +
                        (pausedEvent === e.id ? " paused" : "") +
                        (flash === e.id ? " flash" : "") +
                        (eventErrors.length ? " has-error" : "")
                      }
                      onMouseDown={() => select(e.id)}
                    >
                      <div className="event-head">
                        <button
                          className={"event-bp" + (e.breakpoint ? " on" : "")}
                          title={e.breakpoint ? "Remove breakpoint (F9)" : "Pause here when this event runs (F9)"}
                          onClick={() => updateEvent(e.id, (x) => ({ ...x, breakpoint: !x.breakpoint }))}
                        />
                        <span className="event-num">{number}</span>
                        <input
                          className="event-comment"
                          placeholder="Describe this event…"
                          value={e.comment ?? ""}
                          onChange={(ev) => updateEvent(e.id, (x) => ({ ...x, comment: ev.target.value }))}
                        />
                        {pausedEvent === e.id && <span className="event-paused"><Icon name="pause" size={12} /> paused here</span>}
                        <div className="event-tools">
                          <button title="Move up" onClick={() => move(e.id, -1)}><Icon name="chevron" size={13} className="up" /></button>
                          <button title="Move down" onClick={() => move(e.id, 1)}><Icon name="chevron" size={13} className="down" /></button>
                          <button title={e.disabled ? "Enable" : "Disable (skip this event)"} onClick={() => updateEvent(e.id, (x) => ({ ...x, disabled: !x.disabled }))}>
                            <Icon name={e.disabled ? "play" : "pause"} size={12} />
                          </button>
                          <button
                            title="Duplicate"
                            onClick={() => {
                              const id = Math.max(...sheet.events.map((x) => x.id)) + 1;
                              update((s) => {
                                const events = [...s.events];
                                events.splice(events.findIndex((x) => x.id === e.id) + 1, 0, { ...structuredClone(e), id, breakpoint: false });
                                return { ...s, events };
                              });
                            }}
                          >
                            <Icon name="float" size={13} />
                          </button>
                          <button title="Delete event" onClick={() => update((s) => ({ ...s, events: s.events.filter((x) => x.id !== e.id) }))}>
                            <Icon name="trash" size={13} />
                          </button>
                        </div>
                      </div>
                      <div className="event-grid">
                        <div className="conds">
                          <div className="col-label">If</div>
                          {e.conditions.length === 0 && <div className="row-item always">Always</div>}
                          {e.conditions.map((c, i) => (
                            <ConditionRow
                              key={i}
                              c={c}
                              change={(nc) => updateEvent(e.id, (x) => ({ ...x, conditions: x.conditions.map((y, j) => (j === i ? nc : y)) }))}
                              remove={() => updateEvent(e.id, (x) => ({ ...x, conditions: x.conditions.filter((_, j) => j !== i) }))}
                            />
                          ))}
                          {eventErrors.filter((d) => d.at?.action === undefined).map((d, i) => <div key={i} className="row-error">{d.message}</div>)}
                          <Picker kind="condition" add={(c) => updateEvent(e.id, (x) => ({ ...x, conditions: [...x.conditions, c as Condition] }))} />
                        </div>
                        <div className="acts">
                          <div className="col-label">Then</div>
                          {e.actions.length === 0 && <div className="row-item always muted">Nothing yet</div>}
                          {e.actions.map((a, i) => (
                            <div key={i}>
                              <ActionRow
                                a={a}
                                change={(na) => updateEvent(e.id, (x) => ({ ...x, actions: x.actions.map((y, j) => (j === i ? na : y)) }))}
                                remove={() => updateEvent(e.id, (x) => ({ ...x, actions: x.actions.filter((_, j) => j !== i) }))}
                              />
                              {eventErrors.filter((d) => d.at?.action === i).map((d, k) => <div key={k} className="row-error">{d.message}</div>)}
                            </div>
                          ))}
                          <Picker kind="action" add={(a) => updateEvent(e.id, (x) => ({ ...x, actions: [...x.actions, a as Action] }))} />
                        </div>
                      </div>
                    </div>
                  );
                })}
              </section>
            );
          })}
        </div>
      )}
    </div>
  );
}

// ------------------------------------------------------------------ rows

/** An input that grows with its text; shows the argument name until filled. */
function Field({ value, placeholder, onChange, wide }: { value: string; placeholder: string; onChange: (v: string) => void; wide?: boolean }) {
  return (
    <input
      className={"field mono" + (wide ? " wide" : "")}
      value={value}
      placeholder={placeholder}
      size={Math.max(value.length, placeholder.length, 2)}
      spellCheck={false}
      onChange={(e) => onChange(e.target.value)}
    />
  );
}

function CallFields({ name, args, change }: { name: string; args: string[]; change: (args: string[]) => void }) {
  const cmd = findCommand(name);
  const names = argNames(cmd);
  return (
    <>
      <span className="call-name" title={cmd?.doc}>{name}</span>
      {names.map((n, i) => (
        <Field key={i} value={args[i] ?? ""} placeholder={n} onChange={(v) => change(names.map((_, j) => (j === i ? v : args[j] ?? "")))} />
      ))}
    </>
  );
}

function ConditionRow({ c, change, remove }: { c: Condition; change: (c: Condition) => void; remove: () => void }) {
  return (
    <div className="row-item">
      <button className={"not" + (c.not ? " on" : "")} title="Invert: true when this is NOT the case" onClick={() => change({ ...c, not: !c.not })}>
        not
      </button>
      {c.kind === "call" && <CallFields name={c.name} args={c.args} change={(args) => change({ ...c, args })} />}
      {c.kind === "compare" && (
        <>
          <Field value={c.left} placeholder="value" onChange={(left) => change({ ...c, left })} />
          <select value={c.op} onChange={(e) => change({ ...c, op: e.target.value })}>
            {COMPARE_OPS.map((o) => <option key={o}>{o}</option>)}
          </select>
          <Field value={c.right} placeholder="value" onChange={(right) => change({ ...c, right })} />
        </>
      )}
      {c.kind === "every" && (
        <>
          <span className="call-name">Every</span>
          <Field value={c.seconds} placeholder="1" onChange={(seconds) => change({ ...c, seconds })} />
          <span className="muted">seconds</span>
        </>
      )}
      {c.kind === "expr" && <Field wide value={c.text} placeholder="condition, e.g. x > 100 And lives > 0" onChange={(text) => change({ ...c, text })} />}
      <span className="grow" />
      <button className="row-del" title="Remove" onClick={remove}><Icon name="close" size={11} /></button>
    </div>
  );
}

function ActionRow({ a, change, remove }: { a: Action; change: (a: Action) => void; remove: () => void }) {
  return (
    <div className="row-item">
      <Icon name="chevron" size={11} className="act-mark" />
      {a.kind === "call" && <CallFields name={a.name} args={a.args} change={(args) => change({ ...a, args })} />}
      {a.kind === "set" && (
        <>
          <span className="call-name">Set</span>
          <Field value={a.variable} placeholder="variable" onChange={(variable) => change({ ...a, variable })} />
          <select value={a.op} onChange={(e) => change({ ...a, op: e.target.value as "=" | "+=" | "-=" })}>
            {["=", "+=", "-="].map((o) => <option key={o}>{o}</option>)}
          </select>
          <Field value={a.value} placeholder="value" onChange={(value) => change({ ...a, value })} />
        </>
      )}
      {a.kind === "code" && <Field wide value={a.text} placeholder="a line of CyberBasic, e.g. SpawnEnemy 3" onChange={(text) => change({ ...a, text })} />}
      <span className="grow" />
      <button className="row-del" title="Remove" onClick={remove}><Icon name="close" size={11} /></button>
    </div>
  );
}

// ------------------------------------------------------------------ picker

interface Choice { label: string; group: string; doc: string; make: () => Condition | Action }

function choices(kind: "condition" | "action"): Choice[] {
  const call = (c: Command) => ({ kind: "call" as const, name: c.name, args: argNames(c).map(() => "") });
  const special: Choice[] =
    kind === "condition"
      ? [
          { label: "Compare two values", group: "Common", doc: "True when a value is =, <>, <, <=, > or >= another, e.g. score >= 10.", make: () => ({ kind: "compare", left: "", op: "=", right: "" }) },
          { label: "Every N seconds", group: "Common", doc: "True once every so many seconds.", make: () => ({ kind: "every", seconds: "1" }) },
          { label: "Custom condition", group: "Common", doc: "Any CyberBasic expression, e.g. x > 100 And lives > 0.", make: () => ({ kind: "expr", text: "" }) },
        ]
      : [
          { label: "Set a variable", group: "Common", doc: "Change a variable: =, += or -= a value.", make: () => ({ kind: "set", variable: "", op: "=", value: "" }) },
          { label: "Run a line of code", group: "Common", doc: "Any CyberBasic statement, e.g. calling one of your own functions.", make: () => ({ kind: "code", text: "" }) },
        ];
  const commands = manifest.commands
    .filter((c) => (kind === "condition" ? c.returns && !c.wait : !c.returns))
    .map((c) => ({ label: c.name, group: c.category, doc: c.doc, make: () => call(c) }));
  return [...special, ...commands];
}

function Picker({ kind, add }: { kind: "condition" | "action"; add: (x: Condition | Action) => void }) {
  const [open, setOpen] = useState(false);
  const [q, setQ] = useState("");
  const [i, setI] = useState(0);
  const box = useRef<HTMLDivElement>(null);
  const all = useMemo(() => choices(kind), [kind]);
  const words = q.toLowerCase().split(/\s+/).filter(Boolean);
  const shown = all.filter((c) => words.every((w) => `${c.label} ${c.group} ${c.doc}`.toLowerCase().includes(w)));
  useEffect(() => {
    if (!open) return;
    const close = (e: MouseEvent) => !box.current?.contains(e.target as Node) && setOpen(false);
    window.addEventListener("mousedown", close);
    return () => window.removeEventListener("mousedown", close);
  }, [open]);
  const pick = (c?: Choice) => {
    if (!c) return;
    add(c.make());
    setOpen(false);
    setQ("");
    setI(0);
  };
  let lastGroup = "";
  return (
    <div className="picker" ref={box}>
      <button className="add-row" onClick={() => setOpen(!open)}>
        <Icon name="plus" size={12} /> Add {kind}
      </button>
      {open && (
        <div className="picker-pop" onMouseDown={(e) => e.stopPropagation()}>
          <div className="search">
            <Icon name="search" size={13} />
            <input
              autoFocus
              placeholder={kind === "condition" ? "Key down, compare, overlap…" : "Draw, move, set, play…"}
              value={q}
              onChange={(e) => {
                setQ(e.target.value);
                setI(0);
              }}
              onKeyDown={(e) => {
                if (e.key === "Escape") setOpen(false);
                if (e.key === "ArrowDown") setI(Math.min(i + 1, shown.length - 1));
                if (e.key === "ArrowUp") setI(Math.max(i - 1, 0));
                if (e.key === "Enter") pick(shown[i]);
                if (["ArrowDown", "ArrowUp", "Enter"].includes(e.key)) e.preventDefault();
              }}
            />
          </div>
          <div className="picker-list">
            {shown.length === 0 && <div className="empty small">Nothing matches “{q}”.</div>}
            {shown.map((c, n) => {
              const head = c.group !== lastGroup ? <div className="picker-group">{(lastGroup = c.group)}</div> : null;
              return (
                <div key={c.group + c.label}>
                  {head}
                  <button className={"picker-item" + (n === i ? " on" : "")} onMouseEnter={() => setI(n)} onClick={() => pick(c)}>
                    <span className="mono">{c.label}</span>
                    <span className="muted small">{c.doc}</span>
                  </button>
                </div>
              );
            })}
          </div>
        </div>
      )}
    </div>
  );
}

// ------------------------------------------------------------------ variables & code view

function Variables({ sheet, update }: { sheet: Sheet; update: (fn: (s: Sheet) => Sheet) => void }) {
  const setVar = (i: number, v: { name: string; value: string }) => update((s) => ({ ...s, variables: s.variables.map((x, j) => (j === i ? v : x)) }));
  return (
    <section className="sheet-vars">
      <span className="col-label">Variables</span>
      {sheet.variables.map((v, i) => (
        <span key={i} className="var-chip">
          <Field value={v.name} placeholder="name" onChange={(name) => setVar(i, { ...v, name })} />
          <span className="muted">=</span>
          <Field value={v.value} placeholder="0" onChange={(value) => setVar(i, { ...v, value })} />
          <button className="row-del" title="Remove" onClick={() => update((s) => ({ ...s, variables: s.variables.filter((_, j) => j !== i) }))}>
            <Icon name="close" size={11} />
          </button>
        </span>
      ))}
      <button className="add-row" onClick={() => update((s) => ({ ...s, variables: [...s.variables, { name: "", value: "0" }] }))}>
        <Icon name="plus" size={12} /> Variable
      </button>
    </section>
  );
}

function CodeView({ file }: { file: string }) {
  const text = useStore((s) => s.project.files[file]);
  const [html, setHtml] = useState("");
  useEffect(() => {
    monaco.editor.colorize(A.generatedCode(file), "cyberbasic", { tabSize: 4 }).then(setHtml);
  }, [file, text]);
  return (
    <div className="sheet-code">
      <div className="muted small sheet-code-note">Read only: this is the CyberBasic the sheet becomes. Edit the sheet to change it.</div>
      <pre className="mono" dangerouslySetInnerHTML={{ __html: html }} />
    </div>
  );
}

// App state: one plain object, replaced on every change; components subscribe to slices.
import { useSyncExternalStore } from "react";

export type RunState = "stopped" | "starting" | "running" | "paused" | "error";
export type WorkspaceId = "game" | "code" | "debug" | "learn";

export interface Project {
  name: string;
  source: string;
  /** Asset path as used in code ("assets/ship.svg") -> URL the browser can load. */
  assets: Record<string, string>;
  /** Linked folder on disk, when opened or saved with the File System Access API. */
  dir?: FileSystemDirectoryHandle;
  fileName: string;
}
export interface Diagnostic { line: number; col: number; message: string; kind: "compile" | "runtime" }
export interface LogEntry { id: number; kind: "log" | "error" | "info"; text: string; line?: number; time: string }
export interface Toast { id: number; text: string; kind: "info" | "error" | "success" }

export interface State {
  ready: boolean;
  project: Project;
  dirty: boolean;
  diagnostics: Diagnostic[];
  run: {
    state: RunState;
    session: number; // bumps on every run; the game panel reloads when it changes
    page: string; // the HTML page currently running
    line?: number;
    fn?: string;
    vars?: Record<string, string>;
    fps?: number;
  };
  logs: LogEntry[];
  breakpoints: number[];
  workspace: WorkspaceId;
  cursor: { line: number; col: number };
  toasts: Toast[];
  palette: boolean;
  referenceFilter: string;
}

let state: State = {
  ready: false,
  project: { name: "Untitled", source: "", assets: {}, fileName: "main.cb" },
  dirty: false,
  diagnostics: [],
  run: { state: "stopped", session: 0, page: "" },
  logs: [],
  breakpoints: [],
  workspace: "game",
  cursor: { line: 1, col: 1 },
  toasts: [],
  palette: false,
  referenceFilter: "",
};

const listeners = new Set<() => void>();
export const get = () => state;
export function set(patch: Partial<State> | ((s: State) => Partial<State>)) {
  state = { ...state, ...(typeof patch === "function" ? patch(state) : patch) };
  listeners.forEach((l) => l());
}
const subscribe = (l: () => void) => {
  listeners.add(l);
  return () => listeners.delete(l);
};
/** Re-renders only when the selected value changes (selectors must return existing objects or primitives). */
export const useStore = <T,>(select: (s: State) => T): T => useSyncExternalStore(subscribe, () => select(state));

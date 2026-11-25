// App state: one plain object, replaced on every change; components subscribe to slices.
import { useSyncExternalStore } from "react";

export type RunState = "stopped" | "starting" | "running" | "paused" | "error";
export type WorkspaceId = "game" | "code" | "debug" | "learn";

export interface Project {
  name: string;
  /** Source files by name ("main.cb", "enemies.cb"). `fileName` is the one that runs. */
  files: Record<string, string>;
  /** Asset path as used in code ("assets/ship.svg") -> URL the browser can load. */
  assets: Record<string, string>;
  /** Linked folder on disk, when opened or saved with the File System Access API. */
  dir?: FileSystemDirectoryHandle;
  fileName: string;
}
export interface Diagnostic { file: string; line: number; col: number; message: string; kind: "compile" | "runtime"; node?: string }
export interface LogEntry { id: number; kind: "log" | "error" | "info"; text: string; file?: string; line?: number; time: string }
export interface Breakpoint { file: string; line: number }
export interface Toast { id: number; text: string; kind: "info" | "error" | "success" }

export interface Settings {
  fontSize: number;
  minimap: boolean;
  wordWrap: boolean;
  formatOnSave: boolean;
}
const SETTINGS_KEY = "cyberbasic.settings";
const DEFAULT_SETTINGS: Settings = { fontSize: 14, minimap: true, wordWrap: false, formatOnSave: false };
const loadSettings = (): Settings => {
  try {
    return { ...DEFAULT_SETTINGS, ...JSON.parse(localStorage.getItem(SETTINGS_KEY) || "{}") };
  } catch {
    return DEFAULT_SETTINGS;
  }
};
export function setSettings(patch: Partial<Settings>) {
  set((s) => ({ settings: { ...s.settings, ...patch } }));
  localStorage.setItem(SETTINGS_KEY, JSON.stringify(state.settings));
}

export interface State {
  ready: boolean;
  project: Project;
  dirty: boolean;
  diagnostics: Diagnostic[];
  run: {
    state: RunState;
    session: number; // bumps on every run; the game panel reloads when it changes
    page: string; // the HTML page currently running
    files: string[]; // file order of the running build: locations are file index * 1e6 + line
    file?: string;
    line?: number;
    fn?: string;
    vars?: Record<string, string>;
    fps?: number;
  };
  logs: LogEntry[];
  breakpoints: Breakpoint[];
  /** The file in the editor that last had focus. */
  activeFile: string;
  /** Asks a graph to scroll to and flash the node behind a generated line. */
  graphFocus?: { file: string; line: number; n: number };
  workspace: WorkspaceId;
  cursor: { line: number; col: number };
  toasts: Toast[];
  palette: boolean;
  referenceFilter: string;
  settings: Settings;
}

let state: State = {
  ready: false,
  project: { name: "Untitled", files: { "main.cb": "" }, assets: {}, fileName: "main.cb" },
  dirty: false,
  diagnostics: [],
  run: { state: "stopped", session: 0, page: "", files: [] },
  logs: [],
  breakpoints: [],
  activeFile: "main.cb",
  workspace: "game",
  cursor: { line: 1, col: 1 },
  toasts: [],
  palette: false,
  referenceFilter: "",
  settings: typeof localStorage === "undefined" ? DEFAULT_SETTINGS : loadSettings(),
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

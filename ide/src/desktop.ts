// The Tauri desktop shell (ide/src-tauri). The window is frameless, so our title bar moves it and
// draws its buttons. In a browser none of this runs.
import { getCurrentWindow } from "@tauri-apps/api/window";
import { get } from "./store";

export const isDesktop = typeof window !== "undefined" && "__TAURI_INTERNALS__" in window;
export const appWindow = () => getCurrentWindow();

/** Asks before the window closes (button, Alt+F4, taskbar) if a folder project has unsaved changes. */
export function guardClose() {
  if (!isDesktop) return;
  appWindow().onCloseRequested((e) => {
    const { dirty, project } = get();
    if (dirty && project.dir && !confirm(`${project.name} has unsaved changes. Close anyway?`)) e.preventDefault();
  });
}

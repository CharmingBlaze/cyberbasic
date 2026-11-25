// A small stroke icon set (24px grid, 1.75 stroke), drawn to match the UI.
const P: Record<string, string> = {
  play: "M7 5v14l11-7z",
  pause: "M8 5v14M16 5v14",
  stop: "M7 7h10v10H7z",
  step: "M5 12h10M11 6l6 6-6 6M19 5v14",
  restart: "M4 12a8 8 0 1 0 2.4-5.7M4 4v5h5",
  bug: "M9 9h6a3 3 0 0 1 3 3v2a6 6 0 0 1-12 0v-2a3 3 0 0 1 3-3zM12 9v11M9 5l1.5 2M15 5l-1.5 2M3 13h3M18 13h3M4 19l2.5-1.5M20 19l-2.5-1.5",
  file: "M14 3H7a2 2 0 0 0-2 2v14a2 2 0 0 0 2 2h10a2 2 0 0 0 2-2V8zM14 3v5h5",
  code: "M9 8l-4 4 4 4M15 8l4 4-4 4",
  folder: "M3 7a2 2 0 0 1 2-2h4l2 2h8a2 2 0 0 1 2 2v8a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2z",
  image: "M4 5h16v14H4zM4 16l5-5 4 4 3-3 4 4M15 9h.01",
  cube: "M12 3l8 4.5v9L12 21l-8-4.5v-9zM12 12l8-4.5M12 12v9M12 12L4 7.5",
  save: "M5 4h11l3 3v13H5zM8 4v5h7M8 20v-6h8v6",
  download: "M12 4v11M7 10l5 5 5-5M5 20h14",
  plus: "M12 5v14M5 12h14",
  search: "M11 18a7 7 0 1 1 0-14 7 7 0 0 1 0 14zM16 16l4 4",
  terminal: "M5 7l5 5-5 5M12 18h7",
  alert: "M12 8v5M12 16.5h.01M10.3 4.2L2.8 17.5A2 2 0 0 0 4.5 20.5h15a2 2 0 0 0 1.7-3L13.7 4.2a2 2 0 0 0-3.4 0z",
  book: "M5 4h9a4 4 0 0 1 4 4v12a3 3 0 0 0-3-3H5zM18 8v12",
  float: "M4 8h11v11H4zM9 4h11v11",
  popout: "M14 4h6v6M20 4l-8 8M18 14v5a1 1 0 0 1-1 1H5a1 1 0 0 1-1-1V7a1 1 0 0 1 1-1h5",
  dock: "M4 5h16v14H4zM4 14h16",
  maximize: "M4 9V4h5M20 9V4h-5M4 15v5h5M20 15v5h-5",
  minimize: "M9 4v5H4M15 4v5h5M9 20v-5H4M15 20v-5h5",
  close: "M6 6l12 12M18 6L6 18",
  check: "M5 12l5 5 9-10",
  x: "M15 9l-6 6M9 9l6 6",
  dot: "M12 12h.01",
  layout: "M4 5h16v14H4zM10 5v14M10 12h10",
  gamepad: "M7 9h4M9 7v4M15 10h.01M18 9h.01M6 5h12a4 4 0 0 1 4 4v4a4 4 0 0 1-7 2.6L14 14h-4l-1 1.6A4 4 0 0 1 2 13V9a4 4 0 0 1 4-4z",
  sparkle: "M12 3l1.8 5.2L19 10l-5.2 1.8L12 17l-1.8-5.2L5 10l5.2-1.8z",
  trash: "M5 7h14M10 7V5h4v2M7 7l1 13h8l1-13",
  chevron: "M9 6l6 6-6 6",
  external: "M14 4h6v6M20 4l-9 9M19 14v6H4V5h6",
  keyboard: "M3 6h18v12H3zM7 10h.01M11 10h.01M15 10h.01M7 14h10",
};

export function Icon({ name, size = 16, className }: { name: keyof typeof P; size?: number; className?: string }) {
  const filled = name === "play" || name === "stop";
  return (
    <svg
      className={"icon " + (className ?? "")}
      width={size}
      height={size}
      viewBox="0 0 24 24"
      fill={filled ? "currentColor" : "none"}
      stroke="currentColor"
      strokeWidth={filled ? 1.2 : 1.75}
      strokeLinecap="round"
      strokeLinejoin="round"
      aria-hidden="true"
    >
      <path d={P[name]} />
    </svg>
  );
}
export type IconName = keyof typeof P;

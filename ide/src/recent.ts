// Recently opened folders. Directory handles can only be stored in IndexedDB (not localStorage).
export interface Recent { name: string; handle: FileSystemDirectoryHandle; time: number }

const db = () =>
  new Promise<IDBDatabase>((ok, fail) => {
    const req = indexedDB.open("cyberbasic", 1);
    req.onupgradeneeded = () => req.result.createObjectStore("recent", { keyPath: "name" });
    req.onsuccess = () => ok(req.result);
    req.onerror = () => fail(req.error);
  });

const tx = async <T,>(mode: IDBTransactionMode, fn: (s: IDBObjectStore) => IDBRequest<T>) => {
  const store = (await db()).transaction("recent", mode).objectStore("recent");
  return new Promise<T>((ok, fail) => {
    const req = fn(store);
    req.onsuccess = () => ok(req.result);
    req.onerror = () => fail(req.error);
  });
};

export async function listRecent(): Promise<Recent[]> {
  try {
    const all = await tx<Recent[]>("readonly", (s) => s.getAll() as IDBRequest<Recent[]>);
    return all.sort((a, b) => b.time - a.time).slice(0, 8);
  } catch {
    return []; // IndexedDB unavailable (private mode): just no recent list
  }
}

export async function addRecent(handle: FileSystemDirectoryHandle) {
  try {
    await tx("readwrite", (s) => s.put({ name: handle.name, handle, time: Date.now() }));
  } catch {
    // not fatal
  }
}

export const removeRecent = (name: string) => tx("readwrite", (s) => s.delete(name)).catch(() => undefined);

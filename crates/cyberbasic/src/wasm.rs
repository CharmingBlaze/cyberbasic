//! Plain C-ABI exports so the browser playground can call the compiler without wasm-bindgen.
//! JS: cb_alloc(n) -> copy UTF-8 source in -> cb_compile(ptr, n, debug) returns 1 (ok) or 0 (error);
//! then read cb_out_len() bytes at cb_out_ptr(): the JS, or "line:col:message".

use std::cell::RefCell;

thread_local! {
    static OUT: RefCell<Vec<u8>> = const { RefCell::new(Vec::new()) };
}

#[no_mangle]
pub extern "C" fn cb_alloc(len: usize) -> *mut u8 {
    let mut v = Vec::<u8>::with_capacity(len);
    let p = v.as_mut_ptr();
    std::mem::forget(v);
    p
}

/// # Safety
/// `p` and `len` must come from a matching `cb_alloc` call.
#[no_mangle]
pub unsafe extern "C" fn cb_free(p: *mut u8, len: usize) {
    drop(Vec::from_raw_parts(p, 0, len));
}

/// # Safety
/// `p` must point to `len` initialized bytes (from `cb_alloc`).
#[no_mangle]
pub unsafe extern "C" fn cb_compile(p: *const u8, len: usize, debug: u32) -> u32 {
    let src = String::from_utf8_lossy(std::slice::from_raw_parts(p, len));
    let (ok, text) = match crate::compile_with(&src, debug != 0) {
        Ok(js) => (1, js),
        Err(e) => (0, format!("{}:{}:{}", e.line, e.col, e.msg)),
    };
    OUT.with(|o| *o.borrow_mut() = text.into_bytes());
    ok
}

/// Puts the command manifest (JSON) in the output buffer.
#[no_mangle]
pub extern "C" fn cb_manifest() {
    OUT.with(|o| *o.borrow_mut() = crate::builtins::manifest_json().into_bytes());
}

#[no_mangle]
pub extern "C" fn cb_out_ptr() -> *const u8 {
    OUT.with(|o| o.borrow().as_ptr())
}

#[no_mangle]
pub extern "C" fn cb_out_len() -> usize {
    OUT.with(|o| o.borrow().len())
}

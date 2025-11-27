# Optional MSVC prevention - can be disabled with -DALLOW_MSVC=ON
option(ALLOW_MSVC "Allow building with MSVC (may have compatibility issues)" OFF)

if(MSVC AND NOT ALLOW_MSVC)
  message(FATAL_ERROR "MSVC is not officially supported due to compatibility issues. "
                      "Use MinGW-w64 GCC or Clang for best results. "
                      "To build with MSVC anyway (experimental), use -DALLOW_MSVC=ON")
endif()

# Simple script to fail if FILE_PATH does not exist
if(NOT DEFINED FILE_PATH)
  message(FATAL_ERROR "FILE_PATH not provided to check_file_exists.cmake")
endif()

if(NOT EXISTS "${FILE_PATH}")
  message(FATAL_ERROR "Expected file not found: ${FILE_PATH}")
else()
  message(STATUS "Found file: ${FILE_PATH}")
endif()

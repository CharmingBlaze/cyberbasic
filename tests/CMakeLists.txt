# Basic CTest suite: compile example BASIC files and verify generated C++ outputs

cmake_minimum_required(VERSION 3.16)

if(NOT BUILD_TESTING)
  return()
endif()

# Directory for generated files inside build tree
set(GENERATED_DIR "${CMAKE_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${GENERATED_DIR}")

# Paths
set(HELLO_BAS "${CMAKE_SOURCE_DIR}/examples/hello.bas")
set(PONG_BAS  "${CMAKE_SOURCE_DIR}/examples/pong_demo.bas")
set(HELLO_CPP "${GENERATED_DIR}/hello.cpp")
set(PONG_CPP  "${GENERATED_DIR}/pong_demo.cpp")

# Test: compile hello.bas
add_test(
  NAME compile_hello_bas
  COMMAND $<TARGET_FILE:cyberbasic> "${HELLO_BAS}" "${HELLO_CPP}"
)
set_tests_properties(compile_hello_bas PROPERTIES WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Test: verify hello.cpp exists
add_test(
  NAME check_hello_cpp_exists
  COMMAND ${CMAKE_COMMAND} -DFILE_PATH="${HELLO_CPP}" -P "${CMAKE_SOURCE_DIR}/tests/check_file_exists.cmake"
)
set_tests_properties(check_hello_cpp_exists PROPERTIES DEPENDS compile_hello_bas)

# Test: compile pong_demo.bas (placeholder example)
add_test(
  NAME compile_pong_demo_bas
  COMMAND $<TARGET_FILE:cyberbasic> "${PONG_BAS}" "${PONG_CPP}"
)
set_tests_properties(compile_pong_demo_bas PROPERTIES WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Test: verify pong_demo.cpp exists
add_test(
  NAME check_pong_demo_cpp_exists
  COMMAND ${CMAKE_COMMAND} -DFILE_PATH="${PONG_CPP}" -P "${CMAKE_SOURCE_DIR}/tests/check_file_exists.cmake"
)
set_tests_properties(check_pong_demo_cpp_exists PROPERTIES DEPENDS compile_pong_demo_bas)

# Content checks: ensure generated files include the unified header and use isReady()
add_test(
  NAME check_hello_cpp_contains_header
  COMMAND ${CMAKE_COMMAND} -DFILE_PATH="${HELLO_CPP}" -DREQUIRED_SUBSTRING="#include \"cyberbasic_modules.h\"" -P "${CMAKE_SOURCE_DIR}/tests/check_file_contains.cmake"
)
set_tests_properties(check_hello_cpp_contains_header PROPERTIES DEPENDS check_hello_cpp_exists)

add_test(
  NAME check_hello_cpp_contains_isReady
  COMMAND ${CMAKE_COMMAND} -DFILE_PATH="${HELLO_CPP}" -DREQUIRED_SUBSTRING="while (CyberBasic::isReady())" -P "${CMAKE_SOURCE_DIR}/tests/check_file_contains.cmake"
)
set_tests_properties(check_hello_cpp_contains_isReady PROPERTIES DEPENDS check_hello_cpp_exists)

add_test(
  NAME check_pong_cpp_contains_header
  COMMAND ${CMAKE_COMMAND} -DFILE_PATH="${PONG_CPP}" -DREQUIRED_SUBSTRING="#include \"cyberbasic_modules.h\"" -P "${CMAKE_SOURCE_DIR}/tests/check_file_contains.cmake"
)
set_tests_properties(check_pong_cpp_contains_header PROPERTIES DEPENDS check_pong_demo_cpp_exists)

add_test(
  NAME check_pong_cpp_contains_isReady
  COMMAND ${CMAKE_COMMAND} -DFILE_PATH="${PONG_CPP}" -DREQUIRED_SUBSTRING="while (CyberBasic::isReady())" -P "${CMAKE_SOURCE_DIR}/tests/check_file_contains.cmake"
)
set_tests_properties(check_pong_cpp_contains_isReady PROPERTIES DEPENDS check_pong_demo_cpp_exists)


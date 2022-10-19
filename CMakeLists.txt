if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif()
project(clang-wrapper VERSION 0.5.0)
cmake_minimum_required(VERSION 3.24)

include(cmake/config.cmake)

aux_source_directory(${CMAKE_SOURCE_DIR}/src/common/ COMMON_SRCS)
list(APPEND COMMON_SRCS src/utility/assertion.cc)

list(APPEND BASE_SRCS ${COMMON_SRCS} src/pass/pass_base.cc src/wrapper/wrapper_base.cc)
add_executable(base   ${BASE_SRCS} src/main/base.cc)
add_executable(base++ ${BASE_SRCS} src/main/base.cc)
target_compile_definitions(base++ PUBLIC USE_CXX)

list(APPEND CALL_HOOK_SRCS ${BASE_SRCS} src/pass/pass_call_hook.cc src/wrapper/wrapper_call_hook.cc)
add_executable(call-hook   ${CALL_HOOK_SRCS} src/main/call_hook.cc)
add_executable(call-hook++ ${CALL_HOOK_SRCS} src/main/call_hook.cc)
target_compile_definitions(call-hook   PUBLIC RUNTIME_HOME="${CMAKE_SOURCE_DIR}/src/runtime/call_hook")
target_compile_definitions(call-hook++ PUBLIC RUNTIME_HOME="${CMAKE_SOURCE_DIR}/src/runtime/call_hook")
target_compile_definitions(call-hook++ PUBLIC USE_CXX)

list(APPEND STRUCT_PARSER_SRCS ${BASE_SRCS} src/pass/pass_struct_parser.cc src/wrapper/wrapper_struct_parser.cc src/utility/struct_parser.cc)
add_executable(struct-parser   ${STRUCT_PARSER_SRCS} src/main/struct_parser.cc)
add_executable(struct-parser++ ${STRUCT_PARSER_SRCS} src/main/struct_parser.cc)
target_compile_definitions(struct-parser++ PUBLIC USE_CXX)

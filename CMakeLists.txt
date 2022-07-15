cmake_minimum_required(VERSION 3.22)
project(code)
set(CMAKE_CXX_STANDARD 20)


set(CUDD_DIR ${CMAKE_SOURCE_DIR}/libs/cudd-3.0.0)
set(SPDLOG_DIR ${CMAKE_SOURCE_DIR}/libs/spdlog)

include_directories(${LOGURU_DIR})
add_subdirectory(${SPDLOG_DIR})

add_subdirectory(src)
add_subdirectory(tests)

add_library(libcudd STATIC IMPORTED)
set_target_properties(libcudd PROPERTIES IMPORTED_LOCATION ${CUDD_DIR}/cudd/.libs/libcudd.a)
set_target_properties(libcudd PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CUDD_DIR}/cudd)
target_link_libraries(main libcudd)
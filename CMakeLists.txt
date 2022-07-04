cmake_minimum_required(VERSION 3.22)
project(code)

set(CUDD_DIR ${CMAKE_SOURCE_DIR}/libs/cudd-3.0.0)

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)

add_library(libcudd STATIC IMPORTED)
set_target_properties(libcudd PROPERTIES IMPORTED_LOCATION ${CUDD_DIR}/cudd/.libs/libcudd.a)
set_target_properties(libcudd PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CUDD_DIR}/cudd)
target_link_libraries(main libcudd)
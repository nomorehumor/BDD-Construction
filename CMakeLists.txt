cmake_minimum_required(VERSION 3.22)
project(code)

set(CMAKE_CXX_STANDARD 11)
set(CUDD_DIR ${CMAKE_SOURCE_DIR}/cudd-3.0.0)
include_directories(${CUDD_DIR})
add_executable(main main.cpp file_utils.cpp file_utils.h tutorial.cpp tutorial.h output_utils.cpp output_utils.h ProgressBar.cpp ProgressBar.h)

add_library(libcudd STATIC IMPORTED)
set_target_properties(libcudd PROPERTIES IMPORTED_LOCATION ${CUDD_DIR}/cudd/.libs/libcudd.a)
set_target_properties(libcudd PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CUDD_DIR}/cudd)
target_link_libraries(main libcudd)

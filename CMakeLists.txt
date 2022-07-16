cmake_minimum_required(VERSION 3.22)
project(code)
set(CMAKE_CXX_STANDARD 20)


set(CUDD_DIR ${CMAKE_SOURCE_DIR}/libs/cudd-3.0.0)
set(SPDLOG_DIR ${CMAKE_SOURCE_DIR}/libs/spdlog)
set(YAML_DIR ${CMAKE_SOURCE_DIR}/libs/yaml-cpp)
set(PLT_DIR ${CMAKE_SOURCE_DIR}/libs/matplotlib-cpp)

set(PYTHON_EXECUTABLE /usr/bin/python3.9)
set(PYTHON_LIBRARIES /usr/lib/python3.9)
set(PYTHON_INCLUDE_DIRS /usr/include/python3.9)
set(Python3_NumPy_INCLUDE_DIR /usr/local/lib/python3.9/site-packages/numpy/core/include/)


find_package(PythonLibs 3.9 REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)

add_subdirectory(${YAML_DIR})
include_directories(${YAML_DIR}/include)
add_subdirectory(${SPDLOG_DIR})
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${PLT_DIR})

add_subdirectory(src)
add_subdirectory(tests)


add_library(libcudd STATIC IMPORTED)
set_target_properties(libcudd PROPERTIES IMPORTED_LOCATION ${CUDD_DIR}/cudd/.libs/libcudd.a)
set_target_properties(libcudd PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CUDD_DIR}/cudd)
target_link_libraries(main libcudd)
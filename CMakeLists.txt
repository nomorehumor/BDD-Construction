cmake_minimum_required(VERSION 3.16)

# set(CXX /usr/bin/g++)
# set(CMAKE_CXX_COMPILER /usr/bin/g++)

project(code)
set(CMAKE_CXX_STANDARD 20)


set(CUDD_DIR ${CMAKE_SOURCE_DIR}/libs/cudd-3.0.0)
set(SDD_DIR ${CMAKE_SOURCE_DIR}/libs/sdd-package-2.0/libsdd-2.0)
set(SPDLOG_DIR ${CMAKE_SOURCE_DIR}/libs/spdlog)
set(YAML_DIR ${CMAKE_SOURCE_DIR}/libs/yaml-cpp)
set(PLT_DIR ${CMAKE_SOURCE_DIR}/libs/matplotlib-cpp)
set(GTEST_DIR ${CMAKE_SOURCE_DIR}/libs/googletest)

#set(PYTHON_EXECUTABLE /usr/bin/python3.9)
#set(PYTHON_LIBRARIES /usr/lib/python3.9)
#set(PYTHON_INCLUDE_DIRS /usr/include/python3.9)
#set(Python3_NumPy_INCLUDE_DIR /usr/lo cal/lib/python3.9/site-packages/numpy/core/include/)

find_package(PythonLibs 3.8 REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)

include_directories(${CUDD_DIR}/cudd)
include_directories(${SDD_DIR})
include_directories(${SDD_DIR}/include)
add_subdirectory(${YAML_DIR})
include_directories(${YAML_DIR}/include)
add_subdirectory(${SPDLOG_DIR})
include_directories(${SPDLOG_DIR}/include)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${PLT_DIR})
add_subdirectory(${GTEST_DIR})

add_subdirectory(src)
add_subdirectory(tests)

add_library(libsdd STATIC IMPORTED)
set_target_properties(libsdd PROPERTIES IMPORTED_LOCATION ${SDD_DIR}/build/libsdd.a)
set_target_properties(libsdd PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SDD_DIR}/include)

add_library(libcudd STATIC IMPORTED)
set_target_properties(libcudd PROPERTIES IMPORTED_LOCATION ${CUDD_DIR}/cudd/.libs/libcudd.a)
set_target_properties(libcudd PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CUDD_DIR}/cudd)
target_link_libraries(main libcudd libsdd)


project(Tests)

set(GTEST_DIR ${CMAKE_SOURCE_DIR}/libs/googletest-release-1.12.1)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(googletest-release-1.12.1)
include_directories(${GTEST_DIR}/include ${GTEST_DIR})
include_directories(${CUDD_DIR}/cudd)
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(tests main.cpp utils_tests.h ${SRC_DIR}/utils/file_utils.cpp formula_tests.h)
add_test(NAME tests COMMAND tests)

target_link_libraries(tests PUBLIC gtest gtest_main)
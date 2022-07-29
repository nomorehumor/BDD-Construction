project(Tests)

set(GTEST_DIR ${CMAKE_SOURCE_DIR}/libs/googletest-release-1.12.1)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)


include_directories(${GTEST_DIR}/include ${GTEST_DIR})
include_directories(${CUDD_DIR}/cudd)
include_directories(${SRC_DIR})

file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS ${SRC_DIR}/bdd_formulas/*.h ${SRC_DIR}/bdd_formulas/*.cpp ${SRC_DIR}/utils/*.h ${SRC_DIR}/utils/*.cpp ${SRC_DIR}/stats/*.h ${SRC_DIR}/stats/*.cpp)

add_executable(tests main.cpp utils_tests.h formula_tests.h ${ALL_SOURCES} )
add_test(NAME tests COMMAND tests)

add_library(libcudd STATIC IMPORTED)
set_target_properties(libcudd PROPERTIES IMPORTED_LOCATION ${CUDD_DIR}/cudd/.libs/libcudd.a)
set_target_properties(libcudd PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CUDD_DIR}/cudd)

target_link_libraries(tests PUBLIC libcudd gtest gtest_main spdlog::spdlog ${PYTHON_LIBRARIES} Python3::NumPy yaml-cpp)

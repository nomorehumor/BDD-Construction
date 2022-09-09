project(Tests)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

include_directories(${GTEST_DIR}/googletest/include ${GTEST_DIR})
include_directories(${CUDD_DIR}/cudd)
include_directories(${SRC_DIR})

file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS ${SRC_DIR}/*_formulas/*.h ${SRC_DIR}/*_formulas/*.cpp ${SRC_DIR}/utils/*.h ${SRC_DIR}/utils/*.cpp ${SRC_DIR}/stats/*.h ${SRC_DIR}/stats/*.cpp)

add_executable(tests main.cpp utils_tests.h bdd/formula_tests.h sdd/formula_tests.h ${ALL_SOURCES} )
add_test(NAME tests COMMAND tests)

target_link_libraries(tests gtest gtest_main spdlog::spdlog ${PYTHON_LIBRARIES} Python3::NumPy yaml-cpp)

include_directories(${CUDD_DIR})

file(COPY ${CMAKE_SOURCE_DIR}/config DESTINATION ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(main main.cpp utils/file_utils.cpp utils/file_utils.h utils/output_utils.cpp utils/output_utils.h utils/progress_bar.cpp utils/progress_bar.h bdd_formulas/bdd_formulas.cpp bdd_formulas/bdd_formulas.h bdd_formulas/formula_ordering.cpp bdd_formulas/formula_ordering.h stats/BDDBuildStatistic.cpp stats/BDDBuildStatistic.h)
target_link_libraries(main spdlog::spdlog ${PYTHON_LIBRARIES} Python3::NumPy yaml-cpp)
target_include_directories(main PRIVATE ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})

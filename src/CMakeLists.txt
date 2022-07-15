include_directories(${CUDD_DIR})
add_executable(main main.cpp utils/file_utils.cpp utils/file_utils.h utils/output_utils.cpp utils/output_utils.h utils/progress_bar.cpp utils/progress_bar.h bdd_formulas/bdd_formulas.cpp bdd_formulas/bdd_formulas.h bdd_formulas/formula_ordering.cpp bdd_formulas/formula_ordering.h)
target_link_libraries(main spdlog::spdlog)
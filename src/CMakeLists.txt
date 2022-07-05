include_directories(${CUDD_DIR})
add_executable(main main.cpp formulas.cpp formulas.h utils/file_utils.cpp utils/file_utils.h tutorial.cpp tutorial.h utils/output_utils.cpp utils/output_utils.h progress_bar.cpp progress_bar.h )


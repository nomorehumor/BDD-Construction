# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/maxim/code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maxim/code/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/tests.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/tests.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/tests.dir/flags.make

tests/CMakeFiles/tests.dir/main.cpp.o: tests/CMakeFiles/tests.dir/flags.make
tests/CMakeFiles/tests.dir/main.cpp.o: ../tests/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maxim/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/tests.dir/main.cpp.o"
	cd /home/maxim/code/build/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tests.dir/main.cpp.o -c /home/maxim/code/tests/main.cpp

tests/CMakeFiles/tests.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tests.dir/main.cpp.i"
	cd /home/maxim/code/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maxim/code/tests/main.cpp > CMakeFiles/tests.dir/main.cpp.i

tests/CMakeFiles/tests.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tests.dir/main.cpp.s"
	cd /home/maxim/code/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maxim/code/tests/main.cpp -o CMakeFiles/tests.dir/main.cpp.s

tests/CMakeFiles/tests.dir/__/src/bdd_formulas/bdd_formulas.cpp.o: tests/CMakeFiles/tests.dir/flags.make
tests/CMakeFiles/tests.dir/__/src/bdd_formulas/bdd_formulas.cpp.o: ../src/bdd_formulas/bdd_formulas.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maxim/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tests/CMakeFiles/tests.dir/__/src/bdd_formulas/bdd_formulas.cpp.o"
	cd /home/maxim/code/build/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tests.dir/__/src/bdd_formulas/bdd_formulas.cpp.o -c /home/maxim/code/src/bdd_formulas/bdd_formulas.cpp

tests/CMakeFiles/tests.dir/__/src/bdd_formulas/bdd_formulas.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tests.dir/__/src/bdd_formulas/bdd_formulas.cpp.i"
	cd /home/maxim/code/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maxim/code/src/bdd_formulas/bdd_formulas.cpp > CMakeFiles/tests.dir/__/src/bdd_formulas/bdd_formulas.cpp.i

tests/CMakeFiles/tests.dir/__/src/bdd_formulas/bdd_formulas.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tests.dir/__/src/bdd_formulas/bdd_formulas.cpp.s"
	cd /home/maxim/code/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maxim/code/src/bdd_formulas/bdd_formulas.cpp -o CMakeFiles/tests.dir/__/src/bdd_formulas/bdd_formulas.cpp.s

tests/CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/FORCEPlacer.cpp.o: tests/CMakeFiles/tests.dir/flags.make
tests/CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/FORCEPlacer.cpp.o: ../src/bdd_formulas/static_ordering/FORCEPlacer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maxim/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object tests/CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/FORCEPlacer.cpp.o"
	cd /home/maxim/code/build/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/FORCEPlacer.cpp.o -c /home/maxim/code/src/bdd_formulas/static_ordering/FORCEPlacer.cpp

tests/CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/FORCEPlacer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/FORCEPlacer.cpp.i"
	cd /home/maxim/code/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maxim/code/src/bdd_formulas/static_ordering/FORCEPlacer.cpp > CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/FORCEPlacer.cpp.i

tests/CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/FORCEPlacer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/FORCEPlacer.cpp.s"
	cd /home/maxim/code/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maxim/code/src/bdd_formulas/static_ordering/FORCEPlacer.cpp -o CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/FORCEPlacer.cpp.s

tests/CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/formula_ordering.cpp.o: tests/CMakeFiles/tests.dir/flags.make
tests/CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/formula_ordering.cpp.o: ../src/bdd_formulas/static_ordering/formula_ordering.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maxim/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object tests/CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/formula_ordering.cpp.o"
	cd /home/maxim/code/build/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/formula_ordering.cpp.o -c /home/maxim/code/src/bdd_formulas/static_ordering/formula_ordering.cpp

tests/CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/formula_ordering.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/formula_ordering.cpp.i"
	cd /home/maxim/code/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maxim/code/src/bdd_formulas/static_ordering/formula_ordering.cpp > CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/formula_ordering.cpp.i

tests/CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/formula_ordering.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/formula_ordering.cpp.s"
	cd /home/maxim/code/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maxim/code/src/bdd_formulas/static_ordering/formula_ordering.cpp -o CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/formula_ordering.cpp.s

tests/CMakeFiles/tests.dir/__/src/stats/BDDBuildStatistic.cpp.o: tests/CMakeFiles/tests.dir/flags.make
tests/CMakeFiles/tests.dir/__/src/stats/BDDBuildStatistic.cpp.o: ../src/stats/BDDBuildStatistic.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maxim/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object tests/CMakeFiles/tests.dir/__/src/stats/BDDBuildStatistic.cpp.o"
	cd /home/maxim/code/build/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tests.dir/__/src/stats/BDDBuildStatistic.cpp.o -c /home/maxim/code/src/stats/BDDBuildStatistic.cpp

tests/CMakeFiles/tests.dir/__/src/stats/BDDBuildStatistic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tests.dir/__/src/stats/BDDBuildStatistic.cpp.i"
	cd /home/maxim/code/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maxim/code/src/stats/BDDBuildStatistic.cpp > CMakeFiles/tests.dir/__/src/stats/BDDBuildStatistic.cpp.i

tests/CMakeFiles/tests.dir/__/src/stats/BDDBuildStatistic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tests.dir/__/src/stats/BDDBuildStatistic.cpp.s"
	cd /home/maxim/code/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maxim/code/src/stats/BDDBuildStatistic.cpp -o CMakeFiles/tests.dir/__/src/stats/BDDBuildStatistic.cpp.s

tests/CMakeFiles/tests.dir/__/src/utils/file_utils.cpp.o: tests/CMakeFiles/tests.dir/flags.make
tests/CMakeFiles/tests.dir/__/src/utils/file_utils.cpp.o: ../src/utils/file_utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maxim/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object tests/CMakeFiles/tests.dir/__/src/utils/file_utils.cpp.o"
	cd /home/maxim/code/build/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tests.dir/__/src/utils/file_utils.cpp.o -c /home/maxim/code/src/utils/file_utils.cpp

tests/CMakeFiles/tests.dir/__/src/utils/file_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tests.dir/__/src/utils/file_utils.cpp.i"
	cd /home/maxim/code/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maxim/code/src/utils/file_utils.cpp > CMakeFiles/tests.dir/__/src/utils/file_utils.cpp.i

tests/CMakeFiles/tests.dir/__/src/utils/file_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tests.dir/__/src/utils/file_utils.cpp.s"
	cd /home/maxim/code/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maxim/code/src/utils/file_utils.cpp -o CMakeFiles/tests.dir/__/src/utils/file_utils.cpp.s

tests/CMakeFiles/tests.dir/__/src/utils/output_utils.cpp.o: tests/CMakeFiles/tests.dir/flags.make
tests/CMakeFiles/tests.dir/__/src/utils/output_utils.cpp.o: ../src/utils/output_utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maxim/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object tests/CMakeFiles/tests.dir/__/src/utils/output_utils.cpp.o"
	cd /home/maxim/code/build/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tests.dir/__/src/utils/output_utils.cpp.o -c /home/maxim/code/src/utils/output_utils.cpp

tests/CMakeFiles/tests.dir/__/src/utils/output_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tests.dir/__/src/utils/output_utils.cpp.i"
	cd /home/maxim/code/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maxim/code/src/utils/output_utils.cpp > CMakeFiles/tests.dir/__/src/utils/output_utils.cpp.i

tests/CMakeFiles/tests.dir/__/src/utils/output_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tests.dir/__/src/utils/output_utils.cpp.s"
	cd /home/maxim/code/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maxim/code/src/utils/output_utils.cpp -o CMakeFiles/tests.dir/__/src/utils/output_utils.cpp.s

tests/CMakeFiles/tests.dir/__/src/utils/progress_bar.cpp.o: tests/CMakeFiles/tests.dir/flags.make
tests/CMakeFiles/tests.dir/__/src/utils/progress_bar.cpp.o: ../src/utils/progress_bar.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maxim/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object tests/CMakeFiles/tests.dir/__/src/utils/progress_bar.cpp.o"
	cd /home/maxim/code/build/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tests.dir/__/src/utils/progress_bar.cpp.o -c /home/maxim/code/src/utils/progress_bar.cpp

tests/CMakeFiles/tests.dir/__/src/utils/progress_bar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tests.dir/__/src/utils/progress_bar.cpp.i"
	cd /home/maxim/code/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maxim/code/src/utils/progress_bar.cpp > CMakeFiles/tests.dir/__/src/utils/progress_bar.cpp.i

tests/CMakeFiles/tests.dir/__/src/utils/progress_bar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tests.dir/__/src/utils/progress_bar.cpp.s"
	cd /home/maxim/code/build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maxim/code/src/utils/progress_bar.cpp -o CMakeFiles/tests.dir/__/src/utils/progress_bar.cpp.s

# Object files for target tests
tests_OBJECTS = \
"CMakeFiles/tests.dir/main.cpp.o" \
"CMakeFiles/tests.dir/__/src/bdd_formulas/bdd_formulas.cpp.o" \
"CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/FORCEPlacer.cpp.o" \
"CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/formula_ordering.cpp.o" \
"CMakeFiles/tests.dir/__/src/stats/BDDBuildStatistic.cpp.o" \
"CMakeFiles/tests.dir/__/src/utils/file_utils.cpp.o" \
"CMakeFiles/tests.dir/__/src/utils/output_utils.cpp.o" \
"CMakeFiles/tests.dir/__/src/utils/progress_bar.cpp.o"

# External object files for target tests
tests_EXTERNAL_OBJECTS =

tests/tests: tests/CMakeFiles/tests.dir/main.cpp.o
tests/tests: tests/CMakeFiles/tests.dir/__/src/bdd_formulas/bdd_formulas.cpp.o
tests/tests: tests/CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/FORCEPlacer.cpp.o
tests/tests: tests/CMakeFiles/tests.dir/__/src/bdd_formulas/static_ordering/formula_ordering.cpp.o
tests/tests: tests/CMakeFiles/tests.dir/__/src/stats/BDDBuildStatistic.cpp.o
tests/tests: tests/CMakeFiles/tests.dir/__/src/utils/file_utils.cpp.o
tests/tests: tests/CMakeFiles/tests.dir/__/src/utils/output_utils.cpp.o
tests/tests: tests/CMakeFiles/tests.dir/__/src/utils/progress_bar.cpp.o
tests/tests: tests/CMakeFiles/tests.dir/build.make
tests/tests: ../libs/cudd-3.0.0/cudd/.libs/libcudd.a
tests/tests: lib/libgtest.a
tests/tests: lib/libgtest_main.a
tests/tests: libs/spdlog/libspdlogd.a
tests/tests: /usr/lib/x86_64-linux-gnu/libpython3.8.so
tests/tests: libs/yaml-cpp/libyaml-cppd.a
tests/tests: lib/libgtest.a
tests/tests: tests/CMakeFiles/tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/maxim/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable tests"
	cd /home/maxim/code/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/tests.dir/build: tests/tests

.PHONY : tests/CMakeFiles/tests.dir/build

tests/CMakeFiles/tests.dir/clean:
	cd /home/maxim/code/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/tests.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/tests.dir/clean

tests/CMakeFiles/tests.dir/depend:
	cd /home/maxim/code/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maxim/code /home/maxim/code/tests /home/maxim/code/build /home/maxim/code/build/tests /home/maxim/code/build/tests/CMakeFiles/tests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/tests.dir/depend


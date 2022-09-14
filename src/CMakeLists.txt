set(CMAKE_BUILD_TYPE RelWithDebInfo)

file(COPY ${CMAKE_SOURCE_DIR}/config DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/scripts/create_results.py DESTINATION ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE ALL_SOURCES *.cpp *.h)
add_executable(main ${ALL_SOURCES})

target_link_libraries(main spdlog::spdlog ${PYTHON_LIBRARIES} Python3::NumPy yaml-cpp)
target_include_directories(main PRIVATE ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})
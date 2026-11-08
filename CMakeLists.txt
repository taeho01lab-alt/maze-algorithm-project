cmake_minimum_required(VERSION 3.16)
project(maze_algorithm_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(maze_core
    src/maze.cpp src/io.cpp src/registry.cpp
    src/algorithm_a.cpp src/algorithm_b.cpp src/algorithm_c.cpp
    src/validation.cpp src/analysis.cpp src/experiments.cpp)
target_include_directories(maze_core PUBLIC include)
if(MSVC)
    target_compile_options(maze_core PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(maze_core PRIVATE -Wall -Wextra -Wpedantic)
endif()
add_executable(maze_app src/main.cpp)
target_link_libraries(maze_app PRIVATE maze_core)

include(CTest)
if(BUILD_TESTING)
    add_executable(maze_core_tests tests/test_core.cpp)
    target_link_libraries(maze_core_tests PRIVATE maze_core)
    add_test(NAME common_maze_contract COMMAND maze_core_tests)
    add_test(NAME cli_help COMMAND maze_app --help)
endif()


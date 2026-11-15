#pragma once

#include "maze.h"
#include <cstdint>
#include <string_view>

using Generator = Maze (*)(int width, int height, std::uint32_t seed);

Maze generateA(int width, int height, std::uint32_t seed); // Randomized DFS
Maze generateB(int width, int height, std::uint32_t seed); // Randomized Kruskal
Maze generateC(int width, int height, std::uint32_t seed); // Sidewinder
Generator generatorFor(std::string_view id);


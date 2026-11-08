#pragma once

#include "maze.h"
#include <cstdint>
#include <string_view>

using Generator = Maze (*)(int width, int height, std::uint32_t seed);

Maze generateA(int width, int height, std::uint32_t seed);
Maze generateB(int width, int height, std::uint32_t seed);
Maze generateC(int width, int height, std::uint32_t seed);
Generator generatorFor(std::string_view id);


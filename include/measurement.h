#pragma once

#include "analysis.h"
#include <cstdint>
#include <string_view>

struct MeasuredMaze {
    Maze maze;
    MazeMetrics metrics;
    double generationMs = 0.0;
};

// Time one generation call, then validate and analyze the resulting maze.
MeasuredMaze generateAndAnalyze(std::string_view algorithm, int width, int height,
                               std::uint32_t seed);

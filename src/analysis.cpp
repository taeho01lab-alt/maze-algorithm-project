#include "analysis.h"
#include <stdexcept>

MazeMetrics analyzeMaze(const Maze& maze) {
    // Comparison-results owner: dead ends, S-to-E distance, straight corridors.
    // Agree on corridor endpoints/branches and units before implementation.
    (void)maze;
    throw std::logic_error("Maze metrics are not implemented yet");
}


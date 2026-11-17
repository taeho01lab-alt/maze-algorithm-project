#pragma once

#include "maze.h"
#include <cstddef>

struct ValidationResult {
    bool connected = false;
    bool acyclic = false;
    std::size_t cells = 0;
    std::size_t passages = 0;
    bool perfect() const { return connected && acyclic; }
};

struct MazeMetrics {
    std::size_t deadEnds = 0;
    double averageCorridorLength = 0.0;
    std::size_t longestPathLength = 0; // Tree diameter, measured in moves.
};

// Malformed shape/walls throw invalid_argument; disconnected/cyclic graphs
// produce flags. Analysis requires a perfect maze and rejects other graphs.
ValidationResult validatePerfectMaze(const Maze& maze);
MazeMetrics analyzeMaze(const Maze& maze);


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
    std::size_t pathLength = 0; // Number of moves from S to E.
    double averageCorridorLength = 0.0;
};

ValidationResult validatePerfectMaze(const Maze& maze); // Member 2
MazeMetrics analyzeMaze(const Maze& maze);              // Member 4


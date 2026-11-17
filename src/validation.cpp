#include "analysis.h"
#include "maze_graph.h"

ValidationResult validatePerfectMaze(const Maze& maze) {
    return maze_detail::inspect(maze_detail::graphFor(maze));
}


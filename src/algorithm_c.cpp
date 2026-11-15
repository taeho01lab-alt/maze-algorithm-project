#include "algorithms.h"
#include "generator_random.h"

Maze generateC(int width, int height, std::uint32_t seed) {
    Maze maze = makeMaze(width, height);
    std::mt19937 random(seed);
    for (int row = 0; row < height; ++row) {
        int runStart = 0;
        for (int col = 0; col < width; ++col) {
            const bool atEastBoundary = col == width - 1;
            // The first row is one horizontal run. Every lower run has exactly
            // one north connection to the already connected preceding rows.
            const bool closeRun = atEastBoundary ||
                (row > 0 && maze_detail::randomIndex(random, 2) == 0);
            if (closeRun) {
                if (row > 0) {
                    const int selectedCol = runStart + static_cast<int>(
                        maze_detail::randomIndex(random, static_cast<std::size_t>(col - runStart + 1)));
                    openPassage(maze, row, selectedCol, row - 1, selectedCol);
                }
                runStart = col + 1;
            } else {
                openPassage(maze, row, col, row, col + 1);
            }
        }
    }
    return maze;
}


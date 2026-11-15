#include "algorithms.h"
#include "generator_random.h"

#include <array>
#include <vector>

Maze generateA(int width, int height, std::uint32_t seed) {
    Maze maze = makeMaze(width, height);
    std::mt19937 random(seed);
    std::vector<std::vector<bool>> visited(
        static_cast<std::size_t>(height),
        std::vector<bool>(static_cast<std::size_t>(width), false));
    // An explicit stack performs DFS backtracking without recursive stack limits.
    std::vector<Point> stack{{0, 0}};
    visited[0][0] = true;
    constexpr std::array<Point, 4> directions{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};

    while (!stack.empty()) {
        const Point current = stack.back();
        std::array<Point, 4> candidates{};
        std::size_t count = 0;
        for (const auto direction : directions) {
            const Point next{current.row + direction.row, current.col + direction.col};
            if (next.row >= 0 && next.row < height && next.col >= 0 && next.col < width &&
                !visited[next.row][next.col]) {
                candidates[count++] = next;
            }
        }
        if (count == 0) {
            stack.pop_back();
            continue;
        }
        const Point next = candidates[maze_detail::randomIndex(random, count)];
        // Only enter an unvisited cell: each new passage adds one new vertex.
        openPassage(maze, current.row, current.col, next.row, next.col);
        visited[next.row][next.col] = true;
        stack.push_back(next);
    }
    return maze;
}


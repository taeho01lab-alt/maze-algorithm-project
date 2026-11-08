#include "maze.h"

#include <limits>
#include <stdexcept>

Maze makeMaze(int width, int height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Maze width and height must be positive");
    }
    return Maze(static_cast<std::size_t>(height),
                std::vector<Cell>(static_cast<std::size_t>(width)));
}

int mazeHeight(const Maze& maze) {
    if (maze.empty() || maze.size() > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::invalid_argument("Invalid maze height");
    }
    return static_cast<int>(maze.size());
}

int mazeWidth(const Maze& maze) {
    mazeHeight(maze);
    const auto width = maze.front().size();
    if (width == 0 || width > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::invalid_argument("Invalid maze width");
    }
    for (const auto& row : maze) {
        if (row.size() != width) throw std::invalid_argument("Maze must be rectangular");
    }
    return static_cast<int>(width);
}

bool isInside(const Maze& maze, Point p) {
    return p.row >= 0 && p.col >= 0 &&
           static_cast<std::size_t>(p.row) < maze.size() &&
           static_cast<std::size_t>(p.col) < maze[static_cast<std::size_t>(p.row)].size();
}

namespace {
void setWall(Maze& maze, Point a, Point b, bool wall) {
    if (!isInside(maze, a) || !isInside(maze, b)) {
        throw std::out_of_range("Cell outside maze");
    }
    auto& first = maze[static_cast<std::size_t>(a.row)][static_cast<std::size_t>(a.col)];
    auto& second = maze[static_cast<std::size_t>(b.row)][static_cast<std::size_t>(b.col)];
    const int dr = b.row - a.row;
    const int dc = b.col - a.col;
    if (dr == -1 && dc == 0) first.top = second.bottom = wall;
    else if (dr == 1 && dc == 0) first.bottom = second.top = wall;
    else if (dr == 0 && dc == -1) first.left = second.right = wall;
    else if (dr == 0 && dc == 1) first.right = second.left = wall;
    else throw std::invalid_argument("Cells must be orthogonally adjacent");
}
}

void openPassage(Maze& maze, int r, int c, int nr, int nc) {
    setWall(maze, {r, c}, {nr, nc}, false);
}

void closePassage(Maze& maze, int r, int c, int nr, int nc) {
    setWall(maze, {r, c}, {nr, nc}, true);
}


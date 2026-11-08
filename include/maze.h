#pragma once

#include <vector>

// Access: maze[row][col]. true = wall; false = passage.
struct Cell {
    bool top = true;
    bool bottom = true;
    bool left = true;
    bool right = true;
};

using Maze = std::vector<std::vector<Cell>>;

struct Point {
    int row;
    int col;
};

Maze makeMaze(int width, int height);
int mazeWidth(const Maze& maze);
int mazeHeight(const Maze& maze);
bool isInside(const Maze& maze, Point point);
void openPassage(Maze& maze, int row, int col, int nextRow, int nextCol);
void closePassage(Maze& maze, int row, int col, int nextRow, int nextCol);


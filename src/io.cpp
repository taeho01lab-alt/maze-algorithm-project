#include "io.h"

#include <fstream>
#include <stdexcept>

std::string renderMaze(const Maze& maze) {
    const int width = mazeWidth(maze);
    const int height = mazeHeight(maze);
    const auto rows = static_cast<std::size_t>(height) * 2 + 1;
    const auto cols = static_cast<std::size_t>(width) * 2 + 1;
    std::vector<std::string> text(rows, std::string(cols, '#'));
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            const auto& cell = maze[r][c];
            if ((r == 0 && !cell.top) || (c == 0 && !cell.left) ||
                (r == height - 1 && !cell.bottom) || (c == width - 1 && !cell.right)) {
                throw std::invalid_argument("Outer boundary must remain closed");
            }
            if ((c + 1 < width && cell.right != maze[r][c + 1].left) ||
                (r + 1 < height && cell.bottom != maze[r + 1][c].top)) {
                throw std::invalid_argument("Neighboring walls disagree");
            }
            const auto y = static_cast<std::size_t>(r) * 2 + 1;
            const auto x = static_cast<std::size_t>(c) * 2 + 1;
            text[y][x] = ' ';
            if (!cell.top) text[y - 1][x] = ' ';
            if (!cell.bottom) text[y + 1][x] = ' ';
            if (!cell.left) text[y][x - 1] = ' ';
            if (!cell.right) text[y][x + 1] = ' ';
        }
    }
    text[1][1] = 'S';
    if (width != 1 || height != 1) text[rows - 2][cols - 2] = 'E';
    std::string result;
    for (const auto& row : text) { result += row; result += '\n'; }
    return result;
}

void saveMaze(const Maze& maze, const std::filesystem::path& path) {
    const auto text = renderMaze(maze);
    std::ofstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open output file");
    file << text;
    if (!file) throw std::runtime_error("Cannot write output file");
}


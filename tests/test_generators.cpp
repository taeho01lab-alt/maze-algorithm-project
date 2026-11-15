#include "algorithms.h"
#include "io.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

// Independent graph checks: connected + exactly V-1 undirected edges is a tree.
// The production validation module is intentionally not used by these tests.
void checkPerfect(const Maze& maze, int width, int height) {
    check(mazeWidth(maze) == width && mazeHeight(maze) == height, "Wrong dimensions");
    const auto vertices = static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
    std::size_t edges = 0;
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            const Cell& cell = maze[row][col];
            check(row != 0 || cell.top, "Open north boundary");
            check(col != 0 || cell.left, "Open west boundary");
            check(row != height - 1 || cell.bottom, "Open south boundary");
            check(col != width - 1 || cell.right, "Open east boundary");
            if (col + 1 < width) {
                check(cell.right == maze[row][col + 1].left, "Asymmetric east wall");
                if (!cell.right) ++edges;
            }
            if (row + 1 < height) {
                check(cell.bottom == maze[row + 1][col].top, "Asymmetric south wall");
                if (!cell.bottom) ++edges;
            }
        }
    }
    check(edges == vertices - 1, "Wrong passage count: maze is not a tree");
    std::vector<std::vector<bool>> seen(
        static_cast<std::size_t>(height), std::vector<bool>(static_cast<std::size_t>(width), false));
    std::vector<Point> queue{{0, 0}};
    seen[0][0] = true;
    for (std::size_t i = 0; i < queue.size(); ++i) {
        const Point point = queue[i];
        const Cell& cell = maze[point.row][point.col];
        const std::array<Point, 4> neighbors{{
            {point.row - 1, point.col}, {point.row + 1, point.col},
            {point.row, point.col - 1}, {point.row, point.col + 1}}};
        const std::array<bool, 4> walls{{cell.top, cell.bottom, cell.left, cell.right}};
        for (std::size_t j = 0; j < neighbors.size(); ++j) {
            if (walls[j]) continue;
            const Point next = neighbors[j];
            check(next.row >= 0 && next.row < height && next.col >= 0 && next.col < width,
                  "Passage leaves grid");
            if (!seen[next.row][next.col]) {
                seen[next.row][next.col] = true;
                queue.push_back(next);
            }
        }
    }
    check(queue.size() == vertices, "Disconnected maze");
}
}

int main() {
    try {
        const std::array<Generator, 3> generators{{generateA, generateB, generateC}};
        check(generatorFor("dfs") == generateA && generatorFor("A") == generateA, "DFS alias");
        check(generatorFor("kruskal") == generateB && generatorFor("B") == generateB, "Kruskal alias");
        check(generatorFor("sidewinder") == generateC && generatorFor("C") == generateC, "Sidewinder alias");
        std::vector<Point> dimensions;
        for (int height = 1; height <= 8; ++height) {
            for (int width = 1; width <= 8; ++width) dimensions.push_back({height, width});
        }
        for (const Point size : {Point{10, 10}, Point{20, 20}, Point{30, 30}, Point{7, 19}}) {
            dimensions.push_back(size);
        }
        std::vector<std::uint32_t> seeds;
        for (std::uint32_t seed = 0; seed < 32; ++seed) seeds.push_back(seed);
        seeds.push_back(123456789);
        seeds.push_back(std::numeric_limits<std::uint32_t>::max());
        std::size_t cases = 0;
        for (const auto generate : generators) {
            for (const Point size : dimensions) {
                for (const auto seed : seeds) {
                    const auto maze = generate(size.col, size.row, seed);
                    checkPerfect(maze, size.col, size.row);
                    const auto expected = renderMaze(maze);
                    // An unrelated generation must not change future seeded output.
                    (void)generate(3, 5, seed + std::uint32_t{1});
                    check(renderMaze(generate(size.col, size.row, seed)) == expected,
                          "Seeded result depends on previous calls");
                    ++cases;
                }
            }
            for (const Point size : {Point{1, 50000}, Point{50000, 1}, Point{256, 256}}) {
                checkPerfect(generate(size.col, size.row, 42), size.col, size.row);
                ++cases;
            }
            for (const Point size : {Point{0, 1}, Point{1, 0}, Point{-1, 2}, Point{2, -1}}) {
                bool rejected = false;
                try { (void)generate(size.col, size.row, 42); }
                catch (const std::invalid_argument&) { rejected = true; }
                check(rejected, "Generator accepted a non-positive dimension");
            }
            bool varied = false;
            const auto first = renderMaze(generate(10, 10, 0));
            for (std::uint32_t seed = 1; seed < 32; ++seed) {
                if (renderMaze(generate(10, 10, seed)) != first) varied = true;
            }
            check(varied, "Generator appears to ignore the seed");
        }
        std::cout << cases << " perfect-maze cases passed; seeded replay, aliases and invalid sizes passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}

#include "algorithms.h"
#include "generator_random.h"

#include <limits>
#include <numeric>
#include <utility>
#include <vector>

namespace {
struct Edge { Point first; Point second; };

class DisjointSet {
public:
    explicit DisjointSet(std::size_t count) : parent_(count), size_(count, 1) {
        std::iota(parent_.begin(), parent_.end(), std::size_t{0});
    }

    bool unite(std::size_t first, std::size_t second) {
        first = find(first);
        second = find(second);
        if (first == second) return false;
        if (size_[first] < size_[second]) std::swap(first, second);
        parent_[second] = first;
        size_[first] += size_[second];
        return true;
    }

private:
    std::size_t find(std::size_t cell) {
        while (cell != parent_[cell]) {
            parent_[cell] = parent_[parent_[cell]];
            cell = parent_[cell];
        }
        return cell;
    }
    std::vector<std::size_t> parent_;
    std::vector<std::size_t> size_;
};
}

Maze generateB(int width, int height, std::uint32_t seed) {
    Maze maze = makeMaze(width, height);
    const auto columns = static_cast<std::size_t>(width);
    const auto rows = static_cast<std::size_t>(height);
    if (columns > std::numeric_limits<std::size_t>::max() / rows) {
        throw std::length_error("Maze has too many cells");
    }
    const auto cellCount = columns * rows;
    if (cellCount == 1) return maze;

    std::vector<Edge> edges;
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            // Right and down list each undirected edge exactly once.
            if (col + 1 < width) edges.push_back({{row, col}, {row, col + 1}});
            if (row + 1 < height) edges.push_back({{row, col}, {row + 1, col}});
        }
    }
    std::mt19937 random(seed);
    for (std::size_t remaining = edges.size(); remaining > 1; --remaining) {
        std::swap(edges[remaining - 1], edges[maze_detail::randomIndex(random, remaining)]);
    }

    DisjointSet sets(cellCount);
    std::size_t passages = 0;
    for (const auto& edge : edges) {
        const auto first = static_cast<std::size_t>(edge.first.row) * columns +
                           static_cast<std::size_t>(edge.first.col);
        const auto second = static_cast<std::size_t>(edge.second.row) * columns +
                            static_cast<std::size_t>(edge.second.col);
        // Joining different components cannot introduce a cycle.
        if (sets.unite(first, second)) {
            openPassage(maze, edge.first.row, edge.first.col, edge.second.row, edge.second.col);
            if (++passages == cellCount - 1) break;
        }
    }
    return maze;
}


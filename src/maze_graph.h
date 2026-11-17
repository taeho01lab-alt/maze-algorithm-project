#pragma once

#include "analysis.h"
#include <algorithm>
#include <array>
#include <limits>
#include <stdexcept>
#include <vector>

namespace maze_detail {
struct MazeGraph {
    int width;
    std::vector<std::vector<std::size_t>> neighbors;
    std::size_t edges = 0;
};

// Inspect logical cells, never the rendered character grid.
inline MazeGraph graphFor(const Maze& maze) {
    const int width = mazeWidth(maze);
    const int height = mazeHeight(maze);
    const auto w = static_cast<std::size_t>(width);
    if (static_cast<std::size_t>(height) > std::numeric_limits<std::size_t>::max() / w)
        throw std::invalid_argument("Maze cell count is too large");
    MazeGraph graph{width, std::vector<std::vector<std::size_t>>(w * height), 0};
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            const auto& cell = maze[r][c];
            if ((r == 0 && !cell.top) || (c == 0 && !cell.left) ||
                (r == height - 1 && !cell.bottom) || (c == width - 1 && !cell.right))
                throw std::invalid_argument("Outer boundary must remain closed");
            if ((c + 1 < width && cell.right != maze[r][c + 1].left) ||
                (r + 1 < height && cell.bottom != maze[r + 1][c].top))
                throw std::invalid_argument("Neighboring walls disagree");
            const auto id = static_cast<std::size_t>(r) * w + c;
            auto connect = [&](std::size_t next) {
                graph.neighbors[id].push_back(next);
                graph.neighbors[next].push_back(id);
                ++graph.edges;
            };
            if (c + 1 < width && !cell.right) connect(id + 1);
            if (r + 1 < height && !cell.bottom) connect(id + w);
        }
    }
    return graph;
}

inline ValidationResult inspect(const MazeGraph& graph) {
    const auto count = graph.neighbors.size();
    std::vector<bool> seen(count, false);
    std::vector<std::size_t> queue;
    queue.reserve(count);
    std::size_t components = 0;
    for (std::size_t start = 0; start < count; ++start) {
        if (seen[start]) continue;
        ++components;
        queue.clear();
        queue.push_back(start);
        seen[start] = true;
        for (std::size_t i = 0; i < queue.size(); ++i) {
            for (const auto next : graph.neighbors[queue[i]]) {
                if (seen[next]) continue;
                seen[next] = true;
                queue.push_back(next);
            }
        }
    }
    // An undirected graph is a forest iff E = V - component_count.
    return {components == 1, graph.edges == count - components, count, graph.edges};
}

inline std::vector<std::size_t> distances(const MazeGraph& graph, std::size_t start) {
    const auto unseen = std::numeric_limits<std::size_t>::max();
    std::vector<std::size_t> distance(graph.neighbors.size(), unseen);
    std::vector<std::size_t> queue{start};
    distance[start] = 0;
    for (std::size_t i = 0; i < queue.size(); ++i) {
        const auto current = queue[i];
        for (const auto next : graph.neighbors[current]) {
            if (distance[next] != unseen) continue;
            distance[next] = distance[current] + 1;
            queue.push_back(next);
        }
    }
    return distance;
}
} // namespace maze_detail

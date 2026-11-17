#include "analysis.h"
#include "maze_graph.h"
#include <algorithm>
#include <stdexcept>

MazeMetrics analyzeMaze(const Maze& maze) {
    const auto graph = maze_detail::graphFor(maze);
    if (!maze_detail::inspect(graph).perfect())
        throw std::invalid_argument("Analysis requires a connected, acyclic perfect maze");
    MazeMetrics result;
    std::size_t corridorEnds = 0;
    for (std::size_t id = 0; id < graph.neighbors.size(); ++id) {
        const auto& neighbors = graph.neighbors[id];
        if (neighbors.size() == 1) ++result.deadEnds;
        bool straightInterior = false;
        if (neighbors.size() == 2) {
            const auto w = static_cast<std::size_t>(graph.width);
            const auto a = neighbors[0], b = neighbors[1];
            straightInterior = (a / w == id / w && b / w == id / w) ||
                               (a % w == id % w && b % w == id % w);
        }
        // At a bend, junction or end, each incident passage ends a segment.
        if (!straightInterior) corridorEnds += neighbors.size();
    }
    const auto corridorCount = corridorEnds / 2;
    if (corridorCount != 0)
        result.averageCorridorLength = static_cast<double>(graph.edges) / corridorCount;
    const auto fromStart = maze_detail::distances(graph, 0);
    const auto endpoint = static_cast<std::size_t>(
        std::max_element(fromStart.begin(), fromStart.end()) - fromStart.begin());
    const auto fromEndpoint = maze_detail::distances(graph, endpoint);
    result.longestPathLength = *std::max_element(fromEndpoint.begin(), fromEndpoint.end());
    return result;
}


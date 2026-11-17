#include "analysis.h"
#include "algorithms.h"
#include "measurement.h"
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
void rejects(const std::function<void()>& action) {
    try { action(); } catch (const std::invalid_argument&) { return; }
    throw std::runtime_error("Invalid analysis input accepted");
}

// Independent all-pairs traversal checks tree diameter on small generated mazes.
std::size_t bruteDiameter(const Maze& maze) {
    const int h = mazeHeight(maze), w = mazeWidth(maze);
    std::size_t best = 0;
    for (int r = 0; r < h; ++r) for (int c = 0; c < w; ++c) {
        std::vector<std::vector<int>> distances(h, std::vector<int>(w, -1));
        std::vector<Point> queue{{r, c}};
        distances[r][c] = 0;
        for (std::size_t i = 0; i < queue.size(); ++i) {
            const auto p = queue[i];
            const auto& cell = maze[p.row][p.col];
            const Point next[] = {{p.row-1,p.col},{p.row+1,p.col},{p.row,p.col-1},{p.row,p.col+1}};
            const bool walls[] = {cell.top,cell.bottom,cell.left,cell.right};
            for (int j = 0; j < 4; ++j) {
                if (walls[j]) continue;
                const auto n = next[j];
                if (distances[n.row][n.col] != -1) continue;
                distances[n.row][n.col] = distances[p.row][p.col] + 1;
                best = std::max(best, static_cast<std::size_t>(distances[n.row][n.col]));
                queue.push_back(n);
            }
        }
    }
    return best;
}
}

int main() {
    try {
        const auto singleton = analyzeMaze(makeMaze(1, 1));
        check(singleton.deadEnds == 0 && singleton.longestPathLength == 0 &&
              singleton.averageCorridorLength == 0.0, "1x1 metrics");
        for (const bool vertical : {false, true}) {
            auto line = makeMaze(vertical ? 1 : 5, vertical ? 5 : 1);
            for (int i = 0; i < 4; ++i)
                openPassage(line, vertical ? i : 0, vertical ? 0 : i,
                            vertical ? i+1 : 0, vertical ? 0 : i+1);
            const auto m = analyzeMaze(line);
            check(m.deadEnds == 2 && m.longestPathLength == 4 && m.averageCorridorLength == 4.0, "Straight corridor fixture");
        }
        // S --down--> bottom-left --> E --up--> top-right: diameter != S-to-E.
        auto bent = makeMaze(2, 2);
        openPassage(bent,0,0,1,0); openPassage(bent,1,0,1,1); openPassage(bent,1,1,0,1);
        const auto bm = analyzeMaze(bent);
        check(bm.deadEnds == 2 && bm.longestPathLength == 3 && bm.averageCorridorLength == 1.0, "Bends and diameter fixture");
        // A branch interrupts a straight horizontal trunk; outer cells bend.
        auto branched = makeMaze(3, 2);
        openPassage(branched,0,0,0,1); openPassage(branched,0,1,0,2);
        for (int c = 0; c < 3; ++c) openPassage(branched,0,c,1,c);
        const auto branch = analyzeMaze(branched);
        check(branch.deadEnds == 3 && branch.longestPathLength == 4 && branch.averageCorridorLength == 1.0, "Branch segment fixture");
        const auto disconnected = validatePerfectMaze(makeMaze(2, 2));
        check(!disconnected.connected && disconnected.acyclic, "Disconnected forest flags");
        rejects([] { analyzeMaze(makeMaze(2,2)); });
        openPassage(bent,0,0,0,1);
        const auto cycle = validatePerfectMaze(bent);
        check(cycle.connected && !cycle.acyclic, "Cycle flags");
        rejects([&] { analyzeMaze(bent); });
        auto mixed = makeMaze(3,2);
        openPassage(mixed,0,0,0,1); openPassage(mixed,0,1,1,1);
        openPassage(mixed,1,1,1,0); openPassage(mixed,1,0,0,0);
        openPassage(mixed,0,2,1,2);
        const auto mixedFlags = validatePerfectMaze(mixed);
        check(!mixedFlags.connected && !mixedFlags.acyclic, "Disconnected cyclic graph flags");
        rejects([] { analyzeMaze(Maze{}); });
        rejects([] { analyzeMaze(Maze{{Cell{}},{Cell{},Cell{}}}); });
        auto badWall = makeMaze(2,1); badWall[0][0].right = false;
        rejects([&] { analyzeMaze(badWall); });
        auto badBoundary = makeMaze(1,1); badBoundary[0][0].top = false;
        rejects([&] { validatePerfectMaze(badBoundary); });
        for (const auto generator : {generateA,generateB,generateC})
            for (std::uint32_t seed = 0; seed < 12; ++seed)
                for (int h = 1; h <= 4; ++h) for (int w = 1; w <= 4; ++w) {
                    const auto maze = generator(w,h,seed);
                    const auto valid = validatePerfectMaze(maze);
                    check(valid.perfect() && valid.cells == static_cast<std::size_t>(w*h) &&
                          valid.passages + 1 == valid.cells, "Generated validation");
                    const auto m = analyzeMaze(maze);
                    check(m.longestPathLength == bruteDiameter(maze), "All-pairs diameter mismatch");
                }
        const auto timed = generateAndAnalyze("dfs", 4, 3, 42);
        check(std::isfinite(timed.generationMs) && timed.generationMs >= 0.0, "Invalid generation time");
        rejects([] { generateAndAnalyze("unknown", 4, 3, 42); });
        std::cout << "Analysis fixtures, 576 all-pairs diameter checks and generation timing passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}

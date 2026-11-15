# Implemented maze generators

All three functions accept `(width, height, seed)` and return the shared `Maze`.
Dimensions must be positive. `true` means a wall; `openPassage` updates both
neighboring cells. The outside boundary stays closed. The renderer marks S at
the top-left cell and E at the bottom-right cell (S only for a 1x1 maze).

## A / dfs: Randomized DFS

Start at (0,0), mark it visited, and push it onto an explicit stack. Look at
the top cell and select an unvisited orthogonal neighbor at random. Open the
wall, mark the neighbor visited and push it. When there are no unvisited
neighbors, pop the stack to backtrack. Repeat until the stack is empty.

Only passages to unvisited cells are opened, so each passage attaches one new
cell without making a cycle. DFS reaches every cell of the rectangular grid.
The result is therefore connected and acyclic: a perfect maze. The explicit
stack avoids a recursive call for every cell in a long path.

For N = width * height, time is O(N) in expectation and auxiliary space is
O(N), excluding the O(N) output maze. Every cell has at most four neighbors.
Long, winding passages are a tendency, not a measured conclusion for this team.

## B / kruskal: Randomized Kruskal

List every interior edge once (right and down neighbors). Shuffle the list
using Fisher-Yates. Initially, each cell is its own disjoint set. For each
edge, use Union-Find to check whether its endpoints belong to different sets.
If they do, join the sets and open the corresponding wall. Stop after N-1
passages have been opened.

An edge inside the same component is rejected because it would create a
cycle. Since the original rectangular grid is connected, processing its edges
eventually joins all components. N connected cells with N-1 edges form a tree.
This is the randomized maze variant: edges are shuffled, not sorted by weights.

For E = height*(width-1) + width*(height-1), time is O(N + E*alpha(N))
amortized in expectation, using path compression and union by size. alpha is
the inverse Ackermann function, which grows extremely slowly. Since E = O(N)
on a rectangular grid, this is O(N*alpha(N)). Auxiliary space is O(N+E)=O(N).
The 1x1 case returns immediately with no edges.

## C / sidewinder: Sidewinder

Process rows from top to bottom and columns from left to right. On the top
row, connect every cell eastward. On each lower row, maintain a horizontal
run. Randomly either extend it east or close it. To close a run, choose one
cell in the run, connect it north, then start a new run. The east boundary
always closes the run.

The top row is a tree. Each lower run is a horizontal chain attached to the
already connected rows by exactly one north edge. Adding such a chain cannot
make a cycle and connects every new cell. Induction over the rows proves the
whole maze is a perfect maze. For width 1, each lower cell connects north;
for height 1, the only row is a horizontal chain.

Expected time is O(N), with O(1) auxiliary space excluding the returned maze.
The uninterrupted top-row corridor is a structural bias of this variant.

## Seed policy and integration

Each call creates a fresh `std::mt19937` from the supplied 32-bit seed.
The helper `src/generator_random.h` uses rejection sampling for an unbiased
bounded choice. Explicit choices and shuffling avoid platform-specific
`uniform_int_distribution`/`shuffle` behavior. Same dimensions, algorithm and
seed replay exactly; no global RNG state is used. Different seeds can still
produce the same maze, especially for one-row and one-column grids.

`tests/test_generators.cpp` checks the outputs independently: matching walls,
closed boundaries, connectivity by graph traversal, and exactly N-1 passages.
It also checks repeated seeds, invalid dimensions, narrow and larger grids.
These tests do not implement or replace the production `validatePerfectMaze`
API; that module remains for the generation-results owner to complete.

The comparison-results owner can call `generatorFor(id)` and invoke the
returned function. Measure only the agreed generation interval. Render/save,
validation, maze analysis and CSV writing belong outside that interval.
Sample files are examples, not experimental performance measurements.

## References

These implementations were written for the project using the standard methods
explained in the following algorithm descriptions; they are not copied source
code from those articles.

- Jamis Buck, [Maze Generation: Recursive Backtracking](https://weblog.jamisbuck.org/2010/12/27/maze-generation-recursive-backtracking).
- Jamis Buck, [Maze Generation: Kruskal's Algorithm](https://weblog.jamisbuck.org/2011/1/3/maze-generation-kruskal-s-algorithm).
- Jamis Buck, [Maze Generation: Sidewinder Algorithm](https://weblog.jamisbuck.org/2011/2/3/maze-generation-sidewinder-algorithm).

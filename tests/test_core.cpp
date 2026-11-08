#include "maze.h"
#include "io.h"
#include <functional>
#include <iostream>
#include <stdexcept>

void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
void rejects(const std::function<void()>& action) {
    try { action(); } catch (const std::invalid_argument&) { return; }
    catch (const std::out_of_range&) { return; }
    throw std::runtime_error("Expected invalid input to be rejected");
}

int main() {
    try {
        rejects([] { makeMaze(0, 2); });
        rejects([] { makeMaze(2, -1); });
        check(renderMaze(makeMaze(1, 1)) == "###\n#S#\n###\n", "1x1 rendering");
        Maze maze = makeMaze(2, 2);
        const auto before = renderMaze(maze);
        rejects([&] { openPassage(maze, 0, 0, 1, 1); });
        rejects([&] { openPassage(maze, 0, 0, -1, 0); });
        rejects([&] { openPassage(maze, 0, 0, 0, 0); });
        check(renderMaze(maze) == before, "Rejected edit must not mutate maze");
        openPassage(maze, 0, 0, 0, 1);
        openPassage(maze, 0, 1, 1, 1);
        openPassage(maze, 1, 1, 1, 0);
        check(renderMaze(maze) == "#####\n#S  #\n### #\n#  E#\n#####\n", "2x2 fixture rendering");
        closePassage(maze, 1, 0, 1, 1);
        check(maze[1][0].right && maze[1][1].left, "Symmetric closing");
        closePassage(maze, 1, 1, 0, 1);
        closePassage(maze, 0, 1, 0, 0);
        check(renderMaze(maze) == before, "All directions restore initial walls");
        maze[0][0].right = false;
        rejects([&] { renderMaze(maze); });
        maze[0][1].left = false;
        maze[0][0].top = false;
        rejects([&] { renderMaze(maze); });
        rejects([] { renderMaze(Maze{}); });
        rejects([] { renderMaze(Maze{{Cell{}}, {Cell{}, Cell{}}}); });
        std::cout << "Common maze contract checks passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}


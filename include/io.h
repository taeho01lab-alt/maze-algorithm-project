#pragma once

#include "maze.h"
#include <filesystem>
#include <string>

// S = (0,0); E = (height-1,width-1). A 1x1 maze displays only S.
std::string renderMaze(const Maze& maze);
void saveMaze(const Maze& maze, const std::filesystem::path& path);


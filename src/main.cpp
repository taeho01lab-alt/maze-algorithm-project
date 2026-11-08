#include "algorithms.h"
#include "io.h"

#include <charconv>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string_view>

namespace {
std::uint32_t parseNumber(std::string_view text) {
    std::uint32_t value = 0;
    const auto parsed = std::from_chars(text.data(), text.data() + text.size(), value);
    if (parsed.ec != std::errc{} || parsed.ptr != text.data() + text.size()) {
        throw std::invalid_argument("Expected an unsigned 32-bit integer");
    }
    return value;
}
int parseSize(std::string_view text) {
    const auto n = parseNumber(text);
    if (n == 0 || n > static_cast<std::uint32_t>(std::numeric_limits<int>::max())) {
        throw std::invalid_argument("Maze size must be a positive int");
    }
    return static_cast<int>(n);
}
}

int main(int argc, char** argv) {
    try {
        if (argc == 2 && std::string_view(argv[1]) == "--help") {
            std::cout << "Usage: maze_app <A|B|C> <width> <height> <seed> [output.txt]\n"
                         "Algorithms A/B/C are placeholders until the team implements them.\n";
            return 0;
        }
        if (argc != 5 && argc != 6) {
            throw std::invalid_argument("Usage: maze_app <A|B|C> <width> <height> <seed> [output.txt]");
        }
        const auto generate = generatorFor(argv[1]);
        const int width = parseSize(argv[2]);
        const int height = parseSize(argv[3]);
        const auto seed = parseNumber(argv[4]);
        const auto maze = generate(width, height, seed);
        if (argc == 6) saveMaze(maze, argv[5]);
        else std::cout << renderMaze(maze);
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}


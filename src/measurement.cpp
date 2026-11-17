#include "measurement.h"
#include "algorithms.h"
#include <chrono>
#include <utility>

MeasuredMaze generateAndAnalyze(std::string_view algorithm, int width, int height,
                               std::uint32_t seed) {
    const auto generate = generatorFor(algorithm);
    const auto begin = std::chrono::steady_clock::now();
    auto maze = generate(width, height, seed);
    const auto end = std::chrono::steady_clock::now();
    const double milliseconds = std::chrono::duration<double, std::milli>(end - begin).count();
    const auto metrics = analyzeMaze(maze);
    return {std::move(maze), metrics, milliseconds};
}

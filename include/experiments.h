#pragma once

#include "algorithms.h"
#include <filesystem>
#include <string>
#include <vector>

struct MazeSize { int width; int height; };

struct ExperimentConfig {
    std::vector<MazeSize> sizes;
    std::vector<std::uint32_t> seeds;
    std::vector<std::string> algorithms;
    int repeats = 1;
};

// Comparison-results owner: runner, generation timing, metrics and CSV.
void runExperiments(const ExperimentConfig& config,
                    const std::filesystem::path& csvPath);


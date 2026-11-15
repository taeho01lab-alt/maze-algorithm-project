#include "experiments.h"
#include <stdexcept>

void runExperiments(const ExperimentConfig& config, const std::filesystem::path& csvPath) {
    // Comparison-results owner: loop over sizes/seeds/algorithms/repeats;
    // time only generation, then validate, analyze and save raw CSV rows.
    (void)config; (void)csvPath;
    throw std::logic_error("Experiment runner is not implemented yet");
}


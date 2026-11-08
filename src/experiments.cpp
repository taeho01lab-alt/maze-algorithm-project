#include "experiments.h"
#include <stdexcept>

void runExperiments(const ExperimentConfig& config, const std::filesystem::path& csvPath) {
    // Member 3: loop over sizes/seeds/algorithms/repeats; save raw CSV rows.
    // Member 4: time only the agreed generation interval, then compute metrics.
    (void)config; (void)csvPath;
    throw std::logic_error("Experiment runner is not implemented yet");
}


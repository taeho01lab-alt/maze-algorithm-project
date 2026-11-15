#include "algorithms.h"
#include <stdexcept>

Generator generatorFor(std::string_view id) {
    if (id == "A" || id == "dfs") return generateA;
    if (id == "B" || id == "kruskal") return generateB;
    if (id == "C" || id == "sidewinder") return generateC;
    throw std::invalid_argument("Algorithm must be A/dfs, B/kruskal or C/sidewinder");
}


#include "algorithms.h"
#include <stdexcept>

Generator generatorFor(std::string_view id) {
    if (id == "A") return generateA;
    if (id == "B") return generateB;
    if (id == "C") return generateC;
    throw std::invalid_argument("Algorithm must be A, B or C");
}


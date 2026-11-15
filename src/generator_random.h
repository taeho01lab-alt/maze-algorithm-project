#pragma once

#include <cstddef>
#include <cstdint>
#include <random>
#include <stdexcept>

namespace maze_detail {
// Rejection sampling avoids modulo bias. Using mt19937 output directly also
// keeps seeded results independent of standard-library shuffle implementations.
inline std::size_t randomIndex(std::mt19937& random, std::size_t count) {
    if (count == 0) throw std::invalid_argument("Cannot choose from an empty range");
    const auto bound = static_cast<std::uint64_t>(count);
    const auto threshold = (std::uint64_t{0} - bound) % bound;
    std::uint64_t value;
    do {
        value = static_cast<std::uint64_t>(random()) << 32;
        value |= static_cast<std::uint64_t>(random());
    } while (value < threshold);
    return static_cast<std::size_t>(value % bound);
}
}

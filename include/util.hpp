#ifndef UTILS_HPP
#define UTILS_HPP

#include <random>

auto random_double() -> double {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

#endif
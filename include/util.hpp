#ifndef UTILS_HPP
#define UTILS_HPP

#include <random>
#include <numbers>

auto random_double() -> double {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

auto random_double(double min, double max) -> double {
    return min + (max-min)*random_double();
}

auto degrees_to_radians(double degrees) -> double {
    return degrees * std::numbers::pi / 180.0;
}

#endif
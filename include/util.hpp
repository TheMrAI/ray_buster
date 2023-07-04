#ifndef UTILS_HPP
#define UTILS_HPP

#include <numbers>
#include <random>

auto random_double() -> double
{
  static thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static thread_local std::mt19937 generator;
  return distribution(generator);
}


auto random_double(double min, double max) -> double { return min + (max - min) * random_double(); }

auto random_int(int min, int max) -> int { return static_cast<int>(random_double(min, max)); }

auto degrees_to_radians(double degrees) -> double { return degrees * std::numbers::pi / 180.0; }

#endif
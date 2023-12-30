#ifndef RAY_BUSTER_LIB_TRACE_UTIL_H_
#define RAY_BUSTER_LIB_TRACE_UTIL_H_

#include <random>
#include <cmath>
#include "lib/lina/vec3.h"

namespace trace {

auto randomUniformDouble(std::mt19937& generator, double min = 0.0, double max = 1.0) -> double {
    auto distribution = std::uniform_real_distribution(min, max);
    return distribution(generator);
}

auto randomUniformVec3(std::mt19937& generator, double min = 0.0, double max = 1.0) -> lina::Vec3 {
    auto distribution = std::uniform_real_distribution(min, max);
    return lina::Vec3{distribution(generator), distribution(generator), distribution(generator)};
}

// source: https://karthikkaranth.me/blog/generating-random-points-in-a-sphere/
// source2: Weisstein, Eric W. "Sphere Point Picking." From MathWorld--A Wolfram Web Resource. https://mathworld.wolfram.com/SpherePointPicking.html
auto randomOnUnitSphere(std::mt19937& generator) -> lina::Vec3 
{
  auto u = randomUniformDouble(generator, 0.0, 1.0);
  auto v = randomUniformDouble(generator, 0.0, 1.0);
  auto theta = u * 2.0 * std::numbers::pi;
  auto phi = std::acos(2.0 * v - 1.0);
  auto r = std::cbrt(randomUniformDouble(generator, 0.0, 1.0));
  auto sin_theta = std::sin(theta);
  auto cos_theta = std::cos(theta);
  auto sin_phi = std::sin(phi);
  auto cos_phi = std::cos(phi);
  auto x = r * sin_phi * cos_theta;
  auto y = r * sin_phi * sin_theta;
  auto z = r * cos_phi;
  return lina::Vec3{ x, y, z };
}

auto randomOnUnitHemisphere(std::mt19937& generator, lina::Vec3 const& normal) -> lina::Vec3 {
    auto onHemisphere = lina::Vec3{};
    auto dotProduct = 0.0;
    // For those rare cases, when the stars align and our random vector just happens
    // to be completely parallel with the normal. In those cases, we just reroll.
    do {
        onHemisphere = randomOnUnitSphere(generator);
        dotProduct = lina::dot(onHemisphere, normal);
    } while (dotProduct == 0.0); 
    if (dotProduct < 0.0) {
        return -onHemisphere;
    }
    return onHemisphere;
}

} //namespace trace

#endif
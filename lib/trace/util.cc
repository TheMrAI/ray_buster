#include "util.h"
#include "lib/lina/vec3.h"
#include <cmath>
#include <numbers>
#include <random>

namespace trace {

auto randomUniformDouble(std::mt19937& generator, double min, double max) -> double
{
  auto distribution = std::uniform_real_distribution(min, max);
  return distribution(generator);
}

auto randomUniformVec3(std::mt19937& generator, double min, double max) -> lina::Vec3
{
  auto distribution = std::uniform_real_distribution(min, max);
  return lina::Vec3{ distribution(generator), distribution(generator), distribution(generator) };
}

// source: https://karthikkaranth.me/blog/generating-random-points-in-a-sphere/
// source2: Weisstein, Eric W. "Sphere Point Picking." From MathWorld--A Wolfram Web Resource.
// https://mathworld.wolfram.com/SpherePointPicking.html
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

// Given a normal vector it will generate random vectors on a hemisphere whose axis aligns with the
// given normal.
auto randomOnUnitHemisphere(std::mt19937& generator, lina::Vec3 const& normal) -> lina::Vec3
{
  auto onHemisphere = lina::Vec3{};
  auto dotProduct = 0.0;
  // For those rare cases, when the stars align and our random vector just happens
  // to be completely parallel with the normal. In those cases, we just reroll.
  do {
    onHemisphere = randomOnUnitSphere(generator);
    dotProduct = lina::dot(onHemisphere, normal);
  } while (dotProduct == 0.0);
  if (dotProduct < 0.0) { return -onHemisphere; }
  return onHemisphere;
}

auto randomCosineDirection(std::mt19937& generator) -> lina::Vec3
{
  auto r1 = randomUniformDouble(generator, 0.0, 1.0);
  auto r2 = randomUniformDouble(generator, 0.0, 1.0);
  auto phi = 2.0 * std::numbers::pi * r1;
  auto x = std::cos(phi) * std::sqrt(r2);
  auto y = std::sin(phi) * std::sqrt(r2);
  auto z = std::sqrt(1.0 - r2);
  return lina::Vec3{ x, y, z };
}

// source: https://mathworld.wolfram.com/DiskPointPicking.html
auto randomOnUnitDisk(std::mt19937& generator) -> lina::Vec3
{
  auto r = randomUniformDouble(generator, 0.0, 1.0);
  auto theta = randomUniformDouble(generator, 0.0, 2.0 * std::numbers::pi);
  auto sqrtR = std::sqrt(r);
  return lina::Vec3{ sqrtR * std::cos(theta), sqrtR * std::sin(theta), 0.0 };
}

auto degreesToRadians(double degrees) -> double { return degrees * (std::numbers::pi / 180.0); }

Onb::Onb(lina::Vec3 const& direction)
{
  w_ = lina::unit(direction);
  auto a = (std::fabs(w_[0]) > 0.9) ? lina::Vec3{ 0.0, 1.0, 0.0 } : lina::Vec3{ 1.0, 0.0, 0.0 };
  v_ = lina::unit(lina::cross(w_, a));
  u_ = lina::cross(w_, v_);
}


auto Onb::Transform(lina::Vec3 const& vec) const -> lina::Vec3 { return vec[0] * u_ + vec[1] * v_ + vec[2] * w_; }

}// namespace trace
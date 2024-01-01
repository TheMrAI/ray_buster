#include "lina.h"
#include <array>
#include <cmath>
#include <span>

auto lina::add(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> std::array<double, 3>
{
  auto result = std::array<double, 3>{};
  for (std::size_t i = 0; i < lhs.size(); ++i) { result[i] = lhs[i] + rhs[i]; }
  return result;
}

auto lina::sub(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> std::array<double, 3>
{
  auto result = std::array<double, 3>{};
  for (std::size_t i = 0; i < lhs.size(); ++i) { result[i] = lhs[i] - rhs[i]; }
  return result;
}

auto lina::mul(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> std::array<double, 3>
{
  auto result = std::array<double, 3>{};
  for (std::size_t i = 0; i < lhs.size(); ++i) { result[i] = lhs[i] * rhs[i]; }
  return result;
}

auto lina::scale(double scalar, std::span<double const, 3> const vector) -> std::array<double, 3>
{
  auto result = std::array<double, 3>{};
  for (std::size_t i = 0; i < vector.size(); ++i) { result[i] = vector[i] * scalar; }
  return result;
}

auto lina::scale(std::span<double const, 3> const vector, double scalar) -> std::array<double, 3>
{
  return lina::scale(scalar, vector);
}

auto lina::cross(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> std::array<double, 3>
{
  // in the general case this could be defined using the determinant
  return std::array<double, 3>{
    lhs[1] * rhs[2] - lhs[2] * rhs[1], lhs[2] * rhs[0] - lhs[0] * rhs[2], lhs[0] * rhs[1] - lhs[1] * rhs[0]
  };
}

auto lina::dot(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> double
{
  auto result = 0.0;
  for (std::size_t i = 0; i < lhs.size(); ++i) { result += lhs[i] * rhs[i]; }
  return result;
}

auto lina::unit(std::span<double const, 3> const vector) -> std::array<double, 3>
{
  auto result = std::array<double, 3>{};
  auto length = lina::length(vector);
  for (std::size_t i = 0; i < vector.size(); i++) { result[i] = vector[i] / length; }
  return result;
}

auto lina::lengthSquared(std::span<double const, 3> const vector) -> double
{
  auto result = 0.0;
  for (auto val : vector) { result += val * val; }
  return result;
}

auto lina::length(std::span<double const, 3> const vector) -> double { return std::sqrt(lengthSquared(vector)); }

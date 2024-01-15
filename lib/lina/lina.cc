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

auto lina::dot(std::span<double const, 4> const lhs, std::span<double const, 4> const rhs) -> double
{
  auto result = 0.0;
  for (std::size_t i = 0; i < lhs.size(); ++i) { result += lhs[i] * rhs[i]; }
  return result;
}

auto lina::mul(std::span<double const, 16> lhs, std::span<double const, 16> rhs) -> std::array<double, 16>
{
  auto rowZero = std::span<double const, 4>(lhs.begin(), size_t{ 4 });
  auto rowOne = std::span<double const, 4>(std::next(lhs.begin(), 4), size_t{ 4 });
  auto rowTwo = std::span<double const, 4>(std::next(lhs.begin(), 8), size_t{ 4 });
  auto rowThree = std::span<double const, 4>(std::next(lhs.begin(), 12), size_t{ 4 });
  auto columnZero = std::array<double, 4>{ rhs[0], rhs[4], rhs[8], rhs[12] };
  auto columnOne = std::array<double, 4>{ rhs[1], rhs[5], rhs[9], rhs[13] };
  auto columnTwo = std::array<double, 4>{ rhs[2], rhs[6], rhs[10], rhs[14] };
  auto columnThree = std::array<double, 4>{ rhs[3], rhs[7], rhs[11], rhs[15] };

  auto result = std::array<double, 16>{
    dot(rowZero, columnZero),
    dot(rowZero, columnOne),
    dot(rowZero, columnTwo),
    dot(rowZero, columnThree),
    dot(rowOne, columnZero),
    dot(rowOne, columnOne),
    dot(rowOne, columnTwo),
    dot(rowOne, columnThree),
    dot(rowTwo, columnZero),
    dot(rowTwo, columnOne),
    dot(rowTwo, columnTwo),
    dot(rowTwo, columnThree),
    dot(rowThree, columnZero),
    dot(rowThree, columnOne),
    dot(rowThree, columnTwo),
    dot(rowThree, columnThree),
  };

  return result;
}

auto lina::mul(std::span<double const, 16> lhs, std::span<double const, 4> rhs) -> std::array<double, 4>
{
  auto rowZero = std::span<double const, 4>(lhs.begin(), size_t{ 4 });
  auto rowOne = std::span<double const, 4>(std::next(lhs.begin(), 4), size_t{ 4 });
  auto rowTwo = std::span<double const, 4>(std::next(lhs.begin(), 8), size_t{ 4 });
  auto rowThree = std::span<double const, 4>(std::next(lhs.begin(), 12), size_t{ 4 });

  auto result = std::array<double, 4>{ dot(rowZero, rhs), dot(rowOne, rhs), dot(rowTwo, rhs), dot(rowThree, rhs) };

  return result;
}
#include "transform.h"

#include "lib/lina/vec3.h"
#include <array>
#include <cmath>

namespace trace {

auto translate(lina::Vec3 deltas) -> std::array<double, 16>
{
  return std::array<double, 16>{
    1.0, 0.0, 0.0, deltas[0], 0.0, 1.0, 0.0, deltas[1], 0.0, 0.0, 1.0, deltas[2], 0.0, 0.0, 0.0, 1.0
  };
}

auto scale(lina::Vec3 scalar) -> std::array<double, 16>
{
  return std::array<double, 16>{
    scalar[0], 0.0, 0.0, 0.0, 0.0, scalar[1], 0.0, 0.0, 0.0, 0.0, scalar[2], 0.0, 0.0, 0.0, 0.0, 1.0
  };
}

auto rotateAlongX(double radians) -> std::array<double, 16>
{
  auto cosTheta = std::cos(radians);
  auto sinTheta = std::sin(radians);
  return std::array<double, 16>{
    1.0, 0.0, 0.0, 0.0, 0.0, cosTheta, sinTheta, 0.0, 0.0, -sinTheta, cosTheta, 0.0, 0.0, 0.0, 0.0, 1.0
  };
}

auto rotateAlongY(double radians) -> std::array<double, 16>
{
  auto cosTheta = std::cos(radians);
  auto sinTheta = std::sin(radians);
  return std::array<double, 16>{
    cosTheta, 0.0, -sinTheta, 0.0, 0.0, 1.0, 0.0, 0.0, sinTheta, 0.0, cosTheta, 0.0, 0.0, 0.0, 0.0, 1.0
  };
}

auto rotateAlongZ(double radians) -> std::array<double, 16>
{
  auto cosTheta = std::cos(radians);
  auto sinTheta = std::sin(radians);
  return std::array<double, 16>{
    cosTheta, -sinTheta, 0.0, 0.0, sinTheta, cosTheta, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0
  };
}

}// namespace trace
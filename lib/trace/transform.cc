#include "transform.h"

#include "lib/lina/vec3.h"
#include <array>
#include <cmath>

namespace trace {

auto unitMatrix() -> std::array<double, 16>
{
  return std::array<double, 16>{ 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 };
}

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

auto extend3Dto4D(lina::Vec3 vec, bool direction) -> std::array<double, 4>
{
  auto fourD = std::array<double, 4>{ vec[0], vec[1], vec[2], direction ? 0.0 : 1.0 };
  return fourD;
}

auto cut4Dto3D(std::array<double, 4> fourD) -> lina::Vec3 { return lina::Vec3{ fourD[0], fourD[1], fourD[2] }; }

}// namespace trace
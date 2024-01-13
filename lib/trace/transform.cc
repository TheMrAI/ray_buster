#include "transform.h"

#include "lib/lina/vec3.h"
#include <array>
#include <cmath>

namespace trace {

// auto translate(lina::Vec3 deltas) -> std::array<double, 16> {
//     return std::array<double, 16>{1.0, 0.0, 0.0, 0.0,
//      0.0, 1.0, 0.0, 0.0,
//       0.0, 0.0, 1.0, 0.0,
//        deltas[0], deltas[1], deltas[2], 1.0};
// }

auto rotateAlongX(double radians) -> std::array<double, 9>
{
  auto cosTheta = std::cos(radians);
  auto sinTheta = std::sin(radians);
  return std::array<double, 9>{
    1.0,
    0.0,
    0.0,
    0.0,
    cosTheta,
    -sinTheta,
    0.0,
    sinTheta,
    cosTheta,
  };
}

auto rotateAlongY(double radians) -> std::array<double, 9>
{
  auto cosTheta = std::cos(radians);
  auto sinTheta = std::sin(radians);
  return std::array<double, 9>{
    cosTheta,
    0.0,
    sinTheta,
    0.0,
    1.0,
    0.0,
    -sinTheta,
    0.0,
    cosTheta,
  };
}

auto rotateAlongZ(double radians) -> std::array<double, 9>
{
  auto cosTheta = std::cos(radians);
  auto sinTheta = std::sin(radians);
  return std::array<double, 9>{ cosTheta, -sinTheta, 0.0, sinTheta, cosTheta, 0.0, 0.0, 0.0, 1.0 };
}

}// namespace trace
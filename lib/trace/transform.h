#ifndef RAY_BUSTER_LIB_TRACE_TRANSFORM_H_
#define RAY_BUSTER_LIB_TRACE_TRANSFORM_H_

#include "lib/lina/vec3.h"
#include <array>

namespace trace {
// auto translate(lina::Vec3 deltas) -> std::array<double, 9>;
auto rotateAlongX(double radians) -> std::array<double, 9>;
auto rotateAlongY(double radians) -> std::array<double, 9>;
auto rotateAlongZ(double radians) -> std::array<double, 9>;

}// namespace trace

#endif
#ifndef RAY_BUSTER_LIB_TRACE_TRANSFORM_H_
#define RAY_BUSTER_LIB_TRACE_TRANSFORM_H_

#include "lib/lina/vec3.h"
#include <array>

namespace trace {

auto translate(lina::Vec3 deltas) -> std::array<double, 16>;
auto scale(lina::Vec3 scalar) -> std::array<double, 16>;
auto rotateAlongX(double radians) -> std::array<double, 16>;
auto rotateAlongY(double radians) -> std::array<double, 16>;
auto rotateAlongZ(double radians) -> std::array<double, 16>;

// Extend a 3D vector with a fourth. This fourth for direction vectors will be a 0.0
// and for positional vectors will be a 1.0.
// The reason behind this is to enable us to easily compose multiple linear transformations
// together in matrix form. Translation can only be described by a 4x4 matrix and by setting
// the last dimension for our vector to 1.0 we enable translation, with 0.0 we disable it.
// None of the other transformations are affected by this change.
auto extend3Dto4D(lina::Vec3 vec, bool direction) -> std::array<double, 4>;

// Cut back the 4D vector into a regular 3D vector, by simply dropping the last dimension.
// This is necessary, because 4D vectors have no cross product defined on them, and all they would
// do is just complicate the mental model of the application.
auto cut4Dto3D(std::array<double, 4> fourD) -> lina::Vec3;

}// namespace trace

#endif
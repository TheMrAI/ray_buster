#ifndef RAY_BUSTER_LIB_TRACE_UTIL_H_
#define RAY_BUSTER_LIB_TRACE_UTIL_H_

#include "lib/lina/vec3.h"
#include <random>

namespace trace {

[[nodiscard]] auto randomUniformDouble(std::mt19937& generator, double min = 0.0, double max = 1.0) -> double;
[[nodiscard]] auto randomUniformVec3(std::mt19937& generator, double min = 0.0, double max = 1.0) -> lina::Vec3;
[[nodiscard]] auto randomOnUnitSphere(std::mt19937& generator) -> lina::Vec3;
[[nodiscard]] auto randomOnUnitHemisphere(std::mt19937& generator, lina::Vec3 const& normal) -> lina::Vec3;

// The first two components of the returned vector hold the result.
[[nodiscard]] auto randomOnUnitDisk(std::mt19937& generator) -> lina::Vec3;

[[nodiscard]] auto degreesToRadians(double degrees) -> double;

}// namespace trace

#endif
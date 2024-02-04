#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_TRIANGLE_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_TRIANGLE_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"
#include <optional>
#include <span>
#include <utility>

namespace trace {

auto triangleStripCollide(Ray const& ray,
  lina::Vec3 const& center,
  std::span<lina::Vec3 const> triangleStrip) -> std::optional<std::pair<Collision, double>>;

auto triangleCollide(Ray const& ray,
  lina::Vec3 const& center,
  std::span<lina::Vec3 const, 3> triplet,
  bool swapUV) -> std::optional<std::pair<Collision, double>>;

}// namespace trace

#endif
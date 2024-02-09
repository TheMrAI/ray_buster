#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_TRIANGLE_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_TRIANGLE_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"

#include <array>
#include <cstddef>
#include <optional>
#include <span>
#include <utility>
#include <vector>

namespace trace {

struct MeshCollision
{
  Collision collision;
  std::size_t vertexId;
  double distance;
};

auto triangleCollide(Ray const& ray,
  lina::Vec3 const& center,
  std::vector<lina::Vec3> const& vertices,
  std::vector<std::array<std::size_t, 3>> const& triangles,
  std::size_t triangleId) -> std::optional<MeshCollision>;

auto meshCollide(Ray const& ray,
  lina::Vec3 const& center,
  std::vector<lina::Vec3> const& vertices,
  std::vector<std::array<std::size_t, 3>> const& triangles) -> std::optional<MeshCollision>;

}// namespace trace

#endif
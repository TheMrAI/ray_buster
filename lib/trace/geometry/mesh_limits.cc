#include "lib/trace/geometry/mesh_limits.h"

#include "lib/lina/vec3.h"
#include "lib/trace/geometry/mesh.h"
#include "lib/trace/geometry/triangle_data.h"

#include <algorithm>
#include <array>
#include <limits>

namespace trace {

auto collide(MeshLimits const& lhs, MeshLimits const& rhs) -> bool
{
  auto overlapX = (lhs.minX <= rhs.minX && rhs.minX <= lhs.maxX) || (lhs.minX <= rhs.maxX && rhs.maxX <= lhs.maxX);
  auto containX = (lhs.minX < rhs.minX && lhs.maxX > rhs.maxX) || (rhs.minX < lhs.minX && rhs.maxX > lhs.maxX);
  if (!(overlapX || containX)) { return false; }
  auto overlapY = (lhs.minY <= rhs.minY && rhs.minY <= lhs.maxY) || (lhs.minY <= rhs.maxY && rhs.maxY <= lhs.maxY);
  auto containY = (lhs.minY < rhs.minY && lhs.maxY > rhs.maxY) || (rhs.minY < lhs.minY && rhs.maxY > lhs.maxY);
  if (!((overlapY || containY))) { return false; }
  auto overlapZ = (lhs.minZ <= rhs.minZ && rhs.minZ <= lhs.maxZ) || (lhs.minZ <= rhs.maxZ && rhs.maxZ <= lhs.maxZ);
  auto containZ = (lhs.minZ < rhs.minZ && lhs.maxZ > rhs.maxZ) || (rhs.minZ < lhs.minZ && rhs.maxZ > lhs.maxZ);
  // NOLINTBEGIN(readability-simplify-boolean-expr)
  if (!((overlapZ || containZ))) { return false; }

  // Here it is easier to see the pattern of the checks, then merging the last check into the return statement
  // as clang-tidy would suggest.
  return true;
  // NOLINTEND(readability-simplify-boolean-expr)
}

auto meshLimits(trace::Mesh const& mesh) -> MeshLimits
{
  auto limits = MeshLimits{ std::numeric_limits<double>::max(),
    std::numeric_limits<double>::min(),
    std::numeric_limits<double>::max(),
    std::numeric_limits<double>::min(),
    std::numeric_limits<double>::max(),
    std::numeric_limits<double>::min() };

  for (auto const& vertex : mesh.vertices) {
    limits.minX = std::min(limits.minX, vertex[0]);
    limits.maxX = std::max(limits.maxX, vertex[0]);
    limits.minY = std::min(limits.minY, vertex[1]);
    limits.maxY = std::max(limits.maxY, vertex[1]);
    limits.minZ = std::min(limits.minZ, vertex[2]);
    limits.maxZ = std::max(limits.maxZ, vertex[2]);
  }

  return limits;
}

auto triangleLimits(TriangleData const& triangleData) -> MeshLimits
{
  auto limits = MeshLimits{ std::numeric_limits<double>::max(),
    std::numeric_limits<double>::min(),
    std::numeric_limits<double>::max(),
    std::numeric_limits<double>::min(),
    std::numeric_limits<double>::max(),
    std::numeric_limits<double>::min() };

  auto const vertices =
    std::array<lina::Vec3, 3>{ triangleData.Q, triangleData.Q + triangleData.u, triangleData.Q + triangleData.v };

  for (auto const& vertex : vertices) {
    limits.minX = std::min(limits.minX, vertex[0]);
    limits.maxX = std::max(limits.maxX, vertex[0]);
    limits.minY = std::min(limits.minY, vertex[1]);
    limits.maxY = std::max(limits.maxY, vertex[1]);
    limits.minZ = std::min(limits.minZ, vertex[2]);
    limits.maxZ = std::max(limits.maxZ, vertex[2]);
  }

  return limits;
}

}// namespace trace
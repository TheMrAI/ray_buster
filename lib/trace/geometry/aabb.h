#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_AABB_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_AABB_H_

#include "lib/trace/geometry/mesh.h"
#include "lib/trace/geometry/triangle_data.h"

#include <limits>

namespace trace {

// These are basically the dimensions of the axis aligned bounding box
// for any given mesh.
struct Aabb
{
  double minX = std::numeric_limits<double>::max();
  double maxX = std::numeric_limits<double>::min();
  double minY = std::numeric_limits<double>::max();
  double maxY = std::numeric_limits<double>::min();
  double minZ = std::numeric_limits<double>::max();
  double maxZ = std::numeric_limits<double>::min();
};

auto mergeAABB(Aabb const& lhs, Aabb const& rhs) -> Aabb;

auto collide(Aabb const& lhs, Aabb const& rhs) -> bool;

auto meshAabb(trace::Mesh const& mesh) -> Aabb;

auto triangleAabb(TriangleData const& triangleData) -> Aabb;

}// namespace trace

#endif
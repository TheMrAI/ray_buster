#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_AABB_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_AABB_H_

#include "lib/trace/geometry/mesh.h"
#include "lib/trace/geometry/triangle_data.h"

namespace trace {

// These are basically the dimensions of the axis aligned bounding box
// for any given mesh.
struct Aabb
{
  double minX;
  double maxX;
  double minY;
  double maxY;
  double minZ;
  double maxZ;
};

auto collide(Aabb const& lhs, Aabb const& rhs) -> bool;

auto meshAabb(trace::Mesh const& mesh) -> Aabb;

auto triangleAabb(TriangleData const& triangleData) -> Aabb;

}// namespace trace

#endif
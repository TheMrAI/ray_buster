#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_MESH_LIMITS_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_MESH_LIMITS_H_

#include "lib/trace/geometry/mesh.h"
#include "lib/trace/geometry/triangle_data.h"

namespace trace {

// These are basically the dimensions of the axis aligned bounding box
// for any given mesh.
struct MeshLimits
{
  double minX;
  double maxX;
  double minY;
  double maxY;
  double minZ;
  double maxZ;
};

auto collide(MeshLimits const& lhs, MeshLimits const& rhs) -> bool;

auto meshLimits(trace::Mesh const& mesh) -> MeshLimits;

auto triangleLimits(TriangleData const& triangleData) -> MeshLimits;

}// namespace trace

#endif
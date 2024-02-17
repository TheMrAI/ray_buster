#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_MESH_LIMITS_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_MESH_LIMITS_H_

#include "lib/trace/geometry/mesh.h"

namespace trace {

struct MeshLimits
{
  double minX;
  double maxX;
  double minY;
  double maxY;
  double minZ;
  double maxZ;
};

auto meshLimits(trace::Mesh const& mesh) -> MeshLimits;

}// namespace trace

#endif
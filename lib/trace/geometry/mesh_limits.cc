#include "lib/trace/geometry/mesh_limits.h"

#include "lib/trace/geometry/mesh.h"

#include <algorithm>
#include <limits>

namespace trace {

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

}// namespace trace
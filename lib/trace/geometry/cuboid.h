#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_CUBOID_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_CUBOID_H_

#include "lib/lina/vec3.h"
#include "lib/trace/geometry/component.h"

namespace trace {

class Cuboid : public Component
{
public:
  Cuboid(Cuboid const&) = default;
  Cuboid(Cuboid&&) = default;
  auto operator=(Cuboid const&) -> Cuboid& = default;
  auto operator=(Cuboid&&) -> Cuboid& = default;
  ~Cuboid() override = default;

  friend auto buildCuboid(lina::Vec3 center, double width, double depth, double height) -> Cuboid;

private:
  Cuboid();// NOLINT
};

// Build a cuboid at origin, then scale and move it to the target position. By default the constructed cuboid will be 2
// unit wide in every dimension. The reason is that, this is how it can be conveniently scaled in all bounding box
// applications, for all objects that follow the pattern of having a center and offset vectors from that defining all
// vertex coordinates. In these setups a cuboid with 2 unit dimension lengths means that from that from the center the
// sides of the cuboid (the shortest distance) are exactly 1 unit away.
[[nodiscard]] auto buildCuboid(lina::Vec3 center = lina::Vec3{ 0.0, 0.0, 0.0 },
  double width = 2.0,
  double depth = 2.0,
  double height = 2.0) -> Cuboid;


}// namespace trace

#endif
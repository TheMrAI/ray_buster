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

  friend auto buildCuboid(lina::Vec3 center, double width, double height, double depth) -> Cuboid;

private:
  Cuboid();// NOLINT
};

[[nodiscard]] auto buildCuboid(lina::Vec3 center = lina::Vec3{ 0.0, 0.0, 0.0 },
  double width = 1.0,
  double height = 1.0,
  double depth = 1.0) -> Cuboid;


}// namespace trace

#endif
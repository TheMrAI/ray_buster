#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_PLANE_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_PLANE_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"
#include <optional>

namespace trace {

// A simple Plane, which is always created parallel to the x, z axises, and the normal being on the
// y axis pointing in its positive direction. The reason behind these constraints is that for the time
// being I see no other practical ways of defining a plane.
// This way the initialization is simple and we can still have all possible plane orientations by allowing
// its transformation. A feature not yet implemented.
class Plane : public Component
{
public:
  Plane(lina::Vec3 center = lina::Vec3{ 0.0, 0.0, 0.0 }, double width = 0.0, double depth = 0.0);

  auto Collide(Ray const& ray) -> std::optional<Collision> override;

private:
  lina::Vec3 center_;
  lina::Vec3 normal_;
  lina::Vec3 u_;
  lina::Vec3 v_;
  double D_;
  lina::Vec3 Q_;
  lina::Vec3 Qu_;
  lina::Vec3 Qv_;
};

}// namespace trace

#endif
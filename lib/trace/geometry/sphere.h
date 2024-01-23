#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_SPHERE_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_SPHERE_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/component.h"
#include "lib/trace/ray.h"
#include <optional>
#include <span>

namespace trace {

class Sphere : public Component
{
public:
  Sphere();
  Sphere(lina::Vec3 center, double radius);

  [[nodiscard]] auto Collide(Ray const& ray) const -> std::optional<Collision> override;
  auto Transform(std::span<double const, 16> transformationMatrix) -> void override;

private:
  lina::Vec3 center_;
  double radius_;
};

}// namespace trace

#endif
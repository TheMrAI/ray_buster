#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_SPHERE_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_SPHERE_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/component.h"
#include "lib/trace/pdf.h"
#include "lib/trace/ray.h"

#include <array>
#include <cstddef>
#include <optional>
#include <random>
#include <span>
#include <vector>

namespace trace {

class Sphere : public Component
{
public:
  Sphere();
  Sphere(Sphere const&) = default;
  Sphere(Sphere&&) = default;
  auto operator=(Sphere const&) -> Sphere& = default;
  auto operator=(Sphere&&) -> Sphere& = default;
  virtual ~Sphere() = default;

  friend auto buildSphere(lina::Vec3 center, double radius, std::size_t subdivisionLevel) -> Sphere;
};

// Build a unit sphere at origo then move it to the target position.
auto buildSphere(lina::Vec3 center = lina::Vec3{ 0.0, 0.0, 0.0 },
  double radius = 1.0,
  std::size_t subdivisionLevel = 2) -> Sphere;

}// namespace trace

#endif
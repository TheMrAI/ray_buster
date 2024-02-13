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

// A sphere, made by subdividing an icosahedron.
// It may be tempting to apply Gouraud shading to the sides, to make the render
// smoother without having to increase the mesh count, but this would be folly.
// Gouraud shading adjusts the normal vectors on each face and it will work reasonable
// well, for Lambertian materials, but for metallic or dielectric it will behave very badly.
// Adjusting the normal without having a mesh behind it, leads to a number of nasty calculation
// errors, which only reduce in quantity if the mesh is more subdivided. This isn't helpful, because
// then we can just increase the mesh resolution and not have to worry about any calculation issues.
// Checked the behaviour in Blender, and there I couldn't apply any smoothing to the Icosphere mesh during
// rendering. It simply ignores it, so for now, with my limited knowledge I will assume no normal foolery
// should be done for Icospheres.
class Icosphere : public Component
{
public:
  Icosphere(Icosphere const&) = default;
  Icosphere(Icosphere&&) = default;
  auto operator=(Icosphere const&) -> Icosphere& = default;
  auto operator=(Icosphere&&) -> Icosphere& = default;
  virtual ~Icosphere() = default;

  friend auto buildIcosphere(lina::Vec3 center, double radius, std::size_t subdivisionLevel) -> Icosphere;

protected:
  auto updateTriangleData() -> void override;

private:
  Icosphere();
};

// Build a unit sphere at origo then move it to the target position.
[[nodiscard]] auto buildIcosphere(lina::Vec3 center = lina::Vec3{ 0.0, 0.0, 0.0 },
  double radius = 1.0,
  std::size_t subdivisionLevel = 2) -> Icosphere;

}// namespace trace

#endif
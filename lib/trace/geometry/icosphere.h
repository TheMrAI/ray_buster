#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_ICOSPHERE_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_ICOSPHERE_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/component.h"
#include "lib/trace/geometry/cuboid.h"
#include "lib/trace/ray.h"

#include <cstddef>
#include <optional>
#include <span>

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
  ~Icosphere() override = default;

  [[nodiscard]] auto Collide(Ray const& ray) const -> std::optional<Collision> override;
  // Apply the linear transformation matrix to the object.
  auto Transform(std::span<double const, 16> transformationMatrix) -> void override;

  [[nodiscard]] auto GetBoundingBox() const -> Cuboid const&;

  friend auto buildIcosphere(lina::Vec3 center, double diameter, std::size_t subdivisionLevel) -> Icosphere;

protected:
  auto updateTriangleData() -> void override;

private:
  Icosphere();

  Cuboid boundingBox_;
};

// Build an icosphere at origo, scale and move it to the target position.
// By default the constructed icosphere will have diameter of 2. This is because
// an icosphere built in such a manner will have a radius of exactly one, which can
// easily be scaled in any direction.
[[nodiscard]] auto buildIcosphere(lina::Vec3 center = lina::Vec3{ 0.0, 0.0, 0.0 },
  double diameter = 2.0,
  std::size_t subdivisionLevel = 2) -> Icosphere;

}// namespace trace

#endif
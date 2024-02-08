#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_SPHERE_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_SPHERE_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/component.h"
#include "lib/trace/pdf.h"
#include "lib/trace/ray.h"

#include <array>
#include <optional>
#include <random>
#include <span>
#include <vector>

namespace trace {

class Sphere : public Component
{
public:
  Sphere();

  [[nodiscard]] auto Collide(Ray const& ray) const -> std::optional<Collision> override;
  auto Transform(std::span<double const, 16> transformationMatrix) -> void override;
  [[nodiscard]] auto SamplingPDF(std::mt19937& randomGenerator, lina::Vec3 const& from) const -> PDF override;

  friend auto buildSphere(lina::Vec3 center, double radius, std::size_t subdivisionLevel) -> Sphere;

private:
  lina::Vec3 center_;
  std::vector<lina::Vec3> vertices_;
  std::vector<std::array<std::size_t, 3>> triangles_;
};

// Build a unit sphere at origo then move it to the target position.
auto buildSphere(lina::Vec3 center = lina::Vec3{ 0.0, 0.0, 0.0 },
  double radius = 1.0,
  std::size_t subdivisionLevel = 2) -> Sphere;

}// namespace trace

#endif
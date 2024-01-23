#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_CUBOID_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_CUBOID_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/component.h"
#include "lib/trace/ray.h"
#include <array>
#include <optional>
#include <span>
#include <utility>

namespace trace {

class Cuboid : public Component
{
public:
  Cuboid(lina::Vec3 center = lina::Vec3{ 0.0, 0.0, 0.0 }, double width = 1.0, double height = 1.0, double depth = 1.0);

  auto Collide(Ray const& ray) const -> std::optional<Collision> override;
  auto Transform(std::span<double const, 16> transformationMatrix) -> void override;

private:
  auto triangleCollision(Ray const& ray, std::span<lina::Vec3 const, 3> triplet, bool swapUV) const
    -> std::optional<std::pair<Collision, double>>;

private:
  lina::Vec3 center_;
  std::array<lina::Vec3, 8> triangleStrip1_;
  std::array<lina::Vec3, 8> triangleStrip2_;
};

}// namespace trace

#endif
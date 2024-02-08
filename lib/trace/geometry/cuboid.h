#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_CUBOID_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_CUBOID_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/component.h"
#include "lib/trace/pdf.h"
#include "lib/trace/ray.h"

#include <array>
#include <cstddef>
#include <optional>
#include <random>
#include <span>
#include <utility>
#include <vector>


namespace trace {

class Cuboid : public Component
{
public:
  Cuboid(lina::Vec3 center = lina::Vec3{ 0.0, 0.0, 0.0 }, double width = 1.0, double height = 1.0, double depth = 1.0);

  [[nodiscard]] auto Collide(Ray const& ray) const -> std::optional<Collision> override;
  auto Transform(std::span<double const, 16> transformationMatrix) -> void override;
  [[nodiscard]] auto SamplingPDF(std::mt19937& randomGenerator, lina::Vec3 const& from) const -> PDF override;

private:
  lina::Vec3 center_;
  std::vector<lina::Vec3> vertices_;
  std::vector<std::array<std::size_t, 3>> triangles_;
};

}// namespace trace

#endif
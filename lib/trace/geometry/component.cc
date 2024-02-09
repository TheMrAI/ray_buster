#include "lib/trace/geometry/component.h"

#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/triangle.h"
#include "lib/trace/pdf.h"
#include "lib/trace/ray.h"
#include "lib/trace/transform.h"

#include <array>
#include <cstddef>
#include <optional>
#include <random>
#include <span>
#include <utility>
#include <vector>

namespace trace {

Component::Component(lina::Vec3 center,
  std::vector<lina::Vec3> vertices,
  std::vector<std::array<std::size_t, 3>> triangles)
  : center_{ center }, vertices_{ std::move(vertices) }, triangles_{ std::move(triangles) }
{}

auto Component::Collide(Ray const& ray) const -> std::optional<Collision>
{
  auto closestCollisionData = meshCollide(ray, center_, vertices_, triangles_);
  if (!closestCollisionData) { return std::optional<Collision>{}; }
  return std::optional<Collision>{ closestCollisionData->collision };
}

// Apply the linear transformation matrix to the object.
auto Component::Transform(std::span<double const, 16> transformationMatrix) -> void
{
  auto center4 = trace::extend3Dto4D(center_, false);
  center_ = trace::cut4Dto3D(lina::mul(transformationMatrix, center4));

  for (auto& vertex : vertices_) {
    auto vertex4 = trace::extend3Dto4D(vertex, true);
    vertex = trace::cut4Dto3D(lina::mul(transformationMatrix, vertex4));
  }
}

auto Component::SamplingPDF(std::mt19937& /*randomGenerator*/, lina::Vec3 const& /*from*/) const -> PDF
{
  return PDF{};
}

}// namespace trace
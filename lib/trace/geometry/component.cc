#include "lib/trace/geometry/component.h"

#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/mesh.h"
#include "lib/trace/geometry/triangle_data.h"
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

Component::Component(Mesh mesh) : mesh_{ std::move(mesh) } {}

auto Component::Collide(Ray const& ray) const -> std::optional<Collision>
{
  auto closestCollisionData = meshCollide(ray, mesh_.triangles, mesh_.triangleData);
  if (!closestCollisionData) { return std::optional<Collision>{}; }
  return std::optional<Collision>{ closestCollisionData->collision };
}

// Apply the linear transformation matrix to the object.
auto Component::Transform(std::span<double const, 16> transformationMatrix) -> void
{
  auto center4 = trace::extend3Dto4D(mesh_.center, false);
  mesh_.center = trace::cut4Dto3D(lina::mul(transformationMatrix, center4));

  for (auto& vertex : mesh_.vertices) {
    auto vertex4 = trace::extend3Dto4D(vertex, true);
    vertex = trace::cut4Dto3D(lina::mul(transformationMatrix, vertex4));
  }

  updateTriangleData();
}

auto Component::SamplingPDF(std::mt19937& /*randomGenerator*/, lina::Vec3 const& /*from*/) const -> PDF
{
  return PDF{};
}

auto Component::GetMesh() const -> Mesh const& { return mesh_; }

auto Component::updateTriangleData() -> void
{
  for (auto triangleId = std::size_t{ 0 }; triangleId < mesh_.triangles.size(); ++triangleId) {
    auto& triangleData = mesh_.triangleData[triangleId];

    auto const& triangle = mesh_.triangles[triangleId];
    auto const localizedTriangle = std::array<lina::Vec3, 3>{ mesh_.vertices[triangle[0]] + mesh_.center,
      mesh_.vertices[triangle[1]] + mesh_.center,
      mesh_.vertices[triangle[2]] + mesh_.center };

    triangleData.Q = localizedTriangle[0];
    triangleData.u = localizedTriangle[2] - localizedTriangle[0];
    triangleData.v = localizedTriangle[1] - localizedTriangle[0];

    // lina::cross(u, v) is positive if u is to the right of v in space
    // PS.: Every time I see this code I have to double check.
    triangleData.n = lina::cross(triangleData.u, triangleData.v);
    triangleData.normal = lina::unit(triangleData.n);
    triangleData.D = lina::dot(triangleData.normal, localizedTriangle[0]);
    triangleData.common = triangleData.n / lina::dot(triangleData.n, triangleData.n);
  }
}

}// namespace trace
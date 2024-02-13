#include "lib/trace/geometry/component.h"

#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/triangle.h"
#include "lib/trace/geometry/triangle_data.h"
#include "lib/trace/geometry/vertex_data.h"
#include "lib/trace/pdf.h"
#include "lib/trace/ray.h"
#include "lib/trace/transform.h"

#include <array>
#include <cassert>
#include <cstddef>
#include <optional>
#include <random>
#include <span>
#include <utility>
#include <vector>

namespace trace {

Component::Component(lina::Vec3 center,
  std::vector<lina::Vec3> vertices,
  std::vector<VertexData> vertexData,
  std::vector<std::array<std::size_t, 3>> triangles,
  std::vector<TriangleData> triangleData)
  : center_{ center }, vertices_{ std::move(vertices) }, verticesData_{ std::move(vertexData) },
    triangles_{ std::move(triangles) }, trianglesData_{ std::move(triangleData) }
{}

auto Component::Collide(Ray const& ray) const -> std::optional<Collision>
{
  auto closestCollisionData = meshCollide(ray, triangles_, trianglesData_);
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

  updateTriangleData();
}

auto Component::SamplingPDF(std::mt19937& /*randomGenerator*/, lina::Vec3 const& /*from*/) const -> PDF
{
  return PDF{};
}

auto Component::updateTriangleData() -> void
{
  for (auto triangleId = std::size_t{ 0 }; triangleId < triangles_.size(); ++triangleId) {
    auto& triangleData = trianglesData_[triangleId];

    auto const& triangle = triangles_[triangleId];
    auto const localizedTriangle = std::array<lina::Vec3, 3>{
      vertices_[triangle[0]] + center_, vertices_[triangle[1]] + center_, vertices_[triangle[2]] + center_
    };

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
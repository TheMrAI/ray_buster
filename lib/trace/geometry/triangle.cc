#include "lib/trace/geometry/triangle.h"

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"

#include <array>
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

namespace trace {

auto triangleCollide(Ray const& ray,
  lina::Vec3 const& center,
  std::vector<lina::Vec3> const& vertices,
  std::vector<std::array<std::size_t, 3>> const& triangles,
  std::size_t triangleId) -> std::optional<MeshCollision>
{
  auto const& triangle = triangles[triangleId];
  auto const localizedTriangle = std::array<lina::Vec3, 3>{
    vertices[triangle[0]] + center, vertices[triangle[1]] + center, vertices[triangle[2]] + center
  };
  auto const u = localizedTriangle[2] - localizedTriangle[0];
  auto const v = localizedTriangle[1] - localizedTriangle[0];

  // lina::cross(u, v) is positive if u is to the right of u
  // PS.: Every time I see this code I have to double check.
  auto const n = lina::cross(u, v);
  auto const normal = lina::unit(n);
  auto const D = lina::dot(normal, localizedTriangle[0]);

  auto const denominator = lina::dot(normal, ray.Direction());
  if (denominator == 0.0) { return std::optional<MeshCollision>{}; }

  auto const t = (D - lina::dot(normal, ray.Source())) / denominator;
  if (t <= 0.0) { return std::optional<MeshCollision>{}; }
  auto const collisionPoint = ray.Source() + ray.Direction() * t;

  auto const planeDelta = collisionPoint - localizedTriangle[0];
  auto const common = n / lina::dot(n, n);
  auto const alpha = lina::dot(common, lina::cross(planeDelta, v));
  auto const beta = lina::dot(common, lina::cross(u, planeDelta));

  auto const scalarSum = alpha + beta;
  if (0.0 > alpha || alpha > 1.0 || 0.0 > beta || beta > 1.0 || scalarSum > 1.0) {
    return std::optional<MeshCollision>{};
  }

  auto collision = Collision{};
  collision.point = collisionPoint;
  collision.normal = normal;
  collision.frontFace = lina::dot(normal, ray.Direction()) < 0.0;

  return MeshCollision(collision, triangleId, t);
}

// Just copy-pasting these here so that we can repurpose them here quickly,
// then we can migrate the solution back into the other geometries.
auto meshCollide(Ray const& ray,
  lina::Vec3 const& center,
  std::vector<lina::Vec3> const& vertices,
  std::vector<std::array<std::size_t, 3>> const& triangles) -> std::optional<MeshCollision>
{
  auto closestCollisionData = std::optional<MeshCollision>{};
  for (auto triangleId = std::size_t{ 0 }; triangleId < triangles.size(); ++triangleId) {
    auto collisionCandidateData = triangleCollide(ray, center, vertices, triangles, triangleId);
    if (collisionCandidateData) {
      if (!closestCollisionData || closestCollisionData->distance > collisionCandidateData->distance) {
        std::swap(closestCollisionData, collisionCandidateData);
      }
    }
  }
  return closestCollisionData;
}

}// namespace trace
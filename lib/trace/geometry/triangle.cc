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
  auto localizedTriangle = std::array<lina::Vec3, 3>{
    vertices[triangle[0]] + center, vertices[triangle[1]] + center, vertices[triangle[2]] + center
  };
  auto u = localizedTriangle[2] - localizedTriangle[0];
  auto v = localizedTriangle[1] - localizedTriangle[0];

  // lina::cross(u, v) is positive if u is to the right of u
  // PS.: Every time I see this code I have to double check.
  auto normal = lina::unit(lina::cross(u, v));
  auto D = lina::dot(normal, localizedTriangle[0]);

  auto denominator = lina::dot(normal, ray.Direction());
  if (denominator == 0.0) { return std::optional<MeshCollision>{}; }

  auto t = (D - lina::dot(normal, ray.Source())) / denominator;
  if (t <= 0.0) { return std::optional<MeshCollision>{}; }
  auto collisionPoint = ray.Source() + ray.Direction() * t;

  auto planeDelta = collisionPoint - localizedTriangle[0];
  auto common = normal / (lina::dot(normal, lina::cross(u, v)));
  auto alpha = lina::dot(common, lina::cross(planeDelta, v));
  auto beta = lina::dot(common, lina::cross(u, planeDelta));

  auto scalarSum = alpha + beta;
  if (0.0 > alpha || alpha > 1.0 || 0.0 > beta || beta > 1.0 || scalarSum > 1.0) {
    return std::optional<MeshCollision>{};
  }

  auto collision = Collision{};
  std::swap(collision.point, collisionPoint);
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
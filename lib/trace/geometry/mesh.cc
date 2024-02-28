#include "lib/trace/geometry/mesh.h"

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/triangle_data.h"
#include "lib/trace/ray.h"

#include <array>
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

namespace trace {

auto triangleCollide(Ray const& ray,
  std::vector<TriangleData> const& trianglesData,
  std::size_t triangleId) -> std::optional<MeshCollision>
{
  auto const& triangleData = trianglesData[triangleId];

  auto const denominator = lina::dot(triangleData.normal, ray.Direction());
  if (denominator == 0.0) { return std::optional<MeshCollision>{}; }

  auto const t = (triangleData.D - lina::dot(triangleData.normal, ray.Source())) / denominator;
  if (t <= 0.0) { return std::optional<MeshCollision>{}; }
  auto const collisionPoint = ray.Source() + ray.Direction() * t;

  auto const planeDelta = collisionPoint - triangleData.Q;
  auto const alpha = lina::dot(triangleData.common, lina::cross(planeDelta, triangleData.v));
  auto const beta = lina::dot(triangleData.common, lina::cross(triangleData.u, planeDelta));

  auto const scalarSum = alpha + beta;
  if (0.0 > alpha || alpha > 1.0 || 0.0 > beta || beta > 1.0 || scalarSum > 1.0) {
    return std::optional<MeshCollision>{};
  }

  auto collision = Collision{};
  collision.point = collisionPoint;
  collision.normal = triangleData.normal;
  collision.frontFace = lina::dot(triangleData.normal, ray.Direction()) < 0.0;

  return MeshCollision(collision, triangleId, t, alpha, beta, 1.0 - alpha - beta);
}

// Just copy-pasting these here so that we can repurpose them here quickly,
// then we can migrate the solution back into the other geometries.
auto meshCollide(Ray const& ray,
  std::vector<std::array<std::size_t, 3>> const& triangles,
  std::vector<TriangleData> const& trianglesData) -> std::optional<MeshCollision>
{
  auto closestCollisionData = std::optional<MeshCollision>{};
  for (auto triangleId = std::size_t{ 0 }; triangleId < triangles.size(); ++triangleId) {
    auto collisionCandidateData = triangleCollide(ray, trianglesData, triangleId);
    if (collisionCandidateData) {
      if (!closestCollisionData || closestCollisionData->distance > collisionCandidateData->distance) {
        std::swap(closestCollisionData, collisionCandidateData);
      }
    }
  }
  return closestCollisionData;
}

}// namespace trace
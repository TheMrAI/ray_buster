#include "lib/trace/geometry/mesh.h"

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/aabb.h"
#include "lib/trace/geometry/triangle_data.h"
#include "lib/trace/ray.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

namespace trace {

auto triangleCollide(Ray const& ray, std::vector<TriangleData> const& trianglesData, std::size_t triangleId)
  -> std::optional<MeshCollision>
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

auto triangleVoxelCollide(lina::Vec3 voxelCenter, double const voxelDimension, TriangleData triangleData) -> bool
{
  auto const delta = -voxelCenter;
  // translate the triangle
  triangleData.Q += delta;
  return triangleVoxelCollisionTest(voxelDimension, triangleData);
}

// We are using the algorithm described here:
// https://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/code/tribox_tam.pdf Except that we do not check whether
// the plane of the triangle overlaps with the AABB of the voxel.
auto triangleVoxelCollisionTest(double const voxelDimension, TriangleData const& triangleData) -> bool
{
  auto halfVoxelDimension = voxelDimension / 2.0;
  auto voxelAabb = Aabb{ -halfVoxelDimension,
    halfVoxelDimension,
    -halfVoxelDimension,
    halfVoxelDimension,
    -halfVoxelDimension,
    halfVoxelDimension };

  auto triangleBB = triangleAabb(triangleData);

  // Test set 1
  // voxel and triangle AABB collide
  if (!collide(voxelAabb, triangleBB)) { return false; }

  // Test set 2
  // Based on: https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
  // auto const projectedVoxelRadius = lina::dot(lina::Vec3{0.5, 0.5, 0.5}, triangleData.normal);
  auto const projectedVoxelRadius = (halfVoxelDimension * std::abs(triangleData.normal[0]))
                                    + (halfVoxelDimension * std::abs(triangleData.normal[1]))
                                    + (halfVoxelDimension * std::abs(triangleData.normal[2]));
  // Here we make an optimization. Originally it should be:
  // auto const planeD = lina::dot(triangleData.normal, triangleData.Q);
  // auto const voxelCenterDistanceOfPlane = lina::dot(triangleData.normal, lina::Vec3{0.0, 0.0, 0.0}) - planeD;
  // Since lina::dot(...) will become zero, we omit it's calculation.
  auto const voxelCenterDistanceFromPlane = -lina::dot(triangleData.normal, triangleData.Q);
  if (!(std::abs(voxelCenterDistanceFromPlane) <= projectedVoxelRadius)) { return false; }

  // Test set 3
  // Test using SAT (separating axis theorem)
  auto const voxelAxis =
    std::array<lina::Vec3, 3>{ lina::Vec3{ 1.0, 0.0, 0.0 }, lina::Vec3{ 0.0, 1.0, 0.0 }, lina::Vec3{ 0.0, 0.0, 1.0 } };
  auto const v1 = triangleData.Q + triangleData.u;
  auto const v2 = triangleData.Q + triangleData.v;
  auto const triangleEdges = std::array<lina::Vec3, 3>{ triangleData.u, v2 - v1, -triangleData.v };

  for (auto const& voxelAxes : voxelAxis) {
    for (auto const& triangleEdge : triangleEdges) {
      auto const axesEdgeCross = lina::cross(voxelAxes, triangleEdge);

      auto const p0 = lina::dot(axesEdgeCross, triangleData.Q);
      auto const p1 = lina::dot(axesEdgeCross, v1);
      auto const p2 = lina::dot(axesEdgeCross, v2);

      auto const r = (halfVoxelDimension * std::abs(axesEdgeCross[0]))
                     + (halfVoxelDimension * std::abs(axesEdgeCross[1]))
                     + (halfVoxelDimension * std::abs(axesEdgeCross[2]));

      if (std::min({ p0, p1, p2 }) > r || std::max({ p0, p1, p2 }) < -r) {
        // A separating axis has been found, there is no overlap.
        return false;
      }
    }
  }

  return true;
}

}// namespace trace
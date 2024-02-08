#include "lib/trace/geometry/triangle.h"

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"

#include <array>
#include <cstddef>
#include <iterator>
#include <optional>
#include <span>
#include <utility>
#include <vector>

namespace trace {

auto triangleStripCollide(Ray const& ray,
  lina::Vec3 const& center,
  std::span<lina::Vec3 const> triangleStrip) -> std::optional<std::pair<Collision, double>>
{
  auto closestCollisionData = std::optional<std::pair<Collision, double>>{};
  auto swapUV = false;
  for (auto i = 0; i <= static_cast<int>(triangleStrip.size()) - 3; ++i) {
    auto triplet = std::span<lina::Vec3 const, 3>(std::next(triangleStrip.begin(), i), size_t{ 3 });
    auto collisionCandidateData = triangleCollide(ray, center, triplet, swapUV);
    if (collisionCandidateData) {
      if (!closestCollisionData || closestCollisionData->second > collisionCandidateData->second) {
        closestCollisionData = collisionCandidateData;
      }
    }
    swapUV = !swapUV;
  }
  return closestCollisionData;
}

auto triangleCollide(Ray const& ray,
  lina::Vec3 const& center,
  std::span<lina::Vec3 const, 3> triplet,
  bool swapUV) -> std::optional<std::pair<Collision, double>>
{
  auto localizedTriplet = std::array<lina::Vec3, 3>{ triplet[0] + center, triplet[1] + center, triplet[2] + center };
  auto u = localizedTriplet[2] - localizedTriplet[0];
  auto v = localizedTriplet[1] - localizedTriplet[0];
  if (swapUV) { std::swap(u, v); }

  auto normal = lina::unit(lina::cross(u, v));
  auto D = lina::dot(normal, localizedTriplet[0]);

  auto denominator = lina::dot(normal, ray.Direction());
  if (denominator == 0.0) { return std::optional<std::pair<Collision, double>>{}; }

  auto t = (D - lina::dot(normal, ray.Source())) / denominator;
  if (t <= 0.0) { return std::optional<std::pair<Collision, double>>{}; }
  auto collisionPoint = ray.Source() + ray.Direction() * t;

  auto planeDelta = collisionPoint - localizedTriplet[0];
  auto common = normal / (lina::dot(normal, lina::cross(u, v)));
  auto alpha = lina::dot(common, lina::cross(planeDelta, v));
  auto beta = lina::dot(common, lina::cross(u, planeDelta));

  auto scalarSum = alpha + beta;
  if (0.0 > alpha || alpha > 1.0 || 0.0 > beta || beta > 1.0 || scalarSum > 1.0) {
    return std::optional<std::pair<Collision, double>>{};
  }

  auto collision = Collision{};
  std::swap(collision.point, collisionPoint);
  collision.normal = normal;
  collision.frontFace = lina::dot(normal, ray.Direction()) < 0.0;

  return std::pair<Collision, double>({ collision, t });
}

auto triangleCollide(Ray const& ray,
  lina::Vec3 const& center,
  std::vector<lina::Vec3> const& vertices,
  std::span<std::size_t const, 3> triangle) -> std::optional<std::pair<Collision, double>>
{
  auto localizedTriangle = std::array<lina::Vec3, 3>{
    vertices[triangle[0]] + center, vertices[triangle[1]] + center, vertices[triangle[2]] + center
  };
  auto u = localizedTriangle[2] - localizedTriangle[0];
  auto v = localizedTriangle[1] - localizedTriangle[0];

  auto normal = lina::unit(lina::cross(u, v));
  auto D = lina::dot(normal, localizedTriangle[0]);

  auto denominator = lina::dot(normal, ray.Direction());
  if (denominator == 0.0) { return std::optional<std::pair<Collision, double>>{}; }

  auto t = (D - lina::dot(normal, ray.Source())) / denominator;
  if (t <= 0.0) { return std::optional<std::pair<Collision, double>>{}; }
  auto collisionPoint = ray.Source() + ray.Direction() * t;

  auto planeDelta = collisionPoint - localizedTriangle[0];
  auto common = normal / (lina::dot(normal, lina::cross(u, v)));
  auto alpha = lina::dot(common, lina::cross(planeDelta, v));
  auto beta = lina::dot(common, lina::cross(u, planeDelta));

  auto scalarSum = alpha + beta;
  if (0.0 > alpha || alpha > 1.0 || 0.0 > beta || beta > 1.0 || scalarSum > 1.0) {
    return std::optional<std::pair<Collision, double>>{};
  }

  auto collision = Collision{};
  std::swap(collision.point, collisionPoint);
  collision.normal = normal;
  collision.frontFace = lina::dot(normal, ray.Direction()) < 0.0;

  return std::pair<Collision, double>({ collision, t });
}

// Just copy-pasting these here so that we can repurpose them here quickly,
// then we can migrate the solution back into the other geometries.
auto meshCollide(Ray const& ray,
  lina::Vec3 const& center,
  std::vector<lina::Vec3> const& vertices,
  std::vector<std::array<std::size_t, 3>> const& triangles) -> std::optional<std::pair<Collision, double>>
{
  auto closestCollisionData = std::optional<std::pair<Collision, double>>{};
  for (auto const& triangle : triangles) {
    auto collisionCandidateData = triangleCollide(ray, center, vertices, triangle);
    if (collisionCandidateData) {
      if (!closestCollisionData || closestCollisionData->second > collisionCandidateData->second) {
        closestCollisionData = collisionCandidateData;
      }
    }
  }
  return closestCollisionData;
}

}// namespace trace
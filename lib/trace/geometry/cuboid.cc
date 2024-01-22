#include "cuboid.h"
#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"
#include "lib/trace/transform.h"
#include <array>
#include <iterator>
#include <optional>
#include <span>
#include <utility>

namespace trace {

Cuboid::Cuboid(lina::Vec3 center, double width, double height, double depth) : center_{ center }
{
  auto halfWidth = width / 2.0;
  auto halfHeight = height / 2.0;
  auto halfDepth = depth / 2.0;

  triangleStrip1_[0] = lina::Vec3{ -halfWidth, halfDepth, -halfHeight };
  triangleStrip1_[1] = lina::Vec3{ -halfWidth, -halfDepth, -halfHeight };
  triangleStrip1_[2] = lina::Vec3{ halfWidth, halfDepth, -halfHeight };
  triangleStrip1_[3] = lina::Vec3{ halfWidth, -halfDepth, -halfHeight };
  triangleStrip1_[4] = lina::Vec3{ halfWidth, halfDepth, halfHeight };
  triangleStrip1_[5] = lina::Vec3{ halfWidth, -halfDepth, halfHeight };
  triangleStrip1_[6] = lina::Vec3{ -halfWidth, halfDepth, halfHeight };
  triangleStrip1_[7] = lina::Vec3{ -halfWidth, -halfDepth, halfHeight };
  // non-rendering transitional vertices
  triangleStrip2_[0] = lina::Vec3{ halfWidth, halfDepth, -halfHeight };
  triangleStrip2_[1] = lina::Vec3{ halfWidth, halfDepth, halfHeight };
  triangleStrip2_[2] = lina::Vec3{ -halfWidth, halfDepth, -halfHeight };
  triangleStrip2_[3] = lina::Vec3{ -halfWidth, halfDepth, halfHeight };
  triangleStrip2_[4] = lina::Vec3{ -halfWidth, -halfDepth, -halfHeight };
  triangleStrip2_[5] = lina::Vec3{ -halfWidth, -halfDepth, halfHeight };
  triangleStrip2_[6] = lina::Vec3{ halfWidth, -halfDepth, -halfHeight };
  triangleStrip2_[7] = lina::Vec3{ halfWidth, -halfDepth, halfHeight };
}

auto Cuboid::Collide(Ray const& ray) const -> std::optional<Collision>
{
  auto closestCollisionData = std::optional<std::pair<Collision, double>>{};
  auto swapUV = false;
  for (auto i = std::size_t{ 0 }; i <= triangleStrip1_.size() - 3; ++i) {
    auto triplet = std::span<lina::Vec3 const, 3>(std::next(triangleStrip1_.begin(), i), size_t{ 3 });
    auto collisionCandidateData = triangleCollision(ray, triplet, swapUV);
    if (collisionCandidateData) {
      if (!closestCollisionData || collisionCandidateData->second < closestCollisionData->second) {
        closestCollisionData = collisionCandidateData;
      }
    }
    swapUV = !swapUV;
  }
  swapUV = false;
  for (auto i = std::size_t{ 0 }; i <= triangleStrip2_.size() - 3; ++i) {
    auto triplet = std::span<lina::Vec3 const, 3>(std::next(triangleStrip2_.begin(), i), size_t{ 3 });
    auto collisionCandidateData = triangleCollision(ray, triplet, swapUV);
    if (collisionCandidateData) {
      if (!closestCollisionData || collisionCandidateData->second < closestCollisionData->second) {
        closestCollisionData = collisionCandidateData;
      }
    }
    swapUV = !swapUV;
  }

  if (!closestCollisionData) { return std::optional<Collision>{}; }
  return std::optional<Collision>{ closestCollisionData->first };
}

auto Cuboid::Transform(std::span<double const, 16> transformationMatrix) -> void
{
  // hacked for the time being
  auto center4 = trace::extend3Dto4D(center_, false);
  center_ = trace::cut4Dto3D(lina::mul(transformationMatrix, center4));

  for (auto& vertex : triangleStrip1_) {
    auto vertex4 = trace::extend3Dto4D(vertex, true);
    vertex = trace::cut4Dto3D(lina::mul(transformationMatrix, vertex4));
  }
  for (auto& vertex : triangleStrip2_) {
    auto vertex4 = trace::extend3Dto4D(vertex, true);
    vertex = trace::cut4Dto3D(lina::mul(transformationMatrix, vertex4));
  }
}

auto Cuboid::triangleCollision(Ray const& ray, std::span<lina::Vec3 const, 3> triplet, bool swapUV) const
  -> std::optional<std::pair<Collision, double>>
{
  auto localizedTriplet = std::array<lina::Vec3, 3>{ triplet[0] + center_, triplet[1] + center_, triplet[2] + center_ };
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

  return std::optional<std::pair<Collision, double>>({ collision, t });
}

}// namespace trace
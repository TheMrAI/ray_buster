#include "cuboid.h"
#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/triangle.h"
#include "lib/trace/pdf.h"
#include "lib/trace/ray.h"
#include "lib/trace/transform.h"
#include <array>
#include <optional>
#include <random>
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
  auto closestCollisionData = triangleStripCollide(ray, center_, triangleStrip1_);
  auto stripCollision = triangleStripCollide(ray, center_, triangleStrip2_);
  if (stripCollision) {
    if (!closestCollisionData || closestCollisionData->second > stripCollision->second) {
      closestCollisionData = stripCollision;
    }
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

auto Cuboid::SamplingPDF(std::mt19937& /*randomGenerator*/, lina::Vec3 const& /*from*/) const -> PDF { return PDF{}; }

}// namespace trace
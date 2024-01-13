#include "plane.h"

#include "lib/lina/lina.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"
#include <optional>

namespace trace {

Plane::Plane(lina::Vec3 center, double width, double depth)
  : center_{ center }, normal_{ lina::Vec3{ 0.0, 0.0, 1.0 } }, width_{ width }, depth_{ depth },
    globalU_{ center_ + lina::Vec3{ width, 0.0, 0.0 } }, globalV_{ center_ + lina::Vec3{ 0.0, depth, 0.0 } }, D_{
      lina::dot(center_, normal_)
    }
{}

// using the plane equation of n*v = D
auto Plane::Collide(Ray const& ray) const -> std::optional<Collision>
{
  auto denominator = lina::dot(normal_, ray.Direction());
  if (denominator == 0.0) { return std::optional<Collision>{}; }

  auto t = (D_ - lina::dot(normal_, ray.Source())) / denominator;
  if (t <= 0.0) { return std::optional<Collision>{}; }
  auto collisionPoint = ray.Source() + ray.Direction() * t;

  auto localU = globalU_ - center_;
  auto localV = globalV_ - center_;
  auto Q = center_ - (localU / 2.0) - (localV / 2.0);
  if (width_ > 0.0) {
    // To find out if we hit within the plane area, we just shift
    // the plane collision point using collisionPoint - Q call this P. Then we
    // take its dot product with localU, which is a vector representing the "side" of the plane.
    // This dot product as all dot products represents projecting P onto localU and multiplying this
    // vectors length by the length of localU. So if we divide the result by localU's length, we get the length
    // of the projection. Then it is easy to check if we are within the rectangle as the valid range is
    // [0, |localU|]. Same is true with localV.
    auto alpha = lina::dot(localU, collisionPoint - Q) / localU.Length();
    if (0.0 > alpha || alpha > localU.Length()) { return std::optional<Collision>{}; }
  }

  if (depth_ > 0.0) {
    auto beta = lina::dot(localV, collisionPoint - Q) / localV.Length();
    if (0.0 > beta || beta > localV.Length()) { return std::optional<Collision>{}; }
  }

  auto collision = Collision{};
  std::swap(collision.point, collisionPoint);
  collision.normal = normal_;
  collision.frontFace = lina::dot(normal_, ray.Direction()) < 0.0;

  return std::optional<Collision>{ std::move(collision) };
}

auto Plane::Transform(std::span<double const, 9> transformationMatrix) -> void
{
  center_ = lina::Vec3{ lina::mul(transformationMatrix, center_.Components()) };
  globalU_ = lina::Vec3{ lina::mul(transformationMatrix, globalU_.Components()) };
  globalV_ = lina::Vec3{ lina::mul(transformationMatrix, globalV_.Components()) };
  normal_ = lina::cross(globalU_, globalV_);// order!
}

}// namespace trace
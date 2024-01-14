#include "plane.h"

#include "lib/lina/lina.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"
#include <optional>

#include <iostream>

namespace trace {

Plane::Plane(lina::Vec3 center, double width, double depth)
  : center_{ center }, normal_{ lina::Vec3{ 0.0, 0.0, 1.0 } }, width_{ width }, depth_{ depth },
    localU_{ lina::Vec3{ width, 0.0, 0.0 } }, localV_{ lina::Vec3{ 0.0, depth, 0.0 } }, D_{ lina::dot(center_,
                                                                                          normal_) }
{}

// using the plane equation of n*v = D
auto Plane::Collide(Ray const& ray) const -> std::optional<Collision>
{
  auto denominator = lina::dot(normal_, ray.Direction());
  if (denominator == 0.0) { return std::optional<Collision>{}; }

  auto t = (D_ - lina::dot(normal_, ray.Source())) / denominator;
  if (t <= 0.0) { return std::optional<Collision>{}; }
  auto collisionPoint = ray.Source() + ray.Direction() * t;

  auto Q = center_ - (localU_ / 2.0) - (localV_ / 2.0);
  if (width_ > 0.0) {
    // To find out if we hit within the plane area, we just shift
    // the plane collision point using collisionPoint - Q call this P. Then we
    // take its dot product with localU_, which is a vector representing the "side" of the plane.
    // This dot product as all dot products represents projecting P onto localU_ and multiplying this
    // vectors length by the length of localU_. So if we divide the result by localU_'s length, we get the length
    // of the projection. Then it is easy to check if we are within the rectangle as the valid range is
    // [0, |localU_|]. Same is true with localV.
    auto alpha = lina::dot(localU_, collisionPoint - Q) / localU_.Length();
    if (0.0 > alpha || alpha > localU_.Length()) { return std::optional<Collision>{}; }
  }

  if (depth_ > 0.0) {
    auto beta = lina::dot(localV_, collisionPoint - Q) / localV_.Length();
    if (0.0 > beta || beta > localV_.Length()) { return std::optional<Collision>{}; }
  }

  auto collision = Collision{};
  std::swap(collision.point, collisionPoint);
  collision.normal = normal_;
  collision.frontFace = lina::dot(normal_, ray.Direction()) < 0.0;

  return std::optional<Collision>{ std::move(collision) };
}

auto toFourLong(lina::Vec3 vec, bool vector) -> std::array<double, 4>
{
  auto fourLong = std::array<double, 4>{ vec[0], vec[1], vec[2], vector ? 0.0 : 1.0 };
  return fourLong;
}

auto fourToVec3(std::array<double, 4> four) -> lina::Vec3 { return lina::Vec3{ four[0], four[1], four[2] }; }

auto Plane::Transform(std::span<double const, 16> transformationMatrix) -> void
{
  // hacked for the time being
  auto center4 = toFourLong(center_, false);
  auto localU4 = toFourLong(localU_, true);
  auto localV4 = toFourLong(localV_, true);

  center_ = fourToVec3(lina::mul(transformationMatrix, center4));
  localU_ = fourToVec3(lina::mul(transformationMatrix, localU4));
  localV_ = fourToVec3(lina::mul(transformationMatrix, localV4));
  normal_ = lina::unit(lina::cross(localU_, localV_));// order matters!
  D_ = lina::dot(center_, normal_);
}

}// namespace trace

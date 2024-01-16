#include "plane.h"

#include "lib/lina/lina.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"
#include "lib/trace/transform.h"
#include "lib/trace/util.h"
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

auto Plane::Transform(std::span<double const, 16> transformationMatrix) -> void
{
  // hacked for the time being
  auto center4 = trace::extend3Dto4D(center_, false);
  auto localU4 = trace::extend3Dto4D(localU_, true);
  auto localV4 = trace::extend3Dto4D(localV_, true);

  center_ = trace::cut4Dto3D(lina::mul(transformationMatrix, center4));
  localU_ = trace::cut4Dto3D(lina::mul(transformationMatrix, localU4));
  localV_ = trace::cut4Dto3D(lina::mul(transformationMatrix, localV4));
  normal_ = lina::unit(lina::cross(localU_, localV_));// order matters!
  D_ = lina::dot(center_, normal_);
}

auto build(lina::Vec3 center, double width, double depth, Axis normalAxis, Orientation orientation)
  -> std::optional<Plane>
{
  if (width < 0.0 || depth < 0.0) { return std::optional<Plane>{}; }

  auto plane = std::optional<Plane>{ std::in_place, lina::Vec3{ 0.0, 0.0, 0.0 }, width, depth };
  auto transformation = trace::unitMatrix();

  auto degrees = 90.0;
  switch (normalAxis) {
  case Axis::X: {
    if (orientation == Orientation::Aligned) { degrees *= -1.0; }
    transformation = lina::mul(rotateAlongY(degreesToRadians(degrees)), transformation);
    break;
  }
  case Axis::Y: {
    if (orientation == Orientation::Reverse) { degrees *= -1.0; }
    transformation = lina::mul(rotateAlongX(degreesToRadians(degrees)), transformation);
    break;
  }
  case Axis::Z: {
    if (orientation == Orientation::Aligned) { return plane; }
    transformation = lina::mul(rotateAlongX(degreesToRadians(180.0)), transformation);
    break;
  }
  default:
    return std::optional<Plane>{};
  }

  transformation = lina::mul(translate(center), transformation);
  plane->Transform(transformation);
  return plane;
}

}// namespace trace

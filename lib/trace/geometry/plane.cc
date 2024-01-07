#include "plane.h"

#include "lib/trace/collision.h"
#include "lib/trace/ray.h"
#include <optional>

namespace trace {

Plane::Plane(lina::Vec3 center, double width, double depth)
  : center_{ center }, normal_{ lina::Vec3{ 0.0, 1.0, 0.0 } }, u_{ lina::Vec3{ width, 0.0, 0.0 } }, v_{
      lina::Vec3{ 0.0, 0.0, depth }
    }
{
  D_ = lina::dot(center_, normal_);
  Q_ = center_ - (u_ / 2.0) - (v_ / 2.0);
}

// using the plane equation of n*v = D
auto Plane::Collide(Ray const& ray) -> std::optional<Collision>
{
  auto denominator = dot(normal_, ray.Direction());
  if (denominator == 0.0) { return std::optional<Collision>{}; }

  auto t = (D_ - dot(normal_, ray.Source())) / denominator;
  if (t <= 0.0) { return std::optional<Collision>{}; }
  auto collisionPoint = ray.Source() + ray.Direction() * t;
  auto alpha = dot(u_, collisionPoint - Q_) / u_.Length();
  auto beta = dot(v_, collisionPoint - Q_) / v_.Length();

  if (0.0 > alpha || alpha > u_.Length() || 0.0 > beta || beta > v_.Length()) { return std::optional<Collision>{}; }

  auto collision = Collision{};
  std::swap(collision.point, collisionPoint);
  collision.normal = normal_;
  collision.frontFace = dot(normal_, ray.Direction()) < 0.0;

  return std::optional<Collision>{ std::move(collision) };
}

}// namespace trace
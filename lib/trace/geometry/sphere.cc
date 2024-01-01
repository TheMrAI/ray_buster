#include "sphere.h"
#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"
#include <cmath>
#include <optional>

namespace trace {

Sphere::Sphere() : center_{ lina::Vec3{} }, radius_{ 1.0 } {}
Sphere::Sphere(lina::Vec3 center, double radius) : center_{ center }, radius_{ radius } {}

auto Sphere::Collide(Ray const& ray) -> std::optional<Collision>
{
  auto oc = ray.Source() - center_;
  auto a = lina::lengthSquared(ray.Direction().Components());
  auto half_b = dot(oc, ray.Direction());
  auto c = lina::lengthSquared(oc.Components()) - radius_ * radius_;
  auto discriminant = half_b * half_b - a * c;

  auto t = (-half_b - std::sqrt(discriminant)) / a;
  if (t < 0.0) { t = (-half_b + std::sqrt(discriminant)) / a; }
  if (discriminant < 0.0 || t < 0.0) { return std::optional<Collision>{}; }

  auto collision = Collision{};
  collision.point = ray.Source() + ray.Direction() * t;
  collision.normal = lina::unit(collision.point - center_);

  collision.frontFace = dot(ray.Direction(), collision.normal) < 0.0;
  return std::optional<Collision>{ std::move(collision) };
}

}// namespace trace
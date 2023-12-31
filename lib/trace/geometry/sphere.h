#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_SPHERE_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_SPHERE_H_

#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"

#include <cmath>

namespace trace {

class Sphere : public Component
{
public:
  Sphere() : center_{ lina::Vec3{} }, radius_{ 1.0 } {}
  Sphere(lina::Vec3 center, double radius) : center_{ center }, radius_{ radius } {}

  auto Collide(Ray const& ray) -> std::optional<Collision> override
  {
    auto oc = ray.Source() - center_;
    auto a = lina::length_squared(ray.Direction().Components());
    auto half_b = dot(oc, ray.Direction());
    auto c = lina::length_squared(oc.Components()) - radius_ * radius_;
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

private:
  lina::Vec3 center_;
  double radius_;
};

}// namespace trace

#endif
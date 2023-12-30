#ifndef RAY_BUSTER_COLLISION_HEADER
#define RAY_BUSTER_COLLISION_HEADER

#include "lib/lina/vec3.h"
#include <optional>

namespace trace {

struct Collision
{
  lina::Vec3 point = lina::Vec3{};
  lina::Vec3 normal = lina::Vec3{};
  bool front_face = true;// which face the normal vector is on
  // this is an arbitrary property, front face could be either face, depends on the object itself
};

class Ray
{
public:
  Ray(lina::Vec3 source, lina::Vec3 direction) : source_{ source }, dir_{ direction } {};

  lina::Vec3 const& Source() const { return source_; }

  lina::Vec3 const& Direction() const { return dir_; }

private:
  lina::Vec3 source_;
  lina::Vec3 dir_;
};

class Component
{
public:
  virtual auto Collide(Ray const& ray) -> std::optional<Collision> = 0;
};


class Sphere : public Component
{
public:
  Sphere() : center_{ lina::Vec3{} }, radius_{ 1.0 } {}
  Sphere(lina::Vec3 center, double radius) : center_{ center }, radius_{ radius } {}

  auto Collide(Ray const& ray) -> std::optional<Collision>
  {
    auto oc = ray.Source() - center_;
    auto a = lina::length_squared(ray.Direction().Components());
    auto half_b = dot(oc, ray.Direction());
    auto c = lina::length_squared(oc.Components()) - radius_ * radius_;
    auto discriminant = half_b * half_b - a * c;


    auto collision = Collision{};
    auto t = (-half_b - std::sqrt(discriminant)) / a;
    if (discriminant < 0.0 || t < 0.0) { return std::optional<Collision>{}; }

    collision.point = ray.Source() + ray.Direction() * t;
    collision.normal = lina::unit(collision.point - center_);
    collision.front_face = dot(ray.Direction(), collision.normal) < 0.0;
    return std::optional<Collision>{ collision };
  }

private:
  lina::Vec3 center_;
  double radius_;
};

}// namespace trace

#endif
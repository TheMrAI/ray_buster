#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hittable.hpp"
#include "vec3.hpp"
#include <memory>

class sphere : public hittable
{
private:
  vec3 center_;
  double radius_;
  std::shared_ptr<material> material_ptr_;

public:
  sphere() {}
  sphere(vec3 center, double radius, std::shared_ptr<material> material)
    : center_{ center }, radius_{ radius }, material_ptr_{ material } {};

  virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;
  virtual bool bounding_box(double time_0, double time_1, aabb& bounding_box) const override;

private:
  auto get_sphere_uv(vec3 const& point, double& u, double& v) const -> void
  {
    // point: a given point on the sphere of radius one, centered at the origin.
    // u: returned value [0,1] of angle around the Y axis from X=-1.
    // v: returned value [0,1] of angle from Y=-1 to Y=+1.
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

    auto theta = acos(-point.y());
    auto phi = atan2(-point.z(), point.x()) + std::numbers::pi;

    u = phi / (2.0 * std::numbers::pi);
    v = theta / std::numbers::pi;
  }
};

bool sphere::hit(ray const& r, double t_min, double t_max, hit_record& rec) const
{
  vec3 oc = r.origin() - center_;
  auto r_direction_length = r.direction().length();
  auto a = r_direction_length * r_direction_length;
  auto half_b = dot(oc, r.direction());
  auto oc_length = oc.length();
  auto c = oc_length * oc_length - radius_ * radius_;

  auto discriminant = half_b * half_b - a * c;
  if (discriminant < 0) { return false; }
  auto sqrtd = sqrt(discriminant);

  // Find the nearest root that lies in the acceptable range.
  auto root = (-half_b - sqrtd) / a;
  if (root < t_min || t_max < root) {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || t_max < root) { return false; }
  }

  rec.t = root;
  rec.p = r.at(rec.t);
  auto outward_normal = (rec.p - center_) / radius_;
  rec.set_face_normal(r, outward_normal);
  get_sphere_uv(outward_normal, rec.u, rec.v);
  rec.material_ptr = material_ptr_;

  return true;
}

bool sphere::bounding_box(double /*time_0*/, double /*time_1*/, aabb& bounding_box) const
{
  bounding_box = aabb{ center_ - vec3{ radius_, radius_, radius_ }, center_ + vec3{ radius_, radius_, radius_ } };
  return true;
}

#endif
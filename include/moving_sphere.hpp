#ifndef MOVING_SPHERE_HPP
#define MOVING_SPHERE_HPP

#include "aabb.hpp"
#include "hittable.hpp"
#include "util.hpp"
#include "vec3.hpp"
#include <memory>

class moving_sphere : public hittable
{
private:
  vec3 begin_center_;
  vec3 end_center_;
  double begin_time_;
  double end_time_;
  double radius_;
  std::shared_ptr<material> material_ptr_;

public:
  moving_sphere() {}

  moving_sphere(vec3 begin_center,
    vec3 end_center,
    double begin_time,
    double end_time,
    double radius,
    std::shared_ptr<material> material_ptr)
    : begin_center_{ begin_center }, end_center_{ end_center },
      begin_time_{ begin_time }, end_time_{ end_time }, radius_{ radius }, material_ptr_{ material_ptr } {};

  virtual bool hit(ray const& ray, double t_min, double t_max, hit_record& rec) const override;
  virtual bool bounding_box(double time_0, double time_1, aabb& bounding_box) const override;

  vec3 center_at_time(double time) const
  {
    return begin_center_ + ((time - begin_time_) / (end_time_ - begin_time_)) * (end_center_ - begin_center_);
  }
};

auto moving_sphere::hit(ray const& ray, double t_min, double t_max, hit_record& rec) const -> bool
{
  vec3 oc = ray.origin() - center_at_time(ray.time());
  auto a = ray.direction().length() * ray.direction().length();
  auto half_b = dot(oc, ray.direction());
  auto c = oc.length() * oc.length() - radius_ * radius_;

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
  rec.p = ray.at(rec.t);
  auto outward_normal = (rec.p - center_at_time(ray.time())) / radius_;
  rec.set_face_normal(ray, outward_normal);
  rec.material_ptr = material_ptr_;

  return true;
}

auto moving_sphere::bounding_box(double time_0, double time_1, aabb& bounding_box) const -> bool
{
  aabb box_0{ center_at_time(time_0) - vec3{ radius_, radius_, radius_ },
    center_at_time(time_0) + vec3{ radius_, radius_, radius_ } };
  aabb box_1{ center_at_time(time_1) - vec3{ radius_, radius_, radius_ },
    center_at_time(time_1) + vec3{ radius_, radius_, radius_ } };
  bounding_box = surrounding_box(box_0, box_1);
  return true;
}

#endif
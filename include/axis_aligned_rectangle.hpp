#ifndef AXIS_ALLIGNED_RECTANGLE_HPP
#define AZIS_ALLIGNED_RECTANGLE_HPP

#include <memory>

#include "aabb.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class xy_rect : public hittable
{
private:
  double x0_;
  double x1_;
  double y0_;
  double y1_;
  double k_;
  std::shared_ptr<material> material_ptr_;

public:
  xy_rect() {}
  xy_rect(double x0, double x1, double y0, double y1, double k, std::shared_ptr<material> material)
    : x0_{ x0 }, x1_{ x1 }, y0_{ y0 }, y1_{ y1 }, k_{ k }, material_ptr_{ material }
  {}

  virtual bool hit(ray const& ray, double t_min, double t_max, hit_record& rec) const override;
  virtual bool bounding_box(double time_0, double time_1, aabb& bounding_box) const override;
};

auto xy_rect::hit(ray const& ray, double t_min, double t_max, hit_record& rec) const -> bool
{
  auto t = (k_ - ray.origin().z()) / ray.direction().z();

  if (t < t_min || t > t_max) { return false; }

  auto x = ray.origin().x() + t * ray.direction().x();
  auto y = ray.origin().y() + t * ray.direction().y();

  if (x < x0_ || x > x1_ || y < y0_ || y > y1_) { return false; }
  rec.u = (x - x0_) / (x1_ - x0_);
  rec.v = (y - y0_) / (y1_ - y0_);
  rec.t = t;
  auto outward_normal = vec3{ 0.0, 0.0, 1.0 };
  rec.set_face_normal(ray, outward_normal);
  rec.material_ptr = material_ptr_;
  rec.p = ray.at(t);
  return true;
}

auto xy_rect::bounding_box(double /*time_0*/, double /*time_1*/, aabb& bounding_box) const -> bool
{
  // The bounding box must have non-zero width in each dimension, so pad the Z
  // dimension a small amount.
  bounding_box = aabb(vec3{ x0_, y0_, k_ - 0.0001 }, vec3{ x1_, y1_, k_ + 0.0001 });
  return true;
}

class xz_rect : public hittable
{
private:
  double x0_;
  double x1_;
  double z0_;
  double z1_;
  double k_;
  std::shared_ptr<material> material_ptr_;

public:
  xz_rect() {}
  xz_rect(double x0, double x1, double z0, double z1, double k, std::shared_ptr<material> material)
    : x0_{ x0 }, x1_{ x1 }, z0_{ z0 }, z1_{ z1 }, k_{ k }, material_ptr_{ material }
  {}

  virtual bool hit(ray const& ray, double t_min, double t_max, hit_record& rec) const override;
  virtual bool bounding_box(double time_0, double time_1, aabb& bounding_box) const override;
};

auto xz_rect::hit(ray const& ray, double t_min, double t_max, hit_record& rec) const -> bool
{
  auto t = (k_ - ray.origin().y()) / ray.direction().y();

  if (t < t_min || t > t_max) { return false; }

  auto x = ray.origin().x() + t * ray.direction().x();
  auto z = ray.origin().z() + t * ray.direction().z();

  if (x < x0_ || x > x1_ || z < z0_ || z > z1_) { return false; }
  rec.u = (x - x0_) / (x1_ - x0_);
  rec.v = (z - z0_) / (z1_ - z0_);
  rec.t = t;
  auto outward_normal = vec3{ 0.0, 1.0, 0.0 };
  rec.set_face_normal(ray, outward_normal);
  rec.material_ptr = material_ptr_;
  rec.p = ray.at(t);
  return true;
}

auto xz_rect::bounding_box(double /*time_0*/, double /*time_1*/, aabb& bounding_box) const -> bool
{
  // The bounding box must have non-zero width in each dimension, so pad the Z
  // dimension a small amount.
  bounding_box = aabb(vec3{ x0_, k_ - 0.0001, z0_ }, vec3{ x1_, k_ + 0.0001, z1_ });
  return true;
}


class yz_rect : public hittable
{
private:
  double y0_;
  double y1_;
  double z0_;
  double z1_;
  double k_;
  std::shared_ptr<material> material_ptr_;

public:
  yz_rect() {}
  yz_rect(double y0, double y1, double z0, double z1, double k, std::shared_ptr<material> material)
    : y0_{ y0 }, y1_{ y1 }, z0_{ z0 }, z1_{ z1 }, k_{ k }, material_ptr_{ material }
  {}

  virtual bool hit(ray const& ray, double t_min, double t_max, hit_record& rec) const override;
  virtual bool bounding_box(double time_0, double time_1, aabb& bounding_box) const override;
};

auto yz_rect::hit(ray const& ray, double t_min, double t_max, hit_record& rec) const -> bool
{
  auto t = (k_ - ray.origin().x()) / ray.direction().x();

  if (t < t_min || t > t_max) { return false; }

  auto y = ray.origin().y() + t * ray.direction().y();
  auto z = ray.origin().z() + t * ray.direction().z();

  if (y < y0_ || y > y1_ || z < z0_ || z > z1_) { return false; }
  rec.u = (y - y0_) / (y1_ - y0_);
  rec.v = (z - z0_) / (z1_ - z0_);
  rec.t = t;
  auto outward_normal = vec3{ 1.0, 0.0, 0.0 };
  rec.set_face_normal(ray, outward_normal);
  rec.material_ptr = material_ptr_;
  rec.p = ray.at(t);
  return true;
}

auto yz_rect::bounding_box(double /*time_0*/, double /*time_1*/, aabb& bounding_box) const -> bool
{
  // The bounding box must have non-zero width in each dimension, so pad the Z
  // dimension a small amount.
  bounding_box = aabb(vec3{ k_ - 0.0001, y0_, z0_ }, vec3{ k_ + 0.0001, y1_, z1_ });
  return true;
}


#endif
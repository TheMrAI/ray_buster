#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "aabb.hpp"
#include "ray.hpp"
#include "util.hpp"
#include <memory>

class material;

struct hit_record
{
  vec3 p;
  vec3 normal;
  std::shared_ptr<material> material_ptr;
  double t;
  double u;
  double v;
  bool front_face;

  void set_face_normal(ray const& r, vec3 const& outward_normal)
  {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class hittable
{
public:
  virtual auto hit(ray const& r, double t_min, double t_max, hit_record& rec) const -> bool = 0;
  virtual auto bounding_box(double time_0, double time_1, aabb& bounding_box) const -> bool = 0;
  virtual auto pdf_value(vec3 const& /*origin*/, vec3 const& /*v*/) const -> double { return 0.0; }
  virtual auto random(vec3 const& /*origin*/) const -> vec3 { return vec3{ 1, 0, 0 }; }
};

class flip_face : public hittable
{
public:
  flip_face(std::shared_ptr<hittable> p) : ptr_(p) {}

  bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override
  {

    if (!ptr_->hit(r, t_min, t_max, rec)) return false;

    rec.front_face = !rec.front_face;
    return true;
  }

  bool bounding_box(double time0, double time1, aabb& output_box) const override
  {
    return ptr_->bounding_box(time0, time1, output_box);
  }

private:
  std::shared_ptr<hittable> ptr_;
};

class translate : public hittable
{
private:
  std::shared_ptr<hittable> object_;
  vec3 offset_;

public:
  translate(std::shared_ptr<hittable> object, vec3 const& displacement) : object_{ object }, offset_{ displacement } {}

  bool hit(ray const& incoming_ray, double t_min, double t_max, hit_record& rec) const override;
  bool bounding_box(double time_0, double time_1, aabb& bounding_box) const override;
};

auto translate::hit(ray const& incoming_ray, double t_min, double t_max, hit_record& rec) const -> bool
{
  auto translated_ray = ray{ incoming_ray.origin() - offset_, incoming_ray.direction(), incoming_ray.time() };
  if (!object_->hit(translated_ray, t_min, t_max, rec)) { return false; }
  rec.p += offset_;
  rec.set_face_normal(translated_ray, rec.normal);

  return true;
}

auto translate::bounding_box(double time_0, double time_1, aabb& bounding_box) const -> bool
{
  if (!object_->bounding_box(time_0, time_1, bounding_box)) { return false; }

  bounding_box = aabb(bounding_box.min() + offset_, bounding_box.max() + offset_);

  return true;
}

class rotate_y : public hittable
{
private:
  std::shared_ptr<hittable> object_;
  double sin_theta_;
  double cos_theta_;
  bool hasbox_;
  aabb bounding_box_;

public:
  rotate_y(std::shared_ptr<hittable> object, double angle);

  bool hit(ray const& incoming_ray, double t_min, double t_max, hit_record& rec) const override;
  bool bounding_box(double time_0, double time_1, aabb& bounding_box) const override;
};

rotate_y::rotate_y(std::shared_ptr<hittable> object, double angle) : object_{ object }
{
  auto radians = degrees_to_radians(angle);
  sin_theta_ = sin(radians);
  cos_theta_ = cos(radians);
  hasbox_ = object_->bounding_box(0, 1, bounding_box_);

  vec3 lower_bound{ std::numeric_limits<double>::infinity(),
    std::numeric_limits<double>::infinity(),
    std::numeric_limits<double>::infinity() };
  vec3 upper_bound{ -std::numeric_limits<double>::infinity(),
    -std::numeric_limits<double>::infinity(),
    -std::numeric_limits<double>::infinity() };

  for (auto i = 0; i < 2; ++i) {
    for (auto j = 0; j < 2; ++j) {
      for (auto k = 0; k < 2; ++k) {
        auto x = i * bounding_box_.max().x() + (1 - i) * bounding_box_.min().x();
        auto y = j * bounding_box_.max().y() + (1 - j) * bounding_box_.min().y();
        auto z = k * bounding_box_.max().z() + (1 - k) * bounding_box_.min().z();

        auto new_x = cos_theta_ * x + sin_theta_ * z;
        auto new_z = -sin_theta_ * x + cos_theta_ * z;

        auto tester = vec3{ new_x, y, new_z };

        for (auto c = 0; c < 3; ++c) {
          lower_bound[c] = std::fmin(lower_bound[c], tester[c]);
          upper_bound[c] = std::fmax(upper_bound[c], tester[c]);
        }
      }
    }
  }

  bounding_box_ = aabb{ lower_bound, upper_bound };
}

auto rotate_y::hit(ray const& incoming_ray, double t_min, double t_max, hit_record& rec) const -> bool
{
  auto origin = incoming_ray.origin();
  auto direction = incoming_ray.direction();

  origin[0] = cos_theta_ * incoming_ray.origin()[0] - sin_theta_ * incoming_ray.origin()[2];
  origin[2] = sin_theta_ * incoming_ray.origin()[0] + cos_theta_ * incoming_ray.origin()[2];

  direction[0] = cos_theta_ * incoming_ray.direction()[0] - sin_theta_ * incoming_ray.direction()[2];
  direction[2] = sin_theta_ * incoming_ray.direction()[0] + cos_theta_ * incoming_ray.direction()[2];

  auto rotated_ray = ray{ origin, direction, incoming_ray.time() };

  if (!object_->hit(rotated_ray, t_min, t_max, rec)) { return false; }

  auto p = rec.p;
  auto normal = rec.normal;

  p[0] = cos_theta_ * rec.p[0] + sin_theta_ * rec.p[2];
  p[2] = -sin_theta_ * rec.p[0] + cos_theta_ * rec.p[2];

  normal[0] = cos_theta_ * rec.normal[0] + sin_theta_ * rec.normal[2];
  normal[2] = -sin_theta_ * rec.normal[0] + cos_theta_ * rec.normal[2];

  rec.p = p;
  rec.set_face_normal(rotated_ray, normal);

  return true;
}

auto rotate_y::bounding_box(double /*time_0*/, double /*time_1*/, aabb& bounding_box) const -> bool
{
  bounding_box = bounding_box_;
  return hasbox_;
}

#endif
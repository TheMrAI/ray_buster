#ifndef CONSTANT_MEDIUM_HPP
#define CONSTANT_MEDIUM_HPP

#include "axis_aligned_rectangle.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "texture.hpp"
#include "util.hpp"
#include "vec3.hpp"

#include <memory>

class constant_medium : public hittable
{
private:
  std::shared_ptr<hittable> boundary_;
  double neg_inv_density_;
  std::shared_ptr<material> phase_function_;

public:
  constant_medium(std::shared_ptr<hittable> object, double density, std::shared_ptr<texture> texture)
    : boundary_{ object }, neg_inv_density_{ -1.0 / density }, phase_function_{ std::make_shared<isotropic>(texture) }
  {}

  constant_medium(std::shared_ptr<hittable> object, double density, vec3 color)
    : boundary_{ object }, neg_inv_density_{ -1.0 / density }, phase_function_{ std::make_shared<isotropic>(color) }
  {}

  virtual bool hit(ray const& incoming_ray, double t_min, double t_max, hit_record& rec) const override;
  virtual bool bounding_box(double time_0, double time_1, aabb& bounding_box) const override;
};

// Only handles convex boundary shapes
auto constant_medium::hit(ray const& incoming_ray, double t_min, double t_max, hit_record& rec) const -> bool
{
  hit_record rec_1;
  if (!boundary_->hit(
        incoming_ray, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), rec_1)) {
    return false;
  }

  hit_record rec_2;
  if (!boundary_->hit(incoming_ray, rec_1.t + 0.0001, std::numeric_limits<double>::infinity(), rec_2)) { return false; }

  rec_1.t = std::max(rec_1.t, t_min);
  rec_2.t = std::min(rec_2.t, t_max);

  if (rec_1.t >= rec_2.t) { return false; }

  rec_1.t = std::max(rec_1.t, 0.0);
  auto const ray_length = incoming_ray.direction().length();
  auto const distance_inside_boundary = (rec_2.t - rec_1.t) * ray_length;
  auto const hit_distance = neg_inv_density_ * log(random_double());

  if (hit_distance > distance_inside_boundary) { return false; }

  rec.t = rec_1.t + hit_distance / ray_length;
  rec.p = incoming_ray.at(rec.t);

  rec.normal = vec3{ 1, 0, 0 };// arbitrary
  rec.front_face = true;// arbitrary
  rec.material_ptr = phase_function_;

  return true;
}

auto constant_medium::bounding_box(double time_0, double time_1, aabb& bounding_box) const -> bool
{
  return boundary_->bounding_box(time_0, time_1, bounding_box);
}

#endif
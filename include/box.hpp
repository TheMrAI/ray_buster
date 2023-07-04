#ifndef BOX_HPP
#define BOX_HPP

#include "aabb.hpp"
#include "axis_aligned_rectangle.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <memory>

class box : public hittable
{
private:
  vec3 box_min_;
  vec3 box_max_;
  hittable_list sides_;

public:
  box(){};
  box(vec3 const& box_min, vec3 const& box_max, std::shared_ptr<material> material_ptr);

  virtual bool hit(ray const& ray, double t_min, double t_max, hit_record& rec) const override;
  virtual bool bounding_box(double time_0, double time_1, aabb& bounding_box) const override;
};

box::box(vec3 const& box_min, vec3 const& box_max, std::shared_ptr<material> material_ptr)
  : box_min_{ box_min }, box_max_{ box_max }
{
  sides_.add(
    std::make_shared<xy_rect>(box_min_.x(), box_max_.x(), box_min_.y(), box_max_.y(), box_max_.z(), material_ptr));
  sides_.add(
    std::make_shared<xy_rect>(box_min_.x(), box_max_.x(), box_min_.y(), box_max_.y(), box_min_.z(), material_ptr));

  sides_.add(
    std::make_shared<xz_rect>(box_min_.x(), box_max_.x(), box_min_.z(), box_max_.z(), box_max_.y(), material_ptr));
  sides_.add(
    std::make_shared<xz_rect>(box_min_.x(), box_max_.x(), box_min_.z(), box_max_.z(), box_min_.y(), material_ptr));

  sides_.add(
    std::make_shared<yz_rect>(box_min_.y(), box_max_.y(), box_min_.z(), box_max_.z(), box_max_.x(), material_ptr));
  sides_.add(
    std::make_shared<yz_rect>(box_min_.y(), box_max_.y(), box_min_.z(), box_max_.z(), box_min_.x(), material_ptr));
}

auto box::hit(ray const& ray, double t_min, double t_max, hit_record& rec) const -> bool
{
  return sides_.hit(ray, t_min, t_max, rec);
}

auto box::bounding_box(double /*time_0*/, double /*time_1*/, aabb& bounding_box) const -> bool
{
  bounding_box = aabb(box_min_, box_max_);
  return true;
}

#endif
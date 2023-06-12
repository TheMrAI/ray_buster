#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include "aabb.hpp"
#include "hittable.hpp"

#include <memory>
#include <vector>

class hittable_list : public hittable
{
public:
  std::vector<std::shared_ptr<hittable>> objects_;

public:
  hittable_list() {}
  hittable_list(std::shared_ptr<hittable> object) : objects_{ std::move(object) } {};

  auto clear() -> void { objects_.clear(); }
  void add(std::shared_ptr<hittable> object) { objects_.push_back(object); }

  virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;
  virtual bool bounding_box(double time_0, double time_1, aabb& bounding_box) const override;
};

auto hittable_list::hit(ray const& r, double t_min, double t_max, hit_record& rec) const -> bool
{
  hit_record temp_rec;
  auto hit_anything = false;
  auto closest_so_far = t_max;

  for (auto const& object : objects_) {
    if (object->hit(r, t_min, closest_so_far, temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  return hit_anything;
}

auto hittable_list::bounding_box(double time_0, double time_1, aabb& bounding_box) const -> bool
{
  if (objects_.empty()) { return false; }

  aabb temp_box;
  auto first_box = true;

  for (auto const& object : objects_) {
    if (!object->bounding_box(time_0, time_1, temp_box)) { return false; }
    bounding_box = first_box ? temp_box : surrounding_box(bounding_box, temp_box);
    first_box = false;
  }

  return true;
}

#endif
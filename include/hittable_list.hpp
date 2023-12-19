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
  auto objects() const -> std::vector<std::shared_ptr<hittable>> const& { return objects_; }

  auto hit(ray const& r, double t_min, double t_max, hit_record& rec) const -> bool override;
  auto bounding_box(double time_0, double time_1, aabb& bounding_box) const -> bool override;
  auto pdf_value(vec3 const& /*origin*/, vec3 const& /*v*/) const -> double override;
  auto random(vec3 const& o) const -> vec3 override;
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

auto hittable_list::pdf_value(vec3 const& o, vec3 const& v) const -> double {
  auto weight = 1.0/objects_.size();
  auto sum = 0.0;

  for (auto const& object : objects_) {
    sum += weight * object->pdf_value(o, v);
  }
  return sum;
}

auto hittable_list::random(vec3 const& o) const -> vec3 {
  auto int_size = static_cast<int>(objects_.size());
  return objects_[random_int(0, int_size-1)]->random(o);
}

#endif
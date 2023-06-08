#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

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

  virtual bool hit(ray const &r, double t_min, double t_max, hit_record &rec) const override;
};

auto hittable_list::hit(ray const &r, double t_min, double t_max, hit_record &rec) const -> bool
{
  hit_record temp_rec;
  auto hit_anything = false;
  auto closest_so_far = t_max;

  for (auto const &object : objects_) {
    if (object->hit(r, t_min, closest_so_far, temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  return hit_anything;
}

#endif
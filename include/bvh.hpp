#ifndef BVH_HPP
#define BVH_HPP

#include <algorithm>
#include <memory>
#include <vector>

#include "aabb.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "ray.hpp"

class bvh_node : public hittable
{
private:
  std::shared_ptr<hittable> left_;
  std::shared_ptr<hittable> right_;
  aabb bounding_box_;

public:
  bvh_node();

  bvh_node(std::vector<std::shared_ptr<hittable>> const &list, double time_0, double time_1)
    : bvh_node(list.objects, 0, list.objects.size(), time_0, time_1)
  {}

  bvh_node(std::vector<std::shared_ptr<hittable>> &src_objects, size_t start, size_t end, double time_0, double time_1);

  virtual bool hit(ray const &ray, double t_min, double t_max, hit_record &rec) const override;
  virtual bool bounding_box(double time_0, double time_1, aabb &bounding_box) const override;
};

bvh_node::bvh_node(std::vector<std::shared_ptr<hittable>> &src_objects,
  size_t start,
  size_t end,
  double time_0,
  double time_1)
{
  auto objects = src_objects;

  auto axis = random_int(0, 2);
  auto comparator = box_x_compare;
  if (axis != 0) {
    if (axis == 1) {
      comparator = box_y_compare;
    } else {
      comparator = box_z_compare;
    }
  }

  size_t object_span = end - start;

  if (object_span == 1) {
    left_ = objects[start];
    right_ = objects[start];
  } else if (object_span == 2) {
    left_ = objects[start];
    right_ = objects[start + 1];
    if (!comparator(objects[start], objects[start + 1])) { swap(left_, right_); }
  } else {
    std::sort(std::next(objects.begin(), start), std::next(objects.begin(), end), comparator);

    auto mid = start + object_span / 2;
    left_ = make_shared<bvh_node>(objects, start, mid, time_0, time_1);
    right_ = make_shared<bvh_node>(objects, mid, end, time_0, time_1);
  }

  aabb left_bounding_box;
  aabb right_bounding_box;
  if (!left_->bounding_box(time_0, time_1, left_bounding_box)
      || !right_->bounding_box(time_0, time_1, right_bounding_box)) {
    std::cerr << "No bounding box in bvh_node constructor." << std::endl;
  }

  bounding_box_ = surrounding_box(left_bounding_box, right_bounding_box);
}

auto bvh_node::hit(ray const &ray, double t_min, double t_max, hit_record &rec) const -> bool
{
  if (!box.hit(r, t_min, t_max)) { return false; }

  bool hit_left = left->hit(ray, t_min, t_max, rec);
  bool hit_right = right->hit(ray, t_min, hit_left ? rec.t : t_max, rec);

  return hit_left || hit_right;
}

auto bvh_node::bounding_box(double /*time_0*/, double /*time_1*/, aabb &bounding_box) const -> bool
{
  bounding_box = bounding_box_;
  return true;
}

auto box_compare(std::shared_ptr<hittable> const lhs, std::shared_ptr<hittable> const rhs, int axis)
{
  aabb box_lhs;
  aabb box_rhs;

  if (!lhs->bounding_box(0, 0, box_lhs) || !rhs->bounding_box(0, 0, box_rhs)) {
    std::cerr << "No bounding box in bvh_node constructor." << std::endl;
  }

  return box_lhs.min()[axis] < box_rhs.max()[axis];
}

bool box_x_compare(std::shared_ptr<hittable> const lhs, std::shared_ptr<hittable> box_rhs)
{
  return box_compare(lhs, rhs, 0);
}

bool box_y_compare(std::shared_ptr<hittable> const lhs, std::shared_ptr<hittable> box_rhs)
{
  return box_compare(lhs, rhs, 1);
}

bool box_z_compare(std::shared_ptr<hittable> const lhs, std::shared_ptr<hittable> box_rhs)
{
  return box_compare(lhs, rhs, 2);
}

#endif
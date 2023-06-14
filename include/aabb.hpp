#ifndef AABB_HPP
#define AABB_HPP

#include "vec3.hpp"
#include "ray.hpp"
#include <algorithm>

class aabb
{
private:
  vec3 lower_bounds_;
  vec3 upper_bounds_;

public:
  aabb() {}
  aabb(vec3 const& lower_bounds, vec3 const& upper_bounds)
    : lower_bounds_{ lower_bounds }, upper_bounds_{ upper_bounds }
  {}

  auto min() const -> vec3 { return lower_bounds_; }

  auto max() const -> vec3 { return upper_bounds_; }

  auto hit(ray const& ray, double t_min, double t_max) const;
};

auto aabb::hit(ray const& ray, double t_min, double t_max) const
{
  for (auto i = 0; i < 3; ++i) {
    auto inverse_of_direction = 1.0f / ray.direction()[i];
    auto t_0 = (lower_bounds_[i] - ray.origin()[i]) * inverse_of_direction;
    auto t_1 = (upper_bounds_[i] - ray.origin()[i]) * inverse_of_direction;
    if (inverse_of_direction < 0.0f) { std::swap(t_0, t_1); }
    t_min = t_0 > t_min ? t_0 : t_min;
    t_max = t_1 < t_max ? t_1 : t_max;
    if (t_max <= t_min) { return false; }
  }
  return true;
}

auto surrounding_box(aabb const& box_0, aabb const& box_1) -> aabb
{
  vec3 small{ std::min(box_0.min().x(), box_1.min().x()),
    std::min(box_0.min().y(), box_1.min().y()),
    std::min(box_0.min().z(), box_1.min().z()) };

  vec3 big{ std::max(box_0.max().x(), box_1.max().x()),
    std::max(box_0.max().y(), box_1.max().y()),
    std::max(box_0.max().z(), box_1.max().z()) };
  return aabb{ small, big };
}

#endif
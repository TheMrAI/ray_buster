#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

class ray
{
private:
  vec3 origin_;
  vec3 direction_;
  double time_;

public:
  ray(){};
  ray(vec3 const& origin, vec3 const& direction, double time = 0.0)
    : origin_{ origin }, direction_{ direction }, time_{ time }
  {}

  auto origin() const -> vec3 const& { return origin_; }

  auto origin() -> vec3& { return origin_; }

  auto direction() const -> vec3 const& { return direction_; }

  auto direction() -> vec3& { return direction_; }

  auto time() const -> double { return time_; }

  auto at(double t) const -> vec3 { return origin_ + (t * direction_); }
};

#endif
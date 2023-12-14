#ifndef ORTHONORMAL_BASES_HPP
#define ORTHONORMAL_BASES_HPP

#include "vec3.hpp"
#include <array>
#include <cmath>

class onb
{
public:
  onb() = default;
  onb(vec3 const& n)
  {
    axis[2] = unit_vector(n);
    auto a = (std::fabs(axis[2].x()) > 0.9) ? vec3{ 0, 1, 0 } : vec3{ 1, 0, 0 };
    axis[1] = unit_vector(cross(axis[2], a));
    axis[0] = cross(axis[2], axis[1]);
  }

  auto operator[](int i) const { return axis[i]; }

  auto u() const { return axis[0]; }
  auto v() const { return axis[1]; }
  auto w() const { return axis[2]; }

  auto local(double a, double b, double c) const { return a * u() + b * v() + c * w(); }

  auto local(vec3 const& a) const { return a.x() * u() + a.y() * v() + a.z() * w(); }

private:
  std::array<vec3, 3> axis;
};

#endif
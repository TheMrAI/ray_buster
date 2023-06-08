#ifndef VEC3_HPP
#define VEC3_HPP

#include "util.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>

class vec3
{
private:
  std::array<double, 3> data_;

public:
  vec3() : data_{ 0, 0, 0 } {}
  vec3(double x, double y, double z) : data_{ x, y, z } {}

  auto x() const -> double { return data_[0]; }
  auto y() const -> double { return data_[1]; }
  auto z() const -> double { return data_[2]; }

  auto x() -> double & { return data_[0]; }
  auto y() -> double & { return data_[1]; }
  auto z() -> double & { return data_[2]; }

  auto operator-() const -> vec3 { return vec3{ -data_[0], -data_[1], -data_[2] }; }

  auto operator+=(vec3 const &rhs) -> vec3 &
  {
    data_[0] += rhs.data_[0];
    data_[1] += rhs.data_[1];
    data_[2] += rhs.data_[2];
    return *this;
  }

  auto operator-=(vec3 const &rhs) -> vec3 &
  {
    data_[0] -= rhs.data_[0];
    data_[1] -= rhs.data_[1];
    data_[2] -= rhs.data_[2];
    return *this;
  }

  auto operator*=(vec3 const &rhs) -> vec3 &
  {
    data_[0] *= rhs.data_[0];
    data_[1] *= rhs.data_[1];
    data_[2] *= rhs.data_[2];
    return *this;
  }

  auto operator*=(double const &t) -> vec3 &
  {
    data_[0] *= t;
    data_[1] *= t;
    data_[2] *= t;
    return *this;
  }

  auto operator/=(double const t) -> vec3 & { return *this *= 1 / t; }

  auto dot(vec3 const &rhs) const -> double
  {
    return data_[0] * rhs.data_[0] + data_[1] * rhs.data_[1] + data_[2] * rhs.data_[2];
  }

  auto length() const -> double { return std::sqrt(data_[0] * data_[0] + data_[1] * data_[1] + data_[2] * data_[2]); }

  friend auto operator<<(std::ostream &out, const vec3 &v) -> std::ostream &;

  static auto random() { return vec3{ random_double(), random_double(), random_double() }; }

  static auto random(double min, double max)
  {
    return vec3{ random_double(min, max), random_double(min, max), random_double(min, max) };
  }

  bool near_zero() const
  {
    // Return true if the vector is close to zero in all dimensions.
    constexpr auto s = 1e-8;
    return (fabs(data_[0]) < s) && (fabs(data_[1]) < s) && (fabs(data_[2] < s));
  }
};

auto operator<<(std::ostream &out, const vec3 &v) -> std::ostream &
{
  out << v.data_[0] << " " << v.data_[1] << " " << v.data_[2];
  return out;
}

auto operator+(vec3 const &lhs, vec3 const &rhs) -> vec3
{
  return vec3{ lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z() };
}

auto operator-(vec3 const &lhs, vec3 const &rhs) -> vec3
{
  return vec3{ lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z() };
}

auto operator*(vec3 const &lhs, vec3 const &rhs) -> vec3
{
  return vec3{ lhs.x() * rhs.x(), lhs.y() * rhs.y(), lhs.z() * rhs.z() };
}

auto operator*(double t, vec3 const &v) -> vec3 { return vec3{ v.x() * t, v.y() * t, v.z() * t }; }

auto operator*(vec3 const &v, double t) -> vec3 { return t * v; }

auto operator/(vec3 v, double t) -> vec3 { return (1.0 / t) * v; }

auto dot(vec3 lhs, vec3 const &rhs) -> double { return lhs.dot(rhs); }

auto cross(vec3 const &lhs, vec3 const &rhs) -> vec3
{
  return vec3{
    lhs.y() * rhs.z() - lhs.z() * rhs.y(), lhs.z() * rhs.x() - lhs.x() * rhs.z(), lhs.x() * rhs.y() - lhs.y() * rhs.x()
  };
}

auto unit_vector(vec3 v) -> vec3 { return v / v.length(); }

auto prepare_color(vec3 const &color, unsigned samples_per_pixel) -> vec3
{
  auto r = color.x();
  auto g = color.y();
  auto b = color.z();

  // Divide the color by the number of samples and gamma-correct for gamma=2.0
  r = sqrt(r / samples_per_pixel);
  g = sqrt(g / samples_per_pixel);
  b = sqrt(b / samples_per_pixel);

  return vec3{ 256 * std::clamp(r, 0.0, 0.999), 256 * std::clamp(g, 0.0, 0.999), 256 * std::clamp(b, 0.0, 0.999) };
}

auto write_color(std::ostream &out, vec3 prepared_color)
{
  out << static_cast<int>(prepared_color.x()) << ' ' << static_cast<int>(prepared_color.y()) << ' '
      << static_cast<int>(prepared_color.z()) << std::endl;
}

// nasty, but ok for now
auto write_color(std::ostream &out, vec3 color, unsigned samples_per_pixel)
{
  auto prepared_color = prepare_color(color, samples_per_pixel);
  write_color(out, prepared_color);
}

auto random_in_unit_sphere()
{
  while (true) {
    auto p = vec3::random(-1, 1);
    if (p.length() * p.length() >= 1) { continue; }
    return p;
  }
}

auto random_in_unit_disk()
{
  while (true) {
    auto p = vec3{ random_double(-1.0, 1.0), random_double(-1.0, 1.0), 0.0 };
    if (p.length() * p.length() >= 1.0) { continue; }
    return p;
  }
}

auto random_unit_vector() -> vec3 { return unit_vector(random_in_unit_sphere()); }

auto reflect(vec3 const &v, vec3 const &n) -> vec3 { return v - 2 * dot(v, n) * n; }

auto refract(vec3 const &uv, vec3 const &n, double etai_over_etat)
{
  auto cos_theta = fmin(dot(-uv, n), 1.0);
  auto r_out_perp = etai_over_etat * (uv + cos_theta * n);
  auto r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length() * r_out_perp.length())) * n;
  return r_out_perp + r_out_parallel;
}

#endif
#ifndef RAY_BUSTER_VEC3_HEADER
#define RAY_BUSTER_VEC3_HEADER

#include "lina.h"
#include <array>

namespace lina {

class Vec3
{
public:
  Vec3() : v_{ 0.0, 0.0, 0.0 } {}
  explicit Vec3(std::array<double, 3> v) : v_{ std::move(v) } {}
  Vec3(double x, double y, double z) : v_{ x, y, z } {}

  Vec3& operator+=(Vec3 const& rhs)
  {
    v_ = lina::add(v_, rhs.Components());
    return *this;
  }

  Vec3& operator-=(Vec3 const& rhs)
  {
    v_ = lina::sub(v_, rhs.Components());
    return *this;
  }

  Vec3& operator*=(double scalar)
  {
    v_ = lina::scale(scalar, v_);
    return *this;
  }

  Vec3& operator/=(double scalar)
  {
    v_ = lina::scale(1.0 / scalar, v_);
    return *this;
  }

  Vec3 operator-() const {
    return Vec3{-v_[0], -v_[1], -v_[2]};
  }

  double operator[](size_t i) const { return v_[i]; }

  double Length() const { return lina::length(v_); }

  std::array<double, 3> const& Components() const { return v_; }

private:
  std::array<double, 3> v_;
};

Vec3 operator+(Vec3 const& lhs, Vec3 const& rhs) { return Vec3{ lina::add(lhs.Components(), rhs.Components()) }; }

Vec3 operator-(Vec3 const& lhs, Vec3 const& rhs) { return Vec3{ lina::sub(lhs.Components(), rhs.Components()) }; }

Vec3 operator*(Vec3 const& lhs, double scalar) { return Vec3{ lina::scale(scalar, lhs.Components()) }; }

Vec3 operator*(double scalar, Vec3 const& lhs) { return Vec3{ lina::scale(scalar, lhs.Components()) }; }

Vec3 operator/(Vec3 const& lhs, double scalar) { return Vec3{ lina::scale(1.0 / scalar, lhs.Components()) }; }

double dot(Vec3 const& lhs, Vec3 const& rhs) { return lina::dot(lhs.Components(), rhs.Components()); }

Vec3 cross(Vec3 const& lhs, Vec3 const& rhs) { return Vec3{ lina::cross(lhs.Components(), rhs.Components()) }; }

Vec3 unit(Vec3 const& vector) { return Vec3{ lina::unit(vector.Components()) }; }

}// namespace lina


#endif
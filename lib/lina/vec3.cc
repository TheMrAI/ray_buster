#include "vec3.h"
#include "lina.h"
#include <array>
#include <cmath>
#include <cstddef>

namespace lina {

auto Vec3::operator+=(Vec3 const& rhs) -> Vec3&
{
  v_ = lina::add(v_, rhs.Components());
  return *this;
}

auto Vec3::operator-=(Vec3 const& rhs) -> Vec3&
{
  v_ = lina::sub(v_, rhs.Components());
  return *this;
}

auto Vec3::operator*=(double scalar) -> Vec3&
{
  v_ = lina::scale(scalar, v_);
  return *this;
}

auto Vec3::operator/=(double scalar) -> Vec3&
{
  v_ = lina::scale(1.0 / scalar, v_);
  return *this;
}

auto Vec3::operator-() const -> Vec3 { return Vec3{ -v_[0], -v_[1], -v_[2] }; }

auto Vec3::operator[](std::size_t i) const -> double { return v_.at(i); }

auto Vec3::Length() const -> double { return lina::length(v_); }

auto Vec3::Components() const -> std::array<double, 3> const& { return v_; }

auto operator+(Vec3 const& lhs, Vec3 const& rhs) -> Vec3
{
  return Vec3{ lina::add(lhs.Components(), rhs.Components()) };
}

auto operator-(Vec3 const& lhs, Vec3 const& rhs) -> Vec3
{
  return Vec3{ lina::sub(lhs.Components(), rhs.Components()) };
}

auto operator*(Vec3 const& lhs, Vec3 const& rhs) -> Vec3
{
  return Vec3{ lina::mul(lhs.Components(), rhs.Components()) };
}

auto operator*(Vec3 const& lhs, double scalar) -> Vec3 { return Vec3{ lina::scale(scalar, lhs.Components()) }; }

auto operator*(double scalar, Vec3 const& lhs) -> Vec3 { return Vec3{ lina::scale(scalar, lhs.Components()) }; }

auto operator/(Vec3 const& lhs, double scalar) -> Vec3 { return Vec3{ lina::scale(1.0 / scalar, lhs.Components()) }; }

auto dot(Vec3 const& lhs, Vec3 const& rhs) -> double { return lina::dot(lhs.Components(), rhs.Components()); }

auto cross(Vec3 const& lhs, Vec3 const& rhs) -> Vec3 { return Vec3{ lina::cross(lhs.Components(), rhs.Components()) }; }

auto unit(Vec3 const& vector) -> Vec3 { return Vec3{ lina::unit(vector.Components()) }; }

auto nearZero(Vec3 const& vector) -> bool
{
  constexpr auto epsilon = 1e-8;
  return std::fabs(vector[0]) < epsilon && std::fabs(vector[1]) < epsilon && std::fabs(vector[2]) < epsilon;
}

}// namespace lina

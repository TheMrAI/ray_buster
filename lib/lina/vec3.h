#ifndef RAY_BUSTER_LIB_LINA_VEC3_H_
#define RAY_BUSTER_LIB_LINA_VEC3_H_

#include <array>
#include <cstddef>

namespace lina {

class Vec3
{
public:
  constexpr Vec3() : v_{ 0.0, 0.0, 0.0 } {}
  constexpr explicit Vec3(std::array<double, 3> v) : v_{ v } {}
  constexpr Vec3(double x, double y, double z) : v_{ x, y, z } {}

  auto operator+=(Vec3 const& rhs) -> Vec3&;
  auto operator-=(Vec3 const& rhs) -> Vec3&;
  auto operator*=(double scalar) -> Vec3&;
  auto operator/=(double scalar) -> Vec3&;
  auto operator-() const -> Vec3;
  auto operator[](std::size_t i) const -> double;

  [[nodiscard]] auto Length() const -> double;
  [[nodiscard]] auto Components() const -> std::array<double, 3> const&;

private:
  std::array<double, 3> v_;
};

auto operator+(Vec3 const& lhs, Vec3 const& rhs) -> Vec3;
auto operator-(Vec3 const& lhs, Vec3 const& rhs) -> Vec3;
auto operator*(Vec3 const& lhs, Vec3 const& rhs) -> Vec3;
auto operator*(Vec3 const& lhs, double scalar) -> Vec3;
auto operator*(double scalar, Vec3 const& lhs) -> Vec3;
auto operator/(Vec3 const& lhs, double scalar) -> Vec3;

auto dot(Vec3 const& lhs, Vec3 const& rhs) -> double;
auto cross(Vec3 const& lhs, Vec3 const& rhs) -> Vec3;
auto unit(Vec3 const& vector) -> Vec3;

auto nearZero(Vec3 const& vector) -> bool;

}// namespace lina


#endif

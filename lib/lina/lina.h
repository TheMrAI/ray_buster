#ifndef RAY_BUSTER_VECTOR_HEADER
#define RAY_BUSTER_VECTOR_HEADER

#include <array>
#include <span>

// Additionally having a namespace name "lina" - linear algebra, just fills me with joy.
namespace lina {

auto add(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> std::array<double, 3>;
auto sub(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> std::array<double, 3>;
auto mul(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> std::array<double, 3>;
auto scale(std::span<double const, 3> const vector, double scalar) -> std::array<double, 3>;
auto scale(double scalar, std::span<double const, 3> const vector) -> std::array<double, 3>;
auto cross(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> std::array<double, 3>;
auto dot(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> double;
auto unit(std::span<double const, 3> const vector) -> std::array<double, 3>;
auto length(std::span<double const, 3> const vector) -> double;
auto length_squared(std::span<double const, 3> const vector) -> double;

}// namespace lina

#endif
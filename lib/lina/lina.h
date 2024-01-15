#ifndef RAY_BUSTER_LIB_LINA_LINA_H_
#define RAY_BUSTER_LIB_LINA_LINA_H_

#include <array>
#include <span>

// Additionally having a namespace name "lina" - linear algebra, just fills me with joy.
namespace lina {

// For 3 long vectors
auto add(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> std::array<double, 3>;
auto sub(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> std::array<double, 3>;
auto mul(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> std::array<double, 3>;
auto scale(std::span<double const, 3> const vector, double scalar) -> std::array<double, 3>;
auto scale(double scalar, std::span<double const, 3> const vector) -> std::array<double, 3>;
auto cross(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> std::array<double, 3>;
auto dot(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> double;
auto unit(std::span<double const, 3> const vector) -> std::array<double, 3>;
auto length(std::span<double const, 3> const vector) -> double;
auto lengthSquared(std::span<double const, 3> const vector) -> double;

// For 4 long vectors
auto dot(std::span<double const, 4> const lhs, std::span<double const, 4> const rhs) -> double;

// For 4x4 matrices and matrix, vector multiplications
// It would be preferable to use mdspan, but that is not yet available
auto mul(std::span<double const, 16> lhs, std::span<double const, 16> rhs) -> std::array<double, 16>;
auto mul(std::span<double const, 16> lhs, std::span<double const, 4> rhs) -> std::array<double, 4>;

}// namespace lina

#endif
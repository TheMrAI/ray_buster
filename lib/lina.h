#ifndef RAY_BUSTER_VECTOR_HEADER
#define RAY_BUSTER_VECTOR_HEADER

#include <array>
#include <span>

// Initially during the rewrite I wanted to just make a vector class myself.
// Then started thinking for a bit, looked into linear algebra a bit, then knowing
// that I wanted to do proper transformations and move/rotate/scale the objects into
// their places I have realized I don't need a vector class at all. A vector is just an array
// of values from the perspective of the data. Transformations on it are just vector/matrix
// multiplications. So what I really need is a simple set of linear algebra operations.
// The rest, like accessing the values of the vector, by axis name or renaming it to represent
// color is just crud and hides the true meaning.
// Additionally having a namespace name "lina" - linear algebra, just fills me with joy.
namespace lina {

auto add(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> std::array<double, 3>;
auto sub(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> std::array<double, 3>;
auto scale(std::span<double const, 3> const vector, double scalar) -> std::array<double, 3>;
auto scale(double scalar, std::span<double const, 3> const vector) -> std::array<double, 3>;
auto cross(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> std::array<double, 3>;
auto dot(std::span<double const, 3> const lhs, std::span<double const, 3> const rhs) -> double;
auto length(std::span<double const, 3> const vector) -> double;
auto length_squared(std::span<double const, 3> const vector) -> double;

}// namespace lina

#endif
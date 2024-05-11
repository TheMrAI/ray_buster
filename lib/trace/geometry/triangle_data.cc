#include "lib/trace/geometry/triangle_data.h"

#include "lib/lina/vec3.h"

#include <array>

namespace trace {

// lina::cross(u, v) is positive if u is to the right of v in space
// PS.: Every time I see this code I have to double check.
TriangleData::TriangleData(std::array<lina::Vec3, 3> const& vertices)
  : Q{ vertices[0] }, u{ vertices[2] - vertices[0] }, v{ vertices[1] - vertices[0] }, n{ lina::cross(u, v) },
    normal{ lina::unit(n) }, D{ lina::dot(normal, vertices[0]) }, common{ n / lina::dot(n, n) }
{}

}// namespace trace
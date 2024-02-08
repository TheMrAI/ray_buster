#include "cuboid.h"

#include "lib/lina/vec3.h"
#include "lib/trace/geometry/component.h"

#include <array>
#include <cstddef>
#include <vector>

namespace trace {

Cuboid::Cuboid(lina::Vec3 center, double width, double height, double depth)
  : Component{ center, std::vector<lina::Vec3>(8), std::vector<std::array<std::size_t, 3>>(12) }
{
  auto halfWidth = width / 2.0;
  auto halfHeight = height / 2.0;
  auto halfDepth = depth / 2.0;

  vertices_[0] = lina::Vec3{ -halfWidth, -halfDepth, -halfHeight };
  vertices_[1] = lina::Vec3{ -halfWidth, halfDepth, -halfHeight };
  vertices_[2] = lina::Vec3{ halfWidth, -halfDepth, -halfHeight };
  vertices_[3] = lina::Vec3{ halfWidth, halfDepth, -halfHeight };
  vertices_[4] = lina::Vec3{ -halfWidth, -halfDepth, halfHeight };
  vertices_[5] = lina::Vec3{ -halfWidth, halfDepth, halfHeight };
  vertices_[6] = lina::Vec3{ halfWidth, -halfDepth, halfHeight };
  vertices_[7] = lina::Vec3{ halfWidth, halfDepth, halfHeight };

  triangles_.at(0) = std::array<std::size_t, 3>{ 2, 3, 0 };
  triangles_.at(1) = std::array<std::size_t, 3>{ 0, 3, 1 };
  triangles_.at(2) = std::array<std::size_t, 3>{ 0, 4, 2 };
  triangles_.at(3) = std::array<std::size_t, 3>{ 2, 4, 6 };
  triangles_.at(4) = std::array<std::size_t, 3>{ 1, 5, 0 };
  triangles_.at(5) = std::array<std::size_t, 3>{ 0, 5, 4 };
  triangles_.at(6) = std::array<std::size_t, 3>{ 2, 6, 3 };
  triangles_.at(7) = std::array<std::size_t, 3>{ 3, 6, 7 };
  triangles_.at(8) = std::array<std::size_t, 3>{ 3, 7, 1 };
  triangles_.at(9) = std::array<std::size_t, 3>{ 1, 7, 5 };
  triangles_.at(10) = std::array<std::size_t, 3>{ 4, 5, 6 };
  triangles_.at(11) = std::array<std::size_t, 3>{ 6, 5, 7 };
}

}// namespace trace
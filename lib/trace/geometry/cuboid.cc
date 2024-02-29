#include "cuboid.h"

#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/geometry/component.h"
#include "lib/trace/geometry/mesh.h"
#include "lib/trace/geometry/triangle_data.h"
#include "lib/trace/geometry/vertex_data.h"
#include "lib/trace/transform.h"

#include <array>
#include <cmath>
#include <cstddef>
#include <format>
#include <stdexcept>
#include <vector>

namespace trace {

Cuboid::Cuboid()
  : Component{ Mesh{ lina::Vec3{ 0.0, 0.0, 0.0 },
    std::vector<lina::Vec3>(8),
    std::vector<VertexData>(),// no need
    std::vector<std::array<std::size_t, 3>>(12),
    std::vector<TriangleData>(12) } }
{
  auto unit = 1.0;

  mesh_.vertices[0] = lina::Vec3{ -unit, -unit, -unit };
  mesh_.vertices[1] = lina::Vec3{ -unit, unit, -unit };
  mesh_.vertices[2] = lina::Vec3{ unit, -unit, -unit };
  mesh_.vertices[3] = lina::Vec3{ unit, unit, -unit };
  mesh_.vertices[4] = lina::Vec3{ -unit, -unit, unit };
  mesh_.vertices[5] = lina::Vec3{ -unit, unit, unit };
  mesh_.vertices[6] = lina::Vec3{ unit, -unit, unit };
  mesh_.vertices[7] = lina::Vec3{ unit, unit, unit };

  mesh_.triangles.at(0) = std::array<std::size_t, 3>{ 2, 3, 0 };
  mesh_.triangles.at(1) = std::array<std::size_t, 3>{ 0, 3, 1 };
  mesh_.triangles.at(2) = std::array<std::size_t, 3>{ 0, 4, 2 };
  mesh_.triangles.at(3) = std::array<std::size_t, 3>{ 2, 4, 6 };
  mesh_.triangles.at(4) = std::array<std::size_t, 3>{ 1, 5, 0 };
  mesh_.triangles.at(5) = std::array<std::size_t, 3>{ 0, 5, 4 };
  mesh_.triangles.at(6) = std::array<std::size_t, 3>{ 2, 6, 3 };
  mesh_.triangles.at(7) = std::array<std::size_t, 3>{ 3, 6, 7 };
  mesh_.triangles.at(8) = std::array<std::size_t, 3>{ 3, 7, 1 };
  mesh_.triangles.at(9) = std::array<std::size_t, 3>{ 1, 7, 5 };
  mesh_.triangles.at(10) = std::array<std::size_t, 3>{ 4, 5, 6 };
  mesh_.triangles.at(11) = std::array<std::size_t, 3>{ 6, 5, 7 };
}

auto buildCuboid(lina::Vec3 center, double width, double depth, double height) -> Cuboid
{
  width = std::fabs(width);
  depth = std::fabs(depth);
  height = std::fabs(height);
  if (width < 0.00001 || depth < 0.00001 || height < 0.00001) {
    throw std::logic_error(
      std::format("Width, height and depth must be bigger than 0.0. Got: {}, {}, {}", width, depth, height));
  }

  auto cuboid = Cuboid{};

  auto transformation = trace::scale(lina::Vec3{ width / 2.0, depth / 2.0, height / 2.0 });
  transformation = lina::mul(translate(center), transformation);

  cuboid.Transform(transformation);
  return cuboid;
}

}// namespace trace
#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_MESH_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_MESH_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/triangle_data.h"
#include "lib/trace/geometry/vertex_data.h"
#include "lib/trace/ray.h"

#include <array>
#include <cstddef>
#include <limits>
#include <optional>
#include <vector>

namespace trace {

struct Mesh
{
  lina::Vec3 center;
  std::vector<lina::Vec3> vertices;
  std::vector<VertexData> vertexData;
  std::vector<std::array<std::size_t, 3>> triangles;
  std::vector<TriangleData> triangleData;
};

struct MeshCollision
{
  Collision collision;
  std::size_t triangleId = std::numeric_limits<std::size_t>::max();
  double distance = std::numeric_limits<double>::max();
  // Collision in Barycentric coordinates
  double alpha = 0.0;// weight for 2nd triangle point
  double beta = 0.0;// weight for 1st triangle point
  double gamma = 0.0;// weight for 0th triangle point
};

auto triangleCollide(Ray const& ray, std::vector<TriangleData> const& trianglesData, std::size_t triangleId)
  -> std::optional<MeshCollision>;

auto meshCollide(Ray const& ray,
  std::vector<std::array<std::size_t, 3>> const& triangles,
  std::vector<TriangleData> const& trianglesData) -> std::optional<MeshCollision>;

}// namespace trace

#endif
#include "sphere.h"

#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/geometry/component.h"
#include "lib/trace/transform.h"

#include <array>
#include <cmath>
#include <cstddef>
#include <format>
#include <span>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

namespace trace {

// Sphere tesselation based on article: https://danielsieger.com/blog/2021/01/03/generating-platonic-solids.html
// Very good article, but not enough to generate our icosahedron.
// For that I needed some extra help from:
// https://math.stackexchange.com/questions/2174594/co-ordinates-of-the-vertices-an-icosahedron-relative-to-its-centroid
Sphere::Sphere()
  : Component{ lina::Vec3{ 0.0, 0.0, 0.0 }, std::vector<lina::Vec3>(12), std::vector<std::array<std::size_t, 3>>(20), std::vector<TriangleData>(20) }
{
  auto phi = (1.0 + std::sqrt(5.0)) * 0.5;// golden ratio
  auto a = 1.0;

  // The initial "sphere" will only be an icosahedron
  // Vertexes described by the 3 mutually perpendicular rectangles.
  // For each vertex we take the unit vector, so final result can be inscribed into a unit sphere.
  vertices_.at(0) = lina::unit(lina::Vec3{ -a, 0.0, phi });
  vertices_.at(1) = lina::unit(lina::Vec3{ a, 0.0, phi });
  vertices_.at(2) = lina::unit(lina::Vec3{ -a, 0.0, -phi });
  vertices_.at(3) = lina::unit(lina::Vec3{ a, 0.0, -phi });
  vertices_.at(4) = lina::unit(lina::Vec3{ 0.0, phi, a });
  vertices_.at(5) = lina::unit(lina::Vec3{ 0.0, phi, -a });
  vertices_.at(6) = lina::unit(lina::Vec3{ 0.0, -phi, a });
  vertices_.at(7) = lina::unit(lina::Vec3{ 0.0, -phi, -a });
  vertices_.at(8) = lina::unit(lina::Vec3{ phi, a, 0.0 });
  vertices_.at(9) = lina::unit(lina::Vec3{ phi, -a, 0.0 });
  vertices_.at(10) = lina::unit(lina::Vec3{ -phi, a, 0.0 });
  vertices_.at(11) = lina::unit(lina::Vec3{ -phi, -a, 0.0 });

  // The order of the vertices matters! Always using a clockwise
  // order, such that by default the normal will point outwards
  // from the object.
  triangles_.at(0) = std::array<std::size_t, 3>{ 6, 0, 1 };
  triangles_.at(1) = std::array<std::size_t, 3>{ 4, 1, 0 };
  triangles_.at(2) = std::array<std::size_t, 3>{ 6, 1, 9 };
  triangles_.at(3) = std::array<std::size_t, 3>{ 8, 1, 4 };
  triangles_.at(4) = std::array<std::size_t, 3>{ 0, 6, 11 };
  triangles_.at(5) = std::array<std::size_t, 3>{ 0, 10, 4 };
  triangles_.at(6) = std::array<std::size_t, 3>{ 9, 1, 8 };
  triangles_.at(7) = std::array<std::size_t, 3>{ 10, 0, 11 };
  triangles_.at(8) = std::array<std::size_t, 3>{ 6, 9, 7 };
  triangles_.at(9) = std::array<std::size_t, 3>{ 11, 6, 7 };
  triangles_.at(10) = std::array<std::size_t, 3>{ 5, 8, 4 };
  triangles_.at(11) = std::array<std::size_t, 3>{ 5, 4, 10 };
  triangles_.at(12) = std::array<std::size_t, 3>{ 2, 7, 3 };
  triangles_.at(13) = std::array<std::size_t, 3>{ 3, 5, 2 };
  triangles_.at(14) = std::array<std::size_t, 3>{ 3, 9, 8 };
  triangles_.at(15) = std::array<std::size_t, 3>{ 2, 10, 11 };
  triangles_.at(16) = std::array<std::size_t, 3>{ 3, 7, 9 };
  triangles_.at(17) = std::array<std::size_t, 3>{ 5, 3, 8 };
  triangles_.at(18) = std::array<std::size_t, 3>{ 2, 11, 7 };
  triangles_.at(19) = std::array<std::size_t, 3>{ 2, 5, 10 };
}

auto makeId(std::size_t l, std::size_t r) -> std::pair<std::size_t, std::size_t>
{
  auto id = std::make_pair(l, r);
  if (l > r) { std::swap(id.first, id.second); }
  return id;
}

// This deeply saddens me. Why don't we have hashing functions in the standard for pairs?
// Never mind, for this, we will not pull in additional libraries as dependencies.
struct pairHash
{
  auto operator()(const std::pair<std::size_t, std::size_t>& pair) const -> std::size_t
  {
    return std::hash<std::size_t>{}(pair.first) ^ std::hash<std::size_t>{}(pair.second);
  }
};

auto insertOrRetrieve(std::pair<std::size_t, std::size_t> const& id,
  std::unordered_map<std::pair<std::size_t, std::size_t>, std::size_t, pairHash>& subEdgeVertexIds,
  std::vector<lina::Vec3>& updatedVertices)
{
  auto entry = subEdgeVertexIds.find(id);
  if (entry == subEdgeVertexIds.end()) {
    auto subVertex =
      ((updatedVertices.at(id.first) - updatedVertices.at(id.second)) / 2.0) + updatedVertices.at(id.second);
    updatedVertices.emplace_back(subVertex);
    auto result = subEdgeVertexIds.insert({ id, updatedVertices.size() - 1 });
    entry = result.first;
  }
  return entry->second;
}

// https://graphics.stanford.edu/~mdfisher/subdivision.html
auto loopSubdivision(std::vector<lina::Vec3> const& vertices, std::vector<std::array<std::size_t, 3>> const& triangles)
  -> std::pair<std::vector<lina::Vec3>, std::vector<std::array<std::size_t, 3>>>
{
  auto vertexProcessed = std::unordered_map<std::size_t, std::size_t>();
  // map old vertex id to new ones
  auto updatedVertices = std::vector<lina::Vec3>();
  auto updatedTriangles = std::vector<std::array<std::size_t, 3>>();

  // hold all the new vertices in a map, with their id in the updatedVertices vector
  // for the key, keep the ids in increasing order
  auto subEdgeVertexIds = std::unordered_map<std::pair<std::size_t, std::size_t>, std::size_t, pairHash>{};
  auto mappedVertexIds = std::array<std::size_t, 3>{};
  auto subVertexIds = std::array<std::size_t, 3>{};
  for (auto const& triangle : triangles) {
    // add vertices
    for (auto i = std::size_t{ 0 }; i < triangle.size(); i++) {
      auto processed = vertexProcessed.find(triangle.at(i));
      if (processed == vertexProcessed.end()) {
        updatedVertices.emplace_back(vertices.at(triangle.at(i)));
        auto result = vertexProcessed.insert({ triangle.at(i), updatedVertices.size() - 1 });
        processed = result.first;
      }
      mappedVertexIds.at(i) = processed->second;
    }
    // add sub vertices
    subVertexIds.at(0) =
      insertOrRetrieve(makeId(mappedVertexIds.at(0), mappedVertexIds.at(1)), subEdgeVertexIds, updatedVertices);
    subVertexIds.at(1) =
      insertOrRetrieve(makeId(mappedVertexIds.at(1), mappedVertexIds.at(2)), subEdgeVertexIds, updatedVertices);
    subVertexIds.at(2) =
      insertOrRetrieve(makeId(mappedVertexIds.at(2), mappedVertexIds.at(0)), subEdgeVertexIds, updatedVertices);
    // add triangles
    updatedTriangles.emplace_back(
      std::array<std::size_t, 3>{ mappedVertexIds.at(0), mappedVertexIds.at(1), mappedVertexIds.at(2) });
    updatedTriangles.emplace_back(
      std::array<std::size_t, 3>{ subVertexIds.at(2), mappedVertexIds.at(0), subVertexIds.at(0) });
    updatedTriangles.emplace_back(
      std::array<std::size_t, 3>{ subVertexIds.at(0), mappedVertexIds.at(1), subVertexIds.at(1) });
    updatedTriangles.emplace_back(
      std::array<std::size_t, 3>{ subVertexIds.at(1), mappedVertexIds.at(2), subVertexIds.at(2) });
    updatedTriangles.emplace_back(
      std::array<std::size_t, 3>{ subVertexIds.at(2), subVertexIds.at(0), subVertexIds.at(1) });
  }

  return std::make_pair(updatedVertices, updatedTriangles);
}

auto buildSphere(lina::Vec3 center, double radius, std::size_t subdivisionLevel) -> Sphere
{
  radius = std::fabs(radius);
  if (radius < 0.00001) { throw std::logic_error(std::format("Radius must be bigger than 0.00001. Got: {}", radius)); }

  auto sphere = Sphere{};
  for (auto i = std::size_t{ 0 }; i < subdivisionLevel; i++) {
    // subdivide
    auto [updatedVertices, updatedTriangles] = loopSubdivision(sphere.vertices_, sphere.triangles_);
    std::swap(sphere.vertices_, updatedVertices);
    std::swap(sphere.triangles_, updatedTriangles);
    // normalize vector length / or project them onto the unit sphere
    for (auto& vertex : sphere.vertices_) { vertex = lina::unit(vertex); }
  }

  auto transformMatrix = lina::mul(trace::translate(center), trace::scale(lina::Vec3{ radius, radius, radius }));
  sphere.trianglesData_ = std::vector<TriangleData>(sphere.triangles_.size());
  sphere.Transform(transformMatrix);

  return sphere;
}

}// namespace trace
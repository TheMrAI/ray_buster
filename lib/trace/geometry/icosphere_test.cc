#include "lib/trace/geometry/icosphere.h"

#include <gtest/gtest.h>

TEST(Icosahedron, TwelveVerticesAndTwentyTriangles)
{
  auto sphere = trace::buildIcosphere(lina::Vec3{ 0.0, 0.0, 0.0 }, 1.0, 0);
  auto const& mesh = sphere.GetMesh();

  EXPECT_EQ(mesh.vertices.size(), 12);
  EXPECT_EQ(mesh.vertexData.size(), 12);
  EXPECT_EQ(mesh.triangles.size(), 20);
  EXPECT_EQ(mesh.triangleData.size(), 20);
}

TEST(Icosphere, IcosahedronAfterOneSubdivision)
{
  // on the first subdivision the number of vertices has to become
  // mesh.vertices.size() + ((mesh.vertices.size() * 5) / 2), because each original vertex has 5 edges, and we place
  // a vertex on each edge, all of which we counted twice
  // the number of triangles simply has to be 4 times bigger than the previous number of triangles
  auto sphere = trace::buildIcosphere(lina::Vec3{ 0.0, 0.0, 0.0 }, 1.0, 1);
  auto const& mesh = sphere.GetMesh();

  EXPECT_EQ(mesh.vertices.size(), 42);
  EXPECT_EQ(mesh.vertexData.size(), 42);
  EXPECT_EQ(mesh.triangles.size(), 80);
  EXPECT_EQ(mesh.triangleData.size(), 80);
}

TEST(Icosphere, IcosahedronAfterTwoSubdivision)
{
  // on the second and subsequent subdivision the number of vertices has to become
  // mesh.vertices.size() + (mesh.vertices.size() * 5 / 2) for the 12 original vertices, as these
  // still and will continue to have only 5 edges, while for every other vertex the equation changes to:
  // subdivision_vertex_count = mesh.vertices.size() - 12
  // subdivision_vertex_count + (subdivision_vertex_count * 6 / 2)
  auto sphere = trace::buildIcosphere(lina::Vec3{ 0.0, 0.0, 0.0 }, 1.0, 2);
  auto const& mesh = sphere.GetMesh();

  EXPECT_EQ(mesh.vertices.size(), 162);
  EXPECT_EQ(mesh.vertexData.size(), 162);
  EXPECT_EQ(mesh.triangles.size(), 320);
  EXPECT_EQ(mesh.triangleData.size(), 320);
}

TEST(Icosphere, IcosahedronAfterThreeSubdivision)
{
  auto sphere = trace::buildIcosphere(lina::Vec3{ 0.0, 0.0, 0.0 }, 1.0, 3);
  auto const& mesh = sphere.GetMesh();

  EXPECT_EQ(mesh.vertices.size(), 642);
  EXPECT_EQ(mesh.vertexData.size(), 642);
  EXPECT_EQ(mesh.triangles.size(), 1280);
  EXPECT_EQ(mesh.triangleData.size(), 1280);
}
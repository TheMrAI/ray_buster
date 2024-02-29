#include "lib/lina/vec3.h"
#include "lib/trace/geometry/icosphere.h"
#include "lib/trace/geometry/mesh_limits.h"

#include <gtest/gtest.h>

TEST(buildIcosphere, DefaultIcosphereHasDimensionsOfTwo)
{
  auto icosphere = trace::buildIcosphere();
  ASSERT_DOUBLE_EQ(icosphere.GetMesh().center[0], 0.0);
  ASSERT_DOUBLE_EQ(icosphere.GetMesh().center[1], 0.0);
  ASSERT_DOUBLE_EQ(icosphere.GetMesh().center[2], 0.0);
  auto limits = meshLimits(icosphere.GetMesh());
  ASSERT_EQ(limits.minX, -1.0);
  ASSERT_EQ(limits.maxX, 1.0);
  ASSERT_EQ(limits.minY, -1.0);
  ASSERT_EQ(limits.maxY, 1.0);
  ASSERT_EQ(limits.minZ, -1.0);
  ASSERT_EQ(limits.maxZ, 1.0);
}

TEST(buildIcosphere, DefaultIcosphereHasBoundingBoxWithDimensionsOfTwo)
{
  auto icosphere = trace::buildIcosphere();
  auto const& boundingBox = icosphere.GetBoundingBox();
  ASSERT_DOUBLE_EQ(boundingBox.GetMesh().center[0], 0.0);
  ASSERT_DOUBLE_EQ(boundingBox.GetMesh().center[1], 0.0);
  ASSERT_DOUBLE_EQ(boundingBox.GetMesh().center[2], 0.0);
  auto limits = meshLimits(boundingBox.GetMesh());
  ASSERT_EQ(limits.minX, -1.0);
  ASSERT_EQ(limits.maxX, 1.0);
  ASSERT_EQ(limits.minY, -1.0);
  ASSERT_EQ(limits.maxY, 1.0);
  ASSERT_EQ(limits.minZ, -1.0);
  ASSERT_EQ(limits.maxZ, 1.0);
}

TEST(buildIcosphere, UnitIcosphereHasDimensionsOfOne)
{
  auto icosphere = trace::buildIcosphere(lina::Vec3{ 0.0, 0.0, 0.0 }, 1.0);
  ASSERT_DOUBLE_EQ(icosphere.GetMesh().center[0], 0.0);
  ASSERT_DOUBLE_EQ(icosphere.GetMesh().center[1], 0.0);
  ASSERT_DOUBLE_EQ(icosphere.GetMesh().center[2], 0.0);
  auto limits = meshLimits(icosphere.GetMesh());
  ASSERT_EQ(limits.minX, -0.5);
  ASSERT_EQ(limits.maxX, 0.5);
  ASSERT_EQ(limits.minY, -0.5);
  ASSERT_EQ(limits.maxY, 0.5);
  ASSERT_EQ(limits.minZ, -0.5);
  ASSERT_EQ(limits.maxZ, 0.5);
}

TEST(buildIcosphere, ScaledIcosphereWithShiftedCenter)
{
  auto icosphere = trace::buildIcosphere(lina::Vec3(3.3, 2.0, 1.0), 1.0);
  ASSERT_DOUBLE_EQ(icosphere.GetMesh().center[0], 3.3);
  ASSERT_DOUBLE_EQ(icosphere.GetMesh().center[1], 2.0);
  ASSERT_DOUBLE_EQ(icosphere.GetMesh().center[2], 1.0);
  auto limits = meshLimits(icosphere.GetMesh());
  ASSERT_EQ(limits.minX, -0.5);
  ASSERT_EQ(limits.maxX, 0.5);
  ASSERT_EQ(limits.minY, -0.5);
  ASSERT_EQ(limits.maxY, 0.5);
  ASSERT_EQ(limits.minZ, -0.5);
  ASSERT_EQ(limits.maxZ, 0.5);
}

TEST(buildIcosphere, IcosphereWithShiftedCenterHasShiftedBoundingBox)
{
  auto icosphere = trace::buildIcosphere(lina::Vec3(3.3, 2.0, 1.0), 1.0);
  auto const& boundingBox = icosphere.GetBoundingBox();
  ASSERT_DOUBLE_EQ(boundingBox.GetMesh().center[0], 3.3);
  ASSERT_DOUBLE_EQ(boundingBox.GetMesh().center[1], 2.0);
  ASSERT_DOUBLE_EQ(boundingBox.GetMesh().center[2], 1.0);
  auto limits = meshLimits(boundingBox.GetMesh());
  ASSERT_EQ(limits.minX, -0.5);
  ASSERT_EQ(limits.maxX, 0.5);
  ASSERT_EQ(limits.minY, -0.5);
  ASSERT_EQ(limits.maxY, 0.5);
  ASSERT_EQ(limits.minZ, -0.5);
  ASSERT_EQ(limits.maxZ, 0.5);
}

TEST(buildIcosphere, IcosahedronHasTwelveVerticesAndTwentyTriangles)
{
  auto icosphere = trace::buildIcosphere(lina::Vec3{ 0.0, 0.0, 0.0 }, 1.0, 0);
  auto const& mesh = icosphere.GetMesh();
  EXPECT_EQ(mesh.vertices.size(), 12);
  EXPECT_EQ(mesh.vertexData.size(), 12);
  EXPECT_EQ(mesh.triangles.size(), 20);
  EXPECT_EQ(mesh.triangleData.size(), 20);
}

TEST(buildIcosphere, IcosphereAfterOneSubdivision)
{
  // on the first subdivision the number of vertices has to become
  // mesh.vertices.size() + ((mesh.vertices.size() * 5) / 2), because each original vertex has 5 edges, and we place
  // a vertex on each edge, all of which we counted twice
  // the number of triangles simply has to be 4 times bigger than the previous number of triangles
  auto icosphere = trace::buildIcosphere(lina::Vec3{ 0.0, 0.0, 0.0 }, 1.0, 1);
  auto const& mesh = icosphere.GetMesh();
  EXPECT_EQ(mesh.vertices.size(), 42);
  EXPECT_EQ(mesh.vertexData.size(), 42);
  EXPECT_EQ(mesh.triangles.size(), 80);
  EXPECT_EQ(mesh.triangleData.size(), 80);
}

TEST(buildIcosphere, IcosphereAfterTwoSubdivision)
{
  // on the second and subsequent subdivision the number of vertices has to become
  // mesh.vertices.size() + (mesh.vertices.size() * 5 / 2) for the 12 original vertices, as these
  // still and will continue to have only 5 edges, while for every other vertex the equation changes to:
  // subdivision_vertex_count = mesh.vertices.size() - 12
  // subdivision_vertex_count + (subdivision_vertex_count * 6 / 2)
  auto icosphere = trace::buildIcosphere(lina::Vec3{ 0.0, 0.0, 0.0 }, 1.0, 2);
  auto const& mesh = icosphere.GetMesh();
  EXPECT_EQ(mesh.vertices.size(), 162);
  EXPECT_EQ(mesh.vertexData.size(), 162);
  EXPECT_EQ(mesh.triangles.size(), 320);
  EXPECT_EQ(mesh.triangleData.size(), 320);
}

TEST(buildIcosphere, IcosphereAfterThreeSubdivision)
{
  auto icosphere = trace::buildIcosphere(lina::Vec3{ 0.0, 0.0, 0.0 }, 1.0, 3);
  auto const& mesh = icosphere.GetMesh();
  EXPECT_EQ(mesh.vertices.size(), 642);
  EXPECT_EQ(mesh.vertexData.size(), 642);
  EXPECT_EQ(mesh.triangles.size(), 1280);
  EXPECT_EQ(mesh.triangleData.size(), 1280);
}

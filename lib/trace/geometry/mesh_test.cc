#include "lib/lina/vec3.h"
#include "lib/trace/geometry/mesh.h"
#include "lib/trace/geometry/triangle_data.h"

#include <array>
#include <gtest/gtest.h>

TEST(triangleVoxelCollisionTest, voxelAtOrigoTriangleOnPlaneXY)
{
  // One point at origo
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 1.0, 0.0, 0.0 }, lina::Vec3{ 0.0, 1.0, 0.0 }, lina::Vec3{ 0.0, 0.0, 0.0 }
    };
    EXPECT_TRUE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  // Triangle encompasses origo
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 1.0, 1.0, 0.0 }, lina::Vec3{ -1.0, 1.0, 0.0 }, lina::Vec3{ 0.0, -1.0, 0.0 }
    };
    EXPECT_TRUE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  // Triangle outside of voxel
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 0.51, 0.0, 0.0 }, lina::Vec3{ 0.51, 0.51, 0.0 }, lina::Vec3{ 1.0, 1.0, 0.0 }
    };
    EXPECT_FALSE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  // Triangle plane misses voxel
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 1.0, 0.0, 0.51 }, lina::Vec3{ 0.0, 1.0, 0.51 }, lina::Vec3{ 0.0, 0.0, 0.51 }
    };
    EXPECT_FALSE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 1.0, 0.0, -0.51 }, lina::Vec3{ 0.0, 1.0, -0.51 }, lina::Vec3{ 0.0, 0.0, -0.51 }
    };
    EXPECT_FALSE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
}

TEST(triangleVoxelCollisionTest, voxelAtOrigoTriangleOnPlaneXZ)
{
  // One point at origo
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 1.0, 0.0, 0.0 }, lina::Vec3{ 0.0, 0.0, 1.0 }, lina::Vec3{ 0.0, 0.0, 0.0 }
    };
    EXPECT_TRUE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  // Triangle encompasses origo
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 1.0, 0.0, 1.0 }, lina::Vec3{ -1.0, 0.0, 1.0 }, lina::Vec3{ 0.0, 0.0, -1.0 }
    };
    EXPECT_TRUE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  // Triangle outside of voxel
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 0.51, 0.0, 0.0 }, lina::Vec3{ 0.51, 0.0, 0.51 }, lina::Vec3{ 1.0, 0.0, 1.0 }
    };
    EXPECT_FALSE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  // Triangle plane misses voxel
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 1.0, 0.51, 0.0 }, lina::Vec3{ 0.0, 0.51, 1.0 }, lina::Vec3{ 0.0, 0.51, 0.0 }
    };
    EXPECT_FALSE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 1.0, -0.51, 0.0 }, lina::Vec3{ 0.0, -0.51, 1.0 }, lina::Vec3{ 0.0, -0.51, 0.0 }
    };
    EXPECT_FALSE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
}

TEST(triangleVoxelCollisionTest, voxelAtOrigoTriangleOnPlaneYZ)
{
  // One point at origo
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 0.0, 1.0, 0.0 }, lina::Vec3{ 0.0, 0.0, 1.0 }, lina::Vec3{ 0.0, 0.0, 0.0 }
    };
    EXPECT_TRUE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  // Triangle encompasses origo
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 0.0, 1.0, 1.0 }, lina::Vec3{ 0.0, -1.0, 1.0 }, lina::Vec3{ 0.0, 0.0, -1.0 }
    };
    EXPECT_TRUE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  // Triangle outside of voxel
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 0.0, 0.51, 0.0 }, lina::Vec3{ 0.0, 0.51, 0.51 }, lina::Vec3{ 0.0, 1.0, 1.0 }
    };
    EXPECT_FALSE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  // Triangle plane misses voxel
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 0.51, 1.0, 0.0 }, lina::Vec3{ 0.51, 0.0, 1.0 }, lina::Vec3{ 0.51, 0.0, 0.0 }
    };
    EXPECT_FALSE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ -0.51, 1.0, 0.0 }, lina::Vec3{ -0.51, 0.0, 1.0 }, lina::Vec3{ -0.51, 0.0, 0.0 }
    };
    EXPECT_FALSE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
}

TEST(triangleVoxelCollisionTest, voxelAtOrigoTrianglePockedThroughByOneVoxelVertex)
{
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 0.7, 0.0, 0.0 }, lina::Vec3{ 0.0, 0.7, 0.0 }, lina::Vec3{ 0.0, 0.0, 0.7 }
    };
    EXPECT_TRUE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  // In this configuration the triangle plane should just touch the vertex of the voxel if
  // each of its vertices is exactly 1.5 away on their respective axes.
  // still poking through
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 1.49, 0.0, 0.0 }, lina::Vec3{ 0.0, 1.49, 0.0 }, lina::Vec3{ 0.0, 0.0, 1.49 }
    };
    EXPECT_TRUE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  // not poking through
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 1.51, 0.0, 0.0 }, lina::Vec3{ 0.0, 1.51, 0.0 }, lina::Vec3{ 0.0, 0.0, 1.51 }
    };
    EXPECT_FALSE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  // far away
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ 3.0, 0.0, 0.0 }, lina::Vec3{ 0.0, 3.0, 0.0 }, lina::Vec3{ 0.0, 0.0, 3.0 }
    };
    EXPECT_FALSE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
}

TEST(triangleVoxelCollisionTest, voxelAtOrigoTrianglePockedThroughByOneVoxelVertexOppositeDirection)
{
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ -0.7, 0.0, 0.0 }, lina::Vec3{ 0.0, -0.7, 0.0 }, lina::Vec3{ 0.0, 0.0, -0.7 }
    };
    EXPECT_TRUE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  // still poking through
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ -1.49, 0.0, 0.0 }, lina::Vec3{ 0.0, -1.49, 0.0 }, lina::Vec3{ 0.0, 0.0, -1.49 }
    };
    EXPECT_TRUE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  // not poking through
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ -1.51, 0.0, 0.0 }, lina::Vec3{ 0.0, -1.51, 0.0 }, lina::Vec3{ 0.0, 0.0, -1.51 }
    };
    EXPECT_FALSE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
  // far away
  {
    auto vertices = std::array<lina::Vec3, 3>{
      lina::Vec3{ -3.0, 0.0, 0.0 }, lina::Vec3{ 0.0, -3.0, 0.0 }, lina::Vec3{ 0.0, 0.0, -3.0 }
    };
    EXPECT_FALSE(trace::triangleVoxelCollisionTest(1.0, trace::TriangleData{ vertices }));
  }
}
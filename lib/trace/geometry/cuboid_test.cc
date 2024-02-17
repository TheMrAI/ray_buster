#include "lib/lina/vec3.h"
#include "lib/trace/geometry/cuboid.h"
#include "lib/trace/geometry/mesh.h"
#include "lib/trace/geometry/mesh_limits.h"

#include <gtest/gtest.h>

TEST(buildCuboid, defaultBuildsCuboidWithTwoUnitLongDimensions)
{
  auto cuboid = trace::buildCuboid();
  ASSERT_DOUBLE_EQ(cuboid.GetMesh().center[0], 0.0);
  ASSERT_DOUBLE_EQ(cuboid.GetMesh().center[1], 0.0);
  ASSERT_DOUBLE_EQ(cuboid.GetMesh().center[2], 0.0);
  auto limits = meshLimits(cuboid.GetMesh());
  ASSERT_EQ(limits.minX, -1.0);
  ASSERT_EQ(limits.maxX, 1.0);
  ASSERT_EQ(limits.minY, -1.0);
  ASSERT_EQ(limits.maxY, 1.0);
  ASSERT_EQ(limits.minZ, -1.0);
  ASSERT_EQ(limits.maxZ, 1.0);
}

TEST(buildCuboid, cuboidWithUnitLongDimensions)
{
  auto cuboid = trace::buildCuboid(lina::Vec3(0.0, 0.0, 0.0), 1.0, 1.0, 1.0);
  ASSERT_DOUBLE_EQ(cuboid.GetMesh().center[0], 0.0);
  ASSERT_DOUBLE_EQ(cuboid.GetMesh().center[1], 0.0);
  ASSERT_DOUBLE_EQ(cuboid.GetMesh().center[2], 0.0);
  auto limits = meshLimits(cuboid.GetMesh());
  ASSERT_EQ(limits.minX, -0.5);
  ASSERT_EQ(limits.maxX, 0.5);
  ASSERT_EQ(limits.minY, -0.5);
  ASSERT_EQ(limits.maxY, 0.5);
  ASSERT_EQ(limits.minZ, -0.5);
  ASSERT_EQ(limits.maxZ, 0.5);
}

TEST(buildCuboid, unitWideCubeWithShiftedCenter)
{
  auto cuboid = trace::buildCuboid(lina::Vec3(3.3, 2.0, 1.0), 1.0, 1.0, 1.0);
  ASSERT_DOUBLE_EQ(cuboid.GetMesh().center[0], 3.3);
  ASSERT_DOUBLE_EQ(cuboid.GetMesh().center[1], 2.0);
  ASSERT_DOUBLE_EQ(cuboid.GetMesh().center[2], 1.0);
  auto limits = meshLimits(cuboid.GetMesh());
  ASSERT_EQ(limits.minX, -0.5);
  ASSERT_EQ(limits.maxX, 0.5);
  ASSERT_EQ(limits.minY, -0.5);
  ASSERT_EQ(limits.maxY, 0.5);
  ASSERT_EQ(limits.minZ, -0.5);
  ASSERT_EQ(limits.maxZ, 0.5);
}

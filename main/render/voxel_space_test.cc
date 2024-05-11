#include "lib/lina/vec3.h"
#include "lib/trace/geometry/mesh.h"
#include "lib/trace/geometry/plane.h"
#include "main/render/voxel_space.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

TEST(unitSizedVoxelSpace, halfUnitSquarePlaneAtOrigoPerpendicularToAxisZ)
{
  auto meshes = std::vector<trace::Mesh>{};
  // raised by 0.5 on the Z axis so we only get 4 voxels
  meshes.emplace_back(trace::buildPlane(lina::Vec3{ 0.5, 0.5, 0.5 }, 0.5, 0.5).GetMesh());

  auto voxelSpace = render::VoxelSpace{ meshes };
  auto const& voxelTriangles = voxelSpace.VoxelTriangles();

  // fits into exactly 1 voxel
  EXPECT_EQ(voxelTriangles.size(), 1);
  EXPECT_TRUE(voxelTriangles.contains(std::array<int64_t, 3>{ 0, 0, 0 }));

  auto const& ids = voxelTriangles.at(std::array<int64_t, 3>{ 0, 0, 0 });
  EXPECT_EQ(ids.size(), 2);
  EXPECT_TRUE(
    std::ranges::find_if(ids, [](auto const& id) { return id.triangle == 0; }) != ids.end());// has triangle id 0
  EXPECT_TRUE(std::ranges::find_if(ids, [](auto const& id) { return id.triangle == 1; }) != ids.end());
}

TEST(unitSizedVoxelSpace, unitSquarePlaneShiftedIntoPositiveFromOrigoPerpendicularToAxisZ)
{
  auto meshes = std::vector<trace::Mesh>{};
  // raised by 0.5 on the Z axis so we only get 4 voxels
  // slightly shifted in positive direction so that the boundary between triangles
  // doesn't go through the center
  meshes.emplace_back(trace::buildPlane(lina::Vec3{ 0.25, 0.25, 0.5 }, 1.0, 1.0).GetMesh());

  auto voxelSpace = render::VoxelSpace{ meshes };
  auto const& voxelTriangles = voxelSpace.VoxelTriangles();

  // fits into exactly 4 voxels around the origin
  EXPECT_EQ(voxelTriangles.size(), 4);
  EXPECT_TRUE(voxelTriangles.contains(std::array<int64_t, 3>{ -1, -1, 0 }));
  EXPECT_TRUE(voxelTriangles.contains(std::array<int64_t, 3>{ -1, 0, 0 }));
  EXPECT_TRUE(voxelTriangles.contains(std::array<int64_t, 3>{ 0, 0, 0 }));
  EXPECT_TRUE(voxelTriangles.contains(std::array<int64_t, 3>{ 0, -1, 0 }));

  // each voxel has the right triangles
  {
    auto const& ids = voxelTriangles.at(std::array<int64_t, 3>{ -1, -1, 0 });
    EXPECT_EQ(ids.size(), 1);
    EXPECT_TRUE(
      std::ranges::find_if(ids, [](auto const& id) { return id.triangle == 0; }) != ids.end());// has triangle id 0
  }
  {
    auto const& ids = voxelTriangles.at(std::array<int64_t, 3>{ -1, 0, 0 });
    EXPECT_EQ(ids.size(), 2);
    EXPECT_TRUE(std::ranges::find_if(ids, [](auto const& id) { return id.triangle == 0; }) != ids.end());
    EXPECT_TRUE(std::ranges::find_if(ids, [](auto const& id) { return id.triangle == 1; }) != ids.end());
  }
  {
    auto const& ids = voxelTriangles.at(std::array<int64_t, 3>{ 0, 0, 0 });
    EXPECT_EQ(ids.size(), 2);
    EXPECT_TRUE(std::ranges::find_if(ids, [](auto const& id) { return id.triangle == 0; }) != ids.end());
    EXPECT_TRUE(std::ranges::find_if(ids, [](auto const& id) { return id.triangle == 1; }) != ids.end());
  }
  {
    auto const& ids = voxelTriangles.at(std::array<int64_t, 3>{ 0, -1, 0 });
    EXPECT_EQ(ids.size(), 2);
    EXPECT_TRUE(std::ranges::find_if(ids, [](auto const& id) { return id.triangle == 0; }) != ids.end());
    EXPECT_TRUE(std::ranges::find_if(ids, [](auto const& id) { return id.triangle == 1; }) != ids.end());
  }
}
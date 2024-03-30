#include "main/render/voxel_space.h"

#include "lib/lina/vec3.h"
#include "lib/trace/geometry/aabb.h"
#include "lib/trace/geometry/mesh.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <span>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace render {

auto VoxelIdHash::operator()(std::array<int64_t, 3> const& voxelId) const -> std::size_t
{
  // from: https://www.boost.org/doc/libs/1_55_0/doc/html/hash/reference.html#boost.hash_combine
  auto seed = std::size_t{ 0 };
  // NOLINTBEGIN(hicpp-signed-bitwise)
  for (auto const& value : voxelId) { seed ^= std::hash<int64_t>()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2); }
  // NOLINTEND(hicpp-signed-bitwise)
  return seed;
}

auto IdHash::operator()(Id const& id) const -> std::size_t
{
  auto seed = std::size_t{ 0 };
  // NOLINTBEGIN(hicpp-signed-bitwise)
  seed ^= std::hash<size_t>()(id.object) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  seed ^= std::hash<size_t>()(id.triangle) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  // NOLINTEND(hicpp-signed-bitwise)
  return seed;
}

auto Id::operator==(Id const& rhs) const -> bool { return object == rhs.object && triangle == rhs.triangle; }

VoxelSpace::VoxelSpace(std::vector<trace::Mesh> const& meshes, double voxelDimension)
  : voxelDimension_{ voxelDimension }
{
  for (auto objectId = std::size_t{ 0 }; objectId < meshes.size(); objectId++) {
    auto const& mesh = meshes[objectId];
    for (auto triangleId = std::size_t{ 0 }; triangleId < mesh.triangleData.size(); triangleId++) {
      // Simply get the bounding box of the triangle, convert the limit values into voxel identifiers
      // on that dimension, and then just walk through the voxel matrix and check collision with the
      // triangle
      // Perhaps, not the most efficient algorithm, but very simple and good enough, as it only runs
      // once before rendering a frame.
      auto const& triangleData = mesh.triangleData[triangleId];
      auto const& triangleAabb = trace::triangleAabb(triangleData);
      auto const startVoxelIdX = doubleToVoxelId(triangleAabb.minX, voxelDimension_);
      auto const lastVoxelIdX = doubleToVoxelId(triangleAabb.maxX, voxelDimension_);
      auto const startVoxelIdY = doubleToVoxelId(triangleAabb.minY, voxelDimension_);
      auto const lastVoxelIdY = doubleToVoxelId(triangleAabb.maxY, voxelDimension_);
      auto const startVoxelIdZ = doubleToVoxelId(triangleAabb.minZ, voxelDimension_);
      auto const lastVoxelIdZ = doubleToVoxelId(triangleAabb.maxZ, voxelDimension_);

      aabb_.minVoxelIdX = std::min(aabb_.minVoxelIdX, startVoxelIdX);
      aabb_.maxVoxelIdX = std::max(aabb_.maxVoxelIdX, lastVoxelIdX);
      aabb_.minVoxelIdY = std::min(aabb_.minVoxelIdY, startVoxelIdY);
      aabb_.maxVoxelIdY = std::max(aabb_.maxVoxelIdY, lastVoxelIdY);
      aabb_.minVoxelIdZ = std::min(aabb_.minVoxelIdZ, startVoxelIdZ);
      aabb_.maxVoxelIdZ = std::max(aabb_.maxVoxelIdZ, lastVoxelIdZ);

      for (auto voxelZ = startVoxelIdZ; voxelZ <= lastVoxelIdZ; voxelZ++) {
        for (auto voxelX = startVoxelIdX; voxelX <= lastVoxelIdX; voxelX++) {
          for (auto voxelY = startVoxelIdY; voxelY <= lastVoxelIdY; voxelY++) {
            auto voxelCenter = lina::Vec3{ (static_cast<double>(voxelX) + 0.5) * voxelDimension_,
              (static_cast<double>(voxelY) + 0.5) * voxelDimension_,
              (static_cast<double>(voxelZ) + 0.5) * voxelDimension_ };
            auto voxelId = vec3ToVoxelId(voxelCenter.Components(), voxelDimension_);

            if (triangleVoxelCollide(voxelCenter, voxelDimension_, triangleData)) {
              voxelTriangles_[voxelId].emplace(objectId, triangleId);
            }
          }
        }
      }
    }
  }
}

auto VoxelSpace::trianglesInVoxelByPosition(std::span<double const, 3> position) const
  -> std::optional<std::unordered_set<Id, IdHash> const*>
{
  auto const voxelId = vec3ToVoxelId(position, voxelDimension_);
  return trianglesInVoxelById(voxelId);
}

auto VoxelSpace::trianglesInVoxelById(std::array<int64_t, 3> voxelId) const
  -> std::optional<std::unordered_set<Id, IdHash> const*>
{
  auto const entry = voxelTriangles_.find(voxelId);
  if (entry != voxelTriangles_.end()) {
    return std::optional<std::unordered_set<Id, IdHash> const*>{ &(entry->second) };
  }
  return std::optional<std::unordered_set<Id, IdHash> const*>{};
}

auto VoxelSpace::Dimension() const -> double { return voxelDimension_; }

auto VoxelSpace::IdAabb() const -> IdAABB const& { return aabb_; }

auto VoxelSpace::VoxelTriangles() const
  -> std::unordered_map<std::array<int64_t, 3>, std::unordered_set<Id, IdHash>, VoxelIdHash> const&
{
  return voxelTriangles_;
}

auto doubleToVoxelId(double value, double voxelDimension) -> int64_t { return std::floor(value / voxelDimension); }

auto vec3ToVoxelId(std::span<double const, 3> position, double voxelDimension) -> std::array<int64_t, 3>
{
  return { doubleToVoxelId(position[0], voxelDimension),
    doubleToVoxelId(position[1], voxelDimension),
    doubleToVoxelId(position[2], voxelDimension) };
}

}// namespace render
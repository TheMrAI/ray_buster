#ifndef RAY_BUSTER_MAIN_RENDER_VOXEL_SPACE_H_
#define RAY_BUSTER_MAIN_RENDER_VOXEL_SPACE_H_

#include "lib/trace/geometry/cuboid.h"
#include "lib/trace/geometry/mesh.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <span>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace render {

struct VoxelIdHash
{
  auto operator()(std::array<int64_t, 3> const& voxelId) const -> std::size_t;
};

struct Id
{
  size_t object;
  size_t triangle;

  auto operator==(Id const& rhs) const -> bool;
};

struct IdHash
{
  auto operator()(Id const& id) const -> std::size_t;
};

struct IdAABB
{
  int64_t minVoxelIdX = std::numeric_limits<int64_t>::max();
  int64_t maxVoxelIdX = std::numeric_limits<int64_t>::min();
  int64_t minVoxelIdY = std::numeric_limits<int64_t>::max();
  int64_t maxVoxelIdY = std::numeric_limits<int64_t>::min();
  int64_t minVoxelIdZ = std::numeric_limits<int64_t>::max();
  int64_t maxVoxelIdZ = std::numeric_limits<int64_t>::min();
};

class VoxelSpace
{
public:
  explicit VoxelSpace(std::vector<trace::Mesh> const& meshes, double voxelDimension = 1.0);
  VoxelSpace(VoxelSpace const&) = default;
  VoxelSpace(VoxelSpace&&) = default;
  auto operator=(VoxelSpace const&) -> VoxelSpace& = default;
  auto operator=(VoxelSpace&&) -> VoxelSpace& = default;
  ~VoxelSpace() = default;

  [[nodiscard]] auto trianglesInVoxelByPosition(std::span<double const, 3> position) const
    -> std::optional<std::unordered_set<Id, IdHash> const*>;
  [[nodiscard]] auto trianglesInVoxelById(std::array<int64_t, 3> voxelId) const
    -> std::optional<std::unordered_set<Id, IdHash> const*>;
  [[nodiscard]] auto Dimension() const -> double;
  [[nodiscard]] auto IdAabb() const -> IdAABB const&;
  [[nodiscard]] auto BoundingBox() const -> trace::Cuboid const&;

  [[nodiscard]] auto VoxelTriangles() const
    -> std::unordered_map<std::array<int64_t, 3>, std::unordered_set<Id, IdHash>, VoxelIdHash> const&;

private:
  std::unordered_map<std::array<int64_t, 3>, std::unordered_set<Id, IdHash>, VoxelIdHash> voxelTriangles_;
  double voxelDimension_;
  IdAABB aabb_;
  trace::Cuboid boundingBox_;
};

auto doubleToVoxelId(double value, double voxelDimension) -> int64_t;
auto vec3ToVoxelId(std::span<double const, 3> position, double voxelDimension) -> std::array<int64_t, 3>;

}// namespace render

#endif
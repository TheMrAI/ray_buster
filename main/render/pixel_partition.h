#ifndef RAY_BUSTER_MAIN_RENDER_PIXEL_PARTITION_H
#define RAY_BUSTER_MAIN_RENDER_PIXEL_PARTITION_H

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"
#include "main/render/voxel_space.h"
#include "main/scenes/scene.h"

#include <cstddef>
#include <optional>
#include <ostream>
#include <random>
#include <utility>
#include <vector>

namespace render {

auto closestCollision(trace::Ray const& ray,
  std::vector<scene::Element> const& sceneElements,
  render::VoxelSpace const& voxelSpace) -> std::pair<std::optional<trace::Collision>, std::size_t>;

auto rayColor(trace::Ray const& ray,
  std::vector<scene::Element> const& sceneElements,
  render::VoxelSpace const& voxelSpace,
  int masterLightIndex,
  std::mt19937& randomGenerator,
  std::size_t depth,
  bool useSkybox) -> lina::Vec3;

// applying gamma correction to the colors
auto linearToGamma(double LinearSpaceValue) -> double;

auto writeColor(lina::Vec3 const& color, std::ostream& outputStream) -> void;

auto linearPartition(scene::Composition sceneComposition, std::ostream& outputStream) -> void;

}// namespace render

#endif
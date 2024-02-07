#ifndef RAY_BUSTER_MAIN_RENDER_H
#define RAY_BUSTER_MAIN_RENDER_H

#include <optional>
#include <random>
#include <thread>
#include <utility>
#include <vector>

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "main/scenes/scene.h"

namespace render {

auto closestCollision(trace::Ray const& ray,
  std::vector<scene::Element> const& sceneElements) -> std::pair<std::optional<trace::Collision>, std::size_t>;

auto rayColor(trace::Ray const& ray,
  std::vector<scene::Element> const& sceneElements,
  int masterLightIndex,
  std::mt19937& randomGenerator,
  std::size_t depth,
  bool useSkybox) -> lina::Vec3;

// applying gamma correction to the colors
auto linearToGamma(double LinearSpaceValue) -> double;

auto writeColor(lina::Vec3 const& color) -> void;

auto linearPartition(scene::Composition sceneComposition) -> void;

}// namespace render

#endif
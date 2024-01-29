#include "main/scenes/test/plane.h"

#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/camera.h"
#include "lib/trace/component.h"
#include "lib/trace/geometry/plane.h"
#include "lib/trace/material/dielectric.h"
#include "lib/trace/material/emissive.h"
#include "lib/trace/material/lambertian.h"
#include "lib/trace/material/metal.h"
#include "lib/trace/transform.h"
#include "lib/trace/util.h"
#include "main/scenes/scene.h"
#include <expected>
#include <vector>

namespace scene::test {

constexpr auto imageWidth = std::size_t{ 1024 };
constexpr auto imageHeight = std::size_t{ 768 };
constexpr auto sampleCount = std::size_t{ 10 };
constexpr auto rayDepth = std::size_t{ 10 };

const auto camera = trace::Camera{ imageWidth,
  imageHeight,
  lina::Vec3{ 0.0, -3.0, 1.5 },// camera center
  lina::Vec3{ 0.0, 0.0, 1.5 },// look at
  lina::Vec3{ 0.0, 0.0, 1.0 },
  70.0,
  0.0,
  1.0 };

constexpr auto planeColor = lina::Vec3{ 0.75, 0.75, 0.75 };

auto planeEmissive() -> Composition
{
  // Have to override the sampleCount, otherwise there isn't enough
  // detail to see the light pattern.
  auto sampleCount = std::size_t{ 300 };

  auto camera = trace::Camera{ imageWidth,
    imageHeight,
    lina::Vec3{ 0.0, -3.0, 1.5 },// camera center
    lina::Vec3{ 0.0, 0.0, 1.5 },// look at
    lina::Vec3{ 0.0, 0.0, 1.0 },
    70.0,
    0.0,
    1.0 };

  auto sceneElements = std::vector<scene::Element>{};

  auto bottom = trace::build(lina::Vec3{ 0.0, 0.0, 0.0 }, 100.0, 100.0, trace::Axis::Z, trace::Orientation::Aligned);
  sceneElements.emplace_back(
    std::make_unique<trace::Plane>(std::move(bottom.value())), std::make_unique<trace::Lambertian>(planeColor));

  auto planeOne = trace::build(lina::Vec3{ 0.0, 1.0, 1.5 }, 3.0, 3.0, trace::Axis::X, trace::Orientation::Aligned);
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(planeOne.value())),
    std::make_unique<trace::Emissive>(lina::Vec3{ 3.0, 3.0, 3.0 }, false));

  auto planeTwo = trace::build(lina::Vec3{ 0.0, 5.0, 1.5 }, 3.0, 3.0, trace::Axis::X, trace::Orientation::Aligned);
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(planeTwo.value())),
    std::make_unique<trace::Emissive>(lina::Vec3{ 3.0, 3.0, 3.0 }, true));

  auto planeThree = trace::build(lina::Vec3{ 0.0, 9.0, 1.5 }, 3.0, 3.0, trace::Axis::X, trace::Orientation::Reverse);
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(planeThree.value())),
    std::make_unique<trace::Emissive>(lina::Vec3{ 3.0, 3.0, 3.0 }, true));

  return Composition{ camera, sampleCount, rayDepth, std::move(sceneElements), false };
}

}// namespace scene::test
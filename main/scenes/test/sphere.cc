#include "main/scenes/test/sphere.h"

#include "lib/lina/vec3.h"
#include "lib/trace/camera.h"
#include "lib/trace/geometry/plane.h"
#include "lib/trace/geometry/sphere.h"
#include "lib/trace/material/dielectric.h"
#include "lib/trace/material/emissive.h"
#include "lib/trace/material/lambertian.h"
#include "lib/trace/material/metal.h"
#include "main/scenes/scene.h"

#include <memory>
#include <utility>
#include <vector>

namespace scene::test {

constexpr auto planeColor = lina::Vec3{ 0.75, 0.75, 0.75 };
constexpr auto sphereColor = lina::Vec3{ 0.9296, 0.9179, 0.8476 };

auto sphereMaterial(RenderSettings const& settings) -> Composition
{
  auto const camera = trace::Camera{ settings.imageWidth,
    settings.imageHeight,
    lina::Vec3{ 0.0, -3.0, 1.5 },// camera center
    lina::Vec3{ 0.0, 0.0, 0.75 },// look at
    lina::Vec3{ 0.0, 0.0, 1.0 },
    settings.degreesVerticalFOV,
    settings.defocusAngle,
    settings.focusDistance };

  auto sceneElements = std::vector<scene::Element>{};

  auto bottom =
    trace::buildPlane(lina::Vec3{ 0.0, 0.0, 0.0 }, 100.0, 100.0, trace::Axis::Z, trace::Orientation::Aligned);
  sceneElements.emplace_back(
    std::make_unique<trace::Plane>(std::move(bottom)), std::make_unique<trace::Lambertian>(planeColor));

  auto sphereOneCenter = lina::Vec3{ -1.75, 0.75, 0.75 };
  auto sphereOne = std::make_unique<trace::Sphere>(trace::buildSphere(sphereOneCenter, 0.75, 1));
  sceneElements.emplace_back(std::move(sphereOne), std::make_unique<trace::Lambertian>(sphereColor));

  auto sphereTwoCenter = lina::Vec3{ -0.0, 0.75, 0.75 };
  auto sphereTwo = std::make_unique<trace::Sphere>(trace::buildSphere(sphereTwoCenter, 0.75, 1));
  sceneElements.emplace_back(std::move(sphereTwo), std::make_unique<trace::Dielectric>(1.4));

  auto sphereThreeCenter = lina::Vec3{ 1.75, 0.75, 0.75 };
  auto sphereThree = std::make_unique<trace::Sphere>(trace::buildSphere(sphereThreeCenter, 0.75, 1));
  sceneElements.emplace_back(std::move(sphereThree), std::make_unique<trace::Metal>(sphereColor, 0.01, 3));

  return Composition{ camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), -1, true };
}

auto sphereEmissive(RenderSettings const& settings) -> Composition
{
  auto const camera = trace::Camera{ settings.imageWidth,
    settings.imageHeight,
    lina::Vec3{ 0.0, -3.0, 1.5 },// camera center
    lina::Vec3{ 0.0, 0.0, 1.5 },// look at
    lina::Vec3{ 0.0, 0.0, 1.0 },
    settings.degreesVerticalFOV,
    settings.defocusAngle,
    settings.focusDistance };

  auto sceneElements = std::vector<scene::Element>{};

  auto bottom =
    trace::buildPlane(lina::Vec3{ 0.0, 0.0, 0.0 }, 100.0, 100.0, trace::Axis::Z, trace::Orientation::Aligned);
  sceneElements.emplace_back(
    std::make_unique<trace::Plane>(std::move(bottom)), std::make_unique<trace::Lambertian>(planeColor));

  auto sphereOneCenter = lina::Vec3{ 0.0, 1.5, 1.5 };
  auto sphereOne = std::make_unique<trace::Sphere>(trace::buildSphere(sphereOneCenter, 0.75, 1));
  sceneElements.emplace_back(std::move(sphereOne), std::make_unique<trace::Emissive>(lina::Vec3{ 3.0, 3.0, 3.0 }));

  return Composition{ camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), -1, false };
}

}// namespace scene::test
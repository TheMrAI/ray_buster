#include "main/scenes/collection/floating_sphere.h"

#include "lib/lina/vec3.h"
#include "lib/trace/camera.h"
#include "lib/trace/geometry/icosphere.h"
#include "lib/trace/geometry/plane.h"
#include "lib/trace/material/dielectric.h"
#include "lib/trace/material/emissive.h"
#include "lib/trace/material/lambertian.h"
#include "lib/trace/material/metal.h"
#include "main/scenes/scene.h"

#include <memory>
#include <utility>
#include <vector>

namespace scene {

auto floatingSphere(RenderSettings const& settings) -> Composition
{
  auto camera = trace::Camera{ settings.imageWidth,
    settings.imageHeight,
    lina::Vec3{ 0.0, -1900.0, 100.0 },// camera center
    lina::Vec3{ 0.0, 0.0, 800.0 },// look at
    lina::Vec3{ 0.0, 0.0, 1.0 },
    settings.degreesVerticalFOV,
    settings.defocusAngle,
    settings.focusDistance };

  auto sceneElements = std::vector<scene::Element>{};

  // plane
  auto plane =
    trace::buildPlane(lina::Vec3{ 0.0, 0.0, 0.0 }, 6000.0, 6000.0, trace::Axis::Z, trace::Orientation::Aligned);
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(plane)),
    std::make_unique<trace::Lambertian>(lina::Vec3{ 0.9296, 0.9179, 0.8476 }));

  // dome
  auto dome = trace::buildIcosphere(lina::Vec3{ 0.0, 0.0, -400.0 }, 6000.0, 5);
  sceneElements.emplace_back(
    std::make_unique<trace::Icosphere>(std::move(dome)), std::make_unique<trace::Metal>(lina::Vec3{ 1.0, 1.0, 1.0 }));

  // lights
  auto whiteLight = lina::Vec3{ 6.0, 6.0, 6.0 };
  auto redLight = lina::Vec3{ 6.0, 0.5, 0.5 };
  auto greenLight = lina::Vec3{ 0.5, 6.0, 0.5 };
  auto blueLight = lina::Vec3{ 0.5, 0.5, 6.0 };

  auto light =
    trace::buildPlane(lina::Vec3{ 0.0, 0.0, 0.1 }, 100.0, 100.0, trace::Axis::Z, trace::Orientation::Aligned);
  sceneElements.emplace_back(
    std::make_unique<trace::Plane>(std::move(light)), std::make_unique<trace::Emissive>(whiteLight, true));
  auto masterLightIndex = static_cast<int>(sceneElements.size() - 1);

  auto redStrip =
    trace::buildPlane(lina::Vec3{ 0.0, 1100.0, 0.1 }, 3000.0, 200.0, trace::Axis::Z, trace::Orientation::Aligned);
  sceneElements.emplace_back(
    std::make_unique<trace::Plane>(std::move(redStrip)), std::make_unique<trace::Emissive>(redLight, true));
  auto greenStrip =
    trace::buildPlane(lina::Vec3{ 0.0, 1300.0, 0.1 }, 3000.0, 200.0, trace::Axis::Z, trace::Orientation::Aligned);
  sceneElements.emplace_back(
    std::make_unique<trace::Plane>(std::move(greenStrip)), std::make_unique<trace::Emissive>(greenLight, true));
  auto blueStrip =
    trace::buildPlane(lina::Vec3{ 0.0, 1500.0, 0.1 }, 3000.0, 200.0, trace::Axis::Z, trace::Orientation::Aligned);
  sceneElements.emplace_back(
    std::make_unique<trace::Plane>(std::move(blueStrip)), std::make_unique<trace::Emissive>(blueLight, true));

  // sphere
  auto floatingSphere = trace::buildIcosphere(lina::Vec3{ 0.0, 0.0, 800.0 }, 1000, 4);
  sceneElements.emplace_back(
    std::make_unique<trace::Icosphere>(std::move(floatingSphere)), std::make_unique<trace::Dielectric>(1.5));

  return Composition{
    camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), masterLightIndex, false
  };
}
}// namespace scene
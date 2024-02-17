#include "lib/lina/vec3.h"
#include "lib/trace/camera.h"
#include "lib/trace/geometry/cuboid.h"
#include "lib/trace/geometry/plane.h"
#include "lib/trace/material/emissive.h"
#include "lib/trace/material/lambertian.h"
#include "lib/trace/transform.h"
#include "lib/trace/util.h"
#include "main/scenes/scene.h"

#include <memory>
#include <utility>
#include <vector>

namespace scene {

auto cornellBox(RenderSettings const& settings) -> Composition
{
  auto camera = trace::Camera{ settings.imageWidth,
    settings.imageHeight,
    lina::Vec3{ 0.0, -100.0, 50.0 },// camera center
    lina::Vec3{ 0.0, 0.0, 50.0 },// look at
    lina::Vec3{ 0.0, 0.0, 1.0 },
    settings.degreesVerticalFOV,
    settings.defocusAngle,
    settings.focusDistance };

  auto sceneElements = std::vector<scene::Element>{};

  auto bottom =
    trace::buildPlane(lina::Vec3{ 0.0, 0.0, 0.0 }, 100.0, 100.0, trace::Axis::Z, trace::Orientation::Aligned);
  auto top =
    trace::buildPlane(lina::Vec3{ 0.0, 0.0, 100.0 }, 100.0, 100.0, trace::Axis::Z, trace::Orientation::Reverse);
  auto back =
    trace::buildPlane(lina::Vec3{ 0.0, 50.0, 50.0 }, 100.0, 100.0, trace::Axis::Y, trace::Orientation::Reverse);
  auto left =
    trace::buildPlane(lina::Vec3{ -50.0, 0.0, 50.0 }, 100.0, 100.0, trace::Axis::X, trace::Orientation::Aligned);
  auto right =
    trace::buildPlane(lina::Vec3{ 50.0, 0.0, 50.0 }, 100.0, 100.0, trace::Axis::X, trace::Orientation::Reverse);

  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(bottom)),
    std::make_unique<trace::Lambertian>(lina::Vec3{ 0.9296, 0.9179, 0.8476 }));
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(top)),
    std::make_unique<trace::Lambertian>(lina::Vec3{ 0.9296, 0.9179, 0.8476 }));
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(back)),
    std::make_unique<trace::Lambertian>(lina::Vec3{ 0.07, 0.039, 0.5586 }));
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(left)),
    std::make_unique<trace::Lambertian>(lina::Vec3{ 0.12, 0.45, 0.15 }));
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(right)),
    std::make_unique<trace::Lambertian>(lina::Vec3{ 0.65, 0.05, 0.05 }));

  auto light = trace::buildPlane(lina::Vec3{ 0.0, 0.0, 99.9 }, 20.0, 20.0, trace::Axis::Z, trace::Orientation::Reverse);
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(light)),
    std::make_unique<trace::Emissive>(lina::Vec3{ 15.0, 15.0, 15.0 }, true));
  auto masterLightIndex = static_cast<int>(sceneElements.size() - 1);

  auto cuboidOne =
    std::make_unique<trace::Cuboid>(trace::buildCuboid(lina::Vec3{ -6.0, 30.0, 29.0 }, 28.0, 28.0, 58.0));
  cuboidOne->Transform(trace::rotateAlongZ(trace::degreesToRadians(30)));
  sceneElements.emplace_back(
    std::move(cuboidOne), std::make_unique<trace::Lambertian>(lina::Vec3{ 0.9296, 0.9179, 0.8476 }));

  auto cuboidTwo =
    std::make_unique<trace::Cuboid>(trace::buildCuboid(lina::Vec3{ 22.0, -8.0, 12.5 }, 25.0, 25.0, 25.0));
  cuboidTwo->Transform(trace::rotateAlongZ(trace::degreesToRadians(-8)));
  sceneElements.emplace_back(
    std::move(cuboidTwo), std::make_unique<trace::Lambertian>(lina::Vec3{ 0.9296, 0.9179, 0.8476 }));

  return Composition{
    camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), masterLightIndex, false
  };
}

}// namespace scene
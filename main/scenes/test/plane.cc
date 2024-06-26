#include "main/scenes/test/plane.h"

#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/camera.h"
#include "lib/trace/geometry/plane.h"
#include "lib/trace/material/dielectric.h"
#include "lib/trace/material/emissive.h"
#include "lib/trace/material/lambertian.h"
#include "lib/trace/material/metal.h"
#include "lib/trace/transform.h"
#include "lib/trace/util.h"
#include "main/scenes/scene.h"

#include <memory>
#include <utility>
#include <vector>

namespace scene::test {

constexpr auto planeColor = lina::Vec3{ 0.75, 0.75, 0.75 };

auto planeMaterial(RenderSettings const& settings) -> Composition
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

  auto planeOne =
    trace::buildPlane(lina::Vec3{ -2.0, 0.75, 0.75 }, 1.5, 1.5, trace::Axis::Z, trace::Orientation::Aligned);
  sceneElements.emplace_back(
    std::make_unique<trace::Plane>(std::move(planeOne)), std::make_unique<trace::Lambertian>(planeColor));

  auto planeTwo =
    trace::buildPlane(lina::Vec3{ -0.0, 0.75, 0.75 }, 1.5, 1.5, trace::Axis::Z, trace::Orientation::Aligned);
  sceneElements.emplace_back(
    std::make_unique<trace::Plane>(std::move(planeTwo)), std::make_unique<trace::Dielectric>(1.4));

  auto planeThree =
    trace::buildPlane(lina::Vec3{ 2.0, 0.75, 0.75 }, 1.5, 1.5, trace::Axis::Z, trace::Orientation::Aligned);
  sceneElements.emplace_back(
    std::make_unique<trace::Plane>(std::move(planeThree)), std::make_unique<trace::Metal>(planeColor, 0.01, 3));

  return Composition{ camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), -1, true };
}

auto planeScale(RenderSettings const& settings) -> Composition
{
  auto const camera = trace::Camera{ settings.imageWidth,
    settings.imageHeight,
    lina::Vec3{ 0.0, -4.0, 1.5 },// camera center
    lina::Vec3{ 0.0, 0.0, 1.5 },// look at
    lina::Vec3{ 0.0, 0.0, 1.0 },
    settings.degreesVerticalFOV,
    settings.defocusAngle,
    settings.focusDistance };

  auto sceneElements = std::vector<scene::Element>{};

  auto planeOne = std::make_unique<trace::Plane>(trace::buildPlane(lina::Vec3{ -2.0, 1.0, 0.5 }, 1.0, 2.0));
  sceneElements.emplace_back(std::move(planeOne), std::make_unique<trace::Lambertian>(planeColor));

  auto planeTwo = std::make_unique<trace::Plane>(trace::buildPlane(lina::Vec3{ 0.0, 0.5, 0.5 }, 2.0, 1.0));
  sceneElements.emplace_back(std::move(planeTwo), std::make_unique<trace::Lambertian>(planeColor));

  auto planeThree = std::make_unique<trace::Plane>(trace::buildPlane(lina::Vec3{ 2.0, 0.75, 0.5 }, 1.5, 1.5));
  sceneElements.emplace_back(std::move(planeThree), std::make_unique<trace::Lambertian>(planeColor));

  return Composition{ camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), -1, true };
}

auto planeRotate(RenderSettings const& settings) -> Composition
{
  auto const camera = trace::Camera{ settings.imageWidth,
    settings.imageHeight,
    lina::Vec3{ 0.0, -4.0, 1.5 },// camera center
    lina::Vec3{ 0.0, 0.0, 1.5 },// look at
    lina::Vec3{ 0.0, 0.0, 1.0 },
    settings.degreesVerticalFOV,
    settings.defocusAngle,
    settings.focusDistance };

  auto sceneElements = std::vector<scene::Element>{};

  auto planeOneCenter = lina::Vec3{ -2.0, 0.5, 0.5 };
  auto planeOne = std::make_unique<trace::Plane>(trace::buildPlane(planeOneCenter, 1.0, 1.0));
  planeOne->Transform(lina::mul(trace::translate(planeOneCenter),
    lina::mul(trace::rotateAlongZ(trace::degreesToRadians(30)), trace::translate(-planeOneCenter))));
  sceneElements.emplace_back(std::move(planeOne), std::make_unique<trace::Lambertian>(planeColor));

  auto planeTwoCenter = lina::Vec3{ -0.0, 0.5, 0.7 };
  auto planeTwo = std::make_unique<trace::Plane>(trace::buildPlane(planeTwoCenter, 1.0, 1.0));
  planeTwo->Transform(lina::mul(trace::translate(planeTwoCenter),
    lina::mul(trace::rotateAlongY(trace::degreesToRadians(30)), trace::translate(-planeTwoCenter))));
  sceneElements.emplace_back(std::move(planeTwo), std::make_unique<trace::Lambertian>(planeColor));

  auto planeThreeCenter = lina::Vec3{ 2.0, 0.5, 0.7 };
  auto planeThree = std::make_unique<trace::Plane>(trace::buildPlane(planeThreeCenter, 1.0, 1.0));
  planeThree->Transform(lina::mul(trace::translate(planeThreeCenter),
    lina::mul(trace::rotateAlongX(trace::degreesToRadians(30)), trace::translate(-planeThreeCenter))));
  sceneElements.emplace_back(std::move(planeThree), std::make_unique<trace::Lambertian>(planeColor));


  auto planeFourCenter = lina::Vec3{ -1.75, 0.75, 2.0 };
  auto planeFour = std::make_unique<trace::Plane>(trace::buildPlane(planeFourCenter, 1.0, 1.0));
  auto multiRotation = lina::mul(trace::rotateAlongX(trace::degreesToRadians(30)),
    lina::mul(trace::rotateAlongY(trace::degreesToRadians(30)), trace::rotateAlongZ(trace::degreesToRadians(30))));
  planeFour->Transform(
    lina::mul(trace::translate(planeFourCenter), lina::mul(multiRotation, trace::translate(-planeFourCenter))));
  sceneElements.emplace_back(std::move(planeFour), std::make_unique<trace::Lambertian>(planeColor));

  return Composition{ camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), -1, true };
}

auto planeEmissive(RenderSettings const& settings) -> Composition
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

  auto planeOne = trace::buildPlane(lina::Vec3{ 0.0, 1.0, 1.5 }, 3.0, 3.0, trace::Axis::X, trace::Orientation::Aligned);
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(planeOne)),
    std::make_unique<trace::Emissive>(lina::Vec3{ 3.0, 3.0, 3.0 }, false));
  auto masterLightIndex = static_cast<int>(sceneElements.size() - 1);

  auto planeTwo = trace::buildPlane(lina::Vec3{ 0.0, 5.0, 1.5 }, 3.0, 3.0, trace::Axis::X, trace::Orientation::Aligned);
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(planeTwo)),
    std::make_unique<trace::Emissive>(lina::Vec3{ 3.0, 3.0, 3.0 }, true));

  auto planeThree =
    trace::buildPlane(lina::Vec3{ 0.0, 9.0, 1.5 }, 3.0, 3.0, trace::Axis::X, trace::Orientation::Reverse);
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(planeThree)),
    std::make_unique<trace::Emissive>(lina::Vec3{ 3.0, 3.0, 3.0 }, true));

  return Composition{
    camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), masterLightIndex, false
  };
}

auto planeMaterialCollisionDirection(RenderSettings const& settings) -> Composition
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

  auto planeOne =
    trace::buildPlane(lina::Vec3{ -0.75, 0.0, 3.0 }, 1.0, 1.0, trace::Axis::Y, trace::Orientation::Reverse);
  sceneElements.emplace_back(
    std::make_unique<trace::Plane>(std::move(planeOne)), std::make_unique<trace::Lambertian>(planeColor));
  // the plane is reversed in the sense that it's front face is pointing away from us
  auto planeOneReversed =
    trace::buildPlane(lina::Vec3{ 0.75, 0.0, 3.0 }, 1.0, 1.0, trace::Axis::Y, trace::Orientation::Aligned);
  sceneElements.emplace_back(
    std::make_unique<trace::Plane>(std::move(planeOneReversed)), std::make_unique<trace::Lambertian>(planeColor));

  auto planeTwo =
    trace::buildPlane(lina::Vec3{ -0.75, 0.0, 1.5 }, 1.0, 1.0, trace::Axis::Y, trace::Orientation::Reverse);
  sceneElements.emplace_back(
    std::make_unique<trace::Plane>(std::move(planeTwo)), std::make_unique<trace::Metal>(planeColor, 0.01, 3));
  auto planeTwoReversed =
    trace::buildPlane(lina::Vec3{ 0.75, 0.0, 1.5 }, 1.0, 1.0, trace::Axis::Y, trace::Orientation::Aligned);
  sceneElements.emplace_back(
    std::make_unique<trace::Plane>(std::move(planeTwoReversed)), std::make_unique<trace::Metal>(planeColor, 0.01, 3));

  auto planeThree =
    trace::buildPlane(lina::Vec3{ -0.75, 0.0, 0.0 }, 1.0, 1.0, trace::Axis::Y, trace::Orientation::Reverse);
  sceneElements.emplace_back(
    std::make_unique<trace::Plane>(std::move(planeThree)), std::make_unique<trace::Dielectric>(1.4));
  auto planeThreeReversed =
    trace::buildPlane(lina::Vec3{ 0.75, 0.0, 0.0 }, 1.0, 1.0, trace::Axis::Y, trace::Orientation::Aligned);
  sceneElements.emplace_back(
    std::make_unique<trace::Plane>(std::move(planeThreeReversed)), std::make_unique<trace::Dielectric>(1.4));

  return Composition{ camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), -1, true };
}

}// namespace scene::test
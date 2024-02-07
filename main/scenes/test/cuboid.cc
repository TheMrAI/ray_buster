#include "main/scenes/test/cuboid.h"

#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/camera.h"
#include "lib/trace/geometry/cuboid.h"
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
constexpr auto cuboidColor = lina::Vec3{ 0.9296, 0.9179, 0.8476 };

auto cuboidMaterial(RenderSettings const& settings) -> Composition
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

  auto cuboidOneCenter = lina::Vec3{ -2.0, 0.75, 0.75 };
  auto cuboidOne = std::make_unique<trace::Cuboid>(cuboidOneCenter, 1.5, 1.5, 1.5);
  sceneElements.emplace_back(std::move(cuboidOne), std::make_unique<trace::Lambertian>(cuboidColor));

  auto cuboidTwoCenter = lina::Vec3{ -0.0, 0.75, 0.75 };
  auto cuboidTwo = std::make_unique<trace::Cuboid>(cuboidTwoCenter, 1.5, 1.5, 1.5);
  sceneElements.emplace_back(std::move(cuboidTwo), std::make_unique<trace::Dielectric>(1.4));

  auto cuboidThreeCenter = lina::Vec3{ 2.0, 0.75, 0.75 };
  auto cuboidThree = std::make_unique<trace::Cuboid>(cuboidThreeCenter, 1.5, 1.5, 1.5);
  sceneElements.emplace_back(std::move(cuboidThree), std::make_unique<trace::Metal>(cuboidColor, 0.01, 3));

  return Composition{ camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), -1, true };
}

auto cuboidScale(RenderSettings const& settings) -> Composition
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

  auto cuboidOneCenter = lina::Vec3{ -2.0, 1.0, 0.5 };
  auto cuboidOne = std::make_unique<trace::Cuboid>(cuboidOneCenter, 1.0, 1.0, 1.0);
  cuboidOne->Transform(lina::mul(trace::translate(cuboidOneCenter),
    lina::mul(trace::scale(lina::Vec3{ 1.0, 2.0, 1.0 }), trace::translate(-cuboidOneCenter))));
  sceneElements.emplace_back(std::move(cuboidOne), std::make_unique<trace::Lambertian>(cuboidColor));

  auto cuboidTwoCenter = lina::Vec3{ -0.0, 0.5, 0.5 };
  auto cuboidTwo = std::make_unique<trace::Cuboid>(cuboidTwoCenter, 1.0, 1.0, 1.0);
  cuboidTwo->Transform(lina::mul(trace::translate(cuboidTwoCenter),
    lina::mul(trace::scale(lina::Vec3{ 2.0, 1.0, 1.0 }), trace::translate(-cuboidTwoCenter))));
  sceneElements.emplace_back(std::move(cuboidTwo), std::make_unique<trace::Lambertian>(cuboidColor));

  auto cuboidThreeCenter = lina::Vec3{ 2.0, 0.5, 1.0 };
  auto cuboidThree = std::make_unique<trace::Cuboid>(cuboidThreeCenter, 1.0, 1.0, 1.0);
  cuboidThree->Transform(lina::mul(trace::translate(cuboidThreeCenter),
    lina::mul(trace::scale(lina::Vec3{ 1.0, 1.0, 2.0 }), trace::translate(-cuboidThreeCenter))));
  sceneElements.emplace_back(std::move(cuboidThree), std::make_unique<trace::Lambertian>(cuboidColor));

  auto cuboidFourCenter = lina::Vec3{ -1.75, 0.75, 2.0 };
  auto cuboidFour = std::make_unique<trace::Cuboid>(cuboidFourCenter, 1.0, 1.0, 1.0);
  cuboidFour->Transform(lina::mul(trace::translate(cuboidFourCenter),
    lina::mul(trace::scale(lina::Vec3{ 1.5, 1.5, 1.5 }), trace::translate(-cuboidFourCenter))));
  sceneElements.emplace_back(std::move(cuboidFour), std::make_unique<trace::Lambertian>(cuboidColor));

  return Composition{ camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), -1, true };
}

auto cuboidRotate(RenderSettings const& settings) -> Composition
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

  auto cuboidOneCenter = lina::Vec3{ -2.0, 0.5, 0.5 };
  auto cuboidOne = std::make_unique<trace::Cuboid>(cuboidOneCenter, 1.0, 1.0, 1.0);
  cuboidOne->Transform(lina::mul(trace::translate(cuboidOneCenter),
    lina::mul(trace::rotateAlongZ(trace::degreesToRadians(30)), trace::translate(-cuboidOneCenter))));
  sceneElements.emplace_back(std::move(cuboidOne), std::make_unique<trace::Lambertian>(cuboidColor));

  auto cuboidTwoCenter = lina::Vec3{ -0.0, 0.5, 0.7 };
  auto cuboidTwo = std::make_unique<trace::Cuboid>(cuboidTwoCenter, 1.0, 1.0, 1.0);
  cuboidTwo->Transform(lina::mul(trace::translate(cuboidTwoCenter),
    lina::mul(trace::rotateAlongY(trace::degreesToRadians(30)), trace::translate(-cuboidTwoCenter))));
  sceneElements.emplace_back(std::move(cuboidTwo), std::make_unique<trace::Lambertian>(cuboidColor));

  auto cuboidThreeCenter = lina::Vec3{ 2.0, 0.5, 0.7 };
  auto cuboidThree = std::make_unique<trace::Cuboid>(cuboidThreeCenter, 1.0, 1.0, 1.0);
  cuboidThree->Transform(lina::mul(trace::translate(cuboidThreeCenter),
    lina::mul(trace::rotateAlongX(trace::degreesToRadians(30)), trace::translate(-cuboidThreeCenter))));
  sceneElements.emplace_back(std::move(cuboidThree), std::make_unique<trace::Lambertian>(cuboidColor));

  auto cuboidFourCenter = lina::Vec3{ -1.75, 0.75, 2.0 };
  auto cuboidFour = std::make_unique<trace::Cuboid>(cuboidFourCenter, 1.0, 1.0, 1.0);
  auto multiRotation = lina::mul(trace::rotateAlongX(trace::degreesToRadians(30)),
    lina::mul(trace::rotateAlongY(trace::degreesToRadians(30)), trace::rotateAlongZ(trace::degreesToRadians(30))));
  cuboidFour->Transform(
    lina::mul(trace::translate(cuboidFourCenter), lina::mul(multiRotation, trace::translate(-cuboidFourCenter))));
  sceneElements.emplace_back(std::move(cuboidFour), std::make_unique<trace::Lambertian>(cuboidColor));

  return Composition{ camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), -1, true };
}

auto cuboidEmissive(RenderSettings const& settings) -> Composition
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

  auto cuboidOneCenter = lina::Vec3{ 0.0, 1.5, 1.5 };
  auto cuboidOne = std::make_unique<trace::Cuboid>(cuboidOneCenter, 1.5, 1.5, 1.5);
  sceneElements.emplace_back(std::move(cuboidOne), std::make_unique<trace::Emissive>(lina::Vec3{ 3.0, 3.0, 3.0 }));

  return Composition{ camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), -1, false };
}

}// namespace scene::test
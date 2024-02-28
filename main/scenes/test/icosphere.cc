#include "main/scenes/test/icosphere.h"

#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/camera.h"
#include "lib/trace/geometry/icosphere.h"
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
constexpr auto sphereColor = lina::Vec3{ 0.9296, 0.9179, 0.8476 };

auto icosphereMaterial(RenderSettings const& settings) -> Composition
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
  auto sphereOne = std::make_unique<trace::Icosphere>(trace::buildIcosphere(sphereOneCenter, 1.5, 3));
  sceneElements.emplace_back(std::move(sphereOne), std::make_unique<trace::Lambertian>(sphereColor));

  auto sphereTwoCenter = lina::Vec3{ -0.0, 0.75, 0.75 };
  auto sphereTwo = std::make_unique<trace::Icosphere>(trace::buildIcosphere(sphereTwoCenter, 1.5, 4));
  sceneElements.emplace_back(std::move(sphereTwo), std::make_unique<trace::Dielectric>(1.4));

  auto sphereThreeCenter = lina::Vec3{ 1.75, 0.75, 0.75 };
  auto sphereThree = std::make_unique<trace::Icosphere>(trace::buildIcosphere(sphereThreeCenter, 1.5, 3));
  sceneElements.emplace_back(std::move(sphereThree), std::make_unique<trace::Metal>(sphereColor, 0.01, 3));

  return Composition{ camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), -1, true };
}

auto icosphereScale(RenderSettings const& settings) -> Composition
{
  auto const camera = trace::Camera{ settings.imageWidth,
    settings.imageHeight,
    lina::Vec3{ 0.0, -3.8, 1.3 },// camera center
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

  auto sphereOneCenter = lina::Vec3{ -2.0, 1.0, 0.5 };
  auto sphereOne = std::make_unique<trace::Icosphere>(trace::buildIcosphere(sphereOneCenter, 1.0, 3));
  sphereOne->Transform(lina::mul(trace::translate(sphereOneCenter),
    lina::mul(trace::scale(lina::Vec3{ 1.0, 2.0, 1.0 }), trace::translate(-sphereOneCenter))));
  sceneElements.emplace_back(std::move(sphereOne), std::make_unique<trace::Lambertian>(sphereColor));

  auto sphereTwoCenter = lina::Vec3{ 0.0, 0.5, 0.5 };
  auto sphereTwo = std::make_unique<trace::Icosphere>(trace::buildIcosphere(sphereTwoCenter, 1.0, 3));
  sphereTwo->Transform(lina::mul(trace::translate(sphereTwoCenter),
    lina::mul(trace::scale(lina::Vec3{ 2.0, 1.0, 1.0 }), trace::translate(-sphereTwoCenter))));
  sceneElements.emplace_back(std::move(sphereTwo), std::make_unique<trace::Lambertian>(sphereColor));

  auto sphereThreeCenter = lina::Vec3{ 2.0, 0.5, 1.0 };
  auto sphereThree = std::make_unique<trace::Icosphere>(trace::buildIcosphere(sphereThreeCenter, 1.0, 3));
  sphereThree->Transform(lina::mul(trace::translate(sphereThreeCenter),
    lina::mul(trace::scale(lina::Vec3{ 1.0, 1.0, 2.0 }), trace::translate(-sphereThreeCenter))));
  sceneElements.emplace_back(std::move(sphereThree), std::make_unique<trace::Lambertian>(sphereColor));

  auto sphereFourCenter = lina::Vec3{ -2.0, 1.25, 2.0 };
  auto sphereFour = std::make_unique<trace::Icosphere>(trace::buildIcosphere(sphereFourCenter, 1.0, 3));
  sphereFour->Transform(lina::mul(trace::translate(sphereFourCenter),
    lina::mul(trace::scale(lina::Vec3{ 1.5, 1.5, 1.5 }), trace::translate(-sphereFourCenter))));
  sceneElements.emplace_back(std::move(sphereFour), std::make_unique<trace::Lambertian>(sphereColor));

  return Composition{ camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), -1, true };
}

auto icosphereRotate(RenderSettings const& settings) -> Composition
{
  auto const camera = trace::Camera{ settings.imageWidth,
    settings.imageHeight,
    lina::Vec3{ 0.0, -3.8, 1.3 },// camera center
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

  auto sphereOneCenter = lina::Vec3{ -2.0, 1.0, 0.5 };
  auto sphereOne = std::make_unique<trace::Icosphere>(trace::buildIcosphere(sphereOneCenter, 1.0, 3));
  sphereOne->Transform(lina::mul(trace::translate(sphereOneCenter),
    lina::mul(trace::rotateAlongZ(trace::degreesToRadians(-30)),
      lina::mul(trace::scale(lina::Vec3{ 1.0, 2.0, 1.0 }), trace::translate(-sphereOneCenter)))));
  sceneElements.emplace_back(std::move(sphereOne), std::make_unique<trace::Lambertian>(sphereColor));

  auto sphereTwoCenter = lina::Vec3{ 0.0, 0.5, 1.0 };
  auto sphereTwo = std::make_unique<trace::Icosphere>(trace::buildIcosphere(sphereTwoCenter, 1.0, 3));
  sphereTwo->Transform(lina::mul(trace::translate(sphereTwoCenter),
    lina::mul(trace::rotateAlongY(trace::degreesToRadians(30)),
      lina::mul(trace::scale(lina::Vec3{ 1.0, 1.0, 2.0 }), trace::translate(-sphereTwoCenter)))));
  sceneElements.emplace_back(std::move(sphereTwo), std::make_unique<trace::Lambertian>(sphereColor));

  auto sphereThreeCenter = lina::Vec3{ 2.0, 0.5, 1.0 };
  auto sphereThree = std::make_unique<trace::Icosphere>(trace::buildIcosphere(sphereThreeCenter, 1.0, 3));
  sphereThree->Transform(lina::mul(trace::translate(sphereThreeCenter),
    lina::mul(trace::rotateAlongX(trace::degreesToRadians(90)),
      lina::mul(trace::scale(lina::Vec3{ 1.0, 1.0, 2.0 }), trace::translate(-sphereThreeCenter)))));
  sceneElements.emplace_back(std::move(sphereThree), std::make_unique<trace::Lambertian>(sphereColor));

  return Composition{ camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), -1, true };
}

auto icosphereEmissive(RenderSettings const& settings) -> Composition
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
  auto sphereOne = std::make_unique<trace::Icosphere>(trace::buildIcosphere(sphereOneCenter, 1.5, 3));
  sceneElements.emplace_back(std::move(sphereOne), std::make_unique<trace::Emissive>(lina::Vec3{ 3.0, 3.0, 3.0 }));

  return Composition{ camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), -1, false };
}

auto icosphereInsideLambertian(RenderSettings const& settings) -> Composition
{
  auto const camera = trace::Camera{ settings.imageWidth,
    settings.imageHeight,
    lina::Vec3{ 0.0, -1.7, 0.0 },// camera center
    lina::Vec3{ 0.0, 0.0, 0.0 },// look at
    lina::Vec3{ 0.0, 0.0, 1.0 },
    settings.degreesVerticalFOV,
    settings.defocusAngle,
    settings.focusDistance };

  auto sceneElements = std::vector<scene::Element>{};

  auto icosphereOneCenter = lina::Vec3{ 0.0, 0.0, 0.0 };
  auto icosphereOne = std::make_unique<trace::Icosphere>(trace::buildIcosphere(icosphereOneCenter, 4.0, 1));
  sceneElements.emplace_back(std::move(icosphereOne), std::make_unique<trace::Lambertian>(sphereColor));

  auto backlight =
    trace::buildPlane(lina::Vec3{ 0.0, 0.0, -1.7 }, 4.0, 4.0, trace::Axis::Z, trace::Orientation::Aligned);
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(backlight)),
    std::make_unique<trace::Emissive>(lina::Vec3{ 1.0, 1.0, 1.0 }, false));
  auto masterLightIndex = static_cast<int>(sceneElements.size() - 1);

  return Composition{
    camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), masterLightIndex, false
  };
}

auto icosphereInsideMetal(RenderSettings const& settings) -> Composition
{
  auto const camera = trace::Camera{ settings.imageWidth,
    settings.imageHeight,
    lina::Vec3{ 0.0, -1.5, 0.0 },// camera center
    lina::Vec3{ 0.0, 0.0, 0.0 },// look at
    lina::Vec3{ 0.0, 0.0, 1.0 },
    settings.degreesVerticalFOV,
    settings.defocusAngle,
    settings.focusDistance };

  auto sceneElements = std::vector<scene::Element>{};

  auto icosphereOneCenter = lina::Vec3{ 0.0, 0.0, 0.0 };
  auto icosphereOne = std::make_unique<trace::Icosphere>(trace::buildIcosphere(icosphereOneCenter, 3.5, 4));
  sceneElements.emplace_back(std::move(icosphereOne), std::make_unique<trace::Metal>(sphereColor, 0.01, 1));

  auto backlight =
    trace::buildPlane(lina::Vec3{ 0.0, -1.6, 0.0 }, 3.0, 3.0, trace::Axis::Y, trace::Orientation::Aligned);
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(backlight)),
    std::make_unique<trace::Emissive>(lina::Vec3{ 0.3, 0.3, 0.3 }, true));
  auto masterLightIndex = static_cast<int>(sceneElements.size() - 1);

  return Composition{
    camera, settings.sampleCount, settings.rayDepth, std::move(sceneElements), masterLightIndex, false
  };
}

}// namespace scene::test
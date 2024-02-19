#include "main/scenes/scene_settings.h"

#include "main/scenes/collection/cornell_box.h"
#include "main/scenes/scene.h"
#include "main/scenes/test/cuboid.h"
#include "main/scenes/test/icosphere.h"
#include "main/scenes/test/plane.h"

#include <cstddef>
#include <format>
#include <map>
#include <string>

namespace scene {

auto configurations() -> std::map<std::string, Configuration>
{
  auto const defaultRenderSettings = scene::RenderSettings{
    std::size_t{ 600 },
    std::size_t{ 600 },
    std::size_t{ 10 },
    std::size_t{ 10 },
    "",
    70.0,
    0.0,
    1.0,
  };

  auto defaultConfigurations = std::map<std::string, Configuration>{
    { "cornell-box",
      Configuration{ "Standard cornell box. One overhead light in an enclosed cube, with two slightly rotated cuboids.",
        [](scene::RenderSettings const& settings) -> scene::Composition { return scene::cornellBox(settings); },
        scene::RenderSettings{
          std::size_t{ 800 },
          std::size_t{ 800 },
          std::size_t{ 100 },
          std::size_t{ 10 },
          "",
          70.0,
          0.0,
          1.0,
        } } },
    { "cornell-box-advanced",
      Configuration{ "A modified cornell box. One overhead light, one cuboid with a metal surface and a glass sphere.",
        [](scene::RenderSettings const& settings) -> scene::Composition { return scene::cornellBoxAdvanced(settings); },
        scene::RenderSettings{
          std::size_t{ 800 },
          std::size_t{ 800 },
          std::size_t{ 100 },
          std::size_t{ 10 },
          "",
          70.0,
          0.0,
          1.0,
        } } },
    { "test-cuboid-material",
      Configuration{ "3 cubes with lambertian, dielectric and metallic surfaces.",
        [](scene::RenderSettings const& settings) -> scene::Composition {
          return scene::test::cuboidMaterial(settings);
        },
        defaultRenderSettings } },
    { "test-cuboid-scale",
      Configuration{
        "4 initial cubes are scaled. 3 cubes are scaled along only one dimension while the fourth is scaled uniformly.",
        [](scene::RenderSettings const& settings) -> scene::Composition { return scene::test::cuboidScale(settings); },
        defaultRenderSettings } },
    { "test-cuboid-rotate",
      Configuration{
        "4 cubes are rotated. 3 cubes are rotated along only one axis while the fourth on all three at once.",
        [](scene::RenderSettings const& settings) -> scene::Composition { return scene::test::cuboidRotate(settings); },
        defaultRenderSettings } },
    { "test-cuboid-emissive",
      Configuration{ "1 cube with an emissive material.",
        [](scene::RenderSettings const& settings) -> scene::Composition {
          return scene::test::cuboidEmissive(settings);
        },
        defaultRenderSettings } },
    { "test-plane-material",
      Configuration{ "3 planes with lambertian, dielectric and metallic surfaces.",
        [](
          scene::RenderSettings const& settings) -> scene::Composition { return scene::test::planeMaterial(settings); },
        defaultRenderSettings } },
    { "test-plane-scale",
      Configuration{ "3 initial planes are scaled. 2 planes are scaled along only one dimension while the third is "
                     "scaled uniformly.",
        [](scene::RenderSettings const& settings) -> scene::Composition { return scene::test::planeScale(settings); },
        defaultRenderSettings } },
    { "test-plane-rotate",
      Configuration{
        "4 planes are rotated. 3 planes are rotated along only one axis while the fourth on all three at once.",
        [](scene::RenderSettings const& settings) -> scene::Composition { return scene::test::planeRotate(settings); },
        defaultRenderSettings } },
    { "test-plane-emissive",
      Configuration{
        "3 planes with emissive material. One is emissive in both directions while the other two only in one "
        "direction. The planes aren't visible as they are perfectly parallel to the viewing direction.",
        [](
          scene::RenderSettings const& settings) -> scene::Composition { return scene::test::planeEmissive(settings); },
        defaultRenderSettings } },
    { "test-icosphere-material",
      Configuration{ "3 icospheres with lambertian, dielectric and metallic surfaces.",
        [](scene::RenderSettings const& settings) -> scene::Composition {
          return scene::test::icosphereMaterial(settings);
        },
        defaultRenderSettings } },
    { "test-icosphere-scale",
      Configuration{ "4 initial icospheres are scaled. 3 icospheres are scaled along only one dimension while the "
                     "fourth is scaled uniformly.",
        [](scene::RenderSettings const& settings) -> scene::Composition {
          return scene::test::icosphereScale(settings);
        },
        defaultRenderSettings } },
    { "test-icosphere-rotate",
      Configuration{
        "3 icospheres are rotated along only one axis. The icospheres are stretched to make the rotation possible.",
        [](scene::RenderSettings const& settings) -> scene::Composition {
          return scene::test::icosphereRotate(settings);
        },
        defaultRenderSettings } },
    { "test-icosphere-emissive",
      Configuration{ "1 icosphere with an emissive material.",
        [](scene::RenderSettings const& settings) -> scene::Composition {
          return scene::test::icosphereEmissive(settings);
        },
        defaultRenderSettings } },
  };

  // fill in the default output targets
  for (auto& config : defaultConfigurations) {
    config.second.settings.outputFile = std::format("./{}.ppm", config.first);
  }

  return defaultConfigurations;
}

}// namespace scene
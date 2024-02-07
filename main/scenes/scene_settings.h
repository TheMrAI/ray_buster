#ifndef RAY_BUSTER_MAIN_SCENE_SETTINGS_H
#define RAY_BUSTER_MAIN_SCENE_SETTINGS_H

#include "main/scenes/collection/cornell_box.h"
#include "main/scenes/scene.h"
#include "main/scenes/test/cuboid.h"
#include "main/scenes/test/plane.h"
#include "main/scenes/test/sphere.h"

#include <map>
#include <string>

namespace scene {

struct Configuration
{
  std::string description;
  std::function<scene::Composition(RenderSettings)> sceneLoader;
  RenderSettings settings;
};

auto configurations() -> std::map<std::string, Configuration>;

}// namespace scene

#endif
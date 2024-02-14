#ifndef RAY_BUSTER_MAIN_SCENE_SETTINGS_H
#define RAY_BUSTER_MAIN_SCENE_SETTINGS_H

#include "main/scenes/scene.h"

#include <functional>
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
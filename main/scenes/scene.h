#ifndef RAY_BUSTER_MAIN_SCENE_H
#define RAY_BUSTER_MAIN_SCENE_H

#include "lib/trace/camera.h"
#include "lib/trace/component.h"
#include "lib/trace/material.h"
#include <memory>
#include <vector>

namespace scene {

struct Element
{
  std::unique_ptr<trace::Component> component;
  std::unique_ptr<trace::Material> material;
};

struct Composition
{
  trace::Camera camera;
  std::size_t sampleCount;
  std::size_t rayDepth;
  std::vector<Element> sceneElements;
  int masterLightIndex;// index of the light source that has been designated for PDF sampling, -1 means there is no
                       // designate master light
  bool useSkybox;
};

}// namespace scene


#endif
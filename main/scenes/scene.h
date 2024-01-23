#ifndef RAY_BUSTER_MAIN_SCENE_H
#define RAY_BUSTER_MAIN_SCENE_H

#include "lib/trace/collision.h"
#include "lib/trace/scattering.h"
#include <memory>

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
};

}// namespace scene


#endif
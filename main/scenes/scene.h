#ifndef RAY_BUSTER_MAIN_SCENE_H
#define RAY_BUSTER_MAIN_SCENE_H

#include "lib/trace/camera.h"
#include "lib/trace/geometry/component.h"
#include "lib/trace/material.h"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace scene {

struct Element
{
  std::unique_ptr<trace::Component> component;
  std::unique_ptr<trace::Material> material;

public:
  Element(std::unique_ptr<trace::Component>&& inputComponent, std::unique_ptr<trace::Material>&& material);
  Element(Element const&) = delete;
  Element(Element&&) = default;
  auto operator=(Element const&) -> Element& = delete;
  auto operator=(Element&&) -> Element& = delete;
  ~Element() = default;
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

public:
  Composition(trace::Camera inputCamera,
    std::size_t inputSampleCount,
    std::size_t inputRayDepth,
    std::vector<Element>&& inputSceneElements,
    int inputMasterLightIndex,
    bool inputUseSkybox);
  Composition(Composition const&) = delete;
  Composition(Composition&&) = default;
  auto operator=(Composition const&) -> Composition& = delete;
  auto operator=(Composition&&) -> Composition& = default;
  ~Composition() = default;
};

struct RenderSettings
{
  std::size_t imageWidth;
  std::size_t imageHeight;
  std::size_t sampleCount;
  std::size_t rayDepth;
  std::string outputFile;
  double degreesVerticalFOV;
  double defocusAngle;
  double focusDistance;
};

}// namespace scene


#endif
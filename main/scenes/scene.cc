#include "main/scenes/scene.h"
#include "lib/trace/camera.h"
#include "lib/trace/component.h"
#include "lib/trace/material.h"
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace scene {

Element::Element(std::unique_ptr<trace::Component>&& inputComponent, std::unique_ptr<trace::Material>&& material)
  : component{ std::move(inputComponent) }, material{ std::move(material) }
{}

Composition::Composition(trace::Camera inputCamera,
  std::size_t inputSampleCount,
  std::size_t inputRayDepth,
  std::vector<Element>&& inputSceneElements,
  int inputMasterLightIndex,
  bool inputUseSkybox)
  : camera{ inputCamera }, sampleCount{ inputSampleCount }, rayDepth{ inputRayDepth },
    sceneElements{ std::move(inputSceneElements) }, masterLightIndex{ inputMasterLightIndex },
    useSkybox{ inputUseSkybox }
{}

}// namespace scene
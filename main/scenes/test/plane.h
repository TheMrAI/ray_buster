#ifndef RAY_BUSTER_MAIN_SCENES_TEST_PLANE_H
#define RAY_BUSTER_MAIN_SCENES_TEST_PLANE_H

#include "main/scenes/scene.h"

namespace scene::test {

auto planeMaterial(RenderSettings const& settings) -> Composition;
auto planeScale(RenderSettings const& settings) -> Composition;
auto planeRotate(RenderSettings const& settings) -> Composition;
auto planeEmissive(RenderSettings const& settings) -> Composition;

}// namespace scene::test

#endif
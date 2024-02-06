#ifndef RAY_BUSTER_MAIN_SCENES_TEST_CUBOID_H
#define RAY_BUSTER_MAIN_SCENES_TEST_CUBOID_H

#include "main/scenes/scene.h"

namespace scene::test {

auto cuboidMaterial(RenderSettings const& settings) -> Composition;
auto cuboidScale(RenderSettings const& settings) -> Composition;
auto cuboidRotate(RenderSettings const& settings) -> Composition;
auto cuboidEmissive(RenderSettings const& settings) -> Composition;

}// namespace scene::test

#endif
#ifndef RAY_BUSTER_MAIN_SCENES_TEST_SPHERE_H
#define RAY_BUSTER_MAIN_SCENES_TEST_SPHERE_H

#include "main/scenes/scene.h"

namespace scene::test {

auto sphereMaterial(RenderSettings const& settings) -> Composition;
auto sphereScale(RenderSettings const& settings) -> Composition;
auto sphereRotate(RenderSettings const& settings) -> Composition;
auto sphereEmissive(RenderSettings const& settings) -> Composition;

}// namespace scene::test

#endif
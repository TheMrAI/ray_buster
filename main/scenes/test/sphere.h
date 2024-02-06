#ifndef RAY_BUSTER_MAIN_SCENES_TEST_SPHERE_H
#define RAY_BUSTER_MAIN_SCENES_TEST_SPHERE_H

#include "main/scenes/scene.h"

namespace scene::test {

auto sphereMaterial(RenderSettings const& settings) -> Composition;
// don't test scaling either, as it is no-op for now
// auto sphere_scale() -> Composition;
// we don't test sphere rotation
auto sphereEmissive(RenderSettings const& settings) -> Composition;

}// namespace scene::test

#endif
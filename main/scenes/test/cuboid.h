#ifndef RAY_BUSTER_MAIN_SCENES_TEST_CUBOID_H
#define RAY_BUSTER_MAIN_SCENES_TEST_CUBOID_H

#include "main/scenes/scene.h"

namespace scene::test {

auto cuboidMaterial() -> Composition;
auto cuboidScale() -> Composition;
auto cuboidRotate() -> Composition;
auto cuboidEmissive() -> Composition;

}// namespace scene::test

#endif
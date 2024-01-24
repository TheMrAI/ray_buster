#ifndef RAY_BUSTER_MAIN_SCENES_TEST_CUBOID_TRANSFORM_H
#define RAY_BUSTER_MAIN_SCENES_TEST_CUBOID_TRANSFORM_H

#include "main/scenes/scene.h"

namespace scene::test {

auto cuboid_material() -> Composition;
auto cuboid_scale() -> Composition;
auto cuboid_rotate() -> Composition;
auto cuboid_emissive() -> Composition;

}// namespace scene::test

#endif
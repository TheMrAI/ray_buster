#ifndef RAY_BUSTER_MAIN_SCENES_TEST_ICOSPHERE_H
#define RAY_BUSTER_MAIN_SCENES_TEST_ICOSPHERE_H

#include "main/scenes/scene.h"

namespace scene::test {

auto icosphereMaterial(RenderSettings const& settings) -> Composition;
auto icosphereScale(RenderSettings const& settings) -> Composition;
auto icosphereRotate(RenderSettings const& settings) -> Composition;
auto icosphereEmissive(RenderSettings const& settings) -> Composition;
auto icosphereInsideLambertian(RenderSettings const& settings) -> Composition;
auto icosphereInsideMetal(RenderSettings const& settings) -> Composition;

}// namespace scene::test

#endif
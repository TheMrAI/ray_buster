#ifndef RAY_BUSTER_MAIN_SCENES_COLLECTION_CORNELL_BOX_H
#define RAY_BUSTER_MAIN_SCENES_COLLECTION_CORNELL_BOX_H

#include "main/scenes/scene.h"

namespace scene {

auto cornellBox(RenderSettings const& settings) -> Composition;
auto cornellBoxAdvanced(RenderSettings const& settings) -> Composition;

}// namespace scene

#endif
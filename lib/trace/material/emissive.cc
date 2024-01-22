#include "emissive.h"

#include "lib/lina/vec3.h"

namespace trace {

Emissive::Emissive(lina::Vec3 color) : color_{ color } {}

auto Emissive::Emit() const -> lina::Vec3 { return color_; }

}// namespace trace
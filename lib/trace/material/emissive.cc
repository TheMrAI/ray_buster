#include "emissive.h"

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"

namespace trace {

Emissive::Emissive(lina::Vec3 color, bool directional) : color_{ color }, directional_{ directional } {}

auto Emissive::Emit(Collision const& collision) const -> lina::Vec3
{
  if (directional_ && !collision.frontFace) { return lina::Vec3{ 0.0, 0.0, 0.0 }; }
  return color_;
}

}// namespace trace
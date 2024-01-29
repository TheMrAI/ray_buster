#ifndef RAY_BUSTER_LIB_TRACE_MATERIAL_EMISSIVE_H_
#define RAY_BUSTER_LIB_TRACE_MATERIAL_EMISSIVE_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/material.h"

namespace trace {

class Emissive : public Material
{
public:
  Emissive(lina::Vec3 color, bool directional = false);

  auto Emit(Collision const& collision) const -> lina::Vec3 override;

private:
  lina::Vec3 color_;
  bool directional_;
};

}// namespace trace

#endif
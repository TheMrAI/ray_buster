#ifndef RAY_BUSTER_LIB_TRACE_COLLISION_H
#define RAY_BUSTER_LIB_TRACE_COLLISION_H

#include "lib/lina/vec3.h"

namespace trace {

struct Collision
{
  lina::Vec3 point;
  lina::Vec3 normal;
  bool frontFace = true;// which face the normal vector is on
  // this is an arbitrary property, front face could be either face, depends on the object itself
};

}// namespace trace

#endif
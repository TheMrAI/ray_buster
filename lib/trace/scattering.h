#ifndef RAY_BUSTER_LIB_TRACE_SCATTERING_H
#define RAY_BUSTER_LIB_TRACE_SCATTERING_H

#include "lib/lina/vec3.h"
#include "lib/trace/ray.h"

namespace trace {

struct Scattering
{
  Ray ray;
  lina::Vec3 attenuation;
};

}// namespace trace

#endif
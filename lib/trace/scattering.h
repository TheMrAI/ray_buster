#ifndef RAY_BUSTER_LIB_TRACE_SCATTERING_H
#define RAY_BUSTER_LIB_TRACE_SCATTERING_H

#include "lib/lina/vec3.h"
#include "lib/trace/pdf.h"
#include "lib/trace/ray.h"

#include <variant>

namespace trace {

struct Scattering
{
  lina::Vec3 attenuation;
  // In case of metals and dielectrics where the output ray has a near analytic solution, we skip the PDF sampling,
  // while for other materials PDF functions associated with the material instance will be provided.
  std::variant<Ray, PDF> type;
};

}// namespace trace

#endif
#ifndef RAY_BUSTER_LIB_TRACE_PDF_H_
#define RAY_BUSTER_LIB_TRACE_PDF_H_

#include "lib/lina/vec3.h"

#include <functional>

namespace trace {

struct PDF
{
  std::function<double(lina::Vec3 const&)> Evaluate;
  std::function<lina::Vec3()> GenerateSample;
  std::function<lina::Vec3()> AdjustedCollisionPoint;
};

}// namespace trace

#endif
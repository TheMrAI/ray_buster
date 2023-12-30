#ifndef RAY_BUSTER_LIB_TRACE_RAY_H_
#define RAY_BUSTER_LIB_TRACE_RAY_H_

#include "lib/lina/vec3.h"

namespace trace {

class Ray
{
public:
  Ray(lina::Vec3 source, lina::Vec3 direction) : source_{ source }, dir_{ direction } {};

  lina::Vec3 const& Source() const { return source_; }

  lina::Vec3 const& Direction() const { return dir_; }

private:
  lina::Vec3 source_;
  lina::Vec3 dir_;
};

}// namespace trace

#endif
#ifndef RAY_BUSTER_LIB_TRACE_RAY_H_
#define RAY_BUSTER_LIB_TRACE_RAY_H_

#include "lib/lina/vec3.h"

namespace trace {

class Ray
{
public:
  Ray();
  Ray(lina::Vec3 source, lina::Vec3 direction);

  auto Source() const -> lina::Vec3 const&;
  auto Direction() const -> lina::Vec3 const&;

private:
  lina::Vec3 source_;
  lina::Vec3 dir_;
};

}// namespace trace

#endif
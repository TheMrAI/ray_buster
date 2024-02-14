#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_TRIANGLE_DATA_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_TRIANGLE_DATA_H_

#include "lib/lina/vec3.h"

namespace trace {

struct TriangleData
{
  lina::Vec3 Q;// designated start vertex position of the triangle
  lina::Vec3 u;
  lina::Vec3 v;
  lina::Vec3 n;
  lina::Vec3 normal;// unit vector of n
  double D = 1.0;
  lina::Vec3 common;// common value used in alpha, beta calculation
};

}// namespace trace

#endif
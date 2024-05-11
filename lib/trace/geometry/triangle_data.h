#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_TRIANGLE_DATA_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_TRIANGLE_DATA_H_

#include "lib/lina/vec3.h"

#include <array>

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

  explicit TriangleData() = default;
  explicit TriangleData(std::array<lina::Vec3, 3> const& vertices);
  TriangleData(TriangleData const& rhs) = default;
  TriangleData(TriangleData&& rhs) = default;
  auto operator=(TriangleData const& rhs) -> TriangleData& = default;
  auto operator=(TriangleData&& rhs) -> TriangleData& = default;
  ~TriangleData() = default;
};

}// namespace trace

#endif
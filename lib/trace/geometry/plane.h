#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_PLANE_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_PLANE_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/component.h"
#include "lib/trace/pdf.h"
#include "lib/trace/ray.h"

#include <cstddef>
#include <optional>
#include <random>
#include <string>
#include <vector>

namespace trace {

enum class Axis { X, Y, Z };

enum class Orientation { Aligned, Reverse };

// A simple Plane, which is always lays parallel to the x, y axises, and the normal being on the
// z axis pointing in its positive direction. The reason behind these constraints is that for the time
// being I see no other practical way of defining a plane.
// This way the initialization is simple and we can still have all possible plane orientations by allowing
// its transformation.
class Plane : public Component
{
public:
  Plane(Plane const&) = default;
  Plane(Plane&&) = default;
  auto operator=(Plane const&) -> Plane& = default;
  auto operator=(Plane&&) -> Plane& = default;
  virtual ~Plane() = default;

  [[nodiscard]] auto SamplingPDF(std::mt19937& randomGenerator, lina::Vec3 const& from) const -> PDF override;
  friend auto
    buildPlane(lina::Vec3 center, double width, double depth, Axis normalAxis, Orientation orientation) -> Plane;

private:
  // The plane constructed plane will always have a size of 1.0 * 1.0.
  Plane();
};

// Build a plane conveniently oriented along any of the major axis.
[[nodiscard]] auto buildPlane(lina::Vec3 center = lina::Vec3{ 0.0, 0.0, 0.0 },
  double width = 1.0,
  double depth = 1.0,
  Axis normalAxis = Axis::Z,
  Orientation orientation = Orientation::Aligned) -> Plane;

}// namespace trace

#endif
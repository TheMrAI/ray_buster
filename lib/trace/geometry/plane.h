#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_PLANE_H_
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_PLANE_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/component.h"
#include "lib/trace/ray.h"
#include <expected>
#include <optional>
#include <string>

namespace trace {

enum class Axis { X, Y, Z };

enum class Orientation { Aligned, Reverse };

// A simple Plane, which is always lays parallel to the x, y axises, and the normal being on the
// z axis pointing in its positive direction. The reason behind these constraints is that for the time
// being I see no other practical way of defining a plane.
// This way the initialization is simple and we can still have all possible plane orientations by allowing
// its transformation. A feature not yet implemented.
class Plane : public Component
{
public:
  // Specifying 0.0 or smaller width/depth means that tha plane is infinite in that direction.
  Plane(lina::Vec3 center = lina::Vec3{ 0.0, 0.0, 0.0 }, double width = 0.0, double depth = 0.0);

  auto Collide(Ray const& ray) const -> std::optional<Collision> override;
  auto Transform(std::span<double const, 16> transformationMatrix) -> void override;

  friend auto build(lina::Vec3, double width, double depth, Axis normalAxis, Orientation orientation)
    -> std::expected<Plane, std::string>;

private:
  lina::Vec3 center_;
  lina::Vec3 normal_;
  double width_;
  double depth_;
  lina::Vec3 localU_;// localU is on the x axis and represents the width of the plane
  lina::Vec3 localV_;// localV is on the y axis and represents the depth of the plane
  double D_;
};

auto build(lina::Vec3 center = lina::Vec3{ 0.0, 0.0, 0.0 },
  double width = 0.0,
  double depth = 0.0,
  Axis normalAxis = Axis::Z,
  Orientation orientation = Orientation::Aligned) -> std::expected<Plane, std::string>;

}// namespace trace

#endif
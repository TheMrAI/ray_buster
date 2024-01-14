#ifndef RAY_BUSTER_COLLISION_HEADER
#define RAY_BUSTER_COLLISION_HEADER

#include "lib/lina/vec3.h"
#include "lib/trace/ray.h"
#include <optional>

namespace trace {

struct Collision
{
  lina::Vec3 point = lina::Vec3{};
  lina::Vec3 normal = lina::Vec3{};
  bool frontFace = true;// which face the normal vector is on
  // this is an arbitrary property, front face could be either face, depends on the object itself
};

class Component
{
public:
  virtual auto Collide(Ray const& ray) const -> std::optional<Collision> = 0;

  // Apply the linear transformation matrix to the object.
  virtual auto Transform(std::span<double const, 16> transformationMatrix) -> void = 0;
};

}// namespace trace

#endif
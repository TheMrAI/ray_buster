#ifndef RAY_BUSTER_LIB_TRACE_GEOMETRY_COMPONENT_HEADER
#define RAY_BUSTER_LIB_TRACE_GEOMETRY_COMPONENT_HEADER

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/mesh.h"
#include "lib/trace/pdf.h"
#include "lib/trace/ray.h"

#include <optional>
#include <random>
#include <span>

namespace trace {

class Component
{
public:
  Component() = default;
  explicit Component(Mesh mesh);
  Component(Component const&) = default;
  Component(Component&&) = default;
  auto operator=(Component const&) -> Component& = default;
  auto operator=(Component&&) -> Component& = default;
  virtual ~Component() = default;

  [[nodiscard]] virtual auto Collide(Ray const& ray) const -> std::optional<Collision>;
  // Apply the linear transformation matrix to the object.
  virtual auto Transform(std::span<double const, 16> transformationMatrix) -> void;

  // For a sampling PDF, AdjustedCollisionPoint function will not be implemented, because it makes no sense.
  // So just watch out, never to call it, until this PDF implementation could be replaced with something more
  // general.
  [[nodiscard]] virtual auto SamplingPDF(std::mt19937& randomGenerator, lina::Vec3 const& from) const -> PDF;

  [[nodiscard]] auto GetMesh() const -> Mesh const&;

protected:
  virtual auto updateTriangleData() -> void;

  Mesh mesh_;
};

}// namespace trace

#endif

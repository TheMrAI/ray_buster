#ifndef RAY_BUSTER_LIB_TRACE_COMPONENT_HEADER
#define RAY_BUSTER_LIB_TRACE_COMPONENT_HEADER

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
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
  Component(Component const&) = default;
  Component(Component&&) = default;
  auto operator=(Component const&) -> Component& = default;
  auto operator=(Component&&) -> Component& = default;
  virtual ~Component() = default;

  [[nodiscard]] virtual auto Collide(Ray const& ray) const -> std::optional<Collision> = 0;

  // Apply the linear transformation matrix to the object.
  virtual auto Transform(std::span<double const, 16> transformationMatrix) -> void = 0;

  [[nodiscard]] virtual auto SamplingPDF(std::mt19937& randomGenerator, lina::Vec3 const& from) const -> PDF = 0;
};

}// namespace trace

#endif

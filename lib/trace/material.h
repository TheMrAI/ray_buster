#ifndef RAY_BUSTER_LIB_TRACE_MATERIAL_H_
#define RAY_BUSTER_LIB_TRACE_MATERIAL_H_

#include "lib/lina/vec3.h"
#include "lib/trace/component.h"
#include "lib/trace/ray.h"
#include "lib/trace/scattering.h"
#include <optional>
#include <random>

namespace trace {

class Material
{
public:
  Material() = default;
  Material(Material const&) = default;
  Material(Material&&) = default;
  auto operator=(Material const&) -> Material& = default;
  auto operator=(Material&&) -> Material& = default;
  virtual ~Material() = default;

  [[nodiscard]] virtual auto Scatter(Ray const& /*ray*/,
    Collision const& /*collision*/,
    std::mt19937& /*randomGenerator*/) -> std::optional<Scattering>
  {
    return std::optional<Scattering>{};
  };

  [[nodiscard]] virtual auto Emit() const -> lina::Vec3 { return lina::Vec3{ 0.0, 0.0, 0.0 }; }
};

}// namespace trace

#endif

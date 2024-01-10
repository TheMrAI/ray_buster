#ifndef RAY_BUSTER_LIB_TRACE_SCATTERING_H_
#define RAY_BUSTER_LIB_TRACE_SCATTERING_H_

#include "collision.h"
#include "lib/lina/vec3.h"
#include "ray.h"
#include <optional>
#include <random>

namespace trace {

struct Scattering
{
  Ray ray;
  lina::Vec3 attenuation;
};

class Material
{
public:
  virtual auto Scatter(Ray const& ray, Collision const& collision, std::mt19937& randomGenerator)
    -> std::optional<Scattering>
  {
    return std::optional<Scattering>{};
  };

  virtual auto Emit() const -> lina::Vec3 { return lina::Vec3{ 0.0, 0.0, 0.0 }; }
};

}// namespace trace

#endif
#ifndef RAY_BUSTER_LIB_TRACE_MATERIAL_METAL_H_
#define RAY_BUSTER_LIB_TRACE_MATERIAL_METAL_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/material.h"
#include "lib/trace/ray.h"
#include "lib/trace/scattering.h"
#include <optional>
#include <random>

namespace trace {

class Metal : public Material
{
public:
  // Fuzz 0.0 means perfect reflections, while bigger/lower values increase the probability of the reflected ray
  // deviating from the perfect reflection. Fuzz can take on both positive/negative values, they always produce the
  // same effect.
  // Retry count describes how many times we wish to retry generating the fuzzed ray. It is important to keep
  // this number reasonably small, or the render could spend a lot of time while trying to generate very a ray in
  // very unlikely circumstances.
  Metal(lina::Vec3 albedo, double fuzz = 0.01, std::size_t retryCount = 3);

  auto Scatter(Ray const& ray,
    Collision const& collision,
    std::mt19937& randomGenerator) -> std::optional<Scattering> override;

private:
  lina::Vec3 albedo_;
  double fuzz_;
  std::size_t retryCount_;
};

}// namespace trace

#endif
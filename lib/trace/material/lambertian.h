#ifndef RAY_BUSTER_LIB_TRACE_MATERIAL_LAMBERTIAN_H_
#define RAY_BUSTER_LIB_TRACE_MATERIAL_LAMBERTIAN_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/material.h"
#include "lib/trace/ray.h"
#include "lib/trace/scattering.h"

#include <optional>
#include <random>

namespace trace {

class Lambertian : public Material
{
public:
  explicit Lambertian(lina::Vec3 albedo);

  auto Scatter(Ray const& ray,
    Collision const& collision,
    std::mt19937& randomGenerator) -> std::optional<Scattering> override;

private:
  lina::Vec3 albedo_;
};

}// namespace trace

#endif
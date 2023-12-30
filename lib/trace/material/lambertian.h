#ifndef RAY_BUSTER_LIB_TRACE_MATERIAL_LAMBERTIAN_H_
#define RAY_BUSTER_LIB_TRACE_MATERIAL_LAMBERTIAN_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"
#include "lib/trace/scattering.h"
#include "lib/trace/util.h"

namespace trace {

class Lambertian : public Material
{
public:
  Lambertian(lina::Vec3 albedo) : albedo_{ albedo } {}

  auto Scatter(Ray const& ray, Collision const& collision, std::mt19937& randomGenerator)
    -> std::optional<Scattering> override
  {
    auto scatterDirection = trace::randomOnUnitSphere(randomGenerator) + collision.normal;

    // Resolve edge case where the scatter direction is almost completely inverse to the normal
    // vector.
    if (lina::nearZero(scatterDirection)) { scatterDirection = collision.normal; }

    auto scattering = Scattering{};
    scattering.attenuation = albedo_;
    scattering.ray = trace::Ray{ collision.point, scatterDirection };

    return std::optional<Scattering>{ std::move(scattering) };
  }

private:
  lina::Vec3 albedo_;
};

}// namespace trace

#endif
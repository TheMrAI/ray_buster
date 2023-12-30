#ifndef RAY_BUSTER_LIB_TRACE_MATERIAL_METAL_H_
#define RAY_BUSTER_LIB_TRACE_MATERIAL_METAL_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"
#include "lib/trace/scattering.h"
#include "lib/trace/util.h"

namespace trace {

class Metal : public Material
{
public:
  Metal(lina::Vec3 albedo) : albedo_{ albedo } {}

  auto Scatter(Ray const& ray, Collision const& collision, std::mt19937& randomGenerator)
    -> std::optional<Scattering> override
  {
    auto reflectedDirection = ray.Direction() - 2.0 * lina::dot(ray.Direction(), collision.normal) * collision.normal;

    auto scattering = Scattering{};
    scattering.attenuation = albedo_;
    scattering.ray = trace::Ray{ collision.point, reflectedDirection };

    return std::optional<Scattering>{ std::move(scattering) };
  }

private:
  lina::Vec3 albedo_;
};

}// namespace trace

#endif
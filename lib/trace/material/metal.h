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
  // fuzz_radius 0.0 means reflections act as perfect
  Metal(lina::Vec3 albedo, double fuzz_radius = 0.0) : albedo_{ albedo }, fuzz_radius_{ fuzz_radius } {}

  auto Scatter(Ray const& ray, Collision const& collision, std::mt19937& randomGenerator)
    -> std::optional<Scattering> override
  {
    auto reflectedDirection = ray.Direction() - 2.0 * lina::dot(ray.Direction(), collision.normal) * collision.normal;
    auto scattered = Ray{ ray.Source(), reflectedDirection + randomOnUnitSphere(randomGenerator) };

    // The fuzz would break the ray back into the object, in this case we throw it away.
    // Not optimal, but works for now.
    if (lina::dot(scattered.Direction(), collision.normal) <= 0.0) { return std::optional<Scattering>{}; }

    auto adjustedCollisionPoint = collision.point + collision.normal * 0.00001;

    auto scattering = Scattering{};
    scattering.attenuation = albedo_;
    scattering.ray = trace::Ray{ adjustedCollisionPoint, reflectedDirection };

    return std::optional<Scattering>{ scattering };
  }

private:
  lina::Vec3 albedo_;
  double fuzz_radius_;
};

}// namespace trace

#endif
#include "lambertian.h"
#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"
#include "lib/trace/scattering.h"
#include "lib/trace/util.h"
#include <optional>
#include <random>

namespace trace {

Lambertian::Lambertian(lina::Vec3 albedo) : albedo_{ albedo } {}

auto Lambertian::Scatter(Ray const& /*ray*/, Collision const& collision, std::mt19937& randomGenerator)
  -> std::optional<Scattering>
{
  auto adjustedCollisionPoint = collision.point + collision.normal * 0.00001;
  // Make the normal a smidgen longer than unit length, so that even if the randomUnit would point
  // completely backwards, we wouldn't reach the object.
  // In terms of the Lambertian approximation, by making the normal longer, we modify the
  // non-uniform distribution to prefer angles closer to the normal. The change should be unnoticeable
  // in practice and we ensure a valid ray is always produced.
  auto scatterDirection = randomOnUnitSphere(randomGenerator) + collision.normal * 1.00001;

  auto scattering = Scattering{};
  scattering.attenuation = albedo_;
  scattering.ray = Ray{ adjustedCollisionPoint, scatterDirection };

  return std::optional<Scattering>{ scattering };
}
}// namespace trace
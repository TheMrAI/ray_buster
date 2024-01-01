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

auto Lambertian::Scatter(Ray const& ray, Collision const& collision, std::mt19937& randomGenerator)
  -> std::optional<Scattering>
{
  auto scatterDirection = randomOnUnitSphere(randomGenerator) + collision.normal;

  // Resolve edge case where the scatter direction is almost completely inverse to the normal
  // vector.
  if (lina::nearZero(scatterDirection)) { scatterDirection = collision.normal; }

  auto adjustedCollisionPoint = collision.point + collision.normal * 0.00001;

  auto scattering = Scattering{};
  scattering.attenuation = albedo_;
  scattering.ray = Ray{ adjustedCollisionPoint, scatterDirection };

  return std::optional<Scattering>{ scattering };
}
}// namespace trace
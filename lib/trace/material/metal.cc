#include "metal.h"
#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"
#include "lib/trace/scattering.h"
#include "lib/trace/util.h"
#include <optional>
#include <random>

namespace trace {

Metal::Metal(lina::Vec3 albedo, double fuzz_radius) : albedo_{ albedo }, fuzz_radius_{ fuzz_radius } {}

auto Metal::Scatter(Ray const& ray, Collision const& collision, std::mt19937& randomGenerator)
  -> std::optional<Scattering>
{
  auto reflectedDirection = ray.Direction() - 2.0 * lina::dot(ray.Direction(), collision.normal) * collision.normal;
  auto scattered = Ray{ ray.Source(), reflectedDirection + randomOnUnitSphere(randomGenerator) };

  // The fuzz would break the ray back into the object, in this case we throw it away.
  // Not optimal, but works for now.
  if (lina::dot(scattered.Direction(), collision.normal) <= 0.0) { return std::optional<Scattering>{}; }

  auto adjustedCollisionPoint = collision.point + collision.normal * 0.00001;

  auto scattering = Scattering{};
  scattering.attenuation = albedo_;
  scattering.ray = Ray{ adjustedCollisionPoint, reflectedDirection };

  return std::optional<Scattering>{ scattering };
}

}// namespace trace
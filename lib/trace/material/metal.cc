#include "metal.h"

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"
#include "lib/trace/scattering.h"
#include "lib/trace/util.h"

#include <cstddef>
#include <optional>
#include <random>

namespace trace {

Metal::Metal(lina::Vec3 albedo, double fuzz, std::size_t retryCount)
  : albedo_{ albedo }, fuzz_{ fuzz }, retryCount_{ retryCount }
{}

auto Metal::Scatter(Ray const& ray,
  Collision const& collision,
  std::mt19937& randomGenerator) -> std::optional<Scattering>
{
  auto adjustedCollisionPoint = collision.point + collision.normal * 0.00001;
  auto reflectedDirection = ray.Direction() - (2.0 * lina::dot(ray.Direction(), collision.normal) * collision.normal);

  auto fuzzedDirection = reflectedDirection + randomOnUnitSphere(randomGenerator) * fuzz_;
  // Should the fuzzedDirection point in a direction [orthogonal to normal, opposite to normal], then we just
  // regenerate the fuzzed direction. This way we can ensure that we always return a valid ray.
  for (auto retryCount = std::size_t{ 0 };
       retryCount < retryCount_ && lina::dot(fuzzedDirection, collision.normal) <= 0.0;
       ++retryCount) {
    fuzzedDirection = reflectedDirection + randomOnUnitSphere(randomGenerator) * fuzz_;
  }

  auto scattering = Scattering{};
  scattering.attenuation = albedo_;
  scattering.type = Ray{ adjustedCollisionPoint, fuzzedDirection };

  return std::optional<Scattering>{ scattering };
}

}// namespace trace
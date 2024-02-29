#include "lambertian.h"

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/pdf.h"
#include "lib/trace/ray.h"
#include "lib/trace/scattering.h"
#include "lib/trace/util.h"

#include <numbers>
#include <optional>
#include <random>

namespace trace {

Lambertian::Lambertian(lina::Vec3 albedo) : albedo_{ albedo } {}

auto Lambertian::Scatter(Ray const& /*ray*/, Collision const& collision, std::mt19937& randomGenerator)
  -> std::optional<Scattering>
{
  auto normal = collision.frontFace ? collision.normal : collision.normal * -1.0;
  auto adjustedCollisionPoint = collision.point + (normal * 0.00001);
  auto scattering = Scattering{};
  scattering.attenuation = albedo_;
  scattering.type = PDF{ [normal](const lina::Vec3& rayDirection) -> double {
                          auto cos_theta = lina::dot(normal, lina::unit(rayDirection));
                          return cos_theta < 0.0 ? 0.0 : cos_theta / std::numbers::pi;
                        },
    [normal, &randomGenerator]() -> lina::Vec3 {
      auto onb = Onb{ normal };
      return onb.Transform(randomCosineDirection(randomGenerator));
    },
    [adjustedCollisionPoint]() -> lina::Vec3 { return adjustedCollisionPoint; } };

  return std::optional<Scattering>{ scattering };
}
}// namespace trace
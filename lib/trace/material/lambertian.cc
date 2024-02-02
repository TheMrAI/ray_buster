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

auto Lambertian::Scatter(Ray const& /*ray*/,
  Collision const& collision,
  std::mt19937& randomGenerator) -> std::optional<Scattering>
{
  auto scattering = Scattering{};
  scattering.attenuation = albedo_;
  scattering.type = PDF{ [normal = collision.normal](const lina::Vec3& rayDirection) -> double {
                          auto cos_theta = lina::dot(normal, lina::unit(rayDirection));
                          return cos_theta < 0.0 ? 0.0 : cos_theta / std::numbers::pi;
                        },
    [normal = collision.normal, &randomGenerator]() {
      auto onb = Onb{ normal };
      return onb.Transform(randomCosineDirection(randomGenerator));
    } };

  return std::optional<Scattering>{ scattering };
}
}// namespace trace
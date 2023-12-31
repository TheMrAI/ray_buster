#ifndef RAY_BUSTER_LIB_TRACE_MATERIAL_DIELECTRIC_H_
#define RAY_BUSTER_LIB_TRACE_MATERIAL_DIELECTRIC_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"
#include "lib/trace/scattering.h"
#include "lib/trace/util.h"
#include <cmath>

namespace trace {

auto reflectance(double cosTheta, double indexOfRefraction) -> double
{
  // Schlick's approximation for reflectance
  auto r0 = (1.0 - indexOfRefraction) / (1.0 + indexOfRefraction);
  r0 = r0 * r0;
  return r0 + (1.0 - r0) * std::pow(1.0 - cosTheta, 5.0);
}

class Dielectric : public Material
{
public:
  // air: ~1.0
  // glass: 1.3 - 1.7
  // diamond: 2.4
  Dielectric(double indexOfRefraction = 1.4) : indexOfRefraction_{ indexOfRefraction } {}

  // As a rule of thumb, you know that your refraction calculations are correct, when you make dielectric
  // sphere using indexOfRefraction of 1.0, and it becomes imperceptible.
  auto Scatter(Ray const& ray, Collision const& collision, std::mt19937& randomGenerator)
    -> std::optional<Scattering> override
  {
    // In these calculations we assume that we only ever transition between air and a given material.
    // I.e. => no compound materials will work with this.
    auto refractionRatio = collision.frontFace ? (1.0 / indexOfRefraction_) : indexOfRefraction_;
    auto normal = collision.frontFace ? collision.normal : -collision.normal;

    // refract
    auto cosTheta = std::fmin(lina::dot(-ray.Direction(), normal), 1.0);
    auto sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

    auto canRefract = refractionRatio * sinTheta <= 1.0;
    auto resultRay = Ray{};

    if (!canRefract || reflectance(cosTheta, indexOfRefraction_) > randomUniformDouble(randomGenerator, 0.0, 1.0)) {
      // reflect
      auto reflectedDirection = ray.Direction() - 2.0 * lina::dot(ray.Direction(), collision.normal) * collision.normal;
      auto adjustedCollisionPoint = collision.point - collision.normal * 0.00001;
      if (collision.frontFace) { adjustedCollisionPoint = collision.point + collision.normal * 0.00001; }
      resultRay = trace::Ray{ adjustedCollisionPoint, reflectedDirection };
    } else {
      auto refractDirectionPerpendicular = refractionRatio * (ray.Direction() + cosTheta * normal);
      auto refractDirectionParallel =
        -std::sqrt(std::fabs(1.0 - lina::length_squared(refractDirectionPerpendicular.Components()))) * normal;
      auto refractedDirection = refractDirectionPerpendicular + refractDirectionParallel;

      // adjust collision point depending from which side did we hit the boundary
      auto adjustedCollisionPoint = collision.point + collision.normal * 0.00001;
      if (collision.frontFace) { adjustedCollisionPoint = collision.point - collision.normal * 0.00001; }
      resultRay = trace::Ray{ adjustedCollisionPoint, refractedDirection };
    }

    auto scattering = Scattering{};
    scattering.attenuation = lina::Vec3{ 1.0, 1.0, 1.0 };// no attenuation
    scattering.ray = resultRay;

    return std::optional<Scattering>{ scattering };
  }

private:
  double indexOfRefraction_;
};

}// namespace trace

#endif
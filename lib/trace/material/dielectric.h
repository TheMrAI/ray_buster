#ifndef RAY_BUSTER_LIB_TRACE_MATERIAL_DIELECTRIC_H_
#define RAY_BUSTER_LIB_TRACE_MATERIAL_DIELECTRIC_H_

#include "lib/trace/collision.h"
#include "lib/trace/component.h"
#include "lib/trace/material.h"
#include "lib/trace/ray.h"
#include "lib/trace/scattering.h"

#include <optional>
#include <random>

namespace trace {

class Dielectric : public Material
{
public:
  // air: ~1.0
  // glass: 1.3 - 1.7
  // diamond: 2.4
  Dielectric(double indexOfRefraction = 1.4);

  // As a rule of thumb, you know that your refraction calculations are correct, when you make dielectric
  // sphere using indexOfRefraction of 1.0, and it becomes imperceptible.
  auto Scatter(Ray const& ray, Collision const& collision, std::mt19937& randomGenerator)
    -> std::optional<Scattering> override;

private:
  double indexOfRefraction_;
};

auto reflectance(double cosTheta, double indexOfRefraction) -> double;

}// namespace trace

#endif
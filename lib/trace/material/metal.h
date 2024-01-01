#ifndef RAY_BUSTER_LIB_TRACE_MATERIAL_METAL_H_
#define RAY_BUSTER_LIB_TRACE_MATERIAL_METAL_H_

#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/ray.h"
#include "lib/trace/scattering.h"
#include <optional>
#include <random>

namespace trace {

class Metal : public Material
{
public:
  // fuzz_radius 0.0 means reflections act as perfect
  Metal(lina::Vec3 albedo, double fuzz_radius = 0.0);

  auto Scatter(Ray const& ray, Collision const& collision, std::mt19937& randomGenerator)
    -> std::optional<Scattering> override;

private:
  lina::Vec3 albedo_;
  double fuzz_radius_;
};

}// namespace trace

#endif
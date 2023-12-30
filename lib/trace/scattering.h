#ifndef RAY_BUSTER_LIB_TRACE_SCATTERING_H_
#define RAY_BUSTER_LIB_TRACE_SCATTERING_H_

namespace trace {

struct Scattering
{
  trace::Ray ray;
  lina::Vec3 attenuation;
};

class Material
{
public:
  virtual auto Scatter(Ray const& ray, Collision const& collision, std::mt19937& randomGenerator)
    -> std::optional<Scattering>
  {
    return std::optional<Scattering>{};
  };
};

}// namespace trace

#endif
#ifndef RAY_BUSTER_LIB_TRACE_CAMERA_H_
#define RAY_BUSTER_LIB_TRACE_CAMERA_H_

#include "lib/lina/vec3.h"
#include "lib/trace/ray.h"
#include <expected>
#include <random>

namespace trace {

class Camera
{
public:
  Camera(std::size_t imageWidth,
    std::size_t imageHeight,
    lina::Vec3 cameraCenter,
    lina::Vec3 lookAt,
    lina::Vec3 cameraUp,// this vector serves as a starting point for the orthonormal base generation, doesn't have to
                        // be on the plane of the camera viewport, but it has to be on the proper axis
    double degreesVerticalFOV = 90.0,
    double defocusAngle = 0.0,
    double focusDistance = 1.0);

  // Generate a matrix of rays, with multisamplingCount number of rays per pixel
  [[nodiscard]] auto GetSampleRayAt(std::size_t i,
    std::size_t j,
    std::mt19937& randomGenerator,
    bool multiSampled = false) const -> std::expected<trace::Ray, std::string>;

  [[nodiscard]] auto ImageWidth() const -> std::size_t;
  [[nodiscard]] auto ImageHeight() const -> std::size_t;

private:
  std::size_t imageWidth_;
  std::size_t imageHeight_;
  lina::Vec3 cameraCenter_;
  lina::Vec3 baseU_;
  lina::Vec3 baseV_;
  lina::Vec3 baseW_;
  lina::Vec3 pixelDeltaU_;
  lina::Vec3 pixelDeltaV_;
  lina::Vec3 firstPixelPosition_;
  // defocus lense
  double lenseRadius_;
  lina::Vec3 lenseU_;
  lina::Vec3 lenseV_;
};

auto sampleInUnitSquare(std::mt19937& randomGenerator,
  lina::Vec3 const& unitDeltaU,
  lina::Vec3 const& unitDeltaV) -> lina::Vec3;

auto sampleInUnitDisk(std::mt19937& randomGenerator,
  lina::Vec3 const& center,
  lina::Vec3 const& lenseU,
  lina::Vec3 const& lenseV) -> lina::Vec3;

}// namespace trace

#endif

#ifndef RAY_BUSTER_LIB_TRACE_CAMERA_H_
#define RAY_BUSTER_LIB_TRACE_CAMERA_H_

#include "lib/lina/vec3.h"
#include "lib/trace/ray.h"
#include <random>
#include <vector>

namespace trace {

class Camera
{
public:
  Camera(std::size_t image_width,
    std::size_t image_height,
    lina::Vec3 camera_center,
    lina::Vec3 lookAt,
    lina::Vec3 cameraUp,// this vector serves as a starting point for the orthonormal base generation, doesn't have to
                        // be on the plane of the camera viewport, but it has to be on the proper axis
    double degreesVerticalFOV = 90.0,
    double defocusAngle = 0.0,
    double focusDistance = 1.0);


  // Generate a matrix of rays, with multisamplingCount number of rays per pixel
  auto GenerateSamplingRays(std::mt19937& randomGenerator, std::size_t multisamplingCount = 1)
    -> std::vector<std::vector<std::vector<trace::Ray>>>;

private:
  std::size_t image_width_;
  std::size_t image_height_;
  lina::Vec3 camera_center_;
  lina::Vec3 baseU_;
  lina::Vec3 baseV_;
  lina::Vec3 baseW_;
  lina::Vec3 pixel_delta_u_;
  lina::Vec3 pixel_delta_v_;
  lina::Vec3 first_pixel_position_;
  // defocus lense
  double lenseRadius_;
  lina::Vec3 lenseU_;
  lina::Vec3 lenseV_;
};

auto sampleInUnitSquare(std::mt19937& randomGenerator, lina::Vec3 const& unitDeltaU, lina::Vec3 const& unitDeltaV)
  -> lina::Vec3;

auto sampleInUnitDisk(std::mt19937& randomGenerator,
  lina::Vec3 const& center,
  lina::Vec3 const& lenseU,
  lina::Vec3 const& lenseV) -> lina::Vec3;

}// namespace trace

#endif
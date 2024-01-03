#include "camera.h"
#include "lib/lina/vec3.h"
#include "lib/trace/ray.h"
#include "lib/trace/util.h"
#include <cmath>
#include <numbers>
#include <random>
#include <vector>

namespace trace {

Camera::Camera(std::size_t image_width,
  std::size_t image_height,
  lina::Vec3 camera_center,
  lina::Vec3 lookAt,
  lina::Vec3 cameraUp,
  double degreesVerticalFOV,
  double defocusAngle,
  double focusDistance)
  : image_width_{ image_width }, image_height_{ image_height }, camera_center_{ std::move(camera_center) }
{
  auto theta = degreesToRadians(degreesVerticalFOV);
  auto h = std::tan(theta / 2.0);
  auto viewport_height = 2.0 * h * focusDistance;
  auto viewport_width = viewport_height * (static_cast<double>(image_width) / static_cast<double>(image_height));

  // Calculate u,v,w base vectors for the camera
  baseW_ = lina::unit(camera_center_ - lookAt);
  baseU_ = lina::unit(lina::cross(cameraUp, baseW_));
  baseV_ = lina::cross(baseW_, baseU_);

  // viewport_u points from camera top left corner to the right
  // viewport_v points from camera top left corner down
  auto viewport_u = viewport_width * baseU_;
  auto viewport_v = viewport_height * -baseV_;

  pixel_delta_u_ = viewport_u / image_width;
  pixel_delta_v_ = viewport_v / image_height;

  auto viewport_upper_left = camera_center_ - (focusDistance * baseW_) - (viewport_u / 2.0) - (viewport_v / 2.0);
  first_pixel_position_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);

  // Calculate defocus lense properties
  // On defocusAngle 0.0 the vectors will be null vectors and
  // no blurring should occur.
  lenseRadius_ = focusDistance * std::tan(degreesToRadians(defocusAngle / 2.0));
  lenseU_ = baseU_ * lenseRadius_;
  lenseV_ = baseV_ * lenseRadius_;
}

// Generate a matrix of rays, with multisamplingCount number of rays per pixel
auto Camera::GenerateSamplingRays(std::mt19937& randomGenerator, std::size_t multisamplingCount)
  -> std::vector<std::vector<std::vector<Ray>>>
{
  auto sampling_rays = std::vector<std::vector<std::vector<Ray>>>(image_height_);

  for (auto i = size_t{ 0 }; i < image_height_; ++i) {
    sampling_rays[i] = std::vector<std::vector<Ray>>(image_width_);
    for (auto j = size_t{ 0 }; j < image_width_; ++j) {
      sampling_rays[i][j] = std::vector<Ray>(multisamplingCount);
      for (auto sample = size_t{ 0 }; sample < multisamplingCount; ++sample) {
        auto pixel_center = first_pixel_position_ + (j * pixel_delta_u_) + (i * pixel_delta_v_);
        auto pixelSample = pixel_center + sampleInUnitSquare(randomGenerator, pixel_delta_u_, pixel_delta_v_);
        // in case we want only a single sample it should go through the center of the pixel
        if (multisamplingCount == 1) { pixelSample = pixel_center; }

        auto raySource = sampleInUnitDisk(randomGenerator, camera_center_, lenseU_, lenseV_);
        auto ray_direction = lina::unit(pixelSample - raySource);
        sampling_rays[i][j][sample] = Ray{ raySource, ray_direction };
      }
    }
  }
  return sampling_rays;
}

auto sampleInUnitSquare(std::mt19937& randomGenerator, lina::Vec3 const& unitDeltaU, lina::Vec3 const& unitDeltaV)
  -> lina::Vec3
{
  auto uOffset = randomUniformDouble(randomGenerator, -0.5, 0.5);
  auto vOffset = randomUniformDouble(randomGenerator, -0.5, 0.5);
  return (uOffset * unitDeltaU) + (vOffset * unitDeltaV);
}

auto sampleInUnitDisk(std::mt19937& randomGenerator,
  lina::Vec3 const& center,
  lina::Vec3 const& lenseU,
  lina::Vec3 const& lenseV) -> lina::Vec3
{
  auto onUnitDisk = randomOnUnitDisk(randomGenerator);
  return center + (lenseU * onUnitDisk.Components()[0]) + (lenseV * onUnitDisk.Components()[1]);
}

}// namespace trace
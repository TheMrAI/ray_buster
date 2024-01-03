#include "camera.h"
#include "lib/lina/vec3.h"
#include "lib/trace/ray.h"
#include "lib/trace/util.h"
#include <cmath>
#include <numbers>
#include <random>
#include <vector>

namespace trace {

Camera::Camera(std::size_t imageWidth,
  std::size_t imageHeight,
  lina::Vec3 cameraCenter,
  lina::Vec3 lookAt,
  lina::Vec3 cameraUp,
  double degreesVerticalFOV,
  double defocusAngle,
  double focusDistance)
  : imageWidth_{ imageWidth }, imageHeight_{ imageHeight }, cameraCenter_{ std::move(cameraCenter) }
{
  auto theta = degreesToRadians(degreesVerticalFOV);
  auto h = std::tan(theta / 2.0);
  auto viewportHeight = 2.0 * h * focusDistance;
  auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / static_cast<double>(imageHeight));

  // Calculate u,v,w base vectors for the camera
  baseW_ = lina::unit(cameraCenter_ - lookAt);
  baseU_ = lina::unit(lina::cross(cameraUp, baseW_));
  baseV_ = lina::cross(baseW_, baseU_);

  // viewport_u points from camera top left corner to the right
  // viewport_v points from camera top left corner down
  auto viewportU = viewportWidth * baseU_;
  auto viewportV = viewportHeight * -baseV_;

  pixelDeltaU_ = viewportU / imageWidth;
  pixelDeltaV_ = viewportV / imageHeight;

  auto viewportUpperLeft = cameraCenter_ - (focusDistance * baseW_) - (viewportU / 2.0) - (viewportV / 2.0);
  firstPixelPosition_ = viewportUpperLeft + 0.5 * (pixelDeltaU_ + pixelDeltaV_);

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
  auto samplingRays = std::vector<std::vector<std::vector<Ray>>>(imageHeight_);

  for (auto i = size_t{ 0 }; i < imageHeight_; ++i) {
    samplingRays[i] = std::vector<std::vector<Ray>>(imageWidth_);
    for (auto j = size_t{ 0 }; j < imageWidth_; ++j) {
      samplingRays[i][j] = std::vector<Ray>(multisamplingCount);
      for (auto sample = size_t{ 0 }; sample < multisamplingCount; ++sample) {
        auto pixelCenter = firstPixelPosition_ + (j * pixelDeltaU_) + (i * pixelDeltaV_);
        auto pixelSample = pixelCenter + sampleInUnitSquare(randomGenerator, pixelDeltaU_, pixelDeltaV_);
        // in case we want only a single sample it should go through the center of the pixel
        if (multisamplingCount == 1) { pixelSample = pixelCenter; }

        auto raySource = sampleInUnitDisk(randomGenerator, cameraCenter_, lenseU_, lenseV_);
        auto rayDirection = lina::unit(pixelSample - raySource);
        samplingRays[i][j][sample] = Ray{ raySource, rayDirection };
      }
    }
  }
  return samplingRays;
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
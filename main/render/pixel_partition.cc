
#include "main/render/pixel_partition.h"

#include "lib/lina/vec3.h"
#include "lib/trace/camera.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/component.h"
#include "lib/trace/material.h"
#include "lib/trace/pdf.h"
#include "lib/trace/ray.h"
#include "lib/trace/util.h"
#include "main/scenes/scene.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <format>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <random>
#include <stdexcept>
#include <thread>
#include <utility>
#include <variant>
#include <vector>

namespace render {

// Source: https://codeforces.com/blog/entry/78852
auto ceil2(std::size_t a, std::size_t b) -> std::size_t
{
  if (a == 0) { return 0; }
  return ((a - 1) / b) + 1;
}

auto closestCollision(trace::Ray const& ray,
  std::vector<scene::Element> const& sceneElements) -> std::pair<std::optional<trace::Collision>, std::size_t>
{
  auto closestCollision = std::optional<trace::Collision>{};
  auto elementIndex = std::size_t{ 0 };

  for (auto i = std::size_t{ 0 }; i < sceneElements.size(); ++i) {
    auto collision = sceneElements[i].component->Collide(ray);
    if (!collision) { continue; }
    if (!closestCollision) {
      closestCollision = collision;
      elementIndex = i;
      continue;
    }
    auto closestDistance = (closestCollision->point - ray.Source()).Length();
    auto collisionDistance = (collision->point - ray.Source()).Length();
    if (collisionDistance < closestDistance) {
      closestCollision = collision;
      elementIndex = i;
    }
  }

  return std::make_pair(closestCollision, elementIndex);
}

auto rayColor(trace::Ray const& ray,
  std::vector<scene::Element> const& sceneElements,
  int masterLightIndex,
  std::mt19937& randomGenerator,
  std::size_t depth,
  bool useSkybox) -> lina::Vec3
{
  if (depth == 0) { return lina::Vec3{ 0.0, 0.0, 0.0 }; }

  auto [collision, elementIndex] = closestCollision(ray, sceneElements);
  if (collision) {
    auto const& material = sceneElements[elementIndex].material;
    auto const emission = material->Emit(collision.value());
    auto scattering = material->Scatter(ray, collision.value(), randomGenerator);
    if (!scattering) { return emission; }

    auto scatterColor = lina::Vec3{};
    if (std::holds_alternative<trace::Ray>(scattering.value().type)) {
      auto scatteredRay = std::get<trace::Ray>(scattering.value().type);
      scatterColor = scattering.value().attenuation
                     * rayColor(scatteredRay, sceneElements, masterLightIndex, randomGenerator, depth - 1, useSkybox);
    } else if (std::holds_alternative<trace::PDF>(scattering.value().type)) {
      // combined
      if (masterLightIndex > -1 && masterLightIndex < static_cast<int>(sceneElements.size())) {
        auto lightPDF =
          sceneElements[masterLightIndex].component->SamplingPDF(randomGenerator, collision.value().point);
        auto materialPDF = std::get<trace::PDF>(scattering.value().type);
        auto sampleDirection = lina::Vec3{};
        if (trace::randomUniformDouble(randomGenerator, 0.0, 1.0) < 0.5) {
          sampleDirection = lightPDF.GenerateSample();
        } else {
          sampleDirection = materialPDF.GenerateSample();
        }

        auto scatteredRay = trace::Ray{ materialPDF.AdjustedCollisionPoint(), sampleDirection };

        auto samplingPDFValue =
          (0.5 * lightPDF.Evaluate(scatteredRay.Direction())) + (0.5 * materialPDF.Evaluate(scatteredRay.Direction()));

        auto scatteringPDFValue = materialPDF.Evaluate(scatteredRay.Direction());

        auto incomingColor =
          rayColor(scatteredRay, sceneElements, masterLightIndex, randomGenerator, depth - 1, useSkybox);
        scatterColor = (scattering.value().attenuation * scatteringPDFValue * incomingColor) / samplingPDFValue;
      } else {
        // normal sampling
        auto materialPDF = std::get<trace::PDF>(scattering.value().type);
        auto scatteredRay = trace::Ray{ materialPDF.AdjustedCollisionPoint(), materialPDF.GenerateSample() };

        auto scatteringPDFValue = materialPDF.Evaluate(scatteredRay.Direction());
        auto pdfValue = scatteringPDFValue;

        scatterColor =
          (scattering.value().attenuation * scatteringPDFValue
            * rayColor(scatteredRay, sceneElements, masterLightIndex, randomGenerator, depth - 1, useSkybox))
          / pdfValue;
      }
    } else {
      throw std::logic_error("Unhandled scattering type.");
    }

    return emission + scatterColor;
  }

  if (useSkybox) {
    auto a = 0.5 * (ray.Direction()[2] + 1.0);
    return (1.0 - a) * lina::Vec3{ 1.0, 1.0, 1.0 } + a * lina::Vec3{ 0.5, 0.7, 1.0 };
  }
  return lina::Vec3{ 0.0, 0.0, 0.0 };
}

// applying gamma correction to the colors
auto linearToGamma(double LinearSpaceValue) -> double { return std::sqrt(LinearSpaceValue); }

auto writeColor(lina::Vec3 const& color, std::ostream& outputStream) -> void
{
  auto red = std::min(1.0, linearToGamma(color[0]));
  auto green = std::min(1.0, linearToGamma(color[1]));
  auto blue = std::min(1.0, linearToGamma(color[2]));

  // we can get close to 256, but not above, granted the input comes in between [0.0, 1.0)
  outputStream << static_cast<int>(255.9999 * red) << " " << static_cast<int>(255.9999 * green) << " "
               << static_cast<int>(255.9999 * blue) << '\n';
}

auto linearPartition(scene::Composition sceneComposition, std::ostream& outputStream) -> void
{
  auto [camera, sampleCount, rayDepth, sceneElements, masterLightIndex, useSkybox] = std::move(sceneComposition);
  auto imageWidth = camera.ImageWidth();
  auto imageHeight = camera.ImageHeight();

  auto const hardwareThreads = std::size_t{ std::thread::hardware_concurrency() };
  // if we can't get the actual number of available hardware threads then we just default to four
  auto const numberOfThreads = (hardwareThreads == std::size_t{ 0 } ? std::size_t{ 4 } : hardwareThreads);

  std::cerr << "Number of threads used: " << numberOfThreads << '\n';

  // reportProgress should only be true for one thread at a time
  // capture sceneElements and samplingRays as const refs, because there should be no circumstance where they need
  // to be changed during rendering (and we don't want to copy them)
  auto renderChunk =
    [imageWidth,
      numberOfThreads,
      camera = std::cref(camera),
      sampleCount,
      rayDepth,
      sceneElements = std::cref(sceneElements),
      masterLightIndex,
      useSkybox](std::size_t startIndex, std::size_t endIndex, bool reportProgress = false) -> std::vector<lina::Vec3> {
    auto maxElementCount = ceil2(endIndex, numberOfThreads);
    auto pixelColors = std::vector<lina::Vec3>();
    pixelColors.reserve(maxElementCount);

    auto randomDevice = std::random_device{};
    auto randomGenerator = std::mt19937{ randomDevice() };

    for (auto pixelId = startIndex; pixelId < endIndex; pixelId += numberOfThreads) {
      if (reportProgress) {
        // It complains for static_cast<double>(endIndex / numberOfThreads), but we intend doing
        // the bugprone integer division, to have a nice approximation.
        // NOLINTBEGIN(bugprone-integer-division)
        std::cout << std::format("\rProcessing: {}/{}, {:.2f} %",
          pixelColors.size() + 1,
          endIndex / numberOfThreads,
          100.0 * static_cast<double>(pixelColors.size() + 1) / static_cast<double>(endIndex / numberOfThreads))
                  << std::flush;
        // NOLINTEND(bugprone-integer-division)
      }
      auto i = pixelId / imageWidth;
      auto j = pixelId % imageWidth;
      auto color = lina::Vec3{ 0.0, 0.0, 0.0 };
      for (auto sample = std::size_t{ 0 }; sample < sampleCount; ++sample) {
        auto const ray = camera.get().GetSampleRayAt(i, j, randomGenerator, sampleCount > 1);
        color += rayColor(ray, sceneElements, masterLightIndex, randomGenerator, rayDepth, useSkybox);
      }
      color /= static_cast<double>(sampleCount);
      pixelColors.emplace_back(color);
    }
    if (reportProgress) { std::cout << '\n'; }
    return pixelColors;
  };

  auto renderChunkResults = std::vector<std::future<std::vector<lina::Vec3>>>();
  renderChunkResults.reserve(numberOfThreads);

  auto workingThreads = std::vector<std::jthread>{};
  workingThreads.reserve(numberOfThreads - 1);

  auto const endIndex = imageWidth * imageHeight;
  for (auto startIndex = std::size_t{ 0 }; startIndex < numberOfThreads; ++startIndex) {
    auto renderTask = std::packaged_task<std::vector<lina::Vec3>(std::size_t, std::size_t, bool)>(renderChunk);
    renderChunkResults.emplace_back(renderTask.get_future());

    if (startIndex < numberOfThreads - 1) {
      workingThreads.emplace_back(std::move(renderTask), startIndex, endIndex, false);
    } else {
      renderTask(startIndex, endIndex, true);
    }
  }

  auto pixelData = std::vector<std::vector<lina::Vec3>>();
  pixelData.reserve(numberOfThreads);
  for (auto i = std::size_t{ 0 }; i < renderChunkResults.size(); ++i) {
    pixelData.emplace_back(renderChunkResults[i].get());
  }

  // serialize render results
  outputStream << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
  for (auto pixelId = std::size_t{ 0 }; pixelId < endIndex; ++pixelId) {
    auto renderResultIndex = pixelId % numberOfThreads;
    auto pixelIndex = pixelId / numberOfThreads;
    writeColor(pixelData[renderResultIndex][pixelIndex], outputStream);
  }
}

}// namespace render

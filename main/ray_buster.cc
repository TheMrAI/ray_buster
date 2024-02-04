#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <optional>
#include <random>
#include <thread>
#include <utility>
#include <variant>
#include <vector>

#include "lib/lina/vec3.h"
#include "lib/trace/camera.h"
#include "lib/trace/collision.h"
#include "lib/trace/component.h"
#include "lib/trace/material.h"
#include "lib/trace/material/emissive.h"
#include "lib/trace/pdf.h"
#include "lib/trace/ray.h"
#include "main/scenes/collection/cornell_box.h"
#include "main/scenes/scene.h"
#include "main/scenes/test/cuboid.h"
#include "main/scenes/test/plane.h"
#include "main/scenes/test/sphere.h"
#include <stdexcept>

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
      scatterColor =
        scattering.value().attenuation * rayColor(scatteredRay, sceneElements, randomGenerator, depth - 1, useSkybox);
    } else if (std::holds_alternative<trace::PDF>(scattering.value().type)) {
      // auto materialPdf = std::get<trace::PDF>(scattering.value().type);
      // auto scatteredRay =
      //   trace::Ray{ collision.value().point + (collision.value().normal * 0.00001), materialPdf.GenerateSample() };
      // auto scatteringPdfValue = materialPdf.Evaluate(scatteredRay.Direction());
      // auto pdf = scatteringPdfValue;

      // scatterColor = (scattering.value().attenuation * scatteringPdfValue
      //                  * rayColor(scatteredRay, sceneElements, randomGenerator, depth - 1, useSkybox))
      //                / pdf;

      // combined
      // auto light = std::find_if(sceneElements.cbegin(), sceneElements.cend(), [](auto const& entry){
      //   return dynamic_cast<trace::Emissive*>(entry.material.get()) != nullptr;
      // });

      // auto lightPDF = light->component->SamplingPDF(randomGenerator, collision.value().point);
      // auto materialPdf = std::get<trace::PDF>(scattering.value().type);
      // auto chosenPdf = lightPDF;
      // auto sample = lightPDF.GenerateSample();
      // if (trace::randomUniformDouble(randomGenerator, 0.0, 1.0) >= 0.5) {
      //   sample = materialPdf.GenerateSample();
      //   chosenPdf = materialPdf;
      // }

      // auto scatteredRay =
      //   trace::Ray{ collision.value().point + (collision.value().normal * 0.00001), chosenPdf.GenerateSample() };

      // auto samplingPdf = (0.5 * lightPDF.Evaluate(scatteredRay.Direction())) + (0.5 *
      // materialPdf.Evaluate(scatteredRay.Direction()));

      // auto scatteringPdfValue = materialPdf.Evaluate(scatteredRay.Direction());

      // scatterColor = (scattering.value().attenuation * scatteringPdfValue
      //                  * rayColor(scatteredRay, sceneElements, randomGenerator, depth - 1, useSkybox))
      //                / samplingPdf;

      // direct sampling
      auto light = std::find_if(sceneElements.cbegin(), sceneElements.cend(), [](auto const& entry) {
        return dynamic_cast<trace::Emissive*>(entry.material.get()) != nullptr;
      });

      auto lightPDF = light->component->SamplingPDF(randomGenerator, collision.value().point);
      auto scatteredRay =
        trace::Ray{ collision.value().point + (collision.value().normal * 0.00001), lightPDF.GenerateSample() };
      auto pdf = lightPDF.Evaluate(scatteredRay.Direction());

      auto materialPdf = std::get<trace::PDF>(scattering.value().type);
      auto scatteringPdfValue = materialPdf.Evaluate(scatteredRay.Direction());

      scatterColor = (scattering.value().attenuation * scatteringPdfValue
                       * rayColor(scatteredRay, sceneElements, randomGenerator, depth - 1, useSkybox))
                     / pdf;
    } else {
      throw std::logic_error("Unhandled scattering type.");
    }

    return emission + scatterColor;
  }

  if (useSkybox) {
    auto a = 0.5 * (ray.Direction()[1] + 1.0);
    return (1.0 - a) * lina::Vec3{ 1.0, 1.0, 1.0 } + a * lina::Vec3{ 0.5, 0.7, 1.0 };
  }
  return lina::Vec3{ 0.0, 0.0, 0.0 };
}

// applying gamma correction to the colors
auto linearToGamma(double LinearSpaceValue) -> double { return std::sqrt(LinearSpaceValue); }

auto writeColor(lina::Vec3 const& color)
{
  auto red = linearToGamma(color[0]);
  auto green = linearToGamma(color[1]);
  auto blue = linearToGamma(color[2]);

  // we can get close to 256, but not above, granted the input comes in between [0.0, 1.0)
  std::cout << static_cast<int>(255.9999 * red) << " " << static_cast<int>(255.9999 * green) << " "
            << static_cast<int>(255.9999 * blue) << '\n';
}

auto main() -> int
{
  auto [camera, sampleCount, rayDepth, sceneElements, useSkybox] = scene::cornellBox();
  auto imageWidth = camera.ImageWidth();
  auto imageHeight = camera.ImageHeight();

  constexpr auto minNumberOfLinesPerThread = std::size_t{ 50 };
  auto const maxThreads = (imageHeight + minNumberOfLinesPerThread - std::size_t{ 1 }) / minNumberOfLinesPerThread;
  auto const hardwareThreads = std::size_t{ std::thread::hardware_concurrency() };
  auto const numberOfThreads =
    std::min(hardwareThreads == std::size_t{ 0 } ? std::size_t{ 4 } : hardwareThreads, maxThreads);
  auto const blockSize = (imageHeight + numberOfThreads - std::size_t{ 1 }) / numberOfThreads;

  std::cerr << "Number of threads used: " << numberOfThreads << '\n';

  // reportProgress should only be true for one thread at a time
  // capture sceneElements and samplingRays as const refs, because there should be nor circumstance where they need
  // to be changed during rendering
  auto renderChunk = [imageWidth,
                       camera = std::cref(camera),
                       sampleCount,
                       rayDepth,
                       sceneElements = std::cref(sceneElements),
                       useSkybox](std::size_t from, std::size_t until, bool reportProgress = false) {
    auto distance = until - from;
    auto pixelColors = std::vector<std::vector<lina::Vec3>>(distance, std::vector<lina::Vec3>(imageWidth));

    auto randomDevice = std::random_device{};
    auto randomGenerator = std::mt19937{ randomDevice() };

    for (auto i = from; i < until; ++i) {
      if (reportProgress) { std::clog << "\rScan-lines remaining: " << (until - i - 1) << ' ' << std::flush; }
      for (auto j = std::size_t{ 0 }; j < imageWidth; ++j) {
        auto color = lina::Vec3{ 0.0, 0.0, 0.0 };
        for (auto sample = std::size_t{ 0 }; sample < sampleCount; ++sample) {
          auto const ray = camera.get().GetSampleRayAt(i, j, randomGenerator, sampleCount > 1);
          if (!ray) {
            std::cerr << ray.error() << std::endl;
            std::exit(1);
          }
          color += rayColor(ray.value(), sceneElements, randomGenerator, rayDepth, useSkybox);
        }
        color /= static_cast<double>(sampleCount);
        pixelColors[i - from][j] = color;
      }
    }
    return pixelColors;
  };

  auto renderChunkResults = std::vector<std::future<std::vector<std::vector<lina::Vec3>>>>();
  renderChunkResults.reserve(numberOfThreads);

  auto workingThreads = std::vector<std::jthread>{};
  workingThreads.reserve(numberOfThreads - 1);

  for (auto chunkIndex = std::size_t{ 0 }; chunkIndex < numberOfThreads; ++chunkIndex) {
    auto renderTask =
      std::packaged_task<std::vector<std::vector<lina::Vec3>>(std::size_t, std::size_t, bool)>(renderChunk);
    renderChunkResults.emplace_back(renderTask.get_future());

    auto from = chunkIndex * blockSize;
    auto until = from + blockSize;
    if (chunkIndex < numberOfThreads - 1) {
      workingThreads.emplace_back(std::move(renderTask), from, until, false);
    } else {
      renderTask(from, until, true);
    }
  }

  // serialize render results
  std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
  for (auto& renderResults : renderChunkResults) {
    for (auto const& line : renderResults.get()) {
      for (auto const& color : line) { writeColor(color); }
    }
  }

  return 0;
}
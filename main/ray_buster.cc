#include <cmath>
#include <future>
#include <iostream>
#include <memory>
#include <random>
#include <thread>
#include <utility>
#include <vector>

#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/camera.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/plane.h"
#include "lib/trace/geometry/sphere.h"
#include "lib/trace/material/dielectric.h"
#include "lib/trace/material/emissive.h"
#include "lib/trace/material/lambertian.h"
#include "lib/trace/material/metal.h"
#include "lib/trace/ray.h"
#include "lib/trace/scattering.h"
#include "lib/trace/transform.h"
#include "lib/trace/util.h"

struct SceneElement
{
  std::unique_ptr<trace::Component> component;
  std::unique_ptr<trace::Material> material;
};

auto closestCollision(trace::Ray const& ray, std::vector<SceneElement> const& sceneElements)
  -> std::pair<std::optional<trace::Collision>, size_t>
{
  auto closestCollision = std::optional<trace::Collision>{};
  auto elementIndex = size_t{ 0 };

  for (auto i = size_t{ 0 }; i < sceneElements.size(); ++i) {
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
  std::vector<SceneElement> const& sceneElements,
  std::mt19937& randomGenerator,
  std::size_t depth) -> lina::Vec3
{
  if (depth == 0) { return lina::Vec3{ 0.0, 0.0, 0.0 }; }

  auto [collision, elementIndex] = closestCollision(ray, sceneElements);
  if (collision) {
    auto const& material = sceneElements[elementIndex].material;
    auto const emission = material->Emit();
    auto scattering = material->Scatter(ray, collision.value(), randomGenerator);
    if (!scattering) { return emission; }
    return emission
           + (scattering.value().attenuation
              * rayColor(scattering.value().ray, sceneElements, randomGenerator, depth - 1));
  }

  // auto a = 0.5 * (ray.Direction()[1] + 1.0);
  // return (1.0 - a) * lina::Vec3{ 1.0, 1.0, 1.0 } + a * lina::Vec3{ 0.5, 0.7, 1.0 };
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
            << static_cast<int>(255.9999 * blue) << std::endl;
}

auto main() -> int
{
  auto imageWidth = size_t{ 1024 };
  auto imageHeight = size_t{ 768 };

  // Camera
  auto camera = trace::Camera{ imageWidth,
    imageHeight,
    lina::Vec3{ 0.0, -100.0, 50.0 },// camera center
    lina::Vec3{ 0.0, 0.0, 50.0 },// look at
    lina::Vec3{ 0.0, 0.0, 1.0 },
    70.0,
    0.0 };

  auto sceneElements = std::vector<SceneElement>{};

  auto bottom = trace::build(lina::Vec3{ 0.0, 0.0, 0.0 }, 100.0, 100.0, trace::Axis::Z, trace::Orientation::Aligned);
  auto top = trace::build(lina::Vec3{ 0.0, 0.0, 100.0 }, 100.0, 100.0, trace::Axis::Z, trace::Orientation::Reverse);
  auto back = trace::build(lina::Vec3{ 0.0, 50.0, 50.0 }, 100.0, 100.0, trace::Axis::Y, trace::Orientation::Reverse);
  auto left = trace::build(lina::Vec3{ -50.0, 0.0, 50.0 }, 100.0, 100.0, trace::Axis::X, trace::Orientation::Aligned);
  auto right = trace::build(lina::Vec3{ 50.0, 0.0, 50.0 }, 100.0, 100.0, trace::Axis::X, trace::Orientation::Reverse);

  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(bottom.value())),
    std::make_unique<trace::Lambertian>(lina::Vec3{ 0.9296, 0.9179, 0.8476 }));
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(top.value())),
    std::make_unique<trace::Lambertian>(lina::Vec3{ 0.9296, 0.9179, 0.8476 }));
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(back.value())),
    std::make_unique<trace::Lambertian>(lina::Vec3{ 0.0273, 0.0156, 0.2187 }));
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(left.value())),
    std::make_unique<trace::Lambertian>(lina::Vec3{ 0.0, 0.8125, 0.3828 }));
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(right.value())),
    std::make_unique<trace::Lambertian>(lina::Vec3{ 0.8203, 0.0156, 0.1757 }));

  auto light = trace::build(lina::Vec3{ 0.0, 0.0, 99.9 }, 15.0, 15.0, trace::Axis::Z, trace::Orientation::Reverse);
  sceneElements.emplace_back(std::make_unique<trace::Plane>(std::move(light.value())),
    std::make_unique<trace::Emissive>(lina::Vec3{ 15.0, 15.0, 15.0 }));

  sceneElements.emplace_back(std::make_unique<trace::Sphere>(lina::Vec3{ 15.0, -10.0, 15.0 }, 15),
    std::make_unique<trace::Lambertian>(lina::Vec3{ 0.8203, 0.0156, 0.1757 }));// rando sphere

  auto randomDevice = std::random_device{};
  auto randomGenerator = std::mt19937{ randomDevice() };
  constexpr auto sampleCount = size_t{ 300 };
  auto samplingRays = camera.GenerateSamplingRays(randomGenerator, sampleCount);
  constexpr auto rayDepth = size_t{ 50 };

  constexpr auto minNumberOfLinesPerThread = size_t{ 50 };
  auto const maxThreads = (imageHeight + minNumberOfLinesPerThread - size_t{ 1 }) / minNumberOfLinesPerThread;
  auto const hardwareThreads = size_t{ std::thread::hardware_concurrency() };
  auto const numberOfThreads = std::min(hardwareThreads == size_t{ 0 } ? size_t{ 4 } : hardwareThreads, maxThreads);
  auto const blockSize = (imageHeight + numberOfThreads - size_t{ 1 }) / numberOfThreads;

  std::cerr << "Number of threads used: " << numberOfThreads << std::endl;

  // reportProgress should only be true for one thread at a time
  // capture sceneElements and samplingRays as const refs, because there should be nor circumstance where they need
  // to be changed during rendering
  auto renderChunk = [imageWidth,
                       imageHeight,
                       sampleCount,
                       rayDepth,
                       sceneElements = std::cref(sceneElements),
                       samplingRays = std::cref(samplingRays)](uint from, uint until, bool reportProgress = false) {
    auto distance = until - from;
    auto pixelColors = std::vector<std::vector<lina::Vec3>>(distance, std::vector<lina::Vec3>(imageWidth));

    auto randomDevice = std::random_device{};
    auto randomGenerator = std::mt19937{ randomDevice() };

    for (auto i = from; i < until; ++i) {
      if (reportProgress) { std::clog << "\rScan-lines remaining: " << (until - i - 1) << ' ' << std::flush; }
      for (auto j = size_t{ 0 }; j < imageWidth; ++j) {
        auto color = lina::Vec3{ 0.0, 0.0, 0.0 };
        for (auto sample = size_t{ 0 }; sample < sampleCount; ++sample) {
          auto const& ray = samplingRays.get()[i][j][sample];
          color += rayColor(ray, sceneElements, randomGenerator, rayDepth);
        }
        color /= sampleCount;
        pixelColors[i - from][j] = color;
      }
    }
    return pixelColors;
  };

  auto renderChunkResults = std::vector<std::future<std::vector<std::vector<lina::Vec3>>>>();
  renderChunkResults.reserve(numberOfThreads);

  auto workingThreads = std::vector<std::jthread>{};
  workingThreads.reserve(numberOfThreads - 1);

  for (auto chunkIndex = 0u; chunkIndex < numberOfThreads; ++chunkIndex) {
    auto renderTask = std::packaged_task<std::vector<std::vector<lina::Vec3>>(uint, uint, bool)>(renderChunk);
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
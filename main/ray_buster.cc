#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <utility>
#include <vector>

#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/camera.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/sphere.h"
#include "lib/trace/material/dielectric.h"
#include "lib/trace/material/lambertian.h"
#include "lib/trace/material/metal.h"
#include "lib/trace/ray.h"
#include "lib/trace/scattering.h"
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

auto ray_color(trace::Ray const& ray,
  std::vector<SceneElement> const& sceneElements,
  std::mt19937& randomGenerator,
  std::size_t depth) -> lina::Vec3
{
  if (depth == 0) { return lina::Vec3{ 0.0, 1.0, 0.0 }; }

  auto [collision, elementIndex] = closestCollision(ray, sceneElements);
  if (collision) {
    auto const& material = sceneElements[elementIndex].material;
    auto scattering = material->Scatter(ray, collision.value(), randomGenerator);
    if (scattering) {
      return scattering.value().attenuation
             * ray_color(scattering.value().ray, sceneElements, randomGenerator, depth - 1);
    }
    // This is not acceptable
    return lina::Vec3{ 1.0, 0.0, 0.0 };
  }

  auto a = 0.5 * (ray.Direction()[1] + 1.0);
  return (1.0 - a) * lina::Vec3{ 1.0, 1.0, 1.0 } + a * lina::Vec3{ 0.5, 0.7, 1.0 };
}

// applying gamma correction to the colors
auto linearToGamma(double LinearSpaceValue) -> double { return std::sqrt(LinearSpaceValue); }

auto write_color(lina::Vec3 const& color)
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
  auto image_width = size_t{ 1024 };
  auto image_height = size_t{ 768 };

  // Camera
  auto camera = trace::Camera{ image_width,
    image_height,
    lina::Vec3{ 0.0, 0.0, 0.0 },
    lina::Vec3{ 0.0, 0.0, -1.0 },
    lina::Vec3{ 0.0, 1.0, 0.0 },
    90.0 };

  auto sceneElements = std::vector<SceneElement>{};
  sceneElements.emplace_back(std::make_unique<trace::Sphere>(lina::Vec3{ 0.0, -100.5, -1.0 }, 100),
    std::make_unique<trace::Metal>(lina::Vec3{ 0.7, 0.8, 0.7 }, 0.01));// world
  sceneElements.emplace_back(std::make_unique<trace::Sphere>(lina::Vec3{ 1.0, 0.0, -1.0 }, 0.5),
    std::make_unique<trace::Lambertian>(lina::Vec3{ 0.7, 0.5, 0.5 }));// red sphere
  sceneElements.emplace_back(std::make_unique<trace::Sphere>(lina::Vec3{ 0.0, 0.0, -1.0 }, 0.5),
    std::make_unique<trace::Lambertian>(lina::Vec3{ 0.5, 0.5, 0.5 }));// sphere
  sceneElements.emplace_back(std::make_unique<trace::Sphere>(lina::Vec3{ -1.0, 0.0, -1.0 }, 0.5),
    std::make_unique<trace::Dielectric>(1.7));// glass sphere

  auto randomDevice = std::random_device{};
  auto randomGenerator = std::mt19937{ randomDevice() };

  auto sampleCount = size_t{ 1 };
  auto sampling_rays = camera.GenerateSamplingRays(randomGenerator, sampleCount);

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
  for (auto i = size_t{ 0 }; i < image_height; ++i) {
    for (auto j = size_t{ 0 }; j < image_width; ++j) {
      auto color = lina::Vec3{ 0.0, 0.0, 0.0 };
      for (auto sample = size_t{ 0 }; sample < sampleCount; ++sample) {
        auto ray = sampling_rays[i][j][sample];
        color += ray_color(ray, sceneElements, randomGenerator, 10);
      }
      color /= sampleCount;
      write_color(color);
    }
  }

  return 0;
}
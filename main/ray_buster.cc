#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/camera.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/sphere.h"
#include "lib/trace/material/lambertian.h"
#include "lib/trace/ray.h"
#include "lib/trace/scattering.h"
#include "lib/trace/util.h"

auto closest_collision(trace::Ray const& ray, std::vector<std::unique_ptr<trace::Component>> const& scene_components)
  -> std::optional<trace::Collision>
{
  auto closest_collision = std::optional<trace::Collision>{};

  for (auto const& component : scene_components) {
    auto collision = component->Collide(ray);
    if (!collision) { continue; }
    if (!closest_collision) {
      closest_collision = collision;
      continue;
    }
    auto closest_distance = (closest_collision->point - ray.Source()).Length();
    auto collision_distance = (collision->point - ray.Source()).Length();
    if (collision_distance < closest_distance) { closest_collision = collision; }
  }

  return closest_collision;
}

auto ray_color(trace::Ray const& ray,
  std::vector<std::unique_ptr<trace::Component>> const& scene_components,
  std::mt19937& randomGenerator,
  std::size_t depth) -> lina::Vec3
{
  if (depth == 0) { return lina::Vec3{ 0.0, 0.0, 0.0 }; }

  auto collision = closest_collision(ray, scene_components);
  if (collision) {
    auto material = trace::Lambertian{ lina::Vec3{ 0.5, 0.5, 0.5 } };
    auto scattering = material.Scatter(ray, collision.value(), randomGenerator);
    if (scattering) {
      return scattering.value().attenuation
             * ray_color(scattering.value().ray, scene_components, randomGenerator, depth - 1);
    }
    return lina::Vec3{ 0.0, 0.0, 0.0 };
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
  auto camera = trace::Camera{ image_width, image_height, lina::Vec3{} };
  auto sampling_rays = camera.GenerateSamplingRays();

  auto scene_components = std::vector<std::unique_ptr<trace::Component>>{};
  scene_components.emplace_back(std::make_unique<trace::Sphere>(lina::Vec3{ 0.0, 0.0, -1.0 }, 0.5));// sphere
  scene_components.emplace_back(std::make_unique<trace::Sphere>(lina::Vec3{ 0.0, -100.5, -1.0 }, 100));// world

  auto randomDevice = std::random_device{};
  auto randomGenerator = std::mt19937{ randomDevice() };

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
  for (auto i = size_t{ 0 }; i < image_height; ++i) {
    for (auto j = size_t{ 0 }; j < image_width; ++j) {
      auto ray = sampling_rays[i][j];
      auto color = ray_color(ray, scene_components, randomGenerator, 10);
      write_color(color);
    }
  }

  return 0;
}
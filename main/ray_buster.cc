#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/camera.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/sphere.h"
#include "lib/trace/ray.h"

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

auto ray_color(trace::Ray const& ray, std::vector<std::unique_ptr<trace::Component>> const& scene_components)
  -> lina::Vec3
{
  auto collision = closest_collision(ray, scene_components);
  if (collision) {
    return 0.5
           * lina::Vec3{
               collision.value().normal[0] + 1.0, collision.value().normal[1] + 1.0, collision.value().normal[2] + 1.0
             };
  }

  auto a = 0.5 * (ray.Direction()[1] + 1.0);
  return (1.0 - a) * lina::Vec3{ 1.0, 1.0, 1.0 } + a * lina::Vec3{ 0.5, 0.7, 1.0 };
}

auto write_color(lina::Vec3 const& color)
{
  std::cout << static_cast<int>(255.9999 * color[0]) << " " << static_cast<int>(255.9999 * color[1]) << " "
            << static_cast<int>(255.9999 * color[2]) << std::endl;
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
  scene_components.emplace_back(std::make_unique<trace::Sphere>(lina::Vec3{ 0.0, -105.5, -1.0 }, 100));// world

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
  for (auto i = size_t{ 0 }; i < image_height; ++i) {
    for (auto j = size_t{ 0 }; j < image_width; ++j) {
      auto ray = sampling_rays[i][j];
      auto color = ray_color(ray, scene_components);
      write_color(color);
    }
  }

  return 0;
}
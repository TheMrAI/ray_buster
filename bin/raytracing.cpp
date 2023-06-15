#include "axis_aligned_rectangle.hpp"
#include "box.hpp"
#include "camera.hpp"
#include "constant_medium.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "moving_sphere.hpp"
#include "perlin.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "texture.hpp"
#include "util.hpp"
#include "vec3.hpp"

#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <numbers>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

template<typename Output, typename Iter>
  requires std::derived_from<Output, std::ostream> && std::weakly_incrementable<Iter> && std::indirectly_readable<Iter>
auto dump_to(Output& output, Iter begin, Iter end)
{
  while (begin != end) {
    output << *begin;
    if (std::next(begin) != end) { output << ' '; }
    ++begin;
  }
}

auto hit_sphere(const vec3& center, double radius, const ray& r) -> double
{
  vec3 oc = r.origin() - center;
  auto r_length = r.direction().length();
  auto a = r_length * r_length;
  auto half_b = dot(oc, r.direction());
  auto oc_length = oc.length();
  auto c = oc_length * oc_length - radius * radius;
  auto discriminant = half_b * half_b - a * c;

  if (discriminant < 0) {
    return -1.0;
  } else {
    return (-half_b - sqrt(discriminant)) / a;
  }
}

vec3 ray_color(ray const& r, vec3 const& background_color, hittable const& world, int depth)
{
  // At limit return blackness
  if (depth <= 0) { return vec3(0, 0, 0); }

  hit_record rec;
  // If the ray hits nothing, return the background color
  if (!world.hit(r, 0.001, std::numeric_limits<double>::infinity(), rec)) { return background_color; }

  auto scattered = ray{};
  auto attenuation = vec3{};
  auto emitted = rec.material_ptr->emitted(rec.u, rec.v, rec.p);

  if (!rec.material_ptr->scatter(r, rec, attenuation, scattered)) { return emitted; }

  return emitted + attenuation * ray_color(scattered, background_color, world, depth - 1);
}

struct SceneConfig
{
  hittable_list world;
  vec3 background_color;
  vec3 look_from;
  vec3 look_at;
  vec3 view_up;
  double v_fow;
  double dist_to_focus;
  double aperture;
};

auto random_scene() -> SceneConfig
{
  hittable_list world;

  auto checker_ground = std::make_shared<checker_texture>(vec3{ 0.2, 0.3, 0.1 }, vec3{ 0.9, 0.9, 0.9 });
  world.add(std::make_shared<sphere>(vec3(0, -1000, 0), 1000, std::make_shared<lambertian>(checker_ground)));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      auto center = vec3{ a + 0.9 * random_double(), 0.2, b + 0.9 * random_double() };

      if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
        std::shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = vec3::random() * vec3::random();
          sphere_material = std::make_shared<lambertian>(albedo);
          auto end_center = center + vec3(0.0, random_double(0, 0.5), 0.0);
          world.add(std::make_shared<moving_sphere>(center, end_center, 0.0, 1.0, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = vec3::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = std::make_shared<metal>(albedo, fuzz);
          world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = std::make_shared<dielectric>(1.5);
          world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = std::make_shared<dielectric>(1.5);
  world.add(std::make_shared<sphere>(vec3(0, 1, 0), 1.0, material1));

  auto material2 = std::make_shared<lambertian>(vec3(0.4, 0.2, 0.1));
  world.add(std::make_shared<sphere>(vec3(-4, 1, 0), 1.0, material2));

  auto material3 = std::make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0);
  world.add(std::make_shared<sphere>(vec3(4, 1, 0), 1.0, material3));

  return SceneConfig{
    world, vec3(0.70, 0.80, 1.00), vec3{ 13.0, 2.0, 3.0 }, vec3{ 0.0, 0.0, 0.0 }, vec3{ 0.0, 1.0, 0.0 }, 20.0, 10.0, 0.1
  };
}

auto two_spheres() -> SceneConfig
{
  hittable_list world;

  auto checker = std::make_shared<checker_texture>(vec3(0.2, 0.3, 0.1), vec3(0.9, 0.9, 0.9));

  world.add(std::make_shared<sphere>(vec3(0, -10, 0), 10, std::make_shared<lambertian>(checker)));
  auto albedo = vec3::random(0.5, 1);
  world.add(std::make_shared<sphere>(vec3(0, 10, 0), 10, std::make_shared<metal>(albedo, 0.2)));

  return SceneConfig{
    world, vec3(0.70, 0.80, 1.00), vec3{ 13.0, 2.0, 3.0 }, vec3{ 0.0, 0.0, 0.0 }, vec3{ 0.0, 1.0, 0.0 }, 20.0, 10.0, 0.0
  };
}

auto two_perlin_spheres() -> SceneConfig
{
  hittable_list world;

  auto perlin_texture = std::make_shared<noise_texture>(4);

  world.add(std::make_shared<sphere>(vec3(0, -1000, 0), 1000, std::make_shared<lambertian>(perlin_texture)));
  world.add(std::make_shared<sphere>(vec3(0, 2, 0), 2, std::make_shared<lambertian>(perlin_texture)));

  return SceneConfig{
    world, vec3(0.70, 0.80, 1.00), vec3{ 13.0, 2.0, 3.0 }, vec3{ 0.0, 0.0, 0.0 }, vec3{ 0.0, 1.0, 0.0 }, 20.0, 10.0, 0.0
  };
}

auto earth() -> SceneConfig
{
  hittable_list world;

  auto earth_texture = std::make_shared<image_texture>("/home/davo/tinker/2k_earth_daymap.jpg");
  auto earth_surface = std::make_shared<lambertian>(earth_texture);
  auto globe = std::make_shared<sphere>(vec3(0.0, 0.0, 0.0), 2, earth_surface);

  world.add(globe);

  return SceneConfig{
    world, vec3(0.70, 0.80, 1.00), vec3{ 13.0, 2.0, 3.0 }, vec3{ 0.0, 0.0, 0.0 }, vec3{ 0.0, 1.0, 0.0 }, 20.0, 10.0, 0.0
  };
}

auto simple_light() -> SceneConfig
{
  hittable_list world;

  auto perlin_texture = std::make_shared<noise_texture>(4);
  world.add(std::make_shared<sphere>(vec3(0, -1000, 0), 1000, std::make_shared<lambertian>(perlin_texture)));
  world.add(std::make_shared<sphere>(vec3(0, 2, 0), 2, std::make_shared<lambertian>(perlin_texture)));

  auto diff_light = std::make_shared<diffuse_light>(vec3{ 4.0, 4.0, 4.0 });
  world.add(std::make_shared<xy_rect>(3.0, 5.0, 1.0, 3.0, -2.0, diff_light));
  auto green_light = std::make_shared<diffuse_light>(vec3{ 4.0, 12.0, 4.0 });
  world.add(std::make_shared<xy_rect>(3.0, 5.0, 1.0, 3.0, 4.0, green_light));

  return SceneConfig{
    world, vec3(0.0, 0.0, 0.0), vec3{ 26.0, 3.0, 6.0 }, vec3{ 0.0, 2.0, 0.0 }, vec3{ 0.0, 1.0, 0.0 }, 20.0, 10.0, 0.0
  };
}

auto cornell_box() -> SceneConfig
{
  hittable_list world;

  auto red = std::make_shared<lambertian>(vec3(.65, .05, .05));
  auto white = std::make_shared<lambertian>(vec3(.73, .73, .73));
  auto green = std::make_shared<lambertian>(vec3(.12, .45, .15));
  auto light = std::make_shared<diffuse_light>(vec3(60, 60, 60));

  world.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, light));
  world.add(std::make_shared<yz_rect>(0, 555, -800, 555, 555, green));
  world.add(std::make_shared<yz_rect>(0, 555, -800, 555, 0, red));
  world.add(std::make_shared<xz_rect>(0, 555, -800, 555, 555, white));// top
  world.add(std::make_shared<xz_rect>(0, 555, -800, 555, 0, white));// bottom
  world.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));// back
  world.add(std::make_shared<xy_rect>(0, 555, 0, 555, -800, white));

  std::shared_ptr<hittable> box_1 = std::make_shared<box>(vec3{ 0, 0, 0 }, vec3{ 165, 330, 165 }, white);
  box_1 = std::make_shared<rotate_y>(box_1, 15);
  box_1 = std::make_shared<translate>(box_1, vec3{ 265.0, 0.0, 295.0 });
  world.add(box_1);

  std::shared_ptr<hittable> box_2 = std::make_shared<box>(vec3{ 0, 0, 0 }, vec3{ 165, 165, 165 }, white);
  box_2 = std::make_shared<rotate_y>(box_2, -18);
  box_2 = std::make_shared<translate>(box_2, vec3{ 130.0, 0.0, 65.0 });
  world.add(box_2);

  return SceneConfig{ world,
    vec3(0.0, 0.0, 0.0),
    vec3{ 278.0, 278.0, -800.0 },
    // vec3{ 278.0, 278.0, -1400.0 },
    vec3{ 278.0, 278.0, 0.0 },
    vec3{ 0.0, 1.0, 0.0 },
    40.0,
    10.0,
    0.0 };
}

auto cornell_box_with_smoke() -> SceneConfig
{
  hittable_list world;

  auto red = std::make_shared<lambertian>(vec3(.65, .05, .05));
  auto white = std::make_shared<lambertian>(vec3(.73, .73, .73));
  auto green = std::make_shared<lambertian>(vec3(.12, .45, .15));
  auto light = std::make_shared<diffuse_light>(vec3(60, 60, 60));

  world.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, light));
  world.add(std::make_shared<yz_rect>(0, 555, -800, 555, 555, green));
  world.add(std::make_shared<yz_rect>(0, 555, -800, 555, 0, red));
  world.add(std::make_shared<xz_rect>(0, 555, -800, 555, 555, white));// top
  world.add(std::make_shared<xz_rect>(0, 555, -800, 555, 0, white));// bottom
  world.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));// back
  world.add(std::make_shared<xy_rect>(0, 555, 0, 555, -800, white));

  std::shared_ptr<hittable> box_1 = std::make_shared<box>(vec3{ 0, 0, 0 }, vec3{ 165, 330, 165 }, white);
  box_1 = std::make_shared<rotate_y>(box_1, 15);
  box_1 = std::make_shared<translate>(box_1, vec3{ 265.0, 0.0, 295.0 });
  world.add(std::make_shared<constant_medium>(box_1, 0.01, vec3{ 0, 0, 0 }));

  std::shared_ptr<hittable> box_2 = std::make_shared<box>(vec3{ 0, 0, 0 }, vec3{ 165, 165, 165 }, white);
  box_2 = std::make_shared<rotate_y>(box_2, -18);
  box_2 = std::make_shared<translate>(box_2, vec3{ 130.0, 0.0, 65.0 });
  world.add(std::make_shared<constant_medium>(box_2, 0.01, vec3{ 1, 1, 1 }));

  return SceneConfig{ world,
    vec3(0.0, 0.0, 0.0),
    vec3{ 278.0, 278.0, -800.0 },
    // vec3{ 278.0, 278.0, -1400.0 },
    vec3{ 278.0, 278.0, 0.0 },
    vec3{ 0.0, 1.0, 0.0 },
    40.0,
    10.0,
    0.0 };
}

auto main() -> int
{
  // Image
  constexpr auto aspect_ratio = 1.0;
  constexpr auto image_width = 600;
  constexpr auto image_height = static_cast<int>(image_width / aspect_ratio);
  constexpr auto samples_per_pixel = 100;
  constexpr auto max_depth = 50;

  // World
  auto scene = cornell_box_with_smoke();

  // Camera
  auto cam = camera{ scene.look_from,
    scene.look_at,
    scene.view_up,
    scene.v_fow,
    aspect_ratio,
    scene.aperture,
    scene.dist_to_focus,
    0.0,
    1.0 };

  // Render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  auto const min_number_of_lines_per_thread = 50u;
  auto const max_threads = (image_height + min_number_of_lines_per_thread - 1u) / min_number_of_lines_per_thread;
  auto const hardware_threads = std::thread::hardware_concurrency();
  auto const number_of_threads = std::min(hardware_threads != 0u ? hardware_threads : 2u, max_threads);
  int const block_size = (image_height + number_of_threads - 1) / number_of_threads;
  std::cerr << "max_threads: " << max_threads << " number of threads: " << number_of_threads << std::endl;

  auto render_chunk = [image_height,
                        image_width,
                        samples_per_pixel,
                        max_depth,
                        &cam,
                        &world = scene.world,
                        &background_color = scene.background_color](int from, int until, bool report = false) {
    auto distance = until - from;
    auto pixel_colors = std::vector<std::vector<vec3>>(distance, std::vector<vec3>(image_width));
    for (auto j = from; j < until; ++j) {
      if (report) { std::cerr << "\rScanlines remaining: " << until - j - 1 << ' ' << std::flush; }
      for (auto i = 0; i < image_width; ++i) {
        auto pixel_color = vec3{ 0, 0, 0 };
        for (auto sample = 0; sample < samples_per_pixel; ++sample) {
          auto u = (i + random_double()) / (image_width - 1);
          auto v = (j + random_double()) / (image_height - 1);
          auto r = cam.get_ray(u, v);
          pixel_color += ray_color(r, background_color, world, max_depth);
        }
        pixel_colors[j - from][i] = prepare_color(pixel_color, samples_per_pixel);
      }
    }
    return pixel_colors;
  };

  auto results = std::vector<std::future<std::vector<std::vector<vec3>>>>(number_of_threads);
  auto threads = std::vector<std::jthread>(number_of_threads - 1);
  for (auto block = 0; block < static_cast<int>(number_of_threads); ++block) {
    auto task = std::packaged_task<std::vector<std::vector<vec3>>(int, int, bool)>(render_chunk);
    results[block] = task.get_future();
    auto until = static_cast<int>(image_height) - block * block_size;
    auto from = std::max(static_cast<int>(image_height) - (block + 1) * block_size, 0);
    if (block < static_cast<int>(number_of_threads) - 1) {
      threads[block] = std::jthread(std::move(task), from, until, false);
    } else {
      task(from, until, true);
    }
  }

  for (auto block = 0u; block < number_of_threads; ++block) {
    auto rendered_chunk = results[block].get();
    for (auto i = 0u; i < rendered_chunk.size(); ++i) {
      for (auto j = 0u; j < rendered_chunk[0].size(); ++j) {
        write_color(std::cout, rendered_chunk[rendered_chunk.size() - 1 - i][j]);
      }
    }
  }

  std::cerr << "\nDone.\n";
  return 0;
}

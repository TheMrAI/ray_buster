#ifndef RAY_BUSTER_LIB_TRACE_CAMERA_H_
#define RAY_BUSTER_LIB_TRACE_CAMERA_H_

#include "lib/lina/vec3.h"
#include "lib/trace/ray.h"
#include <vector>

namespace trace {

class Camera
{
public:
  Camera(std::size_t image_width, std::size_t image_height, lina::Vec3 camera_center)
    : image_width_{ image_width }, image_height_{ image_height }, camera_center_{ std::move(camera_center) }
  {
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * double(image_width) / double(image_height);

    auto viewport_u = lina::Vec3{ viewport_width, 0.0, 0.0 };
    auto viewport_v = lina::Vec3{ 0.0, -viewport_height, 0.0 };

    pixel_delta_u_ = viewport_u / image_width;
    pixel_delta_v_ = viewport_v / image_height;

    auto viewport_upper_left =
      camera_center - lina::Vec3{ 0.0, 0.0, focal_length } - viewport_u / 2.0 - viewport_v / 2.0;

    first_pixel_position_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);
  }

  auto GenerateSamplingRays() -> std::vector<std::vector<trace::Ray>>
  {
    auto sampling_rays = std::vector<std::vector<trace::Ray>>(image_height_);

    for (auto i = size_t{ 0 }; i < image_height_; ++i) {
      sampling_rays[i] = std::vector<trace::Ray>(image_width_);
      for (auto j = size_t{ 0 }; j < image_width_; ++j) {
        auto pixel_center = first_pixel_position_ + (j * pixel_delta_u_) + (i * pixel_delta_v_);
        auto ray_direction = lina::unit(pixel_center - camera_center_);
        sampling_rays[i][j] = trace::Ray{ camera_center_, ray_direction };
      }
    }
    return sampling_rays;
  }

private:
  std::size_t image_width_;
  std::size_t image_height_;
  lina::Vec3 camera_center_;
  lina::Vec3 pixel_delta_u_;
  lina::Vec3 pixel_delta_v_;
  lina::Vec3 first_pixel_position_;
};

}// namespace trace

#endif
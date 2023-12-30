#include <iostream>
#include <cmath>

#include "lib/lina.h"
#include "lib/vec3.h"

class Ray {
public:
    Ray(lina::Vec3 source, lina::Vec3 direction): source_{source}, dir_{direction} {};

    lina::Vec3 const& Source() const {
        return source_;
    }

    lina::Vec3 const& Direction() const {
        return dir_;
    }
private:
    lina::Vec3 source_;
    lina::Vec3 dir_;
};

auto sphere_hit(lina::Vec3 const& center, double radius, Ray const& r) -> double {
    auto oc = r.Source() - center;
    auto a = lina::length_squared(r.Direction().Components());
    auto half_b = dot(oc, r.Direction());
    auto c = lina::length_squared(oc.Components()) - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {
        return -1.0;
    }
    return (-half_b - std::sqrt(discriminant)) / a;
}

auto ray_color(Ray const& r) -> lina::Vec3 {
    auto t = sphere_hit(lina::Vec3{0.0, 0.0, -1.0}, 0.5, r);
    if (t != -1.0) {
        auto hit_point = r.Source() + t*r.Direction();
        auto normal = lina::unit(hit_point - lina::Vec3{0.0, 0.0, -1.0});
        return 0.5*lina::Vec3{normal[0] + 1.0, normal[1] + 1.0, normal[2] + 1.0};
    }

    auto a = 0.5*(r.Direction()[1] + 1.0);
    return (1.0-a)*lina::Vec3{1.0, 1.0, 1.0} + a*lina::Vec3{0.5, 0.7, 1.0};
}

auto write_color(lina::Vec3 const& color) {
    std::cout << static_cast<int>(255.9999 * color[0]) << " " << static_cast<int>(255.9999 * color[1]) << " " << static_cast<int>(255.9999 * color[1]) << std::endl;
}

auto main() -> int {
    auto image_width = size_t{1024};
    auto image_height = size_t{768};

    // auto aspect_ratio = double(image_width) / double(image_height);

    // Camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * double(image_width) / double(image_height);
    auto camera_center = lina::Vec3{0.0, 0.0, 0.0};

    auto viewport_u = lina::Vec3{viewport_width, 0.0, 0.0};
    auto viewport_v = lina::Vec3{0.0, -viewport_height, 0.0};

    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    auto viewport_upper_left = camera_center - lina::Vec3{0.0, 0.0, focal_length} - viewport_u/2.0 - viewport_v/2.0;

    auto first_pixel_position = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (auto i = size_t{0}; i < image_height; ++i) {
        for (auto j = size_t{0}; j < image_width; ++j) {
            auto pixel_center = first_pixel_position + (j * pixel_delta_u) + (i * pixel_delta_v);
            auto ray_direction = lina::unit(pixel_center - camera_center);
            auto ray = Ray{camera_center, ray_direction};
            auto color = ray_color(ray);
            write_color(color);
        }
    }

    return 0;
}
#include "camera.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "util.hpp"

#include <memory>
#include <iostream>

template<typename Output, typename Iter>
requires 
    std::derived_from<Output, std::ostream> &&
    std::weakly_incrementable<Iter> && 
    std::indirectly_readable<Iter>
auto dump_to(Output& output, Iter begin, Iter end) {
    while(begin != end) {
        output << *begin;
        if(std::next(begin) != end) {
            output << ' ';
        }
        ++begin;
    }
}

auto hit_sphere(const vec3& center, double radius, const ray& r) -> double {
    vec3 oc = r.origin() - center;
    auto r_length = r.direction().length();
    auto a = r_length * r_length;
    auto half_b = dot(oc, r.direction());
    auto oc_length = oc.length();
    auto c = oc_length*oc_length - radius*radius;
    auto discriminant = half_b*half_b - a*c;
    
    if(discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

vec3 ray_color(ray const& r, const hittable& world) {
    hit_record rec;
    if(world.hit(r, 0, std::numeric_limits<double>::infinity(), rec)) {
        return 0.5 * (rec.normal + vec3{1,1,1});
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

auto main() -> int {
    // Image
    constexpr auto aspect_ratio = 16.0 / 9.0;
    constexpr auto image_width = 800;
    constexpr auto image_height = static_cast<int>(image_width / aspect_ratio);
    constexpr auto samples_per_pixel = 100;

    // Camera
    auto cam = camera{};

    // World
    hittable_list world;
    world.add(std::make_shared<sphere>(vec3{0,0,-1}, 0.5));
    world.add(std::make_shared<sphere>(vec3{0,-100.5,-1}, 100));

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for(auto j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for(auto i = 0; i < image_width; ++i) {
            auto pixel_color = vec3{0, 0, 0};
            for(auto sample = 0; sample < samples_per_pixel; ++sample) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                auto r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
    return 0;
}

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "vec3.hpp"
#include "ray.hpp"

class camera {
private:
    vec3 origin_;
    vec3 lower_left_corner_;
    vec3 horizontal_;
    vec3 vertical_;
public:
    camera() {
        auto aspect_ratio = 16.0 / 9.0;
        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        origin_ = vec3{0, 0, 0};
        horizontal_ = vec3{viewport_width, 0.0, 0.0};
        vertical_ = vec3{0.0, viewport_height, 0.0};
        lower_left_corner_ = origin_ - horizontal_/2 - vertical_/2 - vec3{0, 0, focal_length};
    }

    auto get_ray(double u, double v) const {
        return ray{origin_, lower_left_corner_ + u*horizontal_ + v*vertical_ - origin_};
    }
};

#endif
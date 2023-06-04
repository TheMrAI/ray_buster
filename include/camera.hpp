#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "vec3.hpp"
#include "ray.hpp"
#include "util.hpp"

class camera {
private:
    vec3 origin_;
    vec3 lower_left_corner_;
    vec3 horizontal_;
    vec3 vertical_;
public:
    camera(vec3 look_from,
        vec3 look_at,
        vec3 view_up,
        double vertical_fow, 
        double aspect_ratio) {
        auto theta = degrees_to_radians(vertical_fow);
        auto h = tan(theta/2.0);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;
        
        auto w = unit_vector(look_from - look_at);
        auto u = unit_vector(cross(view_up, w));
        auto v = cross(w, u);

        origin_ = look_from;
        horizontal_ = viewport_width * u;
        vertical_ = viewport_height * v;
        lower_left_corner_ = origin_ - horizontal_/2.0 - vertical_/2.0 - w;
    }

    auto get_ray(double s, double t) const {
        return ray{origin_, lower_left_corner_ + s*horizontal_ + t*vertical_ - origin_};
    }
};

#endif
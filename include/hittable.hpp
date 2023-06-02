#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "ray.hpp"

struct hit_record {
    vec3 p;
    vec3 normal;
    double t;
    bool front_face;

    void set_face_normal(ray const& r, vec3 const& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
    public:
        virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif
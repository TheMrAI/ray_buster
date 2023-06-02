#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hittable.hpp"
#include "vec3.hpp"

class sphere : public hittable {
public:
    vec3 center_;
    double radius_;

public:
    sphere() {}
    sphere(vec3 center, double radius) : center_{center}, radius_{radius} {};

    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;
};

bool sphere::hit(ray const& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center_;
    auto r_direction_length = r.direction().length();
    auto a = r_direction_length*r_direction_length;
    auto half_b = dot(oc, r.direction());
    auto oc_length = oc.length();
    auto c = oc_length*oc_length - radius_*radius_;

    auto discriminant = half_b*half_b - a*c;
    if(discriminant < 0) {
        return false;
    }
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if(root < t_min || t_max < root) {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    auto outward_normal = (rec.p - center_) / radius_;
    rec.set_face_normal(r, outward_normal);

    return true;
}

#endif
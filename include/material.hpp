#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "ray.hpp"
#include "vec3.hpp"

struct hit_record;

class material {
public:
    virtual bool scatter(ray const& ray_in, hit_record const& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
private:
    vec3 albedo_;
public:
    lambertian(vec3 const& albedo) : albedo_{albedo} {}

    virtual bool scatter(ray const& /*ray_in*/, hit_record const& rec, vec3& attenuation, ray& scattered) const override {
        auto scatter_direction = rec.normal + random_unit_vector();
        
        // Catch degenerate scatter direction
        if(scatter_direction.near_zero()) {
            scatter_direction = rec.normal;
        }

        scattered = ray{rec.p, scatter_direction};
        attenuation = albedo_;
        return true;
    }
};

class metal : public material {
private:
    vec3 albedo_;
    double fuzz_;
public:
    metal(vec3 const& albedo, double fuzz) : albedo_{albedo}, fuzz_(fuzz < 1.0 ? fuzz : 1.0) {}

    virtual bool scatter(ray const& ray_in, hit_record const& rec, vec3& attenuation, ray& scattered) const override {
        auto reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
        scattered = ray{rec.p, reflected + fuzz_*random_in_unit_sphere()};
        attenuation = albedo_;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};

#endif
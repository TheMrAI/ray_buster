#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "ray.hpp"
#include "texture.hpp"
#include "vec3.hpp"

struct hit_record;

class material
{
public:
  virtual bool scatter(ray const& ray_in, hit_record const& rec, vec3& attenuation, ray& scattered) const = 0;
  virtual vec3 emitted(double /*d*/, double /*v*/, vec3 const& /*point*/) const { return vec3{ 0.0, 0.0, 0.0 }; }
};

class lambertian : public material
{
private:
  std::shared_ptr<texture> albedo_;

public:
  lambertian(vec3 const& albedo) : albedo_{ std::make_shared<solid_color>(albedo) } {}
  lambertian(std::shared_ptr<texture> albedo) : albedo_{ albedo } {}

  virtual bool scatter(ray const& ray_in, hit_record const& rec, vec3& attenuation, ray& scattered) const override
  {
    auto scatter_direction = rec.normal + random_unit_vector();

    // Catch degenerate scatter direction
    if (scatter_direction.near_zero()) { scatter_direction = rec.normal; }

    scattered = ray{ rec.p, scatter_direction, ray_in.time() };
    attenuation = albedo_->value(rec.u, rec.v, rec.p);
    return true;
  }
};

class metal : public material
{
private:
  vec3 albedo_;
  double fuzz_;

public:
  metal(vec3 const& albedo, double fuzz) : albedo_{ albedo }, fuzz_(fuzz < 1.0 ? fuzz : 1.0) {}

  virtual bool scatter(ray const& ray_in, hit_record const& rec, vec3& attenuation, ray& scattered) const override
  {
    auto reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
    scattered = ray{ rec.p, reflected + fuzz_ * random_in_unit_sphere(), ray_in.time() };
    attenuation = albedo_;
    return (dot(scattered.direction(), rec.normal) > 0);
  }
};

class dielectric : public material
{
private:
  double index_of_refraction_;

public:
  dielectric(double index_of_refraction) : index_of_refraction_{ index_of_refraction } {}

  virtual bool scatter(ray const& ray_in, hit_record const& rec, vec3& attenuation, ray& scattered) const override
  {
    attenuation = vec3{ 1.0, 1.0, 1.0 };
    auto refraction_ratio = rec.front_face ? (1.0 / index_of_refraction_) : index_of_refraction_;

    auto unit_direction = unit_vector(ray_in.direction());
    auto cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
    auto sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    auto direction = vec3{};

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
      direction = reflect(unit_direction, rec.normal);
    } else {
      direction = refract(unit_direction, rec.normal, refraction_ratio);
    }

    scattered = ray{ rec.p, direction, ray_in.time() };
    return true;
  }

private:
  static double reflectance(double cosine, double ref_index)
  {
    // Use Schlick's approximation for reflecatnce.
    auto r0 = (1.0 - ref_index) / (1.0 + ref_index);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * pow(1.0 - cosine, 5);
  }
};

class diffuse_light : public material
{
private:
  std::shared_ptr<texture> emitted_texture_;

public:
  diffuse_light(vec3 emitted_color) : emitted_texture_{ std::make_shared<solid_color>(emitted_color) } {}
  diffuse_light(std::shared_ptr<texture> emitted_texture) : emitted_texture_{ emitted_texture } {}

  virtual bool scatter(ray const& /*ray_in*/, hit_record const& /*rec*/, vec3& /*attenuation*/, ray& /*scattered*/
  ) const override
  {
    return false;
  }

  virtual vec3 emitted(double u, double v, const vec3& point) const override
  {
    return emitted_texture_->value(u, v, point);
  }
};

class isotropic : public material
{
private:
  std::shared_ptr<texture> albedo_;

public:
  isotropic(vec3 color) : albedo_{ std::make_shared<solid_color>(color) } {}
  isotropic(std::shared_ptr<texture> albedo) : albedo_{ albedo } {}

  virtual bool
    scatter(ray const& incoming_ray, hit_record const& rec, vec3& attenuation, ray& scattered_ray) const override;
};

auto isotropic::scatter(ray const& incoming_ray, hit_record const& rec, vec3& attenuation, ray& scattered_ray) const
  -> bool
{
  scattered_ray = ray{ rec.p, random_in_unit_sphere(), incoming_ray.time() };
  attenuation = albedo_->value(rec.u, rec.v, rec.p);
  return true;
}

#endif
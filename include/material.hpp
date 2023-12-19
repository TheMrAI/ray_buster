#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "onb.hpp"
#include "ray.hpp"
#include "texture.hpp"
#include "vec3.hpp"
#include "pdf.hpp"

struct hit_record;

struct scatter_record {
  ray skip_pdf_ray;
  bool skip_pdf;
  vec3 attenuation;
  std::shared_ptr<pdf> pdf_ptr;
};

class material
{
public:
  virtual auto scatter(ray const& /*r_in*/, hit_record const& /*rec*/, scatter_record& /*srec*/) const -> bool
  {
    return false;
  }

  virtual auto scattering_pdf(ray const& /*r_in*/, hit_record const& /*rec*/, const ray& /*scattered*/) const -> double
  {
    return 0;
  }

  virtual auto emitted(ray const& /*r_in*/, hit_record const& /*rec*/, double /*u*/, double /*v*/, vec3 const& /*point*/)const -> vec3
  {
    return vec3{ 0.0, 0.0, 0.0 };
  }
};

class lambertian : public material
{
private:
  std::shared_ptr<texture> albedo_;

public:
  lambertian(vec3 const& albedo) : albedo_{ std::make_shared<solid_color>(albedo) } {}
  lambertian(std::shared_ptr<texture> albedo) : albedo_{ albedo } {}

  bool scatter(ray const& /*r_in*/, hit_record const& rec, scatter_record& srec) const override
  {
    srec.attenuation = albedo_->value(rec.u, rec.v, rec.p);
    srec.pdf_ptr = std::make_shared<cosine_pdf>(rec.normal);
    srec.skip_pdf = false;
    return true;
  }

  double scattering_pdf(ray const& /*r_in*/, const hit_record& rec, ray const& scattered) const override
  {
    auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
    return cosine < 0 ? 0 : cosine / std::numbers::pi;
  }
};

class metal : public material
{
private:
  vec3 albedo_;
  double fuzz_;

public:
  metal(vec3 const& albedo, double fuzz) : albedo_{ albedo }, fuzz_(fuzz < 1.0 ? fuzz : 1.0) {}

  virtual auto
    scatter(ray const& ray_in, hit_record const& rec, scatter_record& srec) const -> bool override
  {
    srec.attenuation = albedo_;
    srec.pdf_ptr = nullptr;
    srec.skip_pdf = true;
    auto reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
    srec.skip_pdf_ray = ray{rec.p, reflected+fuzz_*random_in_unit_sphere(), ray_in.time()};
    return true;
  }
};

class dielectric : public material
{
private:
  double index_of_refraction_;

public:
  dielectric(double index_of_refraction) : index_of_refraction_{ index_of_refraction } {}

  virtual auto
    scatter(ray const& ray_in, hit_record const& rec, scatter_record& srec) const -> bool override
  {
    srec.attenuation = vec3{ 1.0, 1.0, 1.0 };
    srec.pdf_ptr = nullptr;
    srec.skip_pdf = true;
    auto refraction_ratio = rec.front_face ? (1.0 / index_of_refraction_) : index_of_refraction_;

    auto unit_direction = unit_vector(ray_in.direction());
    auto cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
    auto sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    auto cannot_refract = refraction_ratio * sin_theta > 1.0;
    auto direction = vec3{};

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
      direction = reflect(unit_direction, rec.normal);
    } else {
      direction = refract(unit_direction, rec.normal, refraction_ratio);
    }

    srec.skip_pdf_ray = ray{ rec.p, direction, ray_in.time() };
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

  auto emitted(ray const& /*r_in*/, hit_record const& rec, double u, double v, vec3 const& point)const -> vec3
  {
    if (rec.front_face) { return emitted_texture_->value(u, v, point); }
    return vec3{ 0, 0, 0 };
  }
};

class isotropic : public material
{
private:
  std::shared_ptr<texture> albedo_;

public:
  isotropic(vec3 color) : albedo_{ std::make_shared<solid_color>(color) } {}
  isotropic(std::shared_ptr<texture> albedo) : albedo_{ albedo } {}

  auto scatter(ray const& /*r_in*/, hit_record const& rec, scatter_record& srec) const -> bool override
  {
    srec.attenuation = albedo_->value(rec.u, rec.v, rec.p);
    srec.pdf_ptr = std::make_shared<sphere_pdf>();
    srec.skip_pdf = false;
    return true;
  }

  auto scattering_pdf(ray const& /*r_in*/, hit_record const& /*rec*/, const ray& /*scattered*/) const -> double override
  {
    return 1 / (4*std::numbers::pi);
  }
};

#endif
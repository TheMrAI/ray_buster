#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "perlin.hpp"
#include "vec3.hpp"

class texture
{
public:
  virtual vec3 value(double u, double v, vec3 const& p) const = 0;
};

class solid_color : public texture
{
private:
  vec3 color_;

public:
  solid_color() {}
  solid_color(vec3 color) : color_(color) {}

  solid_color(double red, double green, double blue) : color_{ red, green, blue } {}

  virtual vec3 value(double /*u*/, double /*v*/, vec3 const& /*p*/) const override { return color_; }
};

class checker_texture : public texture
{
private:
  std::shared_ptr<texture> even_;
  std::shared_ptr<texture> odd_;

public:
  checker_texture(){};
  checker_texture(std::shared_ptr<texture> even, std::shared_ptr<texture> odd) : even_{ even }, odd_{ odd } {}
  checker_texture(vec3 const& even_color, vec3 const& odd_color)
    : even_{ std::make_shared<solid_color>(even_color) }, odd_{ std::make_shared<solid_color>(odd_color) }
  {}

  virtual vec3 value(double u, double v, vec3 const& point) const override
  {
    auto sines = sin(10.0 * point.x()) * sin(10.0 * point.y()) * sin(10.0 * point.z());
    if (sines < 0) {
      return odd_->value(u, v, point);
    } else {
      return even_->value(u, v, point);
    }
  }
};

class noise_texture : public texture
{
private:
  perlin<256> noise_;
  double scale_;

public:
  noise_texture(double scale = 1.0) : scale_{ scale } {}

  virtual vec3 value(double /*u*/, double /*v*/, vec3 const& point) const override
  {
    // return vec3(1.0, 1.0, 1.0) * noise_.turb(scale_ * point); // netting
    return vec3(1.0, 1.0, 1.0) * 0.5 * (1.0 + sin(scale_ * point.z() + 10.0 * noise_.turb(point)));// marble
  }
};

#endif
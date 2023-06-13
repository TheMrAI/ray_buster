#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "perlin.hpp"
#include "stb_image.h"
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

class image_texture : public texture
{
private:
  std::unique_ptr<unsigned char> data_;
  size_t width_;
  size_t height_;
  size_t bytes_per_scanline_;

public:
  const static auto bytes_per_pixel = static_cast<size_t>(3);

public:
  image_texture() : data_{ nullptr }, width_{ 0 }, height_{ 0 }, bytes_per_scanline_{ 0 } {}

  image_texture(std::string filename)
  {
    auto components_per_pixel = static_cast<int>(bytes_per_scanline_);

    auto int_width = 0;
    auto int_height = 0;
    data_.reset(stbi_load(filename.c_str(), &int_width, &int_height, &components_per_pixel, components_per_pixel));
    width_ = static_cast<size_t>(int_width);
    height_ = static_cast<size_t>(int_height);

    if (!data_) {
      std::cerr << "ERROR: Could not load texture image file: " << filename << std::endl;
      width_ = 0;
      height_ = 0;
    }

    bytes_per_scanline_ = bytes_per_pixel * width_;
  }

  virtual vec3 value(double u, double v, vec3 const& /*point*/) const override
  {
    // If we have no texture data, then return solid cyan as a debugging aid.
    if (!data_) { return vec3(0, 1, 1); }

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = std::clamp(u, 0.0, 1.0);
    v = 1.0 - std::clamp(v, 0.0, 1.0);// Flip V to image coordinates

    auto i = static_cast<size_t>(u * width_);
    auto j = static_cast<size_t>(v * height_);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= width_) { i = width_ - 1; }
    if (j >= height_) { j = height_ - 1; }

    const auto color_scale = 1.0 / 255.0;
    auto pixel = data_.get() + j * bytes_per_scanline_ + i * bytes_per_pixel;

    return vec3{ color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2] };
  }
};

#endif
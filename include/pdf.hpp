#ifndef PDF_HEADER
#define PDF_HEADER

#include "onb.hpp"
#include "vec3.hpp"
#include <cmath>

class pdf
{
public:
  virtual ~pdf() {}

  virtual auto value(vec3 const& direction) const -> double = 0;
  virtual auto generate() const -> vec3 = 0;
};

class cosine_pdf : public pdf
{
public:
  cosine_pdf(vec3 const& w) : uvw_{ w } {}

  auto value(vec3 const& direction) const -> double override
  {
    auto cosine = dot(unit_vector(direction), uvw_.w());
    return (cosine <= 0) ? 0 : cosine / std::numbers::pi;
  }

  auto generate() const -> vec3 override { return uvw_.local(random_cosine_direction()); }

private:
  onb uvw_;
};

class hittable_pdf : public pdf
{
public:
  hittable_pdf(std::shared_ptr<hittable> hittable, vec3 const& origin) : hittable_{ hittable }, origin_{ origin } {}

  auto value(vec3 const& direction) const -> double override { return hittable_->pdf_value(origin_, direction); }

  auto generate() const -> vec3 override { return hittable_->random(origin_); }

private:
  std::shared_ptr<hittable> hittable_;
  vec3 origin_;
};

class mixture_pdf : public pdf
{
private:
  std::array<std::shared_ptr<pdf>, 2> pdfs_;

public:
  mixture_pdf(std::shared_ptr<pdf> lhs, std::shared_ptr<pdf> rhs) : pdfs_{ lhs, rhs } {}

  auto value(vec3 const& direction) const -> double override
  {
    return 0.5 * pdfs_[0]->value(direction) + 0.5 * pdfs_[1]->value(direction);
  }

  auto generate() const -> vec3 override
  {
    if (random_double() < 0.5) { return pdfs_[0]->generate(); }
    return pdfs_[1]->generate();
  }
};

#endif
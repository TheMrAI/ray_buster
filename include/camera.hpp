#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"
#include "util.hpp"
#include "vec3.hpp"

class camera
{
private:
  vec3 origin_;
  vec3 lower_left_corner_;
  vec3 horizontal_;
  vec3 vertical_;
  vec3 u_;
  vec3 v_;
  vec3 w_;
  double lens_radius_;
  double begin_time_;
  double end_time_;

public:
  camera(vec3 look_from,
    vec3 look_at,
    vec3 view_up,
    double vertical_fow,
    double aspect_ratio,
    double aperture,
    double focus_distance,
    double begin_time,
    double end_time)
  {

    auto theta = degrees_to_radians(vertical_fow);
    auto h = tan(theta / 2.0);
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;

    w_ = unit_vector(look_from - look_at);
    u_ = unit_vector(cross(view_up, w_));
    v_ = cross(w_, u_);

    origin_ = look_from;
    horizontal_ = focus_distance * viewport_width * u_;
    vertical_ = focus_distance * viewport_height * v_;
    lower_left_corner_ = origin_ - horizontal_ / 2.0 - vertical_ / 2.0 - focus_distance * w_;

    lens_radius_ = aperture / 2.0;

    begin_time_ = begin_time;
    end_time_ = end_time;
  }

  auto get_ray(double s, double t) const
  {
    auto rd = lens_radius_ * random_in_unit_disk();
    auto offset = u_ * rd.x() + v_ * rd.y();

    return ray{ origin_ + offset,
      lower_left_corner_ + s * horizontal_ + t * vertical_ - origin_ - offset,
      random_double(begin_time_, end_time_) };
  }
};

#endif
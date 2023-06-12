#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <array>
#include <memory>

#include "util.hpp"
#include "vec3.hpp"

template<size_t PointCount> class perlin
{
private:
  std::array<double, PointCount> random_doubles_;
  std::array<vec3, PointCount> random_vectors_;
  std::array<int, PointCount> perm_x_;
  std::array<int, PointCount> perm_y_;
  std::array<int, PointCount> perm_z_;

public:
  perlin()
  {
    std::transform(random_doubles_.begin(), random_doubles_.end(), random_doubles_.begin(), [](auto /*value*/) {
      return random_double();
    });
    for (auto i = static_cast<size_t>(0); i < PointCount; ++i) {
      random_vectors_[i] = unit_vector(vec3::random(-1.0, 1.0));
    }

    perm_x_ = generate_perlin_permutation();
    perm_y_ = generate_perlin_permutation();
    perm_z_ = generate_perlin_permutation();
  }

  auto noise(vec3 const& point) const -> double
  {
    auto floor_x = floor(point.x());
    auto floor_y = floor(point.y());
    auto floor_z = floor(point.z());

    auto u = point.x() - floor_x;
    auto v = point.y() - floor_y;
    auto w = point.z() - floor_z;

    auto i = static_cast<int>(floor_x);
    auto j = static_cast<int>(floor_y);
    auto k = static_cast<int>(floor_z);

    auto c = std::array<std::array<std::array<vec3, 2>, 2>, 2>{};

    for (auto di = 0; di < 2; ++di) {
      for (auto dj = 0; dj < 2; ++dj) {
        for (auto dk = 0; dk < 2; ++dk) {
          c[di][dj][dk] = random_vectors_[perm_x_[(i + di) & 255] ^ perm_y_[(j + dj) & 255] ^ perm_z_[(k + dk) & 255]];
        }
      }
    }

    return perlin_interp(c, u, v, w);
  }

  auto turb(vec3 point, int depth = 7) const -> double
  {
    auto accum = 0.0;
    auto weight = 1.0;

    while (depth--) {
      accum += weight * noise(point);
      weight *= 0.5;
      point *= 2.0;
    }

    return fabs(accum);
  }

  static std::array<int, PointCount> generate_perlin_permutation()
  {
    auto permutation = std::array<int, PointCount>{};
    std::iota(permutation.begin(), permutation.end(), 0);

    for (auto i = static_cast<int>(permutation.size()) - 1; i >= 0; --i) {
      auto target = random_int(0, i);
      std::swap(permutation[i], permutation[target]);
    }

    return permutation;
  }

private:
  auto perlin_interp(std::array<std::array<std::array<vec3, 2>, 2>, 2> const& c, double u, double v, double w) const
  {
    auto uu = u * u * (3.0 - 2.0 * u);
    auto vv = v * v * (3.0 - 2.0 * v);
    auto ww = w * w * (3.0 - 2.0 * w);
    auto accum = 0.0;

    for (auto i = 0; i < 2; ++i) {
      for (auto j = 0; j < 2; ++j) {
        for (auto k = 0; k < 2; ++k) {
          vec3 weight_v{ u - i, v - j, w - k };
          accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww))
                   * dot(c[i][j][k], weight_v);
        }
      }
    }
    return accum;
  }
};

#endif
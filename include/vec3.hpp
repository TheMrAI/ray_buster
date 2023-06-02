#ifndef VEC3_HPP
#define VEC3_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>

class vec3 {
private:
    std::array<double, 3> data_;
public:
    vec3() : data_{0, 0, 0} {}
    vec3(double x, double y, double z) : data_{x, y, z} {}

    auto x() const -> double {return data_[0];}
    auto y() const -> double {return data_[1];}
    auto z() const -> double {return data_[2];}

    auto x() -> double& {return data_[0];}
    auto y() -> double& {return data_[1];}
    auto z() -> double& {return data_[2];}

    auto operator-() const -> vec3 { return vec3{-data_[0], -data_[1], -data_[2]}; }
    
    auto operator+=(vec3 const& rhs) -> vec3& {
        data_[0] += rhs.data_[0];
        data_[1] += rhs.data_[1];
        data_[2] += rhs.data_[2];
        return *this;
    }

    auto operator-=(vec3 const& rhs) -> vec3& {
        data_[0] -= rhs.data_[0];
        data_[1] -= rhs.data_[1];
        data_[2] -= rhs.data_[2];
        return *this;
    }

    auto operator*=(vec3 const& rhs) -> vec3& {
        data_[0] *= rhs.data_[0];
        data_[1] *= rhs.data_[1];
        data_[2] *= rhs.data_[2];
        return *this;
    }

    auto operator*=(double const& t) -> vec3& {
        data_[0] *= t;
        data_[1] *= t;
        data_[2] *= t;
        return *this;
    }

    auto operator/=(double const t) -> vec3& {
        return *this *= 1/t;
    }

    auto dot(vec3 const& rhs) const -> double {
        return data_[0] * rhs.data_[0]
                + data_[1] * rhs.data_[1]
                + data_[2] * rhs.data_[2];
    }

    auto length() const -> double {
        return std::sqrt(data_[0] * data_[0] + data_[1] * data_[1] + data_[2] * data_[2]);
    }

    friend auto operator<<(std::ostream& out, const vec3& v) -> std::ostream&;
};

auto operator<<(std::ostream& out, const vec3& v) -> std::ostream& {
    out << v.data_[0] << " " << v.data_[1] << " " << v.data_[2];
    return out;
}

auto operator+(vec3 const& lhs, vec3 const& rhs) -> vec3 {
    return vec3{lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z()};
}

auto operator-(vec3 const& lhs, vec3 const& rhs) -> vec3 {
    return vec3{lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z()};
}

auto operator*(vec3 const& lhs, vec3 const& rhs) -> vec3 {
    return vec3{lhs.x() * rhs.x(), lhs.y() * rhs.y(), lhs.z() * rhs.z()};
}

auto operator*(vec3 const& v, double t) -> vec3 {
    return v * t;
}

auto operator*(double t, vec3 const& v) -> vec3 {
    return vec3{v.x() * t, v.y() * t, v.z() * t};
}

auto operator/(vec3 v, double t) -> vec3 {
    return (1.0/t) * v;
}

auto dot(vec3 lhs, vec3 const& rhs) -> double {
    return lhs.dot(rhs);
}

auto cross(vec3 const& lhs, vec3 const& rhs) -> vec3 {
    return vec3{
        lhs.y() * rhs.z() - lhs.z() * rhs.y(),
        lhs.z() * rhs.x() - lhs.x() * rhs.z(),
        lhs.x() * rhs.y() - lhs.y() * rhs.x()
    };
}

auto unit_vector(vec3 v) -> vec3 {
    return v / v.length();
}

// nasty, but ok for now
auto write_color(std::ostream& out, vec3 color, unsigned samples_per_pixel) {
    auto r = color.x();
    auto g = color.y();
    auto b = color.z();

    // Divide the color by the number of samples
    r /= samples_per_pixel;
    g /= samples_per_pixel;
    b /= samples_per_pixel;

    out << static_cast<int>(256 * std::clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * std::clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * std::clamp(b, 0.0, 0.999)) << std::endl;
}

#endif
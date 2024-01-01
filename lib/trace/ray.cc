#include "ray.h"
#include "lib/lina/vec3.h"

namespace trace {

Ray::Ray() : source_{ lina::Vec3{ 0.0, 0.0, 0.0 } }, dir_{ lina::Vec3{ 0.0, 1.0, 0.0 } } {}
Ray::Ray(lina::Vec3 source, lina::Vec3 direction) : source_{ source }, dir_{ lina::unit(direction) } {}

auto Ray::Source() const -> lina::Vec3 const& { return source_; }
auto Ray::Direction() const -> lina::Vec3 const& { return dir_; }

}// namespace trace

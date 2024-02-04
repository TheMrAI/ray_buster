#include "plane.h"

#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/triangle.h"
#include "lib/trace/pdf.h"
#include "lib/trace/ray.h"
#include "lib/trace/transform.h"
#include "lib/trace/util.h"
#include <format>
#include <optional>
#include <random>
#include <span>

namespace trace {

Plane::Plane(lina::Vec3 center)
  : center_{ center }, triangleStrip_{ lina::Vec3{ -0.5, -0.5, 0.0 },
      lina::Vec3{ -0.5, 0.5, 0.0 },
      lina::Vec3{ 0.5, -0.5, 0.0 },
      lina::Vec3{ 0.5, 0.5, 0.0 } }
{}

auto Plane::Collide(Ray const& ray) const -> std::optional<Collision>
{
  auto closestCollisionData = triangleStripCollide(ray, center_, triangleStrip_);

  if (!closestCollisionData) { return std::optional<Collision>{}; }
  return std::optional<Collision>{ closestCollisionData->first };
}

auto Plane::Transform(std::span<double const, 16> transformationMatrix) -> void
{
  auto center4 = trace::extend3Dto4D(center_, false);
  center_ = trace::cut4Dto3D(lina::mul(transformationMatrix, center4));

  for (auto& vertex : triangleStrip_) {
    auto vertex4 = trace::extend3Dto4D(vertex, true);
    vertex = trace::cut4Dto3D(lina::mul(transformationMatrix, vertex4));
  }
}

auto Plane::SamplingPDF(std::mt19937& randomGenerator, lina::Vec3 const& from) const -> PDF
{
  auto samplingPDF = PDF{};
  samplingPDF.Evaluate = [this, from](lina::Vec3 const& rayDirection) -> double {
    auto collision = this->Collide(Ray{ from, rayDirection });
    if (!collision) { return 0.0; }

    auto delta = collision.value().point - from;
    auto distanceSquared = lina::lengthSquared(delta.Components());
    auto cosine = std::fabs(lina::dot(rayDirection, collision.value().normal));

    // the length of the cross product is equal to the size of the parallelogram described by the vectors
    auto area =
      lina::cross(this->triangleStrip_[1] - this->triangleStrip_[0], this->triangleStrip_[2] - this->triangleStrip_[0])
        .Length();
    return distanceSquared / (cosine * area);
  };

  samplingPDF.GenerateSample = [&randomGenerator, this, from]() -> lina::Vec3 {
    auto Q = this->triangleStrip_[0] + this->center_;
    auto u = this->triangleStrip_[2] - this->triangleStrip_[0];
    auto v = this->triangleStrip_[1] - this->triangleStrip_[0];

    auto onPlane =
      Q + (randomUniformDouble(randomGenerator, 0.0, 1.0) * u) + (randomUniformDouble(randomGenerator, 0.0, 1.0) * v);
    return lina::unit(onPlane - from);
  };
  return samplingPDF;
}

auto buildPlane(lina::Vec3 center, double width, double depth, Axis normalAxis, Orientation orientation) -> Plane
{
  width = std::fabs(width);
  depth = std::fabs(depth);
  if (width < 0.00001 || depth < 0.00001) {
    throw std::logic_error(std::format("Width and depth must be bigger than 0.0. Got: {}, {}", width, depth));
  }
  auto plane = Plane{ lina::Vec3{} };

  auto transformation = trace::scale(lina::Vec3{ width, depth, 0.0 });
  auto radians = trace::degreesToRadians(90.0);
  switch (normalAxis) {
  case Axis::X: {
    if (orientation == Orientation::Aligned) { radians *= -1.0; }
    transformation = lina::mul(rotateAlongY(radians), transformation);
    break;
  }
  case Axis::Y: {
    if (orientation == Orientation::Reverse) { radians *= -1.0; }
    transformation = lina::mul(rotateAlongX(radians), transformation);
    break;
  }
  case Axis::Z: {
    if (orientation == Orientation::Aligned) { break; }
    transformation = lina::mul(rotateAlongX(radians * 2.0), transformation);
    break;
  }
  default:
    throw std::logic_error("Invalid Axis given.");
  }

  transformation = lina::mul(translate(center), transformation);
  plane.Transform(transformation);
  return plane;
}

}// namespace trace

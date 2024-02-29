#include "plane.h"

#include "lib/lina/lina.h"
#include "lib/lina/vec3.h"
#include "lib/trace/collision.h"
#include "lib/trace/geometry/component.h"
#include "lib/trace/geometry/mesh.h"
#include "lib/trace/geometry/triangle_data.h"
#include "lib/trace/geometry/vertex_data.h"
#include "lib/trace/pdf.h"
#include "lib/trace/ray.h"
#include "lib/trace/transform.h"
#include "lib/trace/util.h"

#include <array>
#include <cmath>
#include <cstddef>
#include <format>
#include <optional>
#include <random>
#include <span>
#include <stdexcept>
#include <vector>

namespace trace {

Plane::Plane()
  : Component{ Mesh{ lina::Vec3{ 0.0, 0.0, 0.0 },
    std::vector<lina::Vec3>(4),
    std::vector<VertexData>(),// no need
    std::vector<std::array<std::size_t, 3>>(2),
    std::vector<TriangleData>(2) } }
{
  mesh_.vertices.at(0) = lina::Vec3{ -0.5, -0.5, 0.0 };
  mesh_.vertices.at(1) = lina::Vec3{ -0.5, 0.5, 0.0 };
  mesh_.vertices.at(2) = lina::Vec3{ 0.5, -0.5, 0.0 };
  mesh_.vertices.at(3) = lina::Vec3{ 0.5, 0.5, 0.0 };

  mesh_.triangles.at(0) = std::array<std::size_t, 3>{ 0, 1, 2 };
  mesh_.triangles.at(1) = std::array<std::size_t, 3>{ 2, 1, 3 };
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
    // the order matters for a positive cross product
    auto area =
      lina::cross(this->mesh_.vertices[2] - this->mesh_.vertices[0], this->mesh_.vertices[1] - this->mesh_.vertices[0])
        .Length();
    return distanceSquared / (cosine * area);
  };

  samplingPDF.GenerateSample = [&randomGenerator, this, from]() -> lina::Vec3 {
    auto Q = this->mesh_.vertices[0] + this->mesh_.center;
    auto u = this->mesh_.vertices[2] - this->mesh_.vertices[0];
    auto v = this->mesh_.vertices[1] - this->mesh_.vertices[0];

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
  auto plane = Plane{};

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

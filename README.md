# ray_buster

A ray tracing based benchmarking application both for CPU and GPU.

Huge inspiration and shootout to the motivating source: <https://raytracing.github.io/books/RayTracingInOneWeekend.html#overview>

The scenes should be oriented in a way that the z axis points upwards. This is because, from all 3D editing software this is the most
common orientation, and it feels natural.
Unit length vectors for orthonormal bases will be u, v and w.

Build:

```bash
bazel build -c opt //main:ray_buster
```

Test:

```bash
bazel test --test_output=all //...
```

Run clang-tidy:

```bash
bazel build //... --config clang-tidy
```

## Inspired by

- <https://raytracing.github.io/>

Huge thanks! Not sure I would have started the project without your efforts!

## Other sources

- <https://jo.dreggn.org/home/2007_intrt.pdf>
- <https://64.github.io/cmake-raytracer/>

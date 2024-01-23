# ray_buster

A ray tracing based benchmarking application both for CPU and GPU.

Huge inspiration and shootout to the motivating source: <https://raytracing.github.io/books/RayTracingInOneWeekend.html#overview>

The scenes should be oriented in a way that the z axis points upwards. This is because, from all 3D editing software this is the most
common orientation, and it feels natural.
Unit length vectors for orthonormal bases will be u, v and w.

Build:

```bash
bazelisk build -c opt //main:ray_buster
```

Test:

```bash
bazelisk test --cxxopt=-std=c++2b --test_output=all //lib/lina:lina_test
```

Generate compile_commands.json from Bazel using hedron:

```bash
bazelisk run @hedron_compile_commands//:refresh_all
```

After this one can use clang-tidy.

## Inspired by

- <https://raytracing.github.io/>

Huge thanks! Not sure I would have started the project without your efforts!

## Other sources

- <https://jo.dreggn.org/home/2007_intrt.pdf>
- <https://64.github.io/cmake-raytracer/>

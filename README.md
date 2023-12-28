# ray_buster

A ray tracing based benchmarking application both for CPU and GPU.

Huge inspiration and shootout to the motivating source: <https://raytracing.github.io/books/RayTracingInOneWeekend.html#overview>

Build:

```bash
bazelisk build //main:ray_buster
```

Test:

```bash
bazelisk test --cxxopt=-std=c++23 --test_output=all //lib:lina_test
```

## Inspired by

- <https://raytracing.github.io/>

Huge thanks! Not sure I would have started the project without your efforts!

## Other sources

- <https://jo.dreggn.org/home/2007_intrt.pdf>
- <https://64.github.io/cmake-raytracer/>

# ray_buster

A ray tracing based benchmarking application both for CPU and GPU.

Huge inspiration and shootout to the motivating source: https://raytracing.github.io/books/RayTracingInOneWeekend.html#overview

## Build

Make the make files using Cmake:

```shell
cmake -S . -B ./build/ -DCMAKE_BUILD_TYPE=Debug
```

or

```shell
cmake -S . -B ./build/ -DCMAKE_BUILD_TYPE=Release
```

Then build by:

```shell
cmake --build build/
```

## Base on CMake template:

Cmake template:

- https://github.com/cpp-best-practices/cmake_template

## Inspired by

- https://raytracing.github.io/

Huge thanks! Not sure I would have started the project without your efforts!

## Other sources

- https://jo.dreggn.org/home/2007_intrt.pdf
- https://64.github.io/cmake-raytracer/

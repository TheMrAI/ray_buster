# ray_buster

A simple path tracer application using only the CPU.

Huge inspiration and shootout to the motivating source: <https://raytracing.github.io/books/RayTracingInOneWeekend.html#overview>

The scenes should be oriented in a way that the z axis points upwards. This is because, from all 3D editing software this is the most
common orientation, and it feels natural.

## Build with Docker

To quickly check out the program here are the instructions on how to run the docker setup. This way you don't have to install any compilers
or tooling except for Docker, and then you can run it from linux or windows.

Build the image:

```bash
docker build -t ray_buster .
```

Run the program inside the package with:

```bash
docker run ray_buster ./ray_buster --help
```

This will give you all options and usage examples.

Then to render your first scene, and get the result out of the container you will have to mount a folder into it like so:

```bash
docker run --mount type=bind,source="$(pwd)"/[local directory],target=/[in docker] ray_buster ./ray_buster --scene cornell-box -o /[in docker]/cornell-box.ppm
```

Where **[local directory]** and **[in docker]** should be replaced with the desired paths. The local directory has to be created beforehand, but the one
inside docker will be created automatically.

Example:

```bash
docker run --mount type=bind,source="$(pwd)"/renders,target=/renders ray_buster ./ray_buster --scene cornell-box -o /renders/cornell-box.ppm
```

Now you can open the result at /renders/cornell-box.ppm.

When done purge all unused docker images with:

```bash
docker system prune -a
```

## Build locally

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

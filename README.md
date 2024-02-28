# ray_buster

## Usage

The tool has a simple interface which can be accessed by the common __help__ command:

```bash
$ ./ray_buster --help
ray_buster is a simple CPU based path tracing application, with a number of built in scenes.
The tool will aim to utilize all available CPU cores.

Usage: ray_buster --scene cornell-box [OPTIONS]
Commands:
        --scene         Render the selected scene with its built in default configuration.
        --list          List all available scenes with their default settings displayed as ready to paste arguments for quick override.
                        Example:
                                -r 800x800 -s 100 -d 10 -o  -f 70 -a 0 -m 1
        -h --help       Print out the help message.

For the scene command it is possible to override a number of optional the rendering settings, which are as follow:
        -r <width>x<height>     - the rendering resolution. Example 1024x768
        -s <value>              - sampling value. This controls how many sub pixel samples to take at each ray bounce.
        -d <value>              - ray depth. How many ray bounces are simulated. After 20, it only gives diminishing returns.
        -o <value>              - output file. The result will always use the PPM image format.
        -f <value>              - vertical FOV in degrees.
        -a <value>              - defocus angle. An angle for simulating camera focusing artifacts. A 0.0 disables the features.
        -m <value>              - focus distance. The distance the camera is focusing at.

Example usage:
./ray_buster --scene cornell-box
If the default configuration should be changed the easiest way is to list it with:
./ray_buster --list
Copy the appropriate parameter list and make the necessary modifications:
./ray_buster --scene cornell-box -r 1000x1000 -s 1000 -d 20
Parameters that we don't intend to override don't have to appear as arguments.
```

All scenes have a default configuration. These are set in such a way, that they produce a quick result, in a few minutes, but not
a very good one. The reason is simply complexity. Some scenes can take hours to render in any reasonable detail. To save you some
time all available scenes can be observed on [Scenes](scenes.md) page.

## Build

## Build with Docker

To quickly check out the program here are the instructions on how to run the docker setup. This way you don't have to install any
compilers or tooling except for Docker, and then you can run it from linux or windows.

Build the image:

```bash
docker build -t ray_buster .
```

Run the program inside the docker container with:

```bash
docker run ray_buster ./ray_buster --help
```

Then to render your first scene, and get the result out of the container you will have to mount a folder into it like so:

```bash
docker run --mount type=bind,source="$(pwd)"/[local directory],target=/[in docker] ray_buster ./ray_buster --scene cornell-box -o /[in docker]/cornell-box.ppm
```

Where __[local directory]__ and __[in docker]__ should be replaced with the desired paths. The local directory has to be created
beforehand, but the one inside docker will be created automatically.

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

This would require you installing [bazel](https://bazel.build/). The recommended way of doing so would be using
[bazelisk](https://github.com/bazelbuild/bazelisk).
The compiler used is [GCC](https://gcc.gnu.org/) and the used C++ version is C++20.

### Run build

```bash
bazel build -c opt //main:ray_buster
```

### Run unit tests

```bash
bazel test --test_output=all //...
```

Depends on [gtest]. Dependency handled by the bazel configuration.

### Run clang-tidy

```bash
bazel build //... --config clang-tidy
```

### Auto-format code

```bash
format.sh
```

This would require a [clang]<https://clang.llvm.org/>), [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) installation.
Depends on: [bazel_calng_tidy](https://github.com/erenon/bazel_clang_tidy). Dependency handled by the bazel configuration.

## Shortcomings/areas for improvement

There are two things I don't like in the current implementation. Both of which slow down rendering significantly.

### Sampling PDF

For once the sampling PDF approach is barely functional (don't confuse this with the PDF describing how a material behaves, this is the PDF
describing how one geometry can be sampled from a point). The reason is that it doesn't account for any visibility at all. Can
we even see the shape from the current point? How much of it can we see? We don't account for any of that. Subsequently, the blanket mixed
PDFs we generate from the sampling and material PDFs will inevitably get skewed.
Furthermore, there is no easy way of adding support for other geometries. Figuring out analytically how a shape would project onto a unit
hemisphere is cumbersome.

A possible reasonable looking solution would be to have the sampling hemisphere, subdivided uniformly into X number of triangles. Shoot a
ray through the center of each of these triangles and record what object did we hit. Then we can approximate the solid angle of the hit
geometry by the size of the triangle on the hemisphere. This way we could "color" the triangles based on what object they have hit and
construct our mixed PDF based on that. Similarly, if we decide to generate a random vector towards on visible object we can just, randomly
pick one appropriately colored triangle and generate a random vector within.

If this was fixed, we could sample information dense parts of the scene more frequently. Every visible light source, every metallic or
dielectric material. Allowing the simulation to converge on the result much faster.

### Ray collision

In the original implementation we used the equation of a sphere to detect a collision. Currently, we have a proper tessellated sphere.
On the upside, the sphere can now be linearly transformed as everything else. On the downside every scene that has a sphere that has been
sufficiently tessellated (it's mesh contains hundreds or thousands of triangles), will slow the rendering down. The reason is that, at the
moment when the collision point for a ray is calculated we have to check each triangle, within a geometry to know if the geometry was
actually hit. This can be and is mitigated by bounding each complex mesh within a simple bounding box. If we didn't hit the bounding box,
we don't need to check the actual mesh for collision. As you may imagine, in the original with one equation, deciding if we hit the sphere
was a single evaluation. In the new method it scales linearly with the number of triangles.

The approach works well, until the ray is traveling inside any such mesh. The moment it is inside, the bounding box will always report a
hit, and we always have to check the full mesh. This is reasonable, but very inefficient in practice for example any dielectric spheres, or
scenes that take place inside a sphere.

A possible approach fixing this would be to tessellate the whole scene into voxels, by finding the bounding box for the whole scene. Mind
you we don't have to store all the voxels. We are only
interested in those voxels that contain any geometry. If we have this, all the rays have to check the voxel they are in, if it has any
geometry in it. If it doesn't the ray proceeds to the next voxel. If it has some geometry in it, we only collide the ray with the triangles
that are inside the given voxel. On no hit we proceed, on hit, we have found the match.
This would increase memory usage, but would decouple collision detection's time complexity from the number of triangles. We wouldn't need
any more bounding boxes. The collision detection would speed up massively, because marching through empty voxels would be cheap and finding
the final collision would only require as many checks as necessary.

We could easily tell if we won't hit anything too, by simply checking whether we have reached the boundary of our tessellated
scene.

This approach would also speed up the Sampling PDF generation.

The idea came from [Super Fast Ray Casting in Tiled Worlds using DDA](https://youtu.be/NbSee-XM7WA)
now this may not extrapolate to 3D, but for the moment I don't see why not.

## Acknowledgements

Huge inspiration and shoutout to the motivating source [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html). Without this tutorial I am unsure if I could have gotten this far.

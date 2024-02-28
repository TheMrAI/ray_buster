
# The project

The goal is to make a path tracer that is sped up by Monte Carlo randomized sampling.
It should run on the CPU, which should be maximally utilized, using multithreading.
The tool is not intended for any commercial use, but more of an exploration on what it takes to build a basic path tracer.
Having a GUI is not in the scope, the output will be a simple PPM file.

The path tracer will be a command line tool, on which the user can select from a number of prebuilt scenes, and reconfigure the
and rendering settings. Moving the camera, rendering movement, or dynamically loading scenes/assets is out of scope.
The tool should allow the user to generate quick snapshots for each pre-backed scene, so they have an easier time selecting one of interest.

At all times, the code should focus on simplicity. It will only support simple geometry like spheres,
planes, cuboids. Similarly, the scope of supported material types is constrained to simple lambertian, metallic, dielectric and
emissive. No composite or layered materials will be supported. We do not make a distinction about surface and internal material
properties.

Each object has to be able to be moved, rotated, scaled. These operations should be implemented using simple linear algebra.
Rotation doesn't have to use quaternions or geometric algebra. The simplest linear transformations are satisfactory.
We are ignoring the issue of gimbal locks in this project.

For a consistent and easy mental model we should assume that for every vector the coordinates are x, y, z.
The scene space follows the right-hand orientation. Which means, on your right hand your index finger represents the x axis,
your middle finger represents the y axis and your thumb is the z axis.

Viewed from the top:

```text
y ^
  |
  |
  o - - > x
```

Then the z axis points towards us. Our world space will follow this right-hand orientation with the z axis pointing up.

Rotations defined by positive angles execute counterclockwise rotations around their given axes.

## Dictionary

[Path tracing](https://en.wikipedia.org/wiki/Path_tracing) is a rendering technique, simulating rays of light and their
interaction with the environment. It is based on physical observations, and aims to mimic how light reacts with real world objects
and materials, producing highly realistic final images.

[Monte Carlo](https://en.wikipedia.org/wiki/Monte_Carlo_algorithm) randomized algorithms - are a family of algorithms
that by sampling the input space, approximate the true value of a function. The more samples it takes the better, the estimation
becomes. These algorithms are very good for problems which don't have an analytical solution, or can't even be written down. For
example calculating PI is not really feasible on a computer precisely, because it is an irrational number. Similarly, in our case
path-tracing can't be calculated precisely, because (for the time being at least), we can't account for all effects of light
particle movements, their quantum interference patterns, etc. Resources are limited, so we approximate.

[Homogeneous Coordinates](https://www.tomdalling.com/blog/modern-opengl/explaining-homogenous-coordinates-and-projective-geometry/)
Basically vectors at which we look at in different ways depending on what we would like to do with them. For describing
3 dimensional vectors we use 4 dimensional vectors. This way the fourth dimension can be used to signal with a 1 if it is a point
in space or a 0 if it is just a direction. This approach is very helpful especially for linear transformations, because for
example to have a matrix that could apply translation we need a 4 dimension.

[Linear algebra](https://youtu.be/fNk_zzaMoSs?list=PLZHQObOWTQDPD3MizzM2xVFitgF8hE_ab) - The baseline mathematical tooling,
necessary to work with vectors in any reasonable way. The linked Youtube playlist from
[3Blue1Brown](https://www.youtube.com/@3blue1brown) is hands down the best resource you may need on the subject online.

[Gimbal lock](https://www.youtube.com/watch?v=zc8b2Jo7mno) - A phenomenon where the combination of successive rotations for a
single linear transformation matrix would lose dimensions for the transformation.

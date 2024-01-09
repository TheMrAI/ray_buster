
# The project

The goal is to make a path tracer that is sped up by Monte Carlo randomized algorithm approach.
It should run on the CPU, which should be maximally utilized, using multithreading.
The tool is not intended for any commercial use, but more of an exploration on what it takes to build a basic path tracer.
This is why having a GUI is not in the scope, the output will be a simple PNG file.

The path tracer will be a command line tool, on which the user can select from a number of prebuilt scenes, and reconfigure the
camera position and rendering settings.
The tool should allow the user to generate quick snapshots for each pre-backed scene, so they have an easier time selecting one of interest.

At all times, the code should focus on simplicity, so the final result may be documented and explained in reasonable detail, helping anybody
who may be interested in such a project.
Because of this, it will only support simple geometry like spheres, planes, cubes.
Similarly, the scope of supported material types is constrained to lambertian, metallic, dielectric and emissive.

Each object has to be able to be moved, rotated, scaled. These operations should be implemented using simple linear algebra.
Rotation doesn't have to use quaternions or geometric algebra. The simplest linear transformations are satisfactory.

## Dictionary

[Path tracing](https://en.wikipedia.org/wiki/Path_tracing) is a rendering technique, simulating rays of light and their interaction with the environment.
It is based on physical observations, and aims to mimic how light reacts with real world objects and materials, producing highly realistic final images.

[Monte Carlo](https://en.wikipedia.org/wiki/Monte_Carlo_algorithm) randomized algorithms - Are a family of randomized algorithms that by sampling
the input space, approximate the true value of a function. The more samples it takes the better, the estimation becomes.
These algorithms are very good for problems which don't have an analytical solution, or can't even be written down. For example calculating PI
is not really feasible on a computer precisely, because it is an irrational number. Similarly, in our case path-tracing can't be calculated
precisely, because (for the time being at least), we can't account for all effects of light particle movements, their quantum interference patterns, etc.
Resources are limited, so we approximate.

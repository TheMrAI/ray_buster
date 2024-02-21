# syntax=docker/dockerfile:1
FROM gcc:13.2.0 AS base
# install bazilisk
RUN ["wget", "-P", "/usr/local/bin/", "https://github.com/bazelbuild/bazelisk/releases/download/v1.19.0/bazelisk-linux-amd64"]
# rename executable to bazel for ease of use
RUN ["mv", "/usr/local/bin/bazelisk-linux-amd64", "/usr/local/bin/bazel"]
RUN ["chmod", "a+x", "/usr/local/bin/bazel"]
# set bazel version for bazilisk
ENV USE_BAZEL_VERSION="7.0.2"
# pull the necessary files for the set bazel version
RUN ["bazel", "--version"]
WORKDIR /ray_buster/
COPY . .
# build the project
RUN ["bazel", "build", "-c", "opt", "//main:ray_buster"]
WORKDIR /ray_buster/bazel-bin/main/
# syntax=docker/dockerfile:1
FROM ubuntu:23.10 AS base
RUN ["apt", "update"]
RUN ["apt", "install", "-y", "wget"]
# needed for bazel for pulling bazel_clang_tidy source 
RUN ["apt", "install", "-y", "git"]
# setup gcc-13 and g++-13
RUN ["apt", "install", "-y", "gcc-13", "g++-13"]
RUN ["ln", "-s", "/usr/bin/gcc-13", "/usr/bin/gcc"]
# install LLVM tools (clang-tidy-17, clang-format-17)
RUN ["apt", "install", "-y", "clang-tidy-17"]
RUN ["ln", "-s", "/usr/bin/clang-tidy-17", "/usr/bin/clang-tidy"]
RUN ["apt", "install", "-y", "clang-format-17"]
RUN ["ln", "-s", "/usr/bin/clang-format-17", "/usr/bin/clang-format"]
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

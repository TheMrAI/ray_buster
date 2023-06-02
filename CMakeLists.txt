cmake_minimum_required(VERSION 3.20)

project(ray_bench CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# release flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
# debug flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address")

add_subdirectory(bin)
# add_subdirectory(src)
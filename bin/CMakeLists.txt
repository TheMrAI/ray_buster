cmake_minimum_required(VERSION 3.20)

add_executable(raytracing raytracing.cpp)

target_include_directories(raytracing
    PUBLIC
        ../include/)

target_compile_features(raytracing PUBLIC cxx_std_20)

target_compile_options(raytracing PRIVATE -Wall -Wextra -Werror)
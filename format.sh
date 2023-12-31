#!/bin/bash
clang-format --style=file -i $(find -iname "*\.cc")
clang-format --style=file -i $(find -iname "*\.h")
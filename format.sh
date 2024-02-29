#!/bin/bash
clang-format --style=file --Werror -i $(find -iname "*\.cc") && clang-format --style=file --Werror -i $(find -iname "*\.h")
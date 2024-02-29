#!/bin/bash
clang-format --style=file --Werror --dry-run $(find -iname "*\.cc") && clang-format --style=file --Werror --dry-run $(find -iname "*\.h")
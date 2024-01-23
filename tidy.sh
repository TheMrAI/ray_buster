#!/bin/bash
relative="$(dirname "$0")"
clang-tidy --config-file="$relative/.clang-tidy" -p "$relative/compile_commands.json" $(find -iname "*\.cc")
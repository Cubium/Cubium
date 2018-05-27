#!/bin/bash
# Run clang-tidy on the sources

if ! [ -f compile_commands.json ]
then
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .
fi

find . -iname *.hpp -o -iname *.cpp | xargs clang-tidy -checks=bugprone-* -header-filter=.*

#!/bin/bash
# Run clang-tidy on the sources

find . -iname *.hpp -o -iname *.cpp | xargs clang-tidy -checks=bugprone

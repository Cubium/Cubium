#!/bin/bash
# Recursively clang-format format the entire repository 
find . -iname *.hpp -o -iname *.cpp | xargs clang-format -i

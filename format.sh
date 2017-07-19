# Recursively clang-format format the entire repository 
#!/bin/bash
find . -iname *.hpp -o -iname *.cpp | xargs clang-format -i

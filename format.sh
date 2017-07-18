# Recursively clang-format format the entire repository 
#!/bin/bash
find . -iname *.h -o -iname *.cpp | xargs clang-format -i

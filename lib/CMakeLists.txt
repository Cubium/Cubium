# Compile all .cpp files
file(GLOB_RECURSE SOURCES "*.cpp")

# Create libCubium.a static library
add_library(Cubium STATIC ${SOURCES})

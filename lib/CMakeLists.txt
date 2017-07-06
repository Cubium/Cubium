# Compile all .cpp files
file(GLOB_RECURSE SOURCES "*.cpp")

# Create libOpenSpa.a static library
add_library(OpenSpa STATIC ${SOURCES})

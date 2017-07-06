# Ensure required packages are available
find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

# Allows us to use things like #include <gmock/gmock.h>
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})

# Creates 'make tests' rule
add_executable(tests main.cpp)

# Link libraries to testing executable
target_link_libraries(tests ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES} pthread)
target_link_libraries(tests OpenSpa)

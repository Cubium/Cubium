add_executable(subnet-driver subnet_manager_driver.cpp)

# Link libraries to testing executable
target_link_libraries(subnet-driver Cubium pthread)

add_executable(subnet_driver.out subnet_manager_driver.cpp)

# Link libraries to testing executable
target_link_libraries(subnet_driver.out Cubium pthread)

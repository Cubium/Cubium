add_executable(py-subnet-driver subnet_manager_driver.cpp)

# Link libraries to testing executable
target_link_libraries(py-subnet-driver Cubium pthread)

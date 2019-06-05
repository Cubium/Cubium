cmake_minimum_required(VERSION 3.0)
project(Cubium)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O1")

# Add project specific .cmake(packge location) files to search path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# allows the usage of #include <my_lib_file.hpp> anywhere in project
include_directories(lib)

# Include sub directories, each with their own CMakeLists.txt file
#add_subdirectory(test)
add_subdirectory(lib)

add_subdirectory(simple_system/component-A)
add_subdirectory(simple_system/component-B)
add_subdirectory(simple_system/subnetmanager)
add_subdirectory(python_system/py_component)
add_subdirectory(python_system/subnetmanager)
add_subdirectory(python_system/component-B)

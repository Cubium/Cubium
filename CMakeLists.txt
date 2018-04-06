cmake_minimum_required(VERSION 3.0)
project(Cubium)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

# Add project specific .cmake(packge location) files to search path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# allows the usage of #include <my_lib_file.hpp> anywhere in project
include_directories(lib)

# Include sub directories, each with their own CMakeLists.txt file
#add_subdirectory(test)
add_subdirectory(lib)
#add_subdirectory(simple_system/component-A)
#add_subdirectory(simple_system/component-B)
#add_subdirectory(simple_system/subnetmanager)
#add_subdirectory(drivers/TempSensor)
#add_subdirectory(python_system/py_component)
#add_subdirectory(python_system/subnetmanager)
#add_subdirectory(python_system/component-B)

add_subdirectory(hab/aggregator)
add_subdirectory(hab/baro)
add_subdirectory(hab/boom)
add_subdirectory(hab/camera)
add_subdirectory(hab/filter)
add_subdirectory(hab/gps)
add_subdirectory(hab/gyro)
add_subdirectory(hab/light)
add_subdirectory(hab/radio)
add_subdirectory(hab/rtc)
add_subdirectory(hab/analogTemp)
add_subdirectory(hab/subnetmanager)
add_subdirectory(hab/temp)
add_subdirectory(hab/uv)

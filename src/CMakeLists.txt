cmake_minimum_required(VERSION 3.25)

# project name: testgenerator
# project type: C++ executable
# project version: 0.1
project(testgenerator VERSION 0.1 LANGUAGES CXX)

# source files: testgenerator.cpp
set(SOURCES generators.cpp formatparser.cpp testgenerator.cpp)

# set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

#include dependencies: yaml-cpp
find_package(yaml-cpp REQUIRED)

# add the executable
add_executable(testgenerator ${SOURCES})
target_link_libraries(testgenerator yaml-cpp)

target_link_libraries(testgenerator)


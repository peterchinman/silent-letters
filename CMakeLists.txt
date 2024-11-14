cmake_minimum_required(VERSION 3.14)
project(silent-letters)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# FetchContent setup to download Catch2
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1 
)

FetchContent_MakeAvailable(Catch2)

# Main application
add_subdirectory(src)

# Enable testing
enable_testing()

# Test directory
add_subdirectory(tests)

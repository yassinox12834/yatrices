cmake_minimum_required(VERSION 3.20)
project(Yatrices VERSION 0.0.5 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(YATRICES_BUILD_TESTS    "Build the C++ tests"           ON)
option(YATRICES_BUILD_EXAMPLES "Build the C++ examples"        ON)
option(YATRICES_BUILD_CAPI     "Build the ctypes C API"        OFF)
option(YATRICES_BUILD_PYTHON   "Build the yatricespy module"   OFF)

if(YATRICES_BUILD_TESTS)
    enable_testing()
endif()

add_subdirectory(cpp)

if(YATRICES_BUILD_PYTHON OR YATRICES_BUILD_CAPI)
    add_subdirectory(python)
endif()

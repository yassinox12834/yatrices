cmake_minimum_required(VERSION 3.20)
project(Yatrices VERSION 0.0.1 LANGUAGES CXX)

# Optimisations only happen in Release; make that the default.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options: build only what you need
option(YATRICES_BUILD_TESTS    "Build the C++ tests"           ON)
option(YATRICES_BUILD_EXAMPLES "Build the C++ examples"        ON)
option(YATRICES_BUILD_CAPI     "Build the ctypes C API"        OFF)
option(YATRICES_BUILD_PYTHON   "Build the yatricespy module"   OFF)

if(YATRICES_BUILD_TESTS)
    enable_testing()
endif()

# 1. C++ library: YatricesEngine (knows nothing about Python) ₂³₂³₂³₂³
add_subdirectory(cpp)

# 2. Python library: yatricespy (pybind11 module and optional C API)
if(YATRICES_BUILD_PYTHON OR YATRICES_BUILD_CAPI)
    add_subdirectory(python)
endif()

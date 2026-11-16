# YatricesEngine: pure C++ core, knows nothing about Python.
add_library(YatricesEngine STATIC
    src/errors.cpp
    src/vector.cpp
    src/matrix.cpp
)
add_library(Yatrices::Engine ALIAS YatricesEngine)

target_include_directories(YatricesEngine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_features(YatricesEngine PUBLIC cxx_std_17)
set_target_properties(YatricesEngine PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(MSVC)
    target_compile_options(YatricesEngine PRIVATE /W4)
else()
    target_compile_options(YatricesEngine PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(YATRICES_BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(YATRICES_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

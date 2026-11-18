if(YATRICES_BUILD_PYTHON)
    find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
    find_package(pybind11 CONFIG REQUIRED)
    pybind11_add_module(_yatricespy bindings/bindings.cpp)
    target_link_libraries(_yatricespy PRIVATE Yatrices::Engine)

    if(MINGW)
        target_link_options(_yatricespy PRIVATE -static -static-libgcc -static-libstdc++)
    endif()

    install(TARGETS _yatricespy DESTINATION yatricespy)
endif()

if(YATRICES_BUILD_CAPI)
    add_subdirectory(capi)
endif()

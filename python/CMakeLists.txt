# yatricespy: the Python front end of YatricesEngine.

if(YATRICES_BUILD_PYTHON)
    find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
    find_package(pybind11 CONFIG REQUIRED)
    pybind11_add_module(_yatricespy bindings/bindings.cpp)
    target_link_libraries(_yatricespy PRIVATE Yatrices::Engine)

    # MinGW (MSYS2 g++) only: bundle the compiler runtime (libstdc++, libgcc, winpthread)
    # inside the module. Python never looks in C:/msys64/ucrt64/bin for the DLLs a module
    # needs, so without this, "import yatricespy" fails with "DLL load failed".
    if(MINGW)
        target_link_options(_yatricespy PRIVATE -static -static-libgcc -static-libstdc++)
    endif()

    install(TARGETS _yatricespy DESTINATION yatricespy)
endif()

if(YATRICES_BUILD_CAPI)
    add_subdirectory(capi)
endif()

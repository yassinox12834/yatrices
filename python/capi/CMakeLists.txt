add_library(yatrices_c SHARED capi.cpp)
target_link_libraries(yatrices_c PRIVATE Yatrices::Engine)
target_include_directories(yatrices_c PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

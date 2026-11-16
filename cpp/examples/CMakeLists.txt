add_executable(example_float_sum float_sum.cpp)
target_link_libraries(example_float_sum PRIVATE Yatrices::Engine)
add_executable(example_vector_ops vector_ops.cpp)
target_link_libraries(example_vector_ops PRIVATE Yatrices::Engine)

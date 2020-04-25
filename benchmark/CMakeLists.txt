add_executable(variant_cpp11_bench_assignment assignment_test.cpp)
target_link_libraries(variant_cpp11_bench_assignment PRIVATE benchmark_main
                                                             variant-cpp11)

add_executable(variant_cpp11_bench_get get_test.cpp)
target_link_libraries(variant_cpp11_bench_get PRIVATE benchmark_main
                                                      variant-cpp11)

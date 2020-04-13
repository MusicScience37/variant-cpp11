add_executable(variant_cpp11_test main.cpp max_size_test.cpp)
target_link_libraries(variant_cpp11_test PRIVATE variant-cpp11)
target_link_system_library(variant_cpp11_test PRIVATE Catch2::Catch2)
target_check_clang_tidy(variant_cpp11_test)

if(VARIANT_CPP11_WRITE_JUNIT)
    set(CATCH2_REPORT_OPTIONS -r junit -o)
else()
    set(CATCH2_REPORT_OPTIONS -r compact)
endif()
add_test(NAME variant_cpp11_test COMMAND variant_cpp11_test
                                         ${CATCH2_REPORT_OPTIONS})

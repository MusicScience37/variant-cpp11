cmake_minimum_required(VERSION 3.12)

project(
    variant-cpp11
    VERSION 0.0
    DESCRIPTION "Implementation of variant class in C++11"
    LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

add_library(variant-cpp11 INTERFACE)
target_include_directories(variant-cpp11 INTERFACE include)

option(VARIANT_CPP11_TESTING "test variant-cpp11" OFF)
if(VARIANT_CPP11_TESTING)
    add_subdirectory(extern/Catch2)
    enable_testing()
    add_subdirectory(tests)
endif()

cmake_minimum_required(VERSION 3.12)

project(
    variant_cpp11
    VERSION 0.0
    DESCRIPTION "Implementation of variant class in C++11"
    LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

set(VARIANT_CPP11_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(TargetLinkSystemLibrary)
include(ConfigureClangTidy)

add_library(variant-cpp11 INTERFACE)
target_include_directories(variant-cpp11 INTERFACE include)

option(VARIANT_CPP11_BUILD_DOC "build documentation of variant-cpp11" OFF)
if(VARIANT_CPP11_BUILD_DOC)
    add_subdirectory(doc)
endif()

option(VARIANT_CPP11_TESTING "test variant-cpp11" OFF)
if(VARIANT_CPP11_TESTING)
    add_subdirectory(extern/Catch2)
    enable_testing()
    add_subdirectory(tests)
endif()

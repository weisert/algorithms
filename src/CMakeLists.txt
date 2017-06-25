cmake_minimum_required(VERSION 2.6.2)

project(algorithms)

set (CMAKE_CXX_STANDARD 14)

add_subdirectory(third_party/gtest)

include_directories(.)
include_directories(third_party/gtest/googletest/include)
include_directories(third_party/gtest/googlemock/include)

config_compiler_and_linker()  # Defined in third_party/gtest/googletest/cmake/internal_utils.cmake

set( sources
  strings/kmp.cc
)

set( test_sources
  strings/kmp_unittest.cc
)

add_executable(unit_tests ${sources} ${test_sources})
target_link_libraries(unit_tests gtest gmock gmock_main)

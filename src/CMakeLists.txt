cmake_minimum_required(VERSION 2.6.2)

project(algorithms)

add_subdirectory(third_party/gtest)

include_directories(third_party/gtest/googletest/include)
include_directories(third_party/gtest/googlemock/include)

config_compiler_and_linker()  # Defined in third_party/gtest/googletest/cmake/internal_utils.cmake

set(sources
    test.cc
)

add_executable(unit_tests ${sources})
target_link_libraries(unit_tests gtest gmock gmock_main)

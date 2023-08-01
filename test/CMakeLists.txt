cmake_minimum_required(VERSION 3.20)

project(json_test)

set(SOURCE_FILES "simple_parse_test.cpp" value_compare_test.cpp simple_dump_test.cpp simple_throw_parse_test.cpp)

add_executable(json_test ${SOURCE_FILES})

find_package(GTest REQUIRED)

include_directories("../lib/include")

target_link_libraries(json_test json GTest::gtest GTest::gtest_main)

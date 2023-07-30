cmake_minimum_required(VERSION 3.20)

project(json_test)

set(SOURCE_FILES "main.cpp")

add_executable(json_test ${SOURCE_FILES})

include_directories("../lib/include")

target_link_libraries(json_test json)
cmake_minimum_required(VERSION 3.20)
project(json_parser)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(test json.cpp main.cpp)


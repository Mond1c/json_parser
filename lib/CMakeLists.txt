cmake_minimum_required(VERSION 3.20)

project(json)

set(SOURCE_FILES "json.cpp")
set(HEADER_FILES "include/json.h")

add_library(json STATIC ${HEADER_FILES} ${SOURCE_FILES})

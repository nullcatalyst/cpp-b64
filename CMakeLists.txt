cmake_minimum_required(VERSION 3.10)

project(b64)
set(CMAKE_CXX_STANDARD 17)

add_library(b64 "")

target_include_directories(b64
PUBLIC
    include
PRIVATE
    src
)

target_sources(b64
PRIVATE
    src/b64.cpp
)

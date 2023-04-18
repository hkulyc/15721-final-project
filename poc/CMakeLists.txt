cmake_minimum_required(VERSION 3.17)
project(poc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 -Wall -mmacosx-version-min=11.6")


include_directories(${CMAKE_SOURCE_DIR}/dependencies/libduckdb-osx-universal2) # *.h
link_directories(${CMAKE_SOURCE_DIR}/dependencies/libduckdb-osx-universal2)    # *.dylib

add_executable(poc test.cpp)

target_link_libraries(poc duckdb)
cmake_minimum_required(VERSION 3.17)
project(poc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 -Wall")

include_directories(${CMAKE_SOURCE_DIR}/dependencies/libduckdb-osx-universal) # *.h
link_directories(${CMAKE_SOURCE_DIR}/dependencies/libduckdb-osx-universal)    # *.dylib

# include_directories(${CMAKE_SOURCE_DIR}/dependencies/libduckdb-linux-amd64) # *.h
# link_directories(${CMAKE_SOURCE_DIR}/dependencies/libduckdb-linux-amd64)    # *.dylib

add_executable(poc test.cpp conn_pool.cpp)

target_link_libraries(poc duckdb)
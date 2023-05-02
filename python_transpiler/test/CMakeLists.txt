cmake_minimum_required(VERSION 3.17)
project(udftranspiler-test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O2 -Wall")


include_directories(${CMAKE_SOURCE_DIR}/dependencies/libduckdb-osx-universal) # *.h
link_directories(${CMAKE_SOURCE_DIR}/dependencies/libduckdb-osx-universal)    # *.dylib

# include_directories(${CMAKE_SOURCE_DIR}/dependencies/libduckdb-linux-amd64) # *.h
# link_directories(${CMAKE_SOURCE_DIR}/dependencies/libduckdb-linux-amd64)    # *.dylib

add_executable(test test.cpp udf.cpp)

target_link_libraries(test duckdb)
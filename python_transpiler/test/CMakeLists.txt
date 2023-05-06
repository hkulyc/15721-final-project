cmake_minimum_required(VERSION 3.17)
project(udftranspiler-test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O2 -Wall")


# include_directories(${CMAKE_SOURCE_DIR}/dependencies/libduckdb-osx-universal) # *.h
# link_directories(${CMAKE_SOURCE_DIR}/dependencies/libduckdb-osx-universal)    # *.dylib

include_directories(${CMAKE_SOURCE_DIR}/dependencies/libduckdb-src) # *.h
# link_directories(${CMAKE_SOURCE_DIR}/dependencies/libduckdb-src)    # *.dylib

add_executable(test test.cpp udf.cpp ${CMAKE_SOURCE_DIR}/dependencies/libduckdb-src/duckdb.cpp)

target_link_libraries(test)
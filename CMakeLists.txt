cmake_minimum_required(VERSION 3.16)
project(Biosphere VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*/*.cpp)

add_executable(Biosphere
    ${SOURCES}
)
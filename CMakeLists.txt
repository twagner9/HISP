cmake_minimum_required(VERSION 3.25.1) 
project(HeterogeneousImageProcessingPipeline)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(extern/CLI11)
add_executable(HISP)
target_sources(HISP
    PRIVATE
        src/filters.cpp
        src/image.cpp
        src/main.cpp)

if(NOT DEFINED CLI11_DIR)
set(CLI11_DIR "${CMAKE_SOURCE_DIR}/extern/CLI11" CACHE STRING "CLI git repository")
endif()

include_directories(${CLI11_DIR}/include)
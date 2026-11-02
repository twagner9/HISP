cmake_minimum_required(VERSION 3.22) 
project(HeterogeneousImageProcessingPipeline)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(extern/CLI11)
add_library(HISPImageProcessing SHARED src/filters.cpp src/image.cpp src/filters_c_api.cpp
    headers/filters_c_api.hpp)
add_executable(HISP)
target_sources(HISP
    PRIVATE
        src/filters.cpp
        src/image.cpp
        src/main.cpp)

if(NOT DEFINED CLI11_DIR)
set(CLI11_DIR "${CMAKE_SOURCE_DIR}/extern/CLI11" CACHE STRING "CLI git repository")
endif()

target_include_directories(HISP PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/extern/stb)
target_include_directories(HISP PRIVATE ${CMAKE_ROOT}/headers)
include_directories(${CLI11_DIR}/include)
cmake_minimum_required(VERSION 3.22.1)

project(
    Toy
    VERSION 0.0.1
    DESCRIPTION "Toy"
    HOMEPAGE_URL ""
    LANGUAGES CXX C)

message(STATUS "system info: " ${CMAKE_SYSTEM})    

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
message(STATUS "Cpp language standard: " ${CMAKE_CXX_STANDARD})

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE "Debug")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(PROJECT_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${PROJECT_OUTPUT_DIR})
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/install)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_OUTPUT_DIR})
message(STATUS "PROJECT_OUTPUT_DIR: " ${PROJECT_OUTPUT_DIR})

include_directories(${PROJECT_SOURCE_DIR}/interface/)


#add_definitions(-DDYNAMIC_DLL)

add_subdirectory(plugin)
add_subdirectory(main)
add_subdirectory(interface)
add_subdirectory(core)


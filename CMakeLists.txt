cmake_minimum_required(VERSION 3.30)
project(MathLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_ASM_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(TEST_TARGET_NAME "Tests")
include(CheckIPOSupported)

add_library(${PROJECT_NAME} STATIC)
add_subdirectory(src)

option(BUILD_TESTING "Build the test suite" OFF)

if(BUILD_TESTING)
    enable_testing()
    add_executable(${TEST_TARGET_NAME})
    add_subdirectory(tests)
endif()
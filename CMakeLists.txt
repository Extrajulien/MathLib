cmake_minimum_required(VERSION 3.30)
project(MathLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_ASM_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(TEST_TARGET_NAME "Tests")
include(CheckIPOSupported)

add_library(${PROJECT_NAME} STATIC)
add_subdirectory(src)

# if MathLib is the main project build the tests
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    option(MATHLIB_BUILD_TESTING "Build MathLib test suite" ON)
else()
    option(MATHLIB_BUILD_TESTING "Build MathLib test suite" OFF)
endif()

if(MATHLIB_BUILD_TESTING)
    enable_testing()
    add_executable(${TEST_TARGET_NAME})
    add_subdirectory(tests)
endif()
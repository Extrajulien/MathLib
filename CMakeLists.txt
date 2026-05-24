cmake_minimum_required(VERSION 4.0)
project(MathLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_ASM_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET CXX_MODULES FILES
        src/linear_algebra/Vector2.cppm
        src/linear_algebra/Vector3.cppm
        src/linear_algebra/Vector4.cppm
        src/internal/math_helpers.cppm
        src/MathLib.cppm
        PRIVATE
        # hidden cpp files
)

option(BUILD_TESTING "Build the test suite" ON)

if(BUILD_TESTING)
    enable_testing()

    add_executable(Tests
            tests/test_main.cpp
            tests/linear_algebra/Vector2_tests.cpp
            tests/linear_algebra/Vector3_tests.cpp
    )

    # 4. Point the test executable to doctest and your library headers
    target_include_directories(Tests PRIVATE tests/)

    include(FetchContent)

    FetchContent_Declare(
            doctest
            GIT_REPOSITORY https://github.com/doctest/doctest.git
            GIT_TAG        v2.5.2
    )
    FetchContent_MakeAvailable(doctest)

    target_link_libraries(Tests PRIVATE ${PROJECT_NAME} PRIVATE doctest)

    # 6. Register it with ctest so you can just run 'ctest' in the terminal
    add_test(NAME RunMathTests COMMAND Tests)
endif()
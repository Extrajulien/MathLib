include(FetchContent)

target_sources(${TEST_TARGET_NAME}
        PUBLIC
        test_main.cpp
        linear_algebra/Vector2_tests.cpp
        linear_algebra/Vector3_tests.cpp
        linear_algebra/Vector4_tests.cpp
        linear_algebra/Matrix4_tests.cpp
        linear_algebra/Factory_tests.cpp
        trigonometry/Angle_tests.cpp
)

FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG        v2.5.2
)
FetchContent_MakeAvailable(doctest)

target_link_libraries(Tests PRIVATE ${PROJECT_NAME} PRIVATE doctest)

# Register to use with CTests
add_test(NAME RunMathTests COMMAND Tests)
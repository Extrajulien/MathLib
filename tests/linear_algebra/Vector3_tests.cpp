#include <cmath>
#include <array>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "doctest.h"

import MathLib;

using MathLib::Vector3;
using MathLib::Vector3i;
using MathLib::Vector3f;
using MathLib::Vector3d;

// Helper function for approximate vector comparison to avoid repeated boilerplate.
template<typename T, typename U>
bool is_approx_equal(const Vector3<T>& actual, U expected_x, U expected_y, U expected_z) {
    return actual.x == doctest::Approx(expected_x) &&
           actual.y == doctest::Approx(expected_y) &&
           actual.z == doctest::Approx(expected_z);
}

TEST_CASE("Vector3 Construction") {
    SUBCASE("Construction from components") {
        Vector3i vecI(1, 2, 3);
        CHECK(vecI.x == 1);
        CHECK(vecI.y == 2);
        CHECK(vecI.z == 3);

        Vector3f vecF(1.5f, 2.5f, 3.5f);
        CHECK(vecF.x == 1.5f);
        CHECK(vecF.y == 2.5f);
        CHECK(vecF.z == 3.5f);

        Vector3d vecD(-1.0, 3.14, 2.71);
        CHECK(vecD.x == -1.0);
        CHECK(vecD.y == 3.14);
        CHECK(vecD.z == 2.71);
    }

    SUBCASE("Construction from std::array") {
        std::array<int, 3> arrI = {10, 20, 30};
        Vector3i vecI(arrI);
        CHECK(vecI.x == 10);
        CHECK(vecI.y == 20);
        CHECK(vecI.z == 30);

        std::array<double, 3> arrD = {0.5, -0.5, 0.25};
        Vector3d vecD(arrD);
        CHECK(vecD.x == 0.5);
        CHECK(vecD.y == -0.5);
        CHECK(vecD.z == 0.25);
    }
}
TEST_CASE("Vector3 Conversion and Assignment") {
    SUBCASE("to_array()") {
        Vector3i v(3, 4, 5);
        auto arr = v.to_array();
        CHECK(arr.size() == 3);
        CHECK(arr[0] == 3);
        CHECK(arr[1] == 4);
        CHECK(arr[2] == 5);
    }

    SUBCASE("Assignment from std::array") {
        Vector3f v(0.0f, 0.0f, 0.0f);
        std::array<float, 3> arr = {1.1f, 2.2f, 3.3f};
        v = arr;
        CHECK(v.x == 1.1f);
        CHECK(v.y == 2.2f);
        CHECK(v.z == 3.3f);

        // Mixed type assignment from array
        std::array<int, 3> arrI = {5, 6, 7};
        v = arrI;
        CHECK(v.x == 5.0f);
        CHECK(v.y == 6.0f);
        CHECK(v.z == 7.0f);
    }

    SUBCASE("Copy assignment from same type") {
        Vector3i source(10, 20, 30);
        Vector3i destination(0, 0, 0);

        destination = source;

        CHECK(destination == source);
        CHECK(destination.x == 10);
        CHECK(destination.y == 20);
        CHECK(destination.z == 30);
    }

    SUBCASE("Cross-type assignment (Float to Int conversion)") {
        Vector3f float_vec(5.7f, 9.2f, 12.9f);
        Vector3i int_vec(0, 0, 0);

        int_vec = float_vec;

        // This verifies truncation behavior during assignment
        CHECK(int_vec == Vector3i(5, 9, 12));
        CHECK_FALSE(int_vec == Vector3i(6, 9, 12));
    }
}

TEST_CASE("Vector3 Equality and Inequality") {
    SUBCASE("Identical vectors should be equal") {
        CHECK(Vector3i(1, 1, 1) == Vector3i(1, 1, 1));
        CHECK(Vector3f(1.0f, 1.0f, 1.0f) == Vector3f(1.0f, 1.0f, 1.0f));
        CHECK(Vector3d(1.0, 1.0, 1.0) == Vector3d(1.0, 1.0, 1.0));
    }

    SUBCASE("Different vectors must fail equality (Negative Testing)") {
        CHECK(Vector3i(1, 2, 3) != Vector3i(1, 1, 3));
        CHECK(Vector3i(1, 2, 3) != Vector3i(1, 2, 1));
        CHECK(Vector3i(1, 2, 3) != Vector3i(0, 2, 3));
        CHECK_FALSE(Vector3i(1, 2, 3) == Vector3i(1, 1, 3));
        CHECK_FALSE(Vector3f(5.0f, 0.0f, 1.0f) == Vector3f(5.0f, 0.001f, 1.0f));
    }

    SUBCASE("Mixed type equality") {
        CHECK(Vector3i(1, 2, 3) == Vector3d(1.0, 2.0, 3.0));
        CHECK(Vector3i(3, 7, 1) != Vector3d(3.7, 7.3, 1.0));
        CHECK(Vector3f(1.5f, 2.5f, 0.5f) != Vector3i(1, 2, 0));
    }
}

TEST_CASE("Vector3 Arithmetic Operators") {
    SUBCASE("Addition") {
        auto vec1 = Vector3i(1, 2, 3);
        auto vec2 = Vector3i(4, 5, 6);
        auto res = vec1 + vec2;
        CHECK(res.x == 5);
        CHECK(res.y == 7);
        CHECK(res.z == 9);

        // Mixed type: int + double -> double
        auto vec3 = Vector3d(0.5, 1.5, 2.5);
        auto res2 = vec1 + vec3;
        CHECK(is_approx_equal(res2, 1.5, 3.5, 5.5));
    }

    SUBCASE("Subtraction") {
        auto vec1 = Vector3i(10, 20, 30);
        auto vec2 = Vector3i(3, 4, 5);
        auto res = vec1 - vec2;
        CHECK(res.x == 7);
        CHECK(res.y == 16);
        CHECK(res.z == 25);

        // Mixed type: int - float -> float
        auto vec3 = Vector3f(1.5f, 2.5f, 3.5f);
        auto res2 = vec1 - vec3;
        CHECK(is_approx_equal(res2, 8.5f, 17.5f, 26.5f));
    }

    SUBCASE("Multiplication (Vector * Vector)") {
        auto vec1 = Vector3i(2, 3, 4);
        auto vec2 = Vector3i(5, 6, 7);
        auto res = vec1 * vec2;
        CHECK(res.x == 10);
        CHECK(res.y == 18);
        CHECK(res.z == 28);

        // Mixed type: int * float -> float
        auto vec3 = Vector3f(0.5f, 2.0f, 1.5f);
        auto res2 = vec1 * vec3;
        CHECK(is_approx_equal(res2, 1.0f, 6.0f, 6.0f));
    }

    SUBCASE("Multiplication (Vector * Scalar)") {
        auto vec1 = Vector3f(1.0f, 2.0f, 3.0f);
        auto res = vec1 * 2.5f;
        CHECK(is_approx_equal(res, 2.5f, 5.0f, 7.5f));

        auto res2 = vec1 * 3;
        CHECK(is_approx_equal(res2, 3.0f, 6.0f, 9.0f));
    }

    SUBCASE("Division (Vector / Vector)") {
        auto vec1 = Vector3f(10.0f, 20.0f, 30.0f);
        auto vec2 = Vector3f(2.0f, 4.0f, 5.0f);
        auto res = vec1 / vec2;
        CHECK(is_approx_equal(res, 5.0f, 5.0f, 6.0f));

        // Mixed type: float / int -> float
        auto vec3 = Vector3i(2, 5, 10);
        auto res2 = vec1 / vec3;
        CHECK(is_approx_equal(res2, 5.0f, 4.0f, 3.0f));
    }

    SUBCASE("Division (Vector / Scalar)") {
        auto vec1 = Vector3f(10.0f, 20.0f, 30.0f);
        auto res = vec1 / 2.0f;
        CHECK(is_approx_equal(res, 5.0f, 10.0f, 15.0f));

        auto res2 = vec1 / 4;
        CHECK(is_approx_equal(res2, 2.5f, 5.0f, 7.5f));
    }

    SUBCASE("Unary - Operator") {
        auto v = Vector3i(1, -2, 3);
        auto negated = -v;
        CHECK(negated.x == -1);
        CHECK(negated.y == 2);
        CHECK(negated.z == -3);
    }
}

TEST_CASE("Vector3 Compound Assignment") {
    SUBCASE("operator+=") {
        Vector3i v(1, 2, 3);
        auto& ref = (v += Vector3i(3, 4, 5));
        CHECK(v.x == 4);
        CHECK(v.y == 6);
        CHECK(v.z == 8);
        CHECK(&ref == &v);

        v += Vector3d(0.5, 0.5, 0.5); // truncation
        CHECK(v.x == 4);
        CHECK(v.y == 6);
        CHECK(v.z == 8);
    }

    SUBCASE("operator-=") {
        Vector3f v(10.0f, 20.0f, 30.0f);
        auto& ref = (v -= Vector3f(3.0f, 4.0f, 5.0f));
        CHECK(is_approx_equal(v, 7.0f, 16.0f, 25.0f));
        CHECK(&ref == &v);
    }

    SUBCASE("operator*=") {
        SUBCASE("component-wise") {
            Vector3i v(2, 3, 4);
            auto& ref = (v *= Vector3i(4, 5, 6));
            CHECK(v.x == 8);
            CHECK(v.y == 15);
            CHECK(v.z == 24);
            CHECK(&ref == &v);
        }
        SUBCASE("scalar") {
            Vector3f v(1.0f, 2.0f, 3.0f);
            auto& ref = (v *= 2.5f);
            CHECK(is_approx_equal(v, 2.5f, 5.0f, 7.5f));
            CHECK(&ref == &v);
        }
    }

    SUBCASE("operator/=") {
        Vector3d v(10.0, 20.0, 30.0);
        auto& ref = (v /= 2.0);
        CHECK(is_approx_equal(v, 5.0, 10.0, 15.0));
        CHECK(&ref == &v);
    }
}

TEST_CASE("Vector3 Type Deductions") {
    Vector3i vecI(1, 1, 1);
    Vector3f vecF(1.0f, 1.0f, 1.0f);
    Vector3d vecD(1.0, 1.0, 1.0);

    SUBCASE("Arithmetic return types") {
        static_assert(std::is_same_v<decltype(vecI + vecF), Vector3<float>>);
        static_assert(std::is_same_v<decltype(vecI + vecD), Vector3<double>>);
        static_assert(std::is_same_v<decltype(vecF + vecD), Vector3<double>>);

        static_assert(std::is_same_v<decltype(vecI * vecF), Vector3<float>>);
        static_assert(std::is_same_v<decltype(vecI / vecF), Vector3<float>>);
    }
}

TEST_CASE("Vector3 Dot Product") {
    SUBCASE("Standard cases") {
        CHECK(MathLib::dot(Vector3i(1, 2, 3), Vector3i(4, 5, 6)) == 32);
        CHECK(MathLib::dot(Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f)) == 0.0f); // Perpendicular
        CHECK(MathLib::dot(Vector3d(1.0, 1.0, 1.0), Vector3d(2.0, 2.0, 2.0)) == 6.0);     // Parallel
        CHECK(MathLib::dot(Vector3d(2.0, 3.0, 4.0), Vector3d(-1.0, -1.0, -1.0)) == -9.0);
    }

    SUBCASE("Mixed types") {
        auto d = MathLib::dot(Vector3i(1, 2, 3), Vector3d(0.5, 2.0, 1.0));
        CHECK(d == doctest::Approx(7.5));
    }
}

TEST_CASE("Vector3 Cross Product") {
    SUBCASE("Standard basis vectors") {
        auto x = Vector3i(1, 0, 0);
        auto y = Vector3i(0, 1, 0);
        auto z = Vector3i(0, 0, 1);

        CHECK(MathLib::cross(x, y) == z);
        CHECK(MathLib::cross(y, z) == x);
        CHECK(MathLib::cross(z, x) == y);

        CHECK(MathLib::cross(y, x) == -z);
    }

    SUBCASE("Parallel vectors produce zero vector") {
        auto v1 = Vector3i(1, 2, 3);
        auto v2 = Vector3i(2, 4, 6);
        auto res = MathLib::cross(v1, v2);
        CHECK(res.x == 0);
        CHECK(res.y == 0);
        CHECK(res.z == 0);
    }

    SUBCASE("General known-value case") {
        auto v1 = Vector3i(3, -3, 1);
        auto v2 = Vector3i(4, 9, 2);
        auto res = MathLib::cross(v1, v2);
        // ( -3*2 - 1*9, 1*4 - 3*2, 3*9 - (-3)*4 )
        // ( -6 - 9, 4 - 6, 27 + 12 )
        // ( -15, -2, 39 )
        CHECK(res.x == -15);
        CHECK(res.y == -2);
        CHECK(res.z == 39);
    }

    SUBCASE("Mixed-type cross product") {
        auto v1 = Vector3i(1, 0, 0);
        auto v2 = Vector3f(0.0f, 1.0f, 0.0f);
        auto res = MathLib::cross(v1, v2);
        static_assert(std::is_same_v<decltype(res), Vector3<float>>);
        CHECK(is_approx_equal(res, 0.0f, 0.0f, 1.0f));
    }

    SUBCASE("Constexpr cross product") {
        constexpr Vector3i v1(1, 0, 0);
        constexpr Vector3i v2(0, 1, 0);
        constexpr auto res = MathLib::cross(v1, v2);
        static_assert(res.x == 0);
        static_assert(res.y == 0);
        static_assert(res.z == 1);
    }
}

TEST_CASE("Vector3 Normalization") {
    SUBCASE("Standard geometric vectors") {
        // (2, 3, 6) normalizes to (2/7, 3/7, 6/7) because length is sqrt(4+9+36) = sqrt(49) = 7
        auto vec = MathLib::normalize(Vector3f(2.0f, 3.0f, 6.0f));
        CHECK(is_approx_equal(vec, 2.0f/7.0f, 3.0f/7.0f, 6.0f/7.0f));

        // An axis-aligned vector should normalize to a pure unit vector
        auto axis_v = MathLib::normalize(Vector3d(5.5, 0.0, 0.0));
        CHECK(is_approx_equal(axis_v, 1.0, 0.0, 0.0));

        auto neg_v = MathLib::normalize(Vector3f(0.0f, -3.0f, 0.0f));
        CHECK(is_approx_equal(neg_v, 0.0f, -1.0f, 0.0f));
    }

    SUBCASE("Resulting length should always approximate 1.0") {
        auto v = MathLib::normalize(Vector3f(12.34f, -56.78f, 9.01f));
        float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        CHECK(length == doctest::Approx(1.0f));
    }

    SUBCASE("Edge Case: Zero Vector") {
        auto zero_v = MathLib::normalize(Vector3f(0.0f, 0.0f, 0.0f));
        CHECK(zero_v.x == 0.0f);
        CHECK(zero_v.y == 0.0f);
        CHECK(zero_v.z == 0.0f);
    }

    SUBCASE("Edge Case: (0, 0, nonzero) - Potential bug check") {
        auto z_v = MathLib::normalize(Vector3f(0.0f, 0.0f, 5.0f));
        CHECK(is_approx_equal(z_v, 0.0f, 0.0f, 1.0f));
    }
}

TEST_CASE("Vector3 Exceptions") {
    SUBCASE("Vector / Scalar division by zero") {
        Vector3f v(1.0f, 2.0f, 3.0f);
        CHECK_THROWS_AS(v / 0.0f, std::invalid_argument);
        CHECK_THROWS_AS(v / 0, std::invalid_argument);
    }

    SUBCASE("Vector / Vector division by zero") {
        Vector3f vec1(1.0f, 2.0f, 3.0f);
        SUBCASE("x component zero") {
            Vector3f vec2(0.0f, 5.0f, 5.0f);
            CHECK_THROWS_AS(vec1 / vec2, std::invalid_argument);
        }
        SUBCASE("y component zero") {
            Vector3f vec3(5.0f, 0.0f, 5.0f);
            CHECK_THROWS_AS(vec1 / vec3, std::invalid_argument);
        }
        SUBCASE("z component zero") {
            Vector3f vec4(5.0f, 5.0f, 0.0f);
            CHECK_THROWS_AS(vec1 / vec4, std::invalid_argument);
        }
    }
}

TEST_CASE("Vector3 Stream Output") {
    Vector3i vecI(1, 2, 3);
    std::stringstream ss;
    ss << vecI;
    CHECK(ss.str() == "(1, 2, 3)");

    Vector3f vecF(3.5f, -4.2f, 0.25f);
    std::stringstream ss2;
    ss2 << vecF;
    CHECK(ss2.str() == "(3.5, -4.2, 0.25)");
}

TEST_CASE("Vector3 Constexpr and Static Assert") {
    SUBCASE("Constexpr construction and access") {
        constexpr Vector3i vec(10, 20, 30);
        static_assert(vec.x == 10);
        static_assert(vec.y == 20);
        static_assert(vec.z == 30);
    }

    SUBCASE("Constexpr arithmetic") {
        constexpr Vector3i vec1(1, 2, 3);
        constexpr Vector3i vec2(4, 5, 6);
        constexpr auto vec3 = vec1 + vec2;
        static_assert(vec3.x == 5);
        static_assert(vec3.y == 7);
        static_assert(vec3.z == 9);

        constexpr auto d = MathLib::dot(vec1, vec2);
        static_assert(d == 32);
    }

    SUBCASE("Constexpr normalization") {
        constexpr auto v = MathLib::normalize(Vector3d(1.0, 0.0, 0.0));
        static_assert(v.x == 1.0);
        static_assert(v.y == 0.0);
        static_assert(v.z == 0.0);
    }
}

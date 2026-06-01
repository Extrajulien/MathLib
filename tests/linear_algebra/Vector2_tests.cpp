#include <cmath>
#include <array>
#include <sstream>
#include <stdexcept>
#include <string>

#include "doctest.h"

import MathLib;

using MathLib::Vector2;
using MathLib::Vector2i;
using MathLib::Vector2f;
using MathLib::Vector2d;

// Helper function for approximate vector comparison to avoid repeated boilerplate.
template<typename T, typename U>
bool is_approx_equal(const Vector2<T>& actual, U expected_x, U expected_y) {
    return actual.x == doctest::Approx(expected_x) &&
           actual.y == doctest::Approx(expected_y);
}

TEST_CASE("Vector2 Construction") {
    SUBCASE("Construction from components") {
        Vector2i vecI(1, 2);
        CHECK(vecI.x == 1);
        CHECK(vecI.y == 2);

        Vector2f vecF(1.5f, 2.5f);
        CHECK(vecF.x == 1.5f);
        CHECK(vecF.y == 2.5f);

        Vector2d vecD(-1.0, 3.14);
        CHECK(vecD.x == -1.0);
        CHECK(vecD.y == 3.14);
    }

    SUBCASE("Construction from std::array") {
        std::array<int, 2> arrI = {10, 20};
        Vector2i vecI(arrI);
        CHECK(vecI.x == 10);
        CHECK(vecI.y == 20);

        std::array<double, 2> arrD = {0.5, -0.5};
        Vector2d vecD(arrD);
        CHECK(vecD.x == 0.5);
        CHECK(vecD.y == -0.5);
    }
}

TEST_CASE("Vector2 Conversion and Assignment") {
    SUBCASE("to_array()") {
        Vector2i v(3, 4);
        auto arr = v.to_array();
        CHECK(arr.size() == 2);
        CHECK(arr[0] == 3);
        CHECK(arr[1] == 4);
    }

    SUBCASE("Assignment from std::array") {
        Vector2f v(0.0f, 0.0f);
        std::array<float, 2> arr = {1.1f, 2.2f};
        v = arr;
        CHECK(v.x == 1.1f);
        CHECK(v.y == 2.2f);

        // Mixed type assignment from array
        std::array<int, 2> arrI = {5, 6};
        v = arrI;
        CHECK(v.x == 5.0f);
        CHECK(v.y == 6.0f);
    }

    SUBCASE("Copy assignment from same type") {
        Vector2i source(10, 20);
        Vector2i destination(0, 0);

        destination = source;

        CHECK(destination == source);
        CHECK(destination.x == 10);
        CHECK(destination.y == 20);
    }

    SUBCASE("Cross-type assignment (Float to Int conversion)") {
        Vector2f float_vec(5.7f, 9.2f);
        Vector2i int_vec(0, 0);

        int_vec = float_vec;

        // This verifies truncation behavior during assignment
        CHECK(int_vec == Vector2i(5, 9));
        CHECK_FALSE(int_vec == Vector2i(6, 9)); // Intentionally proving it didn't round up
    }
}

TEST_CASE("Vector2 Equality and Inequality") {
    SUBCASE("Identical vectors should be equal") {
        CHECK(Vector2i(1, 1) == Vector2i(1, 1));
        CHECK(Vector2f(1.0f, 1.0f) == Vector2f(1.0f, 1.0f));
        CHECK(Vector2d(1.0, 1.0) == Vector2d(1.0, 1.0));
    }

    SUBCASE("Different vectors must fail equality (Negative Testing)") {
        CHECK(Vector2i(1, 2) != Vector2i(1, 1));
        CHECK_FALSE(Vector2i(1, 2) == Vector2i(1, 1));
        CHECK_FALSE(Vector2f(5.0f, 0.0f) == Vector2f(5.0f, 0.001f));
    }

    SUBCASE("Mixed type equality") {
        CHECK(Vector2i(1, 2) == Vector2d(1.0, 2.0));
        CHECK(Vector2i(3, 7) != Vector2d(3.7, 7.3));
        CHECK(Vector2f(1.5f, 2.5f) != Vector2i(1, 2));
    }
}

TEST_CASE("Vector2 Arithmetic Operators") {
    SUBCASE("Addition") {
        auto vec1 = Vector2i(1, 2);
        auto vec2 = Vector2i(3, 4);
        auto res = vec1 + vec2;
        CHECK(res.x == 4);
        CHECK(res.y == 6);

        // Mixed type: int + double -> double
        auto vec3 = Vector2d(0.5, 1.5);
        auto res2 = vec1 + vec3;
        CHECK(is_approx_equal(res2, 1.5, 3.5));
    }

    SUBCASE("Subtraction") {
        auto vec1 = Vector2i(10, 20);
        auto vec2 = Vector2i(3, 4);
        auto res = vec1 - vec2;
        CHECK(res.x == 7);
        CHECK(res.y == 16);

        // Mixed type: int - float -> float
        auto vec3 = Vector2f(1.5f, 2.5f);
        auto res2 = vec1 - vec3;
        CHECK(is_approx_equal(res2, 8.5f, 17.5f));
    }

    SUBCASE("Multiplication (Vector * Vector)") {
        auto vec1 = Vector2i(2, 3);
        auto vec2 = Vector2i(4, 5);
        auto res = vec1 * vec2;
        CHECK(res.x == 8);
        CHECK(res.y == 15);

        // Mixed type: int * float -> float
        auto vec3 = Vector2f(0.5f, 2.0f);
        auto res2 = vec1 * vec3;
        CHECK(is_approx_equal(res2, 1.0f, 6.0f));
    }

    SUBCASE("Division (Vector / Vector)") {
        auto vec1 = Vector2f(10.0f, 20.0f);
        auto vec2 = Vector2f(2.0f, 4.0f);
        auto res = vec1 / vec2;
        CHECK(is_approx_equal(res, 5.0f, 5.0f));

        // Mixed type: float / int -> float
        auto vec3 = Vector2i(2, 5);
        auto res2 = vec1 / vec3;
        CHECK(is_approx_equal(res2, 5.0f, 4.0f));
    }

    SUBCASE("Division (Vector / Scalar)") {
        auto vec1 = Vector2f(10.0f, 20.0f);
        auto res = vec1 / 2.0f;
        CHECK(is_approx_equal(res, 5.0f, 10.0f));

        auto res2 = vec1 / 4;
        CHECK(is_approx_equal(res2, 2.5f, 5.0f));
    }
    
    SUBCASE("Unary - Operator") {
        auto v = Vector2i(1, -2);
        auto negated = -v;
        CHECK(negated.x == -1);
        CHECK(negated.y == 2);
    }
}

TEST_CASE("Vector2 Compound Assignment") {
    SUBCASE("operator+=") {
        Vector2i v(1, 2);
        auto& ref = (v += Vector2i(3, 4));
        CHECK(v.x == 4);
        CHECK(v.y == 6);
        CHECK(&ref == &v); // Check it returns *this

        v += Vector2d(0.5, 0.5); // should truncate if v is int
        CHECK(v.x == 4); 
        CHECK(v.y == 6);
    }

    SUBCASE("operator-=") {
        Vector2f v(10.0f, 20.0f);
        auto& ref = (v -= Vector2f(3.0f, 4.0f));
        CHECK(is_approx_equal(v, 7.0f, 16.0f));
        CHECK(&ref == &v);
    }

    SUBCASE("operator*=") {
        Vector2i v(2, 3);
        auto& ref = (v *= Vector2i(4, 5));
        CHECK(v.x == 8);
        CHECK(v.y == 15);
        CHECK(&ref == &v);
    }

    SUBCASE("operator/=") {
        Vector2d v(10.0, 20.0);
        auto& ref = (v /= 2.0);
        CHECK(is_approx_equal(v, 5.0, 10.0));
        CHECK(&ref == &v);
    }
}

TEST_CASE("Vector2 Type Deductions") {
    Vector2i vecI(1, 1);
    Vector2f vecF(1.0f, 1.0f);
    Vector2d vecD(1.0, 1.0);

    SUBCASE("Arithmetic return types") {
        // use decltype to check if promotion rules follow standard C++ rules
        static_assert(std::is_same_v<decltype(vecI + vecF), Vector2<float>>);
        static_assert(std::is_same_v<decltype(vecI + vecD), Vector2<double>>);
        static_assert(std::is_same_v<decltype(vecF + vecD), Vector2<double>>);

        static_assert(std::is_same_v<decltype(vecI * vecF), Vector2<float>>);
        static_assert(std::is_same_v<decltype(vecI / vecF), Vector2<float>>);
    }
}

TEST_CASE("Vector2 Dot Product") {
    SUBCASE("Standard cases") {
        CHECK(MathLib::dot(Vector2i(1, 2), Vector2i(3, 4)) == 11);
        CHECK(MathLib::dot(Vector2f(1.0f, 0.0f), Vector2f(0.0f, 1.0f)) == 0.0f); // Perpendicular
        CHECK(MathLib::dot(Vector2d(1.0, 1.0), Vector2d(2.0, 2.0)) == 4.0);     // Parallel
        CHECK(MathLib::dot(Vector2d(2.0, 3.0), Vector2d(-1.0, -1.0)) == -5.0);
    }

    SUBCASE("Mixed types") {
        auto d = MathLib::dot(Vector2i(1, 2), Vector2d(0.5, 2.0));
        CHECK(d == doctest::Approx(4.5));
    }
}

TEST_CASE("Vector2 Normalization") {
    SUBCASE("Standard geometric vectors") {
        // A 3-4-5 triangle vector normalized should become (0.6, 0.8)
        auto vec = MathLib::normalize(Vector2f(3.0f, 4.0f));
        CHECK(is_approx_equal(vec, 0.6f, 0.8f));

        // An axis-aligned vector should normalize to a pure unit vector
        auto axis_v = MathLib::normalize(Vector2d(5.5, 0.0));
        CHECK(is_approx_equal(axis_v, 1.0, 0.0));
        
        auto neg_v = MathLib::normalize(Vector2f(-3.0f, 0.0f));
        CHECK(is_approx_equal(neg_v, -1.0f, 0.0f));
    }

    SUBCASE("Resulting length should always approximate 1.0") {
        auto v = MathLib::normalize(Vector2f(12.34f, -56.78f));
        float length = std::sqrt(v.x * v.x + v.y * v.y);
        CHECK(length == doctest::Approx(1.0f));
    }

    SUBCASE("Edge Case: Zero Vector") {
        auto zero_v = MathLib::normalize(Vector2f(0.0f, 0.0f));
        CHECK(zero_v.x == 0.0f);
        CHECK(zero_v.y == 0.0f);
    }
}

TEST_CASE("Vector2 Exceptions") {
    SUBCASE("Vector / Scalar division by zero") {
        Vector2f v(1.0f, 2.0f);
        CHECK_THROWS_AS(v / 0.0f, std::invalid_argument);
        CHECK_THROWS_AS(v / 0, std::invalid_argument);
    }

    SUBCASE("Vector / Vector division by zero") {
        Vector2f vec1(1.0f, 2.0f);
        Vector2f vec2(0.0f, 5.0f);
        Vector2f vec3(5.0f, 0.0f);
        CHECK_THROWS_AS(vec1 / vec2, std::invalid_argument);
        CHECK_THROWS_AS(vec1 / vec3, std::invalid_argument);
    }
}

TEST_CASE("Vector2 Stream Output") {
    Vector2i vecI(1, 2);
    std::stringstream ss;
    ss << vecI;
    CHECK(ss.str() == "(1, 2)");

    Vector2f vecF(3.5f, -4.2f);
    std::stringstream ss2;
    ss2 << vecF;
    CHECK(ss2.str() == "(3.5, -4.2)");
}

TEST_CASE("Vector2 Constexpr and Static Assert") {
    SUBCASE("Constexpr construction and access") {
        constexpr Vector2i vec(10, 20);
        static_assert(vec.x == 10);
        static_assert(vec.y == 20);
    }

    SUBCASE("Constexpr arithmetic") {
        constexpr Vector2i vec1(1, 2);
        constexpr Vector2i vec2(3, 4);
        constexpr auto vec3 = vec1 + vec2;
        static_assert(vec3.x == 4);
        static_assert(vec3.y == 6);

        constexpr auto d = MathLib::dot(vec1, vec2);
        static_assert(d == 11);
    }

    SUBCASE("Constexpr normalization") {
        constexpr auto v = MathLib::normalize(Vector2d(1.0, 0.0));
        static_assert(v.x == 1.0);
        static_assert(v.y == 0.0);
    }
}
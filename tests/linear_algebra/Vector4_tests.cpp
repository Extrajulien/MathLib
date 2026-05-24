#include <cmath>
#include <array>
#include <sstream>
#include <stdexcept>
#include <type_traits>

#include "doctest/doctest.h"

import MathLib;

using math::Vector4;
using math::Vector4i;
using math::Vector4f;
using math::Vector4d;

// Helper function for approximate vector comparison to avoid repeated boilerplate.
template<typename T, typename U>
bool is_approx_equal(const Vector4<T>& actual, U expected_x, U expected_y, U expected_z, U expected_w) {
    return actual.x == doctest::Approx(expected_x) &&
           actual.y == doctest::Approx(expected_y) &&
           actual.z == doctest::Approx(expected_z) &&
           actual.w == doctest::Approx(expected_w);
}

TEST_CASE("Vector4 Construction") {
    SUBCASE("Construction from components") {
        Vector4i vecI(1, 2, 3, 4);
        CHECK(vecI.x == 1);
        CHECK(vecI.y == 2);
        CHECK(vecI.z == 3);
        CHECK(vecI.w == 4);

        Vector4f vecF(1.5f, 2.5f, 3.5f, 4.5f);
        CHECK(vecF.x == 1.5f);
        CHECK(vecF.y == 2.5f);
        CHECK(vecF.z == 3.5f);
        CHECK(vecF.w == 4.5f);

        Vector4d vecD(-1.0, 3.14, 2.71, 0.0);
        CHECK(vecD.x == -1.0);
        CHECK(vecD.y == 3.14);
        CHECK(vecD.z == 2.71);
        CHECK(vecD.w == 0.0);
    }

    SUBCASE("Construction from std::array") {
        std::array<int, 4> arrI = {10, 20, 30, 40};
        Vector4i vecI(arrI);
        CHECK(vecI.x == 10);
        CHECK(vecI.y == 20);
        CHECK(vecI.z == 30);
        CHECK(vecI.w == 40);

        std::array<double, 4> arrD = {0.5, -0.5, 0.25, -0.25};
        Vector4d vecD(arrD);
        CHECK(vecD.x == 0.5);
        CHECK(vecD.y == -0.5);
        CHECK(vecD.z == 0.25);
        CHECK(vecD.w == -0.25);
    }
}

TEST_CASE("Vector4 Conversion and Assignment") {
    SUBCASE("to_array()") {
        Vector4i v(3, 4, 5, 6);
        auto arr = v.to_array();
        CHECK(arr.size() == 4);
        CHECK(arr[0] == 3);
        CHECK(arr[1] == 4);
        CHECK(arr[2] == 5);
        CHECK(arr[3] == 6);
    }

    SUBCASE("Assignment from std::array") {
        Vector4f v(0.0f, 0.0f, 0.0f, 0.0f);
        std::array<float, 4> arr = {1.1f, 2.2f, 3.3f, 4.4f};
        v = arr;
        CHECK(v.x == 1.1f);
        CHECK(v.y == 2.2f);
        CHECK(v.z == 3.3f);
        CHECK(v.w == 4.4f);

        // Mixed type assignment from array
        std::array<int, 4> arrI = {5, 6, 7, 8};
        v = arrI;
        CHECK(v.x == 5.0f);
        CHECK(v.y == 6.0f);
        CHECK(v.z == 7.0f);
        CHECK(v.w == 8.0f);
    }

    SUBCASE("Copy assignment from same type") {
        Vector4i source(10, 20, 30, 40);
        Vector4i destination(0, 0, 0, 0);

        destination = source;

        CHECK(destination == source);
        CHECK(destination.x == 10);
        CHECK(destination.y == 20);
        CHECK(destination.z == 30);
        CHECK(destination.w == 40);
    }

    SUBCASE("Cross-type assignment (Float to Int conversion)") {
        Vector4f float_vec(5.7f, 9.2f, 12.9f, -3.8f);
        Vector4i int_vec(0, 0, 0, 0);

        int_vec = float_vec;

        // This verifies truncation behavior during assignment
        CHECK(int_vec == Vector4i(5, 9, 12, -3));
        CHECK_FALSE(int_vec == Vector4i(6, 9, 12, -3));
    }
}

TEST_CASE("Vector4 Equality and Inequality") {
    SUBCASE("Identical vectors should be equal") {
        CHECK(Vector4i(1, 1, 1, 1) == Vector4i(1, 1, 1, 1));
        CHECK(Vector4f(1.0f, 1.0f, 1.0f, 1.0f) == Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
        CHECK(Vector4d(1.0, 1.0, 1.0, 1.0) == Vector4d(1.0, 1.0, 1.0, 1.0));
    }

    SUBCASE("Different vectors must fail equality (Negative Testing)") {
        CHECK(Vector4i(1, 2, 3, 4) != Vector4i(1, 1, 3, 4));
        CHECK(Vector4i(1, 2, 3, 4) != Vector4i(1, 2, 1, 4));
        CHECK(Vector4i(1, 2, 3, 4) != Vector4i(1, 2, 3, 0));
        CHECK(Vector4i(1, 2, 3, 4) != Vector4i(0, 2, 3, 4));
        CHECK_FALSE(Vector4i(1, 2, 3, 4) == Vector4i(1, 1, 3, 4));
        CHECK_FALSE(Vector4f(5.0f, 0.0f, 1.0f, 2.0f) == Vector4f(5.0f, 0.001f, 1.0f, 2.0f));
    }

    SUBCASE("Mixed type equality") {
        CHECK(Vector4i(1, 2, 3, 4) == Vector4d(1.0, 2.0, 3.0, 4.0));
        CHECK(Vector4i(3, 7, 1, 0) != Vector4d(3.7, 7.3, 1.0, 0.0));
        CHECK(Vector4f(1.5f, 2.5f, 0.5f, 0.1f) != Vector4i(1, 2, 0, 0));
    }
}

TEST_CASE("Vector4 Arithmetic Operators") {
    SUBCASE("Addition") {
        auto vec1 = Vector4i(1, 2, 3, 4);
        auto vec2 = Vector4i(5, 6, 7, 8);
        auto res = vec1 + vec2;
        CHECK(res.x == 6);
        CHECK(res.y == 8);
        CHECK(res.z == 10);
        CHECK(res.w == 12);

        // Mixed type: int + double -> double
        auto vec3 = Vector4d(0.5, 1.5, 2.5, 3.5);
        auto res2 = vec1 + vec3;
        CHECK(is_approx_equal(res2, 1.5, 3.5, 5.5, 7.5));
    }

    SUBCASE("Subtraction") {
        auto vec1 = Vector4i(10, 20, 30, 40);
        auto vec2 = Vector4i(3, 4, 5, 6);
        auto res = vec1 - vec2;
        CHECK(res.x == 7);
        CHECK(res.y == 16);
        CHECK(res.z == 25);
        CHECK(res.w == 34);

        // Mixed type: int - float -> float
        auto vec3 = Vector4f(1.5f, 2.5f, 3.5f, 4.5f);
        auto res2 = vec1 - vec3;
        CHECK(is_approx_equal(res2, 8.5f, 17.5f, 26.5f, 35.5f));
    }

    SUBCASE("Multiplication (Vector * Vector)") {
        auto vec1 = Vector4i(2, 3, 4, 5);
        auto vec2 = Vector4i(6, 7, 8, 9);
        auto res = vec1 * vec2;
        CHECK(res.x == 12);
        CHECK(res.y == 21);
        CHECK(res.z == 32);
        CHECK(res.w == 45);

        // Mixed type: int * float -> float
        auto vec3 = Vector4f(0.5f, 2.0f, 1.5f, 0.2f);
        auto res2 = vec1 * vec3;
        CHECK(is_approx_equal(res2, 1.0f, 6.0f, 6.0f, 1.0f));
    }

    SUBCASE("Multiplication (Vector * Scalar)") {
        auto vec1 = Vector4f(1.0f, 2.0f, 3.0f, 4.0f);
        auto res = vec1 * 2.5f;
        CHECK(is_approx_equal(res, 2.5f, 5.0f, 7.5f, 10.0f));

        auto res2 = vec1 * 3;
        CHECK(is_approx_equal(res2, 3.0f, 6.0f, 9.0f, 12.0f));
    }

    SUBCASE("Division (Vector / Vector)") {
        auto vec1 = Vector4f(10.0f, 20.0f, 30.0f, 40.0f);
        auto vec2 = Vector4f(2.0f, 4.0f, 5.0f, 8.0f);
        auto res = vec1 / vec2;
        CHECK(is_approx_equal(res, 5.0f, 5.0f, 6.0f, 5.0f));

        // Mixed type: float / int -> float
        auto vec3 = Vector4i(2, 5, 10, 20);
        auto res2 = vec1 / vec3;
        CHECK(is_approx_equal(res2, 5.0f, 4.0f, 3.0f, 2.0f));
    }

    SUBCASE("Division (Vector / Scalar)") {
        auto vec1 = Vector4f(10.0f, 20.0f, 30.0f, 40.0f);
        auto res = vec1 / 2.0f;
        CHECK(is_approx_equal(res, 5.0f, 10.0f, 15.0f, 20.0f));

        auto res2 = vec1 / 4;
        CHECK(is_approx_equal(res2, 2.5f, 5.0f, 7.5f, 10.0f));
    }

    SUBCASE("Unary - Operator") {
        auto v = Vector4i(1, -2, 3, -4);
        auto negated = -v;
        CHECK(negated.x == -1);
        CHECK(negated.y == 2);
        CHECK(negated.z == -3);
        CHECK(negated.w == 4);
    }
}

TEST_CASE("Vector4 Compound Assignment") {
    SUBCASE("operator+=") {
        Vector4i v(1, 2, 3, 4);
        auto& ref = (v += Vector4i(3, 4, 5, 6));
        CHECK(v.x == 4);
        CHECK(v.y == 6);
        CHECK(v.z == 8);
        CHECK(v.w == 10);
        CHECK(&ref == &v);

        v += Vector4d(0.5, 0.5, 0.5, 0.5); // truncation
        CHECK(v.x == 4);
        CHECK(v.y == 6);
        CHECK(v.z == 8);
        CHECK(v.w == 10);
    }

    SUBCASE("operator-=") {
        Vector4f v(10.0f, 20.0f, 30.0f, 40.0f);
        auto& ref = (v -= Vector4f(3.0f, 4.0f, 5.0f, 6.0f));
        CHECK(is_approx_equal(v, 7.0f, 16.0f, 25.0f, 34.0f));
        CHECK(&ref == &v);
    }

    SUBCASE("operator*=") {
        SUBCASE("component-wise") {
            Vector4i v(2, 3, 4, 5);
            auto& ref = (v *= Vector4i(4, 5, 6, 7));
            CHECK(v.x == 8);
            CHECK(v.y == 15);
            CHECK(v.z == 24);
            CHECK(v.w == 35);
            CHECK(&ref == &v);
        }
        SUBCASE("scalar") {
            Vector4f v(1.0f, 2.0f, 3.0f, 4.0f);
            auto& ref = (v *= 2.5f);
            CHECK(is_approx_equal(v, 2.5f, 5.0f, 7.5f, 10.0f));
            CHECK(&ref == &v);
        }
        SUBCASE("scalar mixed type") {
            Vector4i v(2, 3, 4, 5);
            v *= 1.5; // 2*1.5=3, 3*1.5=4.5->4, 4*1.5=6, 5*1.5=7.5->7
            CHECK(v.x == 3);
            CHECK(v.y == 4);
            CHECK(v.z == 6);
            CHECK(v.w == 7);
        }
    }

    SUBCASE("operator/=") {
        SUBCASE("scalar") {
            Vector4d v(10.0, 20.0, 30.0, 40.0);
            auto& ref = (v /= 2.0);
            CHECK(is_approx_equal(v, 5.0, 10.0, 15.0, 20.0));
            CHECK(&ref == &v);

            Vector4i vi(10, 20, 30, 40);
            vi /= 4.0; // 10/4 = 2.5 -> 2
            CHECK(vi.x == 2);
            CHECK(vi.y == 5);
            CHECK(vi.z == 7);
            CHECK(vi.w == 10);
        }
        SUBCASE("component-wise") {
            Vector4f v(12.0f, 16.0f, 20.0f, 24.0f);
            auto& ref = (v /= Vector4f(2.0f, 4.0f, 5.0f, 6.0f));
            CHECK(is_approx_equal(v, 6.0f, 4.0f, 4.0f, 4.0f));
            CHECK(&ref == &v);
        }
    }
}

TEST_CASE("Vector4 Type Deductions") {
    Vector4i vecI(1, 1, 1, 1);
    Vector4f vecF(1.0f, 1.0f, 1.0f, 1.0f);
    Vector4d vecD(1.0, 1.0, 1.0, 1.0);

    SUBCASE("Arithmetic return types") {
        static_assert(std::is_same_v<decltype(vecI + vecF), Vector4<float>>);
        static_assert(std::is_same_v<decltype(vecI + vecD), Vector4<double>>);
        static_assert(std::is_same_v<decltype(vecF + vecD), Vector4<double>>);

        static_assert(std::is_same_v<decltype(vecI * vecF), Vector4<float>>);
        static_assert(std::is_same_v<decltype(vecI / vecF), Vector4<float>>);
        
        // Division of two integer vectors
        static_assert(std::is_same_v<decltype(vecI / vecI), Vector4<int>>);
    }
}

TEST_CASE("Vector4 Dot Product") {
    SUBCASE("Standard cases") {
        CHECK(math::dot(Vector4i(1, 2, 3, 4), Vector4i(5, 6, 7, 8)) == 70); 
        CHECK(math::dot(Vector4f(1.0f, 0.0f, 0.0f, 0.0f), Vector4f(0.0f, 1.0f, 0.0f, 0.0f)) == 0.0f); 
        CHECK(math::dot(Vector4d(1.0, 1.0, 1.0, 1.0), Vector4d(2.0, 2.0, 2.0, 2.0)) == 8.0);     
        CHECK(math::dot(Vector4d(2.0, 3.0, 4.0, 5.0), Vector4d(-1.0, -1.0, -1.0, -1.0)) == -14.0);
    }

    SUBCASE("Mixed types") {
        auto d = math::dot(Vector4i(1, 2, 3, 4), Vector4d(0.5, 2.0, 1.0, 0.25));
        CHECK(d == doctest::Approx(8.5));
    }
}

TEST_CASE("Vector4 Normalization") {
    SUBCASE("Standard geometric vectors") {
        // (1, 2, 2, 4) has length sqrt(1 + 4 + 4 + 16) = sqrt(25) = 5
        auto vec = math::normalize(Vector4f(1.0f, 2.0f, 2.0f, 4.0f));
        CHECK(is_approx_equal(vec, 1.0f/5.0f, 2.0f/5.0f, 2.0f/5.0f, 4.0f/5.0f));

        // Axis-aligned vectors
        CHECK(is_approx_equal(math::normalize(Vector4d(5.0, 0.0, 0.0, 0.0)), 1.0, 0.0, 0.0, 0.0));
        CHECK(is_approx_equal(math::normalize(Vector4d(0.0, 5.0, 0.0, 0.0)), 0.0, 1.0, 0.0, 0.0));
        CHECK(is_approx_equal(math::normalize(Vector4d(0.0, 0.0, 5.0, 0.0)), 0.0, 0.0, 1.0, 0.0));
        CHECK(is_approx_equal(math::normalize(Vector4d(0.0, 0.0, 0.0, 5.0)), 0.0, 0.0, 0.0, 1.0));

        auto neg_v = math::normalize(Vector4f(0.0f, 0.0f, 0.0f, -3.0f));
        CHECK(is_approx_equal(neg_v, 0.0f, 0.0f, 0.0f, -1.0f));
    }

    SUBCASE("Resulting length should always approximate 1.0") {
        auto v = math::normalize(Vector4f(1.0f, 2.0f, 3.0f, 4.0f));
        float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
        CHECK(length == doctest::Approx(1.0f));
    }

    SUBCASE("Edge Case: Zero Vector") {
        auto zero_v = math::normalize(Vector4f(0.0f, 0.0f, 0.0f, 0.0f));
        CHECK(zero_v.x == 0.0f);
        CHECK(zero_v.y == 0.0f);
        CHECK(zero_v.z == 0.0f);
        CHECK(zero_v.w == 0.0f);
    }
}

TEST_CASE("Vector4 Exceptions") {
    SUBCASE("Vector / Scalar division by zero") {
        Vector4f v(1.0f, 2.0f, 3.0f, 4.0f);
        CHECK_THROWS_AS(v / 0.0f, std::invalid_argument);
        CHECK_THROWS_AS(v / 0, std::invalid_argument);
    }

    SUBCASE("Vector /= Scalar division by zero") {
        Vector4f v(1.0f, 2.0f, 3.0f, 4.0f);
        CHECK_THROWS_AS(v /= 0.0f, std::invalid_argument);
    }

    SUBCASE("Vector / Vector division by zero") {
        Vector4f vec1(1.0f, 2.0f, 3.0f, 4.0f);
        CHECK_THROWS_AS(vec1 / Vector4f(0.0f, 1.0f, 1.0f, 1.0f), std::invalid_argument);
        CHECK_THROWS_AS(vec1 / Vector4f(1.0f, 0.0f, 1.0f, 1.0f), std::invalid_argument);
        CHECK_THROWS_AS(vec1 / Vector4f(1.0f, 1.0f, 0.0f, 1.0f), std::invalid_argument);
        CHECK_THROWS_AS(vec1 / Vector4f(1.0f, 1.0f, 1.0f, 0.0f), std::invalid_argument);
    }
    
    SUBCASE("Vector /= Vector division by zero") {
        Vector4f vec1(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4f vec2(1.0f, 1.0f, 1.0f, 0.0f);
        CHECK_THROWS_AS(vec1 /= vec2, std::invalid_argument);
    }
}

TEST_CASE("Vector4 Stream Output") {
    Vector4i vecI(1, 2, 3, 4);
    std::stringstream ss;
    ss << vecI;
    CHECK(ss.str() == "(1, 2, 3, 4)");

    Vector4f vecF(3.5f, -4.2f, 0.25f, 8.0f);
    std::stringstream ss2;
    ss2 << vecF;
    CHECK(ss2.str() == "(3.5, -4.2, 0.25, 8)");
}

TEST_CASE("Vector4 Constexpr and Static Assert") {
    SUBCASE("Constexpr construction and access") {
        constexpr Vector4i vec(10, 20, 30, 40);
        static_assert(vec.x == 10);
        static_assert(vec.y == 20);
        static_assert(vec.z == 30);
        static_assert(vec.w == 40);
    }

    SUBCASE("Constexpr to_array") {
        constexpr Vector4i vec(1, 2, 3, 4);
        constexpr auto arr = vec.to_array();
        static_assert(arr[0] == 1);
        static_assert(arr[1] == 2);
        static_assert(arr[2] == 3);
        static_assert(arr[3] == 4);
    }

    SUBCASE("Constexpr arithmetic") {
        constexpr Vector4i vec1(1, 2, 3, 4);
        constexpr Vector4i vec2(5, 6, 7, 8);
        
        constexpr auto sum = vec1 + vec2;
        static_assert(sum.x == 6);
        static_assert(sum.y == 8);
        static_assert(sum.z == 10);
        static_assert(sum.w == 12);

        constexpr auto diff = vec2 - vec1;
        static_assert(diff.x == 4);
        static_assert(diff.y == 4);
        static_assert(diff.z == 4);
        static_assert(diff.w == 4);
        
        constexpr auto prod = vec1 * 2;
        static_assert(prod.x == 2);
        static_assert(prod.y == 4);
        static_assert(prod.z == 6);
        static_assert(prod.w == 8);

        constexpr auto neg = -vec1;
        static_assert(neg.x == -1);
        static_assert(neg.y == -2);
        static_assert(neg.z == -3);
        static_assert(neg.w == -4);

        constexpr auto d = math::dot(vec1, vec2);
        static_assert(d == 70);
    }

    SUBCASE("Constexpr normalization") {
        constexpr auto v = math::normalize(Vector4d(0.0, 0.0, 0.0, 1.0));
        static_assert(v.x == 0.0);
        static_assert(v.y == 0.0);
        static_assert(v.z == 0.0);
        static_assert(v.w == 1.0);
    }
}

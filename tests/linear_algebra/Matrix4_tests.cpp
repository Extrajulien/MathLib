#include <cmath>
#include <array>
#include <sstream>
#include <stdexcept>
#include <type_traits>

#include "doctest.h"

import MathLib;

using math::Matrix4;
using math::Matrix4f;

// Helper function for approximate Matrix4 comparison
bool is_approx_equal(const Matrix4& actual, const std::array<float, 16>& expected_col_major) {
    const float* ptr = actual.toPtr();
    for (int i = 0; i < 16; ++i) {
        if (ptr[i] != doctest::Approx(expected_col_major[i])) {
            return false;
        }
    }
    return true;
}

// Helper to validate all 16 elements in column-major order exactly
bool is_exactly_equal(const Matrix4& actual, const std::array<float, 16>& expected_col_major) {
    const float* ptr = actual.toPtr();
    for (int i = 0; i < 16; ++i) {
        if (ptr[i] != expected_col_major[i]) {
            return false;
        }
    }
    return true;
}

TEST_CASE("Matrix4 Construction") {
    SUBCASE("Construction from components (row-major arguments)") {
        Matrix4 m(
            1.0f,  2.0f,  3.0f,  4.0f,
            5.0f,  6.0f,  7.0f,  8.0f,
            9.0f,  10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        );

        // Verify column-major internal storage via operator[]
        // m[col][row]
        CHECK(m[0][0] == 1.0f);
        CHECK(m[0][1] == 5.0f);
        CHECK(m[0][2] == 9.0f);
        CHECK(m[0][3] == 13.0f);

        CHECK(m[1][0] == 2.0f);
        CHECK(m[1][1] == 6.0f);
        CHECK(m[1][2] == 10.0f);
        CHECK(m[1][3] == 14.0f);

        CHECK(m[2][0] == 3.0f);
        CHECK(m[2][1] == 7.0f);
        CHECK(m[2][2] == 11.0f);
        CHECK(m[2][3] == 15.0f);

        CHECK(m[3][0] == 4.0f);
        CHECK(m[3][1] == 8.0f);
        CHECK(m[3][2] == 12.0f);
        CHECK(m[3][3] == 16.0f);
    }

    SUBCASE("Construction from std::array (interpreted as row-major)") {
        std::array<float, 16> arr = {
            1.0f,  2.0f,  3.0f,  4.0f,
            5.0f,  6.0f,  7.0f,  8.0f,
            9.0f,  10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        };
        Matrix4 m(arr);
        
        CHECK(m[0][0] == 1.0f);
        CHECK(m[1][0] == 2.0f);
        CHECK(m[2][0] == 3.0f);
        CHECK(m[3][0] == 4.0f);
        
        CHECK(m[0][1] == 5.0f);
        CHECK(m[1][1] == 6.0f);
        CHECK(m[2][1] == 7.0f);
        CHECK(m[3][1] == 8.0f);
    }
}

TEST_CASE("Matrix4 Conversion and Assignment") {
    SUBCASE("toArray()") {
        Matrix4 m(
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 10, 11, 12,
            13, 14, 15, 16
        );
        auto arr = m.toArray();
        // toArray() returns row-major array
        std::array<float, 16> expected = {
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 10, 11, 12,
            13, 14, 15, 16
        };
        CHECK(arr == expected);
    }

    SUBCASE("Copy assignment") {
        Matrix4 m1(
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 10, 11, 12,
            13, 14, 15, 16
        );
        Matrix4 m2 = math::factory::Identity();
        m2 = m1;
        CHECK(m2 == m1);
        CHECK(m2[0][0] == 1.0f);
        CHECK(m2[3][3] == 16.0f);
    }
}

TEST_CASE("Matrix4 toPtr Column-Major Layout") {
    Matrix4 m(
        1.0f,  2.0f,  3.0f,  4.0f,
        5.0f,  6.0f,  7.0f,  8.0f,
        9.0f,  10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    );

    const float* ptr = m.toPtr();

    // Verify all 16 elements in column-major order
    CHECK(ptr[0] == 1.0f);  // col 0, row 0
    CHECK(ptr[1] == 5.0f);  // col 0, row 1
    CHECK(ptr[2] == 9.0f);  // col 0, row 2
    CHECK(ptr[3] == 13.0f); // col 0, row 3

    CHECK(ptr[4] == 2.0f);  // col 1, row 0
    CHECK(ptr[5] == 6.0f);  // col 1, row 1
    CHECK(ptr[6] == 10.0f); // col 1, row 2
    CHECK(ptr[7] == 14.0f); // col 1, row 3

    CHECK(ptr[8] == 3.0f);  // col 2, row 0
    CHECK(ptr[9] == 7.0f);  // col 2, row 1
    CHECK(ptr[10] == 11.0f); // col 2, row 2
    CHECK(ptr[11] == 15.0f); // col 2, row 3

    CHECK(ptr[12] == 4.0f);  // col 3, row 0
    CHECK(ptr[13] == 8.0f);  // col 3, row 1
    CHECK(ptr[14] == 12.0f); // col 3, row 2
    CHECK(ptr[15] == 16.0f); // col 3, row 3

    SUBCASE("Mutation reflection") {
        // Matrix4 allows mutation through operator[]
        m[0][0] = 100.0f;
        CHECK(ptr[0] == 100.0f);
        
        m[3][3] = 200.0f;
        CHECK(ptr[15] == 200.0f);
    }
}

TEST_CASE("Matrix4 Equality and Inequality") {
    Matrix4 m1(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    Matrix4 m2 = m1;
    Matrix4 m3(
        0, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );

    CHECK(m1 == m2);
    CHECK(m1 != m3);
    CHECK_FALSE(m1 == m3);

    // Test each element difference
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            Matrix4 m_diff = m1;
            m_diff[col][row] += 1.0f;
            CHECK(m1 != m_diff);
        }
    }
}

TEST_CASE("Matrix4 Arithmetic Operators") {
    Matrix4 m1(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    Matrix4 m2(
        10, 20, 30, 40,
        50, 60, 70, 80,
        90, 100, 110, 120,
        130, 140, 150, 160
    );

    SUBCASE("Unary minus") {
        Matrix4 neg_m1 = -m1;
        CHECK(neg_m1[0][0] == -1.0f);
        CHECK(neg_m1[3][3] == -16.0f);
    }

    SUBCASE("Matrix + Matrix") {
        Matrix4 res = m1 + m2;
        CHECK(res[0][0] == 11.0f);
        CHECK(res[3][3] == 176.0f);
    }

    SUBCASE("Matrix - Matrix") {
        Matrix4 res = m2 - m1;
        CHECK(res[0][0] == 9.0f);
        CHECK(res[3][3] == 144.0f);
    }

    SUBCASE("Matrix * Scalar") {
        Matrix4 res = m1 * 2.0f;
        CHECK(res[0][0] == 2.0f);
        CHECK(res[3][3] == 32.0f);
    }

    SUBCASE("Matrix / Scalar") {
        Matrix4 res = m2 / 10.0f;
        CHECK(res[0][0] == 1.0f);
        CHECK(res[3][3] == 16.0f);
    }

    SUBCASE("Matrix * Matrix") {
        // Identity * m1 = m1
        Matrix4 identity = math::factory::Identity();
        Matrix4 res = identity * m1;
        CHECK(res == m1);

        // m1 * identity = m1
        res = m1 * identity;
        CHECK(res == m1);

        // Simple 2x2 style in 4x4
        Matrix4 ma(
            1, 2, 0, 0,
            3, 4, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );
        Matrix4 mb(
            5, 6, 0, 0,
            7, 8, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );
        // [1 2] [5 6] = [1*5+2*7 1*6+2*8] = [19 22]
        // [3 4] [7 8]   [3*5+4*7 3*6+4*8]   [43 50]
        Matrix4 mc = ma * mb;
        CHECK(mc[0][0] == 19.0f);
        CHECK(mc[1][0] == 22.0f);
        CHECK(mc[0][1] == 43.0f);
        CHECK(mc[1][1] == 50.0f);
        CHECK(mc[2][2] == 1.0f);
        CHECK(mc[3][3] == 1.0f);
    }
}

TEST_CASE("Matrix4 Compound Assignment") {
    Matrix4 m1(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );

    SUBCASE("operator+=") {
        Matrix4 m = m1;
        auto& ref = (m += m1);
        CHECK(m == (m1 + m1));
        CHECK(&ref == &m);
    }

    SUBCASE("operator-=") {
        Matrix4 m = m1;
        auto& ref = (m -= m1);
        CHECK(m == Matrix4(0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0));
        CHECK(&ref == &m);
    }

    SUBCASE("operator*=") {
        Matrix4 m = math::factory::Identity();
        auto& ref = (m *= m1);
        CHECK(m == m1);
        CHECK(&ref == &m);
    }

    SUBCASE("operator*=(scalar)") {
        Matrix4 m = m1;
        auto& ref = (m *= 2.0f);
        CHECK(m == (m1 * 2.0f));
        CHECK(&ref == &m);
    }

    SUBCASE("operator/=(scalar)") {
        Matrix4 m = m1;
        auto& ref = (m /= 1.0f);
        CHECK(m == m1);
        CHECK(&ref == &m);
    }
}

TEST_CASE("Matrix4 Type Deductions") {
    static_assert(std::is_same_v<Matrix4, Matrix4f>);
}

TEST_CASE("Matrix4 Matrix Operations") {
    SUBCASE("Identity") {
        Matrix4 id = math::factory::Identity();
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (i == j) CHECK(id[i][j] == 1.0f);
                else CHECK(id[i][j] == 0.0f);
            }
        }
    }

    SUBCASE("Translate") {
        math::Vector3f offset(10, 20, 30);
        Matrix4 t = math::factory::Translate(offset);
        // Column-major: translation is in the last column
        CHECK(t[3][0] == 10.0f);
        CHECK(t[3][1] == 20.0f);
        CHECK(t[3][2] == 30.0f);
        CHECK(t[3][3] == 1.0f);
        CHECK(t[0][0] == 1.0f);
        CHECK(t[1][1] == 1.0f);
        CHECK(t[2][2] == 1.0f);
    }

    SUBCASE("Scale") {
        math::Vector3f scale(2, 3, 4);
        Matrix4 s = math::factory::Scale(scale);
        CHECK(s[0][0] == 2.0f);
        CHECK(s[1][1] == 3.0f);
        CHECK(s[2][2] == 4.0f);
        CHECK(s[3][3] == 1.0f);
    }
}

TEST_CASE("Matrix4 Exceptions") {
    Matrix4 m = math::factory::Identity();
    CHECK_THROWS_AS(m / 0.0f, std::invalid_argument);
    CHECK_THROWS_AS(m /= 0.0f, std::invalid_argument);
}

TEST_CASE("Matrix4 Constexpr and Static Assert") {
    SUBCASE("Constexpr construction and access") {
        constexpr Matrix4 m(
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 10, 11, 12,
            13, 14, 15, 16
        );
        static_assert(m[0][0] == 1.0f);
        static_assert(m[3][3] == 16.0f);
    }

    SUBCASE("Constexpr arithmetic") {
        constexpr Matrix4 m1 = math::factory::Identity();
        constexpr Matrix4 m2 = m1 + m1;
        static_assert(m2[0][0] == 2.0f);
        static_assert(m2[1][1] == 2.0f);
        
        constexpr Matrix4 m3 = m1 * 5.0f;
        static_assert(m3[0][0] == 5.0f);
    }
}

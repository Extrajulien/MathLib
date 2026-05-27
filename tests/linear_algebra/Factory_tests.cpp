#include "doctest.h"
#include <cmath>

import MathLib;

using math::Matrix4;
using math::Vector3f;
using math::Angle;

namespace {
    void check_matrix4(const Matrix4& actual, const float expected[16]) {
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                CHECK(actual[col][row] == doctest::Approx(expected[row * 4 + col]).epsilon(0.0001));
            }
        }
    }
}

TEST_CASE("math::factory::Identity") {
    auto m = math::factory::Identity();
    float expected[] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    check_matrix4(m, expected);
}

TEST_CASE("math::factory::Translate") {
    auto m = math::factory::Translate(Vector3f(10, -5, 2.5f));
    float expected[] = {
        1, 0, 0, 10,
        0, 1, 0, -5,
        0, 0, 1, 2.5f,
        0, 0, 0, 1
    };
    check_matrix4(m, expected);
}

TEST_CASE("math::factory::Scale") {
    auto m = math::factory::Scale(Vector3f(2, 0.5f, 10));
    float expected[] = {
        2, 0,    0,  0,
        0, 0.5f, 0,  0,
        0, 0,    10, 0,
        0, 0,    0,  1
    };
    check_matrix4(m, expected);
}

TEST_CASE("math::factory::RotationX") {
    SUBCASE("0 degrees produces identity") {
        auto m = math::factory::RotationX(Angle::fromDegrees(0.0f));
        float expected[] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        check_matrix4(m, expected);
    }

    SUBCASE("90 degrees") {
        auto m = math::factory::RotationX(Angle::fromDegrees(90.0f));
        float expected[] = {
            1, 0,  0, 0,
            0, 0, -1, 0,
            0, 1,  0, 0,
            0, 0,  0, 1
        };
        check_matrix4(m, expected);
    }

    SUBCASE("180 degrees") {
        auto m = math::factory::RotationX(Angle::fromDegrees(180.0f));
        float expected[] = {
            1, 0,  0, 0,
            0, -1, 0, 0,
            0, 0, -1, 0,
            0, 0,  0, 1
        };
        check_matrix4(m, expected);
    }

    SUBCASE("-90 degrees") {
        auto m = math::factory::RotationX(Angle::fromDegrees(-90.0f));
        float expected[] = {
            1, 0, 0, 0,
            0, 0, 1, 0,
            0, -1, 0, 0,
            0, 0, 0, 1
        };
        check_matrix4(m, expected);
    }

    SUBCASE("30 degrees comparison with std::sin/cos") {
        float angle = 30.0f;
        auto m = math::factory::RotationX(Angle::fromDegrees(angle));
        float rad = angle * (3.14159265f / 180.0f);
        float s = std::sin(rad);
        float c = std::cos(rad);
        float expected[] = {
            1, 0,  0, 0,
            0, c, -s, 0,
            0, s,  c, 0,
            0, 0,  0, 1
        };
        check_matrix4(m, expected);
    }
}

TEST_CASE("math::factory::RotationY") {
    SUBCASE("0 degrees produces identity") {
        auto m = math::factory::RotationY(Angle::fromDegrees(0.0f));
        float expected[] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        check_matrix4(m, expected);
    }

    SUBCASE("90 degrees") {
        auto m = math::factory::RotationY(Angle::fromDegrees(90.0f));
        float expected[] = {
             0, 0, 1, 0,
             0, 1, 0, 0,
            -1, 0, 0, 0,
             0, 0, 0, 1
        };
        check_matrix4(m, expected);
    }

    SUBCASE("180 degrees") {
        auto m = math::factory::RotationY(Angle::fromDegrees(180.0f));
        float expected[] = {
            -1, 0,  0, 0,
             0, 1,  0, 0,
             0, 0, -1, 0,
             0, 0,  0, 1
        };
        check_matrix4(m, expected);
    }

    SUBCASE("30 degrees comparison with std::sin/cos") {
        float angle = 30.0f;
        auto m = math::factory::RotationY(Angle::fromDegrees(angle));
        float rad = angle * (3.14159265f / 180.0f);
        float s = std::sin(rad);
        float c = std::cos(rad);
        float expected[] = {
             c, 0, s, 0,
             0, 1, 0, 0,
            -s, 0, c, 0,
             0, 0, 0, 1
        };
        check_matrix4(m, expected);
    }
}

TEST_CASE("math::factory::RotationZ") {
    SUBCASE("0 degrees produces identity") {
        auto m = math::factory::RotationZ(Angle::fromDegrees(0.0f));
        float expected[] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        check_matrix4(m, expected);
    }

    SUBCASE("90 degrees") {
        auto m = math::factory::RotationZ(Angle::fromDegrees(90.0f));
        float expected[] = {
            0, -1, 0, 0,
            1,  0, 0, 0,
            0,  0, 1, 0,
            0,  0, 0, 1
        };
        check_matrix4(m, expected);
    }

    SUBCASE("180 degrees") {
        auto m = math::factory::RotationZ(Angle::fromDegrees(180.0f));
        float expected[] = {
            -1,  0, 0, 0,
             0, -1, 0, 0,
             0,  0, 1, 0,
             0,  0, 0, 1
        };
        check_matrix4(m, expected);
    }

    SUBCASE("30 degrees comparison with std::sin/cos") {
        float angle = 30.0f;
        auto m = math::factory::RotationZ(Angle::fromDegrees(angle));
        float rad = angle * (3.14159265f / 180.0f);
        float s = std::sin(rad);
        float c = std::cos(rad);
        float expected[] = {
            c, -s, 0, 0,
            s,  c, 0, 0,
            0,  0, 1, 0,
            0,  0, 0, 1
        };
        check_matrix4(m, expected);
    }
}

TEST_CASE("math::factory::RotationAxis") {
    SUBCASE("Axis (1,0,0) matches RotationX") {
        auto angle = Angle::fromDegrees(45.0f);
        auto m_axis = math::factory::RotationAxis(angle, Vector3f(1, 0, 0));
        auto m_x = math::factory::RotationX(angle);
        CHECK(m_axis == m_x);
    }

    SUBCASE("Axis (0,1,0) matches RotationY") {
        auto angle = Angle::fromDegrees(45.0f);
        auto m_axis = math::factory::RotationAxis(angle, Vector3f(0, 1, 0));
        auto m_y = math::factory::RotationY(angle);
        CHECK(m_axis == m_y);
    }

    SUBCASE("Axis (0,0,1) matches RotationZ") {
        auto angle = Angle::fromDegrees(45.0f);
        auto m_axis = math::factory::RotationAxis(angle, Vector3f(0, 0, 1));
        auto m_z = math::factory::RotationZ(angle);
        CHECK(m_axis == m_z);
    }

    SUBCASE("Non-normalized axes are normalized internally") {
        auto angle = Angle::fromDegrees(60.0f);
        auto m1 = math::factory::RotationAxis(angle, Vector3f(2, 0, 0));
        auto m_expected = math::factory::RotationX(angle);
        CHECK(m1 == m_expected);

        auto m2 = math::factory::RotationAxis(angle, Vector3f(0, -3, 0));
        auto m_expected2 = math::factory::RotationY(Angle::fromDegrees(-60.0f));
        CHECK(m2 == m_expected2);
    }

    SUBCASE("Arbitrary axis (1,2,3) with Rodrigues formula") {
        float deg = 45.0f;
        Angle angle = Angle::fromDegrees(deg);
        Vector3f axis(1, 2, 3);
        
        // Manual normalization
        float len = std::sqrt(1.0f*1.0f + 2.0f*2.0f + 3.0f*3.0f);
        float ux = 1.0f / len;
        float uy = 2.0f / len;
        float uz = 3.0f / len;
        
        float rad = deg * (3.14159265f / 180.0f);
        float c = std::cos(rad);
        float s = std::sin(rad);
        float omc = 1.0f - c;
        
        float expected[] = {
            c + ux*ux*omc,    ux*uy*omc - uz*s, ux*uz*omc + uy*s, 0,
            uy*ux*omc + uz*s, c + uy*uy*omc,    uy*uz*omc - ux*s, 0,
            uz*ux*omc - uy*s, uz*uy*omc + ux*s, c + uz*uz*omc,    0,
            0,                0,                0,                1
        };
        
        auto m = math::factory::RotationAxis(angle, axis);
        check_matrix4(m, expected);
    }

    SUBCASE("Negative axis with positive angle equivalent to positive axis with negative angle") {
        auto angle = Angle::fromDegrees(30.0f);
        auto axis = Vector3f(1, 2, 3);
        auto m1 = math::factory::RotationAxis(angle, -axis);
        auto m2 = math::factory::RotationAxis(Angle::fromDegrees(-30.0f), axis);
        
        for(int c=0; c<4; ++c)
            for(int r=0; r<4; ++r)
                CHECK(m1[c][r] == doctest::Approx(m2[c][r]));
    }

    SUBCASE("0-degree arbitrary-axis rotation returns identity") {
        auto m = math::factory::RotationAxis(Angle::fromDegrees(0.0f), Vector3f(1, 1, 1));
        float expected[] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        check_matrix4(m, expected);
    }

    SUBCASE("180-degree arbitrary-axis rotation") {
        Vector3f axis(1, 0, 0); 
        auto m = math::factory::RotationAxis(Angle::fromDegrees(180.0f), axis);
        float expected[] = {
            1, 0,  0, 0,
            0, -1, 0, 0,
            0, 0, -1, 0,
            0, 0,  0, 1
        };
        check_matrix4(m, expected);
    }
}

TEST_CASE("math::factory::Perspective") {
    SUBCASE("Known simple case: 90 deg, 16/9, 0.1, 100") {
        float fovX = 90.0f;
        float aspect = 16.0f / 9.0f;
        float near = 0.1f;
        float far = 100.0f;
        
        auto m = math::factory::Perspective(Angle::fromDegrees(fovX), aspect, near, far);
        
        float tangent = std::tan(Angle::fromDegrees(fovX).asRadians() / 2.0f);
        float x_scale = 1.0f / tangent;
        float y_scale = aspect / tangent;
        float z_range = far - near;
        
        float expected[] = {
            x_scale, 0,       0,                                 0,
            0,       y_scale, 0,                                 0,
            0,       0,      -(far + near) / z_range, -(2 * far * near) / z_range,
            0,       0,      -1,                                 0
        };
        
        check_matrix4(m, expected);
    }

    SUBCASE("Multiple FOV/aspect/near/far combinations") {
        auto test_persp = [](float fov, float aspect, float near, float far) {
            auto m = math::factory::Perspective(Angle::fromDegrees(fov), aspect, near, far);
            float tangent = std::tan(Angle::fromDegrees(fov).asRadians() / 2.0f);
            float x_scale = 1.0f / tangent;
            float y_scale = aspect / tangent;
            float z_range = far - near;
            
            CHECK(m[0][0] == doctest::Approx(x_scale));
            CHECK(m[1][1] == doctest::Approx(y_scale));
            CHECK(m[2][2] == doctest::Approx(-(far + near) / z_range));
            CHECK(m[3][2] == doctest::Approx(-(2 * far * near) / z_range));
            CHECK(m[2][3] == doctest::Approx(-1.0f));
        };

        test_persp(60.0f, 1.0f, 0.1f, 100.0f);
        test_persp(90.0f, 16.0f/9.0f, 0.01f, 1000.0f);
        test_persp(120.0f, 4.0f/3.0f, 0.5f, 500.0f);
        test_persp(30.0f, 16.0f/10.0f, 1.0f, 50.0f);
    }

    SUBCASE("Structural properties") {
        auto m = math::factory::Perspective(Angle::fromDegrees(90.0f), 1.0f, 0.1f, 100.0f);
        
        CHECK(m[1][0] == 0.0f);
        CHECK(m[2][0] == 0.0f);
        CHECK(m[3][0] == 0.0f);
        CHECK(m[0][1] == 0.0f);
        CHECK(m[2][1] == 0.0f);
        CHECK(m[3][1] == 0.0f);
        CHECK(m[0][2] == 0.0f);
        CHECK(m[1][2] == 0.0f);
        CHECK(m[0][3] == 0.0f);
        CHECK(m[1][3] == 0.0f);
        CHECK(m[3][3] == 0.0f);
        
        CHECK(m[2][3] == -1.0f);
        
        float aspect = 1.5f;
        auto m2 = math::factory::Perspective(Angle::fromDegrees(60.0f), aspect, 0.1f, 100.0f);
        CHECK(m2[1][1] == doctest::Approx(m2[0][0] * aspect));
        
        CHECK(m[2][2] < 0.0f);
        CHECK(m[3][2] < 0.0f);
    }
}

TEST_CASE("constexpr coverage") {
    constexpr auto id = math::factory::Identity();
    static_assert(id[0][0] == 1.0f);
    
    constexpr auto p = math::factory::Perspective(Angle(1.0f), 1.0f, 0.1f, 100.0f);
    static_assert(p[2][3] == -1.0f);
}

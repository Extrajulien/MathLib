#include <cmath>
#include <numbers>
#include <type_traits>

#include "doctest.h"
import MathLib;

using MathLib::Angle;
using MathLib::Vector2f;

constexpr float pi = std::numbers::pi_v<float>;

constexpr bool approx_equal(float lhs, float rhs, float epsilon = 0.00001f) noexcept {
    return lhs > rhs ? lhs - rhs <= epsilon : rhs - lhs <= epsilon;
}

void check_angle(const Angle& angle, float expectedRadians, float expectedDegrees) {
    CHECK(angle.asRadians() == doctest::Approx(expectedRadians));
    CHECK(angle.asDegrees() == doctest::Approx(expectedDegrees));
}

TEST_CASE("Angle Construction") {
    SUBCASE("Default construction") {
        constexpr Angle a;
        static_assert(a.asRadians() == 0.0f);
        static_assert(a.asDegrees() == 0.0f);
        check_angle(a, 0.0f, 0.0f);
    }

    SUBCASE("Direct radian construction") {
        constexpr float rad = 1.23f;
        constexpr Angle a(rad);
        static_assert(a.asRadians() == rad);
        CHECK(a.asRadians() == rad);

        check_angle(Angle(0.0f), 0.0f, 0.0f);
        check_angle(Angle(pi), pi, 180.0f);
        check_angle(Angle(-pi), -pi, -180.0f);
        check_angle(Angle(3.0f * pi), 3.0f * pi, 540.0f);
    }
}

TEST_CASE("Angle::fromDegrees") {
    SUBCASE("Common degree values") {
        check_angle(Angle::fromDegrees(0.0f), 0.0f, 0.0f);
        check_angle(Angle::fromDegrees(90.0f), pi / 2.0f, 90.0f);
        check_angle(Angle::fromDegrees(180.0f), pi, 180.0f);
        check_angle(Angle::fromDegrees(270.0f), 1.5f * pi, 270.0f);
        check_angle(Angle::fromDegrees(360.0f), 2.0f * pi, 360.0f);
        check_angle(Angle::fromDegrees(-90.0f), -pi / 2.0f, -90.0f);
        check_angle(Angle::fromDegrees(-180.0f), -pi, -180.0f);
        check_angle(Angle::fromDegrees(720.0f), 4.0f * pi, 720.0f);
    }

    SUBCASE("Constexpr fromDegrees") {
        constexpr auto a = Angle::fromDegrees(180.0f);
        static_assert(a.asRadians() == pi);
    }
}

TEST_CASE("Angle::asRadians and asDegrees") {
    SUBCASE("asRadians returns exact stored value") {
        CHECK(Angle(0.5f).asRadians() == 0.5f);
        CHECK(Angle(1.25f).asRadians() == 1.25f);
        CHECK(Angle(-0.75f).asRadians() == -0.75f);
    }

    SUBCASE("asDegrees conversion") {
        CHECK(Angle(0.0f).asDegrees() == doctest::Approx(0.0f));
        CHECK(Angle(pi / 2.0f).asDegrees() == doctest::Approx(90.0f));
        CHECK(Angle(pi).asDegrees() == doctest::Approx(180.0f));
        CHECK(Angle(3.0f * pi / 2.0f).asDegrees() == doctest::Approx(270.0f));
        CHECK(Angle(2.0f * pi).asDegrees() == doctest::Approx(360.0f));
        CHECK(Angle(-pi / 2.0f).asDegrees() == doctest::Approx(-90.0f));
        CHECK(Angle(-pi).asDegrees() == doctest::Approx(-180.0f));
    }

    SUBCASE("Round-trip checks") {
        float degrees[] = {0.0f, 45.0f, 90.0f, 135.0f, 180.0f, -60.0f, 720.0f};
        for (float d : degrees) {
            CHECK(Angle::fromDegrees(d).asDegrees() == doctest::Approx(d));
        }

        float radians[] = {0.0f, pi / 4.0f, pi, -pi / 2.0f, 10.0f};
        for (float r : radians) {
            Angle a(r);
            float d = a.asDegrees();
            CHECK(Angle::fromDegrees(d).asRadians() == doctest::Approx(r));
        }
    }
}

TEST_CASE("Angle Construction from Vector2f") {
    SUBCASE("Axis-aligned vectors") {
        check_angle(Angle(Vector2f(1.0f, 0.0f)), 0.0f, 0.0f);
        check_angle(Angle(Vector2f(0.0f, 1.0f)), pi / 2.0f, 90.0f);
        check_angle(Angle(Vector2f(-1.0f, 0.0f)), pi, 180.0f);
        check_angle(Angle(Vector2f(0.0f, -1.0f)), -pi / 2.0f, -90.0f);
    }

    SUBCASE("Diagonal/Quadrant vectors") {
        check_angle(Angle(Vector2f(1.0f, 1.0f)), pi / 4.0f, 45.0f);
        check_angle(Angle(Vector2f(-1.0f, 1.0f)), 3.0f * pi / 4.0f, 135.0f);
        check_angle(Angle(Vector2f(-1.0f, -1.0f)), -3.0f * pi / 4.0f, -135.0f);
        check_angle(Angle(Vector2f(1.0f, -1.0f)), -pi / 4.0f, -45.0f);
    }

    SUBCASE("Non-unit vectors") {
        check_angle(Angle(Vector2f(10.0f, 0.0f)), 0.0f, 0.0f);
        check_angle(Angle(Vector2f(0.0f, 5.0f)), pi / 2.0f, 90.0f);
        check_angle(Angle(Vector2f(3.0f, 3.0f)), pi / 4.0f, 45.0f);
        check_angle(Angle(Vector2f(-2.0f, -2.0f)), -3.0f * pi / 4.0f, -135.0f);
    }

    SUBCASE("Zero-vector behavior") {
        check_angle(Angle(Vector2f(0.0f, 0.0f)), 0.0f, 0.0f);
    }
}

TEST_CASE("Angle Type and noexcept/constexpr properties") {
    SUBCASE("Return types") {
        static_assert(std::is_same_v<decltype(Angle::fromDegrees(0.0f).asRadians()), float>);
        static_assert(std::is_same_v<decltype(Angle(0.0f).asDegrees()), float>);
    }

    SUBCASE("noexcept properties") {
        static_assert(noexcept(Angle(0.0f).asRadians()));
        static_assert(noexcept(Angle(0.0f).asDegrees()));
        static_assert(noexcept(Angle(Vector2f(0.0f, 0.0f))));
    }

    SUBCASE("Constexpr properties") {
        constexpr Angle a1;
        constexpr Angle a2(1.0f);
        constexpr Angle a3 = Angle::fromDegrees(90.0f);

        constexpr Angle zero(Vector2f(0.0f, 0.0f));
        constexpr Angle right(Vector2f(1.0f, 0.0f));
        constexpr Angle up(Vector2f(0.0f, 1.0f));
        constexpr Angle left(Vector2f(-1.0f, 0.0f));
        constexpr Angle down(Vector2f(0.0f, -1.0f));
        constexpr Angle diagonal(Vector2f(1.0f, 1.0f));

        static_assert(a1.asRadians() == 0.0f);
        static_assert(a2.asRadians() == 1.0f);
        static_assert(approx_equal(a3.asRadians(), pi / 2.0f));

        static_assert(zero.asRadians() == 0.0f);
        static_assert(right.asRadians() == 0.0f);
        static_assert(approx_equal(up.asRadians(), pi / 2.0f));
        static_assert(approx_equal(left.asRadians(), pi));
        static_assert(approx_equal(down.asRadians(), -pi / 2.0f));
        static_assert(approx_equal(diagonal.asRadians(), pi / 4.0f));

        check_angle(zero, 0.0f, 0.0f);
        check_angle(right, 0.0f, 0.0f);
        check_angle(up, pi / 2.0f, 90.0f);
        check_angle(left, pi, 180.0f);
        check_angle(down, -pi / 2.0f, -90.0f);
        check_angle(diagonal, pi / 4.0f, 45.0f);
    }
}

TEST_CASE("Angle Precision and Edge Cases") {
    SUBCASE("Small values") {
        check_angle(Angle::fromDegrees(0.001f), 0.001f * (pi / 180.0f), 0.001f);
        check_angle(Angle(0.001f), 0.001f, 0.001f * (180.0f / pi));
    }

    SUBCASE("Large values") {
        check_angle(Angle::fromDegrees(1080.0f), 6.0f * pi, 1080.0f);
        check_angle(Angle::fromDegrees(-1080.0f), -6.0f * pi, -1080.0f);
    }
}
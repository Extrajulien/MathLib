module;

#include <cmath>

export module MathLib:LinearFactory;
import :Matrix4;
import :Vector3;
import :Vector4;
import :Angle;
import :internal;

namespace math::factory {

    export [[nodiscard]] constexpr Matrix4 Identity() noexcept {
        return {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
    }

    export [[nodiscard]] constexpr Matrix4 Translate(const Vector3f& offset) noexcept {
        Matrix4 result = Identity();
        result[3][0] = offset.x;
        result[3][1] = offset.y;
        result[3][2] = offset.z;
        return result;
    }

    export [[nodiscard]] constexpr Matrix4 Scale(const Vector3f& scale) noexcept {
        Matrix4 result = Identity();
        result[0][0] = scale.x;
        result[1][1] = scale.y;
        result[2][2] = scale.z;
        return result;
    }


    export [[nodiscard]] constexpr Matrix4 RotationX(const Angle& angle) noexcept {
        const float a = angle.asRadians();
        const float sin = std::sin(a);
        const float cos = std::cos(a);
        return {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f,  cos, -sin, 0.0f,
            0.0f,  sin,  cos, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};
    }

    export [[nodiscard]] constexpr Matrix4 RotationY(const Angle& angle) noexcept {
        const float a = angle.asRadians();
        const float sin = std::sin(a);
        const float cos = std::cos(a);
        return {
            cos, 0.0f,  sin, 0.0f,
           0.0f, 1.0f, 0.0f, 0.0f,
           -sin, 0.0f,  cos, 0.0f,
           0.0f, 0.0f, 0.0f, 1.0f};
    }

    export [[nodiscard]] constexpr Matrix4 RotationZ(const Angle& angle) noexcept {
        const float a = angle.asRadians();
        const float sin = std::sin(a);
        const float cos = std::cos(a);
        return {
            cos, -sin, 0.0f, 0.0f,
            sin,  cos, 0.0f, 0.0f,
           0.0f, 0.0f, 1.0f, 0.0f,
           0.0f, 0.0f, 0.0f, 1.0f};
    }

    /**
     * Creates a right-handed perspective projection matrix (OpenGL style)
     * @param fovX Horizontal Field of View
     * @param aspectRatio Width divided by height (w/h)
     * @param nearPlane Distance to the closer clipping plane
     * @param farPlane Distance to the further clipping plane
     */
    export [[nodiscard]] constexpr Matrix4 Perspective(const Angle& fovX, const float aspectRatio,
        const float nearPlane, const float farPlane) noexcept {

        float tangent = 1;
        if consteval {
            tangent = constexpr_tan(fovX.asRadians() / 2);
        } else {
            tangent = std::tan(fovX.asRadians() / 2);
        }

        const float x_scale = 1.0f / tangent;
        const float y_scale = aspectRatio / tangent;
        const float z_range = farPlane - nearPlane;

        return Matrix4{
            x_scale, 0,       0,                                 0,
            0,       y_scale, 0,                                 0,
            0,       0,      -(farPlane + nearPlane) / z_range, -(2 * farPlane * nearPlane) / z_range,
            0,       0,      -1,                                 0
        };
    }

    /**
     * Creates a rotation matrix around an arbitrary 3D unit vector axis
     * @param angle The amount of rotation
     * @param axis The 3D vector axis to rotate around (automatically normalized)
     */
    export template <typename T = float>
    [[nodiscard]] constexpr Matrix4 RotationAxis(const Angle& angle, const Vector3<T>& axis) noexcept {
        Vector3<T> vec = normalize(axis);

        const T c = std::cos(angle.asRadians());
        const T s = std::sin(angle.asRadians());
        const T omc = T(1) - c; // "One Minus Cosine"

        const T x = vec.x;
        const T y = vec.y;
        const T z = vec.z;

        // Construct the rotation matrix rows based on Rodrigues' formula
        return Matrix4{
            c + x * x * omc,     x * y * omc - z * s, x * z * omc + y * s, T(0),
            y * x * omc + z * s, c + y * y * omc,     y * z * omc - x * s, T(0),
            z * x * omc - y * s, z * y * omc + x * s, c + z * z * omc,     T(0),
            T(0),                T(0),                T(0),                T(1)
        };
    }
}

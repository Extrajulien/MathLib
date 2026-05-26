module;
export module MathLib:LinearFactory;
import :Matrix4;
import :Vector3;
import :Vector4;

namespace math::factory {

    export [[nodiscard]] static constexpr Matrix4 Identity() {
        return {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
    }

    export [[nodiscard]] static constexpr Matrix4 Translate(const Vector3f& offset) {
        Matrix4 result = Identity();
        result[3][0] = offset.x;
        result[3][1] = offset.y;
        result[3][2] = offset.z;
        return result;
    }

    export [[nodiscard]] static constexpr Matrix4 Scale(const Vector3f& scale) {
        Matrix4 result = Identity();
        result[0][0] = scale.x;
        result[1][1] = scale.y;
        result[2][2] = scale.z;
        return result;
    }
}
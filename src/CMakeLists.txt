target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET CXX_MODULES FILES
        linear_algebra/Vector2.cppm
        linear_algebra/Vector3.cppm
        linear_algebra/Vector4.cppm
        linear_algebra/Matrix4.cppm
        linear_algebra/Factory.cppm
        trigonometry/Angle.cppm
        internal/math_helpers.cppm
        MathLib.cppm
        PRIVATE
        # non modules files
)
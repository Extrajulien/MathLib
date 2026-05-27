module;

#include <cmath>
#include <numbers>
#include <stdexcept>

export module MathLib:Angle;
import :Vector2;
import :internal;
namespace math {
    export class Angle {
        float m_radians = 0;

    public:
        constexpr Angle() noexcept = default;
        constexpr Angle(const float radians) noexcept : m_radians(radians) {}


        [[nodiscard]] static constexpr Angle fromDegrees(const float degrees) noexcept {
            return {
                degrees * (std::numbers::pi_v<float> / 180.0f)
            };
        }

        [[nodiscard]] constexpr float asRadians() const noexcept {
            return m_radians;
        }

        [[nodiscard]] constexpr float asDegrees() const noexcept {
            return m_radians * (180.0f / std::numbers::pi_v<float>);
        }


        /**
             * @brief Constructs an Angle from a directional 2D vector.
             * @details Computes the counter-clockwise angle between the positive X-axis
             * and the given vector using std::atan2.
             * @note A zero-length vector (0,0) will default the angle safely to 0.0 radians.
             * @param vec The direction vector to extract the angle from.
             */
        constexpr Angle(const Vector2f& vec) noexcept {
            if (vec.x == 0.0f && vec.y == 0.0f) {
                m_radians = 0.0f;
            } else {
                if consteval {
                    m_radians = constexpr_atan2(vec.y, vec.x);
                } else {
                    m_radians = std::atan2(vec.y, vec.x);
                }
            }
        }
    };

    export constexpr Angle operator*(const Angle angle, const float rhs) noexcept {
        return {
            angle.asRadians() * rhs
        };
    }

    export constexpr Angle operator/(const Angle angle, const float rhs) {
        if (rhs == 0) [[unlikely]] { throw std::invalid_argument("cannot divide Angle components by 0."); }
        return {
            angle.asRadians() / rhs
        };
    }
}
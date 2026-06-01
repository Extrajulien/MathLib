module;
#include <array>
#include <cmath>
#include <iostream>

export module MathLib:Vector4;
import :internal;

namespace MathLib {
    export
    template<typename T = float>
    struct Vector4 {
        T x, y, z, w;

        constexpr Vector4(const T x,const T y,const T z, const T w) noexcept : x(x), y(y), z(z), w(w) {}
        constexpr explicit Vector4(const std::array<T, 4> array) noexcept : x(array[0]), y(array[1]), z(array[2]), w(array[3]) {}

        constexpr std::array<T, 4> to_array() const noexcept {
            return {x, y, z, w};
        }

        template <typename U>
        constexpr Vector4& operator=(const std::array<U, 4>& rhs) noexcept {
            this->x = static_cast<T>( rhs[0] );
            this->y = static_cast<T>( rhs[1] );
            this->z = static_cast<T>( rhs[2] );
            this->w = static_cast<T>( rhs[3] );
            return *this;
        }

        template <typename U>
        constexpr Vector4& operator=(const Vector4<U>& rhs) noexcept {
            this->x = static_cast<T>( rhs.x );
            this->y = static_cast<T>( rhs.y );
            this->z = static_cast<T>( rhs.z );
            this->w = static_cast<T>( rhs.w );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr Vector4& operator+=(const Vector4<U> &rhs) noexcept {
            this->x = static_cast<T>( this->x + rhs.x );
            this->y = static_cast<T>( this->y + rhs.y );
            this->z = static_cast<T>( this->z + rhs.z );
            this->w = static_cast<T>( this->w + rhs.w );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr Vector4& operator*=(const Vector4<U>& rhs) noexcept {
            this->x = static_cast<T>( this->x * rhs.x );
            this->y = static_cast<T>( this->y * rhs.y );
            this->z = static_cast<T>( this->z * rhs.z );
            this->w = static_cast<T>( this->w * rhs.w );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr Vector4& operator*=(const U rhs) noexcept {
            this->x = static_cast<T>( this->x * rhs );
            this->y = static_cast<T>( this->y * rhs );
            this->z = static_cast<T>( this->z * rhs );
            this->w = static_cast<T>( this->w * rhs );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr Vector4& operator-=(const Vector4<U>& rhs) noexcept {
            this->x = static_cast<T>( this->x - rhs.x );
            this->y = static_cast<T>( this->y - rhs.y );
            this->z = static_cast<T>( this->z - rhs.z );
            this->w = static_cast<T>( this->w - rhs.w );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<T, U> && std::convertible_to<U, double>
        constexpr Vector4& operator/=(const U rhs) {
            if (rhs == 0) [[unlikely]] { throw std::invalid_argument("cannot divide Vector4 components by 0."); }
            using DivType = std::conditional_t<std::is_integral_v<U>, double, U>;
            this->x = static_cast<T>( this->x / static_cast<DivType>( rhs ) );
            this->y = static_cast<T>( this->y / static_cast<DivType>( rhs ) );
            this->z = static_cast<T>( this->z / static_cast<DivType>( rhs ) );
            this->w = static_cast<T>( this->w / static_cast<DivType>( rhs ) );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<T, U>
        constexpr Vector4& operator/=(const Vector4<U>& rhs) {
            using DivType = std::conditional_t<std::is_integral_v<U>, double, U>;
            if (rhs.x == 0 || rhs.y == 0 || rhs.z == 0 || rhs.w == 0) [[unlikely]] { throw std::invalid_argument("cannot divide Vector4 components by 0."); }
            this->x = static_cast<T>( this->x / static_cast<DivType>( rhs.x ) );
            this->y = static_cast<T>( this->y / static_cast<DivType>( rhs.y ) );
            this->z = static_cast<T>( this->z / static_cast<DivType>( rhs.z ) );
            this->w = static_cast<T>( this->w / static_cast<DivType>( rhs.w ) );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr bool operator==(const Vector4<U> &rhs) const noexcept {
            return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr bool operator!=(const Vector4<U> &rhs) const noexcept {
            return this->x != rhs.x || this->y != rhs.y || this->z != rhs.z || this->w != rhs.w;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector4<T>& vec) {
            os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
            return os;
        }
    };

    export using Vector4f = Vector4<>;
    export using Vector4i = Vector4<int>;
    export using Vector4d = Vector4<double>;

    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator+(const Vector4<T>& lhs, const Vector4<U>& rhs) noexcept {
        return Vector4<decltype(std::declval<T>() + std::declval<U>())>(
            lhs.x + rhs.x,
            lhs.y + rhs.y,
            lhs.z + rhs.z,
            lhs.w + rhs.w
        );
    }

    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator*(const Vector4<T>& lhs, const Vector4<U>& rhs) noexcept {
        return Vector4<decltype(std::declval<T>() * std::declval<U>())>(
            lhs.x * rhs.x,
            lhs.y * rhs.y,
            lhs.z * rhs.z,
            lhs.w * rhs.w
        );
    }

    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator*(const Vector4<T>& lhs, const U rhs) noexcept {
        return Vector4<decltype(std::declval<T>() * std::declval<U>())>(
            lhs.x * rhs,
            lhs.y * rhs,
            lhs.z * rhs,
            lhs.w * rhs
        );
    }


    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator-(const Vector4<T>& lhs, const Vector4<U>& rhs) noexcept {
        return Vector4<decltype(std::declval<T>() - std::declval<U>())>(
            lhs.x - rhs.x,
            lhs.y - rhs.y,
            lhs.z - rhs.z,
            lhs.w - rhs.w
        );
    }

    export
    template<typename T>
    constexpr auto operator-(const Vector4<T>& lhs) noexcept {
        return Vector4<decltype(-std::declval<T>())>(
            -lhs.x,
            -lhs.y,
            -lhs.z,
            -lhs.w
        );
    }

    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator/(const Vector4<T>& lhs, const Vector4<U>& rhs) -> Vector4<PrecisionResult<T, U>> {
        using ReturnType = PrecisionResult<T, U>;
        using DivType = std::conditional_t<std::is_integral_v<U>, double, U>;
        if (rhs.x == 0 || rhs.y == 0 || rhs.z == 0 || rhs.w == 0) [[unlikely]]  { throw std::invalid_argument("cannot divide Vector4 components by 0."); }
        return {
            static_cast<ReturnType>( lhs.x / static_cast<DivType>( rhs.x ) ),
            static_cast<ReturnType>( lhs.y / static_cast<DivType>( rhs.y ) ),
            static_cast<ReturnType>( lhs.z / static_cast<DivType>( rhs.z ) ),
            static_cast<ReturnType>( lhs.w / static_cast<DivType>( rhs.w ) )
        };
    }

    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator/(const Vector4<T>& lhs, const U rhs) -> Vector4<PrecisionResult<T, U>> {
        using ReturnType = PrecisionResult<T, U>;
        using DivType = std::conditional_t<std::is_integral_v<U>, double, U>;
        if (rhs == 0) [[unlikely]] { throw std::invalid_argument("cannot divide Vector4 components by 0."); }
        double invRhs = 1.0 / static_cast<DivType>( rhs );
        return {
            static_cast<ReturnType>( lhs.x * invRhs ),
            static_cast<ReturnType>( lhs.y * invRhs ),
            static_cast<ReturnType>( lhs.z * invRhs ),
            static_cast<ReturnType>( lhs.w * invRhs )
        };
    }

    export
    template<typename T, typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto dot(const Vector4<T>& lhs, const Vector4<U>& rhs) noexcept -> PrecisionResult<T, U> {
        using ReturnType = PrecisionResult<T, U>;
        return static_cast<ReturnType>(lhs.x) * static_cast<ReturnType>(rhs.x) +
               static_cast<ReturnType>(lhs.y) * static_cast<ReturnType>(rhs.y) +
               static_cast<ReturnType>(lhs.z) * static_cast<ReturnType>(rhs.z) +
               static_cast<ReturnType>(lhs.w) * static_cast<ReturnType>(rhs.w);
    }

    export
    template<typename T>
    requires ArithmeticTypes<T>
    constexpr Vector4<T> normalize(const Vector4<T>& vec) noexcept {
        if (vec.x == static_cast<T>(0) && vec.y == static_cast<T>(0) && vec.z == static_cast<T>(0) && vec.w == static_cast<T>(0)) {
            return vec;
        }

        const T length_sqr = dot(vec, vec);
        T invLength = 0;
        if consteval {
            invLength = static_cast<T>(1) / static_cast<T>(constexpr_sqrt(length_sqr));
        } else {
            invLength = static_cast<T>(1) / static_cast<T>(std::sqrt(length_sqr));
        }

        return {
            static_cast<T>(vec.x * invLength),
            static_cast<T>(vec.y * invLength),
            static_cast<T>(vec.z * invLength),
            static_cast<T>(vec.w * invLength)
        };
    }
}
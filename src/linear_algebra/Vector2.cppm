module;

#include <array>
#include <cmath>
#include <iostream>
#include <concepts>
#include <stdexcept>
#include <type_traits>


export module MathLib:Vector2;
import :internal;

namespace MathLib {

    export
    template <typename T = float>
    struct Vector2 {
        T x, y;

        constexpr Vector2(const T x,const T y) noexcept : x(x), y(y) {}
        constexpr explicit Vector2(const std::array<T, 2> array) noexcept : x(array[0]), y(array[1]) {}

        constexpr std::array<T, 2> to_array() const noexcept {
            return {x, y};
        }

        template <typename U>
        constexpr Vector2& operator=(const std::array<U, 2>& rhs) noexcept {
            this->x = static_cast<T>( rhs[0] );
            this->y = static_cast<T>( rhs[1] );
            return *this;
        }

        template <typename U>
        constexpr Vector2& operator=(const Vector2<U>& rhs) noexcept {
            this->x = static_cast<T>( rhs.x );
            this->y = static_cast<T>( rhs.y );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr Vector2& operator+=(const Vector2<U> &rhs) noexcept {
            this->x = static_cast<T>( this->x + rhs.x );
            this->y = static_cast<T>( this->y + rhs.y );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr Vector2& operator*=(const Vector2<U>& rhs) noexcept {
            this->x = static_cast<T>( this->x * rhs.x );
            this->y = static_cast<T>( this->y * rhs.y );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr Vector2& operator*=(const U rhs) noexcept {
            this->x = static_cast<T>( this->x * rhs );
            this->y = static_cast<T>( this->y * rhs );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr Vector2& operator-=(const Vector2<U>& rhs) noexcept {
            this->x = static_cast<T>( this->x - rhs.x );
            this->y = static_cast<T>( this->y - rhs.y );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<T, U> && std::convertible_to<U, double>
        constexpr Vector2& operator/=(const U rhs) {
            if (rhs == 0) [[unlikely]] { throw std::invalid_argument("cannot divide Vector2 components by 0."); }
            using DivType = std::conditional_t<std::is_integral_v<U>, double, U>;
            this->x = static_cast<T>( this->x / static_cast<DivType>( rhs ) );
            this->y = static_cast<T>( this->y / static_cast<DivType>( rhs ) );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<T, U>
        constexpr Vector2& operator/=(const Vector2<U>& rhs) {
            using DivType = std::conditional_t<std::is_integral_v<U>, double, U>;
            if (rhs.x == 0 || rhs.y == 0) [[unlikely]] { throw std::invalid_argument("cannot divide Vector2 components by 0."); }
            this->x = static_cast<T>( this->x / static_cast<DivType>( rhs.x ) );
            this->y = static_cast<T>( this->y / static_cast<DivType>( rhs.y ) );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr bool operator==(const Vector2<U> &rhs) const noexcept {
            return this->x == rhs.x && this->y == rhs.y;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr bool operator!=(const Vector2<U> &rhs) const noexcept {
            return this->x != rhs.x || this->y != rhs.y;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector2<T>& vec) {
            os << "(" << vec.x << ", " << vec.y << ")";
            return os;
        }
    };

    export using Vector2f = Vector2<>;
    export using Vector2i = Vector2<int>;
    export using Vector2d = Vector2<double>;

    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator+(const Vector2<T>& lhs, const Vector2<U>& rhs) noexcept {
        return Vector2<decltype(std::declval<T>() + std::declval<U>())>(
            lhs.x + rhs.x,
            lhs.y + rhs.y
        );
    }

    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator*(const Vector2<T>& lhs, const Vector2<U>& rhs) noexcept {
        return Vector2<decltype(std::declval<T>() * std::declval<U>())>(
            lhs.x * rhs.x,
            lhs.y * rhs.y
        );
    }

    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator*(const Vector2<T>& lhs, const U rhs) noexcept {
        return Vector2<decltype(std::declval<T>() * std::declval<U>())>(
            lhs.x * rhs,
            lhs.y * rhs
        );
    }


    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator-(const Vector2<T>& lhs, const Vector2<U>& rhs) noexcept {
        return Vector2<decltype(std::declval<T>() - std::declval<U>())>(
            lhs.x - rhs.x,
            lhs.y - rhs.y
        );
    }

    export
    template<typename T>
    constexpr auto operator-(const Vector2<T>& lhs) noexcept {
        return Vector2<decltype(-std::declval<T>())>(
            -lhs.x,
            -lhs.y
        );
    }

    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator/(const Vector2<T>& lhs, const Vector2<U>& rhs) -> Vector2<PrecisionResult<T, U>> {
        using ReturnType = PrecisionResult<T, U>;
        if (rhs.x == 0 || rhs.y == 0) [[unlikely]] { throw std::invalid_argument("cannot divide Vector2 components by 0."); }
        return {
            static_cast<ReturnType>( lhs.x / static_cast<double>( rhs.x ) ),
            static_cast<ReturnType>( lhs.y / static_cast<double>( rhs.y ) )
        };
    }

    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator/(const Vector2<T>& lhs, const U rhs) -> Vector2<PrecisionResult<T, U>> {
        using ReturnType = PrecisionResult<T, U>;
        if (rhs == 0) [[unlikely]] { throw std::invalid_argument("cannot divide Vector2 components by 0."); }
        double invRhs = 1.0 / static_cast<double>( rhs );
        return {
            static_cast<ReturnType>( lhs.x * invRhs ),
            static_cast<ReturnType>( lhs.y * invRhs )
        };
    }

    export
    template<typename T, typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto dot(const Vector2<T>& lhs, const Vector2<U>& rhs) noexcept -> PrecisionResult<T, U> {
        using ReturnType = PrecisionResult<T, U>;

        return static_cast<ReturnType>(lhs.x) * static_cast<ReturnType>(rhs.x) +
               static_cast<ReturnType>(lhs.y) * static_cast<ReturnType>(rhs.y);
    }

    export
    template<typename T>
    requires ArithmeticTypes<T>
    constexpr Vector2<T> normalize(const Vector2<T>& vec) noexcept {
        if (vec.x == static_cast<T>(0) && vec.y == static_cast<T>(0)) {
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
            static_cast<T>(vec.y * invLength)
        };
    }

}



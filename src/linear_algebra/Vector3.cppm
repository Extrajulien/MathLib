module;
#include <array>
#include <cmath>
#include <iostream>

export module MathLib:Vector3;
import :internal;


namespace math {
    export
    template <typename T = float>
    struct Vector3 {
        T x, y, z;

        constexpr Vector3(const T x,const T y,const T z) : x(x), y(y), z(z) {}
        constexpr explicit Vector3(const std::array<T, 3> array): x(array[0]), y(array[1]), z(array[2]) {}

        constexpr std::array<T, 3> to_array() const {
            return {x, y, z};
        }

        template <typename U>
        constexpr Vector3& operator=(const std::array<U, 3>& rhs) {
            this->x = static_cast<T>( rhs[0] );
            this->y = static_cast<T>( rhs[1] );
            this->z = static_cast<T>( rhs[2] );
            return *this;
        }

        template <typename U>
        constexpr Vector3& operator=(const Vector3<U>& rhs) {
            this->x = static_cast<T>( rhs.x );
            this->y = static_cast<T>( rhs.y );
            this->z = static_cast<T>( rhs.z );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr Vector3& operator+=(const Vector3<U> &rhs) {
            this->x = static_cast<T>( this->x + rhs.x );
            this->y = static_cast<T>( this->y + rhs.y );
            this->z = static_cast<T>( this->z + rhs.z );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr Vector3& operator*=(const Vector3<U>& rhs) {
            this->x = static_cast<T>( this->x * rhs.x );
            this->y = static_cast<T>( this->y * rhs.y );
            this->z = static_cast<T>( this->z * rhs.z );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr Vector3& operator*=(const U rhs) {
            this->x = static_cast<T>( this->x * rhs );
            this->y = static_cast<T>( this->y * rhs );
            this->z = static_cast<T>( this->z * rhs );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr Vector3& operator-=(const Vector3<U>& rhs) {
            this->x = static_cast<T>( this->x - rhs.x );
            this->y = static_cast<T>( this->y - rhs.y );
            this->z = static_cast<T>( this->z - rhs.z );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<T, U> && std::convertible_to<U, double>
        constexpr Vector3& operator/=(const U rhs) {
            if (rhs == 0) [[unlikely]] { throw std::invalid_argument("cannot divide Vector3 components by 0."); }
            using DivType = std::conditional_t<std::is_integral_v<U>, double, U>;
            this->x = static_cast<T>( this->x / static_cast<DivType>( rhs ) );
            this->y = static_cast<T>( this->y / static_cast<DivType>( rhs ) );
            this->z = static_cast<T>( this->z / static_cast<DivType>( rhs ) );
            return *this;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr bool operator==(const Vector3<U> &rhs) const {
            return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z;
        }

        template<typename U>
        requires ArithmeticTypes<U, T>
        constexpr bool operator!=(const Vector3<U> &rhs) const {
            return this->x != rhs.x || this->y != rhs.y || this->z != rhs.z;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector3<T>& vec) {
            os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
            return os;
        }
    };

    export using Vector3f = Vector3<>;
    export using Vector3i = Vector3<int>;
    export using Vector3d = Vector3<double>;

    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator+(const Vector3<T>& lhs, const Vector3<U>& rhs) {
        return Vector3<decltype(std::declval<T>() + std::declval<U>())>(
            lhs.x + rhs.x,
            lhs.y + rhs.y,
            lhs.z + rhs.z
        );
    }

    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator*(const Vector3<T>& lhs, const Vector3<U>& rhs) {
        return Vector3<decltype(std::declval<T>() * std::declval<U>())>(
            lhs.x * rhs.x,
            lhs.y * rhs.y,
            lhs.z * rhs.z
        );
    }

    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator*(const Vector3<T>& lhs, const U rhs) {
        return Vector3<decltype(std::declval<T>() * std::declval<U>())>(
            lhs.x * rhs,
            lhs.y * rhs,
            lhs.z * rhs
        );
    }


    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator-(const Vector3<T>& lhs, const Vector3<U>& rhs) {
        return Vector3<decltype(std::declval<T>() - std::declval<U>())>(
            lhs.x - rhs.x,
            lhs.y - rhs.y,
            lhs.z - rhs.z
        );
    }

    export
    template<typename T>
    constexpr auto operator-(const Vector3<T>& lhs) {
        return Vector3<decltype(-std::declval<T>())>(
            -lhs.x,
            -lhs.y,
            -lhs.z
        );
    }

    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator/(const Vector3<T>& lhs, const Vector3<U>& rhs) -> Vector3<PrecisionResult<T, U>> {
        using ReturnType = PrecisionResult<T, U>;
        if (rhs.x == 0 || rhs.y == 0 || rhs.z == 0) [[unlikely]] { throw std::invalid_argument("cannot divide Vector3 components by 0."); }
        return {
            static_cast<ReturnType>( lhs.x / static_cast<double>( rhs.x ) ),
            static_cast<ReturnType>( lhs.y / static_cast<double>( rhs.y ) ),
            static_cast<ReturnType>( lhs.z / static_cast<double>( rhs.z ) )
        };
    }

    export
    template<typename T,typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto operator/(const Vector3<T>& lhs, const U rhs) -> Vector3<PrecisionResult<T, U>> {
        using ReturnType = PrecisionResult<T, U>;
        if (rhs == 0) [[unlikely]] { throw std::invalid_argument("cannot divide Vector3 components by 0."); }
        double invRhs = 1.0 / static_cast<double>( rhs );
        return {
            static_cast<ReturnType>( lhs.x * invRhs ),
            static_cast<ReturnType>( lhs.y * invRhs ),
            static_cast<ReturnType>( lhs.z * invRhs )
        };
    }

    export
    template<typename T, typename U>
    requires ArithmeticTypes<T, U>
    constexpr auto dot(const Vector3<T>& lhs, const Vector3<U>& rhs) -> PrecisionResult<T, U> {
        using ReturnType = PrecisionResult<T, U>;
        return static_cast<ReturnType>(lhs.x) * static_cast<ReturnType>(rhs.x) +
               static_cast<ReturnType>(lhs.y) * static_cast<ReturnType>(rhs.y) +
               static_cast<ReturnType>(lhs.z) * static_cast<ReturnType>(rhs.z);
    }

    export
    template<typename T, typename U>
    constexpr auto cross(const Vector3<T>& lhs, const Vector3<U>& rhs) -> Vector3<PrecisionResult<T, U>> {
        using ReturnType = PrecisionResult<T, U>;

        return Vector3<ReturnType> {
            lhs.y * rhs.z - lhs.z * rhs.y,
            lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.x * rhs.y - lhs.y * rhs.x
        };
    }

    export
    template<typename T>
    requires ArithmeticTypes<T>
    constexpr Vector3<T> normalize(const Vector3<T>& vec) {
        if (vec.x == static_cast<T>(0) && vec.y == static_cast<T>(0) && vec.z == static_cast<T>(0)) {
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
            static_cast<T>(vec.z * invLength)
        };
    }
}
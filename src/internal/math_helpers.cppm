module;

#include <stdexcept>

export module MathLib:internal;

template<typename T, typename U = int>
concept ArithmeticTypes = std::floating_point<T> || std::integral<T> &&
                          std::floating_point<U> || std::integral<U>;

template<typename T, typename U>
using PrecisionResult = decltype(std::declval<T>() / std::declval<U>());


/**
 * constant expression square root for <b>MathLib</b>
 * @tparam T numeric type
 * @param x numeric value
 * @return square root of <b>x</b>
 */
template <typename T>
constexpr T constexpr_sqrt(T x) {
    if (x < 0) throw std::domain_error("Negative sqrt");
    if (x == 0 || x == 1) return x;

    T current = x / 2;
    T previous = 0;
    while (current != previous) {
        previous = current;
        current = (current + x / current) / 2;
    }
    return current;
}


namespace internal {

    template <typename T>
    constexpr T pi = T(3.141592653589793238462643383279502884L);

    template <typename T>
    constexpr T pi_2 = pi<T> / 2;

    /**
     * Highly accurate minimax polynomial approximation for arctan(x) over [-1, 1]
     * Eliminates the Taylor series edge-case precision drop at x = 1.0
     */
    template <typename T>
    constexpr T constexpr_atan_precise(T x) {
        // Treat as a polynomial in x^2 using Horner's scheme
        T x2 = x * x;

        // Minimax coefficients optimizing maximum error over [-1, 1]
        T res = T(-0.013480470L);
        res = res * x2 + T(0.057477314L);
        res = res * x2 - T(0.121239071L);
        res = res * x2 + T(0.195635925L);
        res = res * x2 - T(0.332994597L);
        res = res * x2 + T(0.999995630L);

        return x * res;
    }

    /**
     * Internal constexpr arctangent using argument reduction to |x| <= 1
     */
    template <typename T>
    constexpr T constexpr_atan(T x) {
        if (x > 1) {
            return pi_2<T> - constexpr_atan_precise(1 / x);
        }
        if (x < -1) {
            return -pi_2<T> - constexpr_atan_precise(1 / x);
        }
        return constexpr_atan_precise(x);
    }
}


/**
 * constant expression 2-argument arctangent for MathLib
 * @tparam T floating-point numeric type
 * @param y ordinate value
 * @param x abscissa value
 * @return principal value of the arc tangent of y/x
 */
template <typename T>
requires std::is_floating_point_v<T>
constexpr T constexpr_atan2(T y, T x) noexcept {

        // Special case handling
        if (x == 0) {
            if (y > 0) return internal::pi_2<T>;
            if (y < 0) return -internal::pi_2<T>;
            return 0;
        }

        T ratio = y / x;
        T angle = internal::constexpr_atan(ratio);

        // Quadrant adjustments
        if (x < 0) {
            if (y >= 0) {
                angle += internal::pi<T>;
            } else {
                angle -= internal::pi<T>;
            }
        }
        return angle;
}
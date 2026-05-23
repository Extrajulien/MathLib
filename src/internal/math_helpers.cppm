module;

#include <stdexcept>

export module MathLib:internal;


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
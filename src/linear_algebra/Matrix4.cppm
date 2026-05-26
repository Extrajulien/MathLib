module;

#include <array>
#include <cmath>
#include <iostream>

export module MathLib:Matrix4;
import :internal;
namespace math {
    //for SIMD optimization matrix4 only supports float
    struct alignas(64) Matrix4 {
        //the column contains the row (accessed [col][row])
    private:
        struct alignas(16) Column {
            std::array<float, 4> rowEntry;

            constexpr float& operator[](const std::size_t col) {
                return rowEntry[col];
            }

            constexpr const float& operator[](const std::size_t col) const {
                return rowEntry[col];
            }
        };
        std::array<Column,4> matrix;
    public:
        constexpr Matrix4(const float n00, const float n01, const float n02, const float n03,
            const float n10, const float n11, const float n12, const float n13,
            const float n20, const float n21, const float n22, const float n23,
            const float n30, const float n31, const float n32, const float n33) : matrix({
                std::array<float, 4>({n00, n10, n20, n30}),
                std::array<float, 4>({n01, n11, n21, n31}),
                std::array<float, 4>({n02, n12, n22, n32}),
                std::array<float, 4>({n03, n13, n23, n33})
            }) {}

        Column& operator[](const size_t row) {
            return matrix[row];
        }
        const Column& operator[](const size_t row) const {
            return matrix[row];
        }

        Matrix4& operator+=(Matrix4 const& other) {
            *this = *this + other;
            return *this;
        }

        Matrix4& operator-=(Matrix4 const& other) {
            *this = *this - other;
            return *this;
        }

        Matrix4& operator*=(const Matrix4 &other) {
            *this = *this * other;
            return *this;
        }

        Matrix4& operator*=(float const& scalar) {
            *this = *this * scalar;
            return *this;
        }

        Matrix4& operator= (const Matrix4 &other) {
            matrix[0][0] = other[0][0];
            matrix[0][1] = other[0][1];
            matrix[0][2] = other[0][2];
            matrix[0][3] = other[0][3];

            matrix[1][0] = other[1][0];
            matrix[1][1] = other[1][1];
            matrix[1][2] = other[1][2];
            matrix[1][3] = other[1][3];

            matrix[2][0] = other[2][0];
            matrix[2][1] = other[2][1];
            matrix[2][2] = other[2][2];
            matrix[2][3] = other[2][3];

            matrix[3][0] = other[3][0];
            matrix[3][1] = other[3][1];
            matrix[3][2] = other[3][2];
            matrix[3][3] = other[3][3];
            return *this;
        }

    };

    Matrix4 operator+(Matrix4 const& lhs,Matrix4 const& rhs) {
    return {
            lhs[0][0]+rhs[0][0],lhs[1][0]+rhs[1][0],lhs[2][0]+rhs[2][0],lhs[3][0]+rhs[3][0],
            lhs[0][1]+rhs[0][1],lhs[1][1]+rhs[1][1],lhs[2][1]+rhs[2][1],lhs[3][1]+rhs[3][1],
            lhs[0][2]+rhs[0][2],lhs[1][2]+rhs[1][2],lhs[2][2]+rhs[2][2],lhs[3][2]+rhs[3][2],
            lhs[0][3]+rhs[0][3],lhs[1][3]+rhs[1][3],lhs[2][3]+rhs[2][3],lhs[3][3]+rhs[3][3]
        };
    }

    Matrix4 operator-(Matrix4 const& lhs,Matrix4 const& rhs) {
    	return {
                lhs[0][0]-rhs[0][0],lhs[1][0]-rhs[1][0],lhs[2][0]-rhs[2][0],lhs[3][0]-rhs[3][0],
                lhs[0][1]-rhs[0][1],lhs[1][1]-rhs[1][1],lhs[2][1]-rhs[2][1],lhs[3][1]-rhs[3][1],
                lhs[0][2]-rhs[0][2],lhs[1][2]-rhs[1][2],lhs[2][2]-rhs[2][2],lhs[3][2]-rhs[3][2],
                lhs[0][3]-rhs[0][3],lhs[1][3]-rhs[1][3],lhs[2][3]-rhs[2][3],lhs[3][3]-rhs[3][3]
            };
    }
    Matrix4 operator-(Matrix4 const& lhs) {
        return {
            -lhs[0][0],-lhs[1][0],-lhs[2][0],-lhs[3][0],
            -lhs[0][1],-lhs[1][1],-lhs[2][1],-lhs[3][1],
            -lhs[0][2],-lhs[1][2],-lhs[2][2],-lhs[3][2],
            -lhs[0][3],-lhs[1][3],-lhs[2][3],-lhs[3][3]
        };
    }

    Matrix4 operator*(Matrix4 const& lhs,Matrix4 const& rhs) {
        return {
                lhs[0][0]*rhs[0][0]+lhs[1][0]*rhs[0][1]+lhs[2][0]*rhs[0][2]+lhs[3][0]*rhs[0][3],
                lhs[0][0]*rhs[1][0]+lhs[1][0]*rhs[1][1]+lhs[2][0]*rhs[1][2]+lhs[3][0]*rhs[1][3],
                lhs[0][0]*rhs[2][0]+lhs[1][0]*rhs[2][1]+lhs[2][0]*rhs[2][2]+lhs[3][0]*rhs[2][3],
                lhs[0][0]*rhs[3][0]+lhs[1][0]*rhs[3][1]+lhs[2][0]*rhs[3][2]+lhs[3][0]*rhs[3][3],

                lhs[0][1]*rhs[0][0]+lhs[1][1]*rhs[0][1]+lhs[2][1]*rhs[0][2]+lhs[3][1]*rhs[0][3],
                lhs[0][1]*rhs[1][0]+lhs[1][1]*rhs[1][1]+lhs[2][1]*rhs[1][2]+lhs[3][1]*rhs[1][3],
                lhs[0][1]*rhs[2][0]+lhs[1][1]*rhs[2][1]+lhs[2][1]*rhs[2][2]+lhs[3][1]*rhs[2][3],
                lhs[0][1]*rhs[3][0]+lhs[1][1]*rhs[3][1]+lhs[2][1]*rhs[3][2]+lhs[3][1]*rhs[3][3],

                lhs[0][2]*rhs[0][0]+lhs[1][2]*rhs[0][1]+lhs[2][2]*rhs[0][2]+lhs[3][2]*rhs[0][3],
                lhs[0][2]*rhs[1][0]+lhs[1][2]*rhs[1][1]+lhs[2][2]*rhs[1][2]+lhs[3][2]*rhs[1][3],
                lhs[0][2]*rhs[2][0]+lhs[1][2]*rhs[2][1]+lhs[2][2]*rhs[2][2]+lhs[3][2]*rhs[2][3],
                lhs[0][2]*rhs[3][0]+lhs[1][2]*rhs[3][1]+lhs[2][2]*rhs[3][2]+lhs[3][2]*rhs[3][3],

                lhs[0][3]*rhs[0][0]+lhs[1][3]*rhs[0][1]+lhs[2][3]*rhs[0][2]+lhs[3][3]*rhs[0][3],
                lhs[0][3]*rhs[1][0]+lhs[1][3]*rhs[1][1]+lhs[2][3]*rhs[1][2]+lhs[3][3]*rhs[1][3],
                lhs[0][3]*rhs[2][0]+lhs[1][3]*rhs[2][1]+lhs[2][3]*rhs[2][2]+lhs[3][3]*rhs[2][3],
                lhs[0][3]*rhs[3][0]+lhs[1][3]*rhs[3][1]+lhs[2][3]*rhs[3][2]+lhs[3][3]*rhs[3][3]
            };
    }

    Matrix4 operator*(Matrix4 const& lhs,float const& scalar) {
        return {
                lhs[0][0]*scalar, lhs[1][0]*scalar, lhs[2][0]*scalar, lhs[3][0]*scalar,
                lhs[0][1]*scalar, lhs[1][1]*scalar, lhs[2][1]*scalar, lhs[3][1]*scalar,
                lhs[0][2]*scalar, lhs[1][2]*scalar, lhs[2][2]*scalar, lhs[3][2]*scalar,
                lhs[0][3]*scalar, lhs[1][3]*scalar, lhs[2][3]*scalar, lhs[3][3]*scalar
        	};
    }

    Matrix4 operator/(Matrix4 const& lhs,float const& scalar) {
        return {
                lhs[0][0]/scalar, lhs[1][0]/scalar, lhs[2][0]/scalar, lhs[3][0]/scalar,
                lhs[0][1]/scalar, lhs[1][1]/scalar, lhs[2][1]/scalar, lhs[3][1]/scalar,
                lhs[0][2]/scalar, lhs[1][2]/scalar, lhs[2][2]/scalar, lhs[3][2]/scalar,
                lhs[0][3]/scalar, lhs[1][3]/scalar, lhs[2][3]/scalar, lhs[3][3]/scalar
        	};
    }
}
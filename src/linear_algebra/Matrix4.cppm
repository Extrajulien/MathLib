module;

#include <array>
#include <cmath>
#include <iostream>

export module MathLib:Matrix4;
//import :internal;
namespace math {
    //for SIMD optimization matrix4 only supports float
    // column major matrix 4x4 float struct
    export struct alignas(64) Matrix4 {
        friend constexpr Matrix4 operator-(Matrix4 const& lhs,Matrix4 const& rhs);
        friend constexpr Matrix4 operator-(Matrix4 const& lhs);
        friend constexpr Matrix4 operator+(Matrix4 const& lhs,Matrix4 const& rhs);
        friend constexpr Matrix4 operator*(Matrix4 const& lhs,Matrix4 const& rhs);
        friend constexpr Matrix4 operator*(Matrix4 const& lhs,float const& scalar);
        friend constexpr Matrix4 operator/(Matrix4 const& lhs,float const& scalar);
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

        constexpr Matrix4(const std::array<float, 16>& array) : matrix({
            {array[0], array[4], array[8],  array[12]},
            {array[1], array[5], array[9],  array[13]},
            {array[2], array[6], array[10], array[14]},
            {array[3], array[7], array[11], array[15]}
        }) {}

        constexpr Column& operator[](const size_t row) {
            return matrix[row];
        }
        constexpr const Column& operator[](const size_t row) const {
            return matrix[row];
        }


        constexpr Matrix4& operator+=(Matrix4 const& other) {
            *this = *this + other;
            return *this;
        }

        constexpr Matrix4& operator-=(Matrix4 const& other) {
            *this = *this - other;
            return *this;
        }

        constexpr Matrix4& operator*=(const Matrix4 &other) {
            *this = *this * other;
            return *this;
        }

        constexpr Matrix4& operator*=(float const& scalar) {
            *this = *this * scalar;
            return *this;
        }

        constexpr Matrix4& operator/=(float const& scalar) {
            *this = *this / scalar;
            return *this;
        }

        [[nodiscard]] constexpr std::array<float, 16> toArray() const {
            return {
                matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
                matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
                matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
                matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]
            };
        }

        [[nodiscard]] const float* toPtr() const {
            return matrix[0].rowEntry.data();
        }

        constexpr bool operator==(const Matrix4& rhs) const {
            return matrix[0][0] == rhs[0][0] && matrix[0][1] == rhs[0][1]
            && matrix[0][2] == rhs[0][2] && matrix[0][3] == rhs[0][3]
            && matrix[1][0] == rhs[1][0] && matrix[1][1] == rhs[1][1]
            && matrix[1][2] == rhs[1][2] && matrix[1][3] == rhs[1][3]
            && matrix[2][0] == rhs[2][0] && matrix[2][1] == rhs[2][1]
            && matrix[2][2] == rhs[2][2] && matrix[2][3] == rhs[2][3]
            && matrix[3][0] == rhs[3][0] && matrix[3][1] == rhs[3][1]
            && matrix[3][2] == rhs[3][2] && matrix[3][3] == rhs[3][3];
        }

        constexpr bool operator!=(const Matrix4& rhs) const {
            return !(*this == rhs);
        }

        constexpr Matrix4& operator= (const Matrix4 &other) {
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

    export using Matrix4f = Matrix4;

    export constexpr Matrix4 operator+(Matrix4 const& lhs,Matrix4 const& rhs) {
        return {
            lhs[0][0]+rhs[0][0],lhs[1][0]+rhs[1][0],lhs[2][0]+rhs[2][0],lhs[3][0]+rhs[3][0],
            lhs[0][1]+rhs[0][1],lhs[1][1]+rhs[1][1],lhs[2][1]+rhs[2][1],lhs[3][1]+rhs[3][1],
            lhs[0][2]+rhs[0][2],lhs[1][2]+rhs[1][2],lhs[2][2]+rhs[2][2],lhs[3][2]+rhs[3][2],
            lhs[0][3]+rhs[0][3],lhs[1][3]+rhs[1][3],lhs[2][3]+rhs[2][3],lhs[3][3]+rhs[3][3]
        };
    }

    export constexpr Matrix4 operator-(Matrix4 const& lhs,Matrix4 const& rhs) {
    	return {
    	    lhs[0][0]-rhs[0][0],lhs[1][0]-rhs[1][0],lhs[2][0]-rhs[2][0],lhs[3][0]-rhs[3][0],
    	    lhs[0][1]-rhs[0][1],lhs[1][1]-rhs[1][1],lhs[2][1]-rhs[2][1],lhs[3][1]-rhs[3][1],
    	    lhs[0][2]-rhs[0][2],lhs[1][2]-rhs[1][2],lhs[2][2]-rhs[2][2],lhs[3][2]-rhs[3][2],
    	    lhs[0][3]-rhs[0][3],lhs[1][3]-rhs[1][3],lhs[2][3]-rhs[2][3],lhs[3][3]-rhs[3][3]
    	};
    }
    export constexpr Matrix4 operator-(Matrix4 const& lhs) {
        return {
            -lhs[0][0],-lhs[1][0],-lhs[2][0],-lhs[3][0],
            -lhs[0][1],-lhs[1][1],-lhs[2][1],-lhs[3][1],
            -lhs[0][2],-lhs[1][2],-lhs[2][2],-lhs[3][2],
            -lhs[0][3],-lhs[1][3],-lhs[2][3],-lhs[3][3]
        };
    }

    export constexpr Matrix4 operator*(Matrix4 const& lhs,Matrix4 const& rhs) {
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

    export constexpr Matrix4 operator*(Matrix4 const& lhs,float const& scalar) {
        return {
            lhs[0][0]*scalar, lhs[1][0]*scalar, lhs[2][0]*scalar, lhs[3][0]*scalar,
            lhs[0][1]*scalar, lhs[1][1]*scalar, lhs[2][1]*scalar, lhs[3][1]*scalar,
            lhs[0][2]*scalar, lhs[1][2]*scalar, lhs[2][2]*scalar, lhs[3][2]*scalar,
            lhs[0][3]*scalar, lhs[1][3]*scalar, lhs[2][3]*scalar, lhs[3][3]*scalar
        };
    }

    export constexpr Matrix4 operator/(Matrix4 const& lhs,float const& scalar) {
        if (scalar == 0) [[unlikely]] { throw std::invalid_argument("cannot divide Matrix4 components by 0."); }
        return {
            lhs[0][0]/scalar, lhs[1][0]/scalar, lhs[2][0]/scalar, lhs[3][0]/scalar,
            lhs[0][1]/scalar, lhs[1][1]/scalar, lhs[2][1]/scalar, lhs[3][1]/scalar,
            lhs[0][2]/scalar, lhs[1][2]/scalar, lhs[2][2]/scalar, lhs[3][2]/scalar,
            lhs[0][3]/scalar, lhs[1][3]/scalar, lhs[2][3]/scalar, lhs[3][3]/scalar
        };
    }
}
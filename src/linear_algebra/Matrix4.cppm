module;

#include <array>
#include <cmath>
#include <iostream>

export module MathLib:Matrix4;
namespace MathLib {
    /**
     * @brief Represents a 4x4 matrix of single-precision floating-point values.
     *
     * Matrix4 is optimized for common linear algebra and graphics use cases.
     * Elements are stored internally in column-major order, while constructors
     * accept values in row-major order for readability.
     *
     * The type is 64-byte aligned to support efficient SIMD-friendly storage.
     */
    export struct alignas(64) Matrix4 {
        friend constexpr Matrix4 operator-(Matrix4 const& lhs,Matrix4 const& rhs) noexcept;
        friend constexpr Matrix4 operator-(Matrix4 const& lhs) noexcept;
        friend constexpr Matrix4 operator+(Matrix4 const& lhs,Matrix4 const& rhs) noexcept;
        friend constexpr Matrix4 operator*(Matrix4 const& lhs,Matrix4 const& rhs) noexcept;
        friend constexpr Matrix4 operator*(Matrix4 const& lhs,float const& scalar) noexcept;
        friend constexpr Matrix4 operator/(Matrix4 const& lhs,float const& scalar);
    private:
        struct alignas(16) Column {
            std::array<float, 4> rowEntry;

            /**
             * @brief Accesses a row of a column by index.
             * @param row The row index (0-3).
             * @return A reference to the specified float.
             */
            constexpr float& operator[](const std::size_t row) noexcept {
                return rowEntry[row];
            }

            /**
             * @brief Accesses a row of a column by index.
             * @param row The row index (0-3).
             * @return A const reference to the specified float.
             */
            constexpr const float& operator[](const std::size_t row) const noexcept {
                return rowEntry[row];
            }
        };
        std::array<Column,4> matrix;
    public:
        /**
         * @brief Constructs a matrix from 16 scalars in row-major order.
         */
        constexpr Matrix4(const float n00, const float n01, const float n02, const float n03,
            const float n10, const float n11, const float n12, const float n13,
            const float n20, const float n21, const float n22, const float n23,
            const float n30, const float n31, const float n32, const float n33) noexcept : matrix({
                std::array<float, 4>({n00, n10, n20, n30}),
                std::array<float, 4>({n01, n11, n21, n31}),
                std::array<float, 4>({n02, n12, n22, n32}),
                std::array<float, 4>({n03, n13, n23, n33})
            }) {}

        /**
         * @brief Constructs a matrix from a flat 16-element row-major array.
         * @param array A 16-element array containing matrix values in row-major order.
         */
        constexpr Matrix4(const std::array<float, 16>& array) noexcept : matrix({
            {array[0], array[4], array[8],  array[12]},
            {array[1], array[5], array[9],  array[13]},
            {array[2], array[6], array[10], array[14]},
            {array[3], array[7], array[11], array[15]}
        }) {}

        /**
         * @brief Accesses a column of the matrix by index.
         * @param col The column index (0-3).
         * @return A reference to the specified column.
         */
        constexpr Column& operator[](const size_t col) noexcept {
            return matrix[col];
        }
        /**
         * @brief Accesses a column of the matrix by index.
         * @param col The column index (0-3).
         * @return A const reference to the specified column.
         */
        constexpr const Column& operator[](const size_t col) const noexcept {
            return matrix[col];
        }


        /**
         * @brief Adds another matrix to this matrix.
         * @return A reference to this matrix.
         */
        constexpr Matrix4& operator+=(Matrix4 const& other) noexcept {
            *this = *this + other;
            return *this;
        }

        /**
         * @brief Subtracts another matrix from this matrix.
         * @return A reference to this matrix.
         */
        constexpr Matrix4& operator-=(Matrix4 const& other) noexcept {
            *this = *this - other;
            return *this;
        }

        /**
         * @brief Multiplies this matrix by another matrix.
         * @return A reference to this matrix.
         */
        constexpr Matrix4& operator*=(const Matrix4 &other) noexcept {
            *this = *this * other;
            return *this;
        }

        /**
         * @brief Multiplies this matrix by a scalar.
         * @return A reference to this matrix.
         */
        constexpr Matrix4& operator*=(float const& scalar) noexcept {
            *this = *this * scalar;
            return *this;
        }

        /**
         * @brief Divides this matrix by a scalar.
         * @return A reference to this matrix.
         */
        constexpr Matrix4& operator/=(float const& scalar) {
            *this = *this / scalar;
            return *this;
        }

        /**
         * @brief Serializes the matrix into a flat, row-major array.
         * @warning The returned array is <b>row-major</b>,
         *  making it inadequate to use in column-major graphics APIs.
         * @return A contiguous 16-element <code>std::array<float, 16></code>.
         */
        [[nodiscard]] constexpr std::array<float, 16> toArray() const noexcept {
            return {
                matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0],
                matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1],
                matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2],
                matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]
            };
        }

        /**
         * @brief Exposes the internal matrix storage as a contiguous raw float array.
         * This accessor maps the memory-aligned columns directly into a flat array layout
         * with O(1) complexity. Ideal for passing data directly to graphics APIs.
         * @warning The returned pointer points to the internal memory managed by this matrix object.
         * It becomes a dangling pointer if the matrix instance is destroyed.
         * @return A <code>const float*</code> pointing to the first element of the contiguous 16-float array.
         */
        [[nodiscard]] const float* toPtr() const noexcept {
            return matrix[0].rowEntry.data();
        }

        /**
         * @brief Checks if two matrices are equal.
         */
        constexpr bool operator==(const Matrix4& rhs) const noexcept {
            return matrix[0][0] == rhs[0][0] && matrix[0][1] == rhs[0][1]
            && matrix[0][2] == rhs[0][2] && matrix[0][3] == rhs[0][3]
            && matrix[1][0] == rhs[1][0] && matrix[1][1] == rhs[1][1]
            && matrix[1][2] == rhs[1][2] && matrix[1][3] == rhs[1][3]
            && matrix[2][0] == rhs[2][0] && matrix[2][1] == rhs[2][1]
            && matrix[2][2] == rhs[2][2] && matrix[2][3] == rhs[2][3]
            && matrix[3][0] == rhs[3][0] && matrix[3][1] == rhs[3][1]
            && matrix[3][2] == rhs[3][2] && matrix[3][3] == rhs[3][3];
        }

        /**
         * @brief Checks if two matrices are not equal.
         */
        constexpr bool operator!=(const Matrix4& rhs) const noexcept {
            return !(*this == rhs);
        }

        /**
         * @brief Assigns another matrix to this matrix.
         * @return A reference to this matrix.
         */
        constexpr Matrix4& operator= (const Matrix4 &other) noexcept {
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

    /**
     * @brief Performs component-wise addition of two matrices.
     */
    export constexpr Matrix4 operator+(Matrix4 const& lhs,Matrix4 const& rhs) noexcept {
        return {
            lhs[0][0]+rhs[0][0],lhs[1][0]+rhs[1][0],lhs[2][0]+rhs[2][0],lhs[3][0]+rhs[3][0],
            lhs[0][1]+rhs[0][1],lhs[1][1]+rhs[1][1],lhs[2][1]+rhs[2][1],lhs[3][1]+rhs[3][1],
            lhs[0][2]+rhs[0][2],lhs[1][2]+rhs[1][2],lhs[2][2]+rhs[2][2],lhs[3][2]+rhs[3][2],
            lhs[0][3]+rhs[0][3],lhs[1][3]+rhs[1][3],lhs[2][3]+rhs[2][3],lhs[3][3]+rhs[3][3]
        };
    }

    /**
     * @brief Performs component-wise subtraction of two matrices.
     */
    export constexpr Matrix4 operator-(Matrix4 const& lhs,Matrix4 const& rhs) noexcept {
    	return {
    	    lhs[0][0]-rhs[0][0],lhs[1][0]-rhs[1][0],lhs[2][0]-rhs[2][0],lhs[3][0]-rhs[3][0],
    	    lhs[0][1]-rhs[0][1],lhs[1][1]-rhs[1][1],lhs[2][1]-rhs[2][1],lhs[3][1]-rhs[3][1],
    	    lhs[0][2]-rhs[0][2],lhs[1][2]-rhs[1][2],lhs[2][2]-rhs[2][2],lhs[3][2]-rhs[3][2],
    	    lhs[0][3]-rhs[0][3],lhs[1][3]-rhs[1][3],lhs[2][3]-rhs[2][3],lhs[3][3]-rhs[3][3]
    	};
    }
    /**
     * @brief Negates all components of the matrix.
     */
    export constexpr Matrix4 operator-(Matrix4 const& lhs) noexcept {
        return {
            -lhs[0][0],-lhs[1][0],-lhs[2][0],-lhs[3][0],
            -lhs[0][1],-lhs[1][1],-lhs[2][1],-lhs[3][1],
            -lhs[0][2],-lhs[1][2],-lhs[2][2],-lhs[3][2],
            -lhs[0][3],-lhs[1][3],-lhs[2][3],-lhs[3][3]
        };
    }

    /**
     * @brief Performs matrix multiplication.
     */
    export constexpr Matrix4 operator*(Matrix4 const& lhs,Matrix4 const& rhs) noexcept {
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

    /**
     * @brief Multiplies a matrix by a scalar.
     */
    export constexpr Matrix4 operator*(Matrix4 const& lhs,float const& scalar) noexcept {
        return {
            lhs[0][0]*scalar, lhs[1][0]*scalar, lhs[2][0]*scalar, lhs[3][0]*scalar,
            lhs[0][1]*scalar, lhs[1][1]*scalar, lhs[2][1]*scalar, lhs[3][1]*scalar,
            lhs[0][2]*scalar, lhs[1][2]*scalar, lhs[2][2]*scalar, lhs[3][2]*scalar,
            lhs[0][3]*scalar, lhs[1][3]*scalar, lhs[2][3]*scalar, lhs[3][3]*scalar
        };
    }

    /**
     * @brief Divides a matrix by a scalar.
     * @throws std::invalid_argument if the scalar is zero.
     */
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
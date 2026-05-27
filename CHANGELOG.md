## [1.0.0]

### 🚀 Features

- Create Vector2 and related operations
- Create Vector3 and related operations
- Create Vector4 and related operations
- Create Matrix4 and related operations
- Create LinearFactory for factory operations in linear_algebra
- Add Angle struct for trigonometric calculations
- Add multiplication and division operators for Angle, handle division by zero
- Add constexpr tangent and rotation matrix functions, extend MathLib factory

### 🐛 Bug Fixes

- Export unary minus operator for Vector2 and also specify return types
- Export Matrix4 related functions and make them constexpr
- Make toArray() return a row-major array instead of a column-major array
- Make a constexpr atan2 function to use in case the library is not constexpr

### 🚜 Refactor

- Extract arithmetic type concept
- Make operator!= independent of operator== for Vector2
- Remove unnecessary `static` from exported Matrix4 factory functions
- Simplify doctest includes

### 📚 Documentation

- Generate initial changelog up to Matrix4 implementation
- Add comprehensive documentation comments for Matrix4 and related operators

### ⚡ Performance

- Add `noexcept` to non-throwing MathLib functions

### 🧪 Testing

- Add Vector2 tests
- Add Vector3 tests
- Add Vector4 tests
- Add Matrix4 unit tests
- Add Angle tests
- Add tests for linear_math factory functions

### ⚙️ Miscellaneous Tasks

- Setup test framework
- Add compiler hints to unlikely division checks
- Extract library and test-specific cmake to separate CMakeLists
- Add cliff configuration file

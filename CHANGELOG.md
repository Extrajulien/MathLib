## [unreleased]

### 🚀 Features

- Create Vector2 and related operations
- Create Vector3 and related operations
- Create Vector4 and related operations
- Create Matrix4 and related operations
- Create LinearFactory module for factory operations in linear_algebra

### 🐛 Bug Fixes

- Export unary minus operator for Vector2 and also specify return types
- Export Matrix4 related functions and make them constexpr

### 🚜 Refactor

- Extract arithmetic type concept
- Make operator!= independent of operator== for Vector2
- Remove unnecessary `static` from exported Matrix4 factory functions

### 🧪 Testing

- Add Vector2 tests
- Add Vector3 tests
- Add Vector4 tests
- Add Matrix4 unit tests

### ⚙️ Miscellaneous Tasks

- Setup test framework
- Add compiler hints to unlikely division checks
- Extract library and test-specific cmake to separate CMakeLists
- Add cliff configuration file

# MathLib

[![C++26](https://img.shields.io/badge/C%2B%2B-26-blue.svg)](https://en.cppreference.com/w/cpp/26)
[![Release](https://img.shields.io/badge/release-1.0.0-brightgreen.svg)](CHANGELOG.md)
[![CMake](https://img.shields.io/badge/CMake-3.30%2B-064F8C.svg?logo=cmake)](https://cmake.org/)

A modern, lightweight C++26 math library built around **named modules** with **near-full `constexpr` support**, designed for use in graphics, simulation, games, and any project that needs fast and expressive math primitives.

> 🎉 **Version 1.0.0** is out! See the [Changelog](CHANGELOG.md) for the full release notes.

---

## ✨ Features

- 🧩 **C++26 Modules** — clean imports, no header bloat, faster builds.
- ⚡ **Near-full `constexpr`** — most operations are usable at compile time, including a `constexpr` `atan2` fallback for when the underlying standard library functions aren't.
- 🚀 **`noexcept` correctness** — non-throwing operations are explicitly marked for better optimization.
- 🧮 **Linear Algebra**
    - `Vector2`, `Vector3`, `Vector4` with full arithmetic, comparison, and unary operators
    - `Matrix4` with row-major `toArray()`, factory helpers, and rotation matrices
    - `LinearFactory` for building common linear-algebra objects
- 📐 **Trigonometry**
    - `Angle` struct for safe, unit-aware trigonometric calculations
    - Arithmetic operators with safe handling of division by zero
    - `constexpr` tangent and rotation utilities
- 🧪 **Thoroughly tested** — every public API is covered by unit tests (doctest).

---

## 📦 Requirements

- A compiler with **C++26 modules** support (e.g. recent Clang or GCC)
- **CMake ≥ 3.30**
- A build system that supports `CMAKE_CXX_SCAN_FOR_MODULES` (e.g. Ninja ≥ 1.11)

---

## 🔧 Building

```bash
git clone https://github.com/Extrajulien/MathLib.git
cd MathLib
cmake -S . -B build -G Ninja
cmake --build build
```

### Build options

| Option           | Default | Description                  |
|------------------|---------|------------------------------|
| `BUILD_TESTING`  | `ON`    | Build the unit test suite    |

To build **without** tests:

```bash
cmake -S . -B build -G Ninja -DBUILD_TESTING=OFF
```

### Running the tests

```bash
cmake --build build --target Tests
ctest --test-dir build
```

---

## 🚀 Usage

MathLib exposes a single top-level module: `MathLib`. Importing it gives you access to every submodule.

```cpp
import MathLib;

int main() {
    constexpr Vector3 a{1.0f, 2.0f, 3.0f};
    constexpr Vector3 b{4.0f, 5.0f, 6.0f};
    constexpr auto sum = a + b;  // computed at compile time

    constexpr Angle theta = Angle::fromDegrees(45.0f);
    constexpr auto rotation = LinearFactory::rotationMatrix(theta);

    return 0;
}
```

### Linking with CMake

```cmake
add_subdirectory(MathLib)
target_link_libraries(your_target PRIVATE MathLib)
```

Make sure your consuming target also uses C++26 and enables module scanning:

```cmake
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
```

---

## 📚 Module Overview

| Module                    | Contents                                                |
|---------------------------|---------------------------------------------------------|
| `MathLib:Vector2`         | 2D vector type and operations                           |
| `MathLib:Vector3`         | 3D vector type and operations                           |
| `MathLib:Vector4`         | 4D vector type and operations                           |
| `MathLib:Matrix4`         | 4×4 matrix type, row-major export, and operations       |
| `MathLib:LinearFactory`   | Factory helpers for linear algebra constructs           |
| `MathLib:Angle`           | Angle abstraction with safe trigonometric operations    |

You can also `import MathLib;` to get everything at once.

---

## 📝 Changelog

See [CHANGELOG.md](CHANGELOG.md) for a detailed list of changes per release.
The changelog is generated using [git-cliff](https://github.com/orhun/git-cliff) (configured via `cliff.toml`).

---

## 🤝 Contributing

Issues and pull requests are welcome! If you're contributing code:

1. Make sure your changes preserve the `constexpr` and `noexcept` guarantees where applicable.
2. Add or update tests under `tests/` for any new behavior.
3. Run the test suite locally before submitting.

---

## 📄 License

This project is released under the MIT License. See `LICENSE` for details.
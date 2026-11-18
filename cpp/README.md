# YatricesEngine (C++ library)

The C++ core of Yatrices. Pure C++17, and it knows nothing about Python.

## Building

YatricesEngine isn't on [vcpkg](https://vcpkg.io/) yet; I'll publish it there later. For now, build it from source:

From the project folder:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

On Windows with MSYS2, make sure CMake is using `C:/msys64/ucrt64/bin/g++.exe`. If the build folder was configured with another compiler, delete `build/` and run:

```bash
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe
```

Useful options, all `ON` or `OFF`:

| Option | Default | Builds |
|---|---|---|
| `YATRICES_BUILD_TESTS` | `ON` | the C++ tests |
| `YATRICES_BUILD_EXAMPLES` | `ON` | the example programs |
| `YATRICES_BUILD_PYTHON` | `OFF` | the Python module (pip turns this on for you) |
| `YATRICES_BUILD_CAPI` | `OFF` | the plain C interface |

## Using it

One include gives you the whole engine:

```cpp
#include <iostream>
#include "yatrices/yatrices.hpp"

using namespace yatrices;

int main() {
    Vecd x{1, 2, 3};
    Vecd y{4, 5, 6};

    std::cout << x + y << "\n";        // [5, 7, 9]
    std::cout << 2 * x << "\n";        // [2, 4, 6]
    std::cout << x.dot(y) << "\n";     // 32
    std::cout << cross(x, y) << "\n";  // [-3, 6, -3]

    try {
        Vecd bad = x + Vecd{1, 2};     // sizes 3 and 2
    } catch (const SizeMismatchError& e) {
        std::cout << e.what() << "\n"; // says what went wrong and how to fix it
    }
}
```

To use it in your own CMake project, put this folder next to yours and add:

```cmake
add_subdirectory(path/to/yatrices yatrices)
target_link_libraries(your_program PRIVATE Yatrices::Engine)
```

For a quick test without CMake, compile the engine's `.cpp` files along with your own:

```bash
g++ -std=c++17 -O2 -Icpp/include my_program.cpp cpp/src/vector.cpp cpp/src/errors.cpp -o my_program
```

## Everything available

**`Vec<T>`**, for `T` = `float` (`Vecf`) or `double` (`Vecd`):

| Group | What's there |
|---|---|
| Construction | `Vecd(n)` zeros, `Vecd(n, value)`, `Vecd(n, uninitialised)` (not zeroed, for speed), `Vecd{1, 2, 3}` |
| Access | `v[i]` (unchecked, fast), `v.at(i)` (checked), `size()`, `empty()`, `data()`, `begin()`, `end()` |
| In place | `fill`, `scale`, `axpy`, `+=`, `-=`, `*=`, `/=` |
| Arithmetic | `x + y`, `x - y`, `-x`, `2 * x`, `x * 2`, `x / 2` |
| Products | `x.dot(y)`, `hadamard(x, y)`, `cross(x, y)` (size 3 only) |
| Norms | `norm1()`, `norm2()` (overflow-safe), `norm_inf()` |
| Reductions | `sum()`, `mean()`, `min()`, `max()` |
| Geometry | `angle(x, y)`, `project(v, onto)` |
| Floats | `approx_equal(a, b)`, `allclose(x, y)`, `default_tolerance<T>()`, `all_finite()` |
| Printing | `to_string(v)`, `std::cout << v` |
| Since 0.0.2 | `ones`, `unit`, `linspace`, `apply`, `argmin`, `argmax`, `prod`, `variance`, `stddev`, `normalise`, `normalised`, `count_nonzero`, `has_nan`, `==`, `!=`, and the free functions `lincomb`, `lerp`, `distance`, `distance1`, `distance_inf`, `cosine_similarity` |

**`Matrix<T>`** (new in 0.0.2), for `T` = `float` (`Matrixf`) or `double` (`Matrixd`), stored row by row:

| Group | What's there |
|---|---|
| Construction | zeros, a constant, uninitialised, `Matrixd{{1, 2}, {3, 4}}` |
| Access | `m(i, j)` (unchecked), `m.at(i, j)` (checked), `data()`, `row_data(i)`, `begin()`, `end()` |
| Shape | `rows`, `cols`, `size`, `empty`, `is_square`, `reshape`, `flatten` |
| Arithmetic | `+`, `-`, unary `-`, `*` and `/` by a scalar, the `+=` family, `fill`, `scale`, `apply`, `hadamard` |
| Structure | `transposed()`, `transpose_in_place()`, `trace`, `diagonal(k)`, `set_diagonal(d, k)` |
| Row operations | `swap_rows`, `scale_row`, `add_row_multiple` |
| Reductions | `sum`, `mean`, `min`, `max`, `max_abs`, `all_finite`, `sum_rows`, `sum_cols`, `mean_rows`, `mean_cols` |
| Broadcasting | `add_to_rows`, `subtract_from_rows`, `centre_columns` |
| Floats and printing | `allclose`, `to_string`, `std::cout << m` |

**Errors**, in `yatrices/errors.hpp`. Each one derives from the standard exception it refines, so catching `std::invalid_argument` still works:

| Error | Derives from | Thrown when |
|---|---|---|
| `IndexError` | `std::out_of_range` | `at(i)` past the end |
| `SizeMismatchError` | `std::invalid_argument` | two sizes that should match don't |
| `EmptyVectorError` | `std::invalid_argument` | `mean`, `min`, `max` of an empty vector |
| `WrongSizeError` | `std::invalid_argument` | `cross` on vectors that aren't of size 3 |
| `ZeroVectorError` | `std::invalid_argument` | `angle` or `project` with a vector of zeros |
| `AllocationSizeError` | `std::length_error` | a size whose byte count can't even be represented |
| `OutOfMemoryError` | `std::bad_alloc` | the computer ran out of memory |
| `ShapeMismatchError` | `std::invalid_argument` | two matrix shapes that should match don't |
| `NotSquareError` | `std::invalid_argument` | a square matrix was needed (`trace`, ...) |
| `RaggedRowsError` | `std::invalid_argument` | a nested list whose rows have different lengths |
| `EmptyMatrixError` | `std::invalid_argument` | a reduction on an empty matrix |

## Layout

```
cpp/
├── include/yatrices/   public headers (yatrices.hpp includes all of them)
├── src/                implementation, one .cpp per header
├── tests/              C++ tests, run with ctest
└── examples/           small programs that use the library
```

The examples are the best place to start: [`examples/vector_ops.cpp`](examples/vector_ops.cpp) goes through everything a `Vec` can do, [`examples/matrix_ops.cpp`](examples/matrix_ops.cpp) does the same for `Matrix`, and [`examples/float_sum.cpp`](examples/float_sum.cpp) shows why adding up floats goes wrong.

# Changelog

All notable changes to Yatrices are recorded in this file, with the most recent first.

The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and version numbers follow [Semantic Versioning](https://semver.org/). Whilst the version number begins with `0.`, any part of the library may still change between releases.

## [Unreleased]

## [0.0.2] - 2026-11-18

Matrices arrive in the C++ engine, the C++ tests become real, and the source code loses its comments. Python bindings for `Matrix` come in a later release.

### Added

**YatricesEngine (C++17)**
- `Matrix<T>` for `float` and `double`, with the aliases `Matrixf` and `Matrixd`. It follows the design of `Vec`.
  - Row-major storage in a single buffer aligned on a 64-byte boundary; each object occupies 24 bytes (a pointer, the number of rows and the number of columns).
  - Constructors for zeros, a constant, uninitialised memory and a nested list such as `{{1, 2}, {3, 4}}`.
  - A complete rule of five; a copy reuses the existing buffer whenever the element count matches.
  - Element access through `(i, j)` (unchecked) and `at(i, j)` (checked), along with `data()`, `row_data(i)`, `begin()` and `end()`.
  - Shape: `rows`, `cols`, `size`, `empty`, `is_square`, `reshape` and `flatten`.
  - Arithmetic, entry by entry: `+`, `-`, unary `-`, `*` and `/` by a scalar, with `+=`, `-=`, `*=` and `/=`; temporaries are reused, as with `Vec`. Also `fill`, `scale`, `apply` and `hadamard`.
  - `transposed()` (cache-blocked, about 5 to 8 times faster than the plain double loop on 1024 x 1024 and 2048 x 2048 matrices) and `transpose_in_place()`.
  - `trace`, `diagonal(k)` and `set_diagonal(d, k)`.
  - The three elementary row operations: `swap_rows`, `scale_row` and `add_row_multiple`.
  - Reductions: `sum`, `mean`, `min`, `max`, `max_abs`, `all_finite`, `sum_rows`, `sum_cols`, `mean_rows` and `mean_cols`. `sum_cols` adds whole rows rather than walking down columns, which makes it about 20 times faster on large matrices.
  - Broadcasting: `add_to_rows`, `subtract_from_rows` and `centre_columns`.
  - `allclose`, `to_string` (columns aligned) and `operator<<`.
- New `Vec` features: `ones`, `unit` and `linspace`; `apply`; `argmin` and `argmax`; `prod`, `variance` and `stddev`; `normalise` and `normalised`; `count_nonzero` and `has_nan`; the free functions `lincomb`, `lerp`, `distance`, `distance1`, `distance_inf` and `cosine_similarity`; exact `==` and `!=`.
- New error types: `ShapeMismatchError`, `NotSquareError`, `RaggedRowsError` and `EmptyMatrixError`. `IndexError` can now name the axis (row or column) and `AllocationSizeError` reports a matrix whose `rows * cols` overflows.
- An example program, `example_matrix_ops`, and the first real engine tests (230 checks) in `cpp/tests/test_main.cpp`.

### Changed
- The version number is now 0.0.2 everywhere it appears: `CMakeLists.txt`, `pyproject.toml`, `yatricespy.__version__` and the README badge.
- The main README now describes the state of 0.0.2 (Matrix in C++, real C++ tests, what is still missing) and announces the planned PyPI and vcpkg releases.
- `cpp/README.md` lists the `Matrix` type, the new `Vec` functions, the new errors and the `matrix_ops` example; `python/README.md` explains that yatricespy is not on PyPI yet.
- The aligned allocator and the fast loops moved from `vector.cpp` into the private header `cpp/src/internal.hpp`, so that `Vec` and `Matrix` share them.

### Removed
- Every comment in the source code: all `.cpp`, `.hpp` and `.h` files of YatricesEngine, the pybind11 bindings and the C API, plus the comments in the CMake files and the Python package, tests and examples. The code is meant to read on its own; explanations live in the READMEs. A copy of the commented files is kept on my computer, outside the repository. The stubs in `after/` keep their comments, since they are nothing but comments.

### Planned
- yatricespy will be published on [PyPI](https://pypi.org/), so that `pip install yatricespy` works without cloning the repository.
- YatricesEngine will be published on [vcpkg](https://vcpkg.io/), so that C++ projects can depend on it directly.

### Fixed
- `Vec::operator+=` and `Vec::operator-=` passed the same pointer twice to a kernel whose arguments are marked `__restrict__`. That is undefined behaviour, even though the results happened to be correct. They now use a dedicated in-place kernel.

## [0.0.1] - 2026-11-16

The first release. It provides a vector type in C++, which is also available from Python. Matrices are the next step; the order of work is set out in `ROADMAP.md`.

### Added

**YatricesEngine (C++17)**
- `Vec<T>` for `float` and `double`, with the aliases `Vecf` and `Vecd`.
  - Each object occupies 16 bytes (a pointer and a size), and its buffer is aligned on a 64-byte boundary.
  - Constructors for `n` zeros, `n` copies of a value, `n` uninitialised elements (via the `uninitialised` tag), and an initialiser list such as `{1.0, 2.0, 3.0}`.
  - A complete rule of five: copies reuse the existing buffer where the sizes match, and moves never copy.
  - Element access through `[]` (unchecked) and `at()` (checked).
- Arithmetic: `+`, `-`, unary `-`, and `*` and `/` by a scalar, together with `+=`, `-=`, `*=` and `/=`.
  - Temporaries are reused, so that `x + y + z` allocates memory only once.
  - `2 * v` is accepted even when `v` holds doubles.
- Kernels: `fill`, `scale`, `axpy` and `dot`.
- Products: `hadamard`, and `cross` for three-dimensional vectors only.
- Norms: `norm1`, `norm2` (protected against overflow) and `norm_inf`.
- Reductions: `sum`, `mean`, `min` and `max`.
- Geometry: `angle` and `project`.
- Floating-point helpers: `approx_equal`, `allclose`, `default_tolerance` and `all_finite`.
- Printing through `to_string` and `operator<<`. Values are rounded only on output, never in storage.
- Error types: `IndexError`, `SizeMismatchError`, `EmptyVectorError`, `WrongSizeError`, `ZeroVectorError`, `AllocationSizeError` and `OutOfMemoryError`.
- A single umbrella header, `yatrices/yatrices.hpp`, which includes the entire engine.
- Two example programs: `example_vector_ops` and `example_float_sum`.

**yatricespy (Python 3.10+)**
- Python bindings for the whole of the engine, built with scikit-build-core and pybind11.
  - `Vec` (double precision, also available as `Vecd`) and `Vecf` (single precision), each of which may be constructed from a size, from a size and a value, or from any sequence of numbers.
  - Element access by index, including negative indices, together with iteration, `len()`, `tolist()` and copying.
  - All arithmetic operators: `+`, `-`, unary `-` and `+`, multiplication and division by a scalar, and the in-place forms `+=`, `-=`, `*=` and `/=`, which modify the existing object rather than creating a new one.
  - The `@` operator as a shorthand for the dot product, in keeping with NumPy.
  - Every method of the C++ vector: `fill`, `scale`, `axpy`, `dot`, `norm1`, `norm2`, `norm_inf`, `sum`, `mean`, `min`, `max` and `all_finite`.
  - The free functions `hadamard`, `cross`, `angle`, `project`, `allclose` and `approx_equal`.
- Each engine error is raised in Python as an exception class of its own. Every class derives both from `YatricesError` and from the corresponding built-in exception (`IndexError`, `ValueError` or `MemoryError`), so that existing `except` clauses behave as expected.
- Type hints (`_yatricespy.pyi` and `py.typed`), enabling editors to offer autocompletion and to check calls.
- A suite of 25 tests, in which every numerical result is verified against NumPy, and an example script, `python/examples/vector_tour.py`.

**Project**
- A CMake build, with options to build only the parts required (tests, examples, Python module, C API).
- A README for each library, together with `TESTING.md` and `ROADMAP.md`, which sets out the next ten algorithms and the order in which they depend upon one another.
- The logo and three banners.
- The C++ tests and the C API are placeholders for the time being.

### Notes
- Verified on Windows (MSYS2 g++, Python 3.14) and on Linux (g++, Python 3.13).

[Unreleased]: https://github.com/yassinox12834/yatrices/compare/v0.0.2...HEAD
[0.0.2]: https://github.com/yassinox12834/yatrices/compare/v0.0.1...v0.0.2
[0.0.1]: https://github.com/yassinox12834/yatrices/releases/tag/v0.0.1

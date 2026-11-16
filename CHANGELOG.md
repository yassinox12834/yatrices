# Changelog

All notable changes to Yatrices are recorded in this file, with the most recent first.

The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and version numbers follow [Semantic Versioning](https://semver.org/). Whilst the version number begins with `0.`, any part of the library may still change between releases.

## [Unreleased]

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

[Unreleased]: https://github.com/yassinox12834/yatrices/compare/v0.0.1...HEAD
[0.0.1]: https://github.com/yassinox12834/yatrices/releases/tag/v0.0.1

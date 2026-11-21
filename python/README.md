# yatricespy (Python library)

The Python front end of YatricesEngine. Every function runs in the C++ engine in `../cpp`; Python only passes the call along.

## Installing

From the project folder:

```bash
python -m pip install ".[test]"
python -m pytest
```

pip compiles the engine, so CMake and a C++20 compiler must be on your PATH.

**On Windows**, MSYS2's g++ (the UCRT64 one, `C:\msys64\ucrt64\bin`) works with a normal python.org Python; this has been tested with Python 3.14. The build links the compiler's runtime into the module, because Python does not search `PATH` for the DLLs that a module needs. The free [Visual Studio Build Tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/) should work as well, but that route has not been tested.

Be careful which `python` you run: on a machine with MSYS2, typing `python` may start MSYS2's own Python, which has no pip. Check with `where python`, or write the full path to the interpreter you want:

```powershell
& "C:\Users\you\AppData\Local\Python\bin\python.exe" -m pip install ".[test]"
```

The dot at the end means "this folder": pip builds the package from your copy and never downloads Yatrices from PyPI. yatricespy isn't on PyPI yet; I'll publish it there later, and then `pip install yatricespy` will be enough.

After changing any C++ file, run the `pip install` line again to rebuild.

## Using it

It is wholeheartedly recommended to import the package as `import yatricespy as yt`. It's the same kind of acronym as `import numpy as np`, and all the examples here use it.

```python
import yatricespy as yt

x = yt.Vec([1, 2, 3])      # from a list
z = yt.Vec(3)              # [0, 0, 0]
s = yt.Vec(3, 7.0)         # [7, 7, 7]

x + x, x - x, -x, 2 * x, x / 2   # new vectors
x += x                           # in place, same object

x[0], x[-1]                # indexing, negative indices included
x[1] = 10.0
list(x), x.tolist()        # back to a plain list
```

There are two vector types, matching the C++ ones:

| Python | C++ | Values |
|---|---|---|
| `yt.Vec` (also `yt.Vecd`) | `Vec<double>` / `Vecd` | 64-bit floats, like NumPy's default |
| `yt.Vecf` | `Vec<float>` / `Vecf` | 32-bit floats: half the memory, fewer digits |

You can't mix them: `yt.Vec([1]) + yt.Vecf([1])` raises a `TypeError`.

A longer tour is in [`examples/vector_tour.py`](examples/vector_tour.py).

### Matrices

Since 0.0.4, the engine's `Matrix` is available from Python too.

```python
A = yt.Matrix([[1, 2, 3],
               [4, 5, 6]])   # from a list of rows
Z = yt.Matrix(2, 3)          # 2 x 3 zeros
C = yt.Matrix(2, 3, 7.0)     # 2 x 3 sevens

A.shape                      # (2, 3)
A[1, 2], A[-1, -1]           # one element, negative indices included
A[0, 0] = 10.0
A + C, A - C, -A, 2 * A, A / 2   # new matrices, element by element
A.T                          # the transpose, a new 3 x 2 matrix
A.tolist()                   # back to a list of rows
```

Every row must have the same length: `yt.Matrix([[1, 2], [3]])` raises `yt.RaggedRowsError`. A NumPy 2-D array works as the list of rows, too: `yt.Matrix(np.eye(3))`.

| Python | C++ | Values |
|---|---|---|
| `yt.Matrix` (also `yt.Matrixd`) | `Matrix<double>` / `Matrixd` | 64-bit floats |
| `yt.Matrixf` | `Matrix<float>` / `Matrixf` | 32-bit floats |

As with vectors, the two can't be mixed, and a `Matrix` only works with a `Vec` (a `Matrixf` with a `Vecf`).

`A @ B` and `A @ x` are not there yet: the matrix products are the next item in the [roadmap](../ROADMAP.md).

A longer tour is in [`examples/matrix_tour.py`](examples/matrix_tour.py).

## Everything available

**Methods on `Vec` and `Vecf`**

| Method | Result |
|---|---|
| `size()`, `len(v)`, `empty()` | number of elements; whether there are none |
| `v[i]`, `v[i] = value` | read or write one element (`-1` is the last) |
| `tolist()`, `copy()` | a plain list; an independent copy |
| `fill(value)`, `scale(alpha)` | set every element; multiply every element (in place) |
| `axpy(alpha, x)` | `v += alpha * x` (in place) |
| `dot(other)`, `v @ other` | dot product |
| `norm1()`, `norm2()`, `norm_inf()` | sum of absolute values; length; largest absolute value |
| `sum()`, `mean()`, `min()`, `max()` | the usual reductions |
| `all_finite()` | `False` if anything is NaN or infinite |
| `Vec.dtype`, `Vec.default_tolerance()` | `"float64"`; the tolerance `allclose` uses by default |

**Methods on `Matrix` and `Matrixf`**

| Method | Result |
|---|---|
| `shape`, `rows()`, `cols()`, `size()`, `len(A)` | `(rows, cols)`; rows; columns; `rows * cols`; rows |
| `empty()`, `is_square()` | whether there are no elements; whether `rows == cols` |
| `A[i, j]`, `A[i, j] = value` | read or write one element (`-1` is the last row or column) |
| `tolist()`, `flatten()`, `copy()` | a list of rows; every element in a `Vec`, row after row; an independent copy |
| `reshape(rows, cols)` | change the shape in place, keeping the number of elements |
| `fill(value)`, `scale(alpha)`, `apply(f)` | set every element; multiply every element; replace each `x` by `f(x)` (all in place) |
| `transposed()`, `A.T` | a new matrix with rows and columns swapped |
| `transpose_in_place()` | the same without a copy (square matrices only) |
| `trace()` | sum of the main diagonal (square matrices only) |
| `diagonal(k=0)`, `set_diagonal(d, k=0)` | read or overwrite a diagonal: `0` is the main one, `k > 0` above, `k < 0` below |
| `swap_rows(i, j)`, `scale_row(i, alpha)`, `add_row_multiple(target, source, alpha)` | the three elementary row operations, in place |
| `sum()`, `mean()`, `min()`, `max()`, `max_abs()` | reductions over every element |
| `sum_rows()`, `sum_cols()`, `mean_rows()`, `mean_cols()` | one result per row or per column, as a vector |
| `all_finite()` | `False` if anything is NaN or infinite |
| `Matrix.dtype`, `Matrix.default_tolerance()` | `"float64"`; the tolerance `allclose` uses by default |

**Functions**

| Function | Result |
|---|---|
| `yt.hadamard(x, y)` | element-wise product (two vectors or two matrices) |
| `yt.cross(x, y)` | cross product (size 3 only) |
| `yt.angle(x, y)` | angle between two vectors, in radians |
| `yt.project(v, onto)` | projection of `v` onto the direction of `onto` |
| `yt.add_to_rows(A, v)`, `yt.subtract_from_rows(A, v)` | add or subtract `v` to every row of `A`, in place |
| `yt.centre_columns(A)` | make every column of `A` average 0, in place; returns the column means |
| `yt.allclose(x, y, rtol=..., atol=...)` | `True` if every pair of elements is close enough (vectors or matrices) |
| `yt.approx_equal(a, b, rtol=..., atol=...)` | the same test for two plain numbers |

## Errors

Every error from the engine arrives as its own Python class. Each one inherits from `yt.YatricesError` **and** from the matching built-in error, so either style of `except` works:

| Error | Also a | Raised when |
|---|---|---|
| `yt.IndexError` | `IndexError` | an index is out of range |
| `yt.SizeMismatchError` | `ValueError` | two vectors should have the same size but don't |
| `yt.EmptyVectorError` | `ValueError` | `mean`, `min` or `max` of an empty vector |
| `yt.WrongSizeError` | `ValueError` | `cross` on vectors that aren't of size 3 |
| `yt.ZeroVectorError` | `ValueError` | `angle` or `project` with a vector of zeros |
| `yt.ShapeMismatchError` | `ValueError` | two matrices whose shapes don't fit the operation, or a `reshape` that changes the element count |
| `yt.NotSquareError` | `ValueError` | `trace` or `transpose_in_place` on a matrix that isn't square |
| `yt.RaggedRowsError` | `ValueError` | building a matrix from rows of different lengths |
| `yt.EmptyMatrixError` | `ValueError` | `mean`, `min`, `max` or `max_abs` of an empty matrix |
| `yt.AllocationSizeError` | `ValueError` | a size far too large to exist |
| `yt.OutOfMemoryError` | `MemoryError` | the computer ran out of memory |

```python
try:
    yt.Vec([1, 2]) + yt.Vec([1, 2, 3])
except yt.SizeMismatchError as e:
    print(e)
# yatrices: operator+: cannot combine a vector of size 2 with one of size 3.
# Both operands must have the same size.
```

`from yatricespy import *` deliberately leaves out `IndexError`, so that it can't hide Python's own one. Write `yt.IndexError` instead.

## Layout

```
python/
├── yatricespy/   the package users import (plus type hints for your editor)
├── bindings/     pybind11 code that exposes the engine as _yatricespy
├── capi/         optional plain C interface, for ctypes (empty for now)
├── examples/     short scripts showing the library in use
└── tests/        pytest tests, checked against NumPy
```

If you add something to `bindings/bindings.cpp`, add it to `yatricespy/__init__.py` and `yatricespy/_yatricespy.pyi` too, or your editor won't know it exists.

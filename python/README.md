# yatricespy (Python library)

The Python front end of YatricesEngine. Every function runs in the C++ engine in `../cpp`; Python only passes the call along.

## Installing

From the project folder:

```bash
python -m pip install ".[test]"
python -m pytest
```

pip compiles the engine, so CMake and a C++17 compiler must be on your PATH.

**On Windows**, MSYS2's g++ (the UCRT64 one, `C:\msys64\ucrt64\bin`) works with a normal python.org Python; this has been tested with Python 3.14. The build links the compiler's runtime into the module, because Python does not search `PATH` for the DLLs that a module needs. The free [Visual Studio Build Tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/) should work as well, but that route has not been tested.

Be careful which `python` you run: on a machine with MSYS2, typing `python` may start MSYS2's own Python, which has no pip. Check with `where python`, or write the full path to the interpreter you want:

```powershell
& "C:\Users\you\AppData\Local\Python\bin\python.exe" -m pip install ".[test]"
```

The dot at the end means "this folder": pip builds the package from your copy and never downloads Yatrices from PyPI. yatricespy isn't on PyPI yet; I'll publish it there later, and then `pip install yatricespy` will be enough.

After changing any C++ file, run the `pip install` line again to rebuild.

## Using it

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

**Functions**

| Function | Result |
|---|---|
| `yt.hadamard(x, y)` | element-wise product |
| `yt.cross(x, y)` | cross product (size 3 only) |
| `yt.angle(x, y)` | angle between two vectors, in radians |
| `yt.project(v, onto)` | projection of `v` onto the direction of `onto` |
| `yt.allclose(x, y, rtol=..., atol=...)` | `True` if every pair of elements is close enough |
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

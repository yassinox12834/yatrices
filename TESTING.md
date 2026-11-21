# Testing Yatrices

Every result gets checked against NumPy. If the two disagree, assume Yatrices is the one that's wrong until you've proved otherwise.

## Python tests (pytest)

```bash
python -m pip install ".[test]"   # rebuild after every C++ change
python -m pytest                  # runs everything in python/tests/
python -m pytest -k dot -v        # only the tests with "dot" in their name
```

On Windows with MSYS2 installed, check that `python` is the interpreter you mean (`where python`), or write its full path, for example `& "C:\Users\you\AppData\Local\Python\bin\python.exe" -m pytest`.

Expected result: `54 passed`.

The tests live in two files:

| File | What it checks |
|---|---|
| `python/tests/test_smoke.py` | the package imports at all |
| `python/tests/test_vec.py` | every part of `Vec` and `Vecf`: construction, indexing, arithmetic, products, norms, geometry, float comparisons and every error |
| `python/tests/test_matrix.py` | every part of `Matrix` and `Matrixf`: construction, indexing, arithmetic, transpose, diagonals, row operations, reductions, broadcasting and every error |

Most tests run twice, once for the double type and once for the float one, thanks to the `V` fixture at the top of `test_vec.py` and the `M` and `MV` fixtures at the top of `test_matrix.py`.

## C++ tests (ctest)

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

`cpp/tests/test_main.cpp` holds the engine's own checks (230 of them since 0.0.2). The Python tests are a second safety net, since every Python call runs the C++ code underneath.

## Writing a test

1. Compute the result with Yatrices.
2. Compute the same thing with NumPy.
3. Compare them with a tolerance, never with `==` on floats.

```python
def test_norm2(V):
    v = [3.0, 4.0]
    assert V(v).norm2() == pytest.approx(np.linalg.norm(v))
```

Test the failures as well: mismatched sizes must raise `yt.SizeMismatchError`, and a bad index must raise `yt.IndexError`.

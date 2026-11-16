# Testing Yatrices

Every result gets checked against NumPy. If the two disagree, assume Yatrices is the one that's wrong until you've proved otherwise.

## Python tests (pytest)

```bash
python -m pip install ".[test]"   # rebuild after every C++ change
python -m pytest                  # runs everything in python/tests/
python -m pytest -k dot -v        # only the tests with "dot" in their name
```

On Windows with MSYS2 installed, check that `python` is the interpreter you mean (`where python`), or write its full path, for example `& "C:\Users\you\AppData\Local\Python\bin\python.exe" -m pytest`.

Expected result: `25 passed`.

The tests live in two files:

| File | What it checks |
|---|---|
| `python/tests/test_smoke.py` | the package imports at all |
| `python/tests/test_vec.py` | every part of `Vec` and `Vecf`: construction, indexing, arithmetic, products, norms, geometry, float comparisons and every error |

Most tests run twice, once for `Vec` (double) and once for `Vecf` (float), thanks to the `V` fixture at the top of `test_vec.py`.

## C++ tests (ctest)

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Fair warning: `cpp/tests/test_main.cpp` is still empty, so `ctest` will happily pass without checking a thing. For now, the Python tests are the real safety net, since every Python call runs the C++ code underneath. Proper C++ tests (Catch2 or GoogleTest) are on the list.

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

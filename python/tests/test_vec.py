"""Tests for yatricespy.Vec and Vecf. NumPy is the reference for every number."""
import copy
import math

import numpy as np
import pytest

import yatricespy as yt

X = [1.0, 2.0, 3.0]
Y = [4.0, 5.0, 6.0]

@pytest.fixture(params=[yt.Vec, yt.Vecf], ids=["Vec", "Vecf"])
def V(request):
    return request.param

def test_constructors(V):
    assert list(V(3)) == [0, 0, 0]
    assert list(V(2, 7.0)) == [7, 7]
    assert list(V([1, 2, 3])) == [1, 2, 3]
    assert list(V(np.array([1.5, 2.5]))) == [1.5, 2.5]
    assert len(V([])) == 0 and V([]).empty()

def test_indexing(V):
    v = V(X)
    assert v[0] == 1 and v[-1] == 3
    v[1] = 10
    assert v.tolist() == [1, 10, 3]
    with pytest.raises(yt.IndexError):
        v[3]
    with pytest.raises(IndexError):
        v[-4]

def test_copy_is_independent(V):
    a = V(X)
    for b in (a.copy(), copy.copy(a), copy.deepcopy(a), +a):
        b[0] = 99
        assert a[0] == 1

def test_repr_and_dtype():
    assert repr(yt.Vec([1, 2.5])) == "Vec([1, 2.5])"
    assert repr(yt.Vecf([1])) == "Vecf([1])"
    assert yt.Vec.dtype == "float64" and yt.Vecf.dtype == "float32"
    assert yt.Vecd is yt.Vec

def test_arithmetic_matches_numpy(V):
    x, y = V(X), V(Y)
    nx, ny = np.array(X), np.array(Y)
    assert np.allclose(list(x + y), nx + ny)
    assert np.allclose(list(y - x), ny - nx)
    assert np.allclose(list(-x), -nx)
    assert np.allclose(list(2 * x), 2 * nx)
    assert np.allclose(list(x * 2), nx * 2)
    assert np.allclose(list(y / 2), ny / 2)

def test_in_place_keeps_the_same_object(V):
    x = V(X)
    alias = x
    x += V(Y)
    x *= 10
    x -= V(Y)
    x /= 2
    assert alias is x
    assert np.allclose(list(x), ((np.array(X) + Y) * 10 - Y) / 2)

def test_kernels(V):
    v = V(X)
    v.fill(2)
    assert v.tolist() == [2, 2, 2]
    v.scale(3)
    assert v.tolist() == [6, 6, 6]
    v.axpy(2, V([1, 1, 1]))
    assert v.tolist() == [8, 8, 8]

def test_products(V):
    x, y = V(X), V(Y)
    assert x.dot(y) == pytest.approx(np.dot(X, Y))
    assert x @ y == pytest.approx(32)
    assert np.allclose(list(yt.hadamard(x, y)), np.multiply(X, Y))
    assert np.allclose(list(yt.cross(x, y)), np.cross(X, Y))

def test_norms_and_reductions(V):
    v = V([3, -4])
    assert v.norm1() == pytest.approx(7)
    assert v.norm2() == pytest.approx(np.linalg.norm([3, -4]))
    assert v.norm_inf() == pytest.approx(4)
    assert (v.sum(), v.mean(), v.min(), v.max()) == (-1, -0.5, -4, 3)
    assert yt.Vec([3e200, 4e200]).norm2() == pytest.approx(5e200)

def test_geometry(V):
    assert yt.angle(V([1, 0]), V([1, 1])) == pytest.approx(math.pi / 4, rel=1e-6)
    assert np.allclose(list(yt.project(V([2, 3]), V([1, 0]))), [2, 0])

def test_float_comparisons():
    assert not (0.1 + 0.2 == 0.3)
    assert yt.approx_equal(0.1 + 0.2, 0.3)
    assert yt.allclose(yt.Vec([0.1 + 0.2]), yt.Vec([0.3]))
    assert not yt.allclose(yt.Vec([1.0]), yt.Vec([1.1]))
    assert not yt.Vec([1, math.nan]).all_finite()

def test_errors(V):
    with pytest.raises(yt.SizeMismatchError):
        V([1, 2]) + V([1, 2, 3])
    with pytest.raises(yt.EmptyVectorError):
        V([]).mean()
    with pytest.raises(yt.WrongSizeError):
        yt.cross(V([1, 2]), V([3, 4]))
    with pytest.raises(yt.ZeroVectorError):
        yt.angle(V([1, 0]), V(2))

def test_error_hierarchy():
    assert issubclass(yt.SizeMismatchError, yt.YatricesError)
    assert issubclass(yt.SizeMismatchError, ValueError)
    assert issubclass(yt.IndexError, IndexError)
    assert issubclass(yt.OutOfMemoryError, MemoryError)

def test_cannot_mix_float_and_double():
    with pytest.raises(TypeError):
        yt.Vec([1.0]) + yt.Vecf([1.0])

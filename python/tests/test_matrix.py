"""Tests for yatricespy.Matrix and Matrixf. NumPy is the reference for every number."""
import copy

import numpy as np
import pytest

import yatricespy as yt

A_ROWS = [[1.0, 2.0, 3.0], [4.0, 5.0, 6.0]]
B_ROWS = [[6.0, 5.0, 4.0], [3.0, 2.0, 1.0]]
SQ_ROWS = [[2.0, -1.0, 0.0], [1.0, 3.0, 4.0], [0.5, 7.0, -2.0]]

@pytest.fixture(params=[(yt.Matrix, yt.Vec), (yt.Matrixf, yt.Vecf)], ids=["Matrix", "Matrixf"])
def MV(request):
    return request.param

@pytest.fixture
def M(MV):
    return MV[0]

def test_constructors(M):
    assert M(2, 3).tolist() == [[0, 0, 0], [0, 0, 0]]
    assert M(2, 2, 7.0).tolist() == [[7, 7], [7, 7]]
    assert M(A_ROWS).tolist() == A_ROWS
    assert M(np.array(A_ROWS)).tolist() == A_ROWS
    assert M([]).empty() and M([]).shape == (0, 0)
    with pytest.raises(yt.RaggedRowsError):
        M([[1, 2], [3]])
    with pytest.raises(ValueError):
        M([[1, 2, 3], [4, 5]])

def test_shape(M):
    A = M(A_ROWS)
    assert A.shape == (2, 3) and A.rows() == 2 and A.cols() == 3
    assert A.size() == 6 and len(A) == 2
    assert not A.is_square() and M(SQ_ROWS).is_square()

def test_indexing(M):
    A = M(A_ROWS)
    assert A[0, 0] == 1 and A[1, 2] == 6 and A[-1, -1] == 6 and A[-2, 1] == 2
    A[1, 0] = 40
    assert A.tolist()[1][0] == 40
    with pytest.raises(yt.IndexError):
        A[2, 0]
    with pytest.raises(IndexError):
        A[0, -4]

def test_copy_is_independent(M):
    A = M(A_ROWS)
    for B in (A.copy(), copy.copy(A), copy.deepcopy(A), +A):
        B[0, 0] = 99
        assert A[0, 0] == 1

def test_repr_and_dtype():
    assert repr(yt.Matrix([[1, 2], [3, 4]])) == "Matrix([[1, 2],\n        [3, 4]])"
    assert str(yt.Matrixf([[1.5]])) == "[[1.5]]"
    assert yt.Matrix.dtype == "float64" and yt.Matrixf.dtype == "float32"
    assert yt.Matrixd is yt.Matrix

def test_arithmetic_matches_numpy(M):
    A, B = M(A_ROWS), M(B_ROWS)
    na, nb = np.array(A_ROWS), np.array(B_ROWS)
    assert np.allclose((A + B).tolist(), na + nb)
    assert np.allclose((A - B).tolist(), na - nb)
    assert np.allclose((-A).tolist(), -na)
    assert np.allclose((2 * A).tolist(), 2 * na)
    assert np.allclose((A * 2).tolist(), na * 2)
    assert np.allclose((A / 4).tolist(), na / 4)
    assert np.allclose(yt.hadamard(A, B).tolist(), na * nb)
    with pytest.raises(yt.ShapeMismatchError):
        A + M(SQ_ROWS)

def test_in_place_keeps_the_same_object(M):
    A = M(A_ROWS)
    alias = A
    A += M(B_ROWS)
    A *= 2
    A -= M(2, 3, 1.0)
    A /= 2
    assert A is alias
    assert np.allclose(A.tolist(), ((np.array(A_ROWS) + B_ROWS) * 2 - 1) / 2)
    A.fill(3)
    A.scale(2)
    assert A.tolist() == [[6, 6, 6], [6, 6, 6]]
    assert A.apply(lambda x: x + 1) is A
    assert A.tolist() == [[7, 7, 7], [7, 7, 7]]

def test_reshape_and_flatten(M):
    A = M(A_ROWS)
    assert A.flatten().tolist() == [1, 2, 3, 4, 5, 6]
    A.reshape(3, 2)
    assert A.tolist() == np.array(A_ROWS).reshape(3, 2).tolist()
    with pytest.raises(yt.ShapeMismatchError):
        A.reshape(4, 2)

def test_transpose(M):
    A = M(A_ROWS)
    assert A.transposed().tolist() == np.array(A_ROWS).T.tolist()
    assert A.T.tolist() == np.array(A_ROWS).T.tolist()
    S = M(SQ_ROWS)
    S.transpose_in_place()
    assert S.tolist() == np.array(SQ_ROWS).T.tolist()
    with pytest.raises(yt.NotSquareError):
        A.transpose_in_place()
    big = np.arange(70 * 45, dtype=float).reshape(70, 45)
    assert M(big).T.tolist() == big.T.tolist()

def test_trace_and_diagonals(MV):
    M, V = MV
    S = M(SQ_ROWS)
    ns = np.array(SQ_ROWS)
    assert S.trace() == pytest.approx(np.trace(ns))
    for k in (-2, -1, 0, 1, 2, 5):
        assert np.allclose(S.diagonal(k).tolist(), np.diagonal(ns, k))
    with pytest.raises(yt.NotSquareError):
        M(A_ROWS).trace()
    S.set_diagonal(V([9, 9]), 1)
    assert S[0, 1] == 9 and S[1, 2] == 9
    with pytest.raises(yt.WrongSizeError):
        S.set_diagonal(V([1, 2, 3]), 1)

def test_row_operations(M):
    A = M(SQ_ROWS)
    na = np.array(SQ_ROWS)
    A.swap_rows(0, 2)
    na[[0, 2]] = na[[2, 0]]
    A.scale_row(1, 3)
    na[1] *= 3
    A.add_row_multiple(2, 0, -2)
    na[2] += -2 * na[0]
    A.add_row_multiple(-1, -1, 1)
    na[2] *= 2
    assert np.allclose(A.tolist(), na)
    with pytest.raises(yt.IndexError):
        A.swap_rows(0, 3)

def test_reductions_match_numpy(M):
    A = M([[1, -7, 3], [4, 5, -6]])
    na = np.array([[1, -7, 3], [4, 5, -6]], dtype=float)
    assert A.sum() == pytest.approx(na.sum())
    assert A.mean() == pytest.approx(na.mean())
    assert A.min() == na.min() and A.max() == na.max()
    assert A.max_abs() == np.abs(na).max()
    assert np.allclose(A.sum_rows().tolist(), na.sum(axis=1))
    assert np.allclose(A.sum_cols().tolist(), na.sum(axis=0))
    assert np.allclose(A.mean_rows().tolist(), na.mean(axis=1))
    assert np.allclose(A.mean_cols().tolist(), na.mean(axis=0))
    assert A.all_finite()
    A[0, 0] = float("nan")
    assert not A.all_finite()
    for f in (M([]).mean, M([]).min, M([]).max, M([]).max_abs):
        with pytest.raises(yt.EmptyMatrixError):
            f()

def test_broadcasting(MV):
    M, V = MV
    A = M(A_ROWS)
    yt.add_to_rows(A, V([10, 20, 30]))
    assert np.allclose(A.tolist(), np.array(A_ROWS) + [10, 20, 30])
    yt.subtract_from_rows(A, V([10, 20, 30]))
    assert np.allclose(A.tolist(), A_ROWS)
    means = yt.centre_columns(A)
    assert np.allclose(means.tolist(), np.mean(A_ROWS, axis=0))
    assert np.allclose(A.mean_cols().tolist(), [0, 0, 0])
    with pytest.raises(yt.SizeMismatchError):
        yt.add_to_rows(A, V([1, 2]))

def test_allclose(M):
    A = M(A_ROWS)
    tol = M.default_tolerance()
    assert yt.allclose(A, A + M(2, 3, tol / 10))
    assert not yt.allclose(A, A + M(2, 3, 1e-2))
    assert yt.allclose(A, A + M(2, 3, 1e-6), rtol=1e-5, atol=1e-5)
    assert not yt.allclose(A, A.T)

def test_types_do_not_mix():
    with pytest.raises(TypeError):
        yt.Matrix([[1]]) + yt.Matrixf([[1]])
    with pytest.raises(TypeError):
        yt.add_to_rows(yt.Matrix([[1]]), yt.Vecf([1]))

def test_error_classes():
    for e in (yt.ShapeMismatchError, yt.NotSquareError, yt.RaggedRowsError, yt.EmptyMatrixError):
        assert issubclass(e, yt.YatricesError) and issubclass(e, ValueError)

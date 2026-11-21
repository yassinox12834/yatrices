"""yatricespy: Python front end of YatricesEngine.

Every function here runs in the C++ engine; Python only forwards the call.

    >>> import yatricespy as yt
    >>> x = yt.Vec([1, 2, 3])
    >>> y = yt.Vec([4, 5, 6])
    >>> x + y
    Vec([5, 7, 9])
    >>> x @ y
    32.0
    >>> A = yt.Matrix([[1, 2], [3, 4]])
    >>> A.T
    Matrix([[1, 3],
            [2, 4]])
"""
from ._yatricespy import (
    Matrix,
    Matrixf,
    Vec,
    Vecf,
    add_to_rows,
    allclose,
    angle,
    approx_equal,
    centre_columns,
    cross,
    hadamard,
    project,
    subtract_from_rows,
    AllocationSizeError,
    EmptyMatrixError,
    EmptyVectorError,
    IndexError,
    NotSquareError,
    OutOfMemoryError,
    RaggedRowsError,
    ShapeMismatchError,
    SizeMismatchError,
    WrongSizeError,
    YatricesError,
    ZeroVectorError,
)

Vecd = Vec
Matrixd = Matrix

__version__ = "0.0.4"

__all__ = [
    "Vec", "Vecd", "Vecf", "Matrix", "Matrixd", "Matrixf",
    "allclose", "angle", "approx_equal", "cross", "hadamard", "project",
    "add_to_rows", "subtract_from_rows", "centre_columns",
    "YatricesError", "SizeMismatchError", "EmptyVectorError",
    "WrongSizeError", "ZeroVectorError", "ShapeMismatchError", "NotSquareError",
    "RaggedRowsError", "EmptyMatrixError", "AllocationSizeError", "OutOfMemoryError",
]

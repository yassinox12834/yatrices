"""yatricespy: Python front end of YatricesEngine.

Every function here runs in the C++ engine; Python only forwards the call.

    >>> import yatricespy as yt
    >>> x = yt.Vec([1, 2, 3])
    >>> y = yt.Vec([4, 5, 6])
    >>> x + y
    Vec([5, 7, 9])
    >>> x @ y
    32.0
"""
from ._yatricespy import (
    Vec,
    Vecf,
    allclose,
    angle,
    approx_equal,
    cross,
    hadamard,
    project,
    AllocationSizeError,
    EmptyVectorError,
    IndexError,
    OutOfMemoryError,
    SizeMismatchError,
    WrongSizeError,
    YatricesError,
    ZeroVectorError,
)

Vecd = Vec

__version__ = "0.0.3"

__all__ = [
    "Vec", "Vecd", "Vecf",
    "allclose", "angle", "approx_equal", "cross", "hadamard", "project",
    "YatricesError", "SizeMismatchError", "EmptyVectorError",
    "WrongSizeError", "ZeroVectorError", "AllocationSizeError", "OutOfMemoryError",
]

"""yatricespy 0.0.4: Python front end of YatricesEngine (type stub).

Using it, the NumPy way
-----------------------
    import yatricespy as yt          # like: import numpy as np

    x = yt.Vec([1, 2, 3])            # like: np.array([1, 2, 3])
    y = yt.Vec([4, 5, 6])
    x + y                            # Vec([5, 7, 9])
    2 * x                            # Vec([2, 4, 6])
    x @ y                            # 32.0, the dot product
    x.norm2()                        # length, like np.linalg.norm(x)

    A = yt.Matrix([[1, 2, 3], [4, 5, 6]])
    A.shape                          # (2, 3)
    A[1, 2]                          # 6.0
    A.T                              # transpose, like A.T in NumPy
    A.mean_cols()                    # like A.mean(axis=0)

Next version: commands
----------------------
Example for 0.0.5. First change the number in CMakeLists.txt, pyproject.toml
and yatricespy/__init__.py, and add an entry to CHANGELOG.md. Then:

    git add -A
    git commit -m "feat(python): short summary, release 0.0.5"
    git push origin main
    git tag -a v0.0.5 -m "yatrices 0.0.5"
    git push origin v0.0.5
"""
import builtins
from collections.abc import Callable, Iterable, Iterator
from typing import ClassVar, overload

class _VecBase:
    """Methods shared by Vec (float64) and Vecf (float32). Not meant to be used directly."""
    dtype: ClassVar[str]
    """Name of the number type: "float64" for Vec and Matrix, "float32" for Vecf and Matrixf."""
    alignment: ClassVar[int]
    """Memory alignment of the data, in bytes, chosen by the engine so that its loops can run fast."""
    @staticmethod
    def default_tolerance() -> float:
        """Tolerance that allclose uses when you give none. It depends on the number type (float32 needs a looser one than float64)."""
    def size(self) -> int:
        """Number of elements. Example: Vec([1, 2, 3]).size() is 3."""
    def empty(self) -> bool:
        """True when the vector has no elements."""
    def __len__(self) -> int:
        """Same as size(), so len(v) works."""
    def __getitem__(self, i: int) -> float:
        """v[i] reads element i. Negative indices count from the end, so v[-1] is the last. Raises IndexError (yt.IndexError) when i is out of range."""
    def __setitem__(self, i: int, value: float) -> None:
        """v[i] = value writes element i, with the same index rules as reading."""
    def __iter__(self) -> Iterator[float]:
        """Lets you loop over the vector (for x in v) and build lists (list(v))."""
    def tolist(self) -> list[float]:
        """Copy the elements into a plain Python list of floats. Example: Vec([1, 2]).tolist() is [1.0, 2.0]."""
    def fill(self, value: float) -> None:
        """Set every element to value, in place. Returns nothing."""
    def scale(self, alpha: float) -> None:
        """Multiply every element by alpha, in place. Example: v = Vec([1, 2]); v.scale(3) makes v equal to [3, 6]."""
    def norm1(self) -> float:
        """Sum of the absolute values, |x1| + |x2| + ... Example: Vec([3, -4]).norm1() is 7."""
    def norm2(self) -> float:
        """Euclidean length, sqrt(x1^2 + x2^2 + ...), computed so that it does not overflow. Example: Vec([3, -4]).norm2() is 5. Like np.linalg.norm(x)."""
    def norm_inf(self) -> float:
        """Largest absolute value. Example: Vec([3, -4]).norm_inf() is 4."""
    def sum(self) -> float:
        """Sum of the elements, added in pairs to keep rounding errors small."""
    def mean(self) -> float:
        """Average of the elements. Raises EmptyVectorError when the vector is empty."""
    def min(self) -> float:
        """Smallest element. Raises EmptyVectorError when the vector is empty."""
    def max(self) -> float:
        """Largest element. Raises EmptyVectorError when the vector is empty."""
    def all_finite(self) -> bool:
        """False if any element is NaN or infinite, True otherwise."""

class Vec(_VecBase):
    """Fixed-size vector of float64 values (C++ Vec<double>), like NumPy's default array. Build one from a list, Vec([1, 2, 3]), from a size, Vec(3), or from a size and a value, Vec(3, 7.0). Cannot be mixed with Vecf."""
    @overload
    def __init__(self, n: int) -> None:
        """A vector of n zeros. Example: Vec(3) is [0, 0, 0]."""
    @overload
    def __init__(self, n: int, value: float) -> None:
        """A vector of n copies of value. Example: Vec(3, 7.0) is [7, 7, 7]."""
    @overload
    def __init__(self, values: Iterable[float]) -> None:
        """A copy of a list, or of any sequence of numbers (a 1-D NumPy array works too). Example: Vec([1, 2, 3])."""
    def copy(self) -> Vec:
        """An independent copy: changing one vector does not change the other."""
    def axpy(self, alpha: float, x: Vec) -> None:
        """In place: self += alpha * x (the BLAS name 'a x plus y'). Both vectors must have the same size, otherwise SizeMismatchError. Example: v = Vec([1, 1]); v.axpy(2, Vec([1, 2])) makes v equal to [3, 5]."""
    def dot(self, other: Vec) -> float:
        """Dot product: the sum of self[i] * other[i]. Example: Vec([1, 2, 3]).dot(Vec([4, 5, 6])) is 32. Raises SizeMismatchError when the sizes differ."""
    def __matmul__(self, other: Vec) -> float:
        """x @ y, the same as x.dot(y)."""
    def __add__(self, other: Vec) -> Vec:
        """x + y, element by element, as a new vector. Raises SizeMismatchError when the sizes differ."""
    def __sub__(self, other: Vec) -> Vec:
        """x - y, element by element, as a new vector. Raises SizeMismatchError when the sizes differ."""
    def __neg__(self) -> Vec:
        """-x, a new vector with every sign flipped."""
    def __pos__(self) -> Vec:
        """+x, an independent copy of x."""
    def __mul__(self, k: float) -> Vec:
        """x * k, a new vector with every element multiplied by the number k."""
    def __rmul__(self, k: float) -> Vec:
        """k * x, the same as x * k (so 2 * x works)."""
    def __truediv__(self, k: float) -> Vec:
        """x / k, a new vector with every element divided by the number k."""
    def __iadd__(self, other: Vec) -> Vec:
        """x += y, in place. Keeps the same object and returns it."""
    def __isub__(self, other: Vec) -> Vec:
        """x -= y, in place. Keeps the same object and returns it."""
    def __imul__(self, k: float) -> Vec:
        """x *= k, in place. Keeps the same object and returns it."""
    def __itruediv__(self, k: float) -> Vec:
        """x /= k, in place. Keeps the same object and returns it."""

class Vecf(_VecBase):
    """Fixed-size vector of float32 values (C++ Vec<float>): half the memory of Vec, fewer digits. Cannot be mixed with Vec."""
    @overload
    def __init__(self, n: int) -> None:
        """A vector of n zeros. Example: Vec(3) is [0, 0, 0]."""
    @overload
    def __init__(self, n: int, value: float) -> None:
        """A vector of n copies of value. Example: Vec(3, 7.0) is [7, 7, 7]."""
    @overload
    def __init__(self, values: Iterable[float]) -> None:
        """A copy of a list, or of any sequence of numbers (a 1-D NumPy array works too). Example: Vec([1, 2, 3])."""
    def copy(self) -> Vecf:
        """An independent copy: changing one vector does not change the other."""
    def axpy(self, alpha: float, x: Vecf) -> None:
        """In place: self += alpha * x (the BLAS name 'a x plus y'). Both vectors must have the same size, otherwise SizeMismatchError. Example: v = Vec([1, 1]); v.axpy(2, Vec([1, 2])) makes v equal to [3, 5]."""
    def dot(self, other: Vecf) -> float:
        """Dot product: the sum of self[i] * other[i]. Example: Vec([1, 2, 3]).dot(Vec([4, 5, 6])) is 32. Raises SizeMismatchError when the sizes differ."""
    def __matmul__(self, other: Vecf) -> float:
        """x @ y, the same as x.dot(y)."""
    def __add__(self, other: Vecf) -> Vecf:
        """x + y, element by element, as a new vector. Raises SizeMismatchError when the sizes differ."""
    def __sub__(self, other: Vecf) -> Vecf:
        """x - y, element by element, as a new vector. Raises SizeMismatchError when the sizes differ."""
    def __neg__(self) -> Vecf:
        """-x, a new vector with every sign flipped."""
    def __pos__(self) -> Vecf:
        """+x, an independent copy of x."""
    def __mul__(self, k: float) -> Vecf:
        """x * k, a new vector with every element multiplied by the number k."""
    def __rmul__(self, k: float) -> Vecf:
        """k * x, the same as x * k (so 2 * x works)."""
    def __truediv__(self, k: float) -> Vecf:
        """x / k, a new vector with every element divided by the number k."""
    def __iadd__(self, other: Vecf) -> Vecf:
        """x += y, in place. Keeps the same object and returns it."""
    def __isub__(self, other: Vecf) -> Vecf:
        """x -= y, in place. Keeps the same object and returns it."""
    def __imul__(self, k: float) -> Vecf:
        """x *= k, in place. Keeps the same object and returns it."""
    def __itruediv__(self, k: float) -> Vecf:
        """x /= k, in place. Keeps the same object and returns it."""

class _MatrixBase:
    """Methods shared by Matrix (float64) and Matrixf (float32). Not meant to be used directly."""
    dtype: ClassVar[str]
    """Name of the number type: "float64" for Vec and Matrix, "float32" for Vecf and Matrixf."""
    alignment: ClassVar[int]
    """Memory alignment of the data, in bytes, chosen by the engine so that its loops can run fast."""
    @staticmethod
    def default_tolerance() -> float:
        """Tolerance that allclose uses when you give none. It depends on the number type (float32 needs a looser one than float64)."""
    @property
    def shape(self) -> tuple[int, int]:
        """(rows, cols), like NumPy's shape. Example: Matrix([[1, 2, 3], [4, 5, 6]]).shape is (2, 3)."""
    def rows(self) -> int:
        """Number of rows."""
    def cols(self) -> int:
        """Number of columns."""
    def size(self) -> int:
        """Number of elements, rows * cols."""
    def empty(self) -> bool:
        """True when the matrix has no elements."""
    def is_square(self) -> bool:
        """True when rows == cols."""
    def __len__(self) -> int:
        """Number of rows, so len(A) works (like NumPy)."""
    def __getitem__(self, ij: tuple[int, int]) -> float:
        """A[i, j] reads one element: row i, column j. Negative indices count from the end, so A[-1, -1] is the bottom-right element. Raises IndexError when out of range."""
    def __setitem__(self, ij: tuple[int, int], value: float) -> None:
        """A[i, j] = value writes one element, with the same index rules as reading."""
    def tolist(self) -> list[list[float]]:
        """Copy into a plain Python list of rows. Example: Matrix([[1, 2], [3, 4]]).tolist() is [[1.0, 2.0], [3.0, 4.0]]."""
    def reshape(self, rows: int, cols: int) -> None:
        """Change the shape in place, keeping the number of elements: rows * cols must not change, otherwise ShapeMismatchError. Example: a 2 x 3 matrix can become 3 x 2 or 1 x 6."""
    def fill(self, value: float) -> None:
        """Set every element to value, in place."""
    def scale(self, alpha: float) -> None:
        """Multiply every element by alpha, in place."""
    def transpose_in_place(self) -> None:
        """Swap rows and columns without making a copy. Square matrices only, otherwise NotSquareError."""
    def trace(self) -> float:
        """Sum of the main diagonal. Square matrices only, otherwise NotSquareError. Example: for [[2, 1], [1, 3]] the trace is 2 + 3 = 5."""
    def swap_rows(self, i: int, j: int) -> None:
        """Exchange rows i and j, in place (elementary row operation 1)."""
    def scale_row(self, i: int, alpha: float) -> None:
        """Multiply row i by alpha, in place (elementary row operation 2)."""
    def add_row_multiple(self, target: int, source: int, alpha: float) -> None:
        """row[target] += alpha * row[source], in place (elementary row operation 3, the one used in Gaussian elimination). Example: add_row_multiple(1, 0, -0.5) subtracts half of row 0 from row 1."""
    def sum(self) -> float:
        """Sum of every element."""
    def mean(self) -> float:
        """Average of every element. Raises EmptyMatrixError when the matrix is empty."""
    def min(self) -> float:
        """Smallest element. Raises EmptyMatrixError when the matrix is empty."""
    def max(self) -> float:
        """Largest element. Raises EmptyMatrixError when the matrix is empty."""
    def max_abs(self) -> float:
        """Largest absolute value. Raises EmptyMatrixError when the matrix is empty."""
    def all_finite(self) -> bool:
        """False if any element is NaN or infinite, True otherwise."""

class Matrix(_MatrixBase):
    """Dense row-major matrix of float64 values (C++ Matrix<double>). Build one from a list of rows, Matrix([[1, 2], [3, 4]]), from a shape, Matrix(2, 3), or from a shape and a value, Matrix(2, 3, 7.0). Works with Vec, not Vecf."""
    @overload
    def __init__(self, rows: int, cols: int) -> None:
        """A rows x cols matrix of zeros. Example: Matrix(2, 3)."""
    @overload
    def __init__(self, rows: int, cols: int, value: float) -> None:
        """A rows x cols matrix filled with value. Example: Matrix(2, 3, 7.0)."""
    @overload
    def __init__(self, values: Iterable[Iterable[float]]) -> None:
        """A copy of a list of rows (a 2-D NumPy array works too). Every row must have the same length, otherwise RaggedRowsError. Example: Matrix([[1, 2], [3, 4]])."""
    def copy(self) -> Matrix:
        """An independent copy: changing one matrix does not change the other."""
    def flatten(self) -> Vec:
        """Every element, row after row, as one vector. Example: [[1, 2], [3, 4]] becomes Vec([1, 2, 3, 4])."""
    def apply(self, f: Callable[[float], float]) -> Matrix:
        """Replace every element x by f(x), in place, where f is any Python function taking a float and returning a float. Returns the matrix itself, so calls can be chained. Example: A.apply(abs)."""
    def transposed(self) -> Matrix:
        """A new matrix with rows and columns swapped. The original is unchanged."""
    @property
    def T(self) -> Matrix:
        """Same as transposed(), like NumPy's .T."""
    def diagonal(self, k: int = 0) -> Vec:
        """Diagonal k as a vector: 0 is the main diagonal, k > 0 is above it, k < 0 is below it. Example: for [[1, 2], [3, 4]], diagonal() is [1, 4] and diagonal(1) is [2]."""
    def set_diagonal(self, d: Vec, k: int = 0) -> None:
        """Overwrite diagonal k with the values of the vector d. The size of d must equal the length of that diagonal, otherwise WrongSizeError."""
    def sum_rows(self) -> Vec:
        """One sum per row, as a vector of size rows. Like A.sum(axis=1) in NumPy."""
    def sum_cols(self) -> Vec:
        """One sum per column, as a vector of size cols. Like A.sum(axis=0) in NumPy."""
    def mean_rows(self) -> Vec:
        """One mean per row, as a vector of size rows. Like A.mean(axis=1) in NumPy."""
    def mean_cols(self) -> Vec:
        """One mean per column, as a vector of size cols. Like A.mean(axis=0) in NumPy."""
    def __add__(self, other: Matrix) -> Matrix:
        """A + B, element by element, as a new matrix. Both must have the same shape, otherwise ShapeMismatchError."""
    def __sub__(self, other: Matrix) -> Matrix:
        """A - B, element by element, as a new matrix. Both must have the same shape, otherwise ShapeMismatchError."""
    def __neg__(self) -> Matrix:
        """-A, a new matrix with every sign flipped."""
    def __pos__(self) -> Matrix:
        """+A, an independent copy of A."""
    def __mul__(self, k: float) -> Matrix:
        """A * k, a new matrix with every element multiplied by the number k. This is not the matrix product."""
    def __rmul__(self, k: float) -> Matrix:
        """k * A, the same as A * k (so 2 * A works)."""
    def __truediv__(self, k: float) -> Matrix:
        """A / k, a new matrix with every element divided by the number k."""
    def __iadd__(self, other: Matrix) -> Matrix:
        """A += B, in place. Keeps the same object and returns it."""
    def __isub__(self, other: Matrix) -> Matrix:
        """A -= B, in place. Keeps the same object and returns it."""
    def __imul__(self, k: float) -> Matrix:
        """A *= k, in place. Keeps the same object and returns it."""
    def __itruediv__(self, k: float) -> Matrix:
        """A /= k, in place. Keeps the same object and returns it."""

class Matrixf(_MatrixBase):
    """Dense row-major matrix of float32 values (C++ Matrix<float>). Works with Vecf, not Vec."""
    @overload
    def __init__(self, rows: int, cols: int) -> None:
        """A rows x cols matrix of zeros. Example: Matrix(2, 3)."""
    @overload
    def __init__(self, rows: int, cols: int, value: float) -> None:
        """A rows x cols matrix filled with value. Example: Matrix(2, 3, 7.0)."""
    @overload
    def __init__(self, values: Iterable[Iterable[float]]) -> None:
        """A copy of a list of rows (a 2-D NumPy array works too). Every row must have the same length, otherwise RaggedRowsError. Example: Matrix([[1, 2], [3, 4]])."""
    def copy(self) -> Matrixf:
        """An independent copy: changing one matrix does not change the other."""
    def flatten(self) -> Vecf:
        """Every element, row after row, as one vector. Example: [[1, 2], [3, 4]] becomes Vec([1, 2, 3, 4])."""
    def apply(self, f: Callable[[float], float]) -> Matrixf:
        """Replace every element x by f(x), in place, where f is any Python function taking a float and returning a float. Returns the matrix itself, so calls can be chained. Example: A.apply(abs)."""
    def transposed(self) -> Matrixf:
        """A new matrix with rows and columns swapped. The original is unchanged."""
    @property
    def T(self) -> Matrixf:
        """Same as transposed(), like NumPy's .T."""
    def diagonal(self, k: int = 0) -> Vecf:
        """Diagonal k as a vector: 0 is the main diagonal, k > 0 is above it, k < 0 is below it. Example: for [[1, 2], [3, 4]], diagonal() is [1, 4] and diagonal(1) is [2]."""
    def set_diagonal(self, d: Vecf, k: int = 0) -> None:
        """Overwrite diagonal k with the values of the vector d. The size of d must equal the length of that diagonal, otherwise WrongSizeError."""
    def sum_rows(self) -> Vecf:
        """One sum per row, as a vector of size rows. Like A.sum(axis=1) in NumPy."""
    def sum_cols(self) -> Vecf:
        """One sum per column, as a vector of size cols. Like A.sum(axis=0) in NumPy."""
    def mean_rows(self) -> Vecf:
        """One mean per row, as a vector of size rows. Like A.mean(axis=1) in NumPy."""
    def mean_cols(self) -> Vecf:
        """One mean per column, as a vector of size cols. Like A.mean(axis=0) in NumPy."""
    def __add__(self, other: Matrixf) -> Matrixf:
        """A + B, element by element, as a new matrix. Both must have the same shape, otherwise ShapeMismatchError."""
    def __sub__(self, other: Matrixf) -> Matrixf:
        """A - B, element by element, as a new matrix. Both must have the same shape, otherwise ShapeMismatchError."""
    def __neg__(self) -> Matrixf:
        """-A, a new matrix with every sign flipped."""
    def __pos__(self) -> Matrixf:
        """+A, an independent copy of A."""
    def __mul__(self, k: float) -> Matrixf:
        """A * k, a new matrix with every element multiplied by the number k. This is not the matrix product."""
    def __rmul__(self, k: float) -> Matrixf:
        """k * A, the same as A * k (so 2 * A works)."""
    def __truediv__(self, k: float) -> Matrixf:
        """A / k, a new matrix with every element divided by the number k."""
    def __iadd__(self, other: Matrixf) -> Matrixf:
        """A += B, in place. Keeps the same object and returns it."""
    def __isub__(self, other: Matrixf) -> Matrixf:
        """A -= B, in place. Keeps the same object and returns it."""
    def __imul__(self, k: float) -> Matrixf:
        """A *= k, in place. Keeps the same object and returns it."""
    def __itruediv__(self, k: float) -> Matrixf:
        """A /= k, in place. Keeps the same object and returns it."""

@overload
def hadamard(x: Vec, y: Vec) -> Vec:
    """Element-wise product of two vectors: result[i] = x[i] * y[i]. Example: hadamard(Vec([1, 2, 3]), Vec([4, 5, 6])) is [4, 10, 18]. Raises SizeMismatchError when the sizes differ."""
@overload
def hadamard(x: Vecf, y: Vecf) -> Vecf:
    """Element-wise product of two vectors: result[i] = x[i] * y[i]. Example: hadamard(Vec([1, 2, 3]), Vec([4, 5, 6])) is [4, 10, 18]. Raises SizeMismatchError when the sizes differ."""
@overload
def cross(x: Vec, y: Vec) -> Vec:
    """Cross product of two vectors of size 3, a vector perpendicular to both. Example: cross(Vec([1, 0, 0]), Vec([0, 1, 0])) is [0, 0, 1]. Raises WrongSizeError when a size is not 3."""
@overload
def cross(x: Vecf, y: Vecf) -> Vecf:
    """Cross product of two vectors of size 3, a vector perpendicular to both. Example: cross(Vec([1, 0, 0]), Vec([0, 1, 0])) is [0, 0, 1]. Raises WrongSizeError when a size is not 3."""
@overload
def project(v: Vec, onto: Vec) -> Vec:
    """Projection of v onto the direction of onto: (v . onto / onto . onto) * onto. Raises ZeroVectorError when onto is all zeros."""
@overload
def project(v: Vecf, onto: Vecf) -> Vecf:
    """Projection of v onto the direction of onto: (v . onto / onto . onto) * onto. Raises ZeroVectorError when onto is all zeros."""
@overload
def hadamard(A: Matrix, B: Matrix) -> Matrix:
    """Element-wise product of two matrices: result[i, j] = A[i, j] * B[i, j]. This is not the matrix product. Raises ShapeMismatchError when the shapes differ."""
@overload
def hadamard(A: Matrixf, B: Matrixf) -> Matrixf:
    """Element-wise product of two matrices: result[i, j] = A[i, j] * B[i, j]. This is not the matrix product. Raises ShapeMismatchError when the shapes differ."""
@overload
def add_to_rows(A: Matrix, v: Vec) -> None:
    """Add the vector v to every row of A, in place (broadcasting). The size of v must equal the number of columns, otherwise SizeMismatchError."""
@overload
def add_to_rows(A: Matrixf, v: Vecf) -> None:
    """Add the vector v to every row of A, in place (broadcasting). The size of v must equal the number of columns, otherwise SizeMismatchError."""
@overload
def subtract_from_rows(A: Matrix, v: Vec) -> None:
    """Subtract the vector v from every row of A, in place (broadcasting). The size of v must equal the number of columns, otherwise SizeMismatchError."""
@overload
def subtract_from_rows(A: Matrixf, v: Vecf) -> None:
    """Subtract the vector v from every row of A, in place (broadcasting). The size of v must equal the number of columns, otherwise SizeMismatchError."""
@overload
def centre_columns(A: Matrix) -> Vec:
    """Make every column of A average 0, in place, by subtracting its mean. Returns the vector of column means. Example: the column [1, 2, 3] has mean 2 and becomes [-1, 0, 1]. First step of PCA."""
@overload
def centre_columns(A: Matrixf) -> Vecf:
    """Make every column of A average 0, in place, by subtracting its mean. Returns the vector of column means. Example: the column [1, 2, 3] has mean 2 and becomes [-1, 0, 1]. First step of PCA."""
def angle(x: Vec | Vecf, y: Vec | Vecf) -> float:
    """Angle between x and y in radians: arccos(x . y / (|x| |y|)). Example: angle(Vec([1, 0]), Vec([0, 1])) is pi / 2. Raises ZeroVectorError when a vector is all zeros."""
def allclose(x: Vec | Vecf | Matrix | Matrixf, y: Vec | Vecf | Matrix | Matrixf, rtol: float = ..., atol: float = ...) -> bool:
    """True when the two vectors (or the two matrices) have the same size (or shape) and every pair of elements is close, with relative tolerance rtol and absolute tolerance atol. Use it instead of == on floats. Like np.allclose."""
def approx_equal(a: float, b: float, rtol: float = ..., atol: float = ...) -> bool:
    """True when two plain numbers are approximately equal. Never compare floats with ==: 0.1 + 0.2 == 0.3 is False, but approx_equal(0.1 + 0.2, 0.3) is True."""

class YatricesError(Exception):
    """Base class of every error raised by the engine. Catch this to catch them all."""
class IndexError(YatricesError, builtins.IndexError):
    """An index is out of range (also a built-in IndexError). Use yt.IndexError, because 'from yatricespy import *' does not export it."""
class SizeMismatchError(YatricesError, ValueError):
    """Two vectors that must have the same size do not (also a ValueError)."""
class EmptyVectorError(YatricesError, ValueError):
    """mean, min or max of an empty vector (also a ValueError)."""
class WrongSizeError(YatricesError, ValueError):
    """A vector of the wrong size for the operation, for example cross needs size 3 (also a ValueError)."""
class ZeroVectorError(YatricesError, ValueError):
    """A vector of zeros where a direction is needed, in angle or project (also a ValueError)."""
class ShapeMismatchError(YatricesError, ValueError):
    """Two matrices whose shapes do not fit the operation, or a reshape that changes the element count (also a ValueError)."""
class NotSquareError(YatricesError, ValueError):
    """trace or transpose_in_place on a matrix that is not square (also a ValueError)."""
class RaggedRowsError(YatricesError, ValueError):
    """Rows of different lengths given to build a matrix (also a ValueError)."""
class EmptyMatrixError(YatricesError, ValueError):
    """mean, min, max or max_abs of an empty matrix (also a ValueError)."""
class AllocationSizeError(YatricesError, ValueError):
    """A size so large that it cannot exist in memory (also a ValueError)."""
class OutOfMemoryError(YatricesError, MemoryError):
    """The computer ran out of memory (also a MemoryError)."""

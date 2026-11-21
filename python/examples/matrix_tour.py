"""A short tour of yatricespy.Matrix. Run it with: python python/examples/matrix_tour.py"""
import yatricespy as yt

A = yt.Matrix([[1, 2, 3], [4, 5, 6]])
B = yt.Matrix(2, 3, 1.0)
print("A =")
print(A)
print("shape =", A.shape, " A[1, 2] =", A[1, 2], " A[-1, 0] =", A[-1, 0])

print("A + B =")
print(A + B)
print("2 * A =")
print(2 * A)
print("A transposed =")
print(A.T)
print("hadamard(A, A) =")
print(yt.hadamard(A, A))

print("sum =", A.sum(), " mean =", A.mean(), " max_abs =", A.max_abs())
print("sum_rows =", A.sum_rows(), " mean_cols =", A.mean_cols())

S = yt.Matrix([[2, 1, 0], [1, 3, 1], [0, 1, 4]])
print("trace(S) =", S.trace(), " diagonal(S) =", S.diagonal(), " above it =", S.diagonal(1))

S.add_row_multiple(1, 0, -0.5)
print("after row1 -= 0.5 * row0:")
print(S)

X = yt.Matrix([[1, 10], [2, 20], [3, 30]])
means = yt.centre_columns(X)
print("column means =", means, " centred X =")
print(X)

try:
    A + S
except yt.ShapeMismatchError as e:
    print("Error:", e)

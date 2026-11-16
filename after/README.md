# after/

Files for everything that comes after release 0.0.1.

Every file here is a **stub**: comments only. Each one lists the concepts that belong in it,
in the order to write them, with a suggested signature. Nothing in this folder is compiled,
so it cannot break the build.

Item numbers refer to my concept roadmap, `why_I_do_this.pdf` (version 4, 400 concepts). That PDF is kept on my computer and is not in the repository.

## When a stub becomes real

| Stub | Move it to |
|---|---|
| `after/cpp/x.hpp` | `cpp/include/yatrices/x.hpp` |
| `after/cpp/x.cpp` | `cpp/src/x.cpp` (and add it to `cpp/CMakeLists.txt`) |
| `after/python/bind_*.cpp` | `python/bindings/` |
| `after/python/linalg.py` | `python/yatricespy/` |
| `after/python/test_*.py` | `python/tests/` |
| other `after/python/*.py` | `python/examples/` |

## cpp/

| Files | Subject | Items |
|---|---|---|
| `banded.hpp` + `banded.cpp` | tridiagonal and banded matrices and their O(n) solvers | 4 |
| `bench.hpp` + `bench.cpp` | timing tools and benchmark programs | 4 |
| `calculus.hpp` + `calculus.cpp` | numerical derivatives, optimisation methods, automatic differentiation | 10 |
| `cholesky.hpp` + `cholesky.cpp` | Cholesky, LDLT and everything for symmetric positive definite matrices | 6 |
| `complexmat.hpp` + `complexmat.cpp` | complex vectors and matrices, DFT and FFT | 6 |
| `eigen.hpp` + `eigen.cpp` | eigenvalues and eigenvectors | 23 |
| `exact.hpp` + `exact.cpp` | exact arithmetic: integers, fractions, arithmetic modulo p | 4 |
| `expr.hpp` + `expr.cpp` | expression templates (lazy evaluation of a + b + c) | 1 |
| `factories.hpp` + `factories.cpp` | functions that build standard matrices (identity, zeros, random) | 1 |
| `fixed.hpp` + `fixed.cpp` | small matrices whose size is known at compile time | 1 |
| `geometry.hpp` + `geometry.cpp` | rotations, projections, transformations, distances | 13 |
| `io.hpp` + `io.cpp` | reading and writing files (CSV, .npy, Matrix Market) | 3 |
| `iterative.hpp` + `iterative.cpp` | iterative solvers for Ax = b (Jacobi, Gauss-Seidel, CG, ...) | 11 |
| `kernels.hpp` + `kernels.cpp` | low-level fast loops (cache blocking, SIMD); internal, not public API | 8 |
| `krylov.hpp` + `krylov.cpp` | Krylov methods (Lanczos, Arnoldi, GMRES) | 6 |
| `lstsq.hpp` + `lstsq.cpp` | least squares and regression | 16 |
| `lu.hpp` + `lu.cpp` | Gaussian elimination, LU decomposition, determinant, inverse | 15 |
| `matfun.hpp` + `matfun.cpp` | functions of a matrix (exponential, square root) and matrix equations | 10 |
| `memory.hpp` + `memory.cpp` | buffer pool for aligned allocations | 1 |
| `norms.hpp` + `norms.cpp` | matrix norms and condition numbers | 8 |
| `numerics.hpp` + `numerics.cpp` | floating-point tools (epsilon, error measures, safe formulas) | 7 |
| `parallel.hpp` + `parallel.cpp` | thread pool and parallel loops | 1 |
| `products.hpp` + `products.cpp` | matrix-vector and matrix-matrix products and their variants | 20 |
| `qr.hpp` + `qr.cpp` | Gram-Schmidt, Householder, Givens and QR decomposition | 11 |
| `random.hpp` + `random.cpp` | reproducible random numbers and random test matrices | 2 |
| `solve.hpp` + `solve.cpp` | general solvers, RREF, rank, null space, block systems | 12 |
| `sparse.hpp` + `sparse.cpp` | sparse matrix formats and sparse algorithms | 9 |
| `special.hpp` + `special.cpp` | named matrices used as examples and test cases | 14 |
| `stats.hpp` + `stats.cpp` | covariance, distances, kernels, multivariate normal | 7 |
| `structure.hpp` + `structure.cpp` | tests of structure (symmetric, triangular, ...) and structured types | 12 |
| `svd.hpp` + `svd.cpp` | singular value decomposition and what is built on it | 15 |
| `triangular.hpp` + `triangular.cpp` | triangular systems and triangular products | 5 |
| `update.hpp` + `update.cpp` | cheap updates of an inverse or a factorisation after a small change | 4 |
| `views.hpp` + `views.cpp` | non-owning views, slicing, stacking, memory layout | 4 |

## python/

| File | Item numbers |
|---|---|
| `attention.py` | 338, 154, 336 |
| `bench_vs_numpy.py` | 392 |
| `bind_errors.cpp` | 388 |
| `bind_linalg.cpp` | 391, 393 |
| `bind_matrix.cpp` | 386, 389, 390 |
| `bind_vec.cpp` | 70, 387 |
| `gaussian_process.py` | 325 |
| `gnn.py` | 340 |
| `image_compression.py` | 322 |
| `kalman_filter.py` | 333 |
| `kernel_ridge.py` | 324 |
| `kmeans.py` | 326 |
| `knn.py` | 327 |
| `lasso.py` | 136 |
| `lda.py` | 157 |
| `linalg.py` | 394 |
| `linear_regression.py` | 259 |
| `logistic_regression.py` | 329, 137 |
| `lora.py` | 337 |
| `lsa.py` | 339 |
| `markov_chain.py` | 312 |
| `mds.py` | 335 |
| `neural_network.py` | 100, 101, 332, 330, 155 |
| `nmf.py` | 334 |
| `optimisers.py` | 331 |
| `pagerank.py` | 311 |
| `pca.py` | 60 |
| `polynomial_fit.py` | 272 |
| `recommender.py` | 152 |
| `ridge.py` | 328 |
| `spectral_clustering.py` | 156 |
| `test_vs_numpy.py` | 395 |

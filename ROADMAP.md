# Roadmap: the next ten algorithms

These are the next ten things I'm planning to build, roughly in order. Each one leans on the ones before it, so the order matters more than the dates (there aren't any dates).

Every algorithm will come with C++ code, Python bindings, tests against NumPy and, eventually, a short tutorial in `docs/`. The stub files in [`after/cpp/`](after/cpp/) already sketch where each one will live.

| # | Algorithm | What it does | Needs | Difficulty |
|---|---|---|---|---|
| 1 | Matrix-vector product | computes $A\mathbf{x}$ | the `Matrix` class | ★☆☆☆☆ |
| 2 | Matrix-matrix product | computes $AB$, first naively, then with a cache-friendly loop order | 1 | ★★☆☆☆ |
| 3 | Triangular solves | solves $L\mathbf{x} = \mathbf{b}$ and $U\mathbf{x} = \mathbf{b}$ by forward and back substitution | 1 | ★★☆☆☆ |
| 4 | LU decomposition with partial pivoting | writes $PA = LU$, the workhorse behind solving $A\mathbf{x} = \mathbf{b}$ | 3 | ★★★☆☆ |
| 5 | Determinant and inverse | both come almost for free once LU exists | 4 | ★★☆☆☆ |
| 6 | Cholesky decomposition | writes $A = LL^\top$ for symmetric positive definite matrices, about twice as fast as LU | 3 | ★★★☆☆ |
| 7 | QR decomposition (Gram-Schmidt, then Householder) | writes $A = QR$, with $Q$ orthogonal and $R$ upper triangular | 2, 3 | ★★★★☆ |
| 8 | Least squares and linear regression | finds the best-fitting $\mathbf{x}$ when $A\mathbf{x} = \mathbf{b}$ has no exact solution: first with the normal equation, then with QR | 6, 7 | ★★★☆☆ |
| 9 | Power iteration | finds the largest eigenvalue of a matrix and its eigenvector | 1 | ★★★☆☆ |
| 10 | Singular value decomposition (SVD) | writes $A = U\Sigma V^\top$, the basis of PCA, image compression and much more | 7, 9 | ★★★★★ |

## Why these ten

- **1 to 3** are the foundations. Nearly everything else is built out of products and triangular solves.
- **4 to 6** solve linear systems, which turn up all over machine learning.
- **7 and 8** lead to the first real application: linear regression, done properly.
- **9 and 10** open the door to eigenvalues and the SVD, which is where linear algebra gets genuinely interesting, and where PCA comes from.

## What happens after that

The longer list (iterative solvers, sparse matrices, the full eigenvalue toolbox and more) lives in the stubs in [`after/`](after/). This page will move on as each algorithm gets done.

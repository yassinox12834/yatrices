// after/cpp/svd.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: singular value decomposition and what is built on it.
//
// When you start it: move this file to cpp/include/yatrices/svd.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/svd.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 15 item(s).
//
// [ ] Item 314: SVD of a 2x2 in closed form
//       difficulty 4/10, stage 8, needs first: 293
//       place: free function in namespace yatrices
//       suggested:
//           SVD<T> svd_2x2(T a, T b, T c, T d)
//
// [ ] Item 55: SVD via eigen-decomposition
//       difficulty 6/10, stage 8, needs first: 48
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct SVD { Matrix<T> U
//           Vec<T> S
//           Matrix<T> Vt
//           }
//           SVD<T> svd_via_eigh(const Matrix<T>& A)
//
// [ ] Item 59: Low-rank approximation
//       difficulty 3/10, stage 8, needs first: 55
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> low_rank(const SVD<T>& s, std::size_t k)
//
// [ ] Item 147: Condition number and rank from SVD
//       difficulty 3/10, stage 8, needs first: 55
//       place: free function in namespace yatrices
//       suggested:
//           T cond2(const SVD<T>& s)
//           std::size_t rank(const SVD<T>& s, T tol)
//
// [ ] Item 313: Thin, full and truncated SVD
//       difficulty 3/10, stage 8, needs first: 55
//       place: free function in namespace yatrices
//       suggested:
//           SVD<T> svd(const Matrix<T>& A, bool full_matrices = false)
//           SVD<T> truncate(const SVD<T>& s, std::size_t k)
//
// [ ] Item 317: Stable rank
//       difficulty 3/10, stage 8, needs first: 147
//       place: free function in namespace yatrices
//       suggested:
//           T stable_rank(const SVD<T>& s)
//
// [ ] Item 315: The four fundamental subspaces
//       difficulty 4/10, stage 8, needs first: 55, 30
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct Subspaces { Matrix<T> column, row, null, left_null
//           }
//           Subspaces<T> subspaces(const SVD<T>& s, T tol)
//
// [ ] Item 58: Pseudo-inverse
//       difficulty 5/10, stage 8, needs first: 55
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> pinv(const Matrix<T>& A, T rcond)
//
// [ ] Item 92: Orthogonal Procrustes
//       difficulty 6/10, stage 8, needs first: 55
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> procrustes(const Matrix<T>& A, const Matrix<T>& B)
//
// [ ] Item 149: Polar decomposition
//       difficulty 6/10, stage 8, needs first: 55
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct Polar { Matrix<T> U, P
//           }
//           Polar<T> polar(const Matrix<T>& A)
//
// [ ] Item 319: Angles between subspaces
//       difficulty 6/10, stage 8, needs first: 38, 55
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> principal_angles(const Matrix<T>& Q1, const Matrix<T>& Q2)
//
// [ ] Item 56: SVD via one-sided Jacobi
//       difficulty 7/10, stage 8, needs first: 40
//       place: free function in namespace yatrices
//       suggested:
//           SVD<T> svd_jacobi(Matrix<T> A, T tol)
//
// [ ] Item 151: Randomised SVD
//       difficulty 7/10, stage 8, needs first: 38, 59
//       place: free function in namespace yatrices
//       suggested:
//           SVD<T> svd_randomised(const Matrix<T>& A, std::size_t k, std::size_t oversample, Rng& rng)
//
// [ ] Item 318: Bidiagonalisation
//       difficulty 7/10, stage 8, needs first: 39
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct Bidiagonal { Vec<T> diag, super
//           Matrix<T> U, Vt
//           }
//           Bidiagonal<T> bidiagonalise(Matrix<T> A)
//
// [ ] Item 57: SVD via Golub-Kahan
//       difficulty 10/10, stage 14, needs first: 49, 51
//       place: free function in namespace yatrices
//       suggested:
//           SVD<T> svd_golub_kahan(Matrix<T> A)
//

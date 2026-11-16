// after/cpp/cholesky.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: Cholesky, LDLT and everything for symmetric positive definite matrices.
//
// When you start it: move this file to cpp/include/yatrices/cholesky.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/cholesky.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 6 item(s).
//
// [ ] Item 31: Cholesky decomposition
//       difficulty 5/10, stage 4, needs first: 21
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> cholesky(const Matrix<T>& A) (throws NotPositiveDefiniteError)
//           Vec<T> cholesky_solve(const Matrix<T>& L, const Vec<T>& b)
//
// [ ] Item 78: Positive definiteness test
//       difficulty 3/10, stage 4, needs first: 25, 31
//       place: free function in namespace yatrices
//       suggested:
//           bool is_positive_definite(const Matrix<T>& A)
//
// [ ] Item 83: LDL^T decomposition
//       difficulty 5/10, stage 4, needs first: 31
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct LDLT { Matrix<T> L
//           Vec<T> d
//           }
//           LDLT<T> ldlt(const Matrix<T>& A)
//
// [ ] Item 284: Pivoted Cholesky
//       difficulty 6/10, stage 6, needs first: 31
//       place: free function in namespace yatrices
//       suggested:
//           PivotedCholesky<T> cholesky_pivoted(Matrix<T> A, T tol)
//
// [ ] Item 307: Inertia
//       difficulty 5/10, stage 7, needs first: 83
//       place: result struct plus free function
//       suggested:
//           struct Inertia { std::size_t positive, negative, zero
//           }
//           Inertia inertia(const LDLT<T>& f, T tol)
//
// [ ] Item 355: Blocked Cholesky
//       difficulty 6/10, stage 11, needs first: 31, 353, 238
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> cholesky_blocked(Matrix<T> A, std::size_t block)
//

// after/cpp/solve.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: general solvers, RREF, rank, null space, block systems.
//
// When you start it: move this file to cpp/include/yatrices/solve.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/solve.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 12 item(s).
//
// [ ] Item 79: Adjugate and Cramer's rule
//       difficulty 3/10, stage 3, needs first: 26
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> adjugate(const Matrix<T>& A)
//           Vec<T> solve_cramer(const Matrix<T>& A, const Vec<T>& b)
//
// [ ] Item 28: Reduced row echelon form
//       difficulty 4/10, stage 3, needs first: 22
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> rref(Matrix<T> A, T tol)
//
// [ ] Item 243: Schur complement
//       difficulty 4/10, stage 3, needs first: 73, 24
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> schur_complement(const Matrix<T>& A, const Matrix<T>& B, const Matrix<T>& C, const Matrix<T>& D)
//
// [ ] Item 27: Inverse (Gauss-Jordan)
//       difficulty 5/10, stage 3, needs first: 23
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> inverse_gauss_jordan(Matrix<T> A)
//
// [ ] Item 29: Rank
//       difficulty 5/10, stage 3, needs first: 28
//       place: free function in namespace yatrices
//       suggested:
//           std::size_t rank(const Matrix<T>& A, T tol)
//
// [ ] Item 30: Null space and column space
//       difficulty 6/10, stage 3, needs first: 28
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> null_space(const Matrix<T>& A, T tol)
//           Matrix<T> column_space(const Matrix<T>& A, T tol)
//
// [ ] Item 253: One solve() that picks the method
//       difficulty 4/10, stage 4, needs first: 24, 31, 14
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> solve(const Matrix<T>& A, const Vec<T>& b)
//           Matrix<T> solve(const Matrix<T>& A, const Matrix<T>& B)
//
// [ ] Item 123: Row and column equilibration
//       difficulty 4/10, stage 4, needs first: 22, 32
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct Scaling { Vec<T> row, col
//           }
//           Scaling<T> equilibrate(Matrix<T>& A)
//
// [ ] Item 82: Iterative refinement
//       difficulty 5/10, stage 4, needs first: 24, 32
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> refine(const Matrix<T>& A, const LU<T>& f, const Vec<T>& b, Vec<T> x, int steps)
//
// [ ] Item 285: Saddle-point (KKT) systems
//       difficulty 6/10, stage 6, needs first: 243, 31
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> solve_kkt(const Matrix<T>& A, const Matrix<T>& B, const Vec<T>& f, const Vec<T>& g)
//
// [ ] Item 287: Vandermonde solve (Björck-Pereyra)
//       difficulty 6/10, stage 6, needs first: 74
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> solve_vandermonde(const Vec<T>& x, const Vec<T>& f)
//
// [ ] Item 361: Mixed-precision refinement
//       difficulty 6/10, stage 11, needs first: 82
//       place: free function in namespace yatrices
//       suggested:
//           Vec<double> solve_mixed(const Matrix<double>& A, const Vec<double>& b, int max_steps)
//

// after/cpp/norms.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: matrix norms and condition numbers.
//
// When you start it: move this file to cpp/include/yatrices/norms.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/norms.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 8 item(s).
//
// [ ] Item 17: Frobenius norm
//       difficulty 1/10, stage 1, needs first: 1
//       place: free function in namespace yatrices
//       suggested:
//           T norm_fro(const Matrix<T>& A)
//
// [ ] Item 108: Frobenius inner product
//       difficulty 1/10, stage 2, needs first: 3, 7, 9
//       place: free function in namespace yatrices
//       suggested:
//           T inner_fro(const Matrix<T>& A, const Matrix<T>& B)
//           T distance_fro(const Matrix<T>& A, const Matrix<T>& B)
//
// [ ] Item 18: Induced matrix norms (1,inf)
//       difficulty 2/10, stage 4, needs first: 15
//       place: free function in namespace yatrices
//       suggested:
//           T norm1(const Matrix<T>& A)
//           T norm_inf(const Matrix<T>& A)
//
// [ ] Item 32: Condition number
//       difficulty 5/10, stage 4, needs first: 18, 27
//       place: free function in namespace yatrices
//       suggested:
//           T cond1(const Matrix<T>& A)
//
// [ ] Item 251: Skeel condition number
//       difficulty 5/10, stage 4, needs first: 32, 123
//       place: free function in namespace yatrices
//       suggested:
//           T cond_skeel(const Matrix<T>& A)
//
// [ ] Item 122: Hager condition estimator
//       difficulty 7/10, stage 6, needs first: 18, 24
//       place: free function in namespace yatrices
//       suggested:
//           T cond1_estimate(const Matrix<T>& A, const LU<T>& f)
//
// [ ] Item 19: Spectral norm |A|_2
//       difficulty 7/10, stage 7, needs first: 3, 7, 45
//       place: free function in namespace yatrices
//       suggested:
//           T norm2(const Matrix<T>& A, T tol, int max_iter)
//
// [ ] Item 148: Nuclear norm
//       difficulty 4/10, stage 8, needs first: 56
//       place: free function in namespace yatrices
//       suggested:
//           T norm_nuclear(const Matrix<T>& A)
//

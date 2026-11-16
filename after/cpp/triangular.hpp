// after/cpp/triangular.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: triangular systems and triangular products.
//
// When you start it: move this file to cpp/include/yatrices/triangular.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/triangular.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 5 item(s).
//
// [ ] Item 203: Triangular matrix-vector product
//       difficulty 2/10, stage 2, needs first: 6, 14
//       place: free function in namespace yatrices
//       suggested:
//           void trmv_lower(const Matrix<T>& L, Vec<T>& x)
//           void trmv_upper(const Matrix<T>& U, Vec<T>& x)
//
// [ ] Item 21: Forward and back substitution
//       difficulty 2/10, stage 3, needs first: 6
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> solve_lower(const Matrix<T>& L, const Vec<T>& b)
//           Vec<T> solve_upper(const Matrix<T>& U, const Vec<T>& b)
//
// [ ] Item 238: Triangular solve with many right-hand sides
//       difficulty 3/10, stage 3, needs first: 21
//       place: free function in namespace yatrices
//       suggested:
//           void trsm_lower(const Matrix<T>& L, Matrix<T>& B)
//           void trsm_upper(const Matrix<T>& U, Matrix<T>& B)
//
// [ ] Item 239: Inverse of a triangular matrix
//       difficulty 3/10, stage 3, needs first: 21
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> inverse_lower(const Matrix<T>& L)
//           Matrix<T> inverse_upper(const Matrix<T>& U)
//
// [ ] Item 240: Triangular wrapper types
//       difficulty 3/10, stage 3, needs first: 21
//       place: new class or type
//       suggested:
//           template <typename T> struct Lower { const Matrix<T>& m
//           }
//           template <typename T> struct Upper { const Matrix<T>& m
//           }
//           Vec<T> solve(Lower<T> L, const Vec<T>& b)
//

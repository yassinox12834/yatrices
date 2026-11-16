// after/cpp/matfun.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: functions of a matrix (exponential, square root) and matrix equations.
//
// When you start it: move this file to cpp/include/yatrices/matfun.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/matfun.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 10 item(s).
//
// [ ] Item 90: Matrix square root (SPD)
//       difficulty 5/10, stage 7, needs first: 48
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> sqrtm_spd(const Matrix<T>& A)
//
// [ ] Item 91: Matrix function f(A)
//       difficulty 5/10, stage 7, needs first: 53
//       place: free function in namespace yatrices
//       suggested:
//           template <typename T, typename F> Matrix<T> funm_sym(const Matrix<T>& A, F f)
//
// [ ] Item 310: Exponential and logarithm of a symmetric matrix
//       difficulty 4/10, stage 7, needs first: 91
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> expm_sym(const Matrix<T>& A)
//           Matrix<T> logm_spd(const Matrix<T>& A)
//
// [ ] Item 379: Inverse by Newton-Schulz iteration
//       difficulty 5/10, stage 12, needs first: 7, 27
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> inverse_newton_schulz(const Matrix<T>& A, T tol, int max_iter)
//
// [ ] Item 381: Matrix polynomials
//       difficulty 5/10, stage 12, needs first: 13, 180
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> polyvalm(const Vec<T>& coeffs, const Matrix<T>& A)
//
// [ ] Item 380: Square root by Denman-Beavers iteration
//       difficulty 6/10, stage 12, needs first: 27, 90
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> sqrtm_iterative(const Matrix<T>& A, T tol, int max_iter)
//
// [ ] Item 383: Sylvester equation
//       difficulty 6/10, stage 12, needs first: 61, 24, 208
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> solve_sylvester(const Matrix<T>& A, const Matrix<T>& B, const Matrix<T>& C)
//
// [ ] Item 384: Lyapunov equation
//       difficulty 6/10, stage 12, needs first: 383
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> solve_lyapunov(const Matrix<T>& A, const Matrix<T>& Q)
//
// [ ] Item 382: Padé approximant of the exponential
//       difficulty 7/10, stage 12, needs first: 27, 381
//       place: internal kernel, not in the public API
//       suggested:
//           Matrix<T> pade_exp(const Matrix<T>& A, int order)
//
// [ ] Item 66: Matrix exponential
//       difficulty 8/10, stage 12, needs first: 13, 27
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> expm(const Matrix<T>& A)
//

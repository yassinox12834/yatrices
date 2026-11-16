// after/cpp/numerics.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: floating-point tools (epsilon, error measures, safe formulas).
//
// When you start it: move this file to cpp/include/yatrices/numerics.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/numerics.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 7 item(s).
//
// [ ] Item 117: Machine epsilon
//       difficulty 2/10, stage 2, needs first: 1
//       place: free function in namespace yatrices
//       suggested:
//           template <typename T> T machine_epsilon() (by halving
//           compare with std::numeric_limits<T>::epsilon())
//
// [ ] Item 231: Anatomy of a float
//       difficulty 2/10, stage 2, needs first: 117
//       place: free function in namespace yatrices
//       suggested:
//           std::string bits(double x)
//           template <typename T> T ulp(T x)
//
// [ ] Item 232: Catastrophic cancellation
//       difficulty 2/10, stage 2, needs first: 117
//       place: free function in namespace yatrices
//       suggested:
//           std::pair<T, T> quadratic_roots_stable(T a, T b, T c)
//
// [ ] Item 234: Safe hypotenuse
//       difficulty 2/10, stage 2, needs first: 16
//       place: free function in namespace yatrices
//       suggested:
//           template <typename T> T hypot_safe(T a, T b) noexcept
//
// [ ] Item 244: Default tolerance for rank decisions
//       difficulty 3/10, stage 3, needs first: 29, 117, 17
//       place: free function in namespace yatrices
//       suggested:
//           template <typename T> T default_rank_tol(const Matrix<T>& A)
//
// [ ] Item 119: Relative and backward error
//       difficulty 3/10, stage 4, needs first: 15, 24
//       place: free function in namespace yatrices
//       suggested:
//           T backward_error(const Matrix<T>& A, const Vec<T>& x, const Vec<T>& b)
//
// [ ] Item 250: Dot product with an error bound
//       difficulty 5/10, stage 4, needs first: 118
//       place: free function in namespace yatrices
//       suggested:
//           std::pair<T, T> dot_with_bound(const Vec<T>& x, const Vec<T>& y)
//

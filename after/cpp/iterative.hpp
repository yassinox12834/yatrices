// after/cpp/iterative.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: iterative solvers for Ax = b (Jacobi, Gauss-Seidel, CG, ...).
//
// When you start it: move this file to cpp/include/yatrices/iterative.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/iterative.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 11 item(s).
//
// [ ] Item 279: Stopping rules and result type
//       difficulty 2/10, stage 4, needs first: 15
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct IterResult { Vec<T> x
//           int iters
//           T residual
//           bool converged
//           }
//
// [ ] Item 130: Gradient descent on a quadratic
//       difficulty 3/10, stage 4, needs first: 6, 32
//       place: free function in namespace yatrices
//       suggested:
//           IterResult<T> gradient_descent_quadratic(const Matrix<T>& A, const Vec<T>& b, T step, int max_iter)
//
// [ ] Item 278: Steepest descent with exact line search
//       difficulty 3/10, stage 6, needs first: 130
//       place: free function in namespace yatrices
//       suggested:
//           IterResult<T> steepest_descent(const Matrix<T>& A, const Vec<T>& b, T tol, int max_iter)
//
// [ ] Item 280: Richardson iteration
//       difficulty 3/10, stage 6, needs first: 6
//       place: free function in namespace yatrices
//       suggested:
//           IterResult<T> richardson(const Matrix<T>& A, const Vec<T>& b, T omega, T tol, int max_iter)
//
// [ ] Item 281: Kaczmarz method
//       difficulty 4/10, stage 6, needs first: 36
//       place: free function in namespace yatrices
//       suggested:
//           IterResult<T> kaczmarz(const Matrix<T>& A, const Vec<T>& b, int sweeps, Rng* rng = nullptr)
//
// [ ] Item 33: Iterative solvers
//       difficulty 5/10, stage 6, needs first: 6
//       place: free function in namespace yatrices
//       suggested:
//           IterResult<T> jacobi(const Matrix<T>& A, const Vec<T>& b, T tol, int max_iter)
//           IterResult<T> gauss_seidel(same arguments)
//
// [ ] Item 127: SOR
//       difficulty 5/10, stage 6, needs first: 33
//       place: free function in namespace yatrices
//       suggested:
//           IterResult<T> sor(const Matrix<T>& A, const Vec<T>& b, T omega, T tol, int max_iter)
//
// [ ] Item 34: Conjugate gradient
//       difficulty 7/10, stage 6, needs first: 6, 33
//       place: free function in namespace yatrices
//       suggested:
//           IterResult<T> conjugate_gradient(const Matrix<T>& A, const Vec<T>& b, T tol, int max_iter)
//
// [ ] Item 129: Matrix-free operators
//       difficulty 4/10, stage 6, needs first: 6, 34
//       place: new class or type
//       suggested:
//           template <typename T> struct LinearOperator { std::size_t n
//           std::function<void(const Vec<T>& x, Vec<T>& y)> apply
//           }
//
// [ ] Item 128: Preconditioned conjugate gradient
//       difficulty 7/10, stage 6, needs first: 34
//       place: free function in namespace yatrices
//       suggested:
//           template <typename T, typename P> IterResult<T> pcg(const Matrix<T>& A, const Vec<T>& b, P apply_inverse_M, T tol, int max_iter)
//
// [ ] Item 347: Two-grid method
//       difficulty 8/10, stage 10, needs first: 33, 98
//       place: free function in namespace yatrices
//       suggested:
//           IterResult<T> two_grid(const Vec<T>& b, int pre_smooth, int post_smooth, T tol, int max_cycles)
//

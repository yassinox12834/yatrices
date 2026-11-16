// after/cpp/calculus.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: numerical derivatives, optimisation methods, automatic differentiation.
//
// When you start it: move this file to cpp/include/yatrices/calculus.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/calculus.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 10 item(s).
//
// [ ] Item 261: Numerical gradient and Jacobian
//       difficulty 3/10, stage 4, needs first: 117
//       place: free function in namespace yatrices
//       suggested:
//           template <typename T, typename F> Vec<T> gradient_fd(F f, const Vec<T>& x)
//           template <typename T, typename F> Matrix<T> jacobian_fd(F f, const Vec<T>& x)
//
// [ ] Item 263: Gradient check
//       difficulty 3/10, stage 4, needs first: 261
//       place: free function in namespace yatrices
//       suggested:
//           T gradient_check(const Vec<T>& analytic, const Vec<T>& numeric)
//
// [ ] Item 131: Newton's method for systems
//       difficulty 4/10, stage 4, needs first: 24
//       place: free function in namespace yatrices
//       suggested:
//           template <typename T, typename F, typename J> Vec<T> newton(F f, J jacobian, Vec<T> x0, T tol, int max_iter)
//
// [ ] Item 262: Numerical Hessian
//       difficulty 4/10, stage 4, needs first: 261
//       place: free function in namespace yatrices
//       suggested:
//           template <typename T, typename F> Matrix<T> hessian_fd(F f, const Vec<T>& x)
//
// [ ] Item 265: Backtracking line search
//       difficulty 3/10, stage 4, needs first: 130
//       place: free function in namespace yatrices
//       suggested:
//           template <typename T, typename F> T backtracking(F f, const Vec<T>& x, const Vec<T>& d, const Vec<T>& grad, T c)
//
// [ ] Item 266: Newton's method for minimisation
//       difficulty 5/10, stage 4, needs first: 262, 31
//       place: free function in namespace yatrices
//       suggested:
//           template <typename T, typename F, typename G, typename H> Vec<T> newton_minimise(F f, G grad, H hess, Vec<T> x0, T tol)
//
// [ ] Item 135: Gauss-Newton and Levenberg-Marquardt
//       difficulty 6/10, stage 5, needs first: 42, 131
//       place: free function in namespace yatrices
//       suggested:
//           template <typename T, typename R, typename J> Vec<T> levenberg_marquardt(R residual, J jacobian, Vec<T> x0, T lambda0, int max_iter)
//
// [ ] Item 292: BFGS quasi-Newton
//       difficulty 7/10, stage 6, needs first: 266, 84, 265
//       place: free function in namespace yatrices
//       suggested:
//           template <typename T, typename F, typename G> Vec<T> bfgs(F f, G grad, Vec<T> x0, T tol, int max_iter)
//
// [ ] Item 342: Reverse-mode automatic differentiation
//       difficulty 8/10, stage 9, needs first: 155
//       place: new class or type
//       suggested:
//           class Tape { std::vector<Node> nodes
//           public: Var matmul(Var a, Var b)
//           Var relu(Var a)
//           void backward(Var loss)
//           }
//
// [ ] Item 385: Forward-mode automatic differentiation
//       difficulty 6/10, stage 12, needs first: 69
//       place: new class or type
//       suggested:
//           template <typename T> struct Dual { T value, deriv
//           }
//           Dual<T> operator*(Dual<T> a, Dual<T> b)
//           Dual<T> sin(Dual<T> a)
//

// after/cpp/krylov.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: Krylov methods (Lanczos, Arnoldi, GMRES).
//
// When you start it: move this file to cpp/include/yatrices/krylov.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/krylov.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 6 item(s).
//
// [ ] Item 346: CG on the normal equations (CGLS)
//       difficulty 7/10, stage 10, needs first: 34, 41, 190
//       place: free function in namespace yatrices
//       suggested:
//           IterResult<T> cgls(const Matrix<T>& A, const Vec<T>& b, T tol, int max_iter)
//
// [ ] Item 144: Lanczos iteration
//       difficulty 8/10, stage 10, needs first: 34, 47
//       place: free function in namespace yatrices
//       suggested:
//           Tridiagonal<T> lanczos(const LinearOperator<T>& A, const Vec<T>& v0, std::size_t k)
//
// [ ] Item 145: Arnoldi iteration
//       difficulty 8/10, stage 10, needs first: 37, 45
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct Arnoldi { Matrix<T> Q, H
//           }
//           Arnoldi<T> arnoldi(const LinearOperator<T>& A, const Vec<T>& v0, std::size_t k)
//
// [ ] Item 146: GMRES
//       difficulty 8/10, stage 10, needs first: 42, 145
//       place: free function in namespace yatrices
//       suggested:
//           IterResult<T> gmres(const LinearOperator<T>& A, const Vec<T>& b, std::size_t restart, T tol, int max_iter)
//
// [ ] Item 344: Lanczos with reorthogonalisation
//       difficulty 8/10, stage 10, needs first: 144
//       place: free function in namespace yatrices
//       suggested:
//           Tridiagonal<T> lanczos_reorth(const LinearOperator<T>& A, const Vec<T>& v0, std::size_t k)
//
// [ ] Item 345: BiCGSTAB
//       difficulty 8/10, stage 10, needs first: 34, 146
//       place: free function in namespace yatrices
//       suggested:
//           IterResult<T> bicgstab(const LinearOperator<T>& A, const Vec<T>& b, T tol, int max_iter)
//

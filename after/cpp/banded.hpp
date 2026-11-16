// after/cpp/banded.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: tridiagonal and banded matrices and their O(n) solvers.
//
// When you start it: move this file to cpp/include/yatrices/banded.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/banded.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 4 item(s).
//
// [ ] Item 35: Tridiagonal solver (Thomas)
//       difficulty 3/10, stage 3, needs first: 22
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> solve_tridiagonal(const Vec<T>& lower, const Vec<T>& diag, const Vec<T>& upper, const Vec<T>& b)
//
// [ ] Item 249: Symmetric tridiagonal type
//       difficulty 2/10, stage 3, needs first: 35
//       place: new class or type
//       suggested:
//           template <typename T> class SymTridiagonal { Vec<T> diag, off
//           }
//           Vec<T> matvec(const SymTridiagonal<T>& A, const Vec<T>& x)
//
// [ ] Item 86: Banded matrices
//       difficulty 5/10, stage 6, needs first: 24, 35
//       place: new class or type
//       suggested:
//           template <typename T> class BandMatrix { std::size_t n, kl, ku
//           Matrix<T> ab
//           }
//           Vec<T> band_solve(BandMatrix<T> A, const Vec<T>& b)
//
// [ ] Item 286: Cyclic tridiagonal solver
//       difficulty 5/10, stage 6, needs first: 35, 84
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> solve_cyclic_tridiagonal(const Vec<T>& lower, const Vec<T>& diag, const Vec<T>& upper, const Vec<T>& b)
//

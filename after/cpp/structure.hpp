// after/cpp/structure.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: tests of structure (symmetric, triangular, ...) and structured types.
//
// When you start it: move this file to cpp/include/yatrices/structure.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/structure.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 12 item(s).
//
// [ ] Item 109: Symmetric and antisymmetric parts
//       difficulty 1/10, stage 1, needs first: 2, 3
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> symmetric_part(const Matrix<T>& A)
//           Matrix<T> antisymmetric_part(const Matrix<T>& A)
//
// [ ] Item 187: Triangular parts
//       difficulty 1/10, stage 1, needs first: 1
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> tril(const Matrix<T>& A, std::ptrdiff_t k = 0)
//           Matrix<T> triu(const Matrix<T>& A, std::ptrdiff_t k = 0)
//
// [ ] Item 14: Structure tests
//       difficulty 2/10, stage 1, needs first: 1
//       place: free function in namespace yatrices
//       suggested:
//           bool is_symmetric(const Matrix<T>& A, T tol)
//           is_diagonal(A, tol)
//           is_upper_triangular(A, tol)
//           is_orthogonal(A, tol)
//
// [ ] Item 71: Elementary matrices
//       difficulty 2/10, stage 2, needs first: 7
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> elementary_swap<T>(n, i, j)
//           elementary_scale<T>(n, i, alpha)
//           elementary_add<T>(n, i, j, alpha)
//
// [ ] Item 72: Permutation matrices
//       difficulty 2/10, stage 2, needs first: 7, 14
//       place: new class or type
//       suggested:
//           class Permutation { std::vector<std::size_t> index
//           Permutation inverse() const
//           }
//           Matrix<T> permute_rows(const Permutation& p, const Matrix<T>& A)
//
// [ ] Item 76: Idempotent and nilpotent tests
//       difficulty 2/10, stage 2, needs first: 13, 14
//       place: free function in namespace yatrices
//       suggested:
//           bool is_idempotent(const Matrix<T>& A, T tol)
//           bool is_nilpotent(const Matrix<T>& A, T tol)
//
// [ ] Item 211: Involutory, normal and skew-symmetric tests
//       difficulty 2/10, stage 2, needs first: 13, 14
//       place: free function in namespace yatrices
//       suggested:
//           bool is_involutory(const Matrix<T>& A, T tol)
//           bool is_normal(const Matrix<T>& A, T tol)
//           bool is_skew_symmetric(const Matrix<T>& A, T tol)
//
// [ ] Item 212: Diagonal dominance test
//       difficulty 2/10, stage 2, needs first: 1
//       place: free function in namespace yatrices
//       suggested:
//           bool is_diagonally_dominant(const Matrix<T>& A, bool strict)
//
// [ ] Item 213: Bandwidth detection
//       difficulty 2/10, stage 2, needs first: 14
//       place: free function in namespace yatrices
//       suggested:
//           std::pair<std::size_t, std::size_t> bandwidth(const Matrix<T>& A, T tol)
//
// [ ] Item 214: Diagonal matrix type
//       difficulty 2/10, stage 2, needs first: 7
//       place: new class or type
//       suggested:
//           template <typename T> class Diagonal { Vec<T> d
//           }
//           Matrix<T> operator*(const Diagonal<T>& D, const Matrix<T>& A)
//           Matrix<T> operator*(const Matrix<T>& A, const Diagonal<T>& D)
//
// [ ] Item 107: Packed storage
//       difficulty 3/10, stage 2, needs first: 14
//       place: new class or type
//       suggested:
//           template <typename T> class SymmetricPacked { std::size_t n
//           Vec<T> data
//           T& operator()(std::size_t i, std::size_t j)
//           }
//
// [ ] Item 99: Stochastic matrix checks
//       difficulty 2/10, stage 7, needs first: 45
//       place: free function in namespace yatrices
//       suggested:
//           bool is_row_stochastic(const Matrix<T>& P, T tol)
//           Vec<T> steady_state(const Matrix<T>& P)
//

// after/cpp/lu.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: Gaussian elimination, LU decomposition, determinant, inverse.
//
// When you start it: move this file to cpp/include/yatrices/lu.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/lu.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 15 item(s).
//
// [ ] Item 235: Closed-form 2x2 and 3x3 determinant and inverse
//       difficulty 1/10, stage 3, needs first: 1
//       place: free function in namespace yatrices
//       suggested:
//           T det2(const Matrix<T>& A)
//           T det3(const Matrix<T>& A)
//           Matrix<T> inverse2(const Matrix<T>& A)
//
// [ ] Item 26: Determinant via cofactors
//       difficulty 3/10, stage 3, needs first: 1
//       place: free function in namespace yatrices
//       suggested:
//           T det_cofactor(const Matrix<T>& A)
//
// [ ] Item 22: Gaussian elimination
//       difficulty 4/10, stage 3, needs first: 21
//       place: free function in namespace yatrices
//       suggested:
//           void eliminate(Matrix<T>& A, Vec<T>& b)
//
// [ ] Item 236: LU without pivoting
//       difficulty 3/10, stage 3, needs first: 22
//       place: free function in namespace yatrices
//       suggested:
//           LU<T> lu_nopivot(Matrix<T> A)
//
// [ ] Item 23: Partial pivoting
//       difficulty 5/10, stage 3, needs first: 22
//       place: internal kernel, not in the public API
//       suggested:
//           std::size_t pivot_row(const Matrix<T>& A, std::size_t k)
//
// [ ] Item 24: LU decomposition
//       difficulty 5/10, stage 3, needs first: 23
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct LU { Matrix<T> lu
//           std::vector<std::size_t> perm
//           int sign
//           }
//           LU<T> lu_factor(Matrix<T> A)
//           Vec<T> lu_solve(const LU<T>& f, const Vec<T>& b)
//
// [ ] Item 25: Determinant via LU
//       difficulty 3/10, stage 3, needs first: 24
//       place: free function in namespace yatrices
//       suggested:
//           T det(const Matrix<T>& A)
//           T det(const LU<T>& f)
//
// [ ] Item 242: Sign and log-determinant
//       difficulty 2/10, stage 3, needs first: 25
//       place: free function in namespace yatrices
//       suggested:
//           std::pair<int, T> slogdet(const Matrix<T>& A)
//
// [ ] Item 124: Several right-hand sides
//       difficulty 3/10, stage 3, needs first: 24
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> lu_solve(const LU<T>& f, const Matrix<T>& B)
//
// [ ] Item 237: Compact LU storage
//       difficulty 3/10, stage 3, needs first: 24
//       place: internal kernel, not in the public API
//       suggested:
//           Matrix<T> unpack_l(const LU<T>& f)
//           Matrix<T> unpack_u(const LU<T>& f)
//
// [ ] Item 241: Transposed solve from the same LU
//       difficulty 3/10, stage 3, needs first: 24
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> lu_solve_transposed(const LU<T>& f, const Vec<T>& b)
//
// [ ] Item 81: Inverse via LU solves
//       difficulty 3/10, stage 3, needs first: 24, 27
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> inverse(const Matrix<T>& A)
//
// [ ] Item 121: Growth factor of LU
//       difficulty 5/10, stage 3, needs first: 24
//       place: free function in namespace yatrices
//       suggested:
//           T growth_factor(const Matrix<T>& A, const LU<T>& f)
//
// [ ] Item 125: Complete pivoting
//       difficulty 5/10, stage 3, needs first: 23
//       place: free function in namespace yatrices
//       suggested:
//           LUFull<T> lu_factor_complete(Matrix<T> A) (row and column permutations)
//
// [ ] Item 354: Blocked LU
//       difficulty 7/10, stage 11, needs first: 24, 202, 63, 238
//       place: free function in namespace yatrices
//       suggested:
//           LU<T> lu_blocked(Matrix<T> A, std::size_t block)
//

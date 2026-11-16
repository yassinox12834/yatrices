// after/cpp/sparse.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: sparse matrix formats and sparse algorithms.
//
// When you start it: move this file to cpp/include/yatrices/sparse.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/sparse.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 9 item(s).
//
// [ ] Item 62: Sparse matrices (CSR)
//       difficulty 6/10, stage 12, needs first: 6
//       place: new class or type
//       suggested:
//           template <typename T> class CsrMatrix { std::vector<T> values
//           std::vector<std::size_t> col_index, row_ptr
//           }
//           Vec<T> matvec(const CsrMatrix<T>& A, const Vec<T>& x)
//
// [ ] Item 362: COO format
//       difficulty 4/10, stage 12, needs first: 62
//       place: new class or type
//       suggested:
//           template <typename T> class CooMatrix { std::vector<std::size_t> row, col
//           std::vector<T> val
//           }
//           CsrMatrix<T> to_csr(const CooMatrix<T>& A)
//
// [ ] Item 363: CSC format and sparse transpose
//       difficulty 4/10, stage 12, needs first: 62
//       place: new class or type
//       suggested:
//           template <typename T> class CscMatrix
//           CsrMatrix<T> transposed(const CsrMatrix<T>& A)
//
// [ ] Item 365: Sparse triangular solve
//       difficulty 5/10, stage 12, needs first: 62, 21
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> solve_lower(const CsrMatrix<T>& L, const Vec<T>& b)
//
// [ ] Item 370: Threaded sparse mat-vec
//       difficulty 5/10, stage 12, needs first: 62, 356
//       place: internal kernel, not in the public API
//       suggested:
//           void matvec_parallel(const CsrMatrix<T>& A, const Vec<T>& x, Vec<T>& y, ThreadPool& pool)
//
// [ ] Item 364: Sparse addition and product
//       difficulty 6/10, stage 12, needs first: 62
//       place: free function in namespace yatrices
//       suggested:
//           CsrMatrix<T> add(const CsrMatrix<T>& A, const CsrMatrix<T>& B)
//           CsrMatrix<T> matmul(const CsrMatrix<T>& A, const CsrMatrix<T>& B)
//
// [ ] Item 367: Reordering to reduce fill-in
//       difficulty 7/10, stage 12, needs first: 62, 213
//       place: free function in namespace yatrices
//       suggested:
//           std::vector<std::size_t> reverse_cuthill_mckee(const CsrMatrix<T>& A)
//
// [ ] Item 368: Incomplete LU, ILU(0)
//       difficulty 7/10, stage 12, needs first: 62, 24, 128
//       place: free function in namespace yatrices
//       suggested:
//           CsrMatrix<T> ilu0(const CsrMatrix<T>& A)
//
// [ ] Item 400: Sparse Cholesky
//       difficulty 9/10, stage 14, needs first: 365, 367, 31
//       place: free function in namespace yatrices
//       suggested:
//           CsrMatrix<T> cholesky(const CsrMatrix<T>& A, const std::vector<std::size_t>& order)
//

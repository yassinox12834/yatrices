// after/cpp/views.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: non-owning views, slicing, stacking, memory layout.
//
// When you start it: move this file to cpp/include/yatrices/views.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/views.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 4 item(s).
//
// [ ] Item 102: Strides and memory layout
//       difficulty 4/10, stage 0, needs first: 1
//       place: result struct plus free function
//       suggested:
//           struct Layout { std::size_t rows, cols
//           std::ptrdiff_t row_stride, col_stride
//           }
//
// [ ] Item 12: Slicing, rows, columns, submatrices
//       difficulty 4/10, stage 1, needs first: 1
//       place: new class or type
//       suggested:
//           template <typename T> class MatrixView { T* data
//           std::size_t rows, cols
//           std::ptrdiff_t row_stride, col_stride
//           }
//           then Matrix::row(i), Matrix::col(j), Matrix::block(i0, j0, m, n)
//
// [ ] Item 73: Block matrices
//       difficulty 3/10, stage 2, needs first: 7, 12
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> hstack(const Matrix<T>& A, const Matrix<T>& B)
//           vstack(A, B)
//           block_2x2(A, B, C, D)
//
// [ ] Item 209: Direct sum (block diagonal)
//       difficulty 1/10, stage 2, needs first: 73
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> block_diag(const std::vector<Matrix<T>>& blocks)
//

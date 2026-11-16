// after/cpp/products.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: matrix-vector and matrix-matrix products and their variants.
//
// When you start it: move this file to cpp/include/yatrices/products.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/products.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 20 item(s).
//
// [ ] Item 10: Outer product
//       difficulty 1/10, stage 1, needs first: 1
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> outer(const Vec<T>& u, const Vec<T>& v)
//
// [ ] Item 6: Matrix-vector product
//       difficulty 2/10, stage 1, needs first: 4
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> matvec(const Matrix<T>& A, const Vec<T>& x)
//           Vec<T> operator*(const Matrix<T>& A, const Vec<T>& x)
//
// [ ] Item 7: Matrix-matrix product (naive)
//       difficulty 2/10, stage 1, needs first: 6
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> matmul(const Matrix<T>& A, const Matrix<T>& B)
//           Matrix<T> operator*(const Matrix<T>& A, const Matrix<T>& B)
//
// [ ] Item 190: Product with the transpose
//       difficulty 2/10, stage 1, needs first: 6
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> matvec_transposed(const Matrix<T>& A, const Vec<T>& x)
//
// [ ] Item 191: Bilinear and quadratic forms
//       difficulty 2/10, stage 1, needs first: 6
//       place: free function in namespace yatrices
//       suggested:
//           T bilinear(const Vec<T>& x, const Matrix<T>& A, const Vec<T>& y)
//           T quadratic_form(const Matrix<T>& A, const Vec<T>& x)
//
// [ ] Item 13: Matrix power A^k
//       difficulty 3/10, stage 1, needs first: 7
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> matrix_power(const Matrix<T>& A, unsigned k)
//
// [ ] Item 80: Commutator
//       difficulty 1/10, stage 2, needs first: 7
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> commutator(const Matrix<T>& A, const Matrix<T>& B)
//
// [ ] Item 61: Kronecker product
//       difficulty 2/10, stage 2, needs first: 7
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> kron(const Matrix<T>& A, const Matrix<T>& B)
//
// [ ] Item 77: Gram matrix
//       difficulty 2/10, stage 2, needs first: 3, 7
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> gram(const Matrix<T>& A) (compute the upper half, then mirror it)
//
// [ ] Item 201: Rank-1 update A<- A+alpha uv^T (ger)
//       difficulty 2/10, stage 2, needs first: 10
//       place: free function in namespace yatrices
//       suggested:
//           void ger(T alpha, const Vec<T>& u, const Vec<T>& v, Matrix<T>& A)
//
// [ ] Item 207: Trace of a product
//       difficulty 2/10, stage 2, needs first: 7, 9
//       place: free function in namespace yatrices
//       suggested:
//           T trace_of_product(const Matrix<T>& A, const Matrix<T>& B)
//
// [ ] Item 200: General update y<-alpha Ax+beta y (gemv)
//       difficulty 3/10, stage 2, needs first: 6
//       place: free function in namespace yatrices
//       suggested:
//           void gemv(T alpha, const Matrix<T>& A, const Vec<T>& x, T beta, Vec<T>& y)
//
// [ ] Item 202: General update C<-alpha AB+beta C (gemm)
//       difficulty 3/10, stage 2, needs first: 7
//       place: free function in namespace yatrices
//       suggested:
//           void gemm(T alpha, const Matrix<T>& A, const Matrix<T>& B, T beta, Matrix<T>& C)
//
// [ ] Item 204: Symmetric matrix-vector product
//       difficulty 3/10, stage 2, needs first: 6, 14
//       place: free function in namespace yatrices
//       suggested:
//           void symv(const Matrix<T>& A_upper, const Vec<T>& x, Vec<T>& y)
//
// [ ] Item 205: Out-parameters and workspace reuse
//       difficulty 3/10, stage 2, needs first: 103
//       place: free function in namespace yatrices
//       suggested:
//           void matmul_into(Matrix<T>& out, const Matrix<T>& A, const Matrix<T>& B)
//           void matvec_into(Vec<T>& out, const Matrix<T>& A, const Vec<T>& x)
//
// [ ] Item 210: Khatri-Rao product
//       difficulty 3/10, stage 2, needs first: 61
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> khatri_rao(const Matrix<T>& A, const Matrix<T>& B)
//
// [ ] Item 206: Matrix chain order
//       difficulty 4/10, stage 2, needs first: 7
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> matmul_chain(const std::vector<const Matrix<T>*>& factors)
//
// [ ] Item 153: Convolution as a matrix product
//       difficulty 5/10, stage 9, needs first: 7, 75
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> im2col(const Matrix<T>& image, std::size_t kh, std::size_t kw)
//
// [ ] Item 353: Symmetric rank-k update (syrk)
//       difficulty 3/10, stage 11, needs first: 77, 202
//       place: free function in namespace yatrices
//       suggested:
//           void syrk(T alpha, const Matrix<T>& A, T beta, Matrix<T>& C)
//
// [ ] Item 65: Strassen multiplication
//       difficulty 7/10, stage 11, needs first: 7
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> matmul_strassen(const Matrix<T>& A, const Matrix<T>& B, std::size_t cutoff)
//

// after/cpp/special.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: named matrices used as examples and test cases.
//
// When you start it: move this file to cpp/include/yatrices/special.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/special.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 14 item(s).
//
// [ ] Item 74: Vandermonde and Hilbert generators
//       difficulty 1/10, stage 2, needs first: 1
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> vandermonde(const Vec<T>& x, std::size_t cols)
//           Matrix<T> hilbert<T>(std::size_t n)
//
// [ ] Item 113: Hadamard (Sylvester) matrices
//       difficulty 2/10, stage 2, needs first: 61
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> hadamard_matrix<T>(std::size_t n) (n a power of 2)
//
// [ ] Item 215: Pascal matrices
//       difficulty 2/10, stage 2, needs first: 1
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> pascal<T>(std::size_t n)
//
// [ ] Item 216: Householder reflector as a matrix
//       difficulty 2/10, stage 2, needs first: 10, 14
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> householder_matrix(const Vec<T>& v)
//
// [ ] Item 218: Projector onto a line
//       difficulty 2/10, stage 2, needs first: 10, 36
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> projector_onto(const Vec<T>& u)
//
// [ ] Item 75: Toeplitz, circulant, Hankel
//       difficulty 3/10, stage 2, needs first: 1
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> toeplitz(const Vec<T>& col, const Vec<T>& row)
//           circulant(const Vec<T>& c)
//           hankel(const Vec<T>& c)
//
// [ ] Item 217: Givens rotation as a matrix
//       difficulty 2/10, stage 2, needs first: 111
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> givens_matrix<T>(std::size_t n, std::size_t i, std::size_t j, T theta)
//
// [ ] Item 219: Discrete cosine transform matrix
//       difficulty 3/10, stage 2, needs first: 14
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> dct_matrix<T>(std::size_t n)
//
// [ ] Item 98: Finite-difference Laplacian
//       difficulty 3/10, stage 6, needs first: 32, 35
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> laplacian_1d<T>(std::size_t n)
//
// [ ] Item 288: 2D Laplacian by Kronecker sums
//       difficulty 4/10, stage 6, needs first: 61, 98
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> laplacian_2d<T>(std::size_t nx, std::size_t ny)
//
// [ ] Item 295: Wilkinson and Frank matrices
//       difficulty 2/10, stage 7, needs first: 1
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> wilkinson<T>(std::size_t n)
//           Matrix<T> frank<T>(std::size_t n)
//
// [ ] Item 297: Adjacency, degree and incidence matrices
//       difficulty 2/10, stage 7, needs first: 1
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> adjacency<T>(std::size_t n, const std::vector<std::pair<std::size_t, std::size_t>>& edges)
//           Matrix<T> degree_matrix(const Matrix<T>& A)
//
// [ ] Item 97: Graph Laplacian
//       difficulty 5/10, stage 7, needs first: 48
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> graph_laplacian(const Matrix<T>& adjacency)
//
// [ ] Item 87: Companion matrix
//       difficulty 4/10, stage 7, needs first: 50
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> companion(const Vec<T>& coeffs)
//

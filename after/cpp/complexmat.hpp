// after/cpp/complexmat.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: complex vectors and matrices, DFT and FFT.
//
// When you start it: move this file to cpp/include/yatrices/complexmat.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/complexmat.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 6 item(s).
//
// [ ] Item 371: Complex vectors and matrices
//       difficulty 5/10, stage 12, needs first: 69
//       place: new class or type
//       suggested:
//           template <typename T> using CVec = Vec<std::complex<T>>
//           template <typename T> using CMatrix = Matrix<std::complex<T>>
//           CMatrix<T> conj_transposed(const CMatrix<T>& A)
//
// [ ] Item 372: Hermitian and unitary tests
//       difficulty 2/10, stage 12, needs first: 371
//       place: free function in namespace yatrices
//       suggested:
//           bool is_hermitian(const CMatrix<T>& A, T tol)
//           bool is_unitary(const CMatrix<T>& U, T tol)
//
// [ ] Item 114: DFT matrix
//       difficulty 4/10, stage 12, needs first: 7, 371
//       place: free function in namespace yatrices
//       suggested:
//           CMatrix<T> dft_matrix<T>(std::size_t n)
//
// [ ] Item 374: Hermitian eigenproblem
//       difficulty 6/10, stage 12, needs first: 371, 48
//       place: free function in namespace yatrices
//       suggested:
//           SymEigen<T> eigh_hermitian(const CMatrix<T>& A)
//
// [ ] Item 375: Fast Fourier transform
//       difficulty 6/10, stage 12, needs first: 114
//       place: free function in namespace yatrices
//       suggested:
//           void fft(std::complex<T>* x, std::size_t n, bool inverse)
//
// [ ] Item 376: Circulant solve by FFT
//       difficulty 6/10, stage 12, needs first: 115, 375
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> solve_circulant(const Vec<T>& first_column, const Vec<T>& b)
//

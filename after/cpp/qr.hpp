// after/cpp/qr.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: Gram-Schmidt, Householder, Givens and QR decomposition.
//
// When you start it: move this file to cpp/include/yatrices/qr.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/qr.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 11 item(s).
//
// [ ] Item 37: Gram-Schmidt (classical and modified)
//       difficulty 5/10, stage 5, needs first: 36
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> gram_schmidt_classical(const Matrix<T>& A)
//           Matrix<T> gram_schmidt_modified(const Matrix<T>& A)
//
// [ ] Item 38: QR via Gram-Schmidt
//       difficulty 5/10, stage 5, needs first: 37
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct QR { Matrix<T> Q, R
//           }
//           QR<T> qr_gram_schmidt(const Matrix<T>& A)
//
// [ ] Item 133: Orthogonal projection matrix
//       difficulty 3/10, stage 5, needs first: 38
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> projector(const Matrix<T>& Q)
//
// [ ] Item 271: Gram-Schmidt with reorthogonalisation
//       difficulty 5/10, stage 5, needs first: 37
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> gram_schmidt_twice(const Matrix<T>& A)
//
// [ ] Item 40: Givens rotations
//       difficulty 6/10, stage 5, needs first: 7
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct Givens { T c, s
//           }
//           Givens<T> make_givens(T a, T b)
//           void apply_left(Matrix<T>& A, std::size_t i, std::size_t k, Givens<T> g)
//
// [ ] Item 267: Stable Givens construction
//       difficulty 4/10, stage 5, needs first: 40, 234
//       place: free function in namespace yatrices
//       suggested:
//           inside make_givens(T a, T b): r = hypot_safe(a, b)
//           c = a / r
//           s = b / r, with the r = 0 case handled
//
// [ ] Item 39: QR via Householder reflections
//       difficulty 7/10, stage 5, needs first: 38
//       place: free function in namespace yatrices
//       suggested:
//           QR<T> qr_householder(Matrix<T> A)
//
// [ ] Item 268: Stable Householder vector
//       difficulty 4/10, stage 5, needs first: 39
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct Reflector { Vec<T> v
//           T tau
//           }
//           Reflector<T> make_reflector(const Vec<T>& x)
//
// [ ] Item 270: Compact QR storage
//       difficulty 5/10, stage 5, needs first: 39
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct QRCompact { Matrix<T> qr
//           Vec<T> tau
//           }
//           void apply_qt(const QRCompact<T>& f, Vec<T>& b)
//           Matrix<T> thin_q(const QRCompact<T>& f)
//
// [ ] Item 277: Orthogonal complement
//       difficulty 5/10, stage 5, needs first: 270
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> orth_complement(const Matrix<T>& Q)
//
// [ ] Item 134: Rank-revealing QR
//       difficulty 7/10, stage 5, needs first: 29, 39
//       place: free function in namespace yatrices
//       suggested:
//           QRP<T> qr_pivoted(Matrix<T> A)
//           std::size_t numerical_rank(const QRP<T>& f, T tol)
//

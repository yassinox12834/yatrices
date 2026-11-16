// after/cpp/stats.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: covariance, distances, kernels, multivariate normal.
//
// When you start it: move this file to cpp/include/yatrices/stats.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/stats.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 7 item(s).
//
// [ ] Item 94: Covariance matrix
//       difficulty 2/10, stage 4, needs first: 3, 7
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> covariance(const Matrix<T>& X) (one sample per row)
//
// [ ] Item 256: Correlation matrix
//       difficulty 2/10, stage 4, needs first: 94
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> correlation(const Matrix<T>& X)
//
// [ ] Item 257: Pairwise distance matrix
//       difficulty 3/10, stage 4, needs first: 7, 160
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> pairwise_sq_dist(const Matrix<T>& X, const Matrix<T>& Y)
//
// [ ] Item 258: Multivariate normal: sampling and log-density
//       difficulty 4/10, stage 4, needs first: 31, 197
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> sample_mvn(const Vec<T>& mu, const Matrix<T>& L, Rng& rng)
//           T logpdf_mvn(const Vec<T>& x, const Vec<T>& mu, const Matrix<T>& L)
//
// [ ] Item 95: Mahalanobis distance
//       difficulty 4/10, stage 8, needs first: 31, 94
//       place: free function in namespace yatrices
//       suggested:
//           T mahalanobis(const Vec<T>& x, const Vec<T>& mu, const Matrix<T>& L)
//
// [ ] Item 96: Whitening
//       difficulty 5/10, stage 8, needs first: 60, 94
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> whiten(const Matrix<T>& X)
//
// [ ] Item 323: Kernel matrices
//       difficulty 3/10, stage 9, needs first: 257, 78
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> rbf_kernel(const Matrix<T>& X, const Matrix<T>& Y, T gamma)
//           Matrix<T> polynomial_kernel(const Matrix<T>& X, const Matrix<T>& Y, int degree, T c)
//

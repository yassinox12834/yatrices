// after/cpp/lstsq.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: least squares and regression.
//
// When you start it: move this file to cpp/include/yatrices/lstsq.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/lstsq.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 16 item(s).
//
// [ ] Item 41: Least squares via normal equation
//       difficulty 4/10, stage 4, needs first: 24, 31
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> lstsq_normal(const Matrix<T>& X, const Vec<T>& y)
//
// [ ] Item 255: R^2 and sums of squares
//       difficulty 2/10, stage 4, needs first: 41
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct FitReport { T r2, rss, tss
//           Vec<T> residuals
//           }
//           FitReport<T> fit_report(const Matrix<T>& X, const Vec<T>& y, const Vec<T>& beta)
//
// [ ] Item 43: Ridge regression
//       difficulty 3/10, stage 4, needs first: 41
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> ridge(const Matrix<T>& X, const Vec<T>& y, T lambda)
//
// [ ] Item 132: Weighted least squares
//       difficulty 3/10, stage 4, needs first: 41
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> lstsq_weighted(const Matrix<T>& X, const Vec<T>& y, const Vec<T>& w)
//
// [ ] Item 254: Minimum-norm solution
//       difficulty 4/10, stage 4, needs first: 31
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> solve_min_norm(const Matrix<T>& A, const Vec<T>& b)
//
// [ ] Item 93: Hat matrix
//       difficulty 3/10, stage 5, needs first: 36, 41
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> hat_matrix(const Matrix<T>& X)
//
// [ ] Item 275: Leverage scores
//       difficulty 4/10, stage 5, needs first: 93, 270
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> leverage(const Matrix<T>& X)
//
// [ ] Item 42: Least squares via QR
//       difficulty 6/10, stage 5, needs first: 39
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> lstsq_qr(const Matrix<T>& X, const Vec<T>& y)
//
// [ ] Item 273: Fitting in an orthogonal polynomial basis
//       difficulty 5/10, stage 5, needs first: 272
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> chebyshev_vandermonde(const Vec<T>& x, std::size_t degree)
//
// [ ] Item 274: General Tikhonov regularisation
//       difficulty 5/10, stage 5, needs first: 42, 43
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> tikhonov(const Matrix<T>& X, const Vec<T>& y, const Matrix<T>& L, T lambda)
//
// [ ] Item 276: Standard errors of coefficients
//       difficulty 5/10, stage 5, needs first: 42, 239
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> standard_errors(const Matrix<T>& X, const Vec<T>& y, const Vec<T>& beta)
//
// [ ] Item 290: Recursive least squares
//       difficulty 6/10, stage 6, needs first: 41, 84
//       place: new class or type
//       suggested:
//           template <typename T> class RecursiveLeastSquares { Matrix<T> P
//           Vec<T> beta
//           public: void update(const Vec<T>& x, T y)
//           }
//
// [ ] Item 291: Equality-constrained least squares
//       difficulty 7/10, stage 6, needs first: 42, 30
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> lstsq_constrained(const Matrix<T>& X, const Vec<T>& y, const Matrix<T>& C, const Vec<T>& d)
//
// [ ] Item 316: Least squares via SVD
//       difficulty 5/10, stage 8, needs first: 58
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> lstsq_svd(const Matrix<T>& X, const Vec<T>& y, T rcond)
//
// [ ] Item 150: Total least squares
//       difficulty 7/10, stage 8, needs first: 56
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> lstsq_total(const Matrix<T>& X, const Vec<T>& y)
//
// [ ] Item 341: Non-negative least squares
//       difficulty 8/10, stage 9, needs first: 42
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> nnls(const Matrix<T>& X, const Vec<T>& y, int max_iter)
//

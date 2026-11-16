// after/cpp/eigen.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: eigenvalues and eigenvectors.
//
// When you start it: move this file to cpp/include/yatrices/eigen.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/eigen.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 23 item(s).
//
// [ ] Item 47: Rayleigh quotient
//       difficulty 2/10, stage 7, needs first: 4
//       place: free function in namespace yatrices
//       suggested:
//           T rayleigh_quotient(const Matrix<T>& A, const Vec<T>& x)
//
// [ ] Item 293: Closed-form eigenvalues of a 2x2
//       difficulty 2/10, stage 7, needs first: 9
//       place: free function in namespace yatrices
//       suggested:
//           std::pair<T, T> eigvals_sym2(T a, T b, T d)
//
// [ ] Item 44: Characteristic polynomial
//       difficulty 3/10, stage 7, needs first: 25
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> char_poly(const Matrix<T>& A)
//
// [ ] Item 45: Power iteration
//       difficulty 3/10, stage 7, needs first: 6, 15
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct EigenPair { T value
//           Vec<T> vector
//           int iters
//           }
//           EigenPair<T> power_iteration(const Matrix<T>& A, T tol, int max_iter)
//
// [ ] Item 88: Gershgorin discs
//       difficulty 3/10, stage 7, needs first: 1
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct Disc { T centre, radius
//           }
//           std::vector<Disc<T>> gershgorin(const Matrix<T>& A)
//
// [ ] Item 138: Spectral radius and Jacobi convergence
//       difficulty 3/10, stage 7, needs first: 33, 45
//       place: free function in namespace yatrices
//       suggested:
//           T spectral_radius(const Matrix<T>& M, T tol, int max_iter)
//           Matrix<T> jacobi_iteration_matrix(const Matrix<T>& A)
//
// [ ] Item 139: Deflation (Hotelling)
//       difficulty 4/10, stage 7, needs first: 45
//       place: free function in namespace yatrices
//       suggested:
//           void deflate(Matrix<T>& A, const EigenPair<T>& p)
//
// [ ] Item 46: Inverse and shifted iteration
//       difficulty 5/10, stage 7, needs first: 45, 24
//       place: free function in namespace yatrices
//       suggested:
//           EigenPair<T> inverse_iteration(const Matrix<T>& A, T shift, T tol, int max_iter)
//
// [ ] Item 48: Jacobi eigenvalue method
//       difficulty 6/10, stage 7, needs first: 40
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct SymEigen { Vec<T> values
//           Matrix<T> vectors
//           }
//           SymEigen<T> eigh_jacobi(Matrix<T> A, T tol)
//
// [ ] Item 140: Rayleigh quotient iteration
//       difficulty 6/10, stage 7, needs first: 46, 47
//       place: free function in namespace yatrices
//       suggested:
//           EigenPair<T> rayleigh_iteration(const Matrix<T>& A, Vec<T> x0, T tol, int max_iter)
//
// [ ] Item 141: Subspace iteration
//       difficulty 6/10, stage 7, needs first: 38, 45
//       place: free function in namespace yatrices
//       suggested:
//           SymEigen<T> subspace_iteration(const Matrix<T>& A, std::size_t k, T tol, int max_iter)
//
// [ ] Item 142: Householder tridiagonalisation
//       difficulty 6/10, stage 7, needs first: 39
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct Tridiagonal { Vec<T> diag, off
//           Matrix<T> Q
//           }
//           Tridiagonal<T> tridiagonalise(Matrix<T> A)
//
// [ ] Item 299: Sturm sequences and bisection
//       difficulty 6/10, stage 7, needs first: 142
//       place: free function in namespace yatrices
//       suggested:
//           std::size_t count_below(const Tridiagonal<T>& t, T x)
//           T kth_eigenvalue(const Tridiagonal<T>& t, std::size_t k)
//
// [ ] Item 301: Eigenvectors from known eigenvalues
//       difficulty 5/10, stage 7, needs first: 46, 299
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> eigenvector_for(const Matrix<T>& A, T eigenvalue)
//
// [ ] Item 302: Generalised symmetric eigenproblem
//       difficulty 6/10, stage 7, needs first: 31, 48
//       place: free function in namespace yatrices
//       suggested:
//           SymEigen<T> eigh_generalised(const Matrix<T>& A, const Matrix<T>& B)
//
// [ ] Item 49: Hessenberg reduction
//       difficulty 7/10, stage 7, needs first: 39
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> hessenberg(Matrix<T> A, Matrix<T>* Q = nullptr)
//
// [ ] Item 50: QR algorithm (basic)
//       difficulty 7/10, stage 7, needs first: 39
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> eigvals_qr(Matrix<T> A, T tol, int max_iter)
//
// [ ] Item 53: Diagonalisation
//       difficulty 4/10, stage 7, needs first: 27, 50
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct Diagonalisation { Matrix<T> P
//           Vec<T> d
//           }
//           Diagonalisation<T> diagonalise(const Matrix<T>& A)
//
// [ ] Item 303: Balancing
//       difficulty 6/10, stage 7, needs first: 50
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> balance(Matrix<T> A, Vec<T>* scale = nullptr)
//
// [ ] Item 300: Symmetric QR with Wilkinson shift
//       difficulty 8/10, stage 7, needs first: 142, 50
//       place: free function in namespace yatrices
//       suggested:
//           SymEigen<T> eigh(Matrix<T> A)
//
// [ ] Item 52: Complex eigenvalues
//       difficulty 8/10, stage 14, needs first: 50
//       place: free function in namespace yatrices
//       suggested:
//           std::vector<std::complex<T>> eigvals(const Matrix<T>& A)
//
// [ ] Item 51: QR algorithm with shifts and deflation
//       difficulty 9/10, stage 14, needs first: 49, 50
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> eigvals_shifted_qr(Matrix<T> A)
//
// [ ] Item 67: Schur decomposition
//       difficulty 9/10, stage 14, needs first: 51
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct Schur { Matrix<T> Q, U
//           }
//           Schur<T> schur(Matrix<T> A)
//

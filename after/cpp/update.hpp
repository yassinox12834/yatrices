// after/cpp/update.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: cheap updates of an inverse or a factorisation after a small change.
//
// When you start it: move this file to cpp/include/yatrices/update.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/update.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 4 item(s).
//
// [ ] Item 84: Sherman-Morrison
//       difficulty 4/10, stage 6, needs first: 10, 27
//       place: free function in namespace yatrices
//       suggested:
//           void sherman_morrison(Matrix<T>& A_inv, const Vec<T>& u, const Vec<T>& v)
//
// [ ] Item 85: Woodbury identity
//       difficulty 6/10, stage 6, needs first: 84
//       place: free function in namespace yatrices
//       suggested:
//           void woodbury(Matrix<T>& A_inv, const Matrix<T>& U, const Matrix<T>& V)
//
// [ ] Item 283: Cholesky rank-1 update and downdate
//       difficulty 6/10, stage 6, needs first: 31, 40
//       place: free function in namespace yatrices
//       suggested:
//           void cholesky_update(Matrix<T>& L, Vec<T> x)
//           void cholesky_downdate(Matrix<T>& L, Vec<T> x)
//
// [ ] Item 289: Updating a QR factorisation
//       difficulty 7/10, stage 6, needs first: 40, 38
//       place: free function in namespace yatrices
//       suggested:
//           void qr_insert_row(QR<T>& f, const Vec<T>& row)
//           void qr_delete_column(QR<T>& f, std::size_t j)
//
